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
uint32 TempList[1024]; 
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

uint8    CurrAttk, CurrFeat;
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
  template_entry(), event_entry(), event_desc(), prototype(), s_prototype(), 
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
      param_list(BSysFunc*,long,VBlock*,int*), sep_expr(hCode *hc);

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



# line 145 "yygram.cpp"
#include "yygram.h"

int YYSTART ()
{
   switch(yyselect()) {
   case 448: {
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
#line 185 "grammar.acc"
 currMap = 0; MemFuncID = 0; MemVarID = 0; 
# line 164 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 449: {
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
#line 189 "grammar.acc"
 theModule->Name = name; 
# line 193 "yygram.cpp"
      } break;
   case 3: {
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 190 "grammar.acc"
 theModule->Slot = n; 
# line 201 "yygram.cpp"
      } break;
   case 4: {
      get_lexval();
      get_lexval();
      fname = yylval;
      get_lexval();
#line 191 "grammar.acc"
 theModule->FName = fname; 
# line 210 "yygram.cpp"
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
      prototype();
      } break;
   case 21: {
      s_prototype();
      } break;
   case 22: {
      sg_prototype();
      } break;
   case 23: {
      g_declaration();
      } break;
   case 24: {
      s_declaration();
      } break;
   case 25: {
      s_object_decl();
      } break;
   case 26: {
      get_lexval();
      switch (yyselect()) {
      case 450: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      cexpr(&n);
#line 213 "grammar.acc"
 CompilerFlags |= n; 
# line 286 "yygram.cpp"
      } break;
   case 27: {
      get_lexval();
      get_lexval();
      nam = yylval;
      get_lexval();
      switch (yyselect()) {
      case 451: {
         get_lexval();
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      text = yylval;
      switch (yyselect()) {
      case 452: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 215 "grammar.acc"
 theRes = theTxt; theTxt->Name = nam; theTxt->Desc = text; theTxt++; 
# line 311 "yygram.cpp"
      } break;
   }
return 0;
}

int res_ref (   rID *res)
{
   YYSTYPE lit;
   YYSTYPE n;
   switch(yyselect()) {
   case 28: {
      get_lexval();
      get_lexval();
      lit = yylval;
#line 219 "grammar.acc"
 PurgeStrings();
                         *res = FIND(theModule->GetText(lit)); 
                         if (!*res) {
                           if (Unresolved.GetLength())
                             Unresolved += ", ";
                           Unresolved += Capitalize(theModule->GetText(lit),true);
                           *res = FIND("unimplemented");
                           }
                        
# line 335 "yygram.cpp"
      } break;
   case 29: {
      get_lexval();
      cexpr3(&n);
#line 228 "grammar.acc"
 *res = n; 
# line 342 "yygram.cpp"
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
   case 30: {
      cexpr(&x);
      get_lexval();
      cexpr2(&y);
#line 232 "grammar.acc"
 *val = x+y; 
# line 359 "yygram.cpp"
      } break;
   case 31: {
      cexpr(&x);
      get_lexval();
      cexpr2(&y);
#line 233 "grammar.acc"
 *val = x-y; 
# line 367 "yygram.cpp"
      } break;
   case 32: {
      cexpr2(&val2);
#line 234 "grammar.acc"
 *val = val2; 
# line 373 "yygram.cpp"
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
   case 33: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 238 "grammar.acc"
 *val = x * y; 
# line 390 "yygram.cpp"
      } break;
   case 34: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 239 "grammar.acc"
 *val = x / y; 
# line 398 "yygram.cpp"
      } break;
   case 35: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 240 "grammar.acc"
 *val = x % y; 
# line 406 "yygram.cpp"
      } break;
   case 36: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 241 "grammar.acc"
 *val = x | y; 
# line 414 "yygram.cpp"
      } break;
   case 37: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 242 "grammar.acc"
 *val = x & y; 
# line 422 "yygram.cpp"
      } break;
   case 38: {
      cexpr3(&val2);
#line 244 "grammar.acc"
 *val = val2; 
# line 428 "yygram.cpp"
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
   case 39: {
      get_lexval();
      cexpr4(&val2);
#line 248 "grammar.acc"
 *val = val2; 
# line 444 "yygram.cpp"
      } break;
   case 40: {
      get_lexval();
      cexpr4(&n);
#line 249 "grammar.acc"
 *val = -n; 
# line 451 "yygram.cpp"
      } break;
   case 41: {
      cexpr4(&val3);
#line 250 "grammar.acc"
 *val = val3; 
# line 457 "yygram.cpp"
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
   case 42: {
      get_lexval();
      cexpr(&val2);
      get_lexval();
#line 254 "grammar.acc"
 * val = val2; 
# line 474 "yygram.cpp"
      } break;
   case 43: {
      get_lexval();
      val3 = yylval;
#line 255 "grammar.acc"
 *val = val3; 
# line 481 "yygram.cpp"
      } break;
   case 44: {
      get_lexval();
#line 256 "grammar.acc"
 *val = 1; 
# line 487 "yygram.cpp"
      } break;
   case 45: {
      get_lexval();
#line 257 "grammar.acc"
 *val = 0; 
# line 493 "yygram.cpp"
      } break;
   case 46: {
      get_lexval();
      val4 = yylval;
#line 258 "grammar.acc"
 * val = val4; 
# line 500 "yygram.cpp"
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
   case 47: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
      get_lexval();
      cexpr4(&bon);
#line 263 "grammar.acc"
 d->Number = num; d->Sides = sid; d->Bonus = bon; 
# line 520 "yygram.cpp"
      } break;
   case 48: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
      get_lexval();
      get_lexval();
      cexpr4(&bon);
#line 265 "grammar.acc"
 d->Number = num; d->Sides = sid; d->Bonus = -bon; 
# line 531 "yygram.cpp"
      } break;
   case 49: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
#line 267 "grammar.acc"
 d->Number = num; d->Sides = sid; d->Bonus = 0; 
# line 539 "yygram.cpp"
      } break;
   case 50: {
      cexpr3(&x);
#line 269 "grammar.acc"
 d->Number = 0; d->Sides = 0; d->Bonus = x; 
# line 545 "yygram.cpp"
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
   case 51: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
      get_lexval();
      cexpr4(&bon);
#line 274 "grammar.acc"
 d->Number = num; d->Sides = sid; d->Bonus = bon; 
# line 564 "yygram.cpp"
      } break;
   case 52: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
#line 276 "grammar.acc"
 d->Number = num; d->Sides = sid; d->Bonus = 0; 
# line 572 "yygram.cpp"
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
   case 53: {
#line 281 "grammar.acc"
 *chance = *iID = *eID = *spec = *qual = *fl = *cond = 0; 
    int16 q; q=0;
    amt->Set(1,1,0); 
# line 594 "yygram.cpp"
      switch (yyselect()) {
      case 453: {
         get_lexval();
         get_lexval();
         sep_expr(&hc);
         get_lexval();
#line 284 "grammar.acc"
 *cond = hc; 
# line 603 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 454: {
         get_lexval();
         n = yylval;
         get_lexval();
#line 285 "grammar.acc"
 *chance = n; 
# line 614 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 455: {
         dice_only(&dv);
#line 286 "grammar.acc"
 *amt = yyDice(dv); 
# line 623 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 456: {
         get_lexval();
#line 287 "grammar.acc"
 *fl |= GF_CURSED; 
# line 632 "yygram.cpp"
         } break;
      case 457: {
         get_lexval();
#line 288 "grammar.acc"
 *fl |= GF_BLESSED; 
# line 638 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 458: {
         res_ref(&rr);
#line 289 "grammar.acc"
 *iID = rr; 
# line 647 "yygram.cpp"
         } break;
      case 459: {
         get_lexval();
         cexpr3(&n);
#line 290 "grammar.acc"
 *iID = n; *fl |= GF_RANDOM_ITEM; 
# line 654 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 460: {
         get_lexval();
         get_lexval();
         n = yylval;
#line 291 "grammar.acc"
 *spec = n; 
# line 665 "yygram.cpp"
         } break;
      case 461: {
         get_lexval();
         get_lexval();
         n = yylval;
#line 291 "grammar.acc"
 *spec = -n; 
# line 673 "yygram.cpp"
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 464: {
            get_lexval();
            get_lexval();
            while (1) {
               switch (yyselect()) {
               case 463: {
                  cexpr(&wq);
                  switch (yyselect()) {
                  case 462: {
                     get_lexval();
                     } break;
                  default: ;
                  }
#line 292 "grammar.acc"
 if (q >= 4)
                                   yyerror("Max 4 qualities in gear lists!");
                                else
                                  ((uint8*)qual)[q++] = wq;
# line 697 "yygram.cpp"
                  } break;
               default: goto yy3;
               }
            }
            yy3: ;
            get_lexval();
            } break;
         case 465: {
            get_lexval();
            res_ref(&rr);
#line 296 "grammar.acc"
 *eID = rr; 
# line 710 "yygram.cpp"
            } break;
         case 466: {
            get_lexval();
            get_lexval();
            cexpr3(&a);
            get_lexval();
            cexpr3(&b);
#line 298 "grammar.acc"
 *eID = a + b*256; *fl |= GF_RANDOM_EFFECT; 
# line 720 "yygram.cpp"
            } break;
         case 467: {
            get_lexval();
            get_lexval();
            cexpr3(&b);
#line 300 "grammar.acc"
 *eID = b*256; *fl |= GF_RANDOM_EFFECT; 
# line 728 "yygram.cpp"
            } break;
         case 468: {
            get_lexval();
            get_lexval();
#line 302 "grammar.acc"
 *eID = 0; *fl |= GF_RANDOM_EFFECT; 
# line 735 "yygram.cpp"
            } break;
         case 469: {
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
   case 54: {
#line 308 "grammar.acc"
 int16 is_else = 0; 
# line 767 "yygram.cpp"
      get_lexval();
      switch (yyselect()) {
      case 470: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      while (1) {
         switch (yyselect()) {
         case 473: {
            gear_entry(&chance, &amt, &iID, &eID, &spec, &qual, &fl, &cond);
#line 311 "grammar.acc"
 theRes->AddEquip(chance,amt,iID,eID,spec,(uint8*)(&qual),fl | is_else,cond); 
# line 781 "yygram.cpp"
            switch (yyselect()) {
            case 471: {
               get_lexval();
#line 312 "grammar.acc"
 is_else = 0; 
# line 787 "yygram.cpp"
               } break;
            case 472: {
               get_lexval();
#line 312 "grammar.acc"
 is_else = GF_ELSE; 
# line 793 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy4;
         }
      }
      yy4: ;
      gear_entry(&chance, &amt, &iID, &eID, &spec, &qual, &fl, &cond);
#line 314 "grammar.acc"
 theRes->AddEquip(chance,amt,iID,eID,spec,(uint8*)(&qual),fl | is_else,cond); 
# line 805 "yygram.cpp"
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
   case 55: {
      switch (yyselect()) {
      case 474: {
         get_lexval();
         } break;
      case 475: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 476: {
         get_lexval();
         } break;
      default: ;
      }
#line 318 "grammar.acc"
 *img = 15 << 8; 
# line 835 "yygram.cpp"
      switch (yyselect()) {
      case 477: {
         color(&col);
#line 319 "grammar.acc"
 *img = col << 8; 
# line 841 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 478: {
         get_lexval();
         n = yylval;
#line 320 "grammar.acc"
 *img |= n; 
# line 851 "yygram.cpp"
         } break;
      case 479: {
         get_lexval();
         n = yylval;
#line 321 "grammar.acc"
 *img |= n; 
# line 858 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 480: {
         get_lexval();
         color(&back);
#line 323 "grammar.acc"
 *img |= back << 12; 
# line 868 "yygram.cpp"
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
   case 56: {
#line 327 "grammar.acc"
 *img = 15 << 8; 
# line 886 "yygram.cpp"
      switch (yyselect()) {
      case 481: {
         color(&col);
#line 328 "grammar.acc"
 *img = col << 8; 
# line 892 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 482: {
         get_lexval();
         n = yylval;
#line 329 "grammar.acc"
 *img |= n; 
# line 902 "yygram.cpp"
         } break;
      case 483: {
         get_lexval();
         n = yylval;
#line 330 "grammar.acc"
 *img |= n; 
# line 909 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 484: {
         get_lexval();
         color(&back);
#line 331 "grammar.acc"
 *img |= back << 12; 
# line 919 "yygram.cpp"
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
   case 57: {
      get_lexval();
      get_lexval();
      event_entry();
      while (1) {
         switch (yyselect()) {
         case 485: {
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
   case 58: {
#line 339 "grammar.acc"
 int16 EvList[32]; int16 i, cEvent = 0; hCode hc; 
# line 958 "yygram.cpp"
      cexpr(&n);
#line 340 "grammar.acc"
 EvList[cEvent++] = n; theEvent = n; 
# line 962 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 486: {
            get_lexval();
            cexpr(&n);
#line 341 "grammar.acc"
 EvList[cEvent++] = n; 
# line 970 "yygram.cpp"
            } break;
         default: goto yy6;
         }
      }
      yy6: ;
      switch (yyselect()) {
      case 488: {
         get_lexval();
         lv = yylval;
#line 342 "grammar.acc"
 for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(-EvList[i],lv); 
# line 983 "yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 487: {
               get_lexval();
               get_lexval();
               lv = yylval;
#line 344 "grammar.acc"
 for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(-EvList[i],lv); 
# line 993 "yygram.cpp"
               } break;
            default: goto yy7;
            }
         }
         yy7: ;
         } break;
      case 489: {
         routine(&vc);
#line 349 "grammar.acc"
 hCode hc = theCodeSeg.Add(&vc);
                            theCodeSeg.Generate(HALT);
                            for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(EvList[i],hc);
                            theEvent = 0; 
# line 1008 "yygram.cpp"
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
   case 59: {
#line 358 "grammar.acc"
 theEvent = EV_CONDITION; 
# line 1023 "yygram.cpp"
      expr(&ex);
#line 360 "grammar.acc"
 *hc = theCodeSeg.Add(ex.Code);
      theCodeSeg.Generate(RET,ex.Storage,ex.Value); 
# line 1028 "yygram.cpp"
#line 362 "grammar.acc"
 theEvent = 0; 
# line 1031 "yygram.cpp"
      } break;
   }
return 0;
}

int mval (   MVal *mv)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 60: {
#line 366 "grammar.acc"
 mv->BType = mv->Bound = 0; 
# line 1043 "yygram.cpp"
      switch (yyselect()) {
      case 491: {
         switch (yyselect()) {
         case 490: {
            get_lexval();
            } break;
         default: ;
         }
         cexpr4(&n);
#line 367 "grammar.acc"
 mv->Value = n;  mv->VType = MVAL_ADD; 
# line 1055 "yygram.cpp"
         } break;
      case 492: {
         get_lexval();
         cexpr4(&n);
#line 368 "grammar.acc"
 mv->Value = -n; mv->VType = MVAL_ADD; 
# line 1062 "yygram.cpp"
         } break;
      case 493: {
         get_lexval();
         cexpr4(&n);
#line 369 "grammar.acc"
 mv->Value = n;  mv->VType = MVAL_SET; 
# line 1069 "yygram.cpp"
         } break;
      case 494: {
         cexpr3(&n);
         get_lexval();
#line 370 "grammar.acc"
 mv->Value = n;  mv->VType = MVAL_PERCENT; 
# line 1076 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 498: {
         get_lexval();
         switch (yyselect()) {
         case 495: {
            get_lexval();
            cexpr4(&n);
#line 372 "grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_MIN; 
# line 1089 "yygram.cpp"
            } break;
         case 496: {
            get_lexval();
            cexpr4(&n);
#line 373 "grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_MAX; 
# line 1096 "yygram.cpp"
            } break;
         case 497: {
            get_lexval();
            cexpr4(&n);
#line 374 "grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_NEAR; 
# line 1103 "yygram.cpp"
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
   case 61: {
      get_lexval();
#line 379 "grammar.acc"
 theMon->Attr[0] = theMon->Attr[1] =
                       theMon->Attr[2] = theMon->Attr[3] =
                         theMon->Attr[4] = theMon->Attr[5] =
                         0; CurrAttk = 0; CurrFeat = 0; theRes = theMon; 
# line 1129 "yygram.cpp"
      get_lexval();
      name = yylval;
#line 383 "grammar.acc"
 theMon->Name = name; 
# line 1134 "yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 384 "grammar.acc"
 theMon->MType[0] = n; 
# line 1139 "yygram.cpp"
      switch (yyselect()) {
      case 500: {
         get_lexval();
         cexpr3(&n2);
#line 385 "grammar.acc"
 theMon->MType[1] = n2; 
# line 1146 "yygram.cpp"
         switch (yyselect()) {
         case 499: {
            get_lexval();
            cexpr3(&n3);
#line 386 "grammar.acc"
 theMon->MType[2] = n3; 
# line 1153 "yygram.cpp"
            } break;
         default: ;
         }
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 501: {
            mon_entry();
            } break;
         default: goto yy8;
         }
      }
      yy8: ;
      get_lexval();
#line 388 "grammar.acc"
 theMon++; 
# line 1173 "yygram.cpp"
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
   case 62: {
      get_lexval();
      switch (yyselect()) {
      case 502: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 391 "grammar.acc"
 theMon->Desc = lv; 
# line 1206 "yygram.cpp"
      } break;
   case 63: {
      get_lexval();
      switch (yyselect()) {
      case 503: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 392 "grammar.acc"
 theMon->Depth = n; 
# line 1220 "yygram.cpp"
      } break;
   case 64: {
      switch (yyselect()) {
      case 504: {
         get_lexval();
         } break;
      case 505: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 506: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 393 "grammar.acc"
 theMon->CR = n; 
# line 1242 "yygram.cpp"
      } break;
   case 65: {
      switch (yyselect()) {
      case 507: {
         get_lexval();
         } break;
      case 508: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 509: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n2);
      get_lexval();
      cexpr3(&n1);
      get_lexval();
#line 395 "grammar.acc"
 if (n2 != 1 || n1 > 8 || n1 < 2)
                                        yyerror("Fractional CR error!");
                                      theMon->CR = (-n1) + 2; 
# line 1268 "yygram.cpp"
      } break;
   case 66: {
      get_lexval();
      switch (yyselect()) {
      case 510: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 398 "grammar.acc"
 theMon->HitDice = n; 
# line 1282 "yygram.cpp"
      } break;
   case 67: {
      get_lexval();
      switch (yyselect()) {
      case 511: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 399 "grammar.acc"
 theMon->HitDice = n; 
                                       theMon->SetFlag(M_FIXED_HP); 
# line 1297 "yygram.cpp"
      } break;
   case 68: {
      switch (yyselect()) {
      case 512: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      switch (yyselect()) {
      case 513: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 401 "grammar.acc"
 theMon->Hit = n; 
# line 1317 "yygram.cpp"
      } break;
   case 69: {
      glyph_entry(&img);
#line 402 "grammar.acc"
 theMon->Image = img; 
# line 1323 "yygram.cpp"
      } break;
   case 70: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      get_lexval();
#line 404 "grammar.acc"
 if (!theMon->GainStati(st)) yyerror("too many Stati in monster"); 
# line 1333 "yygram.cpp"
      } break;
   case 71: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      get_lexval();
#line 406 "grammar.acc"
 if (!theMon->GainStati(st,stV)) yyerror("too many Stati in monster"); 
# line 1345 "yygram.cpp"
      } break;
   case 72: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      cexpr3(&stM);
      get_lexval();
      get_lexval();
#line 408 "grammar.acc"
 if (!theMon->GainStati(st,stV,stM)) 
        yyerror("too many Stati in monster"); 
# line 1360 "yygram.cpp"
      } break;
   case 73: {
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
#line 411 "grammar.acc"
 if (!theMon->GainStati(st,stV,stM,rr)) 
        yyerror("too many Stati in monster"); 
# line 1377 "yygram.cpp"
      } break;
   case 74: {
      get_lexval();
      switch (yyselect()) {
      case 514: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 516: {
            cexpr3(&res);
#line 414 "grammar.acc"
 theMon->Res |= res; 
# line 1393 "yygram.cpp"
            switch (yyselect()) {
            case 515: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy9;
         }
      }
      yy9: ;
      get_lexval();
      } break;
   case 75: {
      get_lexval();
      switch (yyselect()) {
      case 517: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 519: {
            cexpr3(&imm);
#line 415 "grammar.acc"
 theMon->Imm |= imm; 
# line 1421 "yygram.cpp"
            switch (yyselect()) {
            case 518: {
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
   case 76: {
      get_lexval();
      switch (yyselect()) {
      case 520: {
         get_lexval();
         } break;
      default: ;
      }
      attack_entry();
      while (1) {
         switch (yyselect()) {
         case 521: {
            get_lexval();
            attack_entry();
            } break;
         default: goto yy11;
         }
      }
      yy11: ;
      get_lexval();
      } break;
   case 77: {
      get_lexval();
      get_lexval();
      l = yylval;
      get_lexval();
#line 417 "grammar.acc"
 for(TMonster*m=firstMon;m!=theMon;m++)
                                         if(stricmp(theModule->GetText(l),theModule->GetText(m->Name))==0)
                                           { memcpy(theMon,m,sizeof(TMonster));
                                             goto DoneCopy; }
                                       Fatal("Monster '%s' not found!",theModule->GetText(l));
                                       DoneCopy:; 
# line 1468 "yygram.cpp"
      } break;
   case 78: {
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 423 "grammar.acc"
 if (n > (theMon - firstMon) || n < 0)
                                         Fatal("Bad number in Monster:as!");
                                       memcpy(theMon,&(firstMon[n]),sizeof(TMonster)); 
# line 1478 "yygram.cpp"
      } break;
   case 79: {
      get_lexval();
      switch (yyselect()) {
      case 522: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 426 "grammar.acc"
 theMon->Def = n; 
# line 1492 "yygram.cpp"
      } break;
   case 80: {
      get_lexval();
      switch (yyselect()) {
      case 523: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 427 "grammar.acc"
 theMon->Arm = n; 
# line 1506 "yygram.cpp"
      } break;
   case 81: {
      get_lexval();
      switch (yyselect()) {
      case 524: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 428 "grammar.acc"
 theMon->Mana = n; 
# line 1520 "yygram.cpp"
      } break;
   case 82: {
      get_lexval();
      switch (yyselect()) {
      case 525: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 526: {
            cexpr3(&n);
            get_lexval();
#line 429 "grammar.acc"
 theMon->Feats[CurrFeat++] = n; 
    if (CurrFeat > 16) Fatal("Too many feats for '%s' (max 16)",
                                theModule->QTextSeg + theMon->Name);
    
# line 1540 "yygram.cpp"
            } break;
         default: goto yy12;
         }
      }
      yy12: ;
      cexpr3(&n);
      get_lexval();
#line 433 "grammar.acc"
 theMon->Feats[CurrFeat++] = n; 
    if (CurrFeat > 16) Fatal("Too many feats for '%s' (max 16)",
                                theModule->QTextSeg + theMon->Name);
    
# line 1553 "yygram.cpp"
      } break;
   case 83: {
      get_lexval();
      switch (yyselect()) {
      case 527: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 528: {
            mflag_entry();
            } break;
         default: goto yy13;
         }
      }
      yy13: ;
      get_lexval();
      } break;
   case 84: {
      attrib_entry();
      while (1) {
         switch (yyselect()) {
         case 529: {
            get_lexval();
            attrib_entry();
            } break;
         default: goto yy14;
         }
      }
      yy14: ;
      get_lexval();
      } break;
   case 85: {
      get_lexval();
      switch (yyselect()) {
      case 530: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 439 "grammar.acc"
 theMon->Size = n; 
# line 1600 "yygram.cpp"
      } break;
   case 86: {
      get_lexval();
      switch (yyselect()) {
      case 531: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 532: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 440 "grammar.acc"
 theMon->Spd = (n-100)/5; 
# line 1620 "yygram.cpp"
      } break;
   case 87: {
      get_lexval();
      switch (yyselect()) {
      case 533: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 534: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 441 "grammar.acc"
 theMon->Mov = (n-100)/5; 
# line 1640 "yygram.cpp"
      } break;
   case 88: {
      get_lexval();
      switch (yyselect()) {
      case 535: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 537: {
            res_ref(&rr);
#line 442 "grammar.acc"
 theMon->SetFlag(M_INNATE);
                                       theMon->AddResID(AN_INNATE, rr); 
# line 1657 "yygram.cpp"
            switch (yyselect()) {
            case 536: {
               get_lexval();
               } break;
            default: ;
            }
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
      case 538: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 540: {
            res_ref(&rr);
#line 445 "grammar.acc"
 theMon->AddResID(AN_DISEASE, rr); 
# line 1685 "yygram.cpp"
            switch (yyselect()) {
            case 539: {
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
   case 90: {
      get_lexval();
      switch (yyselect()) {
      case 541: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 543: {
            res_ref(&rr);
#line 447 "grammar.acc"
 theMon->AddResID(AN_POISON, rr); 
# line 1713 "yygram.cpp"
            switch (yyselect()) {
            case 542: {
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
   case 91: {
      gear_desc();
      } break;
   case 92: {
      event_desc();
      } break;
   case 93: {
      dconst_entry();
      } break;
   case 94: {
      r_declaration();
      } break;
   }
return 0;
}

int color (   YYSTYPE *col)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 95: {
      get_lexval();
      n = yylval;
#line 456 "grammar.acc"
 *col = n; 
# line 1751 "yygram.cpp"
      } break;
   case 96: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 457 "grammar.acc"
 *col = n | 8; 
# line 1759 "yygram.cpp"
      } break;
   case 97: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 458 "grammar.acc"
 *col = n | 8; 
# line 1767 "yygram.cpp"
      } break;
   case 98: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 459 "grammar.acc"
 *col = n & ~8; 
# line 1775 "yygram.cpp"
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
   case 99: {
#line 463 "grammar.acc"
 uint8 dt = AD_NORM; dam.Set(0,0,0);
                                 theMon->Attk[CurrAttk].u.a.Dmg.Set(0,0,0); 
# line 1796 "yygram.cpp"
      cexpr3(&at);
      get_lexval();
      switch (yyselect()) {
      case 544: {
         dice_only(&dam);
#line 466 "grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg = yyDice(dam); 
# line 1804 "yygram.cpp"
         } break;
      case 545: {
         cexpr4(&chance);
         get_lexval();
#line 467 "grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg.Bonus = chance; 
# line 1811 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 546: {
         cexpr4(&n);
#line 468 "grammar.acc"
 dt = n; 
# line 1820 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 547: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 551: {
         get_lexval();
         switch (yyselect()) {
         case 548: {
            get_lexval();
            } break;
         case 549: {
            get_lexval();
            } break;
         case 550: {
            get_lexval();
            } break;
         default: printf("???\n"); exit(1);
         }
         cexpr3(&dc);
#line 471 "grammar.acc"
 theMon->Attk[CurrAttk].u.a.DC = dc; 
# line 1848 "yygram.cpp"
         get_lexval();
         } break;
      default: ;
      }
#line 472 "grammar.acc"
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
                               
# line 1868 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 559: {
            get_lexval();
#line 482 "grammar.acc"
 dt = AD_NORM; dam2.Set(0,0,0); theMon->Attk[CurrAttk].u.a.Dmg.Set(0,0,0); 
# line 1875 "yygram.cpp"
            switch (yyselect()) {
            case 552: {
               dice_only(&dam2);
#line 483 "grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg = yyDice(dam2); 
# line 1881 "yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 553: {
               cexpr3(&n2);
#line 484 "grammar.acc"
 dt = n2; 
# line 1890 "yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 554: {
               get_lexval();
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 558: {
               get_lexval();
               switch (yyselect()) {
               case 555: {
                  get_lexval();
                  } break;
               case 556: {
                  get_lexval();
                  } break;
               case 557: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&dc);
#line 486 "grammar.acc"
 theMon->Attk[CurrAttk].u.a.DC = dc; 
# line 1918 "yygram.cpp"
               get_lexval();
               } break;
            default: ;
            }
#line 487 "grammar.acc"
 theMon->Attk[CurrAttk].AType = A_ALSO;
                                 theMon->Attk[CurrAttk].DType = dt;
                                 CurrAttk++;
                                 if (CurrAttk >= 12)
                                   Fatal("> 12 attacks on a single monster!");
                               
# line 1930 "yygram.cpp"
            } break;
         default: goto yy18;
         }
      }
      yy18: ;
      } break;
   case 100: {
      cexpr3(&at);
      get_lexval();
      res_ref(&rr);
#line 494 "grammar.acc"
 theMon->Attk[CurrAttk].AType = at;
                                  theMon->Attk[CurrAttk].u.xID = rr; 
# line 1944 "yygram.cpp"
      switch (yyselect()) {
      case 560: {
         cexpr3(&dt);
#line 496 "grammar.acc"
 theMon->Attk[CurrAttk].DType = dt; 
# line 1950 "yygram.cpp"
         } break;
      default: ;
      }
#line 497 "grammar.acc"

                                 CurrAttk++;
                                 if (CurrAttk >= 32)
                                   yyerror("> 32 attacks on a single monster!");
                               
# line 1960 "yygram.cpp"
      } break;
   }
return 0;
}

int mflag_entry ()
{
   YYSTYPE mf;
   switch(yyselect()) {
   case 101: {
      cexpr3(&mf);
      switch (yyselect()) {
      case 561: {
         get_lexval();
         } break;
      default: ;
      }
#line 505 "grammar.acc"
 theMon->SetFlag(mf); 
# line 1979 "yygram.cpp"
      } break;
   case 102: {
      get_lexval();
      cexpr3(&mf);
      switch (yyselect()) {
      case 562: {
         get_lexval();
         } break;
      default: ;
      }
#line 506 "grammar.acc"
 theMon->UnsetFlag(mf); 
# line 1992 "yygram.cpp"
      } break;
   }
return 0;
}

int attrib_entry ()
{
   YYSTYPE at;
   YYSTYPE v;
   switch(yyselect()) {
   case 103: {
      get_lexval();
      at = yylval;
      switch (yyselect()) {
      case 563: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&v);
#line 510 "grammar.acc"
 theMon->Attr[at] = v; 
# line 2014 "yygram.cpp"
      } break;
   }
return 0;
}

int item_def ()
{
   YYSTYPE name;
   YYSTYPE n;
   switch(yyselect()) {
   case 104: {
      get_lexval();
#line 514 "grammar.acc"
 theRes = theItem; 
                               theItem->u.w.Crit = 2; 
                               theItem->u.w.Threat = 1; 
                               theItem->hp = 15; 
# line 2031 "yygram.cpp"
      get_lexval();
      name = yylval;
#line 518 "grammar.acc"
 theItem->Name = name; 
# line 2036 "yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 519 "grammar.acc"
 theItem->IType = n; 
# line 2041 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 564: {
            item_entry();
            } break;
         default: goto yy19;
         }
      }
      yy19: ;
      get_lexval();
#line 520 "grammar.acc"
 theItem++; 
# line 2055 "yygram.cpp"
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
   case 105: {
      get_lexval();
      switch (yyselect()) {
      case 565: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 524 "grammar.acc"
 theItem->Desc = lv; 
# line 2083 "yygram.cpp"
      } break;
   case 106: {
      get_lexval();
      switch (yyselect()) {
      case 566: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 525 "grammar.acc"
 theItem->Level = n; 
# line 2097 "yygram.cpp"
      } break;
   case 107: {
      glyph_entry(&img);
#line 526 "grammar.acc"
 theItem->Image = img; 
# line 2103 "yygram.cpp"
      } break;
   case 108: {
      get_lexval();
      switch (yyselect()) {
      case 567: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&dv);
      get_lexval();
#line 527 "grammar.acc"
 theItem->u.w.SDmg = yyDice(dv); 
# line 2117 "yygram.cpp"
      } break;
   case 109: {
      get_lexval();
      switch (yyselect()) {
      case 568: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&dv);
      get_lexval();
#line 528 "grammar.acc"
 theItem->u.w.LDmg = yyDice(dv); 
# line 2131 "yygram.cpp"
      } break;
   case 110: {
      get_lexval();
      switch (yyselect()) {
      case 569: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 529 "grammar.acc"
 theItem->u.w.Crit = n; 
# line 2145 "yygram.cpp"
      } break;
   case 111: {
      get_lexval();
      switch (yyselect()) {
      case 570: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 530 "grammar.acc"
 theItem->u.w.Threat = n; 
# line 2159 "yygram.cpp"
      } break;
   case 112: {
      get_lexval();
      switch (yyselect()) {
      case 571: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 531 "grammar.acc"
 theItem->u.w.Acc = n; 
# line 2173 "yygram.cpp"
      } break;
   case 113: {
      get_lexval();
      switch (yyselect()) {
      case 572: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 532 "grammar.acc"
 theItem->u.w.Spd = n; 
# line 2187 "yygram.cpp"
      } break;
   case 114: {
      get_lexval();
      switch (yyselect()) {
      case 573: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr4(&n);
      get_lexval();
      get_lexval();
#line 533 "grammar.acc"
 theItem->u.w.Spd = n > 0 ? ((n-100)/5) : n/5; 
# line 2202 "yygram.cpp"
      } break;
   case 115: {
      get_lexval();
      switch (yyselect()) {
      case 574: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      cexpr4(&n);
      get_lexval();
      get_lexval();
#line 534 "grammar.acc"
 theItem->u.w.Spd = n/5; 
# line 2218 "yygram.cpp"
      } break;
   case 116: {
      get_lexval();
      switch (yyselect()) {
      case 575: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 535 "grammar.acc"
 theItem->u.w.ParryMod = n; 
# line 2232 "yygram.cpp"
      } break;
   case 117: {
      get_lexval();
      switch (yyselect()) {
      case 576: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 536 "grammar.acc"
 theItem->u.w.RangeInc = n; 
# line 2246 "yygram.cpp"
      } break;
   case 118: {
      get_lexval();
      switch (yyselect()) {
      case 577: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&m);
      get_lexval();
#line 537 "grammar.acc"
 theItem->Material = m; 
# line 2260 "yygram.cpp"
      } break;
   case 119: {
      get_lexval();
      switch (yyselect()) {
      case 578: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
#line 538 "grammar.acc"
 theItem->Cost = n * 100; 
# line 2273 "yygram.cpp"
      switch (yyselect()) {
      case 579: {
         cexpr3(&unit);
#line 539 "grammar.acc"
 switch (unit)
                                         { case PLATINUM: theItem->Cost *= 5; break;
                                           case GOLD:     break;
                                           case ELECTRUM: theItem->Cost /= 2; break;
                                           case SILVER:   theItem->Cost /= 10; break;
                                           case COPPER:   theItem->Cost /= 100; break;
                                           
                                         }
                                     
# line 2287 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      } break;
   case 120: {
      get_lexval();
      switch (yyselect()) {
      case 580: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 549 "grammar.acc"
 theItem->Weight = n; 
    /* ww: according to the SRD, one inch (thickness) of iron has 30

     * hit point. We assume that the canonical long sword (at 4 lbs = 40

     * weight) is 1/2 inch thick. So a base weight of 80 gets you 30 hit

     * points. */
                               theItem->hp = (n * 30) / 80; 
                               if (theItem->hp < 1) theItem->hp = 1; 
# line 2314 "yygram.cpp"
      } break;
   case 121: {
      get_lexval();
      switch (yyselect()) {
      case 581: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 556 "grammar.acc"
 theItem->Size = n; 
# line 2328 "yygram.cpp"
      } break;
   case 122: {
      get_lexval();
      switch (yyselect()) {
      case 582: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 557 "grammar.acc"
 theItem->Nutrition = n; 
# line 2342 "yygram.cpp"
      } break;
   case 123: {
      get_lexval();
      switch (yyselect()) {
      case 583: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 558 "grammar.acc"
 theItem->u.a.Def = n; 
# line 2356 "yygram.cpp"
      } break;
   case 124: {
      get_lexval();
      switch (yyselect()) {
      case 584: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 559 "grammar.acc"
 theItem->u.a.Cov = n; 
# line 2370 "yygram.cpp"
      } break;
   case 125: {
      get_lexval();
      switch (yyselect()) {
      case 585: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 560 "grammar.acc"
 theItem->u.a.Penalty = n; 
# line 2384 "yygram.cpp"
      } break;
   case 126: {
      get_lexval();
      switch (yyselect()) {
      case 586: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 587: {
            arm_entry();
            } break;
         default: goto yy20;
         }
      }
      yy20: ;
      get_lexval();
      } break;
   case 127: {
      get_lexval();
      switch (yyselect()) {
      case 588: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 562 "grammar.acc"
 theItem->hp = n; 
# line 2417 "yygram.cpp"
      } break;
   case 128: {
      get_lexval();
      switch (yyselect()) {
      case 589: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 591: {
            res_ref(&rr);
#line 563 "grammar.acc"
 theItem->AddResID(AN_SPELLS, rr); 
# line 2433 "yygram.cpp"
            switch (yyselect()) {
            case 590: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy21;
         }
      }
      yy21: ;
      get_lexval();
      } break;
   case 129: {
      get_lexval();
      switch (yyselect()) {
      case 592: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 594: {
            res_ref(&rr);
#line 565 "grammar.acc"
 theItem->AddResID(AN_FIRES, rr); 
# line 2461 "yygram.cpp"
            switch (yyselect()) {
            case 593: {
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
   case 130: {
      get_lexval();
      switch (yyselect()) {
      case 595: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 596: {
            iflag_entry();
            } break;
         default: goto yy23;
         }
      }
      yy23: ;
      get_lexval();
      } break;
   case 131: {
      get_lexval();
      switch (yyselect()) {
      case 597: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 600: {
            cexpr3(&n);
            switch (yyselect()) {
            case 598: {
               get_lexval();
               } break;
            case 599: {
               get_lexval();
               } break;
            default: ;
            }
#line 568 "grammar.acc"
 theItem->Group |= n; 
# line 2517 "yygram.cpp"
            } break;
         default: goto yy24;
         }
      }
      yy24: ;
      get_lexval();
      } break;
   case 132: {
      get_lexval();
      switch (yyselect()) {
      case 601: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 569 "grammar.acc"
 theItem->u.c.Capacity = n; 
# line 2537 "yygram.cpp"
      } break;
   case 133: {
      get_lexval();
      switch (yyselect()) {
      case 602: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 570 "grammar.acc"
 theItem->u.c.WeightLim = n; 
# line 2551 "yygram.cpp"
      } break;
   case 134: {
      get_lexval();
      switch (yyselect()) {
      case 603: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 571 "grammar.acc"
 theItem->u.c.WeightMod = n; 
# line 2565 "yygram.cpp"
      } break;
   case 135: {
      get_lexval();
      switch (yyselect()) {
      case 604: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 572 "grammar.acc"
 theItem->u.c.MaxSize = n; 
# line 2579 "yygram.cpp"
      } break;
   case 136: {
      get_lexval();
      switch (yyselect()) {
      case 605: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 573 "grammar.acc"
 theItem->u.c.Timeout = n; 
# line 2593 "yygram.cpp"
      } break;
   case 137: {
      get_lexval();
      switch (yyselect()) {
      case 606: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 574 "grammar.acc"
 theItem->u.c.CType = n; 
# line 2607 "yygram.cpp"
      } break;
   case 138: {
      get_lexval();
      get_lexval();
      switch (yyselect()) {
      case 607: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 575 "grammar.acc"
 theItem->u.l.LightRange = n; 
# line 2622 "yygram.cpp"
      } break;
   case 139: {
      get_lexval();
      switch (yyselect()) {
      case 608: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 576 "grammar.acc"
 theItem->u.l.Lifespan = n; 
# line 2636 "yygram.cpp"
      } break;
   case 140: {
      get_lexval();
      switch (yyselect()) {
      case 609: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 577 "grammar.acc"
 theItem->u.l.Fuel = rr; 
# line 2650 "yygram.cpp"
      } break;
   case 141: {
      dconst_entry();
      } break;
   case 142: {
      event_desc();
      } break;
   case 143: {
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
   case 144: {
      cexpr3(&n);
      switch (yyselect()) {
      case 610: {
         get_lexval();
         } break;
      default: ;
      }
#line 584 "grammar.acc"
 if (theItem->u.a.Arm[0] == 0) theItem->u.a.Arm[0] = n;
                            if (theItem->u.a.Arm[1] == 0) theItem->u.a.Arm[1] = n;
                            if (theItem->u.a.Arm[2] == 0) theItem->u.a.Arm[2] = n;
                         
# line 2682 "yygram.cpp"
      } break;
   case 145: {
      get_lexval();
      wt = yylval;
      cexpr3(&n);
      switch (yyselect()) {
      case 611: {
         get_lexval();
         } break;
      default: ;
      }
#line 588 "grammar.acc"
 theItem->u.a.Arm[wt] = n; 
# line 2696 "yygram.cpp"
      } break;
   }
return 0;
}

int iflag_entry ()
{
   YYSTYPE itf;
   switch(yyselect()) {
   case 146: {
      cexpr3(&itf);
      switch (yyselect()) {
      case 612: {
         get_lexval();
         } break;
      default: ;
      }
#line 592 "grammar.acc"
 theItem->SetFlag(itf); 
# line 2715 "yygram.cpp"
      } break;
   case 147: {
      get_lexval();
      cexpr3(&itf);
      switch (yyselect()) {
      case 613: {
         get_lexval();
         } break;
      default: ;
      }
#line 593 "grammar.acc"
 theItem->UnsetFlag(itf); 
# line 2728 "yygram.cpp"
      } break;
   }
return 0;
}

int feature_def ()
{
   YYSTYPE name;
   YYSTYPE n;
   switch(yyselect()) {
   case 148: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 599 "grammar.acc"
 theRes = theFeat; theFeat->Name = name; theRes = theFeat; 
# line 2744 "yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 600 "grammar.acc"
 theFeat->FType = n; 
# line 2749 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 614: {
            feat_entry();
            } break;
         default: goto yy25;
         }
      }
      yy25: ;
      get_lexval();
#line 601 "grammar.acc"
 theFeat++; 
# line 2763 "yygram.cpp"
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
   case 149: {
      get_lexval();
      switch (yyselect()) {
      case 615: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 605 "grammar.acc"
 theFeat->Desc = lv; 
# line 2791 "yygram.cpp"
      } break;
   case 150: {
      get_lexval();
      switch (yyselect()) {
      case 616: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&m);
      get_lexval();
#line 606 "grammar.acc"
 theFeat->Material = m; 
# line 2805 "yygram.cpp"
      } break;
   case 151: {
      glyph_entry(&img);
#line 607 "grammar.acc"
 theFeat->Image = img; 
# line 2811 "yygram.cpp"
      } break;
   case 152: {
      get_lexval();
      switch (yyselect()) {
      case 617: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mov);
      get_lexval();
      get_lexval();
#line 608 "grammar.acc"
 theFeat->MoveMod = (mov-100)/5; 
# line 2826 "yygram.cpp"
      } break;
   case 153: {
      get_lexval();
      switch (yyselect()) {
      case 618: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 609 "grammar.acc"
 theFeat->hp = n; 
# line 2840 "yygram.cpp"
      } break;
   case 154: {
      get_lexval();
      switch (yyselect()) {
      case 619: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 622: {
            cexpr3(&n);
            switch (yyselect()) {
            case 620: {
               get_lexval();
               } break;
            case 621: {
               get_lexval();
               } break;
            default: ;
            }
#line 610 "grammar.acc"
 theFeat->Flags |= n; 
# line 2865 "yygram.cpp"
            } break;
         default: goto yy26;
         }
      }
      yy26: ;
      get_lexval();
      } break;
   case 155: {
      switch (yyselect()) {
      case 623: {
         get_lexval();
         } break;
      case 624: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 625: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 611 "grammar.acc"
 theFeat->xID = rr; 
# line 2893 "yygram.cpp"
      } break;
   case 156: {
      get_lexval();
      switch (yyselect()) {
      case 626: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 612 "grammar.acc"
 theFeat->xval = n; 
# line 2907 "yygram.cpp"
      } break;
   case 157: {
      get_lexval();
      switch (yyselect()) {
      case 627: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&d);
      get_lexval();
#line 613 "grammar.acc"
 theFeat->Factor = d; 
# line 2921 "yygram.cpp"
      } break;
   case 158: {
      event_desc();
      } break;
   case 159: {
      dconst_entry();
      } break;
   case 160: {
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
   case 161: {
#line 619 "grammar.acc"
 theRes = theEff; theEff->ef.sval = NOSAVE; 
# line 2943 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 629: {
            cexpr3(&n);
#line 620 "grammar.acc"
 if (theEff->Sources[2])
                                        theEff->Sources[3] = n;
                                      else if (theEff->Sources[1])
                                        theEff->Sources[2] = n;
                                      else if (theEff->Sources[0])
                                        theEff->Sources[1] = n; 
                                      else
                                        theEff->Sources[0] = n; 
# line 2957 "yygram.cpp"
            switch (yyselect()) {
            case 628: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy27;
         }
      }
      yy27: ;
      switch (yyselect()) {
      case 630: {
         get_lexval();
         } break;
      case 631: {
         get_lexval();
         } break;
      case 632: {
         get_lexval();
#line 630 "grammar.acc"
 theEff->ef.aval = AR_POISON; 
# line 2980 "yygram.cpp"
         } break;
      case 633: {
         get_lexval();
#line 631 "grammar.acc"
 theEff->ef.aval = AR_DISEASE; 
# line 2986 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      name = yylval;
#line 632 "grammar.acc"
 theEff->Name = name; 
# line 2994 "yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 633 "grammar.acc"
 theValsIdx = 0; 
                                      theEff->ef.eval = n; 
# line 3000 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 634: {
            eff_entry();
            } break;
         default: goto yy28;
         }
      }
      yy28: ;
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 636: {
            get_lexval();
            cexpr3(&n);
#line 636 "grammar.acc"
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
                             
# line 3038 "yygram.cpp"
            get_lexval();
            while (1) {
               switch (yyselect()) {
               case 635: {
                  eff_entry();
                  } break;
               default: goto yy30;
               }
            }
            yy30: ;
            get_lexval();
            } break;
         default: goto yy29;
         }
      }
      yy29: ;
#line 658 "grammar.acc"
 theEff++; 
# line 3057 "yygram.cpp"
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
   case 162: {
      get_lexval();
      switch (yyselect()) {
      case 637: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 662 "grammar.acc"
 theEff->Desc = lv; 
# line 3085 "yygram.cpp"
      } break;
   case 163: {
      cexpr(&sc);
      get_lexval();
#line 663 "grammar.acc"
 theEff->Schools |= sc; 
# line 3092 "yygram.cpp"
      } break;
   case 164: {
      get_lexval();
      switch (yyselect()) {
      case 638: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 664 "grammar.acc"
 theEff->Level = n; 
# line 3106 "yygram.cpp"
      } break;
   case 165: {
      get_lexval();
      switch (yyselect()) {
      case 639: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 665 "grammar.acc"
 theEff->ManaCost = n; 
# line 3120 "yygram.cpp"
      } break;
   case 166: {
      get_lexval();
      switch (yyselect()) {
      case 640: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 641: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 642: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 666 "grammar.acc"
 theEff->BaseChance = n; 
# line 3146 "yygram.cpp"
      } break;
   case 167: {
      get_lexval();
      switch (yyselect()) {
      case 643: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
#line 667 "grammar.acc"
 theEff->Purpose = n; 
# line 3159 "yygram.cpp"
      switch (yyselect()) {
      case 644: {
         get_lexval();
         cexpr(&n2);
         get_lexval();
#line 668 "grammar.acc"
 theEff->Purpose |= n2<<16; 
# line 3167 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      } break;
   case 168: {
      get_lexval();
      switch (yyselect()) {
      case 645: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&sv);
      switch (yyselect()) {
      case 646: {
         get_lexval();
#line 669 "grammar.acc"
 theEff->SetFlag(EF_PARTIAL); 
# line 3187 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
#line 670 "grammar.acc"
 ESTABLISH_VALS; theVals->sval = sv; 
# line 3194 "yygram.cpp"
      } break;
   case 169: {
      get_lexval();
      switch (yyselect()) {
      case 647: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 671 "grammar.acc"
 ESTABLISH_VALS; theVals->qval = n; 
# line 3208 "yygram.cpp"
      } break;
   case 170: {
      get_lexval();
      switch (yyselect()) {
      case 648: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 672 "grammar.acc"
 ESTABLISH_VALS; theVals->xval = n; 
# line 3222 "yygram.cpp"
      } break;
   case 171: {
      get_lexval();
      switch (yyselect()) {
      case 649: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 673 "grammar.acc"
 ESTABLISH_VALS; theVals->yval = n; 
# line 3236 "yygram.cpp"
      } break;
   case 172: {
      get_lexval();
      switch (yyselect()) {
      case 650: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 674 "grammar.acc"
 ESTABLISH_VALS; theVals->dval = n; 
# line 3250 "yygram.cpp"
      } break;
   case 173: {
      get_lexval();
      switch (yyselect()) {
      case 651: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&d);
      get_lexval();
#line 675 "grammar.acc"
 ESTABLISH_VALS; theVals->pval = d; 
# line 3264 "yygram.cpp"
      } break;
   case 174: {
      get_lexval();
      switch (yyselect()) {
      case 652: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 676 "grammar.acc"
 ESTABLISH_VALS; theVals->aval = n; 
# line 3278 "yygram.cpp"
      } break;
   case 175: {
      get_lexval();
      switch (yyselect()) {
      case 653: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 677 "grammar.acc"
 ESTABLISH_VALS; theVals->lval = n; 
# line 3292 "yygram.cpp"
      } break;
   case 176: {
      get_lexval();
      switch (yyselect()) {
      case 654: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 678 "grammar.acc"
 ESTABLISH_VALS; theVals->tval = n; 
# line 3306 "yygram.cpp"
      } break;
   case 177: {
      get_lexval();
      switch (yyselect()) {
      case 655: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 679 "grammar.acc"
 ESTABLISH_VALS; theVals->rval = rr; 
# line 3320 "yygram.cpp"
      } break;
   case 178: {
      get_lexval();
      switch (yyselect()) {
      case 656: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 680 "grammar.acc"
 ESTABLISH_VALS; theVals->rval = n; 
# line 3334 "yygram.cpp"
      } break;
   case 179: {
      get_lexval();
      switch (yyselect()) {
      case 657: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 681 "grammar.acc"
 ESTABLISH_VALS; theVals->cval = n; 
# line 3348 "yygram.cpp"
      } break;
   case 180: {
      get_lexval();
      switch (yyselect()) {
      case 658: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      n = yylval;
      get_lexval();
#line 682 "grammar.acc"
 ESTABLISH_VALS; theVals->cval = n; 
# line 3363 "yygram.cpp"
      } break;
   case 181: {
      get_lexval();
      switch (yyselect()) {
      case 659: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 662: {
            cexpr3(&n);
#line 683 "grammar.acc"
 theEff->SetFlag(n); 
# line 3379 "yygram.cpp"
            switch (yyselect()) {
            case 660: {
               get_lexval();
               } break;
            case 661: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy31;
         }
      }
      yy31: ;
      get_lexval();
      } break;
   case 182: {
      event_desc();
      } break;
   case 183: {
      r_declaration();
      } break;
   case 184: {
      dconst_entry();
      } break;
   }
return 0;
}

int numlist (   YYSTYPE *val)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 185: {
#line 690 "grammar.acc"
 *val = 0; 
# line 3415 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 665: {
            cexpr3(&n);
#line 691 "grammar.acc"
 *val |= n; 
# line 3422 "yygram.cpp"
            switch (yyselect()) {
            case 663: {
               get_lexval();
               } break;
            case 664: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy32;
         }
      }
      yy32: ;
      } break;
   }
return 0;
}

int object_entry ()
{
   switch(yyselect()) {
   case 186: {
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
   case 187: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 700 "grammar.acc"
 theRes = theRace; theRace->Name = name;
                                    theRace->BaseRace = 0; 
# line 3462 "yygram.cpp"
      switch (yyselect()) {
      case 666: {
         get_lexval();
         res_ref(&base);
#line 702 "grammar.acc"
 TRace tr; 
                                    theRace->BaseRace = base;                                        
                                    if (TRACE(base) > theRace)
                                      Fatal("Subrace definition before def of base race!"); 
# line 3472 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 667: {
            race_entry();
            } break;
         default: goto yy33;
         }
      }
      yy33: ;
      get_lexval();
#line 707 "grammar.acc"
 theRace++; 
# line 3489 "yygram.cpp"
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
   case 188: {
      get_lexval();
      switch (yyselect()) {
      case 668: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 711 "grammar.acc"
 theRace->Desc = lv; 
# line 3523 "yygram.cpp"
      } break;
   case 189: {
      while (1) {
         switch (yyselect()) {
         case 671: {
            get_lexval();
            at = yylval;
            switch (yyselect()) {
            case 669: {
               get_lexval();
               } break;
            default: ;
            }
            cexpr(&v);
#line 712 "grammar.acc"
 theRace->AttrAdj[at] = v; 
# line 3540 "yygram.cpp"
            switch (yyselect()) {
            case 670: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy34;
         }
      }
      yy34: ;
      get_lexval();
      } break;
   case 190: {
      get_lexval();
      switch (yyselect()) {
      case 672: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 674: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 673: {
               get_lexval();
               } break;
            default: ;
            }
#line 715 "grammar.acc"
 theRace->AddAbility(AbT,Abil,p,l1,l2); 
# line 3574 "yygram.cpp"
            } break;
         default: goto yy35;
         }
      }
      yy35: ;
      get_lexval();
      } break;
   case 191: {
      get_lexval();
      switch (yyselect()) {
      case 675: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 716 "grammar.acc"
 theRace->mID = rr; 
# line 3594 "yygram.cpp"
      } break;
   case 192: {
      get_lexval();
      switch (yyselect()) {
      case 676: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 677: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
      res_ref(&rr2);
      get_lexval();
#line 718 "grammar.acc"
 theRace->FavoredClass[0] = rr;
                                  theRace->FavoredClass[1] = rr2; 
# line 3617 "yygram.cpp"
      } break;
   case 193: {
      get_lexval();
      switch (yyselect()) {
      case 678: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 679: {
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
#line 721 "grammar.acc"
 theRace->FavoredClass[0] = rr;
                                  theRace->FavoredClass[1] = rr2; 
                                  theRace->FavoredClass[2] = rr3; 
# line 3643 "yygram.cpp"
      } break;
   case 194: {
      get_lexval();
      switch (yyselect()) {
      case 680: {
         get_lexval();
         } break;
      default: ;
      }
#line 724 "grammar.acc"
int8 skn = 0;
# line 3655 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 682: {
            cexpr3(&sk);
#line 725 "grammar.acc"
 theRace->Skills[skn++] = sk; 
# line 3662 "yygram.cpp"
            switch (yyselect()) {
            case 681: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy36;
         }
      }
      yy36: ;
      get_lexval();
      } break;
   case 195: {
      cexpr3(&n);
      switch (yyselect()) {
      case 683: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 726 "grammar.acc"
 switch(n) {
                                         case MALE_NAMES  : theRace-> MNames = lv; break;
                                         case FEMALE_NAMES: theRace-> FNames = lv; break;
                                         case FAMILY_NAMES: theRace-> SNames = lv; break;
                                         default: yyerror("Illegal name const in race!");
                                         } 
# line 3694 "yygram.cpp"
      } break;
   case 196: {
      get_lexval();
      switch (yyselect()) {
      case 684: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 687: {
            cexpr3(&n);
            switch (yyselect()) {
            case 685: {
               get_lexval();
               } break;
            case 686: {
               get_lexval();
               } break;
            default: ;
            }
#line 732 "grammar.acc"
 theRace->SetFlag(n); 
# line 3719 "yygram.cpp"
            } break;
         default: goto yy37;
         }
      }
      yy37: ;
      get_lexval();
      } break;
   case 197: {
      gear_desc();
      } break;
   case 198: {
      dconst_entry();
      } break;
   case 199: {
      event_desc();
      } break;
   case 200: {
      r_declaration();
      } break;
   }
return 0;
}

int class_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 201: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 740 "grammar.acc"
 theRes = theClass; theClass->Name = name; 
# line 3752 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 688: {
            class_entry();
            } break;
         default: goto yy38;
         }
      }
      yy38: ;
      get_lexval();
#line 741 "grammar.acc"
 theClass++; 
# line 3766 "yygram.cpp"
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
   case 202: {
      get_lexval();
      switch (yyselect()) {
      case 689: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 745 "grammar.acc"
 theClass->Desc = lv; 
# line 3800 "yygram.cpp"
      } break;
   case 203: {
      get_lexval();
      switch (yyselect()) {
      case 690: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 691: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 746 "grammar.acc"
 theClass->HitDie = n; 
# line 3820 "yygram.cpp"
      } break;
   case 204: {
      get_lexval();
      switch (yyselect()) {
      case 692: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 693: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 747 "grammar.acc"
 theClass->ManaDie = n; 
# line 3840 "yygram.cpp"
      } break;
   case 205: {
      get_lexval();
      switch (yyselect()) {
      case 694: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&x);
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 748 "grammar.acc"
 theClass->DefMod = n;
                                             if (x != 1) yyerror("class defense mod must by 1 per N."); 
# line 3857 "yygram.cpp"
      } break;
   case 206: {
      get_lexval();
      switch (yyselect()) {
      case 695: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 697: {
            cexpr3(&typ);
            cexpr3(&l);
#line 751 "grammar.acc"
 theClass->AttkVal[typ] = l; 
# line 3874 "yygram.cpp"
            switch (yyselect()) {
            case 696: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy39;
         }
      }
      yy39: ;
      get_lexval();
      } break;
   case 207: {
      get_lexval();
      switch (yyselect()) {
      case 698: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 700: {
            cexpr3(&typ);
            cexpr3(&l);
#line 753 "grammar.acc"
 theClass->Saves[typ] = l; 
# line 3903 "yygram.cpp"
            switch (yyselect()) {
            case 699: {
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
   case 208: {
      get_lexval();
      switch (yyselect()) {
      case 701: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 703: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 702: {
               get_lexval();
               } break;
            default: ;
            }
#line 755 "grammar.acc"
 theClass->AddAbility(AbT,Abil,p,l1,l2); 
# line 3937 "yygram.cpp"
            } break;
         default: goto yy41;
         }
      }
      yy41: ;
      get_lexval();
      } break;
   case 209: {
      get_lexval();
      switch (yyselect()) {
      case 704: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      cexpr3(&sp);
      switch (yyselect()) {
      case 705: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
#line 756 "grammar.acc"
 theClass->SkillPoints = sp; int8 skn = 0; 
# line 3962 "yygram.cpp"
      switch (yyselect()) {
      case 706: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 708: {
            cexpr3(&sk);
#line 757 "grammar.acc"
 theClass->Skills[skn++] = sk; 
# line 3975 "yygram.cpp"
            switch (yyselect()) {
            case 707: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy42;
         }
      }
      yy42: ;
      get_lexval();
      } break;
   case 210: {
      get_lexval();
      switch (yyselect()) {
      case 709: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 711: {
            cexpr3(&pr);
#line 759 "grammar.acc"
 theClass->Proficiencies |= pr; 
# line 4003 "yygram.cpp"
            switch (yyselect()) {
            case 710: {
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
   case 211: {
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
#line 760 "grammar.acc"
 theClass->SetFlag(n); 
# line 4040 "yygram.cpp"
            } break;
         default: goto yy44;
         }
      }
      yy44: ;
      get_lexval();
      } break;
   case 212: {
      gear_desc();
      } break;
   case 213: {
      event_desc();
      } break;
   case 214: {
      dconst_entry();
      } break;
   case 215: {
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
   case 216: {
#line 768 "grammar.acc"
 *p = 0; 
# line 4075 "yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&ca);
      switch (yyselect()) {
      case 716: {
         abil_param(&xp);
#line 768 "grammar.acc"
 *p = xp; 
# line 4084 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 769 "grammar.acc"
 *AbT = AB_ABILITY; *Abil = ca; *l1 =xl1; *l2 = xl2; 
# line 4093 "yygram.cpp"
      } break;
   case 217: {
#line 770 "grammar.acc"
 *p = 0; 
# line 4098 "yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&st);
      switch (yyselect()) {
      case 717: {
         abil_param(&xp);
#line 770 "grammar.acc"
 *p = xp; 
# line 4107 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 771 "grammar.acc"
 *AbT = AB_STATI;   *Abil = st; *l1 = xl1; *l2 = xl2; 
# line 4116 "yygram.cpp"
      } break;
   case 218: {
#line 772 "grammar.acc"
 *p = 0; 
# line 4121 "yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&ft);
      switch (yyselect()) {
      case 718: {
         abil_param(&xp);
#line 772 "grammar.acc"
 *p = xp; 
# line 4130 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 773 "grammar.acc"
 *AbT = AB_FEAT;    *Abil = ft; *l1 = xl1; *l2 = xl2; 
# line 4139 "yygram.cpp"
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
   case 219: {
      get_lexval();
      cexpr3(&pa);
#line 777 "grammar.acc"
 *p = pa; 
# line 4155 "yygram.cpp"
      } break;
   case 220: {
      get_lexval();
      res_ref(&rr);
#line 778 "grammar.acc"
 *p = rr; 
# line 4162 "yygram.cpp"
      } break;
   case 221: {
      get_lexval();
      cexpr3(&pa);
      get_lexval();
      cexpr3(&pa2);
#line 779 "grammar.acc"
 *p = pa | (pa2 << 16); 
# line 4171 "yygram.cpp"
      } break;
   }
return 0;
}

int abil_level (   YYSTYPE *l1,    YYSTYPE *l2)
{
   YYSTYPE xl1;
   YYSTYPE xl2;
   switch(yyselect()) {
   case 222: {
      cexpr3(&xl1);
      switch (yyselect()) {
      case 719: {
         get_lexval();
         } break;
      default: ;
      }
#line 784 "grammar.acc"
 *l1 = xl1; *l2 = 0; 
# line 4191 "yygram.cpp"
      } break;
   case 223: {
      get_lexval();
      cexpr3(&xl2);
      get_lexval();
#line 786 "grammar.acc"
 *l1 = 1;  *l2 = xl2; 
# line 4199 "yygram.cpp"
      } break;
   case 224: {
      get_lexval();
      get_lexval();
#line 788 "grammar.acc"
 *l1 = 1;  *l2 = 1; 
# line 4206 "yygram.cpp"
      } break;
   case 225: {
      get_lexval();
      get_lexval();
      get_lexval();
      get_lexval();
      cexpr3(&xl1);
      switch (yyselect()) {
      case 720: {
         get_lexval();
         } break;
      default: ;
      }
#line 790 "grammar.acc"
 *l1 = xl1; *l2 = 1; 
# line 4222 "yygram.cpp"
      } break;
   case 226: {
      get_lexval();
      cexpr3(&xl2);
      get_lexval();
      get_lexval();
      get_lexval();
      cexpr3(&xl1);
      switch (yyselect()) {
      case 721: {
         get_lexval();
         } break;
      default: ;
      }
#line 792 "grammar.acc"
 *l1 = xl1; *l2 = xl2; 
# line 4239 "yygram.cpp"
      } break;
   }
return 0;
}

int dungeon_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 227: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 796 "grammar.acc"
 theRes = theDgn; theDgn->Name = name; 
# line 4254 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 722: {
            dun_entry();
            } break;
         default: goto yy45;
         }
      }
      yy45: ;
      get_lexval();
#line 797 "grammar.acc"
 theDgn++; 
# line 4268 "yygram.cpp"
      } break;
   }
return 0;
}

int dun_entry ()
{
   YYSTYPE lv;
   switch(yyselect()) {
   case 228: {
      get_lexval();
      switch (yyselect()) {
      case 723: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 801 "grammar.acc"
 theDgn->Desc = lv; 
# line 4290 "yygram.cpp"
      } break;
   case 229: {
      dconst_entry();
      } break;
   case 230: {
      event_desc();
      } break;
   case 231: {
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
   case 232: {
      get_lexval();
      switch (yyselect()) {
      case 724: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 726: {
            get_lexval();
            cexpr3(&con);
            cexpr3(&val);
#line 809 "grammar.acc"
 theRes->AddConstant(con,val); 
# line 4329 "yygram.cpp"
            switch (yyselect()) {
            case 725: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         case 728: {
            get_lexval();
            cexpr3(&con);
            res_ref(&rr);
#line 810 "grammar.acc"
 theRes->AddConstant(con,rr); 
# line 4343 "yygram.cpp"
            switch (yyselect()) {
            case 727: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy46;
         }
      }
      yy46: ;
      get_lexval();
      } break;
   case 233: {
      get_lexval();
      switch (yyselect()) {
      case 729: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 733: {
            get_lexval();
            cexpr4(&lis);
#line 813 "grammar.acc"
 TempListLoc = 0; 
# line 4372 "yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 730: {
                  list_entry();
                  } break;
               default: goto yy48;
               }
            }
            yy48: ;
            switch (yyselect()) {
            case 731: {
               get_lexval();
               } break;
            case 732: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
#line 814 "grammar.acc"
 TempList[TempListLoc++] = 0;
        TempList[TempListLoc++] = 0;
        TempList[TempListLoc++] = 0;
        ASSERT(TempListLoc < 1023);
        theRes->AddList(lis,TempList); 
# line 4397 "yygram.cpp"
            } break;
         default: goto yy47;
         }
      }
      yy47: ;
      } break;
   case 234: {
      get_lexval();
      switch (yyselect()) {
      case 734: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 740: {
#line 821 "grammar.acc"
 int16 Chance = 100; 
# line 4417 "yygram.cpp"
            get_lexval();
            switch (yyselect()) {
            case 735: {
               cexpr4(&n);
               get_lexval();
#line 821 "grammar.acc"
 Chance = n; 
# line 4425 "yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 736: {
               res_ref(&rr);
               get_lexval();
               get_lexval();
               cexpr3(&n);
#line 822 "grammar.acc"
 theRes->AddSpecial(rr,Chance,n); 
# line 4437 "yygram.cpp"
               } break;
            case 737: {
               res_ref(&rr);
               get_lexval();
               cexpr3(&n);
               get_lexval();
#line 823 "grammar.acc"
 theRes->AddSpecial(rr,Chance,n); 
# line 4446 "yygram.cpp"
               } break;
            case 738: {
               res_ref(&rr);
               get_lexval();
               get_lexval();
               dice_only(&dv);
#line 824 "grammar.acc"
 theRes->AddSpecial(rr,Chance,yyDice(dv)); 
# line 4455 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 739: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy49;
         }
      }
      yy49: ;
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
   case 235: {
      res_ref(&rr);
#line 829 "grammar.acc"
 TempList[TempListLoc++] = rr; ASSERT(TempListLoc < 1023); 
# line 4485 "yygram.cpp"
      } break;
   case 236: {
      cexpr4(&n);
#line 830 "grammar.acc"
 TempList[TempListLoc++] = n;  ASSERT(TempListLoc < 1023); 
# line 4491 "yygram.cpp"
      } break;
   case 237: {
      get_lexval();
      text = yylval;
#line 831 "grammar.acc"
 TempList[TempListLoc++] = text; ASSERT(TempListLoc < 1023); 
# line 4498 "yygram.cpp"
      } break;
   }
return 0;
}

int artifact_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 238: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 835 "grammar.acc"
 theRes = theArti; theArti->Name = name; 
# line 4513 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 741: {
            arti_entry();
            } break;
         default: goto yy50;
         }
      }
      yy50: ;
      get_lexval();
#line 836 "grammar.acc"
 theArti++; 
# line 4527 "yygram.cpp"
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
   case 239: {
      get_lexval();
      switch (yyselect()) {
      case 742: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 840 "grammar.acc"
 theRes = theArti; theArti->Desc = lv; 
# line 4554 "yygram.cpp"
      } break;
   case 240: {
      get_lexval();
      switch (yyselect()) {
      case 743: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 841 "grammar.acc"
 theArti->iID = rr; 
# line 4568 "yygram.cpp"
      } break;
   case 241: {
      while (1) {
         switch (yyselect()) {
         case 746: {
            get_lexval();
            at = yylval;
            switch (yyselect()) {
            case 744: {
               get_lexval();
               } break;
            default: ;
            }
            cexpr3(&v);
#line 842 "grammar.acc"
 theArti->AttrAdjLow[at] = v; 
# line 4585 "yygram.cpp"
            switch (yyselect()) {
            case 745: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy51;
         }
      }
      yy51: ;
      get_lexval();
      } break;
   case 242: {
      get_lexval();
      switch (yyselect()) {
      case 747: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 749: {
            cexpr3(&res);
#line 843 "grammar.acc"
 theArti->Resists |= res; 
# line 4613 "yygram.cpp"
            switch (yyselect()) {
            case 748: {
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
   case 243: {
      get_lexval();
      switch (yyselect()) {
      case 750: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 752: {
            get_lexval();
            at = yylval;
#line 844 "grammar.acc"
 theArti->Sustains |= 1 << (at-1); 
# line 4642 "yygram.cpp"
            switch (yyselect()) {
            case 751: {
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
   case 244: {
      get_lexval();
      switch (yyselect()) {
      case 753: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 755: {
            cexpr3(&qu);
#line 845 "grammar.acc"
 theArti->Qualities |= qu; 
# line 4670 "yygram.cpp"
            switch (yyselect()) {
            case 754: {
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
   case 245: {
      get_lexval();
#line 847 "grammar.acc"
 ArtPowerType = AN_ART_EQUIP; 
# line 4688 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 757: {
            art_power();
            switch (yyselect()) {
            case 756: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy55;
         }
      }
      yy55: ;
      get_lexval();
      } break;
   case 246: {
      get_lexval();
#line 848 "grammar.acc"
 ArtPowerType = AN_ART_WIELD; 
# line 4711 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 759: {
            art_power();
            switch (yyselect()) {
            case 758: {
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
   case 247: {
      get_lexval();
#line 849 "grammar.acc"
 ArtPowerType = AN_ART_HIT;   
# line 4734 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 761: {
            art_power();
            switch (yyselect()) {
            case 760: {
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
   case 248: {
      get_lexval();
#line 850 "grammar.acc"
 ArtPowerType = AN_ART_INVOKE;
# line 4757 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 763: {
            art_power();
            switch (yyselect()) {
            case 762: {
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
   case 249: {
      event_desc();
      } break;
   case 250: {
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
   case 251: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&p);
      get_lexval();
#line 857 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_STATI,st,p,NULL); 
# line 4803 "yygram.cpp"
      } break;
   case 252: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
#line 859 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_STATI,st,0,NULL); 
# line 4812 "yygram.cpp"
      } break;
   case 253: {
      res_ref(&rr);
      switch (yyselect()) {
      case 764: {
         get_lexval();
         get_lexval();
         } break;
      default: ;
      }
#line 861 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_EFFECT,rr,0,NULL); 
# line 4825 "yygram.cpp"
      } break;
   case 254: {
      res_ref(&rr);
      get_lexval();
      dice_val(&dv);
      get_lexval();
#line 863 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_EFFECT,rr,0,&yyDice(dv)); 
# line 4834 "yygram.cpp"
      } break;
   case 255: {
      res_ref(&rr);
      cexpr3(&n);
      switch (yyselect()) {
      case 765: {
         get_lexval();
         } break;
      case 766: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
#line 865 "grammar.acc"
 Dice d; d.Set(0,n,1);
        theArti->AddPower(ArtPowerType,AB_EFFECT,rr,0,&d); 
# line 4852 "yygram.cpp"
      } break;
   case 256: {
      res_ref(&rr);
      cexpr3(&n);
      switch (yyselect()) {
      case 767: {
         get_lexval();
         } break;
      case 768: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
#line 868 "grammar.acc"
 Dice d; d.Set(0,n,7);
        theArti->AddPower(ArtPowerType,AB_EFFECT,rr,0,&d); 
# line 4870 "yygram.cpp"
      } break;
   case 257: {
      get_lexval();
      res_ref(&rr);
#line 871 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_INNATE,rr,0,NULL); 
# line 4877 "yygram.cpp"
      } break;
   case 258: {
      get_lexval();
      cexpr3(&ft);
#line 873 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_FEAT,ft,0,NULL); 
# line 4884 "yygram.cpp"
      } break;
   }
return 0;
}

int domain_def ()
{
   YYSTYPE name;
   YYSTYPE type;
   switch(yyselect()) {
   case 259: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
      cexpr3(&type);
#line 877 "grammar.acc"
 theRes = theDom; theDom->Name = name; theDom->DType = type; 
# line 4902 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 769: {
            domain_entry();
            } break;
         default: goto yy59;
         }
      }
      yy59: ;
      get_lexval();
#line 879 "grammar.acc"
 theDom++; 
# line 4916 "yygram.cpp"
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
   case 260: {
#line 883 "grammar.acc"
 int8 sp = 0; 
# line 4934 "yygram.cpp"
      get_lexval();
      switch (yyselect()) {
      case 770: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 772: {
            res_ref(&rr);
#line 884 "grammar.acc"
 theDom->Spells[min(sp,8)] = rr; sp++; 
# line 4948 "yygram.cpp"
            switch (yyselect()) {
            case 771: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy60;
         }
      }
      yy60: ;
      get_lexval();
#line 885 "grammar.acc"
 if (sp > 9) 
      { yyerror(Format("Error: Domain %s has %d spells; must have at most nine.",
        theModule->QTextSeg + theDom->Name,sp)); } 
# line 4965 "yygram.cpp"
      } break;
   case 261: {
      get_lexval();
      switch (yyselect()) {
      case 773: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 775: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 774: {
               get_lexval();
               } break;
            default: ;
            }
#line 889 "grammar.acc"
 theDom->AddAbility(AbT,Abil,p,l1,l2); 
# line 4987 "yygram.cpp"
            } break;
         default: goto yy61;
         }
      }
      yy61: ;
      get_lexval();
      } break;
   case 262: {
      get_lexval();
      switch (yyselect()) {
      case 776: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 890 "grammar.acc"
 theDom->Desc = lv; 
# line 5008 "yygram.cpp"
      } break;
   case 263: {
      event_desc();
      } break;
   case 264: {
      dconst_entry();
      } break;
   case 265: {
      r_declaration();
      } break;
   }
return 0;
}

int god_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 266: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 896 "grammar.acc"
 theRes = theGod; theGod->Name = name; 
# line 5032 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 777: {
            god_entry();
            } break;
         default: goto yy62;
         }
      }
      yy62: ;
      get_lexval();
#line 898 "grammar.acc"
 theGod++; 
# line 5046 "yygram.cpp"
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
   case 267: {
#line 902 "grammar.acc"
 int8 dm = 0; 
# line 5065 "yygram.cpp"
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
            res_ref(&rr);
#line 903 "grammar.acc"
 theGod->Domains[min(dm,5)] = rr; dm++; 
# line 5079 "yygram.cpp"
            switch (yyselect()) {
            case 779: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy63;
         }
      }
      yy63: ;
      get_lexval();
#line 904 "grammar.acc"
 if (dm < 3 || dm > 6) 
      { yyerror(Format("Error: God %s has %d domains; must have between 3 and 6.",
        theModule->QTextSeg + theGod->Name,dm)); } 
# line 5096 "yygram.cpp"
      } break;
   case 268: {
      get_lexval();
      get_lexval();
      } break;
   case 269: {
      get_lexval();
      switch (yyselect()) {
      case 781: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 784: {
            cexpr3(&n);
            switch (yyselect()) {
            case 782: {
               get_lexval();
               } break;
            case 783: {
               get_lexval();
               } break;
            default: ;
            }
#line 908 "grammar.acc"
 theGod->SetFlag(n); 
# line 5125 "yygram.cpp"
            } break;
         default: goto yy64;
         }
      }
      yy64: ;
      get_lexval();
      } break;
   case 270: {
      get_lexval();
      switch (yyselect()) {
      case 785: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 909 "grammar.acc"
 theGod->Desc = lv; 
# line 5146 "yygram.cpp"
      } break;
   case 271: {
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
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 787: {
               get_lexval();
               } break;
            default: ;
            }
#line 911 "grammar.acc"
 theGod->AddAbility(AbT,Abil,p,l1,l2); 
# line 5168 "yygram.cpp"
            } break;
         default: goto yy65;
         }
      }
      yy65: ;
      get_lexval();
      } break;
   case 272: {
      event_desc();
      } break;
   case 273: {
      dconst_entry();
      } break;
   case 274: {
      r_declaration();
      } break;
   }
return 0;
}

int terrain_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 275: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 917 "grammar.acc"
 theRes = theTer; 
                                      theTer->Name = name; 
# line 5199 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 789: {
            terrain_entry();
            } break;
         default: goto yy66;
         }
      }
      yy66: ;
      get_lexval();
#line 920 "grammar.acc"
 theTer++; 
# line 5213 "yygram.cpp"
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
   case 276: {
      get_lexval();
      switch (yyselect()) {
      case 790: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 924 "grammar.acc"
 theTer->Desc = lv; 
# line 5240 "yygram.cpp"
      } break;
   case 277: {
      glyph_entry(&img);
#line 925 "grammar.acc"
 theTer->Image = img; 
# line 5246 "yygram.cpp"
      } break;
   case 278: {
      get_lexval();
      switch (yyselect()) {
      case 791: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mov);
      get_lexval();
      get_lexval();
#line 926 "grammar.acc"
 theTer->MoveMod = (mov-100)/5; 
# line 5261 "yygram.cpp"
      } break;
   case 279: {
      get_lexval();
      switch (yyselect()) {
      case 792: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&pen);
      get_lexval();
#line 927 "grammar.acc"
 theTer->Penalty = pen; 
# line 5275 "yygram.cpp"
      } break;
   case 280: {
      get_lexval();
      switch (yyselect()) {
      case 793: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mat);
      get_lexval();
#line 928 "grammar.acc"
 theTer->Material = mat; 
# line 5289 "yygram.cpp"
      } break;
   case 281: {
      get_lexval();
      switch (yyselect()) {
      case 794: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 797: {
            cexpr3(&n);
            switch (yyselect()) {
            case 795: {
               get_lexval();
               } break;
            case 796: {
               get_lexval();
               } break;
            default: ;
            }
#line 929 "grammar.acc"
 theTer->SetFlag(n); 
# line 5314 "yygram.cpp"
            } break;
         default: goto yy67;
         }
      }
      yy67: ;
      get_lexval();
      } break;
   case 282: {
      dconst_entry();
      } break;
   case 283: {
      event_desc();
      } break;
   case 284: {
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
   case 285: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
      cexpr3(&regflag);
#line 936 "grammar.acc"
 theRes = theReg; theReg->Name = name; theReg->SetFlag(regflag); 
    theReg->Size = SZ_COLLOSAL; 
# line 5348 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 798: {
            region_entry();
            } break;
         default: goto yy68;
         }
      }
      yy68: ;
      get_lexval();
#line 939 "grammar.acc"
 theReg++; 
# line 5362 "yygram.cpp"
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
   case 286: {
      get_lexval();
      switch (yyselect()) {
      case 799: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 943 "grammar.acc"
 theReg->Desc = lv; 
# line 5392 "yygram.cpp"
      } break;
   case 287: {
      get_lexval();
      switch (yyselect()) {
      case 800: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 944 "grammar.acc"
 theReg->Walls = rr; 
# line 5406 "yygram.cpp"
      } break;
   case 288: {
      get_lexval();
      switch (yyselect()) {
      case 801: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 945 "grammar.acc"
 theReg->Floor = rr; 
# line 5420 "yygram.cpp"
      } break;
   case 289: {
      get_lexval();
      switch (yyselect()) {
      case 802: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 946 "grammar.acc"
 theReg->Door = rr; 
# line 5434 "yygram.cpp"
      } break;
   case 290: {
      get_lexval();
      switch (yyselect()) {
      case 803: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 947 "grammar.acc"
 theReg->Depth = n; 
# line 5448 "yygram.cpp"
      } break;
   case 291: {
      get_lexval();
      switch (yyselect()) {
      case 804: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 948 "grammar.acc"
 theReg->Size = n; 
# line 5462 "yygram.cpp"
      } break;
   case 292: {
      get_lexval();
      switch (yyselect()) {
      case 805: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 808: {
            cexpr3(&n);
#line 949 "grammar.acc"
 theReg->RoomTypes |= BIT(n); 
# line 5478 "yygram.cpp"
            switch (yyselect()) {
            case 806: {
               get_lexval();
               } break;
            case 807: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy69;
         }
      }
      yy69: ;
      get_lexval();
      } break;
   case 293: {
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
#line 951 "grammar.acc"
 theReg->SetFlag(n); 
# line 5518 "yygram.cpp"
            } break;
         default: goto yy70;
         }
      }
      yy70: ;
      get_lexval();
      } break;
   case 294: {
      get_lexval();
      get_lexval();
      get_lexval();
      mg = yylval;
      get_lexval();
#line 952 "grammar.acc"
 theReg->sx   = yyMapSize[currMap] % 256; 
                                    theReg->sy   = yyMapSize[currMap++] / 256;
                                    theReg->Grid = mg; 
# line 5536 "yygram.cpp"
      } break;
   case 295: {
      get_lexval();
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 825: {
#line 955 "grammar.acc"
 rID tID, xID, xID2; Glyph Img; uint8 fl; 
                                    tID = xID = xID2 = 0; Img = 0; fl = 0; 
# line 5547 "yygram.cpp"
            get_lexval();
            ch = yylval;
            get_lexval();
            res_ref(&rr);
#line 957 "grammar.acc"
 tID = rr; 
# line 5554 "yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 813: {
                  get_lexval();
                  cexpr(&n);
                  get_lexval();
#line 958 "grammar.acc"
 fl |= n; 
# line 5563 "yygram.cpp"
                  } break;
               case 822: {
                  get_lexval();
                  switch (yyselect()) {
                  case 814: {
                     res_ref(&rr);
#line 959 "grammar.acc"
 xID = rr; 
# line 5572 "yygram.cpp"
                     } break;
                  case 816: {
                     get_lexval();
                     get_lexval();
                     switch (yyselect()) {
                     case 815: {
                        cexpr(&v);
#line 960 "grammar.acc"
 xID = v; 
# line 5582 "yygram.cpp"
                        } break;
                     default: ;
                     }
#line 961 "grammar.acc"
 fl |= TILE_RANDOM | TILE_ITEM; 
# line 5588 "yygram.cpp"
                     } break;
                  case 818: {
                     get_lexval();
                     get_lexval();
                     switch (yyselect()) {
                     case 817: {
                        cexpr(&v);
#line 962 "grammar.acc"
 xID = v; 
# line 5598 "yygram.cpp"
                        } break;
                     default: ;
                     }
#line 963 "grammar.acc"
 fl |= TILE_RANDOM | TILE_MONSTER; 
# line 5604 "yygram.cpp"
                     } break;
                  default: printf("???\n"); exit(1);
                  }
                  switch (yyselect()) {
                  case 819: {
                     get_lexval();
                     res_ref(&rr2);
#line 964 "grammar.acc"
 xID2 = rr2; 
# line 5614 "yygram.cpp"
                     } break;
                  case 820: {
                     get_lexval();
                     get_lexval();
                     cexpr(&v);
                     get_lexval();
                     cexpr(&v2);
#line 966 "grammar.acc"
 fl |= TILE_RANDOM_EFF; xID2 = v+v2*256; 
# line 5624 "yygram.cpp"
                     } break;
                  case 821: {
                     get_lexval();
                     cexpr(&v);
#line 967 "grammar.acc"
 xID2 = v; 
# line 5631 "yygram.cpp"
                     } break;
                  default: ;
                  }
                  } break;
               case 823: {
                  get_lexval();
                  glyph_desc(&g);
#line 968 "grammar.acc"
 Img = g; 
# line 5641 "yygram.cpp"
                  } break;
               default: goto yy72;
               }
            }
            yy72: ;
#line 969 "grammar.acc"
 theReg->AddTile(ch,Img,fl,tID,xID,xID2); 
# line 5649 "yygram.cpp"
            switch (yyselect()) {
            case 824: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy71;
         }
      }
      yy71: ;
      get_lexval();
      } break;
   case 296: {
      get_lexval();
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 826: {
            region_entry();
            } break;
         default: goto yy73;
         }
      }
      yy73: ;
      } break;
   case 297: {
      get_lexval();
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 827: {
            object_entry();
            } break;
         default: goto yy74;
         }
      }
      yy74: ;
      } break;
   case 298: {
      dconst_entry();
      } break;
   case 299: {
      event_desc();
      } break;
   case 300: {
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
   case 301: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 979 "grammar.acc"
 theRes = theTemp; theTemp->Name = name; CurrFeat = 0; CurrAttk = 0; 
# line 5713 "yygram.cpp"
      get_lexval();
      cexpr3(&ty);
#line 980 "grammar.acc"
 theTemp->TType = ty; 
# line 5718 "yygram.cpp"
      switch (yyselect()) {
      case 828: {
         get_lexval();
         cexpr3(&mt);
#line 981 "grammar.acc"
 theTemp->ForMType = mt; 
# line 5725 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 829: {
            template_entry();
            } break;
         default: goto yy75;
         }
      }
      yy75: ;
      get_lexval();
#line 983 "grammar.acc"
 theTemp++; 
# line 5742 "yygram.cpp"
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
   case 302: {
      get_lexval();
      switch (yyselect()) {
      case 830: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 987 "grammar.acc"
 theTemp->Desc = lv; 
# line 5775 "yygram.cpp"
      } break;
   case 303: {
      get_lexval();
      switch (yyselect()) {
      case 831: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mt);
      get_lexval();
#line 988 "grammar.acc"
 theTemp->ForMType = mt; 
# line 5789 "yygram.cpp"
      } break;
   case 304: {
      get_lexval();
      switch (yyselect()) {
      case 832: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mt);
      get_lexval();
#line 989 "grammar.acc"
 theTemp->AddMType = mt; 
# line 5803 "yygram.cpp"
      } break;
   case 305: {
      glyph_entry(&img);
#line 990 "grammar.acc"
 theTemp->NewImage = img; 
# line 5809 "yygram.cpp"
      } break;
   case 306: {
      switch (yyselect()) {
      case 833: {
         get_lexval();
         } break;
      case 834: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 835: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 991 "grammar.acc"
 theTemp->CR = mv; 
# line 5831 "yygram.cpp"
      } break;
   case 307: {
      get_lexval();
      switch (yyselect()) {
      case 836: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 992 "grammar.acc"
 theTemp->HitDice = mv; 
# line 5845 "yygram.cpp"
      } break;
   case 308: {
      get_lexval();
      switch (yyselect()) {
      case 837: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 993 "grammar.acc"
 theTemp->Hit = mv; 
# line 5859 "yygram.cpp"
      } break;
   case 309: {
      get_lexval();
      switch (yyselect()) {
      case 838: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 994 "grammar.acc"
 theTemp->Def = mv; 
# line 5873 "yygram.cpp"
      } break;
   case 310: {
      get_lexval();
      switch (yyselect()) {
      case 839: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 995 "grammar.acc"
 theTemp->Arm = mv; 
# line 5887 "yygram.cpp"
      } break;
   case 311: {
      get_lexval();
      switch (yyselect()) {
      case 840: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 996 "grammar.acc"
 theTemp->DmgMod = n; 
# line 5901 "yygram.cpp"
      } break;
   case 312: {
      get_lexval();
      switch (yyselect()) {
      case 841: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 997 "grammar.acc"
 theTemp->Power = mv; 
# line 5915 "yygram.cpp"
      } break;
   case 313: {
      get_lexval();
      get_lexval();
      switch (yyselect()) {
      case 842: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 998 "grammar.acc"
 theTemp->CasterLev = mv; 
# line 5930 "yygram.cpp"
      } break;
   case 314: {
      get_lexval();
      switch (yyselect()) {
      case 843: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 999 "grammar.acc"
 theTemp->Size = mv; 
# line 5944 "yygram.cpp"
      } break;
   case 315: {
      get_lexval();
      switch (yyselect()) {
      case 844: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1000 "grammar.acc"
 theTemp->Spd = mv; 
# line 5958 "yygram.cpp"
      } break;
   case 316: {
      get_lexval();
      switch (yyselect()) {
      case 845: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1001 "grammar.acc"
 theTemp->Mov = mv; 
# line 5972 "yygram.cpp"
      } break;
   case 317: {
      get_lexval();
      at = yylval;
      switch (yyselect()) {
      case 846: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      switch (yyselect()) {
      case 847: {
         get_lexval();
         } break;
      case 848: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
#line 1002 "grammar.acc"
 theTemp->Attr[at] = mv; 
# line 5995 "yygram.cpp"
      } break;
   case 318: {
      get_lexval();
      switch (yyselect()) {
      case 849: {
         get_lexval();
         } break;
      default: ;
      }
      temp_attack_entry();
      while (1) {
         switch (yyselect()) {
         case 850: {
            get_lexval();
            temp_attack_entry();
            } break;
         default: goto yy76;
         }
      }
      yy76: ;
      get_lexval();
      } break;
   case 319: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      get_lexval();
#line 1006 "grammar.acc"
 if (!theTemp->GainStati(st)) yyerror("too many Stati in template"); 
# line 6026 "yygram.cpp"
      } break;
   case 320: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      get_lexval();
#line 1008 "grammar.acc"
 if (!theTemp->GainStati(st,stV)) yyerror("too many Stati in template"); 
# line 6038 "yygram.cpp"
      } break;
   case 321: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      cexpr3(&stM);
      get_lexval();
      get_lexval();
#line 1010 "grammar.acc"
 if (!theTemp->GainStati(st,stV,stM)) 
        yyerror("too many Stati in template"); 
# line 6053 "yygram.cpp"
      } break;
   case 322: {
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
#line 1013 "grammar.acc"
 if (!theTemp->GainStati(st,stV,stM,rr)) 
        yyerror("too many Stati in template"); 
# line 6070 "yygram.cpp"
      } break;
   case 323: {
      get_lexval();
      switch (yyselect()) {
      case 851: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 857: {
            switch (yyselect()) {
            case 852: {
               cexpr3(&res);
#line 1016 "grammar.acc"
 theTemp->AddRes |= res; 
# line 6088 "yygram.cpp"
               } break;
            case 855: {
               switch (yyselect()) {
               case 853: {
                  get_lexval();
                  } break;
               case 854: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&res);
#line 1017 "grammar.acc"
 theTemp->SubRes |= res; 
# line 6103 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 856: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy77;
         }
      }
      yy77: ;
      get_lexval();
      } break;
   case 324: {
      get_lexval();
      switch (yyselect()) {
      case 858: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 864: {
            switch (yyselect()) {
            case 859: {
               cexpr3(&res);
#line 1019 "grammar.acc"
 theTemp->AddImm |= res; 
# line 6136 "yygram.cpp"
               } break;
            case 862: {
               switch (yyselect()) {
               case 860: {
                  get_lexval();
                  } break;
               case 861: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&res);
#line 1020 "grammar.acc"
 theTemp->SubImm |= res; 
# line 6151 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 863: {
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
   case 325: {
      get_lexval();
      switch (yyselect()) {
      case 865: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 866: {
            cexpr3(&n);
            get_lexval();
#line 1022 "grammar.acc"
 theTemp->NewFeats[CurrFeat++] = n; 
# line 6183 "yygram.cpp"
            } break;
         default: goto yy79;
         }
      }
      yy79: ;
      cexpr3(&n);
      get_lexval();
#line 1023 "grammar.acc"
 theTemp->NewFeats[CurrFeat++] = n; 
# line 6193 "yygram.cpp"
      } break;
   case 326: {
      get_lexval();
      switch (yyselect()) {
      case 867: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 869: {
            res_ref(&rr);
#line 1024 "grammar.acc"
 theTemp->AddsFlag(M_INNATE);
                                       theTemp->AddResID(AN_INNATE, rr); 
# line 6210 "yygram.cpp"
            switch (yyselect()) {
            case 868: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy80;
         }
      }
      yy80: ;
      get_lexval();
      } break;
   case 327: {
      get_lexval();
      switch (yyselect()) {
      case 870: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 872: {
            res_ref(&rr);
#line 1027 "grammar.acc"
 theTemp->AddResID(AN_DISEASE, rr); 
# line 6238 "yygram.cpp"
            switch (yyselect()) {
            case 871: {
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
   case 328: {
      get_lexval();
      switch (yyselect()) {
      case 873: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 875: {
            res_ref(&rr);
#line 1029 "grammar.acc"
 theTemp->AddResID(AN_POISON, rr); 
# line 6266 "yygram.cpp"
            switch (yyselect()) {
            case 874: {
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
   case 329: {
      get_lexval();
      switch (yyselect()) {
      case 876: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 883: {
            switch (yyselect()) {
            case 877: {
               cexpr3(&fl);
#line 1032 "grammar.acc"
 if (fl < 0) 
                                         theTemp->Flags[(-fl)/8] |= 1 << ((-fl)%8);
                                       else
                                         theTemp->AddFlags[fl/8] |= 1 << (fl % 8); 
                                     
# line 6300 "yygram.cpp"
               } break;
            case 880: {
               switch (yyselect()) {
               case 878: {
                  get_lexval();
                  } break;
               case 879: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&fl);
#line 1037 "grammar.acc"
 theTemp->SubFlags[fl/8] |= 1 << (fl % 8); 
# line 6315 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 881: {
               get_lexval();
               } break;
            case 882: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy83;
         }
      }
      yy83: ;
      } break;
   case 330: {
      gear_desc();
      } break;
   case 331: {
      event_desc();
      } break;
   case 332: {
      dconst_entry();
      } break;
   case 333: {
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
   case 334: {
#line 1045 "grammar.acc"
 uint8 dt = AD_NORM; dam.Set(0,0,0);
                                 theTemp->NewAttk[CurrAttk].u.a.Dmg.Set(0,0,0);
# line 6365 "yygram.cpp"
      cexpr3(&at);
      get_lexval();
      switch (yyselect()) {
      case 884: {
         dice_only(&dam);
#line 1048 "grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam); 
# line 6373 "yygram.cpp"
         } break;
      case 885: {
         cexpr3(&chance);
         get_lexval();
#line 1049 "grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg.Bonus = chance; 
# line 6380 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 886: {
         cexpr3(&n);
#line 1050 "grammar.acc"
 dt = n; 
# line 6389 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 887: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 891: {
         get_lexval();
         switch (yyselect()) {
         case 888: {
            get_lexval();
            } break;
         case 889: {
            get_lexval();
            } break;
         case 890: {
            get_lexval();
            } break;
         default: printf("???\n"); exit(1);
         }
         cexpr3(&dc);
#line 1053 "grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.DC = dc; 
# line 6417 "yygram.cpp"
         get_lexval();
         } break;
      default: ;
      }
#line 1054 "grammar.acc"
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
                               
# line 6437 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 899: {
            get_lexval();
#line 1064 "grammar.acc"
 dt = AD_NORM; theTemp->NewAttk[CurrAttk].u.a.Dmg.Set(0,0,0); dam2.Set(0,0,0); 
# line 6444 "yygram.cpp"
            switch (yyselect()) {
            case 892: {
               dice_only(&dam2);
#line 1065 "grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam2); 
# line 6450 "yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 893: {
               cexpr3(&n2);
#line 1066 "grammar.acc"
 dt = n2; 
# line 6459 "yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 894: {
               get_lexval();
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 898: {
               get_lexval();
               switch (yyselect()) {
               case 895: {
                  get_lexval();
                  } break;
               case 896: {
                  get_lexval();
                  } break;
               case 897: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&dc);
#line 1068 "grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.DC = dc; 
# line 6487 "yygram.cpp"
               get_lexval();
               } break;
            default: ;
            }
#line 1069 "grammar.acc"
 theTemp->NewAttk[CurrAttk].AType = A_ALSO;
                                 theTemp->NewAttk[CurrAttk].DType = dt;
                                 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam2);
                                 CurrAttk++;
                                 if (CurrAttk >= 16)
                                   Fatal("> 16 attacks on a single monster!");
                               
# line 6500 "yygram.cpp"
            } break;
         default: goto yy84;
         }
      }
      yy84: ;
      } break;
   case 335: {
      cexpr3(&at);
      get_lexval();
      res_ref(&rr);
#line 1077 "grammar.acc"
 theTemp->NewAttk[CurrAttk].AType = at;
                                  theTemp->NewAttk[CurrAttk].u.xID = rr; 
# line 6514 "yygram.cpp"
      switch (yyselect()) {
      case 900: {
         cexpr3(&dt);
#line 1079 "grammar.acc"
 theTemp->NewAttk[CurrAttk].DType = dt; 
# line 6520 "yygram.cpp"
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
   case 336: {
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
   case 337: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
      cexpr3(&it);
#line 1086 "grammar.acc"
 theRes = theFlavor; 
                                         theFlavor->Name = name;
                                         theFlavor->IType = it;
                                         theFlavor->Material = -1;
                                         theFlavor->Color = -1;
                                         theFlavor->Weight = 20; 
# line 6555 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 901: {
            flavor_entry();
            } break;
         default: goto yy85;
         }
      }
      yy85: ;
      get_lexval();
#line 1092 "grammar.acc"
 theFlavor++; 
# line 6569 "yygram.cpp"
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
   case 338: {
      get_lexval();
      switch (yyselect()) {
      case 902: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 1096 "grammar.acc"
 theFlavor->Desc     = lv; 
# line 6593 "yygram.cpp"
      } break;
   case 339: {
      get_lexval();
      switch (yyselect()) {
      case 903: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 1097 "grammar.acc"
 theFlavor->Material = n; 
# line 6607 "yygram.cpp"
      } break;
   case 340: {
      get_lexval();
      switch (yyselect()) {
      case 904: {
         get_lexval();
         } break;
      default: ;
      }
      color(&col);
      get_lexval();
#line 1098 "grammar.acc"
 theFlavor->Color    = col; 
# line 6621 "yygram.cpp"
      } break;
   case 341: {
      get_lexval();
      switch (yyselect()) {
      case 905: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
      get_lexval();
#line 1099 "grammar.acc"
 theFlavor->Weight   = n/5; 
# line 6636 "yygram.cpp"
      } break;
   }
return 0;
}

int prototype ()
{
   switch(yyselect()) {
   case 342: {
      get_lexval();
      } break;
   }
return 0;
}

int type_name (   YYSTYPE *ty)
{
   switch(yyselect()) {
   case 343: {
      get_lexval();
#line 1105 "grammar.acc"
 *ty = DT_VOID; 
# line 6657 "yygram.cpp"
      } break;
   case 344: {
      get_lexval();
#line 1106 "grammar.acc"
 *ty = DT_INT32; 
# line 6663 "yygram.cpp"
      } break;
   case 345: {
      get_lexval();
#line 1107 "grammar.acc"
 *ty = DT_HOBJ; 
# line 6669 "yygram.cpp"
      } break;
   case 346: {
      get_lexval();
#line 1108 "grammar.acc"
 *ty = DT_HTEXT; 
# line 6675 "yygram.cpp"
      } break;
   case 347: {
      get_lexval();
#line 1109 "grammar.acc"
 *ty = DT_RID; 
# line 6681 "yygram.cpp"
      } break;
   case 348: {
      get_lexval();
#line 1110 "grammar.acc"
 *ty = DT_STRING; 
# line 6687 "yygram.cpp"
      } break;
   case 349: {
      get_lexval();
#line 1111 "grammar.acc"
 *ty = DT_RECT; 
# line 6693 "yygram.cpp"
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
   case 350: {
#line 1115 "grammar.acc"
 BSysFunc *b; 
# line 6713 "yygram.cpp"
      get_lexval();
      type_name(&ty);
      cexpr3(&ot);
      get_lexval();
      get_lexval();
      fn = yylval;
      get_lexval();
#line 1117 "grammar.acc"
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
    
# line 6739 "yygram.cpp"
      switch (yyselect()) {
      case 913: {
         type_name(&ty_1);
         switch (yyselect()) {
         case 906: {
            get_lexval();
            cexpr3(&ot_1);
#line 1133 "grammar.acc"
 b->ParamOType[0] = ot_1; 
# line 6749 "yygram.cpp"
            } break;
         default: ;
         }
#line 1134 "grammar.acc"
 b->ParamType[0] = ty_1; b->ParamCount++; 
# line 6755 "yygram.cpp"
         switch (yyselect()) {
         case 907: {
            get_lexval();
            paramname = yylval;
            } break;
         default: ;
         }
         switch (yyselect()) {
         case 908: {
            get_lexval();
            cexpr(&def);
#line 1135 "grammar.acc"
 b->Default[0] = def; b->HasDefault |= BIT(2); 
# line 6769 "yygram.cpp"
            } break;
         default: ;
         }
         while (1) {
            switch (yyselect()) {
            case 912: {
               get_lexval();
               type_name(&ty_n);
               switch (yyselect()) {
               case 909: {
                  get_lexval();
                  cexpr3(&ot_n);
#line 1137 "grammar.acc"
 b->ParamOType[b->ParamCount] = ot_n; 
# line 6784 "yygram.cpp"
                  } break;
               default: ;
               }
#line 1138 "grammar.acc"
 b->ParamType[b->ParamCount] = ty_n; b->ParamCount++; 
# line 6790 "yygram.cpp"
               switch (yyselect()) {
               case 910: {
                  get_lexval();
                  paramname = yylval;
                  } break;
               default: ;
               }
               switch (yyselect()) {
               case 911: {
                  get_lexval();
                  cexpr(&def);
#line 1139 "grammar.acc"
 b->Default[b->ParamCount] = def; b->HasDefault |= BIT(1+b->ParamCount); 
# line 6804 "yygram.cpp"
                  } break;
               default: ;
               }
               } break;
            default: goto yy86;
            }
         }
         yy86: ;
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 914: {
         get_lexval();
         get_lexval();
#line 1141 "grammar.acc"
 b->isVarParam = true; 
# line 6822 "yygram.cpp"
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
   case 351: {
#line 1144 "grammar.acc"
 BSysFunc *b; 
# line 6846 "yygram.cpp"
      get_lexval();
      type_name(&ty);
      get_lexval();
      fn = yylval;
      get_lexval();
#line 1146 "grammar.acc"
 if (theSymTab.GetBinding(fn,0,0,SYS_FUNC))
        yyerror(Format("System global function '%s' declared twice!",theSymTab[fn]));
      else if (theSymTab.GetBinding(fn,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[fn]));
      b = new BSysFunc(MemFuncID++,ty);
      theSymTab.Bind(fn,b); 
# line 6859 "yygram.cpp"
      switch (yyselect()) {
      case 922: {
         type_name(&ty_1);
         switch (yyselect()) {
         case 915: {
            get_lexval();
            cexpr3(&ot_1);
#line 1152 "grammar.acc"
 b->ParamOType[0] = ot_1; 
# line 6869 "yygram.cpp"
            } break;
         default: ;
         }
#line 1153 "grammar.acc"
 b->ParamType[0] = ty_1; b->ParamCount++; 
# line 6875 "yygram.cpp"
         switch (yyselect()) {
         case 916: {
            get_lexval();
            paramname = yylval;
            } break;
         default: ;
         }
         switch (yyselect()) {
         case 917: {
            get_lexval();
            cexpr(&def);
#line 1154 "grammar.acc"
 b->Default[0] = def; b->HasDefault |= BIT(2); 
# line 6889 "yygram.cpp"
            } break;
         default: ;
         }
         while (1) {
            switch (yyselect()) {
            case 921: {
               get_lexval();
               type_name(&ty_n);
               switch (yyselect()) {
               case 918: {
                  get_lexval();
                  cexpr3(&ot_n);
#line 1156 "grammar.acc"
 b->ParamOType[b->ParamCount] = ot_n; 
# line 6904 "yygram.cpp"
                  } break;
               default: ;
               }
#line 1157 "grammar.acc"
 b->ParamType[b->ParamCount] = ty_n; b->ParamCount++; 
# line 6910 "yygram.cpp"
               switch (yyselect()) {
               case 919: {
                  get_lexval();
                  paramname = yylval;
                  } break;
               default: ;
               }
               switch (yyselect()) {
               case 920: {
                  get_lexval();
                  cexpr(&def);
#line 1158 "grammar.acc"
 b->Default[b->ParamCount] = def; b->HasDefault |= BIT(1+b->ParamCount); 
# line 6924 "yygram.cpp"
                  } break;
               default: ;
               }
               } break;
            default: goto yy87;
            }
         }
         yy87: ;
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 923: {
         get_lexval();
         get_lexval();
#line 1160 "grammar.acc"
 b->isVarParam = true; 
# line 6942 "yygram.cpp"
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
   case 352: {
#line 1163 "grammar.acc"
 BMemVar *b; BResMem *b2; 
# line 6961 "yygram.cpp"
      get_lexval();
      type_name(&ty);
      cexpr3(&ot);
      get_lexval();
      get_lexval();
      vn = yylval;
      get_lexval();
#line 1165 "grammar.acc"
 
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
    
# line 6999 "yygram.cpp"
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
   case 353: {
#line 1196 "grammar.acc"
 BSysObj *b; 
# line 7013 "yygram.cpp"
      get_lexval();
      cexpr3(&ot);
      get_lexval();
      oname = yylval;
      get_lexval();
      cexpr3(&oid);
      get_lexval();
#line 1198 "grammar.acc"
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
# line 7033 "yygram.cpp"
      } break;
   }
return 0;
}

int g_declaration ()
{
   YYSTYPE ty;
   YYSTYPE vn;
   switch(yyselect()) {
   case 354: {
#line 1212 "grammar.acc"
 BGlobalVar *b; 
# line 7046 "yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1214 "grammar.acc"
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
# line 7065 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 924: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1229 "grammar.acc"
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
# line 7084 "yygram.cpp"
            } break;
         default: goto yy88;
         }
      }
      yy88: ;
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
   case 355: {
#line 1242 "grammar.acc"
 BLocalVar *b; int32 scope, i; String scope_id; 
# line 7103 "yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1244 "grammar.acc"
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
# line 7124 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 925: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1261 "grammar.acc"
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
# line 7148 "yygram.cpp"
            } break;
         default: goto yy89;
         }
      }
      yy89: ;
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
   case 356: {
#line 1279 "grammar.acc"
 BResVar *b; int32 scope; String scope_id; 
# line 7167 "yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1281 "grammar.acc"
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
# line 7183 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 926: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1293 "grammar.acc"
 b = new BResVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      b->Event = 0;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,RES_VAR))
        Error("Resource variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = ty; 
      b->Address = HeapHead++;
      b->type    = RES_VAR;
      theSymTab.Bind(vn,b); 
# line 7200 "yygram.cpp"
            } break;
         default: goto yy90;
         }
      }
      yy90: ;
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
   case 357: {
      expr(&ex);
      get_lexval();
#line 1307 "grammar.acc"

      st->Add(ex.Code);
      if (ex.Storage == RT_REGISTER) {
        st->TrimGratuitousMOV();
        FreeRegister(ex.Value);
        }
      if (ex.Type == DT_STRING && ex.Storage == RT_CONSTANT)
        if (ex.Value < -1)
          FreeString(-ex.Value);
    
# line 7238 "yygram.cpp"
      } break;
   case 358: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
      get_lexval();
      statement(&st2);
#line 1318 "grammar.acc"

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
    
# line 7265 "yygram.cpp"
      } break;
   case 359: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
#line 1333 "grammar.acc"

      st->Add(ex.Code);
      st->Generate(JFAL, ex.Storage, ex.Value, 
        RT_CONSTANT, st1.GetSize()+1);
      if (ex.Storage == RT_REGISTER)
        FreeRegister(ex.Value);
      st->Add(&st1);
    
# line 7282 "yygram.cpp"
      } break;
   case 360: {
      get_lexval();
      get_lexval();
      expr(&ex1);
      get_lexval();
      expr(&ex2);
      get_lexval();
      expr(&ex3);
      get_lexval();
      statement(&st1);
#line 1343 "grammar.acc"

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
    
# line 7322 "yygram.cpp"
      } break;
   case 361: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      get_lexval();
#line 1369 "grammar.acc"

        int16 CaseAddr[256], DefaultAddr;
        int32 CaseVal[256];
        VBlock *CaseCode[256], *DefaultCode;
        int8 CaseCount, i;
        CaseCount = 0;
        DefaultAddr = 0;
        CaseAddr[0] = 0; 
      
# line 7340 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 930: {
            get_lexval();
            switch (yyselect()) {
            case 927: {
               cexpr(&val);
               } break;
            case 928: {
               res_ref(&rr);
#line 1378 "grammar.acc"
 val = rr; 
# line 7353 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            get_lexval();
#line 1379 "grammar.acc"
 CaseCode[CaseCount] = new VBlock; 
# line 7360 "yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 929: {
#line 1380 "grammar.acc"
 st_n.Clear(); 
# line 7366 "yygram.cpp"
                  statement(&st_n);
#line 1380 "grammar.acc"
 CaseCode[CaseCount]->Add(&st_n);  
# line 7370 "yygram.cpp"
                  } break;
               default: goto yy92;
               }
            }
            yy92: ;
#line 1381 "grammar.acc"

        /* We know where *this* case ends, which is synonimous with where

           the *next* case starts... */
        CaseAddr[CaseCount + 1] = CaseAddr[CaseCount] +
          CaseCode[CaseCount]->GetSize();
        CaseVal[CaseCount] = val;
        CaseCount++;  
      
# line 7386 "yygram.cpp"
            } break;
         default: goto yy91;
         }
      }
      yy91: ;
      switch (yyselect()) {
      case 932: {
         get_lexval();
         get_lexval();
#line 1390 "grammar.acc"
 DefaultCode = new VBlock; 
# line 7398 "yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 931: {
#line 1391 "grammar.acc"
 st_n.Clear(); 
# line 7404 "yygram.cpp"
               statement(&st_n);
#line 1391 "grammar.acc"
 DefaultCode->Add(&st_n);  
# line 7408 "yygram.cpp"
               } break;
            default: goto yy93;
            }
         }
         yy93: ;
#line 1392 "grammar.acc"
 
        DefaultAddr = CaseAddr[CaseCount]; 
        CaseAddr[CaseCount] = DefaultAddr + DefaultCode->GetSize(); 
      
# line 7419 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
#line 1399 "grammar.acc"

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
        
      
# line 7465 "yygram.cpp"
      } break;
   case 362: {
      get_lexval();
      get_lexval();
#line 1440 "grammar.acc"

        st->Generate(JBRK);
      
# line 7474 "yygram.cpp"
      } break;
   case 363: {
      get_lexval();
      get_lexval();
#line 1444 "grammar.acc"

        st->Generate(CONT);
      
# line 7483 "yygram.cpp"
      } break;
   case 364: {
      get_lexval();
      statement(&st1);
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      get_lexval();
#line 1448 "grammar.acc"

        st->Add(&st1);
        st->Add(ex.Code);
        st->Generate(JTRU,ex.Storage,ex.Value,
          RT_CONSTANT, -st->GetSize());
        if (ex.Storage == RT_REGISTER)
          FreeRegister(ex.Value);
      
# line 7502 "yygram.cpp"
      } break;
   case 365: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
#line 1457 "grammar.acc"

        st->Add(ex.Code);
        st->Generate(JFAL,ex.Storage,ex.Value,
          RT_CONSTANT, st1.GetSize()+
            (st->GetSize() > 511 ? 3 : 2));
        st->Add(&st1);
        st->Generate(JUMP,RT_CONSTANT, - st->GetSize());
        if (ex.Storage == RT_REGISTER)
          FreeRegister(ex.Value);
      
# line 7521 "yygram.cpp"
      } break;
   case 366: {
      block(&bl);
#line 1468 "grammar.acc"
 
        st->Add(&bl); 
      
# line 7529 "yygram.cpp"
      } break;
   case 367: {
      get_lexval();
      expr(&ex);
      get_lexval();
#line 1472 "grammar.acc"

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
      
# line 7548 "yygram.cpp"
      } break;
   case 368: {
      get_lexval();
      get_lexval();
#line 1485 "grammar.acc"

        if (StackHead)
          st->Generate(DEC,RT_REGISTER,63,RT_CONSTANT,StackHead);
        st->Generate(RET);
      
# line 7559 "yygram.cpp"
      } break;
   }
return 0;
}

int block (   VBlock *bl)
{
   VBlock st;
   switch(yyselect()) {
   case 369: {
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 933: {
#line 1493 "grammar.acc"
 st.Clear(); 
# line 7575 "yygram.cpp"
            statement(&st);
#line 1493 "grammar.acc"
 bl->Add(&st); 
# line 7579 "yygram.cpp"
            } break;
         default: goto yy94;
         }
      }
      yy94: ;
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
   case 370: {
#line 1497 "grammar.acc"
 VBlock decl_code; StackHead = 0; ClearStrings(); 
# line 7598 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 934: {
#line 1498 "grammar.acc"
 code.Clear(); 
# line 7605 "yygram.cpp"
            l_declaration(&code);
#line 1498 "grammar.acc"
 decl_code.Add(&code); 
# line 7609 "yygram.cpp"
            } break;
         case 935: {
            get_lexval();
            r_declaration();
            } break;
         default: goto yy95;
         }
      }
      yy95: ;
#line 1500 "grammar.acc"
 if (StackHead)
          bl->Generate(INC,RT_REGISTER,63,RT_CONSTANT,StackHead);
        bl->Add(&decl_code); 
# line 7623 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 936: {
#line 1503 "grammar.acc"
 st.Clear(); 
# line 7629 "yygram.cpp"
            statement(&st);
#line 1503 "grammar.acc"
 bl->Add(&st); 
# line 7633 "yygram.cpp"
            } break;
         default: goto yy96;
         }
      }
      yy96: ;
      get_lexval();
#line 1504 "grammar.acc"
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
      
# line 7657 "yygram.cpp"
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
   case 371: {
      get_lexval();
      n = yylval;
#line 1540 "grammar.acc"
 ex->Type = DT_INT32; ex->Storage = RT_CONSTANT; ex->Value = n; ex->Code = NULL; 
# line 7681 "yygram.cpp"
      } break;
   case 372: {
      get_lexval();
      n = yylval;
#line 1542 "grammar.acc"
 ex->Type = DT_INT32; ex->Storage = RT_CONSTANT; ex->Value = n; ex->Code = NULL; 
# line 7688 "yygram.cpp"
      } break;
   case 373: {
      get_lexval();
      lv = yylval;
#line 1544 "grammar.acc"
 ex->Type = DT_HTEXT; ex->Storage = RT_CONSTANT; ex->Value = lv; ex->Code = NULL; 
# line 7695 "yygram.cpp"
      } break;
   case 374: {
      get_lexval();
#line 1546 "grammar.acc"
 ex->Type = DT_BOOL; ex->Storage = RT_CONSTANT; ex->Value = 1; ex->Code = NULL; 
# line 7701 "yygram.cpp"
      } break;
   case 375: {
      get_lexval();
#line 1548 "grammar.acc"
 ex->Type = DT_BOOL; ex->Storage = RT_CONSTANT; ex->Value = 0; ex->Code = NULL; 
# line 7707 "yygram.cpp"
      } break;
   case 376: {
      get_lexval();
#line 1550 "grammar.acc"
 ex->Type = DT_UNKNOWN; ex->Storage = RT_CONSTANT; ex->Value = 0; ex->Code = NULL; 
# line 7713 "yygram.cpp"
      } break;
   case 377: {
      res_ref(&rr);
#line 1552 "grammar.acc"
 ex->Type = DT_RID;   ex->Storage = RT_CONSTANT; ex->Value = rr; ex->Code = NULL; 
# line 7719 "yygram.cpp"
      } break;
   case 378: {
      get_lexval();
      expr(&ex2);
      get_lexval();
#line 1554 "grammar.acc"
 ex->Code = ex2.Code; 
      ex->Type = ex2.Type; ex->Storage = ex2.Storage; ex->Value = ex2.Value; 
# line 7728 "yygram.cpp"
      } break;
   case 379: {
      get_lexval();
      id = yylval;
#line 1557 "grammar.acc"
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
    
# line 7774 "yygram.cpp"
      } break;
   case 380: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 937: {
         get_lexval();
         } break;
      case 938: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      id = yylval;
#line 1598 "grammar.acc"

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
    
# line 7845 "yygram.cpp"
      } break;
   case 381: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 939: {
         get_lexval();
         } break;
      case 940: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      fn = yylval;
#line 1654 "grammar.acc"
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
      
# line 7887 "yygram.cpp"
      get_lexval();
      param_list(b, fn, &bl, &narg);
      get_lexval();
#line 1681 "grammar.acc"

        if (obj.Code)
          ex->Code->Add(obj.Code);
        ex->Code->Add(&bl);
        /* Adjust any stack offset to account for PUSH opcodes contained in

           bl. */
        if (obj.Storage == RT_MEMORY)
          if (obj.Value < 0)
            obj.Value -= b->ParamCount;


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
      
# line 7920 "yygram.cpp"
      } break;
   case 382: {
      get_lexval();
      fn = yylval;
#line 1709 "grammar.acc"
 BSysFunc *b;
        if (!(b = GETBIND_SYSFUNC(fn)))
          yyerror(Format("Expected: global function name before '(' (%s).",
            theSymTab[fn]));
        ex->Code = new VBlock;
      
# line 7932 "yygram.cpp"
      get_lexval();
      param_list(b, fn, &bl, &narg);
      get_lexval();
#line 1716 "grammar.acc"

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
      
# line 7955 "yygram.cpp"
      } break;
   case 383: {
      fund_expr(&rect);
      get_lexval();
      get_lexval();
      id = yylval;
#line 1735 "grammar.acc"

      if (rect.Type != DT_RECT)
        yyerror("Type mismatch: '@' operator used on non-Rect target.");
      *ex = CodeRectMember(&rect,id);
      
    
# line 7969 "yygram.cpp"
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
   case 384: {
      get_lexval();
      id = yylval;
#line 1745 "grammar.acc"
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
    
# line 8028 "yygram.cpp"
      } break;
   case 385: {
      lvalue(&rect);
      get_lexval();
      get_lexval();
      id = yylval;
#line 1790 "grammar.acc"

      if (rect.Type != DT_RECT)
        yyerror("Type mismatch: '@' operator used on non-Rect target.");
      *lv = CodeRectLVal(&rect,id);
    
# line 8041 "yygram.cpp"
      } break;
   case 386: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 941: {
         get_lexval();
         } break;
      case 942: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      id = yylval;
#line 1796 "grammar.acc"

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
    
# line 8094 "yygram.cpp"
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
   case 387: {
      lvalue(&lv);
      get_lexval();
#line 1838 "grammar.acc"
 *ex = CodeAssignment('i',lv,NULL); 
# line 8110 "yygram.cpp"
      } break;
   case 388: {
      lvalue(&lv);
      get_lexval();
#line 1840 "grammar.acc"
 *ex = CodeAssignment('d',lv,NULL); 
# line 8117 "yygram.cpp"
      } break;
   case 389: {
      get_lexval();
      get_lexval();
      expr150(&ex1);
      get_lexval();
      expr150(&ex2);
      get_lexval();
#line 1842 "grammar.acc"
 *ex = CodeOperator('m',ex1,ex2); 
# line 8128 "yygram.cpp"
      } break;
   case 390: {
      get_lexval();
      get_lexval();
      expr150(&ex1);
      get_lexval();
      expr150(&ex2);
      get_lexval();
#line 1844 "grammar.acc"
 *ex = CodeOperator('M',ex1,ex2); 
# line 8139 "yygram.cpp"
      } break;
   case 391: {
      fund_expr(&ex1);
#line 1846 "grammar.acc"
 *ex = ex1; 
# line 8145 "yygram.cpp"
      } break;
   }
return 0;
}

int expr20 (   PExp *ex)
{
   LExp lv;
   PExp ex1;
   switch(yyselect()) {
   case 392: {
      get_lexval();
      lvalue(&lv);
#line 1851 "grammar.acc"
 *ex = CodeAssignment('I',lv,NULL); 
# line 8160 "yygram.cpp"
      } break;
   case 393: {
      get_lexval();
      lvalue(&lv);
#line 1853 "grammar.acc"
 *ex = CodeAssignment('D',lv,NULL); 
# line 8167 "yygram.cpp"
      } break;
   case 394: {
      get_lexval();
      expr20(&ex1);
#line 1855 "grammar.acc"
 *ex = ex1; 
# line 8174 "yygram.cpp"
      } break;
   case 395: {
      get_lexval();
      expr20(&ex1);
#line 1857 "grammar.acc"
 *ex = CodeOperator('-',ex1,ex1); 
# line 8181 "yygram.cpp"
      } break;
   case 396: {
      get_lexval();
      expr20(&ex1);
#line 1859 "grammar.acc"
 *ex = CodeOperator('!',ex1,ex1); 
# line 8188 "yygram.cpp"
      } break;
   case 397: {
      get_lexval();
      expr20(&ex1);
#line 1861 "grammar.acc"
 *ex = CodeOperator('~',ex1,ex1); 
# line 8195 "yygram.cpp"
      } break;
   case 398: {
      get_lexval();
      expr20(&ex1);
      get_lexval();
#line 1863 "grammar.acc"
 *ex = CodeOperator(':',ex1,ex1); 
# line 8203 "yygram.cpp"
      } break;
   case 399: {
      expr10(&ex1);
#line 1865 "grammar.acc"
 *ex = ex1; 
# line 8209 "yygram.cpp"
      } break;
   }
return 0;
}

int expr30 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 400: {
      expr20(&ex1);
      get_lexval();
      expr20(&ex2);
#line 1870 "grammar.acc"
 *ex = CodeOperator('d',ex1,ex2); 
# line 8225 "yygram.cpp"
      } break;
   case 401: {
      expr20(&ex1);
#line 1872 "grammar.acc"
 *ex = ex1; 
# line 8231 "yygram.cpp"
      } break;
   }
return 0;
}

int expr40 (   PExp *ex)
{
   YYSTYPE tn;
   PExp ex1;
   switch(yyselect()) {
   case 402: {
      get_lexval();
      type_name(&tn);
      get_lexval();
      expr40(&ex1);
#line 1877 "grammar.acc"
 *ex = ex1; ex->Type = tn; 
# line 8248 "yygram.cpp"
      } break;
   case 403: {
      expr30(&ex1);
#line 1879 "grammar.acc"
 *ex = ex1; 
# line 8254 "yygram.cpp"
      } break;
   }
return 0;
}

int expr50 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 404: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 1885 "grammar.acc"
 *ex = CodeOperator('*',ex1,ex2); 
# line 8270 "yygram.cpp"
      } break;
   case 405: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 1887 "grammar.acc"
 *ex = CodeOperator('/',ex1,ex2); 
# line 8278 "yygram.cpp"
      } break;
   case 406: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 1889 "grammar.acc"
 *ex = CodeOperator('%',ex1,ex2); 
# line 8286 "yygram.cpp"
      } break;
   case 407: {
      expr40(&ex1);
#line 1891 "grammar.acc"
 *ex = ex1; 
# line 8292 "yygram.cpp"
      } break;
   }
return 0;
}

int expr60 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 408: {
      expr60(&ex1);
      get_lexval();
      expr50(&ex2);
#line 1896 "grammar.acc"
 *ex = CodeOperator('+',ex1,ex2); 
# line 8308 "yygram.cpp"
      } break;
   case 409: {
      expr60(&ex1);
      get_lexval();
      expr50(&ex2);
#line 1898 "grammar.acc"
 *ex = CodeOperator('-',ex1,ex2); 
# line 8316 "yygram.cpp"
      } break;
   case 410: {
      expr50(&ex1);
#line 1900 "grammar.acc"
 *ex = ex1; 
# line 8322 "yygram.cpp"
      } break;
   }
return 0;
}

int expr70 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 411: {
      expr70(&ex1);
      get_lexval();
      expr60(&ex2);
#line 1905 "grammar.acc"
 *ex = CodeOperator(LSHIFT_OP,ex1,ex2); 
# line 8338 "yygram.cpp"
      } break;
   case 412: {
      expr70(&ex1);
      get_lexval();
      expr60(&ex2);
#line 1907 "grammar.acc"
 *ex = CodeOperator(RSHIFT_OP,ex1,ex2); 
# line 8346 "yygram.cpp"
      } break;
   case 413: {
      expr60(&ex1);
#line 1909 "grammar.acc"
 *ex = ex1; 
# line 8352 "yygram.cpp"
      } break;
   }
return 0;
}

int expr80 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 414: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 1918 "grammar.acc"
 *ex = CodeOperator('<',ex1,ex2); 
# line 8368 "yygram.cpp"
      } break;
   case 415: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 1920 "grammar.acc"
 *ex = CodeOperator('>',ex1,ex2); 
# line 8376 "yygram.cpp"
      } break;
   case 416: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 1922 "grammar.acc"
 *ex = CodeOperator(GE_OP,ex1,ex2); 
# line 8384 "yygram.cpp"
      } break;
   case 417: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 1924 "grammar.acc"
 *ex = CodeOperator(LE_OP,ex1,ex2); 
# line 8392 "yygram.cpp"
      } break;
   case 418: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 1926 "grammar.acc"
 *ex = CodeOperator(EQ_OP,ex1,ex2); 
# line 8400 "yygram.cpp"
      } break;
   case 419: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 1928 "grammar.acc"
 *ex = CodeOperator(NE_OP,ex1,ex2); 
# line 8408 "yygram.cpp"
      } break;
   case 420: {
      expr70(&ex1);
#line 1930 "grammar.acc"
 *ex = ex1; 
# line 8414 "yygram.cpp"
      } break;
   }
return 0;
}

int expr90 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 421: {
      expr90(&ex1);
      get_lexval();
      expr80(&ex2);
#line 1935 "grammar.acc"
 *ex = CodeOperator('&',ex1,ex2); 
# line 8430 "yygram.cpp"
      } break;
   case 422: {
      expr80(&ex1);
#line 1937 "grammar.acc"
 *ex = ex1; 
# line 8436 "yygram.cpp"
      } break;
   }
return 0;
}

int expr100 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 423: {
      expr100(&ex1);
      get_lexval();
      expr90(&ex2);
#line 1942 "grammar.acc"
 *ex = CodeOperator('^',ex1,ex2); 
# line 8452 "yygram.cpp"
      } break;
   case 424: {
      expr90(&ex1);
#line 1944 "grammar.acc"
 *ex = ex1; 
# line 8458 "yygram.cpp"
      } break;
   }
return 0;
}

int expr110 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 425: {
      expr110(&ex1);
      get_lexval();
      expr100(&ex2);
#line 1949 "grammar.acc"
 *ex = CodeOperator('|',ex1,ex2); 
# line 8474 "yygram.cpp"
      } break;
   case 426: {
      expr100(&ex1);
#line 1951 "grammar.acc"
 *ex = ex1; 
# line 8480 "yygram.cpp"
      } break;
   }
return 0;
}

int expr120 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 427: {
      expr120(&ex1);
      get_lexval();
      expr110(&ex2);
#line 1956 "grammar.acc"
 *ex = CodeOperator(AND_OP,ex1,ex2); 
# line 8496 "yygram.cpp"
      } break;
   case 428: {
      expr110(&ex1);
#line 1958 "grammar.acc"
 *ex = ex1; 
# line 8502 "yygram.cpp"
      } break;
   }
return 0;
}

int expr130 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 429: {
      expr130(&ex1);
      get_lexval();
      expr120(&ex2);
#line 1963 "grammar.acc"
 *ex = CodeOperator(OR_OP,ex1,ex2); 
# line 8518 "yygram.cpp"
      } break;
   case 430: {
      expr120(&ex1);
#line 1965 "grammar.acc"
 *ex = ex1; 
# line 8524 "yygram.cpp"
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
   case 431: {
      expr130(&ex1);
      get_lexval();
      expr(&ex2);
      get_lexval();
      expr140(&ex3);
#line 1970 "grammar.acc"

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
    
# line 8591 "yygram.cpp"
      } break;
   case 432: {
      expr130(&ex1);
#line 2020 "grammar.acc"
 *ex = ex1; 
# line 8597 "yygram.cpp"
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
   case 433: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2028 "grammar.acc"
 *ex = CodeAssignment('=',lv,&ex2); 
# line 8614 "yygram.cpp"
      } break;
   case 434: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2030 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_ADD,lv,&ex2); 
# line 8622 "yygram.cpp"
      } break;
   case 435: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2032 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_SUB,lv,&ex2); 
# line 8630 "yygram.cpp"
      } break;
   case 436: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2034 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_MULT,lv,&ex2); 
# line 8638 "yygram.cpp"
      } break;
   case 437: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2036 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_DIV,lv,&ex2); 
# line 8646 "yygram.cpp"
      } break;
   case 438: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2038 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_MOD,lv,&ex2); 
# line 8654 "yygram.cpp"
      } break;
   case 439: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2040 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_AND,lv,&ex2); 
# line 8662 "yygram.cpp"
      } break;
   case 440: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2042 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_OR,lv,&ex2); 
# line 8670 "yygram.cpp"
      } break;
   case 441: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2044 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_LSHIFT,lv,&ex2); 
# line 8678 "yygram.cpp"
      } break;
   case 442: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2046 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_RSHIFT,lv,&ex2); 
# line 8686 "yygram.cpp"
      } break;
   case 443: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2048 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_NEG,lv,&ex2); 
# line 8694 "yygram.cpp"
      } break;
   case 444: {
      expr140(&ex1);
#line 2050 "grammar.acc"
 *ex = ex1; 
# line 8700 "yygram.cpp"
      } break;
   }
return 0;
}

int expr (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 445: {
      expr(&ex1);
      get_lexval();
      expr150(&ex2);
#line 2055 "grammar.acc"

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
    
# line 8727 "yygram.cpp"
      } break;
   case 446: {
      expr150(&ex1);
#line 2068 "grammar.acc"
 *ex = ex1; 
# line 8733 "yygram.cpp"
      } break;
   }
return 0;
}

int param_list (   pBSysFunc b,    YYSTYPE fn,    VBlock *bl,    int *narg)
{
   PExp p;
   switch(yyselect()) {
   case 447: {
#line 2076 "grammar.acc"
 *narg = 0; int16 i,j; bl->Clear();
    int16 PStorage[10]; int32 PValue[10]; 
# line 8746 "yygram.cpp"
      switch (yyselect()) {
      case 944: {
         expr150(&p);
#line 2079 "grammar.acc"

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
    
# line 8777 "yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 943: {
               get_lexval();
               expr150(&p);
#line 2106 "grammar.acc"

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
    
# line 8818 "yygram.cpp"
               } break;
            default: goto yy97;
            }
         }
         yy97: ;
         } break;
      default: ;
      }
#line 2141 "grammar.acc"

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
      
# line 8847 "yygram.cpp"
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
int c_length = 6270;
extern int yygrammar[];
int yygrammar[] = {
0,
/* 1 */ 0,
/* 2 */ 6,
/* 3 */ 50000,
/* 4 */ -1,
/* 5 */ 448,
/* 6 */ 0,
/* 7 */ 10,
/* 8 */ -6,
/* 9 */ 1,
/* 10 */ 15,
/* 11 */ 18,
/* 12 */ 10,
/* 13 */ -10,
/* 14 */ 449,
/* 15 */ 0,
/* 16 */ -10,
/* 17 */ 945,
/* 18 */ 24,
/* 19 */ 50376,
/* 20 */ 50257,
/* 21 */ 50059,
/* 22 */ -18,
/* 23 */ 2,
/* 24 */ 30,
/* 25 */ 50428,
/* 26 */ 215,
/* 27 */ 50059,
/* 28 */ -18,
/* 29 */ 3,
/* 30 */ 36,
/* 31 */ 50348,
/* 32 */ 50257,
/* 33 */ 50059,
/* 34 */ -18,
/* 35 */ 4,
/* 36 */ 40,
/* 37 */ 640,
/* 38 */ -18,
/* 39 */ 5,
/* 40 */ 44,
/* 41 */ 1395,
/* 42 */ -18,
/* 43 */ 6,
/* 44 */ 48,
/* 45 */ 2049,
/* 46 */ -18,
/* 47 */ 7,
/* 48 */ 52,
/* 49 */ 3547,
/* 50 */ -18,
/* 51 */ 8,
/* 52 */ 56,
/* 53 */ 3345,
/* 54 */ -18,
/* 55 */ 9,
/* 56 */ 60,
/* 57 */ 5284,
/* 58 */ -18,
/* 59 */ 10,
/* 60 */ 64,
/* 61 */ 2224,
/* 62 */ -18,
/* 63 */ 11,
/* 64 */ 68,
/* 65 */ 2661,
/* 66 */ -18,
/* 67 */ 12,
/* 68 */ 72,
/* 69 */ 2918,
/* 70 */ -18,
/* 71 */ 13,
/* 72 */ 76,
/* 73 */ 3979,
/* 74 */ -18,
/* 75 */ 14,
/* 76 */ 80,
/* 77 */ 3875,
/* 78 */ -18,
/* 79 */ 15,
/* 80 */ 84,
/* 81 */ 4243,
/* 82 */ -18,
/* 83 */ 16,
/* 84 */ 88,
/* 85 */ 4120,
/* 86 */ -18,
/* 87 */ 17,
/* 88 */ 92,
/* 89 */ 4560,
/* 90 */ -18,
/* 91 */ 18,
/* 92 */ 96,
/* 93 */ 5288,
/* 94 */ -18,
/* 95 */ 19,
/* 96 */ 100,
/* 97 */ 5363,
/* 98 */ -18,
/* 99 */ 20,
/* 100 */ 104,
/* 101 */ 5395,
/* 102 */ -18,
/* 103 */ 21,
/* 104 */ 108,
/* 105 */ 5485,
/* 106 */ -18,
/* 107 */ 22,
/* 108 */ 112,
/* 109 */ 5591,
/* 110 */ -18,
/* 111 */ 23,
/* 112 */ 116,
/* 113 */ 5573,
/* 114 */ -18,
/* 115 */ 24,
/* 116 */ 120,
/* 117 */ 5582,
/* 118 */ -18,
/* 119 */ 25,
/* 120 */ 126,
/* 121 */ 50346,
/* 122 */ 136,
/* 123 */ 165,
/* 124 */ -18,
/* 125 */ 26,
/* 126 */ 0,
/* 127 */ 50446,
/* 128 */ 50257,
/* 129 */ 50123,
/* 130 */ 140,
/* 131 */ 50257,
/* 132 */ 148,
/* 133 */ 50125,
/* 134 */ -18,
/* 135 */ 27,
/* 136 */ 0,
/* 137 */ 50058,
/* 138 */ -136,
/* 139 */ 450,
/* 140 */ 145,
/* 141 */ 50323,
/* 142 */ 50058,
/* 143 */ -140,
/* 144 */ 451,
/* 145 */ 0,
/* 146 */ -140,
/* 147 */ 946,
/* 148 */ 152,
/* 149 */ 50059,
/* 150 */ -148,
/* 151 */ 452,
/* 152 */ 0,
/* 153 */ -148,
/* 154 */ 947,
/* 155 */ 160,
/* 156 */ 50036,
/* 157 */ 50257,
/* 158 */ -155,
/* 159 */ 28,
/* 160 */ 0,
/* 161 */ 50036,
/* 162 */ 215,
/* 163 */ -155,
/* 164 */ 29,
/* 165 */ 171,
/* 166 */ 165,
/* 167 */ 50043,
/* 168 */ 181,
/* 169 */ -165,
/* 170 */ 30,
/* 171 */ 177,
/* 172 */ 165,
/* 173 */ 50045,
/* 174 */ 181,
/* 175 */ -165,
/* 176 */ 31,
/* 177 */ 0,
/* 178 */ 181,
/* 179 */ -165,
/* 180 */ 32,
/* 181 */ 187,
/* 182 */ 181,
/* 183 */ 50042,
/* 184 */ 215,
/* 185 */ -181,
/* 186 */ 33,
/* 187 */ 193,
/* 188 */ 181,
/* 189 */ 50047,
/* 190 */ 215,
/* 191 */ -181,
/* 192 */ 34,
/* 193 */ 199,
/* 194 */ 181,
/* 195 */ 50037,
/* 196 */ 215,
/* 197 */ -181,
/* 198 */ 35,
/* 199 */ 205,
/* 200 */ 181,
/* 201 */ 50124,
/* 202 */ 215,
/* 203 */ -181,
/* 204 */ 36,
/* 205 */ 211,
/* 206 */ 181,
/* 207 */ 50038,
/* 208 */ 215,
/* 209 */ -181,
/* 210 */ 37,
/* 211 */ 0,
/* 212 */ 215,
/* 213 */ -181,
/* 214 */ 38,
/* 215 */ 220,
/* 216 */ 50043,
/* 217 */ 229,
/* 218 */ -215,
/* 219 */ 39,
/* 220 */ 225,
/* 221 */ 50045,
/* 222 */ 229,
/* 223 */ -215,
/* 224 */ 40,
/* 225 */ 0,
/* 226 */ 229,
/* 227 */ -215,
/* 228 */ 41,
/* 229 */ 235,
/* 230 */ 50040,
/* 231 */ 165,
/* 232 */ 50041,
/* 233 */ -229,
/* 234 */ 42,
/* 235 */ 239,
/* 236 */ 50256,
/* 237 */ -229,
/* 238 */ 43,
/* 239 */ 243,
/* 240 */ 50453,
/* 241 */ -229,
/* 242 */ 44,
/* 243 */ 247,
/* 244 */ 50341,
/* 245 */ -229,
/* 246 */ 45,
/* 247 */ 0,
/* 248 */ 50259,
/* 249 */ -229,
/* 250 */ 46,
/* 251 */ 259,
/* 252 */ 229,
/* 253 */ 50100,
/* 254 */ 229,
/* 255 */ 50043,
/* 256 */ 229,
/* 257 */ -251,
/* 258 */ 47,
/* 259 */ 268,
/* 260 */ 229,
/* 261 */ 50100,
/* 262 */ 229,
/* 263 */ 50043,
/* 264 */ 50045,
/* 265 */ 229,
/* 266 */ -251,
/* 267 */ 48,
/* 268 */ 274,
/* 269 */ 229,
/* 270 */ 50100,
/* 271 */ 229,
/* 272 */ -251,
/* 273 */ 49,
/* 274 */ 0,
/* 275 */ 215,
/* 276 */ -251,
/* 277 */ 50,
/* 278 */ 286,
/* 279 */ 229,
/* 280 */ 50100,
/* 281 */ 229,
/* 282 */ 50043,
/* 283 */ 229,
/* 284 */ -278,
/* 285 */ 51,
/* 286 */ 0,
/* 287 */ 229,
/* 288 */ 50100,
/* 289 */ 229,
/* 290 */ -278,
/* 291 */ 52,
/* 292 */ 0,
/* 293 */ 302,
/* 294 */ 312,
/* 295 */ 320,
/* 296 */ 327,
/* 297 */ 338,
/* 298 */ 347,
/* 299 */ 360,
/* 300 */ -292,
/* 301 */ 53,
/* 302 */ 309,
/* 303 */ 50363,
/* 304 */ 50040,
/* 305 */ 580,
/* 306 */ 50041,
/* 307 */ -302,
/* 308 */ 453,
/* 309 */ 0,
/* 310 */ -302,
/* 311 */ 948,
/* 312 */ 317,
/* 313 */ 50256,
/* 314 */ 50037,
/* 315 */ -312,
/* 316 */ 454,
/* 317 */ 0,
/* 318 */ -312,
/* 319 */ 949,
/* 320 */ 324,
/* 321 */ 278,
/* 322 */ -320,
/* 323 */ 455,
/* 324 */ 0,
/* 325 */ -320,
/* 326 */ 950,
/* 327 */ 331,
/* 328 */ 50314,
/* 329 */ -327,
/* 330 */ 456,
/* 331 */ 335,
/* 332 */ 50297,
/* 333 */ -327,
/* 334 */ 457,
/* 335 */ 0,
/* 336 */ -327,
/* 337 */ 951,
/* 338 */ 342,
/* 339 */ 155,
/* 340 */ -338,
/* 341 */ 458,
/* 342 */ 0,
/* 343 */ 50414,
/* 344 */ 215,
/* 345 */ -338,
/* 346 */ 459,
/* 347 */ 352,
/* 348 */ 50043,
/* 349 */ 50256,
/* 350 */ -347,
/* 351 */ 460,
/* 352 */ 357,
/* 353 */ 50045,
/* 354 */ 50256,
/* 355 */ -347,
/* 356 */ 461,
/* 357 */ 0,
/* 358 */ -347,
/* 359 */ 952,
/* 360 */ 368,
/* 361 */ 50466,
/* 362 */ 50091,
/* 363 */ 409,
/* 364 */ 50093,
/* 365 */ 360,
/* 366 */ -360,
/* 367 */ 464,
/* 368 */ 374,
/* 369 */ 50395,
/* 370 */ 155,
/* 371 */ 360,
/* 372 */ -360,
/* 373 */ 465,
/* 374 */ 383,
/* 375 */ 50395,
/* 376 */ 50414,
/* 377 */ 215,
/* 378 */ 50452,
/* 379 */ 215,
/* 380 */ 360,
/* 381 */ -360,
/* 382 */ 466,
/* 383 */ 390,
/* 384 */ 50395,
/* 385 */ 50414,
/* 386 */ 215,
/* 387 */ 360,
/* 388 */ -360,
/* 389 */ 467,
/* 390 */ 396,
/* 391 */ 50395,
/* 392 */ 50414,
/* 393 */ 360,
/* 394 */ -360,
/* 395 */ 468,
/* 396 */ 406,
/* 397 */ 50293,
/* 398 */ 50040,
/* 399 */ 215,
/* 400 */ 50044,
/* 401 */ 215,
/* 402 */ 50041,
/* 403 */ 360,
/* 404 */ -360,
/* 405 */ 469,
/* 406 */ 0,
/* 407 */ -360,
/* 408 */ 955,
/* 409 */ 415,
/* 410 */ 165,
/* 411 */ 418,
/* 412 */ 409,
/* 413 */ -409,
/* 414 */ 463,
/* 415 */ 0,
/* 416 */ -409,
/* 417 */ 954,
/* 418 */ 422,
/* 419 */ 50044,
/* 420 */ -418,
/* 421 */ 462,
/* 422 */ 0,
/* 423 */ -418,
/* 424 */ 953,
/* 425 */ 0,
/* 426 */ 50352,
/* 427 */ 433,
/* 428 */ 437,
/* 429 */ 292,
/* 430 */ 50059,
/* 431 */ -425,
/* 432 */ 54,
/* 433 */ 0,
/* 434 */ 50058,
/* 435 */ -433,
/* 436 */ 470,
/* 437 */ 443,
/* 438 */ 292,
/* 439 */ 446,
/* 440 */ 437,
/* 441 */ -437,
/* 442 */ 473,
/* 443 */ 0,
/* 444 */ -437,
/* 445 */ 956,
/* 446 */ 450,
/* 447 */ 50044,
/* 448 */ -446,
/* 449 */ 471,
/* 450 */ 0,
/* 451 */ 50332,
/* 452 */ -446,
/* 453 */ 472,
/* 454 */ 0,
/* 455 */ 463,
/* 456 */ 471,
/* 457 */ 478,
/* 458 */ 485,
/* 459 */ 493,
/* 460 */ 50059,
/* 461 */ -454,
/* 462 */ 55,
/* 463 */ 467,
/* 464 */ 50364,
/* 465 */ -463,
/* 466 */ 474,
/* 467 */ 0,
/* 468 */ 50353,
/* 469 */ -463,
/* 470 */ 475,
/* 471 */ 475,
/* 472 */ 50058,
/* 473 */ -471,
/* 474 */ 476,
/* 475 */ 0,
/* 476 */ -471,
/* 477 */ 957,
/* 478 */ 482,
/* 479 */ 1211,
/* 480 */ -478,
/* 481 */ 477,
/* 482 */ 0,
/* 483 */ -478,
/* 484 */ 958,
/* 485 */ 489,
/* 486 */ 50259,
/* 487 */ -485,
/* 488 */ 478,
/* 489 */ 0,
/* 490 */ 50256,
/* 491 */ -485,
/* 492 */ 479,
/* 493 */ 498,
/* 494 */ 50396,
/* 495 */ 1211,
/* 496 */ -493,
/* 497 */ 480,
/* 498 */ 0,
/* 499 */ -493,
/* 500 */ 959,
/* 501 */ 0,
/* 502 */ 507,
/* 503 */ 514,
/* 504 */ 522,
/* 505 */ -501,
/* 506 */ 56,
/* 507 */ 511,
/* 508 */ 1211,
/* 509 */ -507,
/* 510 */ 481,
/* 511 */ 0,
/* 512 */ -507,
/* 513 */ 960,
/* 514 */ 518,
/* 515 */ 50259,
/* 516 */ -514,
/* 517 */ 482,
/* 518 */ 0,
/* 519 */ 50256,
/* 520 */ -514,
/* 521 */ 483,
/* 522 */ 527,
/* 523 */ 50396,
/* 524 */ 1211,
/* 525 */ -522,
/* 526 */ 484,
/* 527 */ 0,
/* 528 */ -522,
/* 529 */ 961,
/* 530 */ 0,
/* 531 */ 50396,
/* 532 */ 50338,
/* 533 */ 547,
/* 534 */ 538,
/* 535 */ 50059,
/* 536 */ -530,
/* 537 */ 57,
/* 538 */ 544,
/* 539 */ 50044,
/* 540 */ 547,
/* 541 */ 538,
/* 542 */ -538,
/* 543 */ 485,
/* 544 */ 0,
/* 545 */ -538,
/* 546 */ 962,
/* 547 */ 0,
/* 548 */ 165,
/* 549 */ 553,
/* 550 */ 562,
/* 551 */ -547,
/* 552 */ 58,
/* 553 */ 559,
/* 554 */ 50044,
/* 555 */ 165,
/* 556 */ 553,
/* 557 */ -553,
/* 558 */ 486,
/* 559 */ 0,
/* 560 */ -553,
/* 561 */ 963,
/* 562 */ 567,
/* 563 */ 50257,
/* 564 */ 571,
/* 565 */ -562,
/* 566 */ 488,
/* 567 */ 0,
/* 568 */ 5786,
/* 569 */ -562,
/* 570 */ 489,
/* 571 */ 577,
/* 572 */ 50047,
/* 573 */ 50257,
/* 574 */ 571,
/* 575 */ -571,
/* 576 */ 487,
/* 577 */ 0,
/* 578 */ -571,
/* 579 */ 964,
/* 580 */ 0,
/* 581 */ 6240,
/* 582 */ -580,
/* 583 */ 59,
/* 584 */ 0,
/* 585 */ 589,
/* 586 */ 616,
/* 587 */ -584,
/* 588 */ 60,
/* 589 */ 594,
/* 590 */ 609,
/* 591 */ 229,
/* 592 */ -589,
/* 593 */ 491,
/* 594 */ 599,
/* 595 */ 50045,
/* 596 */ 229,
/* 597 */ -589,
/* 598 */ 492,
/* 599 */ 604,
/* 600 */ 50061,
/* 601 */ 229,
/* 602 */ -589,
/* 603 */ 493,
/* 604 */ 0,
/* 605 */ 215,
/* 606 */ 50037,
/* 607 */ -589,
/* 608 */ 494,
/* 609 */ 613,
/* 610 */ 50043,
/* 611 */ -609,
/* 612 */ 490,
/* 613 */ 0,
/* 614 */ -609,
/* 615 */ 965,
/* 616 */ 622,
/* 617 */ 50040,
/* 618 */ 625,
/* 619 */ 50041,
/* 620 */ -616,
/* 621 */ 498,
/* 622 */ 0,
/* 623 */ -616,
/* 624 */ 966,
/* 625 */ 630,
/* 626 */ 50383,
/* 627 */ 229,
/* 628 */ -625,
/* 629 */ 495,
/* 630 */ 635,
/* 631 */ 50380,
/* 632 */ 229,
/* 633 */ -625,
/* 634 */ 496,
/* 635 */ 0,
/* 636 */ 50387,
/* 637 */ 229,
/* 638 */ -625,
/* 639 */ 497,
/* 640 */ 0,
/* 641 */ 50384,
/* 642 */ 50257,
/* 643 */ 50058,
/* 644 */ 215,
/* 645 */ 651,
/* 646 */ 50123,
/* 647 */ 668,
/* 648 */ 50125,
/* 649 */ -640,
/* 650 */ 61,
/* 651 */ 657,
/* 652 */ 50044,
/* 653 */ 215,
/* 654 */ 660,
/* 655 */ -651,
/* 656 */ 500,
/* 657 */ 0,
/* 658 */ -651,
/* 659 */ 968,
/* 660 */ 665,
/* 661 */ 50044,
/* 662 */ 215,
/* 663 */ -660,
/* 664 */ 499,
/* 665 */ 0,
/* 666 */ -660,
/* 667 */ 967,
/* 668 */ 673,
/* 669 */ 676,
/* 670 */ 668,
/* 671 */ -668,
/* 672 */ 501,
/* 673 */ 0,
/* 674 */ -668,
/* 675 */ 969,
/* 676 */ 683,
/* 677 */ 50323,
/* 678 */ 912,
/* 679 */ 50257,
/* 680 */ 50059,
/* 681 */ -676,
/* 682 */ 62,
/* 683 */ 690,
/* 684 */ 50322,
/* 685 */ 919,
/* 686 */ 215,
/* 687 */ 50059,
/* 688 */ -676,
/* 689 */ 63,
/* 690 */ 697,
/* 691 */ 926,
/* 692 */ 934,
/* 693 */ 215,
/* 694 */ 50059,
/* 695 */ -676,
/* 696 */ 64,
/* 697 */ 706,
/* 698 */ 941,
/* 699 */ 949,
/* 700 */ 215,
/* 701 */ 50047,
/* 702 */ 215,
/* 703 */ 50059,
/* 704 */ -676,
/* 705 */ 65,
/* 706 */ 713,
/* 707 */ 50360,
/* 708 */ 956,
/* 709 */ 165,
/* 710 */ 50059,
/* 711 */ -676,
/* 712 */ 66,
/* 713 */ 720,
/* 714 */ 50361,
/* 715 */ 963,
/* 716 */ 165,
/* 717 */ 50059,
/* 718 */ -676,
/* 719 */ 67,
/* 720 */ 728,
/* 721 */ 970,
/* 722 */ 50362,
/* 723 */ 977,
/* 724 */ 165,
/* 725 */ 50059,
/* 726 */ -676,
/* 727 */ 68,
/* 728 */ 732,
/* 729 */ 454,
/* 730 */ -676,
/* 731 */ 69,
/* 732 */ 740,
/* 733 */ 50435,
/* 734 */ 50091,
/* 735 */ 215,
/* 736 */ 50093,
/* 737 */ 50059,
/* 738 */ -676,
/* 739 */ 70,
/* 740 */ 750,
/* 741 */ 50435,
/* 742 */ 50091,
/* 743 */ 215,
/* 744 */ 50044,
/* 745 */ 215,
/* 746 */ 50093,
/* 747 */ 50059,
/* 748 */ -676,
/* 749 */ 71,
/* 750 */ 762,
/* 751 */ 50435,
/* 752 */ 50091,
/* 753 */ 215,
/* 754 */ 50044,
/* 755 */ 215,
/* 756 */ 50044,
/* 757 */ 215,
/* 758 */ 50093,
/* 759 */ 50059,
/* 760 */ -676,
/* 761 */ 72,
/* 762 */ 776,
/* 763 */ 50435,
/* 764 */ 50091,
/* 765 */ 215,
/* 766 */ 50044,
/* 767 */ 215,
/* 768 */ 50044,
/* 769 */ 215,
/* 770 */ 50044,
/* 771 */ 155,
/* 772 */ 50093,
/* 773 */ 50059,
/* 774 */ -676,
/* 775 */ 73,
/* 776 */ 783,
/* 777 */ 50419,
/* 778 */ 984,
/* 779 */ 991,
/* 780 */ 50059,
/* 781 */ -676,
/* 782 */ 74,
/* 783 */ 790,
/* 784 */ 50365,
/* 785 */ 1007,
/* 786 */ 1014,
/* 787 */ 50059,
/* 788 */ -676,
/* 789 */ 75,
/* 790 */ 798,
/* 791 */ 50294,
/* 792 */ 1030,
/* 793 */ 1230,
/* 794 */ 1037,
/* 795 */ 50059,
/* 796 */ -676,
/* 797 */ 76,
/* 798 */ 804,
/* 799 */ 50292,
/* 800 */ 50257,
/* 801 */ 50059,
/* 802 */ -676,
/* 803 */ 77,
/* 804 */ 810,
/* 805 */ 50292,
/* 806 */ 215,
/* 807 */ 50059,
/* 808 */ -676,
/* 809 */ 78,
/* 810 */ 817,
/* 811 */ 50320,
/* 812 */ 1046,
/* 813 */ 165,
/* 814 */ 50059,
/* 815 */ -676,
/* 816 */ 79,
/* 817 */ 824,
/* 818 */ 50290,
/* 819 */ 1053,
/* 820 */ 165,
/* 821 */ 50059,
/* 822 */ -676,
/* 823 */ 80,
/* 824 */ 831,
/* 825 */ 50377,
/* 826 */ 1060,
/* 827 */ 165,
/* 828 */ 50059,
/* 829 */ -676,
/* 830 */ 81,
/* 831 */ 839,
/* 832 */ 50342,
/* 833 */ 1067,
/* 834 */ 1074,
/* 835 */ 215,
/* 836 */ 50059,
/* 837 */ -676,
/* 838 */ 82,
/* 839 */ 846,
/* 840 */ 50346,
/* 841 */ 1083,
/* 842 */ 1090,
/* 843 */ 50059,
/* 844 */ -676,
/* 845 */ 83,
/* 846 */ 852,
/* 847 */ 1382,
/* 848 */ 1098,
/* 849 */ 50059,
/* 850 */ -676,
/* 851 */ 84,
/* 852 */ 859,
/* 853 */ 50426,
/* 854 */ 1107,
/* 855 */ 215,
/* 856 */ 50059,
/* 857 */ -676,
/* 858 */ 85,
/* 859 */ 867,
/* 860 */ 50430,
/* 861 */ 1114,
/* 862 */ 215,
/* 863 */ 1121,
/* 864 */ 50059,
/* 865 */ -676,
/* 866 */ 86,
/* 867 */ 875,
/* 868 */ 50385,
/* 869 */ 1128,
/* 870 */ 215,
/* 871 */ 1135,
/* 872 */ 50059,
/* 873 */ -676,
/* 874 */ 87,
/* 875 */ 882,
/* 876 */ 50432,
/* 877 */ 1142,
/* 878 */ 1149,
/* 879 */ 50059,
/* 880 */ -676,
/* 881 */ 88,
/* 882 */ 889,
/* 883 */ 50327,
/* 884 */ 1165,
/* 885 */ 1172,
/* 886 */ 50059,
/* 887 */ -676,
/* 888 */ 89,
/* 889 */ 896,
/* 890 */ 50404,
/* 891 */ 1188,
/* 892 */ 1195,
/* 893 */ 50059,
/* 894 */ -676,
/* 895 */ 90,
/* 896 */ 900,
/* 897 */ 425,
/* 898 */ -676,
/* 899 */ 91,
/* 900 */ 904,
/* 901 */ 530,
/* 902 */ -676,
/* 903 */ 92,
/* 904 */ 908,
/* 905 */ 3387,
/* 906 */ -676,
/* 907 */ 93,
/* 908 */ 0,
/* 909 */ 5623,
/* 910 */ -676,
/* 911 */ 94,
/* 912 */ 916,
/* 913 */ 50058,
/* 914 */ -912,
/* 915 */ 502,
/* 916 */ 0,
/* 917 */ -912,
/* 918 */ 970,
/* 919 */ 923,
/* 920 */ 50058,
/* 921 */ -919,
/* 922 */ 503,
/* 923 */ 0,
/* 924 */ -919,
/* 925 */ 971,
/* 926 */ 930,
/* 927 */ 50370,
/* 928 */ -926,
/* 929 */ 504,
/* 930 */ 0,
/* 931 */ 50304,
/* 932 */ -926,
/* 933 */ 505,
/* 934 */ 938,
/* 935 */ 50058,
/* 936 */ -934,
/* 937 */ 506,
/* 938 */ 0,
/* 939 */ -934,
/* 940 */ 972,
/* 941 */ 945,
/* 942 */ 50370,
/* 943 */ -941,
/* 944 */ 507,
/* 945 */ 0,
/* 946 */ 50304,
/* 947 */ -941,
/* 948 */ 508,
/* 949 */ 953,
/* 950 */ 50058,
/* 951 */ -949,
/* 952 */ 509,
/* 953 */ 0,
/* 954 */ -949,
/* 955 */ 973,
/* 956 */ 960,
/* 957 */ 50058,
/* 958 */ -956,
/* 959 */ 510,
/* 960 */ 0,
/* 961 */ -956,
/* 962 */ 974,
/* 963 */ 967,
/* 964 */ 50058,
/* 965 */ -963,
/* 966 */ 511,
/* 967 */ 0,
/* 968 */ -963,
/* 969 */ 975,
/* 970 */ 974,
/* 971 */ 50043,
/* 972 */ -970,
/* 973 */ 512,
/* 974 */ 0,
/* 975 */ -970,
/* 976 */ 976,
/* 977 */ 981,
/* 978 */ 50058,
/* 979 */ -977,
/* 980 */ 513,
/* 981 */ 0,
/* 982 */ -977,
/* 983 */ 977,
/* 984 */ 988,
/* 985 */ 50058,
/* 986 */ -984,
/* 987 */ 514,
/* 988 */ 0,
/* 989 */ -984,
/* 990 */ 978,
/* 991 */ 997,
/* 992 */ 215,
/* 993 */ 1000,
/* 994 */ 991,
/* 995 */ -991,
/* 996 */ 516,
/* 997 */ 0,
/* 998 */ -991,
/* 999 */ 980,
/* 1000 */ 1004,
/* 1001 */ 50044,
/* 1002 */ -1000,
/* 1003 */ 515,
/* 1004 */ 0,
/* 1005 */ -1000,
/* 1006 */ 979,
/* 1007 */ 1011,
/* 1008 */ 50058,
/* 1009 */ -1007,
/* 1010 */ 517,
/* 1011 */ 0,
/* 1012 */ -1007,
/* 1013 */ 981,
/* 1014 */ 1020,
/* 1015 */ 215,
/* 1016 */ 1023,
/* 1017 */ 1014,
/* 1018 */ -1014,
/* 1019 */ 519,
/* 1020 */ 0,
/* 1021 */ -1014,
/* 1022 */ 983,
/* 1023 */ 1027,
/* 1024 */ 50044,
/* 1025 */ -1023,
/* 1026 */ 518,
/* 1027 */ 0,
/* 1028 */ -1023,
/* 1029 */ 982,
/* 1030 */ 1034,
/* 1031 */ 50058,
/* 1032 */ -1030,
/* 1033 */ 520,
/* 1034 */ 0,
/* 1035 */ -1030,
/* 1036 */ 984,
/* 1037 */ 1043,
/* 1038 */ 50044,
/* 1039 */ 1230,
/* 1040 */ 1037,
/* 1041 */ -1037,
/* 1042 */ 521,
/* 1043 */ 0,
/* 1044 */ -1037,
/* 1045 */ 985,
/* 1046 */ 1050,
/* 1047 */ 50058,
/* 1048 */ -1046,
/* 1049 */ 522,
/* 1050 */ 0,
/* 1051 */ -1046,
/* 1052 */ 986,
/* 1053 */ 1057,
/* 1054 */ 50058,
/* 1055 */ -1053,
/* 1056 */ 523,
/* 1057 */ 0,
/* 1058 */ -1053,
/* 1059 */ 987,
/* 1060 */ 1064,
/* 1061 */ 50058,
/* 1062 */ -1060,
/* 1063 */ 524,
/* 1064 */ 0,
/* 1065 */ -1060,
/* 1066 */ 988,
/* 1067 */ 1071,
/* 1068 */ 50058,
/* 1069 */ -1067,
/* 1070 */ 525,
/* 1071 */ 0,
/* 1072 */ -1067,
/* 1073 */ 989,
/* 1074 */ 1080,
/* 1075 */ 215,
/* 1076 */ 50044,
/* 1077 */ 1074,
/* 1078 */ -1074,
/* 1079 */ 526,
/* 1080 */ 0,
/* 1081 */ -1074,
/* 1082 */ 990,
/* 1083 */ 1087,
/* 1084 */ 50058,
/* 1085 */ -1083,
/* 1086 */ 527,
/* 1087 */ 0,
/* 1088 */ -1083,
/* 1089 */ 991,
/* 1090 */ 1095,
/* 1091 */ 1357,
/* 1092 */ 1090,
/* 1093 */ -1090,
/* 1094 */ 528,
/* 1095 */ 0,
/* 1096 */ -1090,
/* 1097 */ 992,
/* 1098 */ 1104,
/* 1099 */ 50044,
/* 1100 */ 1382,
/* 1101 */ 1098,
/* 1102 */ -1098,
/* 1103 */ 529,
/* 1104 */ 0,
/* 1105 */ -1098,
/* 1106 */ 993,
/* 1107 */ 1111,
/* 1108 */ 50058,
/* 1109 */ -1107,
/* 1110 */ 530,
/* 1111 */ 0,
/* 1112 */ -1107,
/* 1113 */ 994,
/* 1114 */ 1118,
/* 1115 */ 50058,
/* 1116 */ -1114,
/* 1117 */ 531,
/* 1118 */ 0,
/* 1119 */ -1114,
/* 1120 */ 995,
/* 1121 */ 1125,
/* 1122 */ 50037,
/* 1123 */ -1121,
/* 1124 */ 532,
/* 1125 */ 0,
/* 1126 */ -1121,
/* 1127 */ 996,
/* 1128 */ 1132,
/* 1129 */ 50058,
/* 1130 */ -1128,
/* 1131 */ 533,
/* 1132 */ 0,
/* 1133 */ -1128,
/* 1134 */ 997,
/* 1135 */ 1139,
/* 1136 */ 50037,
/* 1137 */ -1135,
/* 1138 */ 534,
/* 1139 */ 0,
/* 1140 */ -1135,
/* 1141 */ 998,
/* 1142 */ 1146,
/* 1143 */ 50058,
/* 1144 */ -1142,
/* 1145 */ 535,
/* 1146 */ 0,
/* 1147 */ -1142,
/* 1148 */ 999,
/* 1149 */ 1155,
/* 1150 */ 155,
/* 1151 */ 1158,
/* 1152 */ 1149,
/* 1153 */ -1149,
/* 1154 */ 537,
/* 1155 */ 0,
/* 1156 */ -1149,
/* 1157 */ 1001,
/* 1158 */ 1162,
/* 1159 */ 50044,
/* 1160 */ -1158,
/* 1161 */ 536,
/* 1162 */ 0,
/* 1163 */ -1158,
/* 1164 */ 1000,
/* 1165 */ 1169,
/* 1166 */ 50058,
/* 1167 */ -1165,
/* 1168 */ 538,
/* 1169 */ 0,
/* 1170 */ -1165,
/* 1171 */ 1002,
/* 1172 */ 1178,
/* 1173 */ 155,
/* 1174 */ 1181,
/* 1175 */ 1172,
/* 1176 */ -1172,
/* 1177 */ 540,
/* 1178 */ 0,
/* 1179 */ -1172,
/* 1180 */ 1004,
/* 1181 */ 1185,
/* 1182 */ 50044,
/* 1183 */ -1181,
/* 1184 */ 539,
/* 1185 */ 0,
/* 1186 */ -1181,
/* 1187 */ 1003,
/* 1188 */ 1192,
/* 1189 */ 50058,
/* 1190 */ -1188,
/* 1191 */ 541,
/* 1192 */ 0,
/* 1193 */ -1188,
/* 1194 */ 1005,
/* 1195 */ 1201,
/* 1196 */ 155,
/* 1197 */ 1204,
/* 1198 */ 1195,
/* 1199 */ -1195,
/* 1200 */ 543,
/* 1201 */ 0,
/* 1202 */ -1195,
/* 1203 */ 1007,
/* 1204 */ 1208,
/* 1205 */ 50044,
/* 1206 */ -1204,
/* 1207 */ 542,
/* 1208 */ 0,
/* 1209 */ -1204,
/* 1210 */ 1006,
/* 1211 */ 1215,
/* 1212 */ 50274,
/* 1213 */ -1211,
/* 1214 */ 95,
/* 1215 */ 1220,
/* 1216 */ 50300,
/* 1217 */ 50274,
/* 1218 */ -1211,
/* 1219 */ 96,
/* 1220 */ 1225,
/* 1221 */ 50372,
/* 1222 */ 50274,
/* 1223 */ -1211,
/* 1224 */ 97,
/* 1225 */ 0,
/* 1226 */ 50317,
/* 1227 */ 50274,
/* 1228 */ -1211,
/* 1229 */ 98,
/* 1230 */ 1240,
/* 1231 */ 215,
/* 1232 */ 50349,
/* 1233 */ 1247,
/* 1234 */ 1259,
/* 1235 */ 1266,
/* 1236 */ 1273,
/* 1237 */ 1295,
/* 1238 */ -1230,
/* 1239 */ 99,
/* 1240 */ 0,
/* 1241 */ 215,
/* 1242 */ 50349,
/* 1243 */ 155,
/* 1244 */ 1350,
/* 1245 */ -1230,
/* 1246 */ 100,
/* 1247 */ 1251,
/* 1248 */ 278,
/* 1249 */ -1247,
/* 1250 */ 544,
/* 1251 */ 1256,
/* 1252 */ 229,
/* 1253 */ 50037,
/* 1254 */ -1247,
/* 1255 */ 545,
/* 1256 */ 0,
/* 1257 */ -1247,
/* 1258 */ 1008,
/* 1259 */ 1263,
/* 1260 */ 229,
/* 1261 */ -1259,
/* 1262 */ 546,
/* 1263 */ 0,
/* 1264 */ -1259,
/* 1265 */ 1009,
/* 1266 */ 1270,
/* 1267 */ 50316,
/* 1268 */ -1266,
/* 1269 */ 547,
/* 1270 */ 0,
/* 1271 */ -1266,
/* 1272 */ 1010,
/* 1273 */ 1280,
/* 1274 */ 50040,
/* 1275 */ 1283,
/* 1276 */ 215,
/* 1277 */ 50041,
/* 1278 */ -1273,
/* 1279 */ 551,
/* 1280 */ 0,
/* 1281 */ -1273,
/* 1282 */ 1011,
/* 1283 */ 1287,
/* 1284 */ 50319,
/* 1285 */ -1283,
/* 1286 */ 548,
/* 1287 */ 1291,
/* 1288 */ 50304,
/* 1289 */ -1283,
/* 1290 */ 549,
/* 1291 */ 0,
/* 1292 */ 50370,
/* 1293 */ -1283,
/* 1294 */ 550,
/* 1295 */ 1304,
/* 1296 */ 50287,
/* 1297 */ 1307,
/* 1298 */ 1314,
/* 1299 */ 1321,
/* 1300 */ 1328,
/* 1301 */ 1295,
/* 1302 */ -1295,
/* 1303 */ 559,
/* 1304 */ 0,
/* 1305 */ -1295,
/* 1306 */ 1016,
/* 1307 */ 1311,
/* 1308 */ 278,
/* 1309 */ -1307,
/* 1310 */ 552,
/* 1311 */ 0,
/* 1312 */ -1307,
/* 1313 */ 1012,
/* 1314 */ 1318,
/* 1315 */ 215,
/* 1316 */ -1314,
/* 1317 */ 553,
/* 1318 */ 0,
/* 1319 */ -1314,
/* 1320 */ 1013,
/* 1321 */ 1325,
/* 1322 */ 50316,
/* 1323 */ -1321,
/* 1324 */ 554,
/* 1325 */ 0,
/* 1326 */ -1321,
/* 1327 */ 1014,
/* 1328 */ 1335,
/* 1329 */ 50040,
/* 1330 */ 1338,
/* 1331 */ 215,
/* 1332 */ 50041,
/* 1333 */ -1328,
/* 1334 */ 558,
/* 1335 */ 0,
/* 1336 */ -1328,
/* 1337 */ 1015,
/* 1338 */ 1342,
/* 1339 */ 50319,
/* 1340 */ -1338,
/* 1341 */ 555,
/* 1342 */ 1346,
/* 1343 */ 50304,
/* 1344 */ -1338,
/* 1345 */ 556,
/* 1346 */ 0,
/* 1347 */ 50370,
/* 1348 */ -1338,
/* 1349 */ 557,
/* 1350 */ 1354,
/* 1351 */ 215,
/* 1352 */ -1350,
/* 1353 */ 560,
/* 1354 */ 0,
/* 1355 */ -1350,
/* 1356 */ 1017,
/* 1357 */ 1362,
/* 1358 */ 215,
/* 1359 */ 1368,
/* 1360 */ -1357,
/* 1361 */ 101,
/* 1362 */ 0,
/* 1363 */ 50126,
/* 1364 */ 215,
/* 1365 */ 1375,
/* 1366 */ -1357,
/* 1367 */ 102,
/* 1368 */ 1372,
/* 1369 */ 50044,
/* 1370 */ -1368,
/* 1371 */ 561,
/* 1372 */ 0,
/* 1373 */ -1368,
/* 1374 */ 1018,
/* 1375 */ 1379,
/* 1376 */ 50044,
/* 1377 */ -1375,
/* 1378 */ 562,
/* 1379 */ 0,
/* 1380 */ -1375,
/* 1381 */ 1019,
/* 1382 */ 0,
/* 1383 */ 50276,
/* 1384 */ 1388,
/* 1385 */ 215,
/* 1386 */ -1382,
/* 1387 */ 103,
/* 1388 */ 1392,
/* 1389 */ 50058,
/* 1390 */ -1388,
/* 1391 */ 563,
/* 1392 */ 0,
/* 1393 */ -1388,
/* 1394 */ 1020,
/* 1395 */ 0,
/* 1396 */ 50368,
/* 1397 */ 50257,
/* 1398 */ 50058,
/* 1399 */ 215,
/* 1400 */ 50123,
/* 1401 */ 1405,
/* 1402 */ 50125,
/* 1403 */ -1395,
/* 1404 */ 104,
/* 1405 */ 1410,
/* 1406 */ 1413,
/* 1407 */ 1405,
/* 1408 */ -1405,
/* 1409 */ 564,
/* 1410 */ 0,
/* 1411 */ -1405,
/* 1412 */ 1021,
/* 1413 */ 1420,
/* 1414 */ 50323,
/* 1415 */ 1679,
/* 1416 */ 50257,
/* 1417 */ 50059,
/* 1418 */ -1413,
/* 1419 */ 105,
/* 1420 */ 1427,
/* 1421 */ 50370,
/* 1422 */ 1686,
/* 1423 */ 165,
/* 1424 */ 50059,
/* 1425 */ -1413,
/* 1426 */ 106,
/* 1427 */ 1431,
/* 1428 */ 454,
/* 1429 */ -1413,
/* 1430 */ 107,
/* 1431 */ 1438,
/* 1432 */ 50425,
/* 1433 */ 1693,
/* 1434 */ 251,
/* 1435 */ 50059,
/* 1436 */ -1413,
/* 1437 */ 108,
/* 1438 */ 1445,
/* 1439 */ 50369,
/* 1440 */ 1700,
/* 1441 */ 251,
/* 1442 */ 50059,
/* 1443 */ -1413,
/* 1444 */ 109,
/* 1445 */ 1452,
/* 1446 */ 50313,
/* 1447 */ 1707,
/* 1448 */ 215,
/* 1449 */ 50059,
/* 1450 */ -1413,
/* 1451 */ 110,
/* 1452 */ 1459,
/* 1453 */ 50447,
/* 1454 */ 1714,
/* 1455 */ 215,
/* 1456 */ 50059,
/* 1457 */ -1413,
/* 1458 */ 111,
/* 1459 */ 1466,
/* 1460 */ 50283,
/* 1461 */ 1721,
/* 1462 */ 165,
/* 1463 */ 50059,
/* 1464 */ -1413,
/* 1465 */ 112,
/* 1466 */ 1473,
/* 1467 */ 50430,
/* 1468 */ 1728,
/* 1469 */ 165,
/* 1470 */ 50059,
/* 1471 */ -1413,
/* 1472 */ 113,
/* 1473 */ 1481,
/* 1474 */ 50430,
/* 1475 */ 1735,
/* 1476 */ 229,
/* 1477 */ 50037,
/* 1478 */ 50059,
/* 1479 */ -1413,
/* 1480 */ 114,
/* 1481 */ 1490,
/* 1482 */ 50430,
/* 1483 */ 1742,
/* 1484 */ 50043,
/* 1485 */ 229,
/* 1486 */ 50037,
/* 1487 */ 50059,
/* 1488 */ -1413,
/* 1489 */ 115,
/* 1490 */ 1497,
/* 1491 */ 50400,
/* 1492 */ 1749,
/* 1493 */ 165,
/* 1494 */ 50059,
/* 1495 */ -1413,
/* 1496 */ 116,
/* 1497 */ 1504,
/* 1498 */ 50415,
/* 1499 */ 1756,
/* 1500 */ 165,
/* 1501 */ 50059,
/* 1502 */ -1413,
/* 1503 */ 117,
/* 1504 */ 1511,
/* 1505 */ 50379,
/* 1506 */ 1763,
/* 1507 */ 215,
/* 1508 */ 50059,
/* 1509 */ -1413,
/* 1510 */ 118,
/* 1511 */ 1519,
/* 1512 */ 50312,
/* 1513 */ 1770,
/* 1514 */ 215,
/* 1515 */ 1777,
/* 1516 */ 50059,
/* 1517 */ -1413,
/* 1518 */ 119,
/* 1519 */ 1526,
/* 1520 */ 50461,
/* 1521 */ 1784,
/* 1522 */ 215,
/* 1523 */ 50059,
/* 1524 */ -1413,
/* 1525 */ 120,
/* 1526 */ 1533,
/* 1527 */ 50426,
/* 1528 */ 1791,
/* 1529 */ 215,
/* 1530 */ 50059,
/* 1531 */ -1413,
/* 1532 */ 121,
/* 1533 */ 1540,
/* 1534 */ 50392,
/* 1535 */ 1798,
/* 1536 */ 215,
/* 1537 */ 50059,
/* 1538 */ -1413,
/* 1539 */ 122,
/* 1540 */ 1547,
/* 1541 */ 50320,
/* 1542 */ 1805,
/* 1543 */ 215,
/* 1544 */ 50059,
/* 1545 */ -1413,
/* 1546 */ 123,
/* 1547 */ 1554,
/* 1548 */ 50309,
/* 1549 */ 1812,
/* 1550 */ 215,
/* 1551 */ 50059,
/* 1552 */ -1413,
/* 1553 */ 124,
/* 1554 */ 1561,
/* 1555 */ 50402,
/* 1556 */ 1819,
/* 1557 */ 215,
/* 1558 */ 50059,
/* 1559 */ -1413,
/* 1560 */ 125,
/* 1561 */ 1568,
/* 1562 */ 50290,
/* 1563 */ 1826,
/* 1564 */ 1833,
/* 1565 */ 50059,
/* 1566 */ -1413,
/* 1567 */ 126,
/* 1568 */ 1575,
/* 1569 */ 50361,
/* 1570 */ 1841,
/* 1571 */ 215,
/* 1572 */ 50059,
/* 1573 */ -1413,
/* 1574 */ 127,
/* 1575 */ 1582,
/* 1576 */ 50432,
/* 1577 */ 1848,
/* 1578 */ 1855,
/* 1579 */ 50059,
/* 1580 */ -1413,
/* 1581 */ 128,
/* 1582 */ 1589,
/* 1583 */ 50345,
/* 1584 */ 1871,
/* 1585 */ 1878,
/* 1586 */ 50059,
/* 1587 */ -1413,
/* 1588 */ 129,
/* 1589 */ 1596,
/* 1590 */ 50346,
/* 1591 */ 1894,
/* 1592 */ 1901,
/* 1593 */ 50059,
/* 1594 */ -1413,
/* 1595 */ 130,
/* 1596 */ 1603,
/* 1597 */ 50357,
/* 1598 */ 1909,
/* 1599 */ 1916,
/* 1600 */ 50059,
/* 1601 */ -1413,
/* 1602 */ 131,
/* 1603 */ 1610,
/* 1604 */ 50301,
/* 1605 */ 1936,
/* 1606 */ 215,
/* 1607 */ 50059,
/* 1608 */ -1413,
/* 1609 */ 132,
/* 1610 */ 1617,
/* 1611 */ 50463,
/* 1612 */ 1943,
/* 1613 */ 215,
/* 1614 */ 50059,
/* 1615 */ -1413,
/* 1616 */ 133,
/* 1617 */ 1624,
/* 1618 */ 50462,
/* 1619 */ 1950,
/* 1620 */ 215,
/* 1621 */ 50059,
/* 1622 */ -1413,
/* 1623 */ 134,
/* 1624 */ 1631,
/* 1625 */ 50375,
/* 1626 */ 1957,
/* 1627 */ 215,
/* 1628 */ 50059,
/* 1629 */ -1413,
/* 1630 */ 135,
/* 1631 */ 1638,
/* 1632 */ 50451,
/* 1633 */ 1964,
/* 1634 */ 215,
/* 1635 */ 50059,
/* 1636 */ -1413,
/* 1637 */ 136,
/* 1638 */ 1645,
/* 1639 */ 50310,
/* 1640 */ 1971,
/* 1641 */ 215,
/* 1642 */ 50059,
/* 1643 */ -1413,
/* 1644 */ 137,
/* 1645 */ 1653,
/* 1646 */ 50372,
/* 1647 */ 50415,
/* 1648 */ 1978,
/* 1649 */ 215,
/* 1650 */ 50059,
/* 1651 */ -1413,
/* 1652 */ 138,
/* 1653 */ 1660,
/* 1654 */ 50371,
/* 1655 */ 1985,
/* 1656 */ 215,
/* 1657 */ 50059,
/* 1658 */ -1413,
/* 1659 */ 139,
/* 1660 */ 1667,
/* 1661 */ 50351,
/* 1662 */ 1992,
/* 1663 */ 155,
/* 1664 */ 50059,
/* 1665 */ -1413,
/* 1666 */ 140,
/* 1667 */ 1671,
/* 1668 */ 3387,
/* 1669 */ -1413,
/* 1670 */ 141,
/* 1671 */ 1675,
/* 1672 */ 530,
/* 1673 */ -1413,
/* 1674 */ 142,
/* 1675 */ 0,
/* 1676 */ 5623,
/* 1677 */ -1413,
/* 1678 */ 143,
/* 1679 */ 1683,
/* 1680 */ 50058,
/* 1681 */ -1679,
/* 1682 */ 565,
/* 1683 */ 0,
/* 1684 */ -1679,
/* 1685 */ 1022,
/* 1686 */ 1690,
/* 1687 */ 50058,
/* 1688 */ -1686,
/* 1689 */ 566,
/* 1690 */ 0,
/* 1691 */ -1686,
/* 1692 */ 1023,
/* 1693 */ 1697,
/* 1694 */ 50058,
/* 1695 */ -1693,
/* 1696 */ 567,
/* 1697 */ 0,
/* 1698 */ -1693,
/* 1699 */ 1024,
/* 1700 */ 1704,
/* 1701 */ 50058,
/* 1702 */ -1700,
/* 1703 */ 568,
/* 1704 */ 0,
/* 1705 */ -1700,
/* 1706 */ 1025,
/* 1707 */ 1711,
/* 1708 */ 50058,
/* 1709 */ -1707,
/* 1710 */ 569,
/* 1711 */ 0,
/* 1712 */ -1707,
/* 1713 */ 1026,
/* 1714 */ 1718,
/* 1715 */ 50058,
/* 1716 */ -1714,
/* 1717 */ 570,
/* 1718 */ 0,
/* 1719 */ -1714,
/* 1720 */ 1027,
/* 1721 */ 1725,
/* 1722 */ 50058,
/* 1723 */ -1721,
/* 1724 */ 571,
/* 1725 */ 0,
/* 1726 */ -1721,
/* 1727 */ 1028,
/* 1728 */ 1732,
/* 1729 */ 50058,
/* 1730 */ -1728,
/* 1731 */ 572,
/* 1732 */ 0,
/* 1733 */ -1728,
/* 1734 */ 1029,
/* 1735 */ 1739,
/* 1736 */ 50058,
/* 1737 */ -1735,
/* 1738 */ 573,
/* 1739 */ 0,
/* 1740 */ -1735,
/* 1741 */ 1030,
/* 1742 */ 1746,
/* 1743 */ 50058,
/* 1744 */ -1742,
/* 1745 */ 574,
/* 1746 */ 0,
/* 1747 */ -1742,
/* 1748 */ 1031,
/* 1749 */ 1753,
/* 1750 */ 50058,
/* 1751 */ -1749,
/* 1752 */ 575,
/* 1753 */ 0,
/* 1754 */ -1749,
/* 1755 */ 1032,
/* 1756 */ 1760,
/* 1757 */ 50058,
/* 1758 */ -1756,
/* 1759 */ 576,
/* 1760 */ 0,
/* 1761 */ -1756,
/* 1762 */ 1033,
/* 1763 */ 1767,
/* 1764 */ 50058,
/* 1765 */ -1763,
/* 1766 */ 577,
/* 1767 */ 0,
/* 1768 */ -1763,
/* 1769 */ 1034,
/* 1770 */ 1774,
/* 1771 */ 50058,
/* 1772 */ -1770,
/* 1773 */ 578,
/* 1774 */ 0,
/* 1775 */ -1770,
/* 1776 */ 1035,
/* 1777 */ 1781,
/* 1778 */ 215,
/* 1779 */ -1777,
/* 1780 */ 579,
/* 1781 */ 0,
/* 1782 */ -1777,
/* 1783 */ 1036,
/* 1784 */ 1788,
/* 1785 */ 50058,
/* 1786 */ -1784,
/* 1787 */ 580,
/* 1788 */ 0,
/* 1789 */ -1784,
/* 1790 */ 1037,
/* 1791 */ 1795,
/* 1792 */ 50058,
/* 1793 */ -1791,
/* 1794 */ 581,
/* 1795 */ 0,
/* 1796 */ -1791,
/* 1797 */ 1038,
/* 1798 */ 1802,
/* 1799 */ 50058,
/* 1800 */ -1798,
/* 1801 */ 582,
/* 1802 */ 0,
/* 1803 */ -1798,
/* 1804 */ 1039,
/* 1805 */ 1809,
/* 1806 */ 50058,
/* 1807 */ -1805,
/* 1808 */ 583,
/* 1809 */ 0,
/* 1810 */ -1805,
/* 1811 */ 1040,
/* 1812 */ 1816,
/* 1813 */ 50058,
/* 1814 */ -1812,
/* 1815 */ 584,
/* 1816 */ 0,
/* 1817 */ -1812,
/* 1818 */ 1041,
/* 1819 */ 1823,
/* 1820 */ 50058,
/* 1821 */ -1819,
/* 1822 */ 585,
/* 1823 */ 0,
/* 1824 */ -1819,
/* 1825 */ 1042,
/* 1826 */ 1830,
/* 1827 */ 50058,
/* 1828 */ -1826,
/* 1829 */ 586,
/* 1830 */ 0,
/* 1831 */ -1826,
/* 1832 */ 1043,
/* 1833 */ 1838,
/* 1834 */ 1999,
/* 1835 */ 1833,
/* 1836 */ -1833,
/* 1837 */ 587,
/* 1838 */ 0,
/* 1839 */ -1833,
/* 1840 */ 1044,
/* 1841 */ 1845,
/* 1842 */ 50058,
/* 1843 */ -1841,
/* 1844 */ 588,
/* 1845 */ 0,
/* 1846 */ -1841,
/* 1847 */ 1045,
/* 1848 */ 1852,
/* 1849 */ 50058,
/* 1850 */ -1848,
/* 1851 */ 589,
/* 1852 */ 0,
/* 1853 */ -1848,
/* 1854 */ 1046,
/* 1855 */ 1861,
/* 1856 */ 155,
/* 1857 */ 1864,
/* 1858 */ 1855,
/* 1859 */ -1855,
/* 1860 */ 591,
/* 1861 */ 0,
/* 1862 */ -1855,
/* 1863 */ 1048,
/* 1864 */ 1868,
/* 1865 */ 50044,
/* 1866 */ -1864,
/* 1867 */ 590,
/* 1868 */ 0,
/* 1869 */ -1864,
/* 1870 */ 1047,
/* 1871 */ 1875,
/* 1872 */ 50058,
/* 1873 */ -1871,
/* 1874 */ 592,
/* 1875 */ 0,
/* 1876 */ -1871,
/* 1877 */ 1049,
/* 1878 */ 1884,
/* 1879 */ 155,
/* 1880 */ 1887,
/* 1881 */ 1878,
/* 1882 */ -1878,
/* 1883 */ 594,
/* 1884 */ 0,
/* 1885 */ -1878,
/* 1886 */ 1051,
/* 1887 */ 1891,
/* 1888 */ 50044,
/* 1889 */ -1887,
/* 1890 */ 593,
/* 1891 */ 0,
/* 1892 */ -1887,
/* 1893 */ 1050,
/* 1894 */ 1898,
/* 1895 */ 50058,
/* 1896 */ -1894,
/* 1897 */ 595,
/* 1898 */ 0,
/* 1899 */ -1894,
/* 1900 */ 1052,
/* 1901 */ 1906,
/* 1902 */ 2024,
/* 1903 */ 1901,
/* 1904 */ -1901,
/* 1905 */ 596,
/* 1906 */ 0,
/* 1907 */ -1901,
/* 1908 */ 1053,
/* 1909 */ 1913,
/* 1910 */ 50058,
/* 1911 */ -1909,
/* 1912 */ 597,
/* 1913 */ 0,
/* 1914 */ -1909,
/* 1915 */ 1054,
/* 1916 */ 1922,
/* 1917 */ 215,
/* 1918 */ 1925,
/* 1919 */ 1916,
/* 1920 */ -1916,
/* 1921 */ 600,
/* 1922 */ 0,
/* 1923 */ -1916,
/* 1924 */ 1056,
/* 1925 */ 1929,
/* 1926 */ 50044,
/* 1927 */ -1925,
/* 1928 */ 598,
/* 1929 */ 1933,
/* 1930 */ 50124,
/* 1931 */ -1925,
/* 1932 */ 599,
/* 1933 */ 0,
/* 1934 */ -1925,
/* 1935 */ 1055,
/* 1936 */ 1940,
/* 1937 */ 50058,
/* 1938 */ -1936,
/* 1939 */ 601,
/* 1940 */ 0,
/* 1941 */ -1936,
/* 1942 */ 1057,
/* 1943 */ 1947,
/* 1944 */ 50058,
/* 1945 */ -1943,
/* 1946 */ 602,
/* 1947 */ 0,
/* 1948 */ -1943,
/* 1949 */ 1058,
/* 1950 */ 1954,
/* 1951 */ 50058,
/* 1952 */ -1950,
/* 1953 */ 603,
/* 1954 */ 0,
/* 1955 */ -1950,
/* 1956 */ 1059,
/* 1957 */ 1961,
/* 1958 */ 50058,
/* 1959 */ -1957,
/* 1960 */ 604,
/* 1961 */ 0,
/* 1962 */ -1957,
/* 1963 */ 1060,
/* 1964 */ 1968,
/* 1965 */ 50058,
/* 1966 */ -1964,
/* 1967 */ 605,
/* 1968 */ 0,
/* 1969 */ -1964,
/* 1970 */ 1061,
/* 1971 */ 1975,
/* 1972 */ 50058,
/* 1973 */ -1971,
/* 1974 */ 606,
/* 1975 */ 0,
/* 1976 */ -1971,
/* 1977 */ 1062,
/* 1978 */ 1982,
/* 1979 */ 50058,
/* 1980 */ -1978,
/* 1981 */ 607,
/* 1982 */ 0,
/* 1983 */ -1978,
/* 1984 */ 1063,
/* 1985 */ 1989,
/* 1986 */ 50058,
/* 1987 */ -1985,
/* 1988 */ 608,
/* 1989 */ 0,
/* 1990 */ -1985,
/* 1991 */ 1064,
/* 1992 */ 1996,
/* 1993 */ 50058,
/* 1994 */ -1992,
/* 1995 */ 609,
/* 1996 */ 0,
/* 1997 */ -1992,
/* 1998 */ 1065,
/* 1999 */ 2004,
/* 2000 */ 215,
/* 2001 */ 2010,
/* 2002 */ -1999,
/* 2003 */ 144,
/* 2004 */ 0,
/* 2005 */ 50275,
/* 2006 */ 215,
/* 2007 */ 2017,
/* 2008 */ -1999,
/* 2009 */ 145,
/* 2010 */ 2014,
/* 2011 */ 50044,
/* 2012 */ -2010,
/* 2013 */ 610,
/* 2014 */ 0,
/* 2015 */ -2010,
/* 2016 */ 1066,
/* 2017 */ 2021,
/* 2018 */ 50044,
/* 2019 */ -2017,
/* 2020 */ 611,
/* 2021 */ 0,
/* 2022 */ -2017,
/* 2023 */ 1067,
/* 2024 */ 2029,
/* 2025 */ 215,
/* 2026 */ 2035,
/* 2027 */ -2024,
/* 2028 */ 146,
/* 2029 */ 0,
/* 2030 */ 50126,
/* 2031 */ 215,
/* 2032 */ 2042,
/* 2033 */ -2024,
/* 2034 */ 147,
/* 2035 */ 2039,
/* 2036 */ 50044,
/* 2037 */ -2035,
/* 2038 */ 612,
/* 2039 */ 0,
/* 2040 */ -2035,
/* 2041 */ 1068,
/* 2042 */ 2046,
/* 2043 */ 50044,
/* 2044 */ -2042,
/* 2045 */ 613,
/* 2046 */ 0,
/* 2047 */ -2042,
/* 2048 */ 1069,
/* 2049 */ 0,
/* 2050 */ 50344,
/* 2051 */ 50257,
/* 2052 */ 50058,
/* 2053 */ 215,
/* 2054 */ 50123,
/* 2055 */ 2059,
/* 2056 */ 50125,
/* 2057 */ -2049,
/* 2058 */ 148,
/* 2059 */ 2064,
/* 2060 */ 2067,
/* 2061 */ 2059,
/* 2062 */ -2059,
/* 2063 */ 614,
/* 2064 */ 0,
/* 2065 */ -2059,
/* 2066 */ 1070,
/* 2067 */ 2074,
/* 2068 */ 50323,
/* 2069 */ 2140,
/* 2070 */ 50257,
/* 2071 */ 50059,
/* 2072 */ -2067,
/* 2073 */ 149,
/* 2074 */ 2081,
/* 2075 */ 50379,
/* 2076 */ 2147,
/* 2077 */ 215,
/* 2078 */ 50059,
/* 2079 */ -2067,
/* 2080 */ 150,
/* 2081 */ 2085,
/* 2082 */ 454,
/* 2083 */ -2067,
/* 2084 */ 151,
/* 2085 */ 2093,
/* 2086 */ 50385,
/* 2087 */ 2154,
/* 2088 */ 215,
/* 2089 */ 50037,
/* 2090 */ 50059,
/* 2091 */ -2067,
/* 2092 */ 152,
/* 2093 */ 2100,
/* 2094 */ 50361,
/* 2095 */ 2161,
/* 2096 */ 215,
/* 2097 */ 50059,
/* 2098 */ -2067,
/* 2099 */ 153,
/* 2100 */ 2107,
/* 2101 */ 50346,
/* 2102 */ 2168,
/* 2103 */ 2175,
/* 2104 */ 50059,
/* 2105 */ -2067,
/* 2106 */ 154,
/* 2107 */ 2114,
/* 2108 */ 2195,
/* 2109 */ 2203,
/* 2110 */ 155,
/* 2111 */ 50059,
/* 2112 */ -2067,
/* 2113 */ 155,
/* 2114 */ 2121,
/* 2115 */ 50467,
/* 2116 */ 2210,
/* 2117 */ 165,
/* 2118 */ 50059,
/* 2119 */ -2067,
/* 2120 */ 156,
/* 2121 */ 2128,
/* 2122 */ 50340,
/* 2123 */ 2217,
/* 2124 */ 251,
/* 2125 */ 50059,
/* 2126 */ -2067,
/* 2127 */ 157,
/* 2128 */ 2132,
/* 2129 */ 530,
/* 2130 */ -2067,
/* 2131 */ 158,
/* 2132 */ 2136,
/* 2133 */ 3387,
/* 2134 */ -2067,
/* 2135 */ 159,
/* 2136 */ 0,
/* 2137 */ 5623,
/* 2138 */ -2067,
/* 2139 */ 160,
/* 2140 */ 2144,
/* 2141 */ 50058,
/* 2142 */ -2140,
/* 2143 */ 615,
/* 2144 */ 0,
/* 2145 */ -2140,
/* 2146 */ 1071,
/* 2147 */ 2151,
/* 2148 */ 50058,
/* 2149 */ -2147,
/* 2150 */ 616,
/* 2151 */ 0,
/* 2152 */ -2147,
/* 2153 */ 1072,
/* 2154 */ 2158,
/* 2155 */ 50058,
/* 2156 */ -2154,
/* 2157 */ 617,
/* 2158 */ 0,
/* 2159 */ -2154,
/* 2160 */ 1073,
/* 2161 */ 2165,
/* 2162 */ 50058,
/* 2163 */ -2161,
/* 2164 */ 618,
/* 2165 */ 0,
/* 2166 */ -2161,
/* 2167 */ 1074,
/* 2168 */ 2172,
/* 2169 */ 50058,
/* 2170 */ -2168,
/* 2171 */ 619,
/* 2172 */ 0,
/* 2173 */ -2168,
/* 2174 */ 1075,
/* 2175 */ 2181,
/* 2176 */ 215,
/* 2177 */ 2184,
/* 2178 */ 2175,
/* 2179 */ -2175,
/* 2180 */ 622,
/* 2181 */ 0,
/* 2182 */ -2175,
/* 2183 */ 1077,
/* 2184 */ 2188,
/* 2185 */ 50124,
/* 2186 */ -2184,
/* 2187 */ 620,
/* 2188 */ 2192,
/* 2189 */ 50044,
/* 2190 */ -2184,
/* 2191 */ 621,
/* 2192 */ 0,
/* 2193 */ -2184,
/* 2194 */ 1076,
/* 2195 */ 2199,
/* 2196 */ 50443,
/* 2197 */ -2195,
/* 2198 */ 623,
/* 2199 */ 0,
/* 2200 */ 50331,
/* 2201 */ -2195,
/* 2202 */ 624,
/* 2203 */ 2207,
/* 2204 */ 50058,
/* 2205 */ -2203,
/* 2206 */ 625,
/* 2207 */ 0,
/* 2208 */ -2203,
/* 2209 */ 1078,
/* 2210 */ 2214,
/* 2211 */ 50058,
/* 2212 */ -2210,
/* 2213 */ 626,
/* 2214 */ 0,
/* 2215 */ -2210,
/* 2216 */ 1079,
/* 2217 */ 2221,
/* 2218 */ 50058,
/* 2219 */ -2217,
/* 2220 */ 627,
/* 2221 */ 0,
/* 2222 */ -2217,
/* 2223 */ 1080,
/* 2224 */ 0,
/* 2225 */ 2236,
/* 2226 */ 2252,
/* 2227 */ 50257,
/* 2228 */ 50058,
/* 2229 */ 215,
/* 2230 */ 50123,
/* 2231 */ 2268,
/* 2232 */ 50125,
/* 2233 */ 2276,
/* 2234 */ -2224,
/* 2235 */ 161,
/* 2236 */ 2242,
/* 2237 */ 215,
/* 2238 */ 2245,
/* 2239 */ 2236,
/* 2240 */ -2236,
/* 2241 */ 629,
/* 2242 */ 0,
/* 2243 */ -2236,
/* 2244 */ 1082,
/* 2245 */ 2249,
/* 2246 */ 50047,
/* 2247 */ -2245,
/* 2248 */ 628,
/* 2249 */ 0,
/* 2250 */ -2245,
/* 2251 */ 1081,
/* 2252 */ 2256,
/* 2253 */ 50431,
/* 2254 */ -2252,
/* 2255 */ 630,
/* 2256 */ 2260,
/* 2257 */ 50331,
/* 2258 */ -2252,
/* 2259 */ 631,
/* 2260 */ 2264,
/* 2261 */ 50404,
/* 2262 */ -2252,
/* 2263 */ 632,
/* 2264 */ 0,
/* 2265 */ 50327,
/* 2266 */ -2252,
/* 2267 */ 633,
/* 2268 */ 2273,
/* 2269 */ 2296,
/* 2270 */ 2268,
/* 2271 */ -2268,
/* 2272 */ 634,
/* 2273 */ 0,
/* 2274 */ -2268,
/* 2275 */ 1083,
/* 2276 */ 2285,
/* 2277 */ 50287,
/* 2278 */ 215,
/* 2279 */ 50123,
/* 2280 */ 2288,
/* 2281 */ 50125,
/* 2282 */ 2276,
/* 2283 */ -2276,
/* 2284 */ 636,
/* 2285 */ 0,
/* 2286 */ -2276,
/* 2287 */ 1085,
/* 2288 */ 2293,
/* 2289 */ 2296,
/* 2290 */ 2288,
/* 2291 */ -2288,
/* 2292 */ 635,
/* 2293 */ 0,
/* 2294 */ -2288,
/* 2295 */ 1084,
/* 2296 */ 2303,
/* 2297 */ 50323,
/* 2298 */ 2450,
/* 2299 */ 50257,
/* 2300 */ 50059,
/* 2301 */ -2296,
/* 2302 */ 162,
/* 2303 */ 2308,
/* 2304 */ 165,
/* 2305 */ 50059,
/* 2306 */ -2296,
/* 2307 */ 163,
/* 2308 */ 2315,
/* 2309 */ 50370,
/* 2310 */ 2457,
/* 2311 */ 215,
/* 2312 */ 50059,
/* 2313 */ -2296,
/* 2314 */ 164,
/* 2315 */ 2322,
/* 2316 */ 50312,
/* 2317 */ 2464,
/* 2318 */ 215,
/* 2319 */ 50059,
/* 2320 */ -2296,
/* 2321 */ 165,
/* 2322 */ 2331,
/* 2323 */ 50296,
/* 2324 */ 2471,
/* 2325 */ 2478,
/* 2326 */ 215,
/* 2327 */ 2485,
/* 2328 */ 50059,
/* 2329 */ -2296,
/* 2330 */ 166,
/* 2331 */ 2339,
/* 2332 */ 50408,
/* 2333 */ 2492,
/* 2334 */ 165,
/* 2335 */ 2499,
/* 2336 */ 50059,
/* 2337 */ -2296,
/* 2338 */ 167,
/* 2339 */ 2347,
/* 2340 */ 50440,
/* 2341 */ 2508,
/* 2342 */ 165,
/* 2343 */ 2515,
/* 2344 */ 50059,
/* 2345 */ -2296,
/* 2346 */ 168,
/* 2347 */ 2354,
/* 2348 */ 50412,
/* 2349 */ 2522,
/* 2350 */ 165,
/* 2351 */ 50059,
/* 2352 */ -2296,
/* 2353 */ 169,
/* 2354 */ 2361,
/* 2355 */ 50467,
/* 2356 */ 2529,
/* 2357 */ 165,
/* 2358 */ 50059,
/* 2359 */ -2296,
/* 2360 */ 170,
/* 2361 */ 2368,
/* 2362 */ 50468,
/* 2363 */ 2536,
/* 2364 */ 165,
/* 2365 */ 50059,
/* 2366 */ -2296,
/* 2367 */ 171,
/* 2368 */ 2375,
/* 2369 */ 50330,
/* 2370 */ 2543,
/* 2371 */ 165,
/* 2372 */ 50059,
/* 2373 */ -2296,
/* 2374 */ 172,
/* 2375 */ 2382,
/* 2376 */ 50409,
/* 2377 */ 2550,
/* 2378 */ 251,
/* 2379 */ 50059,
/* 2380 */ -2296,
/* 2381 */ 173,
/* 2382 */ 2389,
/* 2383 */ 50295,
/* 2384 */ 2557,
/* 2385 */ 165,
/* 2386 */ 50059,
/* 2387 */ -2296,
/* 2388 */ 174,
/* 2389 */ 2396,
/* 2390 */ 50374,
/* 2391 */ 2564,
/* 2392 */ 165,
/* 2393 */ 50059,
/* 2394 */ -2296,
/* 2395 */ 175,
/* 2396 */ 2403,
/* 2397 */ 50456,
/* 2398 */ 2571,
/* 2399 */ 165,
/* 2400 */ 50059,
/* 2401 */ -2296,
/* 2402 */ 176,
/* 2403 */ 2410,
/* 2404 */ 50423,
/* 2405 */ 2578,
/* 2406 */ 155,
/* 2407 */ 50059,
/* 2408 */ -2296,
/* 2409 */ 177,
/* 2410 */ 2417,
/* 2411 */ 50423,
/* 2412 */ 2585,
/* 2413 */ 165,
/* 2414 */ 50059,
/* 2415 */ -2296,
/* 2416 */ 178,
/* 2417 */ 2424,
/* 2418 */ 50315,
/* 2419 */ 2592,
/* 2420 */ 165,
/* 2421 */ 50059,
/* 2422 */ -2296,
/* 2423 */ 179,
/* 2424 */ 2431,
/* 2425 */ 50315,
/* 2426 */ 2599,
/* 2427 */ 50274,
/* 2428 */ 50059,
/* 2429 */ -2296,
/* 2430 */ 180,
/* 2431 */ 2438,
/* 2432 */ 50346,
/* 2433 */ 2606,
/* 2434 */ 2613,
/* 2435 */ 50059,
/* 2436 */ -2296,
/* 2437 */ 181,
/* 2438 */ 2442,
/* 2439 */ 530,
/* 2440 */ -2296,
/* 2441 */ 182,
/* 2442 */ 2446,
/* 2443 */ 5623,
/* 2444 */ -2296,
/* 2445 */ 183,
/* 2446 */ 0,
/* 2447 */ 3387,
/* 2448 */ -2296,
/* 2449 */ 184,
/* 2450 */ 2454,
/* 2451 */ 50058,
/* 2452 */ -2450,
/* 2453 */ 637,
/* 2454 */ 0,
/* 2455 */ -2450,
/* 2456 */ 1086,
/* 2457 */ 2461,
/* 2458 */ 50058,
/* 2459 */ -2457,
/* 2460 */ 638,
/* 2461 */ 0,
/* 2462 */ -2457,
/* 2463 */ 1087,
/* 2464 */ 2468,
/* 2465 */ 50058,
/* 2466 */ -2464,
/* 2467 */ 639,
/* 2468 */ 0,
/* 2469 */ -2464,
/* 2470 */ 1088,
/* 2471 */ 2475,
/* 2472 */ 50305,
/* 2473 */ -2471,
/* 2474 */ 640,
/* 2475 */ 0,
/* 2476 */ -2471,
/* 2477 */ 1089,
/* 2478 */ 2482,
/* 2479 */ 50058,
/* 2480 */ -2478,
/* 2481 */ 641,
/* 2482 */ 0,
/* 2483 */ -2478,
/* 2484 */ 1090,
/* 2485 */ 2489,
/* 2486 */ 50037,
/* 2487 */ -2485,
/* 2488 */ 642,
/* 2489 */ 0,
/* 2490 */ -2485,
/* 2491 */ 1091,
/* 2492 */ 2496,
/* 2493 */ 50058,
/* 2494 */ -2492,
/* 2495 */ 643,
/* 2496 */ 0,
/* 2497 */ -2492,
/* 2498 */ 1092,
/* 2499 */ 2505,
/* 2500 */ 50040,
/* 2501 */ 165,
/* 2502 */ 50041,
/* 2503 */ -2499,
/* 2504 */ 644,
/* 2505 */ 0,
/* 2506 */ -2499,
/* 2507 */ 1093,
/* 2508 */ 2512,
/* 2509 */ 50058,
/* 2510 */ -2508,
/* 2511 */ 645,
/* 2512 */ 0,
/* 2513 */ -2508,
/* 2514 */ 1094,
/* 2515 */ 2519,
/* 2516 */ 50401,
/* 2517 */ -2515,
/* 2518 */ 646,
/* 2519 */ 0,
/* 2520 */ -2515,
/* 2521 */ 1095,
/* 2522 */ 2526,
/* 2523 */ 50058,
/* 2524 */ -2522,
/* 2525 */ 647,
/* 2526 */ 0,
/* 2527 */ -2522,
/* 2528 */ 1096,
/* 2529 */ 2533,
/* 2530 */ 50058,
/* 2531 */ -2529,
/* 2532 */ 648,
/* 2533 */ 0,
/* 2534 */ -2529,
/* 2535 */ 1097,
/* 2536 */ 2540,
/* 2537 */ 50058,
/* 2538 */ -2536,
/* 2539 */ 649,
/* 2540 */ 0,
/* 2541 */ -2536,
/* 2542 */ 1098,
/* 2543 */ 2547,
/* 2544 */ 50058,
/* 2545 */ -2543,
/* 2546 */ 650,
/* 2547 */ 0,
/* 2548 */ -2543,
/* 2549 */ 1099,
/* 2550 */ 2554,
/* 2551 */ 50058,
/* 2552 */ -2550,
/* 2553 */ 651,
/* 2554 */ 0,
/* 2555 */ -2550,
/* 2556 */ 1100,
/* 2557 */ 2561,
/* 2558 */ 50058,
/* 2559 */ -2557,
/* 2560 */ 652,
/* 2561 */ 0,
/* 2562 */ -2557,
/* 2563 */ 1101,
/* 2564 */ 2568,
/* 2565 */ 50058,
/* 2566 */ -2564,
/* 2567 */ 653,
/* 2568 */ 0,
/* 2569 */ -2564,
/* 2570 */ 1102,
/* 2571 */ 2575,
/* 2572 */ 50058,
/* 2573 */ -2571,
/* 2574 */ 654,
/* 2575 */ 0,
/* 2576 */ -2571,
/* 2577 */ 1103,
/* 2578 */ 2582,
/* 2579 */ 50058,
/* 2580 */ -2578,
/* 2581 */ 655,
/* 2582 */ 0,
/* 2583 */ -2578,
/* 2584 */ 1104,
/* 2585 */ 2589,
/* 2586 */ 50058,
/* 2587 */ -2585,
/* 2588 */ 656,
/* 2589 */ 0,
/* 2590 */ -2585,
/* 2591 */ 1105,
/* 2592 */ 2596,
/* 2593 */ 50058,
/* 2594 */ -2592,
/* 2595 */ 657,
/* 2596 */ 0,
/* 2597 */ -2592,
/* 2598 */ 1106,
/* 2599 */ 2603,
/* 2600 */ 50058,
/* 2601 */ -2599,
/* 2602 */ 658,
/* 2603 */ 0,
/* 2604 */ -2599,
/* 2605 */ 1107,
/* 2606 */ 2610,
/* 2607 */ 50058,
/* 2608 */ -2606,
/* 2609 */ 659,
/* 2610 */ 0,
/* 2611 */ -2606,
/* 2612 */ 1108,
/* 2613 */ 2619,
/* 2614 */ 215,
/* 2615 */ 2622,
/* 2616 */ 2613,
/* 2617 */ -2613,
/* 2618 */ 662,
/* 2619 */ 0,
/* 2620 */ -2613,
/* 2621 */ 1110,
/* 2622 */ 2626,
/* 2623 */ 50044,
/* 2624 */ -2622,
/* 2625 */ 660,
/* 2626 */ 2630,
/* 2627 */ 50124,
/* 2628 */ -2622,
/* 2629 */ 661,
/* 2630 */ 0,
/* 2631 */ -2622,
/* 2632 */ 1109,
/* 2633 */ 0,
/* 2634 */ 2637,
/* 2635 */ -2633,
/* 2636 */ 185,
/* 2637 */ 2643,
/* 2638 */ 215,
/* 2639 */ 2646,
/* 2640 */ 2637,
/* 2641 */ -2637,
/* 2642 */ 665,
/* 2643 */ 0,
/* 2644 */ -2637,
/* 2645 */ 1112,
/* 2646 */ 2650,
/* 2647 */ 50044,
/* 2648 */ -2646,
/* 2649 */ 663,
/* 2650 */ 2654,
/* 2651 */ 50124,
/* 2652 */ -2646,
/* 2653 */ 664,
/* 2654 */ 0,
/* 2655 */ -2646,
/* 2656 */ 1111,
/* 2657 */ 0,
/* 2658 */ 50261,
/* 2659 */ -2657,
/* 2660 */ 186,
/* 2661 */ 0,
/* 2662 */ 50413,
/* 2663 */ 50257,
/* 2664 */ 2670,
/* 2665 */ 50123,
/* 2666 */ 2678,
/* 2667 */ 50125,
/* 2668 */ -2661,
/* 2669 */ 187,
/* 2670 */ 2675,
/* 2671 */ 50058,
/* 2672 */ 155,
/* 2673 */ -2670,
/* 2674 */ 666,
/* 2675 */ 0,
/* 2676 */ -2670,
/* 2677 */ 1113,
/* 2678 */ 2683,
/* 2679 */ 2686,
/* 2680 */ 2678,
/* 2681 */ -2678,
/* 2682 */ 667,
/* 2683 */ 0,
/* 2684 */ -2678,
/* 2685 */ 1114,
/* 2686 */ 2693,
/* 2687 */ 50323,
/* 2688 */ 2771,
/* 2689 */ 50257,
/* 2690 */ 50059,
/* 2691 */ -2686,
/* 2692 */ 188,
/* 2693 */ 2698,
/* 2694 */ 2778,
/* 2695 */ 50059,
/* 2696 */ -2686,
/* 2697 */ 189,
/* 2698 */ 2705,
/* 2699 */ 50355,
/* 2700 */ 2803,
/* 2701 */ 2810,
/* 2702 */ 50059,
/* 2703 */ -2686,
/* 2704 */ 190,
/* 2705 */ 2712,
/* 2706 */ 50384,
/* 2707 */ 2826,
/* 2708 */ 155,
/* 2709 */ 50059,
/* 2710 */ -2686,
/* 2711 */ 191,
/* 2712 */ 2722,
/* 2713 */ 50336,
/* 2714 */ 2833,
/* 2715 */ 2840,
/* 2716 */ 155,
/* 2717 */ 50044,
/* 2718 */ 155,
/* 2719 */ 50059,
/* 2720 */ -2686,
/* 2721 */ 192,
/* 2722 */ 2734,
/* 2723 */ 50336,
/* 2724 */ 2847,
/* 2725 */ 2854,
/* 2726 */ 155,
/* 2727 */ 50044,
/* 2728 */ 155,
/* 2729 */ 50044,
/* 2730 */ 155,
/* 2731 */ 50059,
/* 2732 */ -2686,
/* 2733 */ 193,
/* 2734 */ 2741,
/* 2735 */ 50427,
/* 2736 */ 2861,
/* 2737 */ 2868,
/* 2738 */ 50059,
/* 2739 */ -2686,
/* 2740 */ 194,
/* 2741 */ 2748,
/* 2742 */ 215,
/* 2743 */ 2884,
/* 2744 */ 50257,
/* 2745 */ 50059,
/* 2746 */ -2686,
/* 2747 */ 195,
/* 2748 */ 2755,
/* 2749 */ 50346,
/* 2750 */ 2891,
/* 2751 */ 2898,
/* 2752 */ 50059,
/* 2753 */ -2686,
/* 2754 */ 196,
/* 2755 */ 2759,
/* 2756 */ 425,
/* 2757 */ -2686,
/* 2758 */ 197,
/* 2759 */ 2763,
/* 2760 */ 3387,
/* 2761 */ -2686,
/* 2762 */ 198,
/* 2763 */ 2767,
/* 2764 */ 530,
/* 2765 */ -2686,
/* 2766 */ 199,
/* 2767 */ 0,
/* 2768 */ 5623,
/* 2769 */ -2686,
/* 2770 */ 200,
/* 2771 */ 2775,
/* 2772 */ 50058,
/* 2773 */ -2771,
/* 2774 */ 668,
/* 2775 */ 0,
/* 2776 */ -2771,
/* 2777 */ 1115,
/* 2778 */ 2786,
/* 2779 */ 50276,
/* 2780 */ 2789,
/* 2781 */ 165,
/* 2782 */ 2796,
/* 2783 */ 2778,
/* 2784 */ -2778,
/* 2785 */ 671,
/* 2786 */ 0,
/* 2787 */ -2778,
/* 2788 */ 1118,
/* 2789 */ 2793,
/* 2790 */ 50058,
/* 2791 */ -2789,
/* 2792 */ 669,
/* 2793 */ 0,
/* 2794 */ -2789,
/* 2795 */ 1116,
/* 2796 */ 2800,
/* 2797 */ 50044,
/* 2798 */ -2796,
/* 2799 */ 670,
/* 2800 */ 0,
/* 2801 */ -2796,
/* 2802 */ 1117,
/* 2803 */ 2807,
/* 2804 */ 50058,
/* 2805 */ -2803,
/* 2806 */ 672,
/* 2807 */ 0,
/* 2808 */ -2803,
/* 2809 */ 1119,
/* 2810 */ 2816,
/* 2811 */ 3221,
/* 2812 */ 2819,
/* 2813 */ 2810,
/* 2814 */ -2810,
/* 2815 */ 674,
/* 2816 */ 0,
/* 2817 */ -2810,
/* 2818 */ 1121,
/* 2819 */ 2823,
/* 2820 */ 50044,
/* 2821 */ -2819,
/* 2822 */ 673,
/* 2823 */ 0,
/* 2824 */ -2819,
/* 2825 */ 1120,
/* 2826 */ 2830,
/* 2827 */ 50058,
/* 2828 */ -2826,
/* 2829 */ 675,
/* 2830 */ 0,
/* 2831 */ -2826,
/* 2832 */ 1122,
/* 2833 */ 2837,
/* 2834 */ 50306,
/* 2835 */ -2833,
/* 2836 */ 676,
/* 2837 */ 0,
/* 2838 */ -2833,
/* 2839 */ 1123,
/* 2840 */ 2844,
/* 2841 */ 50058,
/* 2842 */ -2840,
/* 2843 */ 677,
/* 2844 */ 0,
/* 2845 */ -2840,
/* 2846 */ 1124,
/* 2847 */ 2851,
/* 2848 */ 50306,
/* 2849 */ -2847,
/* 2850 */ 678,
/* 2851 */ 0,
/* 2852 */ -2847,
/* 2853 */ 1125,
/* 2854 */ 2858,
/* 2855 */ 50058,
/* 2856 */ -2854,
/* 2857 */ 679,
/* 2858 */ 0,
/* 2859 */ -2854,
/* 2860 */ 1126,
/* 2861 */ 2865,
/* 2862 */ 50058,
/* 2863 */ -2861,
/* 2864 */ 680,
/* 2865 */ 0,
/* 2866 */ -2861,
/* 2867 */ 1127,
/* 2868 */ 2874,
/* 2869 */ 215,
/* 2870 */ 2877,
/* 2871 */ 2868,
/* 2872 */ -2868,
/* 2873 */ 682,
/* 2874 */ 0,
/* 2875 */ -2868,
/* 2876 */ 1129,
/* 2877 */ 2881,
/* 2878 */ 50044,
/* 2879 */ -2877,
/* 2880 */ 681,
/* 2881 */ 0,
/* 2882 */ -2877,
/* 2883 */ 1128,
/* 2884 */ 2888,
/* 2885 */ 50058,
/* 2886 */ -2884,
/* 2887 */ 683,
/* 2888 */ 0,
/* 2889 */ -2884,
/* 2890 */ 1130,
/* 2891 */ 2895,
/* 2892 */ 50058,
/* 2893 */ -2891,
/* 2894 */ 684,
/* 2895 */ 0,
/* 2896 */ -2891,
/* 2897 */ 1131,
/* 2898 */ 2904,
/* 2899 */ 215,
/* 2900 */ 2907,
/* 2901 */ 2898,
/* 2902 */ -2898,
/* 2903 */ 687,
/* 2904 */ 0,
/* 2905 */ -2898,
/* 2906 */ 1133,
/* 2907 */ 2911,
/* 2908 */ 50124,
/* 2909 */ -2907,
/* 2910 */ 685,
/* 2911 */ 2915,
/* 2912 */ 50044,
/* 2913 */ -2907,
/* 2914 */ 686,
/* 2915 */ 0,
/* 2916 */ -2907,
/* 2917 */ 1132,
/* 2918 */ 0,
/* 2919 */ 50306,
/* 2920 */ 50257,
/* 2921 */ 50123,
/* 2922 */ 2926,
/* 2923 */ 50125,
/* 2924 */ -2918,
/* 2925 */ 201,
/* 2926 */ 2931,
/* 2927 */ 2934,
/* 2928 */ 2926,
/* 2929 */ -2926,
/* 2930 */ 688,
/* 2931 */ 0,
/* 2932 */ -2926,
/* 2933 */ 1134,
/* 2934 */ 2941,
/* 2935 */ 50323,
/* 2936 */ 3027,
/* 2937 */ 50257,
/* 2938 */ 50059,
/* 2939 */ -2934,
/* 2940 */ 202,
/* 2941 */ 2949,
/* 2942 */ 50360,
/* 2943 */ 3034,
/* 2944 */ 3041,
/* 2945 */ 215,
/* 2946 */ 50059,
/* 2947 */ -2934,
/* 2948 */ 203,
/* 2949 */ 2957,
/* 2950 */ 50377,
/* 2951 */ 3048,
/* 2952 */ 3055,
/* 2953 */ 215,
/* 2954 */ 50059,
/* 2955 */ -2934,
/* 2956 */ 204,
/* 2957 */ 2966,
/* 2958 */ 50320,
/* 2959 */ 3062,
/* 2960 */ 215,
/* 2961 */ 50047,
/* 2962 */ 215,
/* 2963 */ 50059,
/* 2964 */ -2934,
/* 2965 */ 205,
/* 2966 */ 2973,
/* 2967 */ 50294,
/* 2968 */ 3069,
/* 2969 */ 3076,
/* 2970 */ 50059,
/* 2971 */ -2934,
/* 2972 */ 206,
/* 2973 */ 2980,
/* 2974 */ 50424,
/* 2975 */ 3093,
/* 2976 */ 3100,
/* 2977 */ 50059,
/* 2978 */ -2934,
/* 2979 */ 207,
/* 2980 */ 2987,
/* 2981 */ 50355,
/* 2982 */ 3117,
/* 2983 */ 3124,
/* 2984 */ 50059,
/* 2985 */ -2934,
/* 2986 */ 208,
/* 2987 */ 2997,
/* 2988 */ 50427,
/* 2989 */ 3140,
/* 2990 */ 215,
/* 2991 */ 3144,
/* 2992 */ 3148,
/* 2993 */ 3155,
/* 2994 */ 50059,
/* 2995 */ -2934,
/* 2996 */ 209,
/* 2997 */ 3004,
/* 2998 */ 50406,
/* 2999 */ 3171,
/* 3000 */ 3178,
/* 3001 */ 50059,
/* 3002 */ -2934,
/* 3003 */ 210,
/* 3004 */ 3011,
/* 3005 */ 50346,
/* 3006 */ 3194,
/* 3007 */ 3201,
/* 3008 */ 50059,
/* 3009 */ -2934,
/* 3010 */ 211,
/* 3011 */ 3015,
/* 3012 */ 425,
/* 3013 */ -2934,
/* 3014 */ 212,
/* 3015 */ 3019,
/* 3016 */ 530,
/* 3017 */ -2934,
/* 3018 */ 213,
/* 3019 */ 3023,
/* 3020 */ 3387,
/* 3021 */ -2934,
/* 3022 */ 214,
/* 3023 */ 0,
/* 3024 */ 5623,
/* 3025 */ -2934,
/* 3026 */ 215,
/* 3027 */ 3031,
/* 3028 */ 50058,
/* 3029 */ -3027,
/* 3030 */ 689,
/* 3031 */ 0,
/* 3032 */ -3027,
/* 3033 */ 1135,
/* 3034 */ 3038,
/* 3035 */ 50058,
/* 3036 */ -3034,
/* 3037 */ 690,
/* 3038 */ 0,
/* 3039 */ -3034,
/* 3040 */ 1136,
/* 3041 */ 3045,
/* 3042 */ 50100,
/* 3043 */ -3041,
/* 3044 */ 691,
/* 3045 */ 0,
/* 3046 */ -3041,
/* 3047 */ 1137,
/* 3048 */ 3052,
/* 3049 */ 50058,
/* 3050 */ -3048,
/* 3051 */ 692,
/* 3052 */ 0,
/* 3053 */ -3048,
/* 3054 */ 1138,
/* 3055 */ 3059,
/* 3056 */ 50100,
/* 3057 */ -3055,
/* 3058 */ 693,
/* 3059 */ 0,
/* 3060 */ -3055,
/* 3061 */ 1139,
/* 3062 */ 3066,
/* 3063 */ 50058,
/* 3064 */ -3062,
/* 3065 */ 694,
/* 3066 */ 0,
/* 3067 */ -3062,
/* 3068 */ 1140,
/* 3069 */ 3073,
/* 3070 */ 50058,
/* 3071 */ -3069,
/* 3072 */ 695,
/* 3073 */ 0,
/* 3074 */ -3069,
/* 3075 */ 1141,
/* 3076 */ 3083,
/* 3077 */ 215,
/* 3078 */ 215,
/* 3079 */ 3086,
/* 3080 */ 3076,
/* 3081 */ -3076,
/* 3082 */ 697,
/* 3083 */ 0,
/* 3084 */ -3076,
/* 3085 */ 1143,
/* 3086 */ 3090,
/* 3087 */ 50044,
/* 3088 */ -3086,
/* 3089 */ 696,
/* 3090 */ 0,
/* 3091 */ -3086,
/* 3092 */ 1142,
/* 3093 */ 3097,
/* 3094 */ 50058,
/* 3095 */ -3093,
/* 3096 */ 698,
/* 3097 */ 0,
/* 3098 */ -3093,
/* 3099 */ 1144,
/* 3100 */ 3107,
/* 3101 */ 215,
/* 3102 */ 215,
/* 3103 */ 3110,
/* 3104 */ 3100,
/* 3105 */ -3100,
/* 3106 */ 700,
/* 3107 */ 0,
/* 3108 */ -3100,
/* 3109 */ 1146,
/* 3110 */ 3114,
/* 3111 */ 50044,
/* 3112 */ -3110,
/* 3113 */ 699,
/* 3114 */ 0,
/* 3115 */ -3110,
/* 3116 */ 1145,
/* 3117 */ 3121,
/* 3118 */ 50058,
/* 3119 */ -3117,
/* 3120 */ 701,
/* 3121 */ 0,
/* 3122 */ -3117,
/* 3123 */ 1147,
/* 3124 */ 3130,
/* 3125 */ 3221,
/* 3126 */ 3133,
/* 3127 */ 3124,
/* 3128 */ -3124,
/* 3129 */ 703,
/* 3130 */ 0,
/* 3131 */ -3124,
/* 3132 */ 1149,
/* 3133 */ 3137,
/* 3134 */ 50044,
/* 3135 */ -3133,
/* 3136 */ 702,
/* 3137 */ 0,
/* 3138 */ -3133,
/* 3139 */ 1148,
/* 3140 */ 0,
/* 3141 */ 50091,
/* 3142 */ -3140,
/* 3143 */ 704,
/* 3144 */ 0,
/* 3145 */ 50093,
/* 3146 */ -3144,
/* 3147 */ 705,
/* 3148 */ 3152,
/* 3149 */ 50058,
/* 3150 */ -3148,
/* 3151 */ 706,
/* 3152 */ 0,
/* 3153 */ -3148,
/* 3154 */ 1150,
/* 3155 */ 3161,
/* 3156 */ 215,
/* 3157 */ 3164,
/* 3158 */ 3155,
/* 3159 */ -3155,
/* 3160 */ 708,
/* 3161 */ 0,
/* 3162 */ -3155,
/* 3163 */ 1152,
/* 3164 */ 3168,
/* 3165 */ 50044,
/* 3166 */ -3164,
/* 3167 */ 707,
/* 3168 */ 0,
/* 3169 */ -3164,
/* 3170 */ 1151,
/* 3171 */ 3175,
/* 3172 */ 50058,
/* 3173 */ -3171,
/* 3174 */ 709,
/* 3175 */ 0,
/* 3176 */ -3171,
/* 3177 */ 1153,
/* 3178 */ 3184,
/* 3179 */ 215,
/* 3180 */ 3187,
/* 3181 */ 3178,
/* 3182 */ -3178,
/* 3183 */ 711,
/* 3184 */ 0,
/* 3185 */ -3178,
/* 3186 */ 1155,
/* 3187 */ 3191,
/* 3188 */ 50044,
/* 3189 */ -3187,
/* 3190 */ 710,
/* 3191 */ 0,
/* 3192 */ -3187,
/* 3193 */ 1154,
/* 3194 */ 3198,
/* 3195 */ 50058,
/* 3196 */ -3194,
/* 3197 */ 712,
/* 3198 */ 0,
/* 3199 */ -3194,
/* 3200 */ 1156,
/* 3201 */ 3207,
/* 3202 */ 215,
/* 3203 */ 3210,
/* 3204 */ 3201,
/* 3205 */ -3201,
/* 3206 */ 715,
/* 3207 */ 0,
/* 3208 */ -3201,
/* 3209 */ 1158,
/* 3210 */ 3214,
/* 3211 */ 50124,
/* 3212 */ -3210,
/* 3213 */ 713,
/* 3214 */ 3218,
/* 3215 */ 50044,
/* 3216 */ -3210,
/* 3217 */ 714,
/* 3218 */ 0,
/* 3219 */ -3210,
/* 3220 */ 1157,
/* 3221 */ 3231,
/* 3222 */ 50281,
/* 3223 */ 50091,
/* 3224 */ 215,
/* 3225 */ 3251,
/* 3226 */ 50093,
/* 3227 */ 50293,
/* 3228 */ 3289,
/* 3229 */ -3221,
/* 3230 */ 216,
/* 3231 */ 3241,
/* 3232 */ 50435,
/* 3233 */ 50091,
/* 3234 */ 215,
/* 3235 */ 3258,
/* 3236 */ 50093,
/* 3237 */ 50293,
/* 3238 */ 3289,
/* 3239 */ -3221,
/* 3240 */ 217,
/* 3241 */ 0,
/* 3242 */ 50343,
/* 3243 */ 50091,
/* 3244 */ 215,
/* 3245 */ 3265,
/* 3246 */ 50093,
/* 3247 */ 50293,
/* 3248 */ 3289,
/* 3249 */ -3221,
/* 3250 */ 218,
/* 3251 */ 3255,
/* 3252 */ 3272,
/* 3253 */ -3251,
/* 3254 */ 716,
/* 3255 */ 0,
/* 3256 */ -3251,
/* 3257 */ 1159,
/* 3258 */ 3262,
/* 3259 */ 3272,
/* 3260 */ -3258,
/* 3261 */ 717,
/* 3262 */ 0,
/* 3263 */ -3258,
/* 3264 */ 1160,
/* 3265 */ 3269,
/* 3266 */ 3272,
/* 3267 */ -3265,
/* 3268 */ 718,
/* 3269 */ 0,
/* 3270 */ -3265,
/* 3271 */ 1161,
/* 3272 */ 3277,
/* 3273 */ 50044,
/* 3274 */ 215,
/* 3275 */ -3272,
/* 3276 */ 219,
/* 3277 */ 3282,
/* 3278 */ 50044,
/* 3279 */ 155,
/* 3280 */ -3272,
/* 3281 */ 220,
/* 3282 */ 0,
/* 3283 */ 50044,
/* 3284 */ 215,
/* 3285 */ 50044,
/* 3286 */ 215,
/* 3287 */ -3272,
/* 3288 */ 221,
/* 3289 */ 3294,
/* 3290 */ 215,
/* 3291 */ 3324,
/* 3292 */ -3289,
/* 3293 */ 222,
/* 3294 */ 3300,
/* 3295 */ 50339,
/* 3296 */ 215,
/* 3297 */ 50370,
/* 3298 */ -3289,
/* 3299 */ 223,
/* 3300 */ 3305,
/* 3301 */ 50339,
/* 3302 */ 50370,
/* 3303 */ -3289,
/* 3304 */ 224,
/* 3305 */ 3314,
/* 3306 */ 50339,
/* 3307 */ 50370,
/* 3308 */ 50434,
/* 3309 */ 50293,
/* 3310 */ 215,
/* 3311 */ 3331,
/* 3312 */ -3289,
/* 3313 */ 225,
/* 3314 */ 0,
/* 3315 */ 50339,
/* 3316 */ 215,
/* 3317 */ 50370,
/* 3318 */ 50434,
/* 3319 */ 50293,
/* 3320 */ 215,
/* 3321 */ 3338,
/* 3322 */ -3289,
/* 3323 */ 226,
/* 3324 */ 3328,
/* 3325 */ 50370,
/* 3326 */ -3324,
/* 3327 */ 719,
/* 3328 */ 0,
/* 3329 */ -3324,
/* 3330 */ 1162,
/* 3331 */ 3335,
/* 3332 */ 50370,
/* 3333 */ -3331,
/* 3334 */ 720,
/* 3335 */ 0,
/* 3336 */ -3331,
/* 3337 */ 1163,
/* 3338 */ 3342,
/* 3339 */ 50370,
/* 3340 */ -3338,
/* 3341 */ 721,
/* 3342 */ 0,
/* 3343 */ -3338,
/* 3344 */ 1164,
/* 3345 */ 0,
/* 3346 */ 50329,
/* 3347 */ 50257,
/* 3348 */ 50123,
/* 3349 */ 3353,
/* 3350 */ 50125,
/* 3351 */ -3345,
/* 3352 */ 227,
/* 3353 */ 3358,
/* 3354 */ 3361,
/* 3355 */ 3353,
/* 3356 */ -3353,
/* 3357 */ 722,
/* 3358 */ 0,
/* 3359 */ -3353,
/* 3360 */ 1165,
/* 3361 */ 3368,
/* 3362 */ 50323,
/* 3363 */ 3380,
/* 3364 */ 50257,
/* 3365 */ 50059,
/* 3366 */ -3361,
/* 3367 */ 228,
/* 3368 */ 3372,
/* 3369 */ 3387,
/* 3370 */ -3361,
/* 3371 */ 229,
/* 3372 */ 3376,
/* 3373 */ 530,
/* 3374 */ -3361,
/* 3375 */ 230,
/* 3376 */ 0,
/* 3377 */ 5623,
/* 3378 */ -3361,
/* 3379 */ 231,
/* 3380 */ 3384,
/* 3381 */ 50058,
/* 3382 */ -3380,
/* 3383 */ 723,
/* 3384 */ 0,
/* 3385 */ -3380,
/* 3386 */ 1166,
/* 3387 */ 3394,
/* 3388 */ 50308,
/* 3389 */ 3407,
/* 3390 */ 3414,
/* 3391 */ 50059,
/* 3392 */ -3387,
/* 3393 */ 232,
/* 3394 */ 3400,
/* 3395 */ 50373,
/* 3396 */ 3447,
/* 3397 */ 3454,
/* 3398 */ -3387,
/* 3399 */ 233,
/* 3400 */ 0,
/* 3401 */ 50429,
/* 3402 */ 3481,
/* 3403 */ 3488,
/* 3404 */ 50059,
/* 3405 */ -3387,
/* 3406 */ 234,
/* 3407 */ 3411,
/* 3408 */ 50058,
/* 3409 */ -3407,
/* 3410 */ 724,
/* 3411 */ 0,
/* 3412 */ -3407,
/* 3413 */ 1167,
/* 3414 */ 3422,
/* 3415 */ 50042,
/* 3416 */ 215,
/* 3417 */ 215,
/* 3418 */ 3433,
/* 3419 */ 3414,
/* 3420 */ -3414,
/* 3421 */ 726,
/* 3422 */ 3430,
/* 3423 */ 50042,
/* 3424 */ 215,
/* 3425 */ 155,
/* 3426 */ 3440,
/* 3427 */ 3414,
/* 3428 */ -3414,
/* 3429 */ 728,
/* 3430 */ 0,
/* 3431 */ -3414,
/* 3432 */ 1170,
/* 3433 */ 3437,
/* 3434 */ 50044,
/* 3435 */ -3433,
/* 3436 */ 725,
/* 3437 */ 0,
/* 3438 */ -3433,
/* 3439 */ 1168,
/* 3440 */ 3444,
/* 3441 */ 50044,
/* 3442 */ -3440,
/* 3443 */ 727,
/* 3444 */ 0,
/* 3445 */ -3440,
/* 3446 */ 1169,
/* 3447 */ 3451,
/* 3448 */ 50058,
/* 3449 */ -3447,
/* 3450 */ 729,
/* 3451 */ 0,
/* 3452 */ -3447,
/* 3453 */ 1171,
/* 3454 */ 3462,
/* 3455 */ 50042,
/* 3456 */ 229,
/* 3457 */ 3465,
/* 3458 */ 3473,
/* 3459 */ 3454,
/* 3460 */ -3454,
/* 3461 */ 733,
/* 3462 */ 0,
/* 3463 */ -3454,
/* 3464 */ 1173,
/* 3465 */ 3470,
/* 3466 */ 3535,
/* 3467 */ 3465,
/* 3468 */ -3465,
/* 3469 */ 730,
/* 3470 */ 0,
/* 3471 */ -3465,
/* 3472 */ 1172,
/* 3473 */ 3477,
/* 3474 */ 50044,
/* 3475 */ -3473,
/* 3476 */ 731,
/* 3477 */ 0,
/* 3478 */ 50059,
/* 3479 */ -3473,
/* 3480 */ 732,
/* 3481 */ 3485,
/* 3482 */ 50058,
/* 3483 */ -3481,
/* 3484 */ 734,
/* 3485 */ 0,
/* 3486 */ -3481,
/* 3487 */ 1174,
/* 3488 */ 3496,
/* 3489 */ 50042,
/* 3490 */ 3499,
/* 3491 */ 3507,
/* 3492 */ 3528,
/* 3493 */ 3488,
/* 3494 */ -3488,
/* 3495 */ 740,
/* 3496 */ 0,
/* 3497 */ -3488,
/* 3498 */ 1177,
/* 3499 */ 3504,
/* 3500 */ 229,
/* 3501 */ 50037,
/* 3502 */ -3499,
/* 3503 */ 735,
/* 3504 */ 0,
/* 3505 */ -3499,
/* 3506 */ 1175,
/* 3507 */ 3514,
/* 3508 */ 155,
/* 3509 */ 50293,
/* 3510 */ 50370,
/* 3511 */ 215,
/* 3512 */ -3507,
/* 3513 */ 736,
/* 3514 */ 3521,
/* 3515 */ 155,
/* 3516 */ 50293,
/* 3517 */ 215,
/* 3518 */ 50370,
/* 3519 */ -3507,
/* 3520 */ 737,
/* 3521 */ 0,
/* 3522 */ 155,
/* 3523 */ 50293,
/* 3524 */ 50370,
/* 3525 */ 278,
/* 3526 */ -3507,
/* 3527 */ 738,
/* 3528 */ 3532,
/* 3529 */ 50044,
/* 3530 */ -3528,
/* 3531 */ 739,
/* 3532 */ 0,
/* 3533 */ -3528,
/* 3534 */ 1176,
/* 3535 */ 3539,
/* 3536 */ 155,
/* 3537 */ -3535,
/* 3538 */ 235,
/* 3539 */ 3543,
/* 3540 */ 229,
/* 3541 */ -3535,
/* 3542 */ 236,
/* 3543 */ 0,
/* 3544 */ 50257,
/* 3545 */ -3535,
/* 3546 */ 237,
/* 3547 */ 0,
/* 3548 */ 50291,
/* 3549 */ 50257,
/* 3550 */ 50123,
/* 3551 */ 3555,
/* 3552 */ 50125,
/* 3553 */ -3547,
/* 3554 */ 238,
/* 3555 */ 3560,
/* 3556 */ 3563,
/* 3557 */ 3555,
/* 3558 */ -3555,
/* 3559 */ 741,
/* 3560 */ 0,
/* 3561 */ -3555,
/* 3562 */ 1178,
/* 3563 */ 3570,
/* 3564 */ 50323,
/* 3565 */ 3639,
/* 3566 */ 50257,
/* 3567 */ 50059,
/* 3568 */ -3563,
/* 3569 */ 239,
/* 3570 */ 3577,
/* 3571 */ 50368,
/* 3572 */ 3646,
/* 3573 */ 155,
/* 3574 */ 50059,
/* 3575 */ -3563,
/* 3576 */ 240,
/* 3577 */ 3582,
/* 3578 */ 3653,
/* 3579 */ 50059,
/* 3580 */ -3563,
/* 3581 */ 241,
/* 3582 */ 3589,
/* 3583 */ 50419,
/* 3584 */ 3678,
/* 3585 */ 3685,
/* 3586 */ 50059,
/* 3587 */ -3563,
/* 3588 */ 242,
/* 3589 */ 3596,
/* 3590 */ 50439,
/* 3591 */ 3701,
/* 3592 */ 3708,
/* 3593 */ 50059,
/* 3594 */ -3563,
/* 3595 */ 243,
/* 3596 */ 3603,
/* 3597 */ 50410,
/* 3598 */ 3724,
/* 3599 */ 3731,
/* 3600 */ 50059,
/* 3601 */ -3563,
/* 3602 */ 244,
/* 3603 */ 3610,
/* 3604 */ 50334,
/* 3605 */ 50349,
/* 3606 */ 3747,
/* 3607 */ 50059,
/* 3608 */ -3563,
/* 3609 */ 245,
/* 3610 */ 3617,
/* 3611 */ 50465,
/* 3612 */ 50349,
/* 3613 */ 3760,
/* 3614 */ 50059,
/* 3615 */ -3563,
/* 3616 */ 246,
/* 3617 */ 3624,
/* 3618 */ 50362,
/* 3619 */ 50349,
/* 3620 */ 3773,
/* 3621 */ 50059,
/* 3622 */ -3563,
/* 3623 */ 247,
/* 3624 */ 3631,
/* 3625 */ 50367,
/* 3626 */ 50349,
/* 3627 */ 3786,
/* 3628 */ 50059,
/* 3629 */ -3563,
/* 3630 */ 248,
/* 3631 */ 3635,
/* 3632 */ 530,
/* 3633 */ -3563,
/* 3634 */ 249,
/* 3635 */ 0,
/* 3636 */ 5623,
/* 3637 */ -3563,
/* 3638 */ 250,
/* 3639 */ 3643,
/* 3640 */ 50058,
/* 3641 */ -3639,
/* 3642 */ 742,
/* 3643 */ 0,
/* 3644 */ -3639,
/* 3645 */ 1179,
/* 3646 */ 3650,
/* 3647 */ 50058,
/* 3648 */ -3646,
/* 3649 */ 743,
/* 3650 */ 0,
/* 3651 */ -3646,
/* 3652 */ 1180,
/* 3653 */ 3661,
/* 3654 */ 50276,
/* 3655 */ 3664,
/* 3656 */ 215,
/* 3657 */ 3671,
/* 3658 */ 3653,
/* 3659 */ -3653,
/* 3660 */ 746,
/* 3661 */ 0,
/* 3662 */ -3653,
/* 3663 */ 1183,
/* 3664 */ 3668,
/* 3665 */ 50058,
/* 3666 */ -3664,
/* 3667 */ 744,
/* 3668 */ 0,
/* 3669 */ -3664,
/* 3670 */ 1181,
/* 3671 */ 3675,
/* 3672 */ 50044,
/* 3673 */ -3671,
/* 3674 */ 745,
/* 3675 */ 0,
/* 3676 */ -3671,
/* 3677 */ 1182,
/* 3678 */ 3682,
/* 3679 */ 50058,
/* 3680 */ -3678,
/* 3681 */ 747,
/* 3682 */ 0,
/* 3683 */ -3678,
/* 3684 */ 1184,
/* 3685 */ 3691,
/* 3686 */ 215,
/* 3687 */ 3694,
/* 3688 */ 3685,
/* 3689 */ -3685,
/* 3690 */ 749,
/* 3691 */ 0,
/* 3692 */ -3685,
/* 3693 */ 1186,
/* 3694 */ 3698,
/* 3695 */ 50044,
/* 3696 */ -3694,
/* 3697 */ 748,
/* 3698 */ 0,
/* 3699 */ -3694,
/* 3700 */ 1185,
/* 3701 */ 3705,
/* 3702 */ 50058,
/* 3703 */ -3701,
/* 3704 */ 750,
/* 3705 */ 0,
/* 3706 */ -3701,
/* 3707 */ 1187,
/* 3708 */ 3714,
/* 3709 */ 50276,
/* 3710 */ 3717,
/* 3711 */ 3708,
/* 3712 */ -3708,
/* 3713 */ 752,
/* 3714 */ 0,
/* 3715 */ -3708,
/* 3716 */ 1189,
/* 3717 */ 3721,
/* 3718 */ 50044,
/* 3719 */ -3717,
/* 3720 */ 751,
/* 3721 */ 0,
/* 3722 */ -3717,
/* 3723 */ 1188,
/* 3724 */ 3728,
/* 3725 */ 50058,
/* 3726 */ -3724,
/* 3727 */ 753,
/* 3728 */ 0,
/* 3729 */ -3724,
/* 3730 */ 1190,
/* 3731 */ 3737,
/* 3732 */ 215,
/* 3733 */ 3740,
/* 3734 */ 3731,
/* 3735 */ -3731,
/* 3736 */ 755,
/* 3737 */ 0,
/* 3738 */ -3731,
/* 3739 */ 1192,
/* 3740 */ 3744,
/* 3741 */ 50044,
/* 3742 */ -3740,
/* 3743 */ 754,
/* 3744 */ 0,
/* 3745 */ -3740,
/* 3746 */ 1191,
/* 3747 */ 3753,
/* 3748 */ 3799,
/* 3749 */ 3756,
/* 3750 */ 3747,
/* 3751 */ -3747,
/* 3752 */ 757,
/* 3753 */ 0,
/* 3754 */ -3747,
/* 3755 */ 1193,
/* 3756 */ 0,
/* 3757 */ 50044,
/* 3758 */ -3756,
/* 3759 */ 756,
/* 3760 */ 3766,
/* 3761 */ 3799,
/* 3762 */ 3769,
/* 3763 */ 3760,
/* 3764 */ -3760,
/* 3765 */ 759,
/* 3766 */ 0,
/* 3767 */ -3760,
/* 3768 */ 1194,
/* 3769 */ 0,
/* 3770 */ 50044,
/* 3771 */ -3769,
/* 3772 */ 758,
/* 3773 */ 3779,
/* 3774 */ 3799,
/* 3775 */ 3782,
/* 3776 */ 3773,
/* 3777 */ -3773,
/* 3778 */ 761,
/* 3779 */ 0,
/* 3780 */ -3773,
/* 3781 */ 1195,
/* 3782 */ 0,
/* 3783 */ 50044,
/* 3784 */ -3782,
/* 3785 */ 760,
/* 3786 */ 3792,
/* 3787 */ 3799,
/* 3788 */ 3795,
/* 3789 */ 3786,
/* 3790 */ -3786,
/* 3791 */ 763,
/* 3792 */ 0,
/* 3793 */ -3786,
/* 3794 */ 1196,
/* 3795 */ 0,
/* 3796 */ 50044,
/* 3797 */ -3795,
/* 3798 */ 762,
/* 3799 */ 3808,
/* 3800 */ 50435,
/* 3801 */ 50091,
/* 3802 */ 215,
/* 3803 */ 50044,
/* 3804 */ 215,
/* 3805 */ 50093,
/* 3806 */ -3799,
/* 3807 */ 251,
/* 3808 */ 3815,
/* 3809 */ 50435,
/* 3810 */ 50091,
/* 3811 */ 215,
/* 3812 */ 50093,
/* 3813 */ -3799,
/* 3814 */ 252,
/* 3815 */ 3820,
/* 3816 */ 155,
/* 3817 */ 3851,
/* 3818 */ -3799,
/* 3819 */ 253,
/* 3820 */ 3827,
/* 3821 */ 155,
/* 3822 */ 50339,
/* 3823 */ 251,
/* 3824 */ 50454,
/* 3825 */ -3799,
/* 3826 */ 254,
/* 3827 */ 3834,
/* 3828 */ 155,
/* 3829 */ 215,
/* 3830 */ 3859,
/* 3831 */ 50318,
/* 3832 */ -3799,
/* 3833 */ 255,
/* 3834 */ 3841,
/* 3835 */ 155,
/* 3836 */ 215,
/* 3837 */ 3867,
/* 3838 */ 50460,
/* 3839 */ -3799,
/* 3840 */ 256,
/* 3841 */ 3846,
/* 3842 */ 50366,
/* 3843 */ 155,
/* 3844 */ -3799,
/* 3845 */ 257,
/* 3846 */ 0,
/* 3847 */ 50343,
/* 3848 */ 215,
/* 3849 */ -3799,
/* 3850 */ 258,
/* 3851 */ 3856,
/* 3852 */ 50293,
/* 3853 */ 50278,
/* 3854 */ -3851,
/* 3855 */ 764,
/* 3856 */ 0,
/* 3857 */ -3851,
/* 3858 */ 1197,
/* 3859 */ 3863,
/* 3860 */ 50047,
/* 3861 */ -3859,
/* 3862 */ 765,
/* 3863 */ 0,
/* 3864 */ 50403,
/* 3865 */ -3859,
/* 3866 */ 766,
/* 3867 */ 3871,
/* 3868 */ 50047,
/* 3869 */ -3867,
/* 3870 */ 767,
/* 3871 */ 0,
/* 3872 */ 50403,
/* 3873 */ -3867,
/* 3874 */ 768,
/* 3875 */ 0,
/* 3876 */ 50325,
/* 3877 */ 50257,
/* 3878 */ 50058,
/* 3879 */ 215,
/* 3880 */ 50123,
/* 3881 */ 3885,
/* 3882 */ 50125,
/* 3883 */ -3875,
/* 3884 */ 259,
/* 3885 */ 3890,
/* 3886 */ 3893,
/* 3887 */ 3885,
/* 3888 */ -3885,
/* 3889 */ 769,
/* 3890 */ 0,
/* 3891 */ -3885,
/* 3892 */ 1198,
/* 3893 */ 3900,
/* 3894 */ 50432,
/* 3895 */ 3926,
/* 3896 */ 3933,
/* 3897 */ 50059,
/* 3898 */ -3893,
/* 3899 */ 260,
/* 3900 */ 3907,
/* 3901 */ 50355,
/* 3902 */ 3949,
/* 3903 */ 3956,
/* 3904 */ 50059,
/* 3905 */ -3893,
/* 3906 */ 261,
/* 3907 */ 3914,
/* 3908 */ 50323,
/* 3909 */ 3972,
/* 3910 */ 50257,
/* 3911 */ 50059,
/* 3912 */ -3893,
/* 3913 */ 262,
/* 3914 */ 3918,
/* 3915 */ 530,
/* 3916 */ -3893,
/* 3917 */ 263,
/* 3918 */ 3922,
/* 3919 */ 3387,
/* 3920 */ -3893,
/* 3921 */ 264,
/* 3922 */ 0,
/* 3923 */ 5623,
/* 3924 */ -3893,
/* 3925 */ 265,
/* 3926 */ 3930,
/* 3927 */ 50058,
/* 3928 */ -3926,
/* 3929 */ 770,
/* 3930 */ 0,
/* 3931 */ -3926,
/* 3932 */ 1199,
/* 3933 */ 3939,
/* 3934 */ 155,
/* 3935 */ 3942,
/* 3936 */ 3933,
/* 3937 */ -3933,
/* 3938 */ 772,
/* 3939 */ 0,
/* 3940 */ -3933,
/* 3941 */ 1201,
/* 3942 */ 3946,
/* 3943 */ 50044,
/* 3944 */ -3942,
/* 3945 */ 771,
/* 3946 */ 0,
/* 3947 */ -3942,
/* 3948 */ 1200,
/* 3949 */ 3953,
/* 3950 */ 50058,
/* 3951 */ -3949,
/* 3952 */ 773,
/* 3953 */ 0,
/* 3954 */ -3949,
/* 3955 */ 1202,
/* 3956 */ 3962,
/* 3957 */ 3221,
/* 3958 */ 3965,
/* 3959 */ 3956,
/* 3960 */ -3956,
/* 3961 */ 775,
/* 3962 */ 0,
/* 3963 */ -3956,
/* 3964 */ 1204,
/* 3965 */ 3969,
/* 3966 */ 50044,
/* 3967 */ -3965,
/* 3968 */ 774,
/* 3969 */ 0,
/* 3970 */ -3965,
/* 3971 */ 1203,
/* 3972 */ 3976,
/* 3973 */ 50058,
/* 3974 */ -3972,
/* 3975 */ 776,
/* 3976 */ 0,
/* 3977 */ -3972,
/* 3978 */ 1205,
/* 3979 */ 0,
/* 3980 */ 50354,
/* 3981 */ 50257,
/* 3982 */ 50123,
/* 3983 */ 3987,
/* 3984 */ 50125,
/* 3985 */ -3979,
/* 3986 */ 266,
/* 3987 */ 3992,
/* 3988 */ 3995,
/* 3989 */ 3987,
/* 3990 */ -3987,
/* 3991 */ 777,
/* 3992 */ 0,
/* 3993 */ -3987,
/* 3994 */ 1206,
/* 3995 */ 4002,
/* 3996 */ 50326,
/* 3997 */ 4040,
/* 3998 */ 4047,
/* 3999 */ 50059,
/* 4000 */ -3995,
/* 4001 */ 267,
/* 4002 */ 4007,
/* 4003 */ 50308,
/* 4004 */ 50059,
/* 4005 */ -3995,
/* 4006 */ 268,
/* 4007 */ 4014,
/* 4008 */ 50346,
/* 4009 */ 4063,
/* 4010 */ 4070,
/* 4011 */ 50059,
/* 4012 */ -3995,
/* 4013 */ 269,
/* 4014 */ 4021,
/* 4015 */ 50323,
/* 4016 */ 4090,
/* 4017 */ 50257,
/* 4018 */ 50059,
/* 4019 */ -3995,
/* 4020 */ 270,
/* 4021 */ 4028,
/* 4022 */ 50355,
/* 4023 */ 4097,
/* 4024 */ 4104,
/* 4025 */ 50059,
/* 4026 */ -3995,
/* 4027 */ 271,
/* 4028 */ 4032,
/* 4029 */ 530,
/* 4030 */ -3995,
/* 4031 */ 272,
/* 4032 */ 4036,
/* 4033 */ 3387,
/* 4034 */ -3995,
/* 4035 */ 273,
/* 4036 */ 0,
/* 4037 */ 5623,
/* 4038 */ -3995,
/* 4039 */ 274,
/* 4040 */ 4044,
/* 4041 */ 50058,
/* 4042 */ -4040,
/* 4043 */ 778,
/* 4044 */ 0,
/* 4045 */ -4040,
/* 4046 */ 1207,
/* 4047 */ 4053,
/* 4048 */ 155,
/* 4049 */ 4056,
/* 4050 */ 4047,
/* 4051 */ -4047,
/* 4052 */ 780,
/* 4053 */ 0,
/* 4054 */ -4047,
/* 4055 */ 1209,
/* 4056 */ 4060,
/* 4057 */ 50044,
/* 4058 */ -4056,
/* 4059 */ 779,
/* 4060 */ 0,
/* 4061 */ -4056,
/* 4062 */ 1208,
/* 4063 */ 4067,
/* 4064 */ 50058,
/* 4065 */ -4063,
/* 4066 */ 781,
/* 4067 */ 0,
/* 4068 */ -4063,
/* 4069 */ 1210,
/* 4070 */ 4076,
/* 4071 */ 215,
/* 4072 */ 4079,
/* 4073 */ 4070,
/* 4074 */ -4070,
/* 4075 */ 784,
/* 4076 */ 0,
/* 4077 */ -4070,
/* 4078 */ 1212,
/* 4079 */ 4083,
/* 4080 */ 50124,
/* 4081 */ -4079,
/* 4082 */ 782,
/* 4083 */ 4087,
/* 4084 */ 50044,
/* 4085 */ -4079,
/* 4086 */ 783,
/* 4087 */ 0,
/* 4088 */ -4079,
/* 4089 */ 1211,
/* 4090 */ 4094,
/* 4091 */ 50058,
/* 4092 */ -4090,
/* 4093 */ 785,
/* 4094 */ 0,
/* 4095 */ -4090,
/* 4096 */ 1213,
/* 4097 */ 4101,
/* 4098 */ 50058,
/* 4099 */ -4097,
/* 4100 */ 786,
/* 4101 */ 0,
/* 4102 */ -4097,
/* 4103 */ 1214,
/* 4104 */ 4110,
/* 4105 */ 3221,
/* 4106 */ 4113,
/* 4107 */ 4104,
/* 4108 */ -4104,
/* 4109 */ 788,
/* 4110 */ 0,
/* 4111 */ -4104,
/* 4112 */ 1216,
/* 4113 */ 4117,
/* 4114 */ 50044,
/* 4115 */ -4113,
/* 4116 */ 787,
/* 4117 */ 0,
/* 4118 */ -4113,
/* 4119 */ 1215,
/* 4120 */ 0,
/* 4121 */ 50445,
/* 4122 */ 50257,
/* 4123 */ 50123,
/* 4124 */ 4128,
/* 4125 */ 50125,
/* 4126 */ -4120,
/* 4127 */ 275,
/* 4128 */ 4133,
/* 4129 */ 4136,
/* 4130 */ 4128,
/* 4131 */ -4128,
/* 4132 */ 789,
/* 4133 */ 0,
/* 4134 */ -4128,
/* 4135 */ 1217,
/* 4136 */ 4143,
/* 4137 */ 50323,
/* 4138 */ 4188,
/* 4139 */ 50257,
/* 4140 */ 50059,
/* 4141 */ -4136,
/* 4142 */ 276,
/* 4143 */ 4147,
/* 4144 */ 454,
/* 4145 */ -4136,
/* 4146 */ 277,
/* 4147 */ 4155,
/* 4148 */ 50385,
/* 4149 */ 4195,
/* 4150 */ 215,
/* 4151 */ 50037,
/* 4152 */ 50059,
/* 4153 */ -4136,
/* 4154 */ 278,
/* 4155 */ 4162,
/* 4156 */ 50402,
/* 4157 */ 4202,
/* 4158 */ 215,
/* 4159 */ 50059,
/* 4160 */ -4136,
/* 4161 */ 279,
/* 4162 */ 4169,
/* 4163 */ 50379,
/* 4164 */ 4209,
/* 4165 */ 215,
/* 4166 */ 50059,
/* 4167 */ -4136,
/* 4168 */ 280,
/* 4169 */ 4176,
/* 4170 */ 50346,
/* 4171 */ 4216,
/* 4172 */ 4223,
/* 4173 */ 50059,
/* 4174 */ -4136,
/* 4175 */ 281,
/* 4176 */ 4180,
/* 4177 */ 3387,
/* 4178 */ -4136,
/* 4179 */ 282,
/* 4180 */ 4184,
/* 4181 */ 530,
/* 4182 */ -4136,
/* 4183 */ 283,
/* 4184 */ 0,
/* 4185 */ 5623,
/* 4186 */ -4136,
/* 4187 */ 284,
/* 4188 */ 4192,
/* 4189 */ 50058,
/* 4190 */ -4188,
/* 4191 */ 790,
/* 4192 */ 0,
/* 4193 */ -4188,
/* 4194 */ 1218,
/* 4195 */ 4199,
/* 4196 */ 50058,
/* 4197 */ -4195,
/* 4198 */ 791,
/* 4199 */ 0,
/* 4200 */ -4195,
/* 4201 */ 1219,
/* 4202 */ 4206,
/* 4203 */ 50058,
/* 4204 */ -4202,
/* 4205 */ 792,
/* 4206 */ 0,
/* 4207 */ -4202,
/* 4208 */ 1220,
/* 4209 */ 4213,
/* 4210 */ 50058,
/* 4211 */ -4209,
/* 4212 */ 793,
/* 4213 */ 0,
/* 4214 */ -4209,
/* 4215 */ 1221,
/* 4216 */ 4220,
/* 4217 */ 50058,
/* 4218 */ -4216,
/* 4219 */ 794,
/* 4220 */ 0,
/* 4221 */ -4216,
/* 4222 */ 1222,
/* 4223 */ 4229,
/* 4224 */ 215,
/* 4225 */ 4232,
/* 4226 */ 4223,
/* 4227 */ -4223,
/* 4228 */ 797,
/* 4229 */ 0,
/* 4230 */ -4223,
/* 4231 */ 1224,
/* 4232 */ 4236,
/* 4233 */ 50124,
/* 4234 */ -4232,
/* 4235 */ 795,
/* 4236 */ 4240,
/* 4237 */ 50044,
/* 4238 */ -4232,
/* 4239 */ 796,
/* 4240 */ 0,
/* 4241 */ -4232,
/* 4242 */ 1223,
/* 4243 */ 0,
/* 4244 */ 50417,
/* 4245 */ 50257,
/* 4246 */ 50058,
/* 4247 */ 215,
/* 4248 */ 50123,
/* 4249 */ 4253,
/* 4250 */ 50125,
/* 4251 */ -4243,
/* 4252 */ 285,
/* 4253 */ 4258,
/* 4254 */ 4261,
/* 4255 */ 4253,
/* 4256 */ -4253,
/* 4257 */ 798,
/* 4258 */ 0,
/* 4259 */ -4253,
/* 4260 */ 1225,
/* 4261 */ 4268,
/* 4262 */ 50323,
/* 4263 */ 4355,
/* 4264 */ 50257,
/* 4265 */ 50059,
/* 4266 */ -4261,
/* 4267 */ 286,
/* 4268 */ 4275,
/* 4269 */ 50459,
/* 4270 */ 4362,
/* 4271 */ 155,
/* 4272 */ 50059,
/* 4273 */ -4261,
/* 4274 */ 287,
/* 4275 */ 4282,
/* 4276 */ 50347,
/* 4277 */ 4369,
/* 4278 */ 155,
/* 4279 */ 50059,
/* 4280 */ -4261,
/* 4281 */ 288,
/* 4282 */ 4289,
/* 4283 */ 50324,
/* 4284 */ 4376,
/* 4285 */ 155,
/* 4286 */ 50059,
/* 4287 */ -4261,
/* 4288 */ 289,
/* 4289 */ 4296,
/* 4290 */ 50322,
/* 4291 */ 4383,
/* 4292 */ 165,
/* 4293 */ 50059,
/* 4294 */ -4261,
/* 4295 */ 290,
/* 4296 */ 4303,
/* 4297 */ 50426,
/* 4298 */ 4390,
/* 4299 */ 165,
/* 4300 */ 50059,
/* 4301 */ -4261,
/* 4302 */ 291,
/* 4303 */ 4310,
/* 4304 */ 50421,
/* 4305 */ 4397,
/* 4306 */ 4404,
/* 4307 */ 50059,
/* 4308 */ -4261,
/* 4309 */ 292,
/* 4310 */ 4317,
/* 4311 */ 50346,
/* 4312 */ 4424,
/* 4313 */ 4431,
/* 4314 */ 50059,
/* 4315 */ -4261,
/* 4316 */ 293,
/* 4317 */ 4324,
/* 4318 */ 50356,
/* 4319 */ 50058,
/* 4320 */ 50260,
/* 4321 */ 50059,
/* 4322 */ -4261,
/* 4323 */ 294,
/* 4324 */ 4331,
/* 4325 */ 50450,
/* 4326 */ 50058,
/* 4327 */ 4451,
/* 4328 */ 50059,
/* 4329 */ -4261,
/* 4330 */ 295,
/* 4331 */ 4337,
/* 4332 */ 50418,
/* 4333 */ 50058,
/* 4334 */ 4544,
/* 4335 */ -4261,
/* 4336 */ 296,
/* 4337 */ 4343,
/* 4338 */ 50394,
/* 4339 */ 50058,
/* 4340 */ 4552,
/* 4341 */ -4261,
/* 4342 */ 297,
/* 4343 */ 4347,
/* 4344 */ 3387,
/* 4345 */ -4261,
/* 4346 */ 298,
/* 4347 */ 4351,
/* 4348 */ 530,
/* 4349 */ -4261,
/* 4350 */ 299,
/* 4351 */ 0,
/* 4352 */ 5623,
/* 4353 */ -4261,
/* 4354 */ 300,
/* 4355 */ 4359,
/* 4356 */ 50058,
/* 4357 */ -4355,
/* 4358 */ 799,
/* 4359 */ 0,
/* 4360 */ -4355,
/* 4361 */ 1226,
/* 4362 */ 4366,
/* 4363 */ 50058,
/* 4364 */ -4362,
/* 4365 */ 800,
/* 4366 */ 0,
/* 4367 */ -4362,
/* 4368 */ 1227,
/* 4369 */ 4373,
/* 4370 */ 50058,
/* 4371 */ -4369,
/* 4372 */ 801,
/* 4373 */ 0,
/* 4374 */ -4369,
/* 4375 */ 1228,
/* 4376 */ 4380,
/* 4377 */ 50058,
/* 4378 */ -4376,
/* 4379 */ 802,
/* 4380 */ 0,
/* 4381 */ -4376,
/* 4382 */ 1229,
/* 4383 */ 4387,
/* 4384 */ 50058,
/* 4385 */ -4383,
/* 4386 */ 803,
/* 4387 */ 0,
/* 4388 */ -4383,
/* 4389 */ 1230,
/* 4390 */ 4394,
/* 4391 */ 50058,
/* 4392 */ -4390,
/* 4393 */ 804,
/* 4394 */ 0,
/* 4395 */ -4390,
/* 4396 */ 1231,
/* 4397 */ 4401,
/* 4398 */ 50058,
/* 4399 */ -4397,
/* 4400 */ 805,
/* 4401 */ 0,
/* 4402 */ -4397,
/* 4403 */ 1232,
/* 4404 */ 4410,
/* 4405 */ 215,
/* 4406 */ 4413,
/* 4407 */ 4404,
/* 4408 */ -4404,
/* 4409 */ 808,
/* 4410 */ 0,
/* 4411 */ -4404,
/* 4412 */ 1234,
/* 4413 */ 4417,
/* 4414 */ 50044,
/* 4415 */ -4413,
/* 4416 */ 806,
/* 4417 */ 4421,
/* 4418 */ 50124,
/* 4419 */ -4413,
/* 4420 */ 807,
/* 4421 */ 0,
/* 4422 */ -4413,
/* 4423 */ 1233,
/* 4424 */ 4428,
/* 4425 */ 50058,
/* 4426 */ -4424,
/* 4427 */ 809,
/* 4428 */ 0,
/* 4429 */ -4424,
/* 4430 */ 1235,
/* 4431 */ 4437,
/* 4432 */ 215,
/* 4433 */ 4440,
/* 4434 */ 4431,
/* 4435 */ -4431,
/* 4436 */ 812,
/* 4437 */ 0,
/* 4438 */ -4431,
/* 4439 */ 1237,
/* 4440 */ 4444,
/* 4441 */ 50124,
/* 4442 */ -4440,
/* 4443 */ 810,
/* 4444 */ 4448,
/* 4445 */ 50044,
/* 4446 */ -4440,
/* 4447 */ 811,
/* 4448 */ 0,
/* 4449 */ -4440,
/* 4450 */ 1236,
/* 4451 */ 4460,
/* 4452 */ 50259,
/* 4453 */ 50058,
/* 4454 */ 155,
/* 4455 */ 4463,
/* 4456 */ 4537,
/* 4457 */ 4451,
/* 4458 */ -4451,
/* 4459 */ 825,
/* 4460 */ 0,
/* 4461 */ -4451,
/* 4462 */ 1243,
/* 4463 */ 4470,
/* 4464 */ 50091,
/* 4465 */ 165,
/* 4466 */ 50093,
/* 4467 */ 4463,
/* 4468 */ -4463,
/* 4469 */ 813,
/* 4470 */ 4477,
/* 4471 */ 50466,
/* 4472 */ 4486,
/* 4473 */ 4516,
/* 4474 */ 4463,
/* 4475 */ -4463,
/* 4476 */ 822,
/* 4477 */ 4483,
/* 4478 */ 50292,
/* 4479 */ 501,
/* 4480 */ 4463,
/* 4481 */ -4463,
/* 4482 */ 823,
/* 4483 */ 0,
/* 4484 */ -4463,
/* 4485 */ 1241,
/* 4486 */ 4490,
/* 4487 */ 155,
/* 4488 */ -4486,
/* 4489 */ 814,
/* 4490 */ 4496,
/* 4491 */ 50414,
/* 4492 */ 50368,
/* 4493 */ 4502,
/* 4494 */ -4486,
/* 4495 */ 816,
/* 4496 */ 0,
/* 4497 */ 50414,
/* 4498 */ 50384,
/* 4499 */ 4509,
/* 4500 */ -4486,
/* 4501 */ 818,
/* 4502 */ 4506,
/* 4503 */ 165,
/* 4504 */ -4502,
/* 4505 */ 815,
/* 4506 */ 0,
/* 4507 */ -4502,
/* 4508 */ 1238,
/* 4509 */ 4513,
/* 4510 */ 165,
/* 4511 */ -4509,
/* 4512 */ 817,
/* 4513 */ 0,
/* 4514 */ -4509,
/* 4515 */ 1239,
/* 4516 */ 4521,
/* 4517 */ 50395,
/* 4518 */ 155,
/* 4519 */ -4516,
/* 4520 */ 819,
/* 4521 */ 4529,
/* 4522 */ 50395,
/* 4523 */ 50414,
/* 4524 */ 165,
/* 4525 */ 50452,
/* 4526 */ 165,
/* 4527 */ -4516,
/* 4528 */ 820,
/* 4529 */ 4534,
/* 4530 */ 50395,
/* 4531 */ 165,
/* 4532 */ -4516,
/* 4533 */ 821,
/* 4534 */ 0,
/* 4535 */ -4516,
/* 4536 */ 1240,
/* 4537 */ 4541,
/* 4538 */ 50044,
/* 4539 */ -4537,
/* 4540 */ 824,
/* 4541 */ 0,
/* 4542 */ -4537,
/* 4543 */ 1242,
/* 4544 */ 4549,
/* 4545 */ 4261,
/* 4546 */ 4544,
/* 4547 */ -4544,
/* 4548 */ 826,
/* 4549 */ 0,
/* 4550 */ -4544,
/* 4551 */ 1244,
/* 4552 */ 4557,
/* 4553 */ 2657,
/* 4554 */ 4552,
/* 4555 */ -4552,
/* 4556 */ 827,
/* 4557 */ 0,
/* 4558 */ -4552,
/* 4559 */ 1245,
/* 4560 */ 0,
/* 4561 */ 50444,
/* 4562 */ 50257,
/* 4563 */ 50058,
/* 4564 */ 215,
/* 4565 */ 4571,
/* 4566 */ 50123,
/* 4567 */ 4579,
/* 4568 */ 50125,
/* 4569 */ -4560,
/* 4570 */ 301,
/* 4571 */ 4576,
/* 4572 */ 50349,
/* 4573 */ 215,
/* 4574 */ -4571,
/* 4575 */ 828,
/* 4576 */ 0,
/* 4577 */ -4571,
/* 4578 */ 1246,
/* 4579 */ 4584,
/* 4580 */ 4587,
/* 4581 */ 4579,
/* 4582 */ -4579,
/* 4583 */ 829,
/* 4584 */ 0,
/* 4585 */ -4579,
/* 4586 */ 1247,
/* 4587 */ 4594,
/* 4588 */ 50323,
/* 4589 */ 4814,
/* 4590 */ 50257,
/* 4591 */ 50059,
/* 4592 */ -4587,
/* 4593 */ 302,
/* 4594 */ 4601,
/* 4595 */ 50349,
/* 4596 */ 4821,
/* 4597 */ 215,
/* 4598 */ 50059,
/* 4599 */ -4587,
/* 4600 */ 303,
/* 4601 */ 4608,
/* 4602 */ 50386,
/* 4603 */ 4828,
/* 4604 */ 215,
/* 4605 */ 50059,
/* 4606 */ -4587,
/* 4607 */ 304,
/* 4608 */ 4612,
/* 4609 */ 454,
/* 4610 */ -4587,
/* 4611 */ 305,
/* 4612 */ 4619,
/* 4613 */ 4835,
/* 4614 */ 4843,
/* 4615 */ 584,
/* 4616 */ 50059,
/* 4617 */ -4587,
/* 4618 */ 306,
/* 4619 */ 4626,
/* 4620 */ 50360,
/* 4621 */ 4850,
/* 4622 */ 584,
/* 4623 */ 50059,
/* 4624 */ -4587,
/* 4625 */ 307,
/* 4626 */ 4633,
/* 4627 */ 50362,
/* 4628 */ 4857,
/* 4629 */ 584,
/* 4630 */ 50059,
/* 4631 */ -4587,
/* 4632 */ 308,
/* 4633 */ 4640,
/* 4634 */ 50320,
/* 4635 */ 4864,
/* 4636 */ 584,
/* 4637 */ 50059,
/* 4638 */ -4587,
/* 4639 */ 309,
/* 4640 */ 4647,
/* 4641 */ 50290,
/* 4642 */ 4871,
/* 4643 */ 584,
/* 4644 */ 50059,
/* 4645 */ -4587,
/* 4646 */ 310,
/* 4647 */ 4654,
/* 4648 */ 50316,
/* 4649 */ 4878,
/* 4650 */ 165,
/* 4651 */ 50059,
/* 4652 */ -4587,
/* 4653 */ 311,
/* 4654 */ 4661,
/* 4655 */ 50405,
/* 4656 */ 4885,
/* 4657 */ 584,
/* 4658 */ 50059,
/* 4659 */ -4587,
/* 4660 */ 312,
/* 4661 */ 4669,
/* 4662 */ 50303,
/* 4663 */ 50370,
/* 4664 */ 4892,
/* 4665 */ 584,
/* 4666 */ 50059,
/* 4667 */ -4587,
/* 4668 */ 313,
/* 4669 */ 4676,
/* 4670 */ 50426,
/* 4671 */ 4899,
/* 4672 */ 584,
/* 4673 */ 50059,
/* 4674 */ -4587,
/* 4675 */ 314,
/* 4676 */ 4683,
/* 4677 */ 50430,
/* 4678 */ 4906,
/* 4679 */ 584,
/* 4680 */ 50059,
/* 4681 */ -4587,
/* 4682 */ 315,
/* 4683 */ 4690,
/* 4684 */ 50385,
/* 4685 */ 4913,
/* 4686 */ 584,
/* 4687 */ 50059,
/* 4688 */ -4587,
/* 4689 */ 316,
/* 4690 */ 4697,
/* 4691 */ 50276,
/* 4692 */ 4920,
/* 4693 */ 584,
/* 4694 */ 4927,
/* 4695 */ -4587,
/* 4696 */ 317,
/* 4697 */ 4705,
/* 4698 */ 50294,
/* 4699 */ 4935,
/* 4700 */ 5157,
/* 4701 */ 4942,
/* 4702 */ 50059,
/* 4703 */ -4587,
/* 4704 */ 318,
/* 4705 */ 4713,
/* 4706 */ 50435,
/* 4707 */ 50091,
/* 4708 */ 215,
/* 4709 */ 50093,
/* 4710 */ 50059,
/* 4711 */ -4587,
/* 4712 */ 319,
/* 4713 */ 4723,
/* 4714 */ 50435,
/* 4715 */ 50091,
/* 4716 */ 215,
/* 4717 */ 50044,
/* 4718 */ 215,
/* 4719 */ 50093,
/* 4720 */ 50059,
/* 4721 */ -4587,
/* 4722 */ 320,
/* 4723 */ 4735,
/* 4724 */ 50435,
/* 4725 */ 50091,
/* 4726 */ 215,
/* 4727 */ 50044,
/* 4728 */ 215,
/* 4729 */ 50044,
/* 4730 */ 215,
/* 4731 */ 50093,
/* 4732 */ 50059,
/* 4733 */ -4587,
/* 4734 */ 321,
/* 4735 */ 4749,
/* 4736 */ 50435,
/* 4737 */ 50091,
/* 4738 */ 215,
/* 4739 */ 50044,
/* 4740 */ 215,
/* 4741 */ 50044,
/* 4742 */ 215,
/* 4743 */ 50044,
/* 4744 */ 155,
/* 4745 */ 50093,
/* 4746 */ 50059,
/* 4747 */ -4587,
/* 4748 */ 322,
/* 4749 */ 4756,
/* 4750 */ 50419,
/* 4751 */ 4951,
/* 4752 */ 4958,
/* 4753 */ 50059,
/* 4754 */ -4587,
/* 4755 */ 323,
/* 4756 */ 4763,
/* 4757 */ 50365,
/* 4758 */ 4991,
/* 4759 */ 4998,
/* 4760 */ 50059,
/* 4761 */ -4587,
/* 4762 */ 324,
/* 4763 */ 4771,
/* 4764 */ 50342,
/* 4765 */ 5031,
/* 4766 */ 5038,
/* 4767 */ 215,
/* 4768 */ 50059,
/* 4769 */ -4587,
/* 4770 */ 325,
/* 4771 */ 4778,
/* 4772 */ 50432,
/* 4773 */ 5047,
/* 4774 */ 5054,
/* 4775 */ 50059,
/* 4776 */ -4587,
/* 4777 */ 326,
/* 4778 */ 4785,
/* 4779 */ 50327,
/* 4780 */ 5070,
/* 4781 */ 5077,
/* 4782 */ 50059,
/* 4783 */ -4587,
/* 4784 */ 327,
/* 4785 */ 4792,
/* 4786 */ 50404,
/* 4787 */ 5093,
/* 4788 */ 5100,
/* 4789 */ 50059,
/* 4790 */ -4587,
/* 4791 */ 328,
/* 4792 */ 4798,
/* 4793 */ 50346,
/* 4794 */ 5116,
/* 4795 */ 5123,
/* 4796 */ -4587,
/* 4797 */ 329,
/* 4798 */ 4802,
/* 4799 */ 425,
/* 4800 */ -4587,
/* 4801 */ 330,
/* 4802 */ 4806,
/* 4803 */ 530,
/* 4804 */ -4587,
/* 4805 */ 331,
/* 4806 */ 4810,
/* 4807 */ 3387,
/* 4808 */ -4587,
/* 4809 */ 332,
/* 4810 */ 0,
/* 4811 */ 5623,
/* 4812 */ -4587,
/* 4813 */ 333,
/* 4814 */ 4818,
/* 4815 */ 50058,
/* 4816 */ -4814,
/* 4817 */ 830,
/* 4818 */ 0,
/* 4819 */ -4814,
/* 4820 */ 1248,
/* 4821 */ 4825,
/* 4822 */ 50058,
/* 4823 */ -4821,
/* 4824 */ 831,
/* 4825 */ 0,
/* 4826 */ -4821,
/* 4827 */ 1249,
/* 4828 */ 4832,
/* 4829 */ 50058,
/* 4830 */ -4828,
/* 4831 */ 832,
/* 4832 */ 0,
/* 4833 */ -4828,
/* 4834 */ 1250,
/* 4835 */ 4839,
/* 4836 */ 50370,
/* 4837 */ -4835,
/* 4838 */ 833,
/* 4839 */ 0,
/* 4840 */ 50304,
/* 4841 */ -4835,
/* 4842 */ 834,
/* 4843 */ 4847,
/* 4844 */ 50058,
/* 4845 */ -4843,
/* 4846 */ 835,
/* 4847 */ 0,
/* 4848 */ -4843,
/* 4849 */ 1251,
/* 4850 */ 4854,
/* 4851 */ 50058,
/* 4852 */ -4850,
/* 4853 */ 836,
/* 4854 */ 0,
/* 4855 */ -4850,
/* 4856 */ 1252,
/* 4857 */ 4861,
/* 4858 */ 50058,
/* 4859 */ -4857,
/* 4860 */ 837,
/* 4861 */ 0,
/* 4862 */ -4857,
/* 4863 */ 1253,
/* 4864 */ 4868,
/* 4865 */ 50058,
/* 4866 */ -4864,
/* 4867 */ 838,
/* 4868 */ 0,
/* 4869 */ -4864,
/* 4870 */ 1254,
/* 4871 */ 4875,
/* 4872 */ 50058,
/* 4873 */ -4871,
/* 4874 */ 839,
/* 4875 */ 0,
/* 4876 */ -4871,
/* 4877 */ 1255,
/* 4878 */ 4882,
/* 4879 */ 50058,
/* 4880 */ -4878,
/* 4881 */ 840,
/* 4882 */ 0,
/* 4883 */ -4878,
/* 4884 */ 1256,
/* 4885 */ 4889,
/* 4886 */ 50058,
/* 4887 */ -4885,
/* 4888 */ 841,
/* 4889 */ 0,
/* 4890 */ -4885,
/* 4891 */ 1257,
/* 4892 */ 4896,
/* 4893 */ 50058,
/* 4894 */ -4892,
/* 4895 */ 842,
/* 4896 */ 0,
/* 4897 */ -4892,
/* 4898 */ 1258,
/* 4899 */ 4903,
/* 4900 */ 50058,
/* 4901 */ -4899,
/* 4902 */ 843,
/* 4903 */ 0,
/* 4904 */ -4899,
/* 4905 */ 1259,
/* 4906 */ 4910,
/* 4907 */ 50058,
/* 4908 */ -4906,
/* 4909 */ 844,
/* 4910 */ 0,
/* 4911 */ -4906,
/* 4912 */ 1260,
/* 4913 */ 4917,
/* 4914 */ 50058,
/* 4915 */ -4913,
/* 4916 */ 845,
/* 4917 */ 0,
/* 4918 */ -4913,
/* 4919 */ 1261,
/* 4920 */ 4924,
/* 4921 */ 50058,
/* 4922 */ -4920,
/* 4923 */ 846,
/* 4924 */ 0,
/* 4925 */ -4920,
/* 4926 */ 1262,
/* 4927 */ 4931,
/* 4928 */ 50044,
/* 4929 */ -4927,
/* 4930 */ 847,
/* 4931 */ 0,
/* 4932 */ 50059,
/* 4933 */ -4927,
/* 4934 */ 848,
/* 4935 */ 4939,
/* 4936 */ 50058,
/* 4937 */ -4935,
/* 4938 */ 849,
/* 4939 */ 0,
/* 4940 */ -4935,
/* 4941 */ 1263,
/* 4942 */ 4948,
/* 4943 */ 50044,
/* 4944 */ 5157,
/* 4945 */ 4942,
/* 4946 */ -4942,
/* 4947 */ 850,
/* 4948 */ 0,
/* 4949 */ -4942,
/* 4950 */ 1264,
/* 4951 */ 4955,
/* 4952 */ 50058,
/* 4953 */ -4951,
/* 4954 */ 851,
/* 4955 */ 0,
/* 4956 */ -4951,
/* 4957 */ 1265,
/* 4958 */ 4964,
/* 4959 */ 4967,
/* 4960 */ 4984,
/* 4961 */ 4958,
/* 4962 */ -4958,
/* 4963 */ 857,
/* 4964 */ 0,
/* 4965 */ -4958,
/* 4966 */ 1267,
/* 4967 */ 4971,
/* 4968 */ 215,
/* 4969 */ -4967,
/* 4970 */ 852,
/* 4971 */ 0,
/* 4972 */ 4976,
/* 4973 */ 215,
/* 4974 */ -4967,
/* 4975 */ 855,
/* 4976 */ 4980,
/* 4977 */ 50126,
/* 4978 */ -4976,
/* 4979 */ 853,
/* 4980 */ 0,
/* 4981 */ 50033,
/* 4982 */ -4976,
/* 4983 */ 854,
/* 4984 */ 4988,
/* 4985 */ 50044,
/* 4986 */ -4984,
/* 4987 */ 856,
/* 4988 */ 0,
/* 4989 */ -4984,
/* 4990 */ 1266,
/* 4991 */ 4995,
/* 4992 */ 50058,
/* 4993 */ -4991,
/* 4994 */ 858,
/* 4995 */ 0,
/* 4996 */ -4991,
/* 4997 */ 1268,
/* 4998 */ 5004,
/* 4999 */ 5007,
/* 5000 */ 5024,
/* 5001 */ 4998,
/* 5002 */ -4998,
/* 5003 */ 864,
/* 5004 */ 0,
/* 5005 */ -4998,
/* 5006 */ 1270,
/* 5007 */ 5011,
/* 5008 */ 215,
/* 5009 */ -5007,
/* 5010 */ 859,
/* 5011 */ 0,
/* 5012 */ 5016,
/* 5013 */ 215,
/* 5014 */ -5007,
/* 5015 */ 862,
/* 5016 */ 5020,
/* 5017 */ 50126,
/* 5018 */ -5016,
/* 5019 */ 860,
/* 5020 */ 0,
/* 5021 */ 50033,
/* 5022 */ -5016,
/* 5023 */ 861,
/* 5024 */ 5028,
/* 5025 */ 50044,
/* 5026 */ -5024,
/* 5027 */ 863,
/* 5028 */ 0,
/* 5029 */ -5024,
/* 5030 */ 1269,
/* 5031 */ 5035,
/* 5032 */ 50058,
/* 5033 */ -5031,
/* 5034 */ 865,
/* 5035 */ 0,
/* 5036 */ -5031,
/* 5037 */ 1271,
/* 5038 */ 5044,
/* 5039 */ 215,
/* 5040 */ 50044,
/* 5041 */ 5038,
/* 5042 */ -5038,
/* 5043 */ 866,
/* 5044 */ 0,
/* 5045 */ -5038,
/* 5046 */ 1272,
/* 5047 */ 5051,
/* 5048 */ 50058,
/* 5049 */ -5047,
/* 5050 */ 867,
/* 5051 */ 0,
/* 5052 */ -5047,
/* 5053 */ 1273,
/* 5054 */ 5060,
/* 5055 */ 155,
/* 5056 */ 5063,
/* 5057 */ 5054,
/* 5058 */ -5054,
/* 5059 */ 869,
/* 5060 */ 0,
/* 5061 */ -5054,
/* 5062 */ 1275,
/* 5063 */ 5067,
/* 5064 */ 50044,
/* 5065 */ -5063,
/* 5066 */ 868,
/* 5067 */ 0,
/* 5068 */ -5063,
/* 5069 */ 1274,
/* 5070 */ 5074,
/* 5071 */ 50058,
/* 5072 */ -5070,
/* 5073 */ 870,
/* 5074 */ 0,
/* 5075 */ -5070,
/* 5076 */ 1276,
/* 5077 */ 5083,
/* 5078 */ 155,
/* 5079 */ 5086,
/* 5080 */ 5077,
/* 5081 */ -5077,
/* 5082 */ 872,
/* 5083 */ 0,
/* 5084 */ -5077,
/* 5085 */ 1278,
/* 5086 */ 5090,
/* 5087 */ 50044,
/* 5088 */ -5086,
/* 5089 */ 871,
/* 5090 */ 0,
/* 5091 */ -5086,
/* 5092 */ 1277,
/* 5093 */ 5097,
/* 5094 */ 50058,
/* 5095 */ -5093,
/* 5096 */ 873,
/* 5097 */ 0,
/* 5098 */ -5093,
/* 5099 */ 1279,
/* 5100 */ 5106,
/* 5101 */ 155,
/* 5102 */ 5109,
/* 5103 */ 5100,
/* 5104 */ -5100,
/* 5105 */ 875,
/* 5106 */ 0,
/* 5107 */ -5100,
/* 5108 */ 1281,
/* 5109 */ 5113,
/* 5110 */ 50044,
/* 5111 */ -5109,
/* 5112 */ 874,
/* 5113 */ 0,
/* 5114 */ -5109,
/* 5115 */ 1280,
/* 5116 */ 5120,
/* 5117 */ 50058,
/* 5118 */ -5116,
/* 5119 */ 876,
/* 5120 */ 0,
/* 5121 */ -5116,
/* 5122 */ 1282,
/* 5123 */ 5129,
/* 5124 */ 5132,
/* 5125 */ 5149,
/* 5126 */ 5123,
/* 5127 */ -5123,
/* 5128 */ 883,
/* 5129 */ 0,
/* 5130 */ -5123,
/* 5131 */ 1283,
/* 5132 */ 5136,
/* 5133 */ 215,
/* 5134 */ -5132,
/* 5135 */ 877,
/* 5136 */ 0,
/* 5137 */ 5141,
/* 5138 */ 215,
/* 5139 */ -5132,
/* 5140 */ 880,
/* 5141 */ 5145,
/* 5142 */ 50126,
/* 5143 */ -5141,
/* 5144 */ 878,
/* 5145 */ 0,
/* 5146 */ 50033,
/* 5147 */ -5141,
/* 5148 */ 879,
/* 5149 */ 5153,
/* 5150 */ 50044,
/* 5151 */ -5149,
/* 5152 */ 881,
/* 5153 */ 0,
/* 5154 */ 50059,
/* 5155 */ -5149,
/* 5156 */ 882,
/* 5157 */ 5167,
/* 5158 */ 215,
/* 5159 */ 50349,
/* 5160 */ 5174,
/* 5161 */ 5186,
/* 5162 */ 5193,
/* 5163 */ 5200,
/* 5164 */ 5222,
/* 5165 */ -5157,
/* 5166 */ 334,
/* 5167 */ 0,
/* 5168 */ 215,
/* 5169 */ 50349,
/* 5170 */ 155,
/* 5171 */ 5277,
/* 5172 */ -5157,
/* 5173 */ 335,
/* 5174 */ 5178,
/* 5175 */ 278,
/* 5176 */ -5174,
/* 5177 */ 884,
/* 5178 */ 5183,
/* 5179 */ 215,
/* 5180 */ 50037,
/* 5181 */ -5174,
/* 5182 */ 885,
/* 5183 */ 0,
/* 5184 */ -5174,
/* 5185 */ 1284,
/* 5186 */ 5190,
/* 5187 */ 215,
/* 5188 */ -5186,
/* 5189 */ 886,
/* 5190 */ 0,
/* 5191 */ -5186,
/* 5192 */ 1285,
/* 5193 */ 5197,
/* 5194 */ 50316,
/* 5195 */ -5193,
/* 5196 */ 887,
/* 5197 */ 0,
/* 5198 */ -5193,
/* 5199 */ 1286,
/* 5200 */ 5207,
/* 5201 */ 50040,
/* 5202 */ 5210,
/* 5203 */ 215,
/* 5204 */ 50041,
/* 5205 */ -5200,
/* 5206 */ 891,
/* 5207 */ 0,
/* 5208 */ -5200,
/* 5209 */ 1287,
/* 5210 */ 5214,
/* 5211 */ 50319,
/* 5212 */ -5210,
/* 5213 */ 888,
/* 5214 */ 5218,
/* 5215 */ 50304,
/* 5216 */ -5210,
/* 5217 */ 889,
/* 5218 */ 0,
/* 5219 */ 50370,
/* 5220 */ -5210,
/* 5221 */ 890,
/* 5222 */ 5231,
/* 5223 */ 50287,
/* 5224 */ 5234,
/* 5225 */ 5241,
/* 5226 */ 5248,
/* 5227 */ 5255,
/* 5228 */ 5222,
/* 5229 */ -5222,
/* 5230 */ 899,
/* 5231 */ 0,
/* 5232 */ -5222,
/* 5233 */ 1292,
/* 5234 */ 5238,
/* 5235 */ 278,
/* 5236 */ -5234,
/* 5237 */ 892,
/* 5238 */ 0,
/* 5239 */ -5234,
/* 5240 */ 1288,
/* 5241 */ 5245,
/* 5242 */ 215,
/* 5243 */ -5241,
/* 5244 */ 893,
/* 5245 */ 0,
/* 5246 */ -5241,
/* 5247 */ 1289,
/* 5248 */ 5252,
/* 5249 */ 50316,
/* 5250 */ -5248,
/* 5251 */ 894,
/* 5252 */ 0,
/* 5253 */ -5248,
/* 5254 */ 1290,
/* 5255 */ 5262,
/* 5256 */ 50040,
/* 5257 */ 5265,
/* 5258 */ 215,
/* 5259 */ 50041,
/* 5260 */ -5255,
/* 5261 */ 898,
/* 5262 */ 0,
/* 5263 */ -5255,
/* 5264 */ 1291,
/* 5265 */ 5269,
/* 5266 */ 50319,
/* 5267 */ -5265,
/* 5268 */ 895,
/* 5269 */ 5273,
/* 5270 */ 50304,
/* 5271 */ -5265,
/* 5272 */ 896,
/* 5273 */ 0,
/* 5274 */ 50370,
/* 5275 */ -5265,
/* 5276 */ 897,
/* 5277 */ 5281,
/* 5278 */ 215,
/* 5279 */ -5277,
/* 5280 */ 900,
/* 5281 */ 0,
/* 5282 */ -5277,
/* 5283 */ 1293,
/* 5284 */ 0,
/* 5285 */ 50261,
/* 5286 */ -5284,
/* 5287 */ 336,
/* 5288 */ 0,
/* 5289 */ 50337,
/* 5290 */ 50257,
/* 5291 */ 50058,
/* 5292 */ 215,
/* 5293 */ 50123,
/* 5294 */ 5298,
/* 5295 */ 50125,
/* 5296 */ -5288,
/* 5297 */ 337,
/* 5298 */ 5303,
/* 5299 */ 5306,
/* 5300 */ 5298,
/* 5301 */ -5298,
/* 5302 */ 901,
/* 5303 */ 0,
/* 5304 */ -5298,
/* 5305 */ 1294,
/* 5306 */ 5313,
/* 5307 */ 50323,
/* 5308 */ 5335,
/* 5309 */ 50257,
/* 5310 */ 50059,
/* 5311 */ -5306,
/* 5312 */ 338,
/* 5313 */ 5320,
/* 5314 */ 50379,
/* 5315 */ 5342,
/* 5316 */ 215,
/* 5317 */ 50059,
/* 5318 */ -5306,
/* 5319 */ 339,
/* 5320 */ 5327,
/* 5321 */ 50307,
/* 5322 */ 5349,
/* 5323 */ 1211,
/* 5324 */ 50059,
/* 5325 */ -5306,
/* 5326 */ 340,
/* 5327 */ 0,
/* 5328 */ 50461,
/* 5329 */ 5356,
/* 5330 */ 215,
/* 5331 */ 50037,
/* 5332 */ 50059,
/* 5333 */ -5306,
/* 5334 */ 341,
/* 5335 */ 5339,
/* 5336 */ 50058,
/* 5337 */ -5335,
/* 5338 */ 902,
/* 5339 */ 0,
/* 5340 */ -5335,
/* 5341 */ 1295,
/* 5342 */ 5346,
/* 5343 */ 50058,
/* 5344 */ -5342,
/* 5345 */ 903,
/* 5346 */ 0,
/* 5347 */ -5342,
/* 5348 */ 1296,
/* 5349 */ 5353,
/* 5350 */ 50058,
/* 5351 */ -5349,
/* 5352 */ 904,
/* 5353 */ 0,
/* 5354 */ -5349,
/* 5355 */ 1297,
/* 5356 */ 5360,
/* 5357 */ 50058,
/* 5358 */ -5356,
/* 5359 */ 905,
/* 5360 */ 0,
/* 5361 */ -5356,
/* 5362 */ 1298,
/* 5363 */ 0,
/* 5364 */ 50261,
/* 5365 */ -5363,
/* 5366 */ 342,
/* 5367 */ 5371,
/* 5368 */ 50264,
/* 5369 */ -5367,
/* 5370 */ 343,
/* 5371 */ 5375,
/* 5372 */ 50267,
/* 5373 */ -5367,
/* 5374 */ 344,
/* 5375 */ 5379,
/* 5376 */ 50263,
/* 5377 */ -5367,
/* 5378 */ 345,
/* 5379 */ 5383,
/* 5380 */ 50271,
/* 5381 */ -5367,
/* 5382 */ 346,
/* 5383 */ 5387,
/* 5384 */ 50272,
/* 5385 */ -5367,
/* 5386 */ 347,
/* 5387 */ 5391,
/* 5388 */ 50273,
/* 5389 */ -5367,
/* 5390 */ 348,
/* 5391 */ 0,
/* 5392 */ 50469,
/* 5393 */ -5367,
/* 5394 */ 349,
/* 5395 */ 0,
/* 5396 */ 50442,
/* 5397 */ 5367,
/* 5398 */ 215,
/* 5399 */ 50262,
/* 5400 */ 50258,
/* 5401 */ 50040,
/* 5402 */ 5408,
/* 5403 */ 5477,
/* 5404 */ 50041,
/* 5405 */ 50059,
/* 5406 */ -5395,
/* 5407 */ 350,
/* 5408 */ 5416,
/* 5409 */ 5367,
/* 5410 */ 5419,
/* 5411 */ 5427,
/* 5412 */ 5434,
/* 5413 */ 5442,
/* 5414 */ -5408,
/* 5415 */ 913,
/* 5416 */ 0,
/* 5417 */ -5408,
/* 5418 */ 1306,
/* 5419 */ 5424,
/* 5420 */ 50058,
/* 5421 */ 215,
/* 5422 */ -5419,
/* 5423 */ 906,
/* 5424 */ 0,
/* 5425 */ -5419,
/* 5426 */ 1299,
/* 5427 */ 5431,
/* 5428 */ 50258,
/* 5429 */ -5427,
/* 5430 */ 907,
/* 5431 */ 0,
/* 5432 */ -5427,
/* 5433 */ 1300,
/* 5434 */ 5439,
/* 5435 */ 50061,
/* 5436 */ 165,
/* 5437 */ -5434,
/* 5438 */ 908,
/* 5439 */ 0,
/* 5440 */ -5434,
/* 5441 */ 1301,
/* 5442 */ 5451,
/* 5443 */ 50044,
/* 5444 */ 5367,
/* 5445 */ 5454,
/* 5446 */ 5462,
/* 5447 */ 5469,
/* 5448 */ 5442,
/* 5449 */ -5442,
/* 5450 */ 912,
/* 5451 */ 0,
/* 5452 */ -5442,
/* 5453 */ 1305,
/* 5454 */ 5459,
/* 5455 */ 50058,
/* 5456 */ 215,
/* 5457 */ -5454,
/* 5458 */ 909,
/* 5459 */ 0,
/* 5460 */ -5454,
/* 5461 */ 1302,
/* 5462 */ 5466,
/* 5463 */ 50258,
/* 5464 */ -5462,
/* 5465 */ 910,
/* 5466 */ 0,
/* 5467 */ -5462,
/* 5468 */ 1303,
/* 5469 */ 5474,
/* 5470 */ 50061,
/* 5471 */ 165,
/* 5472 */ -5469,
/* 5473 */ 911,
/* 5474 */ 0,
/* 5475 */ -5469,
/* 5476 */ 1304,
/* 5477 */ 5482,
/* 5478 */ 50044,
/* 5479 */ 50490,
/* 5480 */ -5477,
/* 5481 */ 914,
/* 5482 */ 0,
/* 5483 */ -5477,
/* 5484 */ 1307,
/* 5485 */ 0,
/* 5486 */ 50442,
/* 5487 */ 5367,
/* 5488 */ 50258,
/* 5489 */ 50040,
/* 5490 */ 5496,
/* 5491 */ 5565,
/* 5492 */ 50041,
/* 5493 */ 50059,
/* 5494 */ -5485,
/* 5495 */ 351,
/* 5496 */ 5504,
/* 5497 */ 5367,
/* 5498 */ 5507,
/* 5499 */ 5515,
/* 5500 */ 5522,
/* 5501 */ 5530,
/* 5502 */ -5496,
/* 5503 */ 922,
/* 5504 */ 0,
/* 5505 */ -5496,
/* 5506 */ 1315,
/* 5507 */ 5512,
/* 5508 */ 50058,
/* 5509 */ 215,
/* 5510 */ -5507,
/* 5511 */ 915,
/* 5512 */ 0,
/* 5513 */ -5507,
/* 5514 */ 1308,
/* 5515 */ 5519,
/* 5516 */ 50258,
/* 5517 */ -5515,
/* 5518 */ 916,
/* 5519 */ 0,
/* 5520 */ -5515,
/* 5521 */ 1309,
/* 5522 */ 5527,
/* 5523 */ 50061,
/* 5524 */ 165,
/* 5525 */ -5522,
/* 5526 */ 917,
/* 5527 */ 0,
/* 5528 */ -5522,
/* 5529 */ 1310,
/* 5530 */ 5539,
/* 5531 */ 50044,
/* 5532 */ 5367,
/* 5533 */ 5542,
/* 5534 */ 5550,
/* 5535 */ 5557,
/* 5536 */ 5530,
/* 5537 */ -5530,
/* 5538 */ 921,
/* 5539 */ 0,
/* 5540 */ -5530,
/* 5541 */ 1314,
/* 5542 */ 5547,
/* 5543 */ 50058,
/* 5544 */ 215,
/* 5545 */ -5542,
/* 5546 */ 918,
/* 5547 */ 0,
/* 5548 */ -5542,
/* 5549 */ 1311,
/* 5550 */ 5554,
/* 5551 */ 50258,
/* 5552 */ -5550,
/* 5553 */ 919,
/* 5554 */ 0,
/* 5555 */ -5550,
/* 5556 */ 1312,
/* 5557 */ 5562,
/* 5558 */ 50061,
/* 5559 */ 165,
/* 5560 */ -5557,
/* 5561 */ 920,
/* 5562 */ 0,
/* 5563 */ -5557,
/* 5564 */ 1313,
/* 5565 */ 5570,
/* 5566 */ 50044,
/* 5567 */ 50490,
/* 5568 */ -5565,
/* 5569 */ 923,
/* 5570 */ 0,
/* 5571 */ -5565,
/* 5572 */ 1316,
/* 5573 */ 0,
/* 5574 */ 50442,
/* 5575 */ 5367,
/* 5576 */ 215,
/* 5577 */ 50262,
/* 5578 */ 50258,
/* 5579 */ 50059,
/* 5580 */ -5573,
/* 5581 */ 352,
/* 5582 */ 0,
/* 5583 */ 50442,
/* 5584 */ 215,
/* 5585 */ 50258,
/* 5586 */ 50476,
/* 5587 */ 215,
/* 5588 */ 50059,
/* 5589 */ -5582,
/* 5590 */ 353,
/* 5591 */ 0,
/* 5592 */ 5367,
/* 5593 */ 50258,
/* 5594 */ 5598,
/* 5595 */ 50059,
/* 5596 */ -5591,
/* 5597 */ 354,
/* 5598 */ 5604,
/* 5599 */ 50044,
/* 5600 */ 50258,
/* 5601 */ 5598,
/* 5602 */ -5598,
/* 5603 */ 924,
/* 5604 */ 0,
/* 5605 */ -5598,
/* 5606 */ 1317,
/* 5607 */ 0,
/* 5608 */ 5367,
/* 5609 */ 50258,
/* 5610 */ 5614,
/* 5611 */ 50059,
/* 5612 */ -5607,
/* 5613 */ 355,
/* 5614 */ 5620,
/* 5615 */ 50044,
/* 5616 */ 50258,
/* 5617 */ 5614,
/* 5618 */ -5614,
/* 5619 */ 925,
/* 5620 */ 0,
/* 5621 */ -5614,
/* 5622 */ 1318,
/* 5623 */ 0,
/* 5624 */ 5367,
/* 5625 */ 50258,
/* 5626 */ 5630,
/* 5627 */ 50059,
/* 5628 */ -5623,
/* 5629 */ 356,
/* 5630 */ 5636,
/* 5631 */ 50044,
/* 5632 */ 50258,
/* 5633 */ 5630,
/* 5634 */ -5630,
/* 5635 */ 926,
/* 5636 */ 0,
/* 5637 */ -5630,
/* 5638 */ 1319,
/* 5639 */ 5644,
/* 5640 */ 6240,
/* 5641 */ 50059,
/* 5642 */ -5639,
/* 5643 */ 357,
/* 5644 */ 5654,
/* 5645 */ 50363,
/* 5646 */ 50040,
/* 5647 */ 6240,
/* 5648 */ 50041,
/* 5649 */ 5639,
/* 5650 */ 50332,
/* 5651 */ 5639,
/* 5652 */ -5639,
/* 5653 */ 358,
/* 5654 */ 5662,
/* 5655 */ 50363,
/* 5656 */ 50040,
/* 5657 */ 6240,
/* 5658 */ 50041,
/* 5659 */ 5639,
/* 5660 */ -5639,
/* 5661 */ 359,
/* 5662 */ 5674,
/* 5663 */ 50349,
/* 5664 */ 50040,
/* 5665 */ 6240,
/* 5666 */ 50059,
/* 5667 */ 6240,
/* 5668 */ 50059,
/* 5669 */ 6240,
/* 5670 */ 50041,
/* 5671 */ 5639,
/* 5672 */ -5639,
/* 5673 */ 360,
/* 5674 */ 5685,
/* 5675 */ 50441,
/* 5676 */ 50040,
/* 5677 */ 6240,
/* 5678 */ 50041,
/* 5679 */ 50123,
/* 5680 */ 5728,
/* 5681 */ 5755,
/* 5682 */ 50125,
/* 5683 */ -5639,
/* 5684 */ 361,
/* 5685 */ 5690,
/* 5686 */ 50299,
/* 5687 */ 50059,
/* 5688 */ -5639,
/* 5689 */ 362,
/* 5690 */ 5695,
/* 5691 */ 50311,
/* 5692 */ 50059,
/* 5693 */ -5639,
/* 5694 */ 363,
/* 5695 */ 5705,
/* 5696 */ 50328,
/* 5697 */ 5639,
/* 5698 */ 50464,
/* 5699 */ 50040,
/* 5700 */ 6240,
/* 5701 */ 50041,
/* 5702 */ 50059,
/* 5703 */ -5639,
/* 5704 */ 364,
/* 5705 */ 5713,
/* 5706 */ 50464,
/* 5707 */ 50040,
/* 5708 */ 6240,
/* 5709 */ 50041,
/* 5710 */ 5639,
/* 5711 */ -5639,
/* 5712 */ 365,
/* 5713 */ 5717,
/* 5714 */ 5772,
/* 5715 */ -5639,
/* 5716 */ 366,
/* 5717 */ 5723,
/* 5718 */ 50420,
/* 5719 */ 6240,
/* 5720 */ 50059,
/* 5721 */ -5639,
/* 5722 */ 367,
/* 5723 */ 0,
/* 5724 */ 50420,
/* 5725 */ 50059,
/* 5726 */ -5639,
/* 5727 */ 368,
/* 5728 */ 5736,
/* 5729 */ 50302,
/* 5730 */ 5739,
/* 5731 */ 50058,
/* 5732 */ 5747,
/* 5733 */ 5728,
/* 5734 */ -5728,
/* 5735 */ 930,
/* 5736 */ 0,
/* 5737 */ -5728,
/* 5738 */ 1321,
/* 5739 */ 5743,
/* 5740 */ 165,
/* 5741 */ -5739,
/* 5742 */ 927,
/* 5743 */ 0,
/* 5744 */ 155,
/* 5745 */ -5739,
/* 5746 */ 928,
/* 5747 */ 5752,
/* 5748 */ 5639,
/* 5749 */ 5747,
/* 5750 */ -5747,
/* 5751 */ 929,
/* 5752 */ 0,
/* 5753 */ -5747,
/* 5754 */ 1320,
/* 5755 */ 5761,
/* 5756 */ 50321,
/* 5757 */ 50058,
/* 5758 */ 5764,
/* 5759 */ -5755,
/* 5760 */ 932,
/* 5761 */ 0,
/* 5762 */ -5755,
/* 5763 */ 1323,
/* 5764 */ 5769,
/* 5765 */ 5639,
/* 5766 */ 5764,
/* 5767 */ -5764,
/* 5768 */ 931,
/* 5769 */ 0,
/* 5770 */ -5764,
/* 5771 */ 1322,
/* 5772 */ 0,
/* 5773 */ 50123,
/* 5774 */ 5778,
/* 5775 */ 50125,
/* 5776 */ -5772,
/* 5777 */ 369,
/* 5778 */ 5783,
/* 5779 */ 5639,
/* 5780 */ 5778,
/* 5781 */ -5778,
/* 5782 */ 933,
/* 5783 */ 0,
/* 5784 */ -5778,
/* 5785 */ 1324,
/* 5786 */ 0,
/* 5787 */ 50123,
/* 5788 */ 5793,
/* 5789 */ 5807,
/* 5790 */ 50125,
/* 5791 */ -5786,
/* 5792 */ 370,
/* 5793 */ 5798,
/* 5794 */ 5607,
/* 5795 */ 5793,
/* 5796 */ -5793,
/* 5797 */ 934,
/* 5798 */ 5804,
/* 5799 */ 50436,
/* 5800 */ 5623,
/* 5801 */ 5793,
/* 5802 */ -5793,
/* 5803 */ 935,
/* 5804 */ 0,
/* 5805 */ -5793,
/* 5806 */ 1325,
/* 5807 */ 5812,
/* 5808 */ 5639,
/* 5809 */ 5807,
/* 5810 */ -5807,
/* 5811 */ 936,
/* 5812 */ 0,
/* 5813 */ -5807,
/* 5814 */ 1326,
/* 5815 */ 5819,
/* 5816 */ 50256,
/* 5817 */ -5815,
/* 5818 */ 371,
/* 5819 */ 5823,
/* 5820 */ 50259,
/* 5821 */ -5815,
/* 5822 */ 372,
/* 5823 */ 5827,
/* 5824 */ 50257,
/* 5825 */ -5815,
/* 5826 */ 373,
/* 5827 */ 5831,
/* 5828 */ 50453,
/* 5829 */ -5815,
/* 5830 */ 374,
/* 5831 */ 5835,
/* 5832 */ 50341,
/* 5833 */ -5815,
/* 5834 */ 375,
/* 5835 */ 5839,
/* 5836 */ 50391,
/* 5837 */ -5815,
/* 5838 */ 376,
/* 5839 */ 5843,
/* 5840 */ 155,
/* 5841 */ -5815,
/* 5842 */ 377,
/* 5843 */ 5849,
/* 5844 */ 50040,
/* 5845 */ 6240,
/* 5846 */ 50041,
/* 5847 */ -5815,
/* 5848 */ 378,
/* 5849 */ 5853,
/* 5850 */ 50258,
/* 5851 */ -5815,
/* 5852 */ 379,
/* 5853 */ 5859,
/* 5854 */ 5815,
/* 5855 */ 5881,
/* 5856 */ 50258,
/* 5857 */ -5815,
/* 5858 */ 380,
/* 5859 */ 5868,
/* 5860 */ 5815,
/* 5861 */ 5889,
/* 5862 */ 50258,
/* 5863 */ 50040,
/* 5864 */ 6250,
/* 5865 */ 50041,
/* 5866 */ -5815,
/* 5867 */ 381,
/* 5868 */ 5875,
/* 5869 */ 50258,
/* 5870 */ 50040,
/* 5871 */ 6250,
/* 5872 */ 50041,
/* 5873 */ -5815,
/* 5874 */ 382,
/* 5875 */ 0,
/* 5876 */ 5815,
/* 5877 */ 50064,
/* 5878 */ 50258,
/* 5879 */ -5815,
/* 5880 */ 383,
/* 5881 */ 5885,
/* 5882 */ 50046,
/* 5883 */ -5881,
/* 5884 */ 937,
/* 5885 */ 0,
/* 5886 */ 50491,
/* 5887 */ -5881,
/* 5888 */ 938,
/* 5889 */ 5893,
/* 5890 */ 50046,
/* 5891 */ -5889,
/* 5892 */ 939,
/* 5893 */ 0,
/* 5894 */ 50491,
/* 5895 */ -5889,
/* 5896 */ 940,
/* 5897 */ 5901,
/* 5898 */ 50258,
/* 5899 */ -5897,
/* 5900 */ 384,
/* 5901 */ 5907,
/* 5902 */ 5897,
/* 5903 */ 50064,
/* 5904 */ 50258,
/* 5905 */ -5897,
/* 5906 */ 385,
/* 5907 */ 0,
/* 5908 */ 5815,
/* 5909 */ 5913,
/* 5910 */ 50258,
/* 5911 */ -5897,
/* 5912 */ 386,
/* 5913 */ 5917,
/* 5914 */ 50046,
/* 5915 */ -5913,
/* 5916 */ 941,
/* 5917 */ 0,
/* 5918 */ 50491,
/* 5919 */ -5913,
/* 5920 */ 942,
/* 5921 */ 5926,
/* 5922 */ 5897,
/* 5923 */ 50470,
/* 5924 */ -5921,
/* 5925 */ 387,
/* 5926 */ 5931,
/* 5927 */ 5897,
/* 5928 */ 50471,
/* 5929 */ -5921,
/* 5930 */ 388,
/* 5931 */ 5940,
/* 5932 */ 50383,
/* 5933 */ 50040,
/* 5934 */ 6170,
/* 5935 */ 50044,
/* 5936 */ 6170,
/* 5937 */ 50041,
/* 5938 */ -5921,
/* 5939 */ 389,
/* 5940 */ 5949,
/* 5941 */ 50380,
/* 5942 */ 50040,
/* 5943 */ 6170,
/* 5944 */ 50044,
/* 5945 */ 6170,
/* 5946 */ 50041,
/* 5947 */ -5921,
/* 5948 */ 390,
/* 5949 */ 0,
/* 5950 */ 5815,
/* 5951 */ -5921,
/* 5952 */ 391,
/* 5953 */ 5958,
/* 5954 */ 50470,
/* 5955 */ 5897,
/* 5956 */ -5953,
/* 5957 */ 392,
/* 5958 */ 5963,
/* 5959 */ 50471,
/* 5960 */ 5897,
/* 5961 */ -5953,
/* 5962 */ 393,
/* 5963 */ 5968,
/* 5964 */ 50043,
/* 5965 */ 5953,
/* 5966 */ -5953,
/* 5967 */ 394,
/* 5968 */ 5973,
/* 5969 */ 50045,
/* 5970 */ 5953,
/* 5971 */ -5953,
/* 5972 */ 395,
/* 5973 */ 5978,
/* 5974 */ 50033,
/* 5975 */ 5953,
/* 5976 */ -5953,
/* 5977 */ 396,
/* 5978 */ 5983,
/* 5979 */ 50126,
/* 5980 */ 5953,
/* 5981 */ -5953,
/* 5982 */ 397,
/* 5983 */ 5989,
/* 5984 */ 50058,
/* 5985 */ 5953,
/* 5986 */ 50058,
/* 5987 */ -5953,
/* 5988 */ 398,
/* 5989 */ 0,
/* 5990 */ 5921,
/* 5991 */ -5953,
/* 5992 */ 399,
/* 5993 */ 5999,
/* 5994 */ 5953,
/* 5995 */ 50100,
/* 5996 */ 5953,
/* 5997 */ -5993,
/* 5998 */ 400,
/* 5999 */ 0,
/* 6000 */ 5953,
/* 6001 */ -5993,
/* 6002 */ 401,
/* 6003 */ 6010,
/* 6004 */ 50040,
/* 6005 */ 5367,
/* 6006 */ 50041,
/* 6007 */ 6003,
/* 6008 */ -6003,
/* 6009 */ 402,
/* 6010 */ 0,
/* 6011 */ 5993,
/* 6012 */ -6003,
/* 6013 */ 403,
/* 6014 */ 6020,
/* 6015 */ 6014,
/* 6016 */ 50042,
/* 6017 */ 6003,
/* 6018 */ -6014,
/* 6019 */ 404,
/* 6020 */ 6026,
/* 6021 */ 6014,
/* 6022 */ 50047,
/* 6023 */ 6003,
/* 6024 */ -6014,
/* 6025 */ 405,
/* 6026 */ 6032,
/* 6027 */ 6014,
/* 6028 */ 50037,
/* 6029 */ 6003,
/* 6030 */ -6014,
/* 6031 */ 406,
/* 6032 */ 0,
/* 6033 */ 6003,
/* 6034 */ -6014,
/* 6035 */ 407,
/* 6036 */ 6042,
/* 6037 */ 6036,
/* 6038 */ 50043,
/* 6039 */ 6014,
/* 6040 */ -6036,
/* 6041 */ 408,
/* 6042 */ 6048,
/* 6043 */ 6036,
/* 6044 */ 50045,
/* 6045 */ 6014,
/* 6046 */ -6036,
/* 6047 */ 409,
/* 6048 */ 0,
/* 6049 */ 6014,
/* 6050 */ -6036,
/* 6051 */ 410,
/* 6052 */ 6058,
/* 6053 */ 6052,
/* 6054 */ 50472,
/* 6055 */ 6036,
/* 6056 */ -6052,
/* 6057 */ 411,
/* 6058 */ 6064,
/* 6059 */ 6052,
/* 6060 */ 50473,
/* 6061 */ 6036,
/* 6062 */ -6052,
/* 6063 */ 412,
/* 6064 */ 0,
/* 6065 */ 6036,
/* 6066 */ -6052,
/* 6067 */ 413,
/* 6068 */ 6074,
/* 6069 */ 6068,
/* 6070 */ 50060,
/* 6071 */ 6052,
/* 6072 */ -6068,
/* 6073 */ 414,
/* 6074 */ 6080,
/* 6075 */ 6068,
/* 6076 */ 50062,
/* 6077 */ 6052,
/* 6078 */ -6068,
/* 6079 */ 415,
/* 6080 */ 6086,
/* 6081 */ 6068,
/* 6082 */ 50474,
/* 6083 */ 6052,
/* 6084 */ -6068,
/* 6085 */ 416,
/* 6086 */ 6092,
/* 6087 */ 6068,
/* 6088 */ 50475,
/* 6089 */ 6052,
/* 6090 */ -6068,
/* 6091 */ 417,
/* 6092 */ 6098,
/* 6093 */ 6068,
/* 6094 */ 50476,
/* 6095 */ 6052,
/* 6096 */ -6068,
/* 6097 */ 418,
/* 6098 */ 6104,
/* 6099 */ 6068,
/* 6100 */ 50477,
/* 6101 */ 6052,
/* 6102 */ -6068,
/* 6103 */ 419,
/* 6104 */ 0,
/* 6105 */ 6052,
/* 6106 */ -6068,
/* 6107 */ 420,
/* 6108 */ 6114,
/* 6109 */ 6108,
/* 6110 */ 50038,
/* 6111 */ 6068,
/* 6112 */ -6108,
/* 6113 */ 421,
/* 6114 */ 0,
/* 6115 */ 6068,
/* 6116 */ -6108,
/* 6117 */ 422,
/* 6118 */ 6124,
/* 6119 */ 6118,
/* 6120 */ 50094,
/* 6121 */ 6108,
/* 6122 */ -6118,
/* 6123 */ 423,
/* 6124 */ 0,
/* 6125 */ 6108,
/* 6126 */ -6118,
/* 6127 */ 424,
/* 6128 */ 6134,
/* 6129 */ 6128,
/* 6130 */ 50124,
/* 6131 */ 6118,
/* 6132 */ -6128,
/* 6133 */ 425,
/* 6134 */ 0,
/* 6135 */ 6118,
/* 6136 */ -6128,
/* 6137 */ 426,
/* 6138 */ 6144,
/* 6139 */ 6138,
/* 6140 */ 50478,
/* 6141 */ 6128,
/* 6142 */ -6138,
/* 6143 */ 427,
/* 6144 */ 0,
/* 6145 */ 6128,
/* 6146 */ -6138,
/* 6147 */ 428,
/* 6148 */ 6154,
/* 6149 */ 6148,
/* 6150 */ 50479,
/* 6151 */ 6138,
/* 6152 */ -6148,
/* 6153 */ 429,
/* 6154 */ 0,
/* 6155 */ 6138,
/* 6156 */ -6148,
/* 6157 */ 430,
/* 6158 */ 6166,
/* 6159 */ 6148,
/* 6160 */ 50063,
/* 6161 */ 6240,
/* 6162 */ 50058,
/* 6163 */ 6158,
/* 6164 */ -6158,
/* 6165 */ 431,
/* 6166 */ 0,
/* 6167 */ 6148,
/* 6168 */ -6158,
/* 6169 */ 432,
/* 6170 */ 6176,
/* 6171 */ 5897,
/* 6172 */ 50061,
/* 6173 */ 6158,
/* 6174 */ -6170,
/* 6175 */ 433,
/* 6176 */ 6182,
/* 6177 */ 5897,
/* 6178 */ 50481,
/* 6179 */ 6158,
/* 6180 */ -6170,
/* 6181 */ 434,
/* 6182 */ 6188,
/* 6183 */ 5897,
/* 6184 */ 50489,
/* 6185 */ 6158,
/* 6186 */ -6170,
/* 6187 */ 435,
/* 6188 */ 6194,
/* 6189 */ 5897,
/* 6190 */ 50480,
/* 6191 */ 6158,
/* 6192 */ -6170,
/* 6193 */ 436,
/* 6194 */ 6200,
/* 6195 */ 5897,
/* 6196 */ 50482,
/* 6197 */ 6158,
/* 6198 */ -6170,
/* 6199 */ 437,
/* 6200 */ 6206,
/* 6201 */ 5897,
/* 6202 */ 50483,
/* 6203 */ 6158,
/* 6204 */ -6170,
/* 6205 */ 438,
/* 6206 */ 6212,
/* 6207 */ 5897,
/* 6208 */ 50484,
/* 6209 */ 6158,
/* 6210 */ -6170,
/* 6211 */ 439,
/* 6212 */ 6218,
/* 6213 */ 5897,
/* 6214 */ 50485,
/* 6215 */ 6158,
/* 6216 */ -6170,
/* 6217 */ 440,
/* 6218 */ 6224,
/* 6219 */ 5897,
/* 6220 */ 50487,
/* 6221 */ 6158,
/* 6222 */ -6170,
/* 6223 */ 441,
/* 6224 */ 6230,
/* 6225 */ 5897,
/* 6226 */ 50488,
/* 6227 */ 6158,
/* 6228 */ -6170,
/* 6229 */ 442,
/* 6230 */ 6236,
/* 6231 */ 5897,
/* 6232 */ 50486,
/* 6233 */ 6158,
/* 6234 */ -6170,
/* 6235 */ 443,
/* 6236 */ 0,
/* 6237 */ 6158,
/* 6238 */ -6170,
/* 6239 */ 444,
/* 6240 */ 6246,
/* 6241 */ 6240,
/* 6242 */ 50044,
/* 6243 */ 6170,
/* 6244 */ -6240,
/* 6245 */ 445,
/* 6246 */ 0,
/* 6247 */ 6170,
/* 6248 */ -6240,
/* 6249 */ 446,
/* 6250 */ 0,
/* 6251 */ 6254,
/* 6252 */ -6250,
/* 6253 */ 447,
/* 6254 */ 6259,
/* 6255 */ 6170,
/* 6256 */ 6262,
/* 6257 */ -6254,
/* 6258 */ 944,
/* 6259 */ 0,
/* 6260 */ -6254,
/* 6261 */ 1328,
/* 6262 */ 6268,
/* 6263 */ 50044,
/* 6264 */ 6170,
/* 6265 */ 6262,
/* 6266 */ -6262,
/* 6267 */ 943,
/* 6268 */ 0,
/* 6269 */ -6262,
/* 6270 */ 1327,
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
/* 19 */ 50376,
/* 20 */ 50257,
/* 21 */ 50059,
/* 22 */ -18,
/* 23 */ -1,
/* 24 */ 30,
/* 25 */ 50428,
/* 26 */ -1,
/* 27 */ 50059,
/* 28 */ -18,
/* 29 */ -1,
/* 30 */ 36,
/* 31 */ 50348,
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
/* 120 */ 126,
/* 121 */ 50346,
/* 122 */ -1,
/* 123 */ -1,
/* 124 */ -18,
/* 125 */ -1,
/* 126 */ 0,
/* 127 */ 50446,
/* 128 */ 50257,
/* 129 */ 50123,
/* 130 */ -1,
/* 131 */ 50257,
/* 132 */ -1,
/* 133 */ 50125,
/* 134 */ -18,
/* 135 */ -1,
/* 136 */ 0,
/* 137 */ 50058,
/* 138 */ -136,
/* 139 */ -1,
/* 140 */ 145,
/* 141 */ 50323,
/* 142 */ 50058,
/* 143 */ -140,
/* 144 */ -1,
/* 145 */ 0,
/* 146 */ -140,
/* 147 */ -1,
/* 148 */ 152,
/* 149 */ 50059,
/* 150 */ -148,
/* 151 */ -1,
/* 152 */ 0,
/* 153 */ -148,
/* 154 */ -1,
/* 155 */ 160,
/* 156 */ 50036,
/* 157 */ 50257,
/* 158 */ -155,
/* 159 */ -1,
/* 160 */ 0,
/* 161 */ 50036,
/* 162 */ -1,
/* 163 */ -155,
/* 164 */ -1,
/* 165 */ 171,
/* 166 */ -1,
/* 167 */ 50043,
/* 168 */ -1,
/* 169 */ -165,
/* 170 */ -1,
/* 171 */ 177,
/* 172 */ -1,
/* 173 */ 50045,
/* 174 */ -1,
/* 175 */ -165,
/* 176 */ -1,
/* 177 */ 0,
/* 178 */ -1,
/* 179 */ -165,
/* 180 */ -1,
/* 181 */ 187,
/* 182 */ -1,
/* 183 */ 50042,
/* 184 */ -1,
/* 185 */ -181,
/* 186 */ -1,
/* 187 */ 193,
/* 188 */ -1,
/* 189 */ 50047,
/* 190 */ -1,
/* 191 */ -181,
/* 192 */ -1,
/* 193 */ 199,
/* 194 */ -1,
/* 195 */ 50037,
/* 196 */ -1,
/* 197 */ -181,
/* 198 */ -1,
/* 199 */ 205,
/* 200 */ -1,
/* 201 */ 50124,
/* 202 */ -1,
/* 203 */ -181,
/* 204 */ -1,
/* 205 */ 211,
/* 206 */ -1,
/* 207 */ 50038,
/* 208 */ -1,
/* 209 */ -181,
/* 210 */ -1,
/* 211 */ 0,
/* 212 */ -1,
/* 213 */ -181,
/* 214 */ -1,
/* 215 */ 220,
/* 216 */ 50043,
/* 217 */ -1,
/* 218 */ -215,
/* 219 */ -1,
/* 220 */ 225,
/* 221 */ 50045,
/* 222 */ -1,
/* 223 */ -215,
/* 224 */ -1,
/* 225 */ 0,
/* 226 */ -1,
/* 227 */ -215,
/* 228 */ -1,
/* 229 */ 235,
/* 230 */ 50040,
/* 231 */ -1,
/* 232 */ 50041,
/* 233 */ -229,
/* 234 */ -1,
/* 235 */ 239,
/* 236 */ 50256,
/* 237 */ -229,
/* 238 */ -1,
/* 239 */ 243,
/* 240 */ 50453,
/* 241 */ -229,
/* 242 */ -1,
/* 243 */ 247,
/* 244 */ 50341,
/* 245 */ -229,
/* 246 */ -1,
/* 247 */ 0,
/* 248 */ 50259,
/* 249 */ -229,
/* 250 */ -1,
/* 251 */ 259,
/* 252 */ -1,
/* 253 */ 50100,
/* 254 */ -1,
/* 255 */ 50043,
/* 256 */ -1,
/* 257 */ -251,
/* 258 */ -1,
/* 259 */ 268,
/* 260 */ -1,
/* 261 */ 50100,
/* 262 */ -1,
/* 263 */ 50043,
/* 264 */ 50045,
/* 265 */ -1,
/* 266 */ -251,
/* 267 */ -1,
/* 268 */ 274,
/* 269 */ -1,
/* 270 */ 50100,
/* 271 */ -1,
/* 272 */ -251,
/* 273 */ -1,
/* 274 */ 0,
/* 275 */ -1,
/* 276 */ -251,
/* 277 */ -1,
/* 278 */ 286,
/* 279 */ -1,
/* 280 */ 50100,
/* 281 */ -1,
/* 282 */ 50043,
/* 283 */ -1,
/* 284 */ -278,
/* 285 */ -1,
/* 286 */ 0,
/* 287 */ -1,
/* 288 */ 50100,
/* 289 */ -1,
/* 290 */ -278,
/* 291 */ -1,
/* 292 */ 0,
/* 293 */ -1,
/* 294 */ -1,
/* 295 */ -1,
/* 296 */ -1,
/* 297 */ -1,
/* 298 */ -1,
/* 299 */ -1,
/* 300 */ -292,
/* 301 */ -1,
/* 302 */ 309,
/* 303 */ 50363,
/* 304 */ 50040,
/* 305 */ -1,
/* 306 */ 50041,
/* 307 */ -302,
/* 308 */ -1,
/* 309 */ 0,
/* 310 */ -302,
/* 311 */ -1,
/* 312 */ 317,
/* 313 */ 50256,
/* 314 */ 50037,
/* 315 */ -312,
/* 316 */ -1,
/* 317 */ 0,
/* 318 */ -312,
/* 319 */ -1,
/* 320 */ 324,
/* 321 */ -1,
/* 322 */ -320,
/* 323 */ -1,
/* 324 */ 0,
/* 325 */ -320,
/* 326 */ -1,
/* 327 */ 331,
/* 328 */ 50314,
/* 329 */ -327,
/* 330 */ -1,
/* 331 */ 335,
/* 332 */ 50297,
/* 333 */ -327,
/* 334 */ -1,
/* 335 */ 0,
/* 336 */ -327,
/* 337 */ -1,
/* 338 */ 342,
/* 339 */ -1,
/* 340 */ -338,
/* 341 */ -1,
/* 342 */ 0,
/* 343 */ 50414,
/* 344 */ -1,
/* 345 */ -338,
/* 346 */ -1,
/* 347 */ 352,
/* 348 */ 50043,
/* 349 */ 50256,
/* 350 */ -347,
/* 351 */ -1,
/* 352 */ 357,
/* 353 */ 50045,
/* 354 */ 50256,
/* 355 */ -347,
/* 356 */ -1,
/* 357 */ 0,
/* 358 */ -347,
/* 359 */ -1,
/* 360 */ 368,
/* 361 */ 50466,
/* 362 */ 50091,
/* 363 */ -1,
/* 364 */ 50093,
/* 365 */ -1,
/* 366 */ -360,
/* 367 */ -1,
/* 368 */ 374,
/* 369 */ 50395,
/* 370 */ -1,
/* 371 */ -1,
/* 372 */ -360,
/* 373 */ -1,
/* 374 */ 383,
/* 375 */ 50395,
/* 376 */ 50414,
/* 377 */ -1,
/* 378 */ 50452,
/* 379 */ -1,
/* 380 */ -1,
/* 381 */ -360,
/* 382 */ -1,
/* 383 */ 390,
/* 384 */ 50395,
/* 385 */ 50414,
/* 386 */ -1,
/* 387 */ -1,
/* 388 */ -360,
/* 389 */ -1,
/* 390 */ 396,
/* 391 */ 50395,
/* 392 */ 50414,
/* 393 */ -1,
/* 394 */ -360,
/* 395 */ -1,
/* 396 */ 406,
/* 397 */ 50293,
/* 398 */ 50040,
/* 399 */ -1,
/* 400 */ 50044,
/* 401 */ -1,
/* 402 */ 50041,
/* 403 */ -1,
/* 404 */ -360,
/* 405 */ -1,
/* 406 */ 0,
/* 407 */ -360,
/* 408 */ -1,
/* 409 */ 415,
/* 410 */ -1,
/* 411 */ -1,
/* 412 */ -1,
/* 413 */ -409,
/* 414 */ -1,
/* 415 */ 0,
/* 416 */ -409,
/* 417 */ -1,
/* 418 */ 422,
/* 419 */ 50044,
/* 420 */ -418,
/* 421 */ -1,
/* 422 */ 0,
/* 423 */ -418,
/* 424 */ -1,
/* 425 */ 0,
/* 426 */ 50352,
/* 427 */ -1,
/* 428 */ -1,
/* 429 */ -1,
/* 430 */ 50059,
/* 431 */ -425,
/* 432 */ -1,
/* 433 */ 0,
/* 434 */ 50058,
/* 435 */ -433,
/* 436 */ -1,
/* 437 */ 443,
/* 438 */ -1,
/* 439 */ -1,
/* 440 */ -1,
/* 441 */ -437,
/* 442 */ -1,
/* 443 */ 0,
/* 444 */ -437,
/* 445 */ -1,
/* 446 */ 450,
/* 447 */ 50044,
/* 448 */ -446,
/* 449 */ -1,
/* 450 */ 0,
/* 451 */ 50332,
/* 452 */ -446,
/* 453 */ -1,
/* 454 */ 0,
/* 455 */ -1,
/* 456 */ -1,
/* 457 */ -1,
/* 458 */ -1,
/* 459 */ -1,
/* 460 */ 50059,
/* 461 */ -454,
/* 462 */ -1,
/* 463 */ 467,
/* 464 */ 50364,
/* 465 */ -463,
/* 466 */ -1,
/* 467 */ 0,
/* 468 */ 50353,
/* 469 */ -463,
/* 470 */ -1,
/* 471 */ 475,
/* 472 */ 50058,
/* 473 */ -471,
/* 474 */ -1,
/* 475 */ 0,
/* 476 */ -471,
/* 477 */ -1,
/* 478 */ 482,
/* 479 */ -1,
/* 480 */ -478,
/* 481 */ -1,
/* 482 */ 0,
/* 483 */ -478,
/* 484 */ -1,
/* 485 */ 489,
/* 486 */ 50259,
/* 487 */ -485,
/* 488 */ -1,
/* 489 */ 0,
/* 490 */ 50256,
/* 491 */ -485,
/* 492 */ -1,
/* 493 */ 498,
/* 494 */ 50396,
/* 495 */ -1,
/* 496 */ -493,
/* 497 */ -1,
/* 498 */ 0,
/* 499 */ -493,
/* 500 */ -1,
/* 501 */ 0,
/* 502 */ -1,
/* 503 */ -1,
/* 504 */ -1,
/* 505 */ -501,
/* 506 */ -1,
/* 507 */ 511,
/* 508 */ -1,
/* 509 */ -507,
/* 510 */ -1,
/* 511 */ 0,
/* 512 */ -507,
/* 513 */ -1,
/* 514 */ 518,
/* 515 */ 50259,
/* 516 */ -514,
/* 517 */ -1,
/* 518 */ 0,
/* 519 */ 50256,
/* 520 */ -514,
/* 521 */ -1,
/* 522 */ 527,
/* 523 */ 50396,
/* 524 */ -1,
/* 525 */ -522,
/* 526 */ -1,
/* 527 */ 0,
/* 528 */ -522,
/* 529 */ -1,
/* 530 */ 0,
/* 531 */ 50396,
/* 532 */ 50338,
/* 533 */ -1,
/* 534 */ -1,
/* 535 */ 50059,
/* 536 */ -530,
/* 537 */ -1,
/* 538 */ 544,
/* 539 */ 50044,
/* 540 */ -1,
/* 541 */ -1,
/* 542 */ -538,
/* 543 */ -1,
/* 544 */ 0,
/* 545 */ -538,
/* 546 */ -1,
/* 547 */ 0,
/* 548 */ -1,
/* 549 */ -1,
/* 550 */ -1,
/* 551 */ -547,
/* 552 */ -1,
/* 553 */ 559,
/* 554 */ 50044,
/* 555 */ -1,
/* 556 */ -1,
/* 557 */ -553,
/* 558 */ -1,
/* 559 */ 0,
/* 560 */ -553,
/* 561 */ -1,
/* 562 */ 567,
/* 563 */ 50257,
/* 564 */ -1,
/* 565 */ -562,
/* 566 */ -1,
/* 567 */ 0,
/* 568 */ -1,
/* 569 */ -562,
/* 570 */ -1,
/* 571 */ 577,
/* 572 */ 50047,
/* 573 */ 50257,
/* 574 */ -1,
/* 575 */ -571,
/* 576 */ -1,
/* 577 */ 0,
/* 578 */ -571,
/* 579 */ -1,
/* 580 */ 0,
/* 581 */ -1,
/* 582 */ -580,
/* 583 */ -1,
/* 584 */ 0,
/* 585 */ -1,
/* 586 */ -1,
/* 587 */ -584,
/* 588 */ -1,
/* 589 */ 594,
/* 590 */ -1,
/* 591 */ -1,
/* 592 */ -589,
/* 593 */ -1,
/* 594 */ 599,
/* 595 */ 50045,
/* 596 */ -1,
/* 597 */ -589,
/* 598 */ -1,
/* 599 */ 604,
/* 600 */ 50061,
/* 601 */ -1,
/* 602 */ -589,
/* 603 */ -1,
/* 604 */ 0,
/* 605 */ -1,
/* 606 */ 50037,
/* 607 */ -589,
/* 608 */ -1,
/* 609 */ 613,
/* 610 */ 50043,
/* 611 */ -609,
/* 612 */ -1,
/* 613 */ 0,
/* 614 */ -609,
/* 615 */ -1,
/* 616 */ 622,
/* 617 */ 50040,
/* 618 */ -1,
/* 619 */ 50041,
/* 620 */ -616,
/* 621 */ -1,
/* 622 */ 0,
/* 623 */ -616,
/* 624 */ -1,
/* 625 */ 630,
/* 626 */ 50383,
/* 627 */ -1,
/* 628 */ -625,
/* 629 */ -1,
/* 630 */ 635,
/* 631 */ 50380,
/* 632 */ -1,
/* 633 */ -625,
/* 634 */ -1,
/* 635 */ 0,
/* 636 */ 50387,
/* 637 */ -1,
/* 638 */ -625,
/* 639 */ -1,
/* 640 */ 0,
/* 641 */ 50384,
/* 642 */ 50257,
/* 643 */ 50058,
/* 644 */ -1,
/* 645 */ -1,
/* 646 */ 50123,
/* 647 */ -1,
/* 648 */ 50125,
/* 649 */ -640,
/* 650 */ -1,
/* 651 */ 657,
/* 652 */ 50044,
/* 653 */ -1,
/* 654 */ -1,
/* 655 */ -651,
/* 656 */ -1,
/* 657 */ 0,
/* 658 */ -651,
/* 659 */ -1,
/* 660 */ 665,
/* 661 */ 50044,
/* 662 */ -1,
/* 663 */ -660,
/* 664 */ -1,
/* 665 */ 0,
/* 666 */ -660,
/* 667 */ -1,
/* 668 */ 673,
/* 669 */ -1,
/* 670 */ -1,
/* 671 */ -668,
/* 672 */ -1,
/* 673 */ 0,
/* 674 */ -668,
/* 675 */ -1,
/* 676 */ 683,
/* 677 */ 50323,
/* 678 */ -1,
/* 679 */ 50257,
/* 680 */ 50059,
/* 681 */ -676,
/* 682 */ -1,
/* 683 */ 690,
/* 684 */ 50322,
/* 685 */ -1,
/* 686 */ -1,
/* 687 */ 50059,
/* 688 */ -676,
/* 689 */ -1,
/* 690 */ 697,
/* 691 */ -1,
/* 692 */ -1,
/* 693 */ -1,
/* 694 */ 50059,
/* 695 */ -676,
/* 696 */ -1,
/* 697 */ 706,
/* 698 */ -1,
/* 699 */ -1,
/* 700 */ -1,
/* 701 */ 50047,
/* 702 */ -1,
/* 703 */ 50059,
/* 704 */ -676,
/* 705 */ -1,
/* 706 */ 713,
/* 707 */ 50360,
/* 708 */ -1,
/* 709 */ -1,
/* 710 */ 50059,
/* 711 */ -676,
/* 712 */ -1,
/* 713 */ 720,
/* 714 */ 50361,
/* 715 */ -1,
/* 716 */ -1,
/* 717 */ 50059,
/* 718 */ -676,
/* 719 */ -1,
/* 720 */ 728,
/* 721 */ -1,
/* 722 */ 50362,
/* 723 */ -1,
/* 724 */ -1,
/* 725 */ 50059,
/* 726 */ -676,
/* 727 */ -1,
/* 728 */ 732,
/* 729 */ -1,
/* 730 */ -676,
/* 731 */ -1,
/* 732 */ 740,
/* 733 */ 50435,
/* 734 */ 50091,
/* 735 */ -1,
/* 736 */ 50093,
/* 737 */ 50059,
/* 738 */ -676,
/* 739 */ -1,
/* 740 */ 750,
/* 741 */ 50435,
/* 742 */ 50091,
/* 743 */ -1,
/* 744 */ 50044,
/* 745 */ -1,
/* 746 */ 50093,
/* 747 */ 50059,
/* 748 */ -676,
/* 749 */ -1,
/* 750 */ 762,
/* 751 */ 50435,
/* 752 */ 50091,
/* 753 */ -1,
/* 754 */ 50044,
/* 755 */ -1,
/* 756 */ 50044,
/* 757 */ -1,
/* 758 */ 50093,
/* 759 */ 50059,
/* 760 */ -676,
/* 761 */ -1,
/* 762 */ 776,
/* 763 */ 50435,
/* 764 */ 50091,
/* 765 */ -1,
/* 766 */ 50044,
/* 767 */ -1,
/* 768 */ 50044,
/* 769 */ -1,
/* 770 */ 50044,
/* 771 */ -1,
/* 772 */ 50093,
/* 773 */ 50059,
/* 774 */ -676,
/* 775 */ -1,
/* 776 */ 783,
/* 777 */ 50419,
/* 778 */ -1,
/* 779 */ -1,
/* 780 */ 50059,
/* 781 */ -676,
/* 782 */ -1,
/* 783 */ 790,
/* 784 */ 50365,
/* 785 */ -1,
/* 786 */ -1,
/* 787 */ 50059,
/* 788 */ -676,
/* 789 */ -1,
/* 790 */ 798,
/* 791 */ 50294,
/* 792 */ -1,
/* 793 */ -1,
/* 794 */ -1,
/* 795 */ 50059,
/* 796 */ -676,
/* 797 */ -1,
/* 798 */ 804,
/* 799 */ 50292,
/* 800 */ 50257,
/* 801 */ 50059,
/* 802 */ -676,
/* 803 */ -1,
/* 804 */ 810,
/* 805 */ 50292,
/* 806 */ -1,
/* 807 */ 50059,
/* 808 */ -676,
/* 809 */ -1,
/* 810 */ 817,
/* 811 */ 50320,
/* 812 */ -1,
/* 813 */ -1,
/* 814 */ 50059,
/* 815 */ -676,
/* 816 */ -1,
/* 817 */ 824,
/* 818 */ 50290,
/* 819 */ -1,
/* 820 */ -1,
/* 821 */ 50059,
/* 822 */ -676,
/* 823 */ -1,
/* 824 */ 831,
/* 825 */ 50377,
/* 826 */ -1,
/* 827 */ -1,
/* 828 */ 50059,
/* 829 */ -676,
/* 830 */ -1,
/* 831 */ 839,
/* 832 */ 50342,
/* 833 */ -1,
/* 834 */ -1,
/* 835 */ -1,
/* 836 */ 50059,
/* 837 */ -676,
/* 838 */ -1,
/* 839 */ 846,
/* 840 */ 50346,
/* 841 */ -1,
/* 842 */ -1,
/* 843 */ 50059,
/* 844 */ -676,
/* 845 */ -1,
/* 846 */ 852,
/* 847 */ -1,
/* 848 */ -1,
/* 849 */ 50059,
/* 850 */ -676,
/* 851 */ -1,
/* 852 */ 859,
/* 853 */ 50426,
/* 854 */ -1,
/* 855 */ -1,
/* 856 */ 50059,
/* 857 */ -676,
/* 858 */ -1,
/* 859 */ 867,
/* 860 */ 50430,
/* 861 */ -1,
/* 862 */ -1,
/* 863 */ -1,
/* 864 */ 50059,
/* 865 */ -676,
/* 866 */ -1,
/* 867 */ 875,
/* 868 */ 50385,
/* 869 */ -1,
/* 870 */ -1,
/* 871 */ -1,
/* 872 */ 50059,
/* 873 */ -676,
/* 874 */ -1,
/* 875 */ 882,
/* 876 */ 50432,
/* 877 */ -1,
/* 878 */ -1,
/* 879 */ 50059,
/* 880 */ -676,
/* 881 */ -1,
/* 882 */ 889,
/* 883 */ 50327,
/* 884 */ -1,
/* 885 */ -1,
/* 886 */ 50059,
/* 887 */ -676,
/* 888 */ -1,
/* 889 */ 896,
/* 890 */ 50404,
/* 891 */ -1,
/* 892 */ -1,
/* 893 */ 50059,
/* 894 */ -676,
/* 895 */ -1,
/* 896 */ 900,
/* 897 */ -1,
/* 898 */ -676,
/* 899 */ -1,
/* 900 */ 904,
/* 901 */ -1,
/* 902 */ -676,
/* 903 */ -1,
/* 904 */ 908,
/* 905 */ -1,
/* 906 */ -676,
/* 907 */ -1,
/* 908 */ 0,
/* 909 */ -1,
/* 910 */ -676,
/* 911 */ -1,
/* 912 */ 916,
/* 913 */ 50058,
/* 914 */ -912,
/* 915 */ -1,
/* 916 */ 0,
/* 917 */ -912,
/* 918 */ -1,
/* 919 */ 923,
/* 920 */ 50058,
/* 921 */ -919,
/* 922 */ -1,
/* 923 */ 0,
/* 924 */ -919,
/* 925 */ -1,
/* 926 */ 930,
/* 927 */ 50370,
/* 928 */ -926,
/* 929 */ -1,
/* 930 */ 0,
/* 931 */ 50304,
/* 932 */ -926,
/* 933 */ -1,
/* 934 */ 938,
/* 935 */ 50058,
/* 936 */ -934,
/* 937 */ -1,
/* 938 */ 0,
/* 939 */ -934,
/* 940 */ -1,
/* 941 */ 945,
/* 942 */ 50370,
/* 943 */ -941,
/* 944 */ -1,
/* 945 */ 0,
/* 946 */ 50304,
/* 947 */ -941,
/* 948 */ -1,
/* 949 */ 953,
/* 950 */ 50058,
/* 951 */ -949,
/* 952 */ -1,
/* 953 */ 0,
/* 954 */ -949,
/* 955 */ -1,
/* 956 */ 960,
/* 957 */ 50058,
/* 958 */ -956,
/* 959 */ -1,
/* 960 */ 0,
/* 961 */ -956,
/* 962 */ -1,
/* 963 */ 967,
/* 964 */ 50058,
/* 965 */ -963,
/* 966 */ -1,
/* 967 */ 0,
/* 968 */ -963,
/* 969 */ -1,
/* 970 */ 974,
/* 971 */ 50043,
/* 972 */ -970,
/* 973 */ -1,
/* 974 */ 0,
/* 975 */ -970,
/* 976 */ -1,
/* 977 */ 981,
/* 978 */ 50058,
/* 979 */ -977,
/* 980 */ -1,
/* 981 */ 0,
/* 982 */ -977,
/* 983 */ -1,
/* 984 */ 988,
/* 985 */ 50058,
/* 986 */ -984,
/* 987 */ -1,
/* 988 */ 0,
/* 989 */ -984,
/* 990 */ -1,
/* 991 */ 997,
/* 992 */ -1,
/* 993 */ -1,
/* 994 */ -1,
/* 995 */ -991,
/* 996 */ -1,
/* 997 */ 0,
/* 998 */ -991,
/* 999 */ -1,
/* 1000 */ 1004,
/* 1001 */ 50044,
/* 1002 */ -1000,
/* 1003 */ -1,
/* 1004 */ 0,
/* 1005 */ -1000,
/* 1006 */ -1,
/* 1007 */ 1011,
/* 1008 */ 50058,
/* 1009 */ -1007,
/* 1010 */ -1,
/* 1011 */ 0,
/* 1012 */ -1007,
/* 1013 */ -1,
/* 1014 */ 1020,
/* 1015 */ -1,
/* 1016 */ -1,
/* 1017 */ -1,
/* 1018 */ -1014,
/* 1019 */ -1,
/* 1020 */ 0,
/* 1021 */ -1014,
/* 1022 */ -1,
/* 1023 */ 1027,
/* 1024 */ 50044,
/* 1025 */ -1023,
/* 1026 */ -1,
/* 1027 */ 0,
/* 1028 */ -1023,
/* 1029 */ -1,
/* 1030 */ 1034,
/* 1031 */ 50058,
/* 1032 */ -1030,
/* 1033 */ -1,
/* 1034 */ 0,
/* 1035 */ -1030,
/* 1036 */ -1,
/* 1037 */ 1043,
/* 1038 */ 50044,
/* 1039 */ -1,
/* 1040 */ -1,
/* 1041 */ -1037,
/* 1042 */ -1,
/* 1043 */ 0,
/* 1044 */ -1037,
/* 1045 */ -1,
/* 1046 */ 1050,
/* 1047 */ 50058,
/* 1048 */ -1046,
/* 1049 */ -1,
/* 1050 */ 0,
/* 1051 */ -1046,
/* 1052 */ -1,
/* 1053 */ 1057,
/* 1054 */ 50058,
/* 1055 */ -1053,
/* 1056 */ -1,
/* 1057 */ 0,
/* 1058 */ -1053,
/* 1059 */ -1,
/* 1060 */ 1064,
/* 1061 */ 50058,
/* 1062 */ -1060,
/* 1063 */ -1,
/* 1064 */ 0,
/* 1065 */ -1060,
/* 1066 */ -1,
/* 1067 */ 1071,
/* 1068 */ 50058,
/* 1069 */ -1067,
/* 1070 */ -1,
/* 1071 */ 0,
/* 1072 */ -1067,
/* 1073 */ -1,
/* 1074 */ 1080,
/* 1075 */ -1,
/* 1076 */ 50044,
/* 1077 */ -1,
/* 1078 */ -1074,
/* 1079 */ -1,
/* 1080 */ 0,
/* 1081 */ -1074,
/* 1082 */ -1,
/* 1083 */ 1087,
/* 1084 */ 50058,
/* 1085 */ -1083,
/* 1086 */ -1,
/* 1087 */ 0,
/* 1088 */ -1083,
/* 1089 */ -1,
/* 1090 */ 1095,
/* 1091 */ -1,
/* 1092 */ -1,
/* 1093 */ -1090,
/* 1094 */ -1,
/* 1095 */ 0,
/* 1096 */ -1090,
/* 1097 */ -1,
/* 1098 */ 1104,
/* 1099 */ 50044,
/* 1100 */ -1,
/* 1101 */ -1,
/* 1102 */ -1098,
/* 1103 */ -1,
/* 1104 */ 0,
/* 1105 */ -1098,
/* 1106 */ -1,
/* 1107 */ 1111,
/* 1108 */ 50058,
/* 1109 */ -1107,
/* 1110 */ -1,
/* 1111 */ 0,
/* 1112 */ -1107,
/* 1113 */ -1,
/* 1114 */ 1118,
/* 1115 */ 50058,
/* 1116 */ -1114,
/* 1117 */ -1,
/* 1118 */ 0,
/* 1119 */ -1114,
/* 1120 */ -1,
/* 1121 */ 1125,
/* 1122 */ 50037,
/* 1123 */ -1121,
/* 1124 */ -1,
/* 1125 */ 0,
/* 1126 */ -1121,
/* 1127 */ -1,
/* 1128 */ 1132,
/* 1129 */ 50058,
/* 1130 */ -1128,
/* 1131 */ -1,
/* 1132 */ 0,
/* 1133 */ -1128,
/* 1134 */ -1,
/* 1135 */ 1139,
/* 1136 */ 50037,
/* 1137 */ -1135,
/* 1138 */ -1,
/* 1139 */ 0,
/* 1140 */ -1135,
/* 1141 */ -1,
/* 1142 */ 1146,
/* 1143 */ 50058,
/* 1144 */ -1142,
/* 1145 */ -1,
/* 1146 */ 0,
/* 1147 */ -1142,
/* 1148 */ -1,
/* 1149 */ 1155,
/* 1150 */ -1,
/* 1151 */ -1,
/* 1152 */ -1,
/* 1153 */ -1149,
/* 1154 */ -1,
/* 1155 */ 0,
/* 1156 */ -1149,
/* 1157 */ -1,
/* 1158 */ 1162,
/* 1159 */ 50044,
/* 1160 */ -1158,
/* 1161 */ -1,
/* 1162 */ 0,
/* 1163 */ -1158,
/* 1164 */ -1,
/* 1165 */ 1169,
/* 1166 */ 50058,
/* 1167 */ -1165,
/* 1168 */ -1,
/* 1169 */ 0,
/* 1170 */ -1165,
/* 1171 */ -1,
/* 1172 */ 1178,
/* 1173 */ -1,
/* 1174 */ -1,
/* 1175 */ -1,
/* 1176 */ -1172,
/* 1177 */ -1,
/* 1178 */ 0,
/* 1179 */ -1172,
/* 1180 */ -1,
/* 1181 */ 1185,
/* 1182 */ 50044,
/* 1183 */ -1181,
/* 1184 */ -1,
/* 1185 */ 0,
/* 1186 */ -1181,
/* 1187 */ -1,
/* 1188 */ 1192,
/* 1189 */ 50058,
/* 1190 */ -1188,
/* 1191 */ -1,
/* 1192 */ 0,
/* 1193 */ -1188,
/* 1194 */ -1,
/* 1195 */ 1201,
/* 1196 */ -1,
/* 1197 */ -1,
/* 1198 */ -1,
/* 1199 */ -1195,
/* 1200 */ -1,
/* 1201 */ 0,
/* 1202 */ -1195,
/* 1203 */ -1,
/* 1204 */ 1208,
/* 1205 */ 50044,
/* 1206 */ -1204,
/* 1207 */ -1,
/* 1208 */ 0,
/* 1209 */ -1204,
/* 1210 */ -1,
/* 1211 */ 1215,
/* 1212 */ 50274,
/* 1213 */ -1211,
/* 1214 */ -1,
/* 1215 */ 1220,
/* 1216 */ 50300,
/* 1217 */ 50274,
/* 1218 */ -1211,
/* 1219 */ -1,
/* 1220 */ 1225,
/* 1221 */ 50372,
/* 1222 */ 50274,
/* 1223 */ -1211,
/* 1224 */ -1,
/* 1225 */ 0,
/* 1226 */ 50317,
/* 1227 */ 50274,
/* 1228 */ -1211,
/* 1229 */ -1,
/* 1230 */ 1240,
/* 1231 */ -1,
/* 1232 */ 50349,
/* 1233 */ -1,
/* 1234 */ -1,
/* 1235 */ -1,
/* 1236 */ -1,
/* 1237 */ -1,
/* 1238 */ -1230,
/* 1239 */ -1,
/* 1240 */ 0,
/* 1241 */ -1,
/* 1242 */ 50349,
/* 1243 */ -1,
/* 1244 */ -1,
/* 1245 */ -1230,
/* 1246 */ -1,
/* 1247 */ 1251,
/* 1248 */ -1,
/* 1249 */ -1247,
/* 1250 */ -1,
/* 1251 */ 1256,
/* 1252 */ -1,
/* 1253 */ 50037,
/* 1254 */ -1247,
/* 1255 */ -1,
/* 1256 */ 0,
/* 1257 */ -1247,
/* 1258 */ -1,
/* 1259 */ 1263,
/* 1260 */ -1,
/* 1261 */ -1259,
/* 1262 */ -1,
/* 1263 */ 0,
/* 1264 */ -1259,
/* 1265 */ -1,
/* 1266 */ 1270,
/* 1267 */ 50316,
/* 1268 */ -1266,
/* 1269 */ -1,
/* 1270 */ 0,
/* 1271 */ -1266,
/* 1272 */ -1,
/* 1273 */ 1280,
/* 1274 */ 50040,
/* 1275 */ -1,
/* 1276 */ -1,
/* 1277 */ 50041,
/* 1278 */ -1273,
/* 1279 */ -1,
/* 1280 */ 0,
/* 1281 */ -1273,
/* 1282 */ -1,
/* 1283 */ 1287,
/* 1284 */ 50319,
/* 1285 */ -1283,
/* 1286 */ -1,
/* 1287 */ 1291,
/* 1288 */ 50304,
/* 1289 */ -1283,
/* 1290 */ -1,
/* 1291 */ 0,
/* 1292 */ 50370,
/* 1293 */ -1283,
/* 1294 */ -1,
/* 1295 */ 1304,
/* 1296 */ 50287,
/* 1297 */ -1,
/* 1298 */ -1,
/* 1299 */ -1,
/* 1300 */ -1,
/* 1301 */ -1,
/* 1302 */ -1295,
/* 1303 */ -1,
/* 1304 */ 0,
/* 1305 */ -1295,
/* 1306 */ -1,
/* 1307 */ 1311,
/* 1308 */ -1,
/* 1309 */ -1307,
/* 1310 */ -1,
/* 1311 */ 0,
/* 1312 */ -1307,
/* 1313 */ -1,
/* 1314 */ 1318,
/* 1315 */ -1,
/* 1316 */ -1314,
/* 1317 */ -1,
/* 1318 */ 0,
/* 1319 */ -1314,
/* 1320 */ -1,
/* 1321 */ 1325,
/* 1322 */ 50316,
/* 1323 */ -1321,
/* 1324 */ -1,
/* 1325 */ 0,
/* 1326 */ -1321,
/* 1327 */ -1,
/* 1328 */ 1335,
/* 1329 */ 50040,
/* 1330 */ -1,
/* 1331 */ -1,
/* 1332 */ 50041,
/* 1333 */ -1328,
/* 1334 */ -1,
/* 1335 */ 0,
/* 1336 */ -1328,
/* 1337 */ -1,
/* 1338 */ 1342,
/* 1339 */ 50319,
/* 1340 */ -1338,
/* 1341 */ -1,
/* 1342 */ 1346,
/* 1343 */ 50304,
/* 1344 */ -1338,
/* 1345 */ -1,
/* 1346 */ 0,
/* 1347 */ 50370,
/* 1348 */ -1338,
/* 1349 */ -1,
/* 1350 */ 1354,
/* 1351 */ -1,
/* 1352 */ -1350,
/* 1353 */ -1,
/* 1354 */ 0,
/* 1355 */ -1350,
/* 1356 */ -1,
/* 1357 */ 1362,
/* 1358 */ -1,
/* 1359 */ -1,
/* 1360 */ -1357,
/* 1361 */ -1,
/* 1362 */ 0,
/* 1363 */ 50126,
/* 1364 */ -1,
/* 1365 */ -1,
/* 1366 */ -1357,
/* 1367 */ -1,
/* 1368 */ 1372,
/* 1369 */ 50044,
/* 1370 */ -1368,
/* 1371 */ -1,
/* 1372 */ 0,
/* 1373 */ -1368,
/* 1374 */ -1,
/* 1375 */ 1379,
/* 1376 */ 50044,
/* 1377 */ -1375,
/* 1378 */ -1,
/* 1379 */ 0,
/* 1380 */ -1375,
/* 1381 */ -1,
/* 1382 */ 0,
/* 1383 */ 50276,
/* 1384 */ -1,
/* 1385 */ -1,
/* 1386 */ -1382,
/* 1387 */ -1,
/* 1388 */ 1392,
/* 1389 */ 50058,
/* 1390 */ -1388,
/* 1391 */ -1,
/* 1392 */ 0,
/* 1393 */ -1388,
/* 1394 */ -1,
/* 1395 */ 0,
/* 1396 */ 50368,
/* 1397 */ 50257,
/* 1398 */ 50058,
/* 1399 */ -1,
/* 1400 */ 50123,
/* 1401 */ -1,
/* 1402 */ 50125,
/* 1403 */ -1395,
/* 1404 */ -1,
/* 1405 */ 1410,
/* 1406 */ -1,
/* 1407 */ -1,
/* 1408 */ -1405,
/* 1409 */ -1,
/* 1410 */ 0,
/* 1411 */ -1405,
/* 1412 */ -1,
/* 1413 */ 1420,
/* 1414 */ 50323,
/* 1415 */ -1,
/* 1416 */ 50257,
/* 1417 */ 50059,
/* 1418 */ -1413,
/* 1419 */ -1,
/* 1420 */ 1427,
/* 1421 */ 50370,
/* 1422 */ -1,
/* 1423 */ -1,
/* 1424 */ 50059,
/* 1425 */ -1413,
/* 1426 */ -1,
/* 1427 */ 1431,
/* 1428 */ -1,
/* 1429 */ -1413,
/* 1430 */ -1,
/* 1431 */ 1438,
/* 1432 */ 50425,
/* 1433 */ -1,
/* 1434 */ -1,
/* 1435 */ 50059,
/* 1436 */ -1413,
/* 1437 */ -1,
/* 1438 */ 1445,
/* 1439 */ 50369,
/* 1440 */ -1,
/* 1441 */ -1,
/* 1442 */ 50059,
/* 1443 */ -1413,
/* 1444 */ -1,
/* 1445 */ 1452,
/* 1446 */ 50313,
/* 1447 */ -1,
/* 1448 */ -1,
/* 1449 */ 50059,
/* 1450 */ -1413,
/* 1451 */ -1,
/* 1452 */ 1459,
/* 1453 */ 50447,
/* 1454 */ -1,
/* 1455 */ -1,
/* 1456 */ 50059,
/* 1457 */ -1413,
/* 1458 */ -1,
/* 1459 */ 1466,
/* 1460 */ 50283,
/* 1461 */ -1,
/* 1462 */ -1,
/* 1463 */ 50059,
/* 1464 */ -1413,
/* 1465 */ -1,
/* 1466 */ 1473,
/* 1467 */ 50430,
/* 1468 */ -1,
/* 1469 */ -1,
/* 1470 */ 50059,
/* 1471 */ -1413,
/* 1472 */ -1,
/* 1473 */ 1481,
/* 1474 */ 50430,
/* 1475 */ -1,
/* 1476 */ -1,
/* 1477 */ 50037,
/* 1478 */ 50059,
/* 1479 */ -1413,
/* 1480 */ -1,
/* 1481 */ 1490,
/* 1482 */ 50430,
/* 1483 */ -1,
/* 1484 */ 50043,
/* 1485 */ -1,
/* 1486 */ 50037,
/* 1487 */ 50059,
/* 1488 */ -1413,
/* 1489 */ -1,
/* 1490 */ 1497,
/* 1491 */ 50400,
/* 1492 */ -1,
/* 1493 */ -1,
/* 1494 */ 50059,
/* 1495 */ -1413,
/* 1496 */ -1,
/* 1497 */ 1504,
/* 1498 */ 50415,
/* 1499 */ -1,
/* 1500 */ -1,
/* 1501 */ 50059,
/* 1502 */ -1413,
/* 1503 */ -1,
/* 1504 */ 1511,
/* 1505 */ 50379,
/* 1506 */ -1,
/* 1507 */ -1,
/* 1508 */ 50059,
/* 1509 */ -1413,
/* 1510 */ -1,
/* 1511 */ 1519,
/* 1512 */ 50312,
/* 1513 */ -1,
/* 1514 */ -1,
/* 1515 */ -1,
/* 1516 */ 50059,
/* 1517 */ -1413,
/* 1518 */ -1,
/* 1519 */ 1526,
/* 1520 */ 50461,
/* 1521 */ -1,
/* 1522 */ -1,
/* 1523 */ 50059,
/* 1524 */ -1413,
/* 1525 */ -1,
/* 1526 */ 1533,
/* 1527 */ 50426,
/* 1528 */ -1,
/* 1529 */ -1,
/* 1530 */ 50059,
/* 1531 */ -1413,
/* 1532 */ -1,
/* 1533 */ 1540,
/* 1534 */ 50392,
/* 1535 */ -1,
/* 1536 */ -1,
/* 1537 */ 50059,
/* 1538 */ -1413,
/* 1539 */ -1,
/* 1540 */ 1547,
/* 1541 */ 50320,
/* 1542 */ -1,
/* 1543 */ -1,
/* 1544 */ 50059,
/* 1545 */ -1413,
/* 1546 */ -1,
/* 1547 */ 1554,
/* 1548 */ 50309,
/* 1549 */ -1,
/* 1550 */ -1,
/* 1551 */ 50059,
/* 1552 */ -1413,
/* 1553 */ -1,
/* 1554 */ 1561,
/* 1555 */ 50402,
/* 1556 */ -1,
/* 1557 */ -1,
/* 1558 */ 50059,
/* 1559 */ -1413,
/* 1560 */ -1,
/* 1561 */ 1568,
/* 1562 */ 50290,
/* 1563 */ -1,
/* 1564 */ -1,
/* 1565 */ 50059,
/* 1566 */ -1413,
/* 1567 */ -1,
/* 1568 */ 1575,
/* 1569 */ 50361,
/* 1570 */ -1,
/* 1571 */ -1,
/* 1572 */ 50059,
/* 1573 */ -1413,
/* 1574 */ -1,
/* 1575 */ 1582,
/* 1576 */ 50432,
/* 1577 */ -1,
/* 1578 */ -1,
/* 1579 */ 50059,
/* 1580 */ -1413,
/* 1581 */ -1,
/* 1582 */ 1589,
/* 1583 */ 50345,
/* 1584 */ -1,
/* 1585 */ -1,
/* 1586 */ 50059,
/* 1587 */ -1413,
/* 1588 */ -1,
/* 1589 */ 1596,
/* 1590 */ 50346,
/* 1591 */ -1,
/* 1592 */ -1,
/* 1593 */ 50059,
/* 1594 */ -1413,
/* 1595 */ -1,
/* 1596 */ 1603,
/* 1597 */ 50357,
/* 1598 */ -1,
/* 1599 */ -1,
/* 1600 */ 50059,
/* 1601 */ -1413,
/* 1602 */ -1,
/* 1603 */ 1610,
/* 1604 */ 50301,
/* 1605 */ -1,
/* 1606 */ -1,
/* 1607 */ 50059,
/* 1608 */ -1413,
/* 1609 */ -1,
/* 1610 */ 1617,
/* 1611 */ 50463,
/* 1612 */ -1,
/* 1613 */ -1,
/* 1614 */ 50059,
/* 1615 */ -1413,
/* 1616 */ -1,
/* 1617 */ 1624,
/* 1618 */ 50462,
/* 1619 */ -1,
/* 1620 */ -1,
/* 1621 */ 50059,
/* 1622 */ -1413,
/* 1623 */ -1,
/* 1624 */ 1631,
/* 1625 */ 50375,
/* 1626 */ -1,
/* 1627 */ -1,
/* 1628 */ 50059,
/* 1629 */ -1413,
/* 1630 */ -1,
/* 1631 */ 1638,
/* 1632 */ 50451,
/* 1633 */ -1,
/* 1634 */ -1,
/* 1635 */ 50059,
/* 1636 */ -1413,
/* 1637 */ -1,
/* 1638 */ 1645,
/* 1639 */ 50310,
/* 1640 */ -1,
/* 1641 */ -1,
/* 1642 */ 50059,
/* 1643 */ -1413,
/* 1644 */ -1,
/* 1645 */ 1653,
/* 1646 */ 50372,
/* 1647 */ 50415,
/* 1648 */ -1,
/* 1649 */ -1,
/* 1650 */ 50059,
/* 1651 */ -1413,
/* 1652 */ -1,
/* 1653 */ 1660,
/* 1654 */ 50371,
/* 1655 */ -1,
/* 1656 */ -1,
/* 1657 */ 50059,
/* 1658 */ -1413,
/* 1659 */ -1,
/* 1660 */ 1667,
/* 1661 */ 50351,
/* 1662 */ -1,
/* 1663 */ -1,
/* 1664 */ 50059,
/* 1665 */ -1413,
/* 1666 */ -1,
/* 1667 */ 1671,
/* 1668 */ -1,
/* 1669 */ -1413,
/* 1670 */ -1,
/* 1671 */ 1675,
/* 1672 */ -1,
/* 1673 */ -1413,
/* 1674 */ -1,
/* 1675 */ 0,
/* 1676 */ -1,
/* 1677 */ -1413,
/* 1678 */ -1,
/* 1679 */ 1683,
/* 1680 */ 50058,
/* 1681 */ -1679,
/* 1682 */ -1,
/* 1683 */ 0,
/* 1684 */ -1679,
/* 1685 */ -1,
/* 1686 */ 1690,
/* 1687 */ 50058,
/* 1688 */ -1686,
/* 1689 */ -1,
/* 1690 */ 0,
/* 1691 */ -1686,
/* 1692 */ -1,
/* 1693 */ 1697,
/* 1694 */ 50058,
/* 1695 */ -1693,
/* 1696 */ -1,
/* 1697 */ 0,
/* 1698 */ -1693,
/* 1699 */ -1,
/* 1700 */ 1704,
/* 1701 */ 50058,
/* 1702 */ -1700,
/* 1703 */ -1,
/* 1704 */ 0,
/* 1705 */ -1700,
/* 1706 */ -1,
/* 1707 */ 1711,
/* 1708 */ 50058,
/* 1709 */ -1707,
/* 1710 */ -1,
/* 1711 */ 0,
/* 1712 */ -1707,
/* 1713 */ -1,
/* 1714 */ 1718,
/* 1715 */ 50058,
/* 1716 */ -1714,
/* 1717 */ -1,
/* 1718 */ 0,
/* 1719 */ -1714,
/* 1720 */ -1,
/* 1721 */ 1725,
/* 1722 */ 50058,
/* 1723 */ -1721,
/* 1724 */ -1,
/* 1725 */ 0,
/* 1726 */ -1721,
/* 1727 */ -1,
/* 1728 */ 1732,
/* 1729 */ 50058,
/* 1730 */ -1728,
/* 1731 */ -1,
/* 1732 */ 0,
/* 1733 */ -1728,
/* 1734 */ -1,
/* 1735 */ 1739,
/* 1736 */ 50058,
/* 1737 */ -1735,
/* 1738 */ -1,
/* 1739 */ 0,
/* 1740 */ -1735,
/* 1741 */ -1,
/* 1742 */ 1746,
/* 1743 */ 50058,
/* 1744 */ -1742,
/* 1745 */ -1,
/* 1746 */ 0,
/* 1747 */ -1742,
/* 1748 */ -1,
/* 1749 */ 1753,
/* 1750 */ 50058,
/* 1751 */ -1749,
/* 1752 */ -1,
/* 1753 */ 0,
/* 1754 */ -1749,
/* 1755 */ -1,
/* 1756 */ 1760,
/* 1757 */ 50058,
/* 1758 */ -1756,
/* 1759 */ -1,
/* 1760 */ 0,
/* 1761 */ -1756,
/* 1762 */ -1,
/* 1763 */ 1767,
/* 1764 */ 50058,
/* 1765 */ -1763,
/* 1766 */ -1,
/* 1767 */ 0,
/* 1768 */ -1763,
/* 1769 */ -1,
/* 1770 */ 1774,
/* 1771 */ 50058,
/* 1772 */ -1770,
/* 1773 */ -1,
/* 1774 */ 0,
/* 1775 */ -1770,
/* 1776 */ -1,
/* 1777 */ 1781,
/* 1778 */ -1,
/* 1779 */ -1777,
/* 1780 */ -1,
/* 1781 */ 0,
/* 1782 */ -1777,
/* 1783 */ -1,
/* 1784 */ 1788,
/* 1785 */ 50058,
/* 1786 */ -1784,
/* 1787 */ -1,
/* 1788 */ 0,
/* 1789 */ -1784,
/* 1790 */ -1,
/* 1791 */ 1795,
/* 1792 */ 50058,
/* 1793 */ -1791,
/* 1794 */ -1,
/* 1795 */ 0,
/* 1796 */ -1791,
/* 1797 */ -1,
/* 1798 */ 1802,
/* 1799 */ 50058,
/* 1800 */ -1798,
/* 1801 */ -1,
/* 1802 */ 0,
/* 1803 */ -1798,
/* 1804 */ -1,
/* 1805 */ 1809,
/* 1806 */ 50058,
/* 1807 */ -1805,
/* 1808 */ -1,
/* 1809 */ 0,
/* 1810 */ -1805,
/* 1811 */ -1,
/* 1812 */ 1816,
/* 1813 */ 50058,
/* 1814 */ -1812,
/* 1815 */ -1,
/* 1816 */ 0,
/* 1817 */ -1812,
/* 1818 */ -1,
/* 1819 */ 1823,
/* 1820 */ 50058,
/* 1821 */ -1819,
/* 1822 */ -1,
/* 1823 */ 0,
/* 1824 */ -1819,
/* 1825 */ -1,
/* 1826 */ 1830,
/* 1827 */ 50058,
/* 1828 */ -1826,
/* 1829 */ -1,
/* 1830 */ 0,
/* 1831 */ -1826,
/* 1832 */ -1,
/* 1833 */ 1838,
/* 1834 */ -1,
/* 1835 */ -1,
/* 1836 */ -1833,
/* 1837 */ -1,
/* 1838 */ 0,
/* 1839 */ -1833,
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
/* 1855 */ 1861,
/* 1856 */ -1,
/* 1857 */ -1,
/* 1858 */ -1,
/* 1859 */ -1855,
/* 1860 */ -1,
/* 1861 */ 0,
/* 1862 */ -1855,
/* 1863 */ -1,
/* 1864 */ 1868,
/* 1865 */ 50044,
/* 1866 */ -1864,
/* 1867 */ -1,
/* 1868 */ 0,
/* 1869 */ -1864,
/* 1870 */ -1,
/* 1871 */ 1875,
/* 1872 */ 50058,
/* 1873 */ -1871,
/* 1874 */ -1,
/* 1875 */ 0,
/* 1876 */ -1871,
/* 1877 */ -1,
/* 1878 */ 1884,
/* 1879 */ -1,
/* 1880 */ -1,
/* 1881 */ -1,
/* 1882 */ -1878,
/* 1883 */ -1,
/* 1884 */ 0,
/* 1885 */ -1878,
/* 1886 */ -1,
/* 1887 */ 1891,
/* 1888 */ 50044,
/* 1889 */ -1887,
/* 1890 */ -1,
/* 1891 */ 0,
/* 1892 */ -1887,
/* 1893 */ -1,
/* 1894 */ 1898,
/* 1895 */ 50058,
/* 1896 */ -1894,
/* 1897 */ -1,
/* 1898 */ 0,
/* 1899 */ -1894,
/* 1900 */ -1,
/* 1901 */ 1906,
/* 1902 */ -1,
/* 1903 */ -1,
/* 1904 */ -1901,
/* 1905 */ -1,
/* 1906 */ 0,
/* 1907 */ -1901,
/* 1908 */ -1,
/* 1909 */ 1913,
/* 1910 */ 50058,
/* 1911 */ -1909,
/* 1912 */ -1,
/* 1913 */ 0,
/* 1914 */ -1909,
/* 1915 */ -1,
/* 1916 */ 1922,
/* 1917 */ -1,
/* 1918 */ -1,
/* 1919 */ -1,
/* 1920 */ -1916,
/* 1921 */ -1,
/* 1922 */ 0,
/* 1923 */ -1916,
/* 1924 */ -1,
/* 1925 */ 1929,
/* 1926 */ 50044,
/* 1927 */ -1925,
/* 1928 */ -1,
/* 1929 */ 1933,
/* 1930 */ 50124,
/* 1931 */ -1925,
/* 1932 */ -1,
/* 1933 */ 0,
/* 1934 */ -1925,
/* 1935 */ -1,
/* 1936 */ 1940,
/* 1937 */ 50058,
/* 1938 */ -1936,
/* 1939 */ -1,
/* 1940 */ 0,
/* 1941 */ -1936,
/* 1942 */ -1,
/* 1943 */ 1947,
/* 1944 */ 50058,
/* 1945 */ -1943,
/* 1946 */ -1,
/* 1947 */ 0,
/* 1948 */ -1943,
/* 1949 */ -1,
/* 1950 */ 1954,
/* 1951 */ 50058,
/* 1952 */ -1950,
/* 1953 */ -1,
/* 1954 */ 0,
/* 1955 */ -1950,
/* 1956 */ -1,
/* 1957 */ 1961,
/* 1958 */ 50058,
/* 1959 */ -1957,
/* 1960 */ -1,
/* 1961 */ 0,
/* 1962 */ -1957,
/* 1963 */ -1,
/* 1964 */ 1968,
/* 1965 */ 50058,
/* 1966 */ -1964,
/* 1967 */ -1,
/* 1968 */ 0,
/* 1969 */ -1964,
/* 1970 */ -1,
/* 1971 */ 1975,
/* 1972 */ 50058,
/* 1973 */ -1971,
/* 1974 */ -1,
/* 1975 */ 0,
/* 1976 */ -1971,
/* 1977 */ -1,
/* 1978 */ 1982,
/* 1979 */ 50058,
/* 1980 */ -1978,
/* 1981 */ -1,
/* 1982 */ 0,
/* 1983 */ -1978,
/* 1984 */ -1,
/* 1985 */ 1989,
/* 1986 */ 50058,
/* 1987 */ -1985,
/* 1988 */ -1,
/* 1989 */ 0,
/* 1990 */ -1985,
/* 1991 */ -1,
/* 1992 */ 1996,
/* 1993 */ 50058,
/* 1994 */ -1992,
/* 1995 */ -1,
/* 1996 */ 0,
/* 1997 */ -1992,
/* 1998 */ -1,
/* 1999 */ 2004,
/* 2000 */ -1,
/* 2001 */ -1,
/* 2002 */ -1999,
/* 2003 */ -1,
/* 2004 */ 0,
/* 2005 */ 50275,
/* 2006 */ -1,
/* 2007 */ -1,
/* 2008 */ -1999,
/* 2009 */ -1,
/* 2010 */ 2014,
/* 2011 */ 50044,
/* 2012 */ -2010,
/* 2013 */ -1,
/* 2014 */ 0,
/* 2015 */ -2010,
/* 2016 */ -1,
/* 2017 */ 2021,
/* 2018 */ 50044,
/* 2019 */ -2017,
/* 2020 */ -1,
/* 2021 */ 0,
/* 2022 */ -2017,
/* 2023 */ -1,
/* 2024 */ 2029,
/* 2025 */ -1,
/* 2026 */ -1,
/* 2027 */ -2024,
/* 2028 */ -1,
/* 2029 */ 0,
/* 2030 */ 50126,
/* 2031 */ -1,
/* 2032 */ -1,
/* 2033 */ -2024,
/* 2034 */ -1,
/* 2035 */ 2039,
/* 2036 */ 50044,
/* 2037 */ -2035,
/* 2038 */ -1,
/* 2039 */ 0,
/* 2040 */ -2035,
/* 2041 */ -1,
/* 2042 */ 2046,
/* 2043 */ 50044,
/* 2044 */ -2042,
/* 2045 */ -1,
/* 2046 */ 0,
/* 2047 */ -2042,
/* 2048 */ -1,
/* 2049 */ 0,
/* 2050 */ 50344,
/* 2051 */ 50257,
/* 2052 */ 50058,
/* 2053 */ -1,
/* 2054 */ 50123,
/* 2055 */ -1,
/* 2056 */ 50125,
/* 2057 */ -2049,
/* 2058 */ -1,
/* 2059 */ 2064,
/* 2060 */ -1,
/* 2061 */ -1,
/* 2062 */ -2059,
/* 2063 */ -1,
/* 2064 */ 0,
/* 2065 */ -2059,
/* 2066 */ -1,
/* 2067 */ 2074,
/* 2068 */ 50323,
/* 2069 */ -1,
/* 2070 */ 50257,
/* 2071 */ 50059,
/* 2072 */ -2067,
/* 2073 */ -1,
/* 2074 */ 2081,
/* 2075 */ 50379,
/* 2076 */ -1,
/* 2077 */ -1,
/* 2078 */ 50059,
/* 2079 */ -2067,
/* 2080 */ -1,
/* 2081 */ 2085,
/* 2082 */ -1,
/* 2083 */ -2067,
/* 2084 */ -1,
/* 2085 */ 2093,
/* 2086 */ 50385,
/* 2087 */ -1,
/* 2088 */ -1,
/* 2089 */ 50037,
/* 2090 */ 50059,
/* 2091 */ -2067,
/* 2092 */ -1,
/* 2093 */ 2100,
/* 2094 */ 50361,
/* 2095 */ -1,
/* 2096 */ -1,
/* 2097 */ 50059,
/* 2098 */ -2067,
/* 2099 */ -1,
/* 2100 */ 2107,
/* 2101 */ 50346,
/* 2102 */ -1,
/* 2103 */ -1,
/* 2104 */ 50059,
/* 2105 */ -2067,
/* 2106 */ -1,
/* 2107 */ 2114,
/* 2108 */ -1,
/* 2109 */ -1,
/* 2110 */ -1,
/* 2111 */ 50059,
/* 2112 */ -2067,
/* 2113 */ -1,
/* 2114 */ 2121,
/* 2115 */ 50467,
/* 2116 */ -1,
/* 2117 */ -1,
/* 2118 */ 50059,
/* 2119 */ -2067,
/* 2120 */ -1,
/* 2121 */ 2128,
/* 2122 */ 50340,
/* 2123 */ -1,
/* 2124 */ -1,
/* 2125 */ 50059,
/* 2126 */ -2067,
/* 2127 */ -1,
/* 2128 */ 2132,
/* 2129 */ -1,
/* 2130 */ -2067,
/* 2131 */ -1,
/* 2132 */ 2136,
/* 2133 */ -1,
/* 2134 */ -2067,
/* 2135 */ -1,
/* 2136 */ 0,
/* 2137 */ -1,
/* 2138 */ -2067,
/* 2139 */ -1,
/* 2140 */ 2144,
/* 2141 */ 50058,
/* 2142 */ -2140,
/* 2143 */ -1,
/* 2144 */ 0,
/* 2145 */ -2140,
/* 2146 */ -1,
/* 2147 */ 2151,
/* 2148 */ 50058,
/* 2149 */ -2147,
/* 2150 */ -1,
/* 2151 */ 0,
/* 2152 */ -2147,
/* 2153 */ -1,
/* 2154 */ 2158,
/* 2155 */ 50058,
/* 2156 */ -2154,
/* 2157 */ -1,
/* 2158 */ 0,
/* 2159 */ -2154,
/* 2160 */ -1,
/* 2161 */ 2165,
/* 2162 */ 50058,
/* 2163 */ -2161,
/* 2164 */ -1,
/* 2165 */ 0,
/* 2166 */ -2161,
/* 2167 */ -1,
/* 2168 */ 2172,
/* 2169 */ 50058,
/* 2170 */ -2168,
/* 2171 */ -1,
/* 2172 */ 0,
/* 2173 */ -2168,
/* 2174 */ -1,
/* 2175 */ 2181,
/* 2176 */ -1,
/* 2177 */ -1,
/* 2178 */ -1,
/* 2179 */ -2175,
/* 2180 */ -1,
/* 2181 */ 0,
/* 2182 */ -2175,
/* 2183 */ -1,
/* 2184 */ 2188,
/* 2185 */ 50124,
/* 2186 */ -2184,
/* 2187 */ -1,
/* 2188 */ 2192,
/* 2189 */ 50044,
/* 2190 */ -2184,
/* 2191 */ -1,
/* 2192 */ 0,
/* 2193 */ -2184,
/* 2194 */ -1,
/* 2195 */ 2199,
/* 2196 */ 50443,
/* 2197 */ -2195,
/* 2198 */ -1,
/* 2199 */ 0,
/* 2200 */ 50331,
/* 2201 */ -2195,
/* 2202 */ -1,
/* 2203 */ 2207,
/* 2204 */ 50058,
/* 2205 */ -2203,
/* 2206 */ -1,
/* 2207 */ 0,
/* 2208 */ -2203,
/* 2209 */ -1,
/* 2210 */ 2214,
/* 2211 */ 50058,
/* 2212 */ -2210,
/* 2213 */ -1,
/* 2214 */ 0,
/* 2215 */ -2210,
/* 2216 */ -1,
/* 2217 */ 2221,
/* 2218 */ 50058,
/* 2219 */ -2217,
/* 2220 */ -1,
/* 2221 */ 0,
/* 2222 */ -2217,
/* 2223 */ -1,
/* 2224 */ 0,
/* 2225 */ -1,
/* 2226 */ -1,
/* 2227 */ 50257,
/* 2228 */ 50058,
/* 2229 */ -1,
/* 2230 */ 50123,
/* 2231 */ -1,
/* 2232 */ 50125,
/* 2233 */ -1,
/* 2234 */ -2224,
/* 2235 */ -1,
/* 2236 */ 2242,
/* 2237 */ -1,
/* 2238 */ -1,
/* 2239 */ -1,
/* 2240 */ -2236,
/* 2241 */ -1,
/* 2242 */ 0,
/* 2243 */ -2236,
/* 2244 */ -1,
/* 2245 */ 2249,
/* 2246 */ 50047,
/* 2247 */ -2245,
/* 2248 */ -1,
/* 2249 */ 0,
/* 2250 */ -2245,
/* 2251 */ -1,
/* 2252 */ 2256,
/* 2253 */ 50431,
/* 2254 */ -2252,
/* 2255 */ -1,
/* 2256 */ 2260,
/* 2257 */ 50331,
/* 2258 */ -2252,
/* 2259 */ -1,
/* 2260 */ 2264,
/* 2261 */ 50404,
/* 2262 */ -2252,
/* 2263 */ -1,
/* 2264 */ 0,
/* 2265 */ 50327,
/* 2266 */ -2252,
/* 2267 */ -1,
/* 2268 */ 2273,
/* 2269 */ -1,
/* 2270 */ -1,
/* 2271 */ -2268,
/* 2272 */ -1,
/* 2273 */ 0,
/* 2274 */ -2268,
/* 2275 */ -1,
/* 2276 */ 2285,
/* 2277 */ 50287,
/* 2278 */ -1,
/* 2279 */ 50123,
/* 2280 */ -1,
/* 2281 */ 50125,
/* 2282 */ -1,
/* 2283 */ -2276,
/* 2284 */ -1,
/* 2285 */ 0,
/* 2286 */ -2276,
/* 2287 */ -1,
/* 2288 */ 2293,
/* 2289 */ -1,
/* 2290 */ -1,
/* 2291 */ -2288,
/* 2292 */ -1,
/* 2293 */ 0,
/* 2294 */ -2288,
/* 2295 */ -1,
/* 2296 */ 2303,
/* 2297 */ 50323,
/* 2298 */ -1,
/* 2299 */ 50257,
/* 2300 */ 50059,
/* 2301 */ -2296,
/* 2302 */ -1,
/* 2303 */ 2308,
/* 2304 */ -1,
/* 2305 */ 50059,
/* 2306 */ -2296,
/* 2307 */ -1,
/* 2308 */ 2315,
/* 2309 */ 50370,
/* 2310 */ -1,
/* 2311 */ -1,
/* 2312 */ 50059,
/* 2313 */ -2296,
/* 2314 */ -1,
/* 2315 */ 2322,
/* 2316 */ 50312,
/* 2317 */ -1,
/* 2318 */ -1,
/* 2319 */ 50059,
/* 2320 */ -2296,
/* 2321 */ -1,
/* 2322 */ 2331,
/* 2323 */ 50296,
/* 2324 */ -1,
/* 2325 */ -1,
/* 2326 */ -1,
/* 2327 */ -1,
/* 2328 */ 50059,
/* 2329 */ -2296,
/* 2330 */ -1,
/* 2331 */ 2339,
/* 2332 */ 50408,
/* 2333 */ -1,
/* 2334 */ -1,
/* 2335 */ -1,
/* 2336 */ 50059,
/* 2337 */ -2296,
/* 2338 */ -1,
/* 2339 */ 2347,
/* 2340 */ 50440,
/* 2341 */ -1,
/* 2342 */ -1,
/* 2343 */ -1,
/* 2344 */ 50059,
/* 2345 */ -2296,
/* 2346 */ -1,
/* 2347 */ 2354,
/* 2348 */ 50412,
/* 2349 */ -1,
/* 2350 */ -1,
/* 2351 */ 50059,
/* 2352 */ -2296,
/* 2353 */ -1,
/* 2354 */ 2361,
/* 2355 */ 50467,
/* 2356 */ -1,
/* 2357 */ -1,
/* 2358 */ 50059,
/* 2359 */ -2296,
/* 2360 */ -1,
/* 2361 */ 2368,
/* 2362 */ 50468,
/* 2363 */ -1,
/* 2364 */ -1,
/* 2365 */ 50059,
/* 2366 */ -2296,
/* 2367 */ -1,
/* 2368 */ 2375,
/* 2369 */ 50330,
/* 2370 */ -1,
/* 2371 */ -1,
/* 2372 */ 50059,
/* 2373 */ -2296,
/* 2374 */ -1,
/* 2375 */ 2382,
/* 2376 */ 50409,
/* 2377 */ -1,
/* 2378 */ -1,
/* 2379 */ 50059,
/* 2380 */ -2296,
/* 2381 */ -1,
/* 2382 */ 2389,
/* 2383 */ 50295,
/* 2384 */ -1,
/* 2385 */ -1,
/* 2386 */ 50059,
/* 2387 */ -2296,
/* 2388 */ -1,
/* 2389 */ 2396,
/* 2390 */ 50374,
/* 2391 */ -1,
/* 2392 */ -1,
/* 2393 */ 50059,
/* 2394 */ -2296,
/* 2395 */ -1,
/* 2396 */ 2403,
/* 2397 */ 50456,
/* 2398 */ -1,
/* 2399 */ -1,
/* 2400 */ 50059,
/* 2401 */ -2296,
/* 2402 */ -1,
/* 2403 */ 2410,
/* 2404 */ 50423,
/* 2405 */ -1,
/* 2406 */ -1,
/* 2407 */ 50059,
/* 2408 */ -2296,
/* 2409 */ -1,
/* 2410 */ 2417,
/* 2411 */ 50423,
/* 2412 */ -1,
/* 2413 */ -1,
/* 2414 */ 50059,
/* 2415 */ -2296,
/* 2416 */ -1,
/* 2417 */ 2424,
/* 2418 */ 50315,
/* 2419 */ -1,
/* 2420 */ -1,
/* 2421 */ 50059,
/* 2422 */ -2296,
/* 2423 */ -1,
/* 2424 */ 2431,
/* 2425 */ 50315,
/* 2426 */ -1,
/* 2427 */ 50274,
/* 2428 */ 50059,
/* 2429 */ -2296,
/* 2430 */ -1,
/* 2431 */ 2438,
/* 2432 */ 50346,
/* 2433 */ -1,
/* 2434 */ -1,
/* 2435 */ 50059,
/* 2436 */ -2296,
/* 2437 */ -1,
/* 2438 */ 2442,
/* 2439 */ -1,
/* 2440 */ -2296,
/* 2441 */ -1,
/* 2442 */ 2446,
/* 2443 */ -1,
/* 2444 */ -2296,
/* 2445 */ -1,
/* 2446 */ 0,
/* 2447 */ -1,
/* 2448 */ -2296,
/* 2449 */ -1,
/* 2450 */ 2454,
/* 2451 */ 50058,
/* 2452 */ -2450,
/* 2453 */ -1,
/* 2454 */ 0,
/* 2455 */ -2450,
/* 2456 */ -1,
/* 2457 */ 2461,
/* 2458 */ 50058,
/* 2459 */ -2457,
/* 2460 */ -1,
/* 2461 */ 0,
/* 2462 */ -2457,
/* 2463 */ -1,
/* 2464 */ 2468,
/* 2465 */ 50058,
/* 2466 */ -2464,
/* 2467 */ -1,
/* 2468 */ 0,
/* 2469 */ -2464,
/* 2470 */ -1,
/* 2471 */ 2475,
/* 2472 */ 50305,
/* 2473 */ -2471,
/* 2474 */ -1,
/* 2475 */ 0,
/* 2476 */ -2471,
/* 2477 */ -1,
/* 2478 */ 2482,
/* 2479 */ 50058,
/* 2480 */ -2478,
/* 2481 */ -1,
/* 2482 */ 0,
/* 2483 */ -2478,
/* 2484 */ -1,
/* 2485 */ 2489,
/* 2486 */ 50037,
/* 2487 */ -2485,
/* 2488 */ -1,
/* 2489 */ 0,
/* 2490 */ -2485,
/* 2491 */ -1,
/* 2492 */ 2496,
/* 2493 */ 50058,
/* 2494 */ -2492,
/* 2495 */ -1,
/* 2496 */ 0,
/* 2497 */ -2492,
/* 2498 */ -1,
/* 2499 */ 2505,
/* 2500 */ 50040,
/* 2501 */ -1,
/* 2502 */ 50041,
/* 2503 */ -2499,
/* 2504 */ -1,
/* 2505 */ 0,
/* 2506 */ -2499,
/* 2507 */ -1,
/* 2508 */ 2512,
/* 2509 */ 50058,
/* 2510 */ -2508,
/* 2511 */ -1,
/* 2512 */ 0,
/* 2513 */ -2508,
/* 2514 */ -1,
/* 2515 */ 2519,
/* 2516 */ 50401,
/* 2517 */ -2515,
/* 2518 */ -1,
/* 2519 */ 0,
/* 2520 */ -2515,
/* 2521 */ -1,
/* 2522 */ 2526,
/* 2523 */ 50058,
/* 2524 */ -2522,
/* 2525 */ -1,
/* 2526 */ 0,
/* 2527 */ -2522,
/* 2528 */ -1,
/* 2529 */ 2533,
/* 2530 */ 50058,
/* 2531 */ -2529,
/* 2532 */ -1,
/* 2533 */ 0,
/* 2534 */ -2529,
/* 2535 */ -1,
/* 2536 */ 2540,
/* 2537 */ 50058,
/* 2538 */ -2536,
/* 2539 */ -1,
/* 2540 */ 0,
/* 2541 */ -2536,
/* 2542 */ -1,
/* 2543 */ 2547,
/* 2544 */ 50058,
/* 2545 */ -2543,
/* 2546 */ -1,
/* 2547 */ 0,
/* 2548 */ -2543,
/* 2549 */ -1,
/* 2550 */ 2554,
/* 2551 */ 50058,
/* 2552 */ -2550,
/* 2553 */ -1,
/* 2554 */ 0,
/* 2555 */ -2550,
/* 2556 */ -1,
/* 2557 */ 2561,
/* 2558 */ 50058,
/* 2559 */ -2557,
/* 2560 */ -1,
/* 2561 */ 0,
/* 2562 */ -2557,
/* 2563 */ -1,
/* 2564 */ 2568,
/* 2565 */ 50058,
/* 2566 */ -2564,
/* 2567 */ -1,
/* 2568 */ 0,
/* 2569 */ -2564,
/* 2570 */ -1,
/* 2571 */ 2575,
/* 2572 */ 50058,
/* 2573 */ -2571,
/* 2574 */ -1,
/* 2575 */ 0,
/* 2576 */ -2571,
/* 2577 */ -1,
/* 2578 */ 2582,
/* 2579 */ 50058,
/* 2580 */ -2578,
/* 2581 */ -1,
/* 2582 */ 0,
/* 2583 */ -2578,
/* 2584 */ -1,
/* 2585 */ 2589,
/* 2586 */ 50058,
/* 2587 */ -2585,
/* 2588 */ -1,
/* 2589 */ 0,
/* 2590 */ -2585,
/* 2591 */ -1,
/* 2592 */ 2596,
/* 2593 */ 50058,
/* 2594 */ -2592,
/* 2595 */ -1,
/* 2596 */ 0,
/* 2597 */ -2592,
/* 2598 */ -1,
/* 2599 */ 2603,
/* 2600 */ 50058,
/* 2601 */ -2599,
/* 2602 */ -1,
/* 2603 */ 0,
/* 2604 */ -2599,
/* 2605 */ -1,
/* 2606 */ 2610,
/* 2607 */ 50058,
/* 2608 */ -2606,
/* 2609 */ -1,
/* 2610 */ 0,
/* 2611 */ -2606,
/* 2612 */ -1,
/* 2613 */ 2619,
/* 2614 */ -1,
/* 2615 */ -1,
/* 2616 */ -1,
/* 2617 */ -2613,
/* 2618 */ -1,
/* 2619 */ 0,
/* 2620 */ -2613,
/* 2621 */ -1,
/* 2622 */ 2626,
/* 2623 */ 50044,
/* 2624 */ -2622,
/* 2625 */ -1,
/* 2626 */ 2630,
/* 2627 */ 50124,
/* 2628 */ -2622,
/* 2629 */ -1,
/* 2630 */ 0,
/* 2631 */ -2622,
/* 2632 */ -1,
/* 2633 */ 0,
/* 2634 */ -1,
/* 2635 */ -2633,
/* 2636 */ -1,
/* 2637 */ 2643,
/* 2638 */ -1,
/* 2639 */ -1,
/* 2640 */ -1,
/* 2641 */ -2637,
/* 2642 */ -1,
/* 2643 */ 0,
/* 2644 */ -2637,
/* 2645 */ -1,
/* 2646 */ 2650,
/* 2647 */ 50044,
/* 2648 */ -2646,
/* 2649 */ -1,
/* 2650 */ 2654,
/* 2651 */ 50124,
/* 2652 */ -2646,
/* 2653 */ -1,
/* 2654 */ 0,
/* 2655 */ -2646,
/* 2656 */ -1,
/* 2657 */ 0,
/* 2658 */ 50261,
/* 2659 */ -2657,
/* 2660 */ -1,
/* 2661 */ 0,
/* 2662 */ 50413,
/* 2663 */ 50257,
/* 2664 */ -1,
/* 2665 */ 50123,
/* 2666 */ -1,
/* 2667 */ 50125,
/* 2668 */ -2661,
/* 2669 */ -1,
/* 2670 */ 2675,
/* 2671 */ 50058,
/* 2672 */ -1,
/* 2673 */ -2670,
/* 2674 */ -1,
/* 2675 */ 0,
/* 2676 */ -2670,
/* 2677 */ -1,
/* 2678 */ 2683,
/* 2679 */ -1,
/* 2680 */ -1,
/* 2681 */ -2678,
/* 2682 */ -1,
/* 2683 */ 0,
/* 2684 */ -2678,
/* 2685 */ -1,
/* 2686 */ 2693,
/* 2687 */ 50323,
/* 2688 */ -1,
/* 2689 */ 50257,
/* 2690 */ 50059,
/* 2691 */ -2686,
/* 2692 */ -1,
/* 2693 */ 2698,
/* 2694 */ -1,
/* 2695 */ 50059,
/* 2696 */ -2686,
/* 2697 */ -1,
/* 2698 */ 2705,
/* 2699 */ 50355,
/* 2700 */ -1,
/* 2701 */ -1,
/* 2702 */ 50059,
/* 2703 */ -2686,
/* 2704 */ -1,
/* 2705 */ 2712,
/* 2706 */ 50384,
/* 2707 */ -1,
/* 2708 */ -1,
/* 2709 */ 50059,
/* 2710 */ -2686,
/* 2711 */ -1,
/* 2712 */ 2722,
/* 2713 */ 50336,
/* 2714 */ -1,
/* 2715 */ -1,
/* 2716 */ -1,
/* 2717 */ 50044,
/* 2718 */ -1,
/* 2719 */ 50059,
/* 2720 */ -2686,
/* 2721 */ -1,
/* 2722 */ 2734,
/* 2723 */ 50336,
/* 2724 */ -1,
/* 2725 */ -1,
/* 2726 */ -1,
/* 2727 */ 50044,
/* 2728 */ -1,
/* 2729 */ 50044,
/* 2730 */ -1,
/* 2731 */ 50059,
/* 2732 */ -2686,
/* 2733 */ -1,
/* 2734 */ 2741,
/* 2735 */ 50427,
/* 2736 */ -1,
/* 2737 */ -1,
/* 2738 */ 50059,
/* 2739 */ -2686,
/* 2740 */ -1,
/* 2741 */ 2748,
/* 2742 */ -1,
/* 2743 */ -1,
/* 2744 */ 50257,
/* 2745 */ 50059,
/* 2746 */ -2686,
/* 2747 */ -1,
/* 2748 */ 2755,
/* 2749 */ 50346,
/* 2750 */ -1,
/* 2751 */ -1,
/* 2752 */ 50059,
/* 2753 */ -2686,
/* 2754 */ -1,
/* 2755 */ 2759,
/* 2756 */ -1,
/* 2757 */ -2686,
/* 2758 */ -1,
/* 2759 */ 2763,
/* 2760 */ -1,
/* 2761 */ -2686,
/* 2762 */ -1,
/* 2763 */ 2767,
/* 2764 */ -1,
/* 2765 */ -2686,
/* 2766 */ -1,
/* 2767 */ 0,
/* 2768 */ -1,
/* 2769 */ -2686,
/* 2770 */ -1,
/* 2771 */ 2775,
/* 2772 */ 50058,
/* 2773 */ -2771,
/* 2774 */ -1,
/* 2775 */ 0,
/* 2776 */ -2771,
/* 2777 */ -1,
/* 2778 */ 2786,
/* 2779 */ 50276,
/* 2780 */ -1,
/* 2781 */ -1,
/* 2782 */ -1,
/* 2783 */ -1,
/* 2784 */ -2778,
/* 2785 */ -1,
/* 2786 */ 0,
/* 2787 */ -2778,
/* 2788 */ -1,
/* 2789 */ 2793,
/* 2790 */ 50058,
/* 2791 */ -2789,
/* 2792 */ -1,
/* 2793 */ 0,
/* 2794 */ -2789,
/* 2795 */ -1,
/* 2796 */ 2800,
/* 2797 */ 50044,
/* 2798 */ -2796,
/* 2799 */ -1,
/* 2800 */ 0,
/* 2801 */ -2796,
/* 2802 */ -1,
/* 2803 */ 2807,
/* 2804 */ 50058,
/* 2805 */ -2803,
/* 2806 */ -1,
/* 2807 */ 0,
/* 2808 */ -2803,
/* 2809 */ -1,
/* 2810 */ 2816,
/* 2811 */ -1,
/* 2812 */ -1,
/* 2813 */ -1,
/* 2814 */ -2810,
/* 2815 */ -1,
/* 2816 */ 0,
/* 2817 */ -2810,
/* 2818 */ -1,
/* 2819 */ 2823,
/* 2820 */ 50044,
/* 2821 */ -2819,
/* 2822 */ -1,
/* 2823 */ 0,
/* 2824 */ -2819,
/* 2825 */ -1,
/* 2826 */ 2830,
/* 2827 */ 50058,
/* 2828 */ -2826,
/* 2829 */ -1,
/* 2830 */ 0,
/* 2831 */ -2826,
/* 2832 */ -1,
/* 2833 */ 2837,
/* 2834 */ 50306,
/* 2835 */ -2833,
/* 2836 */ -1,
/* 2837 */ 0,
/* 2838 */ -2833,
/* 2839 */ -1,
/* 2840 */ 2844,
/* 2841 */ 50058,
/* 2842 */ -2840,
/* 2843 */ -1,
/* 2844 */ 0,
/* 2845 */ -2840,
/* 2846 */ -1,
/* 2847 */ 2851,
/* 2848 */ 50306,
/* 2849 */ -2847,
/* 2850 */ -1,
/* 2851 */ 0,
/* 2852 */ -2847,
/* 2853 */ -1,
/* 2854 */ 2858,
/* 2855 */ 50058,
/* 2856 */ -2854,
/* 2857 */ -1,
/* 2858 */ 0,
/* 2859 */ -2854,
/* 2860 */ -1,
/* 2861 */ 2865,
/* 2862 */ 50058,
/* 2863 */ -2861,
/* 2864 */ -1,
/* 2865 */ 0,
/* 2866 */ -2861,
/* 2867 */ -1,
/* 2868 */ 2874,
/* 2869 */ -1,
/* 2870 */ -1,
/* 2871 */ -1,
/* 2872 */ -2868,
/* 2873 */ -1,
/* 2874 */ 0,
/* 2875 */ -2868,
/* 2876 */ -1,
/* 2877 */ 2881,
/* 2878 */ 50044,
/* 2879 */ -2877,
/* 2880 */ -1,
/* 2881 */ 0,
/* 2882 */ -2877,
/* 2883 */ -1,
/* 2884 */ 2888,
/* 2885 */ 50058,
/* 2886 */ -2884,
/* 2887 */ -1,
/* 2888 */ 0,
/* 2889 */ -2884,
/* 2890 */ -1,
/* 2891 */ 2895,
/* 2892 */ 50058,
/* 2893 */ -2891,
/* 2894 */ -1,
/* 2895 */ 0,
/* 2896 */ -2891,
/* 2897 */ -1,
/* 2898 */ 2904,
/* 2899 */ -1,
/* 2900 */ -1,
/* 2901 */ -1,
/* 2902 */ -2898,
/* 2903 */ -1,
/* 2904 */ 0,
/* 2905 */ -2898,
/* 2906 */ -1,
/* 2907 */ 2911,
/* 2908 */ 50124,
/* 2909 */ -2907,
/* 2910 */ -1,
/* 2911 */ 2915,
/* 2912 */ 50044,
/* 2913 */ -2907,
/* 2914 */ -1,
/* 2915 */ 0,
/* 2916 */ -2907,
/* 2917 */ -1,
/* 2918 */ 0,
/* 2919 */ 50306,
/* 2920 */ 50257,
/* 2921 */ 50123,
/* 2922 */ -1,
/* 2923 */ 50125,
/* 2924 */ -2918,
/* 2925 */ -1,
/* 2926 */ 2931,
/* 2927 */ -1,
/* 2928 */ -1,
/* 2929 */ -2926,
/* 2930 */ -1,
/* 2931 */ 0,
/* 2932 */ -2926,
/* 2933 */ -1,
/* 2934 */ 2941,
/* 2935 */ 50323,
/* 2936 */ -1,
/* 2937 */ 50257,
/* 2938 */ 50059,
/* 2939 */ -2934,
/* 2940 */ -1,
/* 2941 */ 2949,
/* 2942 */ 50360,
/* 2943 */ -1,
/* 2944 */ -1,
/* 2945 */ -1,
/* 2946 */ 50059,
/* 2947 */ -2934,
/* 2948 */ -1,
/* 2949 */ 2957,
/* 2950 */ 50377,
/* 2951 */ -1,
/* 2952 */ -1,
/* 2953 */ -1,
/* 2954 */ 50059,
/* 2955 */ -2934,
/* 2956 */ -1,
/* 2957 */ 2966,
/* 2958 */ 50320,
/* 2959 */ -1,
/* 2960 */ -1,
/* 2961 */ 50047,
/* 2962 */ -1,
/* 2963 */ 50059,
/* 2964 */ -2934,
/* 2965 */ -1,
/* 2966 */ 2973,
/* 2967 */ 50294,
/* 2968 */ -1,
/* 2969 */ -1,
/* 2970 */ 50059,
/* 2971 */ -2934,
/* 2972 */ -1,
/* 2973 */ 2980,
/* 2974 */ 50424,
/* 2975 */ -1,
/* 2976 */ -1,
/* 2977 */ 50059,
/* 2978 */ -2934,
/* 2979 */ -1,
/* 2980 */ 2987,
/* 2981 */ 50355,
/* 2982 */ -1,
/* 2983 */ -1,
/* 2984 */ 50059,
/* 2985 */ -2934,
/* 2986 */ -1,
/* 2987 */ 2997,
/* 2988 */ 50427,
/* 2989 */ -1,
/* 2990 */ -1,
/* 2991 */ -1,
/* 2992 */ -1,
/* 2993 */ -1,
/* 2994 */ 50059,
/* 2995 */ -2934,
/* 2996 */ -1,
/* 2997 */ 3004,
/* 2998 */ 50406,
/* 2999 */ -1,
/* 3000 */ -1,
/* 3001 */ 50059,
/* 3002 */ -2934,
/* 3003 */ -1,
/* 3004 */ 3011,
/* 3005 */ 50346,
/* 3006 */ -1,
/* 3007 */ -1,
/* 3008 */ 50059,
/* 3009 */ -2934,
/* 3010 */ -1,
/* 3011 */ 3015,
/* 3012 */ -1,
/* 3013 */ -2934,
/* 3014 */ -1,
/* 3015 */ 3019,
/* 3016 */ -1,
/* 3017 */ -2934,
/* 3018 */ -1,
/* 3019 */ 3023,
/* 3020 */ -1,
/* 3021 */ -2934,
/* 3022 */ -1,
/* 3023 */ 0,
/* 3024 */ -1,
/* 3025 */ -2934,
/* 3026 */ -1,
/* 3027 */ 3031,
/* 3028 */ 50058,
/* 3029 */ -3027,
/* 3030 */ -1,
/* 3031 */ 0,
/* 3032 */ -3027,
/* 3033 */ -1,
/* 3034 */ 3038,
/* 3035 */ 50058,
/* 3036 */ -3034,
/* 3037 */ -1,
/* 3038 */ 0,
/* 3039 */ -3034,
/* 3040 */ -1,
/* 3041 */ 3045,
/* 3042 */ 50100,
/* 3043 */ -3041,
/* 3044 */ -1,
/* 3045 */ 0,
/* 3046 */ -3041,
/* 3047 */ -1,
/* 3048 */ 3052,
/* 3049 */ 50058,
/* 3050 */ -3048,
/* 3051 */ -1,
/* 3052 */ 0,
/* 3053 */ -3048,
/* 3054 */ -1,
/* 3055 */ 3059,
/* 3056 */ 50100,
/* 3057 */ -3055,
/* 3058 */ -1,
/* 3059 */ 0,
/* 3060 */ -3055,
/* 3061 */ -1,
/* 3062 */ 3066,
/* 3063 */ 50058,
/* 3064 */ -3062,
/* 3065 */ -1,
/* 3066 */ 0,
/* 3067 */ -3062,
/* 3068 */ -1,
/* 3069 */ 3073,
/* 3070 */ 50058,
/* 3071 */ -3069,
/* 3072 */ -1,
/* 3073 */ 0,
/* 3074 */ -3069,
/* 3075 */ -1,
/* 3076 */ 3083,
/* 3077 */ -1,
/* 3078 */ -1,
/* 3079 */ -1,
/* 3080 */ -1,
/* 3081 */ -3076,
/* 3082 */ -1,
/* 3083 */ 0,
/* 3084 */ -3076,
/* 3085 */ -1,
/* 3086 */ 3090,
/* 3087 */ 50044,
/* 3088 */ -3086,
/* 3089 */ -1,
/* 3090 */ 0,
/* 3091 */ -3086,
/* 3092 */ -1,
/* 3093 */ 3097,
/* 3094 */ 50058,
/* 3095 */ -3093,
/* 3096 */ -1,
/* 3097 */ 0,
/* 3098 */ -3093,
/* 3099 */ -1,
/* 3100 */ 3107,
/* 3101 */ -1,
/* 3102 */ -1,
/* 3103 */ -1,
/* 3104 */ -1,
/* 3105 */ -3100,
/* 3106 */ -1,
/* 3107 */ 0,
/* 3108 */ -3100,
/* 3109 */ -1,
/* 3110 */ 3114,
/* 3111 */ 50044,
/* 3112 */ -3110,
/* 3113 */ -1,
/* 3114 */ 0,
/* 3115 */ -3110,
/* 3116 */ -1,
/* 3117 */ 3121,
/* 3118 */ 50058,
/* 3119 */ -3117,
/* 3120 */ -1,
/* 3121 */ 0,
/* 3122 */ -3117,
/* 3123 */ -1,
/* 3124 */ 3130,
/* 3125 */ -1,
/* 3126 */ -1,
/* 3127 */ -1,
/* 3128 */ -3124,
/* 3129 */ -1,
/* 3130 */ 0,
/* 3131 */ -3124,
/* 3132 */ -1,
/* 3133 */ 3137,
/* 3134 */ 50044,
/* 3135 */ -3133,
/* 3136 */ -1,
/* 3137 */ 0,
/* 3138 */ -3133,
/* 3139 */ -1,
/* 3140 */ 0,
/* 3141 */ 50091,
/* 3142 */ -3140,
/* 3143 */ -1,
/* 3144 */ 0,
/* 3145 */ 50093,
/* 3146 */ -3144,
/* 3147 */ -1,
/* 3148 */ 3152,
/* 3149 */ 50058,
/* 3150 */ -3148,
/* 3151 */ -1,
/* 3152 */ 0,
/* 3153 */ -3148,
/* 3154 */ -1,
/* 3155 */ 3161,
/* 3156 */ -1,
/* 3157 */ -1,
/* 3158 */ -1,
/* 3159 */ -3155,
/* 3160 */ -1,
/* 3161 */ 0,
/* 3162 */ -3155,
/* 3163 */ -1,
/* 3164 */ 3168,
/* 3165 */ 50044,
/* 3166 */ -3164,
/* 3167 */ -1,
/* 3168 */ 0,
/* 3169 */ -3164,
/* 3170 */ -1,
/* 3171 */ 3175,
/* 3172 */ 50058,
/* 3173 */ -3171,
/* 3174 */ -1,
/* 3175 */ 0,
/* 3176 */ -3171,
/* 3177 */ -1,
/* 3178 */ 3184,
/* 3179 */ -1,
/* 3180 */ -1,
/* 3181 */ -1,
/* 3182 */ -3178,
/* 3183 */ -1,
/* 3184 */ 0,
/* 3185 */ -3178,
/* 3186 */ -1,
/* 3187 */ 3191,
/* 3188 */ 50044,
/* 3189 */ -3187,
/* 3190 */ -1,
/* 3191 */ 0,
/* 3192 */ -3187,
/* 3193 */ -1,
/* 3194 */ 3198,
/* 3195 */ 50058,
/* 3196 */ -3194,
/* 3197 */ -1,
/* 3198 */ 0,
/* 3199 */ -3194,
/* 3200 */ -1,
/* 3201 */ 3207,
/* 3202 */ -1,
/* 3203 */ -1,
/* 3204 */ -1,
/* 3205 */ -3201,
/* 3206 */ -1,
/* 3207 */ 0,
/* 3208 */ -3201,
/* 3209 */ -1,
/* 3210 */ 3214,
/* 3211 */ 50124,
/* 3212 */ -3210,
/* 3213 */ -1,
/* 3214 */ 3218,
/* 3215 */ 50044,
/* 3216 */ -3210,
/* 3217 */ -1,
/* 3218 */ 0,
/* 3219 */ -3210,
/* 3220 */ -1,
/* 3221 */ 3231,
/* 3222 */ 50281,
/* 3223 */ 50091,
/* 3224 */ -1,
/* 3225 */ -1,
/* 3226 */ 50093,
/* 3227 */ 50293,
/* 3228 */ -1,
/* 3229 */ -3221,
/* 3230 */ -1,
/* 3231 */ 3241,
/* 3232 */ 50435,
/* 3233 */ 50091,
/* 3234 */ -1,
/* 3235 */ -1,
/* 3236 */ 50093,
/* 3237 */ 50293,
/* 3238 */ -1,
/* 3239 */ -3221,
/* 3240 */ -1,
/* 3241 */ 0,
/* 3242 */ 50343,
/* 3243 */ 50091,
/* 3244 */ -1,
/* 3245 */ -1,
/* 3246 */ 50093,
/* 3247 */ 50293,
/* 3248 */ -1,
/* 3249 */ -3221,
/* 3250 */ -1,
/* 3251 */ 3255,
/* 3252 */ -1,
/* 3253 */ -3251,
/* 3254 */ -1,
/* 3255 */ 0,
/* 3256 */ -3251,
/* 3257 */ -1,
/* 3258 */ 3262,
/* 3259 */ -1,
/* 3260 */ -3258,
/* 3261 */ -1,
/* 3262 */ 0,
/* 3263 */ -3258,
/* 3264 */ -1,
/* 3265 */ 3269,
/* 3266 */ -1,
/* 3267 */ -3265,
/* 3268 */ -1,
/* 3269 */ 0,
/* 3270 */ -3265,
/* 3271 */ -1,
/* 3272 */ 3277,
/* 3273 */ 50044,
/* 3274 */ -1,
/* 3275 */ -3272,
/* 3276 */ -1,
/* 3277 */ 3282,
/* 3278 */ 50044,
/* 3279 */ -1,
/* 3280 */ -3272,
/* 3281 */ -1,
/* 3282 */ 0,
/* 3283 */ 50044,
/* 3284 */ -1,
/* 3285 */ 50044,
/* 3286 */ -1,
/* 3287 */ -3272,
/* 3288 */ -1,
/* 3289 */ 3294,
/* 3290 */ -1,
/* 3291 */ -1,
/* 3292 */ -3289,
/* 3293 */ -1,
/* 3294 */ 3300,
/* 3295 */ 50339,
/* 3296 */ -1,
/* 3297 */ 50370,
/* 3298 */ -3289,
/* 3299 */ -1,
/* 3300 */ 3305,
/* 3301 */ 50339,
/* 3302 */ 50370,
/* 3303 */ -3289,
/* 3304 */ -1,
/* 3305 */ 3314,
/* 3306 */ 50339,
/* 3307 */ 50370,
/* 3308 */ 50434,
/* 3309 */ 50293,
/* 3310 */ -1,
/* 3311 */ -1,
/* 3312 */ -3289,
/* 3313 */ -1,
/* 3314 */ 0,
/* 3315 */ 50339,
/* 3316 */ -1,
/* 3317 */ 50370,
/* 3318 */ 50434,
/* 3319 */ 50293,
/* 3320 */ -1,
/* 3321 */ -1,
/* 3322 */ -3289,
/* 3323 */ -1,
/* 3324 */ 3328,
/* 3325 */ 50370,
/* 3326 */ -3324,
/* 3327 */ -1,
/* 3328 */ 0,
/* 3329 */ -3324,
/* 3330 */ -1,
/* 3331 */ 3335,
/* 3332 */ 50370,
/* 3333 */ -3331,
/* 3334 */ -1,
/* 3335 */ 0,
/* 3336 */ -3331,
/* 3337 */ -1,
/* 3338 */ 3342,
/* 3339 */ 50370,
/* 3340 */ -3338,
/* 3341 */ -1,
/* 3342 */ 0,
/* 3343 */ -3338,
/* 3344 */ -1,
/* 3345 */ 0,
/* 3346 */ 50329,
/* 3347 */ 50257,
/* 3348 */ 50123,
/* 3349 */ -1,
/* 3350 */ 50125,
/* 3351 */ -3345,
/* 3352 */ -1,
/* 3353 */ 3358,
/* 3354 */ -1,
/* 3355 */ -1,
/* 3356 */ -3353,
/* 3357 */ -1,
/* 3358 */ 0,
/* 3359 */ -3353,
/* 3360 */ -1,
/* 3361 */ 3368,
/* 3362 */ 50323,
/* 3363 */ -1,
/* 3364 */ 50257,
/* 3365 */ 50059,
/* 3366 */ -3361,
/* 3367 */ -1,
/* 3368 */ 3372,
/* 3369 */ -1,
/* 3370 */ -3361,
/* 3371 */ -1,
/* 3372 */ 3376,
/* 3373 */ -1,
/* 3374 */ -3361,
/* 3375 */ -1,
/* 3376 */ 0,
/* 3377 */ -1,
/* 3378 */ -3361,
/* 3379 */ -1,
/* 3380 */ 3384,
/* 3381 */ 50058,
/* 3382 */ -3380,
/* 3383 */ -1,
/* 3384 */ 0,
/* 3385 */ -3380,
/* 3386 */ -1,
/* 3387 */ 3394,
/* 3388 */ 50308,
/* 3389 */ -1,
/* 3390 */ -1,
/* 3391 */ 50059,
/* 3392 */ -3387,
/* 3393 */ -1,
/* 3394 */ 3400,
/* 3395 */ 50373,
/* 3396 */ -1,
/* 3397 */ -1,
/* 3398 */ -3387,
/* 3399 */ -1,
/* 3400 */ 0,
/* 3401 */ 50429,
/* 3402 */ -1,
/* 3403 */ -1,
/* 3404 */ 50059,
/* 3405 */ -3387,
/* 3406 */ -1,
/* 3407 */ 3411,
/* 3408 */ 50058,
/* 3409 */ -3407,
/* 3410 */ -1,
/* 3411 */ 0,
/* 3412 */ -3407,
/* 3413 */ -1,
/* 3414 */ 3422,
/* 3415 */ 50042,
/* 3416 */ -1,
/* 3417 */ -1,
/* 3418 */ -1,
/* 3419 */ -1,
/* 3420 */ -3414,
/* 3421 */ -1,
/* 3422 */ 3430,
/* 3423 */ 50042,
/* 3424 */ -1,
/* 3425 */ -1,
/* 3426 */ -1,
/* 3427 */ -1,
/* 3428 */ -3414,
/* 3429 */ -1,
/* 3430 */ 0,
/* 3431 */ -3414,
/* 3432 */ -1,
/* 3433 */ 3437,
/* 3434 */ 50044,
/* 3435 */ -3433,
/* 3436 */ -1,
/* 3437 */ 0,
/* 3438 */ -3433,
/* 3439 */ -1,
/* 3440 */ 3444,
/* 3441 */ 50044,
/* 3442 */ -3440,
/* 3443 */ -1,
/* 3444 */ 0,
/* 3445 */ -3440,
/* 3446 */ -1,
/* 3447 */ 3451,
/* 3448 */ 50058,
/* 3449 */ -3447,
/* 3450 */ -1,
/* 3451 */ 0,
/* 3452 */ -3447,
/* 3453 */ -1,
/* 3454 */ 3462,
/* 3455 */ 50042,
/* 3456 */ -1,
/* 3457 */ -1,
/* 3458 */ -1,
/* 3459 */ -1,
/* 3460 */ -3454,
/* 3461 */ -1,
/* 3462 */ 0,
/* 3463 */ -3454,
/* 3464 */ -1,
/* 3465 */ 3470,
/* 3466 */ -1,
/* 3467 */ -1,
/* 3468 */ -3465,
/* 3469 */ -1,
/* 3470 */ 0,
/* 3471 */ -3465,
/* 3472 */ -1,
/* 3473 */ 3477,
/* 3474 */ 50044,
/* 3475 */ -3473,
/* 3476 */ -1,
/* 3477 */ 0,
/* 3478 */ 50059,
/* 3479 */ -3473,
/* 3480 */ -1,
/* 3481 */ 3485,
/* 3482 */ 50058,
/* 3483 */ -3481,
/* 3484 */ -1,
/* 3485 */ 0,
/* 3486 */ -3481,
/* 3487 */ -1,
/* 3488 */ 3496,
/* 3489 */ 50042,
/* 3490 */ -1,
/* 3491 */ -1,
/* 3492 */ -1,
/* 3493 */ -1,
/* 3494 */ -3488,
/* 3495 */ -1,
/* 3496 */ 0,
/* 3497 */ -3488,
/* 3498 */ -1,
/* 3499 */ 3504,
/* 3500 */ -1,
/* 3501 */ 50037,
/* 3502 */ -3499,
/* 3503 */ -1,
/* 3504 */ 0,
/* 3505 */ -3499,
/* 3506 */ -1,
/* 3507 */ 3514,
/* 3508 */ -1,
/* 3509 */ 50293,
/* 3510 */ 50370,
/* 3511 */ -1,
/* 3512 */ -3507,
/* 3513 */ -1,
/* 3514 */ 3521,
/* 3515 */ -1,
/* 3516 */ 50293,
/* 3517 */ -1,
/* 3518 */ 50370,
/* 3519 */ -3507,
/* 3520 */ -1,
/* 3521 */ 0,
/* 3522 */ -1,
/* 3523 */ 50293,
/* 3524 */ 50370,
/* 3525 */ -1,
/* 3526 */ -3507,
/* 3527 */ -1,
/* 3528 */ 3532,
/* 3529 */ 50044,
/* 3530 */ -3528,
/* 3531 */ -1,
/* 3532 */ 0,
/* 3533 */ -3528,
/* 3534 */ -1,
/* 3535 */ 3539,
/* 3536 */ -1,
/* 3537 */ -3535,
/* 3538 */ -1,
/* 3539 */ 3543,
/* 3540 */ -1,
/* 3541 */ -3535,
/* 3542 */ -1,
/* 3543 */ 0,
/* 3544 */ 50257,
/* 3545 */ -3535,
/* 3546 */ -1,
/* 3547 */ 0,
/* 3548 */ 50291,
/* 3549 */ 50257,
/* 3550 */ 50123,
/* 3551 */ -1,
/* 3552 */ 50125,
/* 3553 */ -3547,
/* 3554 */ -1,
/* 3555 */ 3560,
/* 3556 */ -1,
/* 3557 */ -1,
/* 3558 */ -3555,
/* 3559 */ -1,
/* 3560 */ 0,
/* 3561 */ -3555,
/* 3562 */ -1,
/* 3563 */ 3570,
/* 3564 */ 50323,
/* 3565 */ -1,
/* 3566 */ 50257,
/* 3567 */ 50059,
/* 3568 */ -3563,
/* 3569 */ -1,
/* 3570 */ 3577,
/* 3571 */ 50368,
/* 3572 */ -1,
/* 3573 */ -1,
/* 3574 */ 50059,
/* 3575 */ -3563,
/* 3576 */ -1,
/* 3577 */ 3582,
/* 3578 */ -1,
/* 3579 */ 50059,
/* 3580 */ -3563,
/* 3581 */ -1,
/* 3582 */ 3589,
/* 3583 */ 50419,
/* 3584 */ -1,
/* 3585 */ -1,
/* 3586 */ 50059,
/* 3587 */ -3563,
/* 3588 */ -1,
/* 3589 */ 3596,
/* 3590 */ 50439,
/* 3591 */ -1,
/* 3592 */ -1,
/* 3593 */ 50059,
/* 3594 */ -3563,
/* 3595 */ -1,
/* 3596 */ 3603,
/* 3597 */ 50410,
/* 3598 */ -1,
/* 3599 */ -1,
/* 3600 */ 50059,
/* 3601 */ -3563,
/* 3602 */ -1,
/* 3603 */ 3610,
/* 3604 */ 50334,
/* 3605 */ 50349,
/* 3606 */ -1,
/* 3607 */ 50059,
/* 3608 */ -3563,
/* 3609 */ -1,
/* 3610 */ 3617,
/* 3611 */ 50465,
/* 3612 */ 50349,
/* 3613 */ -1,
/* 3614 */ 50059,
/* 3615 */ -3563,
/* 3616 */ -1,
/* 3617 */ 3624,
/* 3618 */ 50362,
/* 3619 */ 50349,
/* 3620 */ -1,
/* 3621 */ 50059,
/* 3622 */ -3563,
/* 3623 */ -1,
/* 3624 */ 3631,
/* 3625 */ 50367,
/* 3626 */ 50349,
/* 3627 */ -1,
/* 3628 */ 50059,
/* 3629 */ -3563,
/* 3630 */ -1,
/* 3631 */ 3635,
/* 3632 */ -1,
/* 3633 */ -3563,
/* 3634 */ -1,
/* 3635 */ 0,
/* 3636 */ -1,
/* 3637 */ -3563,
/* 3638 */ -1,
/* 3639 */ 3643,
/* 3640 */ 50058,
/* 3641 */ -3639,
/* 3642 */ -1,
/* 3643 */ 0,
/* 3644 */ -3639,
/* 3645 */ -1,
/* 3646 */ 3650,
/* 3647 */ 50058,
/* 3648 */ -3646,
/* 3649 */ -1,
/* 3650 */ 0,
/* 3651 */ -3646,
/* 3652 */ -1,
/* 3653 */ 3661,
/* 3654 */ 50276,
/* 3655 */ -1,
/* 3656 */ -1,
/* 3657 */ -1,
/* 3658 */ -1,
/* 3659 */ -3653,
/* 3660 */ -1,
/* 3661 */ 0,
/* 3662 */ -3653,
/* 3663 */ -1,
/* 3664 */ 3668,
/* 3665 */ 50058,
/* 3666 */ -3664,
/* 3667 */ -1,
/* 3668 */ 0,
/* 3669 */ -3664,
/* 3670 */ -1,
/* 3671 */ 3675,
/* 3672 */ 50044,
/* 3673 */ -3671,
/* 3674 */ -1,
/* 3675 */ 0,
/* 3676 */ -3671,
/* 3677 */ -1,
/* 3678 */ 3682,
/* 3679 */ 50058,
/* 3680 */ -3678,
/* 3681 */ -1,
/* 3682 */ 0,
/* 3683 */ -3678,
/* 3684 */ -1,
/* 3685 */ 3691,
/* 3686 */ -1,
/* 3687 */ -1,
/* 3688 */ -1,
/* 3689 */ -3685,
/* 3690 */ -1,
/* 3691 */ 0,
/* 3692 */ -3685,
/* 3693 */ -1,
/* 3694 */ 3698,
/* 3695 */ 50044,
/* 3696 */ -3694,
/* 3697 */ -1,
/* 3698 */ 0,
/* 3699 */ -3694,
/* 3700 */ -1,
/* 3701 */ 3705,
/* 3702 */ 50058,
/* 3703 */ -3701,
/* 3704 */ -1,
/* 3705 */ 0,
/* 3706 */ -3701,
/* 3707 */ -1,
/* 3708 */ 3714,
/* 3709 */ 50276,
/* 3710 */ -1,
/* 3711 */ -1,
/* 3712 */ -3708,
/* 3713 */ -1,
/* 3714 */ 0,
/* 3715 */ -3708,
/* 3716 */ -1,
/* 3717 */ 3721,
/* 3718 */ 50044,
/* 3719 */ -3717,
/* 3720 */ -1,
/* 3721 */ 0,
/* 3722 */ -3717,
/* 3723 */ -1,
/* 3724 */ 3728,
/* 3725 */ 50058,
/* 3726 */ -3724,
/* 3727 */ -1,
/* 3728 */ 0,
/* 3729 */ -3724,
/* 3730 */ -1,
/* 3731 */ 3737,
/* 3732 */ -1,
/* 3733 */ -1,
/* 3734 */ -1,
/* 3735 */ -3731,
/* 3736 */ -1,
/* 3737 */ 0,
/* 3738 */ -3731,
/* 3739 */ -1,
/* 3740 */ 3744,
/* 3741 */ 50044,
/* 3742 */ -3740,
/* 3743 */ -1,
/* 3744 */ 0,
/* 3745 */ -3740,
/* 3746 */ -1,
/* 3747 */ 3753,
/* 3748 */ -1,
/* 3749 */ -1,
/* 3750 */ -1,
/* 3751 */ -3747,
/* 3752 */ -1,
/* 3753 */ 0,
/* 3754 */ -3747,
/* 3755 */ -1,
/* 3756 */ 0,
/* 3757 */ 50044,
/* 3758 */ -3756,
/* 3759 */ -1,
/* 3760 */ 3766,
/* 3761 */ -1,
/* 3762 */ -1,
/* 3763 */ -1,
/* 3764 */ -3760,
/* 3765 */ -1,
/* 3766 */ 0,
/* 3767 */ -3760,
/* 3768 */ -1,
/* 3769 */ 0,
/* 3770 */ 50044,
/* 3771 */ -3769,
/* 3772 */ -1,
/* 3773 */ 3779,
/* 3774 */ -1,
/* 3775 */ -1,
/* 3776 */ -1,
/* 3777 */ -3773,
/* 3778 */ -1,
/* 3779 */ 0,
/* 3780 */ -3773,
/* 3781 */ -1,
/* 3782 */ 0,
/* 3783 */ 50044,
/* 3784 */ -3782,
/* 3785 */ -1,
/* 3786 */ 3792,
/* 3787 */ -1,
/* 3788 */ -1,
/* 3789 */ -1,
/* 3790 */ -3786,
/* 3791 */ -1,
/* 3792 */ 0,
/* 3793 */ -3786,
/* 3794 */ -1,
/* 3795 */ 0,
/* 3796 */ 50044,
/* 3797 */ -3795,
/* 3798 */ -1,
/* 3799 */ 3808,
/* 3800 */ 50435,
/* 3801 */ 50091,
/* 3802 */ -1,
/* 3803 */ 50044,
/* 3804 */ -1,
/* 3805 */ 50093,
/* 3806 */ -3799,
/* 3807 */ -1,
/* 3808 */ 3815,
/* 3809 */ 50435,
/* 3810 */ 50091,
/* 3811 */ -1,
/* 3812 */ 50093,
/* 3813 */ -3799,
/* 3814 */ -1,
/* 3815 */ 3820,
/* 3816 */ -1,
/* 3817 */ -1,
/* 3818 */ -3799,
/* 3819 */ -1,
/* 3820 */ 3827,
/* 3821 */ -1,
/* 3822 */ 50339,
/* 3823 */ -1,
/* 3824 */ 50454,
/* 3825 */ -3799,
/* 3826 */ -1,
/* 3827 */ 3834,
/* 3828 */ -1,
/* 3829 */ -1,
/* 3830 */ -1,
/* 3831 */ 50318,
/* 3832 */ -3799,
/* 3833 */ -1,
/* 3834 */ 3841,
/* 3835 */ -1,
/* 3836 */ -1,
/* 3837 */ -1,
/* 3838 */ 50460,
/* 3839 */ -3799,
/* 3840 */ -1,
/* 3841 */ 3846,
/* 3842 */ 50366,
/* 3843 */ -1,
/* 3844 */ -3799,
/* 3845 */ -1,
/* 3846 */ 0,
/* 3847 */ 50343,
/* 3848 */ -1,
/* 3849 */ -3799,
/* 3850 */ -1,
/* 3851 */ 3856,
/* 3852 */ 50293,
/* 3853 */ 50278,
/* 3854 */ -3851,
/* 3855 */ -1,
/* 3856 */ 0,
/* 3857 */ -3851,
/* 3858 */ -1,
/* 3859 */ 3863,
/* 3860 */ 50047,
/* 3861 */ -3859,
/* 3862 */ -1,
/* 3863 */ 0,
/* 3864 */ 50403,
/* 3865 */ -3859,
/* 3866 */ -1,
/* 3867 */ 3871,
/* 3868 */ 50047,
/* 3869 */ -3867,
/* 3870 */ -1,
/* 3871 */ 0,
/* 3872 */ 50403,
/* 3873 */ -3867,
/* 3874 */ -1,
/* 3875 */ 0,
/* 3876 */ 50325,
/* 3877 */ 50257,
/* 3878 */ 50058,
/* 3879 */ -1,
/* 3880 */ 50123,
/* 3881 */ -1,
/* 3882 */ 50125,
/* 3883 */ -3875,
/* 3884 */ -1,
/* 3885 */ 3890,
/* 3886 */ -1,
/* 3887 */ -1,
/* 3888 */ -3885,
/* 3889 */ -1,
/* 3890 */ 0,
/* 3891 */ -3885,
/* 3892 */ -1,
/* 3893 */ 3900,
/* 3894 */ 50432,
/* 3895 */ -1,
/* 3896 */ -1,
/* 3897 */ 50059,
/* 3898 */ -3893,
/* 3899 */ -1,
/* 3900 */ 3907,
/* 3901 */ 50355,
/* 3902 */ -1,
/* 3903 */ -1,
/* 3904 */ 50059,
/* 3905 */ -3893,
/* 3906 */ -1,
/* 3907 */ 3914,
/* 3908 */ 50323,
/* 3909 */ -1,
/* 3910 */ 50257,
/* 3911 */ 50059,
/* 3912 */ -3893,
/* 3913 */ -1,
/* 3914 */ 3918,
/* 3915 */ -1,
/* 3916 */ -3893,
/* 3917 */ -1,
/* 3918 */ 3922,
/* 3919 */ -1,
/* 3920 */ -3893,
/* 3921 */ -1,
/* 3922 */ 0,
/* 3923 */ -1,
/* 3924 */ -3893,
/* 3925 */ -1,
/* 3926 */ 3930,
/* 3927 */ 50058,
/* 3928 */ -3926,
/* 3929 */ -1,
/* 3930 */ 0,
/* 3931 */ -3926,
/* 3932 */ -1,
/* 3933 */ 3939,
/* 3934 */ -1,
/* 3935 */ -1,
/* 3936 */ -1,
/* 3937 */ -3933,
/* 3938 */ -1,
/* 3939 */ 0,
/* 3940 */ -3933,
/* 3941 */ -1,
/* 3942 */ 3946,
/* 3943 */ 50044,
/* 3944 */ -3942,
/* 3945 */ -1,
/* 3946 */ 0,
/* 3947 */ -3942,
/* 3948 */ -1,
/* 3949 */ 3953,
/* 3950 */ 50058,
/* 3951 */ -3949,
/* 3952 */ -1,
/* 3953 */ 0,
/* 3954 */ -3949,
/* 3955 */ -1,
/* 3956 */ 3962,
/* 3957 */ -1,
/* 3958 */ -1,
/* 3959 */ -1,
/* 3960 */ -3956,
/* 3961 */ -1,
/* 3962 */ 0,
/* 3963 */ -3956,
/* 3964 */ -1,
/* 3965 */ 3969,
/* 3966 */ 50044,
/* 3967 */ -3965,
/* 3968 */ -1,
/* 3969 */ 0,
/* 3970 */ -3965,
/* 3971 */ -1,
/* 3972 */ 3976,
/* 3973 */ 50058,
/* 3974 */ -3972,
/* 3975 */ -1,
/* 3976 */ 0,
/* 3977 */ -3972,
/* 3978 */ -1,
/* 3979 */ 0,
/* 3980 */ 50354,
/* 3981 */ 50257,
/* 3982 */ 50123,
/* 3983 */ -1,
/* 3984 */ 50125,
/* 3985 */ -3979,
/* 3986 */ -1,
/* 3987 */ 3992,
/* 3988 */ -1,
/* 3989 */ -1,
/* 3990 */ -3987,
/* 3991 */ -1,
/* 3992 */ 0,
/* 3993 */ -3987,
/* 3994 */ -1,
/* 3995 */ 4002,
/* 3996 */ 50326,
/* 3997 */ -1,
/* 3998 */ -1,
/* 3999 */ 50059,
/* 4000 */ -3995,
/* 4001 */ -1,
/* 4002 */ 4007,
/* 4003 */ 50308,
/* 4004 */ 50059,
/* 4005 */ -3995,
/* 4006 */ -1,
/* 4007 */ 4014,
/* 4008 */ 50346,
/* 4009 */ -1,
/* 4010 */ -1,
/* 4011 */ 50059,
/* 4012 */ -3995,
/* 4013 */ -1,
/* 4014 */ 4021,
/* 4015 */ 50323,
/* 4016 */ -1,
/* 4017 */ 50257,
/* 4018 */ 50059,
/* 4019 */ -3995,
/* 4020 */ -1,
/* 4021 */ 4028,
/* 4022 */ 50355,
/* 4023 */ -1,
/* 4024 */ -1,
/* 4025 */ 50059,
/* 4026 */ -3995,
/* 4027 */ -1,
/* 4028 */ 4032,
/* 4029 */ -1,
/* 4030 */ -3995,
/* 4031 */ -1,
/* 4032 */ 4036,
/* 4033 */ -1,
/* 4034 */ -3995,
/* 4035 */ -1,
/* 4036 */ 0,
/* 4037 */ -1,
/* 4038 */ -3995,
/* 4039 */ -1,
/* 4040 */ 4044,
/* 4041 */ 50058,
/* 4042 */ -4040,
/* 4043 */ -1,
/* 4044 */ 0,
/* 4045 */ -4040,
/* 4046 */ -1,
/* 4047 */ 4053,
/* 4048 */ -1,
/* 4049 */ -1,
/* 4050 */ -1,
/* 4051 */ -4047,
/* 4052 */ -1,
/* 4053 */ 0,
/* 4054 */ -4047,
/* 4055 */ -1,
/* 4056 */ 4060,
/* 4057 */ 50044,
/* 4058 */ -4056,
/* 4059 */ -1,
/* 4060 */ 0,
/* 4061 */ -4056,
/* 4062 */ -1,
/* 4063 */ 4067,
/* 4064 */ 50058,
/* 4065 */ -4063,
/* 4066 */ -1,
/* 4067 */ 0,
/* 4068 */ -4063,
/* 4069 */ -1,
/* 4070 */ 4076,
/* 4071 */ -1,
/* 4072 */ -1,
/* 4073 */ -1,
/* 4074 */ -4070,
/* 4075 */ -1,
/* 4076 */ 0,
/* 4077 */ -4070,
/* 4078 */ -1,
/* 4079 */ 4083,
/* 4080 */ 50124,
/* 4081 */ -4079,
/* 4082 */ -1,
/* 4083 */ 4087,
/* 4084 */ 50044,
/* 4085 */ -4079,
/* 4086 */ -1,
/* 4087 */ 0,
/* 4088 */ -4079,
/* 4089 */ -1,
/* 4090 */ 4094,
/* 4091 */ 50058,
/* 4092 */ -4090,
/* 4093 */ -1,
/* 4094 */ 0,
/* 4095 */ -4090,
/* 4096 */ -1,
/* 4097 */ 4101,
/* 4098 */ 50058,
/* 4099 */ -4097,
/* 4100 */ -1,
/* 4101 */ 0,
/* 4102 */ -4097,
/* 4103 */ -1,
/* 4104 */ 4110,
/* 4105 */ -1,
/* 4106 */ -1,
/* 4107 */ -1,
/* 4108 */ -4104,
/* 4109 */ -1,
/* 4110 */ 0,
/* 4111 */ -4104,
/* 4112 */ -1,
/* 4113 */ 4117,
/* 4114 */ 50044,
/* 4115 */ -4113,
/* 4116 */ -1,
/* 4117 */ 0,
/* 4118 */ -4113,
/* 4119 */ -1,
/* 4120 */ 0,
/* 4121 */ 50445,
/* 4122 */ 50257,
/* 4123 */ 50123,
/* 4124 */ -1,
/* 4125 */ 50125,
/* 4126 */ -4120,
/* 4127 */ -1,
/* 4128 */ 4133,
/* 4129 */ -1,
/* 4130 */ -1,
/* 4131 */ -4128,
/* 4132 */ -1,
/* 4133 */ 0,
/* 4134 */ -4128,
/* 4135 */ -1,
/* 4136 */ 4143,
/* 4137 */ 50323,
/* 4138 */ -1,
/* 4139 */ 50257,
/* 4140 */ 50059,
/* 4141 */ -4136,
/* 4142 */ -1,
/* 4143 */ 4147,
/* 4144 */ -1,
/* 4145 */ -4136,
/* 4146 */ -1,
/* 4147 */ 4155,
/* 4148 */ 50385,
/* 4149 */ -1,
/* 4150 */ -1,
/* 4151 */ 50037,
/* 4152 */ 50059,
/* 4153 */ -4136,
/* 4154 */ -1,
/* 4155 */ 4162,
/* 4156 */ 50402,
/* 4157 */ -1,
/* 4158 */ -1,
/* 4159 */ 50059,
/* 4160 */ -4136,
/* 4161 */ -1,
/* 4162 */ 4169,
/* 4163 */ 50379,
/* 4164 */ -1,
/* 4165 */ -1,
/* 4166 */ 50059,
/* 4167 */ -4136,
/* 4168 */ -1,
/* 4169 */ 4176,
/* 4170 */ 50346,
/* 4171 */ -1,
/* 4172 */ -1,
/* 4173 */ 50059,
/* 4174 */ -4136,
/* 4175 */ -1,
/* 4176 */ 4180,
/* 4177 */ -1,
/* 4178 */ -4136,
/* 4179 */ -1,
/* 4180 */ 4184,
/* 4181 */ -1,
/* 4182 */ -4136,
/* 4183 */ -1,
/* 4184 */ 0,
/* 4185 */ -1,
/* 4186 */ -4136,
/* 4187 */ -1,
/* 4188 */ 4192,
/* 4189 */ 50058,
/* 4190 */ -4188,
/* 4191 */ -1,
/* 4192 */ 0,
/* 4193 */ -4188,
/* 4194 */ -1,
/* 4195 */ 4199,
/* 4196 */ 50058,
/* 4197 */ -4195,
/* 4198 */ -1,
/* 4199 */ 0,
/* 4200 */ -4195,
/* 4201 */ -1,
/* 4202 */ 4206,
/* 4203 */ 50058,
/* 4204 */ -4202,
/* 4205 */ -1,
/* 4206 */ 0,
/* 4207 */ -4202,
/* 4208 */ -1,
/* 4209 */ 4213,
/* 4210 */ 50058,
/* 4211 */ -4209,
/* 4212 */ -1,
/* 4213 */ 0,
/* 4214 */ -4209,
/* 4215 */ -1,
/* 4216 */ 4220,
/* 4217 */ 50058,
/* 4218 */ -4216,
/* 4219 */ -1,
/* 4220 */ 0,
/* 4221 */ -4216,
/* 4222 */ -1,
/* 4223 */ 4229,
/* 4224 */ -1,
/* 4225 */ -1,
/* 4226 */ -1,
/* 4227 */ -4223,
/* 4228 */ -1,
/* 4229 */ 0,
/* 4230 */ -4223,
/* 4231 */ -1,
/* 4232 */ 4236,
/* 4233 */ 50124,
/* 4234 */ -4232,
/* 4235 */ -1,
/* 4236 */ 4240,
/* 4237 */ 50044,
/* 4238 */ -4232,
/* 4239 */ -1,
/* 4240 */ 0,
/* 4241 */ -4232,
/* 4242 */ -1,
/* 4243 */ 0,
/* 4244 */ 50417,
/* 4245 */ 50257,
/* 4246 */ 50058,
/* 4247 */ -1,
/* 4248 */ 50123,
/* 4249 */ -1,
/* 4250 */ 50125,
/* 4251 */ -4243,
/* 4252 */ -1,
/* 4253 */ 4258,
/* 4254 */ -1,
/* 4255 */ -1,
/* 4256 */ -4253,
/* 4257 */ -1,
/* 4258 */ 0,
/* 4259 */ -4253,
/* 4260 */ -1,
/* 4261 */ 4268,
/* 4262 */ 50323,
/* 4263 */ -1,
/* 4264 */ 50257,
/* 4265 */ 50059,
/* 4266 */ -4261,
/* 4267 */ -1,
/* 4268 */ 4275,
/* 4269 */ 50459,
/* 4270 */ -1,
/* 4271 */ -1,
/* 4272 */ 50059,
/* 4273 */ -4261,
/* 4274 */ -1,
/* 4275 */ 4282,
/* 4276 */ 50347,
/* 4277 */ -1,
/* 4278 */ -1,
/* 4279 */ 50059,
/* 4280 */ -4261,
/* 4281 */ -1,
/* 4282 */ 4289,
/* 4283 */ 50324,
/* 4284 */ -1,
/* 4285 */ -1,
/* 4286 */ 50059,
/* 4287 */ -4261,
/* 4288 */ -1,
/* 4289 */ 4296,
/* 4290 */ 50322,
/* 4291 */ -1,
/* 4292 */ -1,
/* 4293 */ 50059,
/* 4294 */ -4261,
/* 4295 */ -1,
/* 4296 */ 4303,
/* 4297 */ 50426,
/* 4298 */ -1,
/* 4299 */ -1,
/* 4300 */ 50059,
/* 4301 */ -4261,
/* 4302 */ -1,
/* 4303 */ 4310,
/* 4304 */ 50421,
/* 4305 */ -1,
/* 4306 */ -1,
/* 4307 */ 50059,
/* 4308 */ -4261,
/* 4309 */ -1,
/* 4310 */ 4317,
/* 4311 */ 50346,
/* 4312 */ -1,
/* 4313 */ -1,
/* 4314 */ 50059,
/* 4315 */ -4261,
/* 4316 */ -1,
/* 4317 */ 4324,
/* 4318 */ 50356,
/* 4319 */ 50058,
/* 4320 */ 50260,
/* 4321 */ 50059,
/* 4322 */ -4261,
/* 4323 */ -1,
/* 4324 */ 4331,
/* 4325 */ 50450,
/* 4326 */ 50058,
/* 4327 */ -1,
/* 4328 */ 50059,
/* 4329 */ -4261,
/* 4330 */ -1,
/* 4331 */ 4337,
/* 4332 */ 50418,
/* 4333 */ 50058,
/* 4334 */ -1,
/* 4335 */ -4261,
/* 4336 */ -1,
/* 4337 */ 4343,
/* 4338 */ 50394,
/* 4339 */ 50058,
/* 4340 */ -1,
/* 4341 */ -4261,
/* 4342 */ -1,
/* 4343 */ 4347,
/* 4344 */ -1,
/* 4345 */ -4261,
/* 4346 */ -1,
/* 4347 */ 4351,
/* 4348 */ -1,
/* 4349 */ -4261,
/* 4350 */ -1,
/* 4351 */ 0,
/* 4352 */ -1,
/* 4353 */ -4261,
/* 4354 */ -1,
/* 4355 */ 4359,
/* 4356 */ 50058,
/* 4357 */ -4355,
/* 4358 */ -1,
/* 4359 */ 0,
/* 4360 */ -4355,
/* 4361 */ -1,
/* 4362 */ 4366,
/* 4363 */ 50058,
/* 4364 */ -4362,
/* 4365 */ -1,
/* 4366 */ 0,
/* 4367 */ -4362,
/* 4368 */ -1,
/* 4369 */ 4373,
/* 4370 */ 50058,
/* 4371 */ -4369,
/* 4372 */ -1,
/* 4373 */ 0,
/* 4374 */ -4369,
/* 4375 */ -1,
/* 4376 */ 4380,
/* 4377 */ 50058,
/* 4378 */ -4376,
/* 4379 */ -1,
/* 4380 */ 0,
/* 4381 */ -4376,
/* 4382 */ -1,
/* 4383 */ 4387,
/* 4384 */ 50058,
/* 4385 */ -4383,
/* 4386 */ -1,
/* 4387 */ 0,
/* 4388 */ -4383,
/* 4389 */ -1,
/* 4390 */ 4394,
/* 4391 */ 50058,
/* 4392 */ -4390,
/* 4393 */ -1,
/* 4394 */ 0,
/* 4395 */ -4390,
/* 4396 */ -1,
/* 4397 */ 4401,
/* 4398 */ 50058,
/* 4399 */ -4397,
/* 4400 */ -1,
/* 4401 */ 0,
/* 4402 */ -4397,
/* 4403 */ -1,
/* 4404 */ 4410,
/* 4405 */ -1,
/* 4406 */ -1,
/* 4407 */ -1,
/* 4408 */ -4404,
/* 4409 */ -1,
/* 4410 */ 0,
/* 4411 */ -4404,
/* 4412 */ -1,
/* 4413 */ 4417,
/* 4414 */ 50044,
/* 4415 */ -4413,
/* 4416 */ -1,
/* 4417 */ 4421,
/* 4418 */ 50124,
/* 4419 */ -4413,
/* 4420 */ -1,
/* 4421 */ 0,
/* 4422 */ -4413,
/* 4423 */ -1,
/* 4424 */ 4428,
/* 4425 */ 50058,
/* 4426 */ -4424,
/* 4427 */ -1,
/* 4428 */ 0,
/* 4429 */ -4424,
/* 4430 */ -1,
/* 4431 */ 4437,
/* 4432 */ -1,
/* 4433 */ -1,
/* 4434 */ -1,
/* 4435 */ -4431,
/* 4436 */ -1,
/* 4437 */ 0,
/* 4438 */ -4431,
/* 4439 */ -1,
/* 4440 */ 4444,
/* 4441 */ 50124,
/* 4442 */ -4440,
/* 4443 */ -1,
/* 4444 */ 4448,
/* 4445 */ 50044,
/* 4446 */ -4440,
/* 4447 */ -1,
/* 4448 */ 0,
/* 4449 */ -4440,
/* 4450 */ -1,
/* 4451 */ 4460,
/* 4452 */ 50259,
/* 4453 */ 50058,
/* 4454 */ -1,
/* 4455 */ -1,
/* 4456 */ -1,
/* 4457 */ -1,
/* 4458 */ -4451,
/* 4459 */ -1,
/* 4460 */ 0,
/* 4461 */ -4451,
/* 4462 */ -1,
/* 4463 */ 4470,
/* 4464 */ 50091,
/* 4465 */ -1,
/* 4466 */ 50093,
/* 4467 */ -1,
/* 4468 */ -4463,
/* 4469 */ -1,
/* 4470 */ 4477,
/* 4471 */ 50466,
/* 4472 */ -1,
/* 4473 */ -1,
/* 4474 */ -1,
/* 4475 */ -4463,
/* 4476 */ -1,
/* 4477 */ 4483,
/* 4478 */ 50292,
/* 4479 */ -1,
/* 4480 */ -1,
/* 4481 */ -4463,
/* 4482 */ -1,
/* 4483 */ 0,
/* 4484 */ -4463,
/* 4485 */ -1,
/* 4486 */ 4490,
/* 4487 */ -1,
/* 4488 */ -4486,
/* 4489 */ -1,
/* 4490 */ 4496,
/* 4491 */ 50414,
/* 4492 */ 50368,
/* 4493 */ -1,
/* 4494 */ -4486,
/* 4495 */ -1,
/* 4496 */ 0,
/* 4497 */ 50414,
/* 4498 */ 50384,
/* 4499 */ -1,
/* 4500 */ -4486,
/* 4501 */ -1,
/* 4502 */ 4506,
/* 4503 */ -1,
/* 4504 */ -4502,
/* 4505 */ -1,
/* 4506 */ 0,
/* 4507 */ -4502,
/* 4508 */ -1,
/* 4509 */ 4513,
/* 4510 */ -1,
/* 4511 */ -4509,
/* 4512 */ -1,
/* 4513 */ 0,
/* 4514 */ -4509,
/* 4515 */ -1,
/* 4516 */ 4521,
/* 4517 */ 50395,
/* 4518 */ -1,
/* 4519 */ -4516,
/* 4520 */ -1,
/* 4521 */ 4529,
/* 4522 */ 50395,
/* 4523 */ 50414,
/* 4524 */ -1,
/* 4525 */ 50452,
/* 4526 */ -1,
/* 4527 */ -4516,
/* 4528 */ -1,
/* 4529 */ 4534,
/* 4530 */ 50395,
/* 4531 */ -1,
/* 4532 */ -4516,
/* 4533 */ -1,
/* 4534 */ 0,
/* 4535 */ -4516,
/* 4536 */ -1,
/* 4537 */ 4541,
/* 4538 */ 50044,
/* 4539 */ -4537,
/* 4540 */ -1,
/* 4541 */ 0,
/* 4542 */ -4537,
/* 4543 */ -1,
/* 4544 */ 4549,
/* 4545 */ -1,
/* 4546 */ -1,
/* 4547 */ -4544,
/* 4548 */ -1,
/* 4549 */ 0,
/* 4550 */ -4544,
/* 4551 */ -1,
/* 4552 */ 4557,
/* 4553 */ -1,
/* 4554 */ -1,
/* 4555 */ -4552,
/* 4556 */ -1,
/* 4557 */ 0,
/* 4558 */ -4552,
/* 4559 */ -1,
/* 4560 */ 0,
/* 4561 */ 50444,
/* 4562 */ 50257,
/* 4563 */ 50058,
/* 4564 */ -1,
/* 4565 */ -1,
/* 4566 */ 50123,
/* 4567 */ -1,
/* 4568 */ 50125,
/* 4569 */ -4560,
/* 4570 */ -1,
/* 4571 */ 4576,
/* 4572 */ 50349,
/* 4573 */ -1,
/* 4574 */ -4571,
/* 4575 */ -1,
/* 4576 */ 0,
/* 4577 */ -4571,
/* 4578 */ -1,
/* 4579 */ 4584,
/* 4580 */ -1,
/* 4581 */ -1,
/* 4582 */ -4579,
/* 4583 */ -1,
/* 4584 */ 0,
/* 4585 */ -4579,
/* 4586 */ -1,
/* 4587 */ 4594,
/* 4588 */ 50323,
/* 4589 */ -1,
/* 4590 */ 50257,
/* 4591 */ 50059,
/* 4592 */ -4587,
/* 4593 */ -1,
/* 4594 */ 4601,
/* 4595 */ 50349,
/* 4596 */ -1,
/* 4597 */ -1,
/* 4598 */ 50059,
/* 4599 */ -4587,
/* 4600 */ -1,
/* 4601 */ 4608,
/* 4602 */ 50386,
/* 4603 */ -1,
/* 4604 */ -1,
/* 4605 */ 50059,
/* 4606 */ -4587,
/* 4607 */ -1,
/* 4608 */ 4612,
/* 4609 */ -1,
/* 4610 */ -4587,
/* 4611 */ -1,
/* 4612 */ 4619,
/* 4613 */ -1,
/* 4614 */ -1,
/* 4615 */ -1,
/* 4616 */ 50059,
/* 4617 */ -4587,
/* 4618 */ -1,
/* 4619 */ 4626,
/* 4620 */ 50360,
/* 4621 */ -1,
/* 4622 */ -1,
/* 4623 */ 50059,
/* 4624 */ -4587,
/* 4625 */ -1,
/* 4626 */ 4633,
/* 4627 */ 50362,
/* 4628 */ -1,
/* 4629 */ -1,
/* 4630 */ 50059,
/* 4631 */ -4587,
/* 4632 */ -1,
/* 4633 */ 4640,
/* 4634 */ 50320,
/* 4635 */ -1,
/* 4636 */ -1,
/* 4637 */ 50059,
/* 4638 */ -4587,
/* 4639 */ -1,
/* 4640 */ 4647,
/* 4641 */ 50290,
/* 4642 */ -1,
/* 4643 */ -1,
/* 4644 */ 50059,
/* 4645 */ -4587,
/* 4646 */ -1,
/* 4647 */ 4654,
/* 4648 */ 50316,
/* 4649 */ -1,
/* 4650 */ -1,
/* 4651 */ 50059,
/* 4652 */ -4587,
/* 4653 */ -1,
/* 4654 */ 4661,
/* 4655 */ 50405,
/* 4656 */ -1,
/* 4657 */ -1,
/* 4658 */ 50059,
/* 4659 */ -4587,
/* 4660 */ -1,
/* 4661 */ 4669,
/* 4662 */ 50303,
/* 4663 */ 50370,
/* 4664 */ -1,
/* 4665 */ -1,
/* 4666 */ 50059,
/* 4667 */ -4587,
/* 4668 */ -1,
/* 4669 */ 4676,
/* 4670 */ 50426,
/* 4671 */ -1,
/* 4672 */ -1,
/* 4673 */ 50059,
/* 4674 */ -4587,
/* 4675 */ -1,
/* 4676 */ 4683,
/* 4677 */ 50430,
/* 4678 */ -1,
/* 4679 */ -1,
/* 4680 */ 50059,
/* 4681 */ -4587,
/* 4682 */ -1,
/* 4683 */ 4690,
/* 4684 */ 50385,
/* 4685 */ -1,
/* 4686 */ -1,
/* 4687 */ 50059,
/* 4688 */ -4587,
/* 4689 */ -1,
/* 4690 */ 4697,
/* 4691 */ 50276,
/* 4692 */ -1,
/* 4693 */ -1,
/* 4694 */ -1,
/* 4695 */ -4587,
/* 4696 */ -1,
/* 4697 */ 4705,
/* 4698 */ 50294,
/* 4699 */ -1,
/* 4700 */ -1,
/* 4701 */ -1,
/* 4702 */ 50059,
/* 4703 */ -4587,
/* 4704 */ -1,
/* 4705 */ 4713,
/* 4706 */ 50435,
/* 4707 */ 50091,
/* 4708 */ -1,
/* 4709 */ 50093,
/* 4710 */ 50059,
/* 4711 */ -4587,
/* 4712 */ -1,
/* 4713 */ 4723,
/* 4714 */ 50435,
/* 4715 */ 50091,
/* 4716 */ -1,
/* 4717 */ 50044,
/* 4718 */ -1,
/* 4719 */ 50093,
/* 4720 */ 50059,
/* 4721 */ -4587,
/* 4722 */ -1,
/* 4723 */ 4735,
/* 4724 */ 50435,
/* 4725 */ 50091,
/* 4726 */ -1,
/* 4727 */ 50044,
/* 4728 */ -1,
/* 4729 */ 50044,
/* 4730 */ -1,
/* 4731 */ 50093,
/* 4732 */ 50059,
/* 4733 */ -4587,
/* 4734 */ -1,
/* 4735 */ 4749,
/* 4736 */ 50435,
/* 4737 */ 50091,
/* 4738 */ -1,
/* 4739 */ 50044,
/* 4740 */ -1,
/* 4741 */ 50044,
/* 4742 */ -1,
/* 4743 */ 50044,
/* 4744 */ -1,
/* 4745 */ 50093,
/* 4746 */ 50059,
/* 4747 */ -4587,
/* 4748 */ -1,
/* 4749 */ 4756,
/* 4750 */ 50419,
/* 4751 */ -1,
/* 4752 */ -1,
/* 4753 */ 50059,
/* 4754 */ -4587,
/* 4755 */ -1,
/* 4756 */ 4763,
/* 4757 */ 50365,
/* 4758 */ -1,
/* 4759 */ -1,
/* 4760 */ 50059,
/* 4761 */ -4587,
/* 4762 */ -1,
/* 4763 */ 4771,
/* 4764 */ 50342,
/* 4765 */ -1,
/* 4766 */ -1,
/* 4767 */ -1,
/* 4768 */ 50059,
/* 4769 */ -4587,
/* 4770 */ -1,
/* 4771 */ 4778,
/* 4772 */ 50432,
/* 4773 */ -1,
/* 4774 */ -1,
/* 4775 */ 50059,
/* 4776 */ -4587,
/* 4777 */ -1,
/* 4778 */ 4785,
/* 4779 */ 50327,
/* 4780 */ -1,
/* 4781 */ -1,
/* 4782 */ 50059,
/* 4783 */ -4587,
/* 4784 */ -1,
/* 4785 */ 4792,
/* 4786 */ 50404,
/* 4787 */ -1,
/* 4788 */ -1,
/* 4789 */ 50059,
/* 4790 */ -4587,
/* 4791 */ -1,
/* 4792 */ 4798,
/* 4793 */ 50346,
/* 4794 */ -1,
/* 4795 */ -1,
/* 4796 */ -4587,
/* 4797 */ -1,
/* 4798 */ 4802,
/* 4799 */ -1,
/* 4800 */ -4587,
/* 4801 */ -1,
/* 4802 */ 4806,
/* 4803 */ -1,
/* 4804 */ -4587,
/* 4805 */ -1,
/* 4806 */ 4810,
/* 4807 */ -1,
/* 4808 */ -4587,
/* 4809 */ -1,
/* 4810 */ 0,
/* 4811 */ -1,
/* 4812 */ -4587,
/* 4813 */ -1,
/* 4814 */ 4818,
/* 4815 */ 50058,
/* 4816 */ -4814,
/* 4817 */ -1,
/* 4818 */ 0,
/* 4819 */ -4814,
/* 4820 */ -1,
/* 4821 */ 4825,
/* 4822 */ 50058,
/* 4823 */ -4821,
/* 4824 */ -1,
/* 4825 */ 0,
/* 4826 */ -4821,
/* 4827 */ -1,
/* 4828 */ 4832,
/* 4829 */ 50058,
/* 4830 */ -4828,
/* 4831 */ -1,
/* 4832 */ 0,
/* 4833 */ -4828,
/* 4834 */ -1,
/* 4835 */ 4839,
/* 4836 */ 50370,
/* 4837 */ -4835,
/* 4838 */ -1,
/* 4839 */ 0,
/* 4840 */ 50304,
/* 4841 */ -4835,
/* 4842 */ -1,
/* 4843 */ 4847,
/* 4844 */ 50058,
/* 4845 */ -4843,
/* 4846 */ -1,
/* 4847 */ 0,
/* 4848 */ -4843,
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
/* 4872 */ 50058,
/* 4873 */ -4871,
/* 4874 */ -1,
/* 4875 */ 0,
/* 4876 */ -4871,
/* 4877 */ -1,
/* 4878 */ 4882,
/* 4879 */ 50058,
/* 4880 */ -4878,
/* 4881 */ -1,
/* 4882 */ 0,
/* 4883 */ -4878,
/* 4884 */ -1,
/* 4885 */ 4889,
/* 4886 */ 50058,
/* 4887 */ -4885,
/* 4888 */ -1,
/* 4889 */ 0,
/* 4890 */ -4885,
/* 4891 */ -1,
/* 4892 */ 4896,
/* 4893 */ 50058,
/* 4894 */ -4892,
/* 4895 */ -1,
/* 4896 */ 0,
/* 4897 */ -4892,
/* 4898 */ -1,
/* 4899 */ 4903,
/* 4900 */ 50058,
/* 4901 */ -4899,
/* 4902 */ -1,
/* 4903 */ 0,
/* 4904 */ -4899,
/* 4905 */ -1,
/* 4906 */ 4910,
/* 4907 */ 50058,
/* 4908 */ -4906,
/* 4909 */ -1,
/* 4910 */ 0,
/* 4911 */ -4906,
/* 4912 */ -1,
/* 4913 */ 4917,
/* 4914 */ 50058,
/* 4915 */ -4913,
/* 4916 */ -1,
/* 4917 */ 0,
/* 4918 */ -4913,
/* 4919 */ -1,
/* 4920 */ 4924,
/* 4921 */ 50058,
/* 4922 */ -4920,
/* 4923 */ -1,
/* 4924 */ 0,
/* 4925 */ -4920,
/* 4926 */ -1,
/* 4927 */ 4931,
/* 4928 */ 50044,
/* 4929 */ -4927,
/* 4930 */ -1,
/* 4931 */ 0,
/* 4932 */ 50059,
/* 4933 */ -4927,
/* 4934 */ -1,
/* 4935 */ 4939,
/* 4936 */ 50058,
/* 4937 */ -4935,
/* 4938 */ -1,
/* 4939 */ 0,
/* 4940 */ -4935,
/* 4941 */ -1,
/* 4942 */ 4948,
/* 4943 */ 50044,
/* 4944 */ -1,
/* 4945 */ -1,
/* 4946 */ -4942,
/* 4947 */ -1,
/* 4948 */ 0,
/* 4949 */ -4942,
/* 4950 */ -1,
/* 4951 */ 4955,
/* 4952 */ 50058,
/* 4953 */ -4951,
/* 4954 */ -1,
/* 4955 */ 0,
/* 4956 */ -4951,
/* 4957 */ -1,
/* 4958 */ 4964,
/* 4959 */ -1,
/* 4960 */ -1,
/* 4961 */ -1,
/* 4962 */ -4958,
/* 4963 */ -1,
/* 4964 */ 0,
/* 4965 */ -4958,
/* 4966 */ -1,
/* 4967 */ 4971,
/* 4968 */ -1,
/* 4969 */ -4967,
/* 4970 */ -1,
/* 4971 */ 0,
/* 4972 */ -1,
/* 4973 */ -1,
/* 4974 */ -4967,
/* 4975 */ -1,
/* 4976 */ 4980,
/* 4977 */ 50126,
/* 4978 */ -4976,
/* 4979 */ -1,
/* 4980 */ 0,
/* 4981 */ 50033,
/* 4982 */ -4976,
/* 4983 */ -1,
/* 4984 */ 4988,
/* 4985 */ 50044,
/* 4986 */ -4984,
/* 4987 */ -1,
/* 4988 */ 0,
/* 4989 */ -4984,
/* 4990 */ -1,
/* 4991 */ 4995,
/* 4992 */ 50058,
/* 4993 */ -4991,
/* 4994 */ -1,
/* 4995 */ 0,
/* 4996 */ -4991,
/* 4997 */ -1,
/* 4998 */ 5004,
/* 4999 */ -1,
/* 5000 */ -1,
/* 5001 */ -1,
/* 5002 */ -4998,
/* 5003 */ -1,
/* 5004 */ 0,
/* 5005 */ -4998,
/* 5006 */ -1,
/* 5007 */ 5011,
/* 5008 */ -1,
/* 5009 */ -5007,
/* 5010 */ -1,
/* 5011 */ 0,
/* 5012 */ -1,
/* 5013 */ -1,
/* 5014 */ -5007,
/* 5015 */ -1,
/* 5016 */ 5020,
/* 5017 */ 50126,
/* 5018 */ -5016,
/* 5019 */ -1,
/* 5020 */ 0,
/* 5021 */ 50033,
/* 5022 */ -5016,
/* 5023 */ -1,
/* 5024 */ 5028,
/* 5025 */ 50044,
/* 5026 */ -5024,
/* 5027 */ -1,
/* 5028 */ 0,
/* 5029 */ -5024,
/* 5030 */ -1,
/* 5031 */ 5035,
/* 5032 */ 50058,
/* 5033 */ -5031,
/* 5034 */ -1,
/* 5035 */ 0,
/* 5036 */ -5031,
/* 5037 */ -1,
/* 5038 */ 5044,
/* 5039 */ -1,
/* 5040 */ 50044,
/* 5041 */ -1,
/* 5042 */ -5038,
/* 5043 */ -1,
/* 5044 */ 0,
/* 5045 */ -5038,
/* 5046 */ -1,
/* 5047 */ 5051,
/* 5048 */ 50058,
/* 5049 */ -5047,
/* 5050 */ -1,
/* 5051 */ 0,
/* 5052 */ -5047,
/* 5053 */ -1,
/* 5054 */ 5060,
/* 5055 */ -1,
/* 5056 */ -1,
/* 5057 */ -1,
/* 5058 */ -5054,
/* 5059 */ -1,
/* 5060 */ 0,
/* 5061 */ -5054,
/* 5062 */ -1,
/* 5063 */ 5067,
/* 5064 */ 50044,
/* 5065 */ -5063,
/* 5066 */ -1,
/* 5067 */ 0,
/* 5068 */ -5063,
/* 5069 */ -1,
/* 5070 */ 5074,
/* 5071 */ 50058,
/* 5072 */ -5070,
/* 5073 */ -1,
/* 5074 */ 0,
/* 5075 */ -5070,
/* 5076 */ -1,
/* 5077 */ 5083,
/* 5078 */ -1,
/* 5079 */ -1,
/* 5080 */ -1,
/* 5081 */ -5077,
/* 5082 */ -1,
/* 5083 */ 0,
/* 5084 */ -5077,
/* 5085 */ -1,
/* 5086 */ 5090,
/* 5087 */ 50044,
/* 5088 */ -5086,
/* 5089 */ -1,
/* 5090 */ 0,
/* 5091 */ -5086,
/* 5092 */ -1,
/* 5093 */ 5097,
/* 5094 */ 50058,
/* 5095 */ -5093,
/* 5096 */ -1,
/* 5097 */ 0,
/* 5098 */ -5093,
/* 5099 */ -1,
/* 5100 */ 5106,
/* 5101 */ -1,
/* 5102 */ -1,
/* 5103 */ -1,
/* 5104 */ -5100,
/* 5105 */ -1,
/* 5106 */ 0,
/* 5107 */ -5100,
/* 5108 */ -1,
/* 5109 */ 5113,
/* 5110 */ 50044,
/* 5111 */ -5109,
/* 5112 */ -1,
/* 5113 */ 0,
/* 5114 */ -5109,
/* 5115 */ -1,
/* 5116 */ 5120,
/* 5117 */ 50058,
/* 5118 */ -5116,
/* 5119 */ -1,
/* 5120 */ 0,
/* 5121 */ -5116,
/* 5122 */ -1,
/* 5123 */ 5129,
/* 5124 */ -1,
/* 5125 */ -1,
/* 5126 */ -1,
/* 5127 */ -5123,
/* 5128 */ -1,
/* 5129 */ 0,
/* 5130 */ -5123,
/* 5131 */ -1,
/* 5132 */ 5136,
/* 5133 */ -1,
/* 5134 */ -5132,
/* 5135 */ -1,
/* 5136 */ 0,
/* 5137 */ -1,
/* 5138 */ -1,
/* 5139 */ -5132,
/* 5140 */ -1,
/* 5141 */ 5145,
/* 5142 */ 50126,
/* 5143 */ -5141,
/* 5144 */ -1,
/* 5145 */ 0,
/* 5146 */ 50033,
/* 5147 */ -5141,
/* 5148 */ -1,
/* 5149 */ 5153,
/* 5150 */ 50044,
/* 5151 */ -5149,
/* 5152 */ -1,
/* 5153 */ 0,
/* 5154 */ 50059,
/* 5155 */ -5149,
/* 5156 */ -1,
/* 5157 */ 5167,
/* 5158 */ -1,
/* 5159 */ 50349,
/* 5160 */ -1,
/* 5161 */ -1,
/* 5162 */ -1,
/* 5163 */ -1,
/* 5164 */ -1,
/* 5165 */ -5157,
/* 5166 */ -1,
/* 5167 */ 0,
/* 5168 */ -1,
/* 5169 */ 50349,
/* 5170 */ -1,
/* 5171 */ -1,
/* 5172 */ -5157,
/* 5173 */ -1,
/* 5174 */ 5178,
/* 5175 */ -1,
/* 5176 */ -5174,
/* 5177 */ -1,
/* 5178 */ 5183,
/* 5179 */ -1,
/* 5180 */ 50037,
/* 5181 */ -5174,
/* 5182 */ -1,
/* 5183 */ 0,
/* 5184 */ -5174,
/* 5185 */ -1,
/* 5186 */ 5190,
/* 5187 */ -1,
/* 5188 */ -5186,
/* 5189 */ -1,
/* 5190 */ 0,
/* 5191 */ -5186,
/* 5192 */ -1,
/* 5193 */ 5197,
/* 5194 */ 50316,
/* 5195 */ -5193,
/* 5196 */ -1,
/* 5197 */ 0,
/* 5198 */ -5193,
/* 5199 */ -1,
/* 5200 */ 5207,
/* 5201 */ 50040,
/* 5202 */ -1,
/* 5203 */ -1,
/* 5204 */ 50041,
/* 5205 */ -5200,
/* 5206 */ -1,
/* 5207 */ 0,
/* 5208 */ -5200,
/* 5209 */ -1,
/* 5210 */ 5214,
/* 5211 */ 50319,
/* 5212 */ -5210,
/* 5213 */ -1,
/* 5214 */ 5218,
/* 5215 */ 50304,
/* 5216 */ -5210,
/* 5217 */ -1,
/* 5218 */ 0,
/* 5219 */ 50370,
/* 5220 */ -5210,
/* 5221 */ -1,
/* 5222 */ 5231,
/* 5223 */ 50287,
/* 5224 */ -1,
/* 5225 */ -1,
/* 5226 */ -1,
/* 5227 */ -1,
/* 5228 */ -1,
/* 5229 */ -5222,
/* 5230 */ -1,
/* 5231 */ 0,
/* 5232 */ -5222,
/* 5233 */ -1,
/* 5234 */ 5238,
/* 5235 */ -1,
/* 5236 */ -5234,
/* 5237 */ -1,
/* 5238 */ 0,
/* 5239 */ -5234,
/* 5240 */ -1,
/* 5241 */ 5245,
/* 5242 */ -1,
/* 5243 */ -5241,
/* 5244 */ -1,
/* 5245 */ 0,
/* 5246 */ -5241,
/* 5247 */ -1,
/* 5248 */ 5252,
/* 5249 */ 50316,
/* 5250 */ -5248,
/* 5251 */ -1,
/* 5252 */ 0,
/* 5253 */ -5248,
/* 5254 */ -1,
/* 5255 */ 5262,
/* 5256 */ 50040,
/* 5257 */ -1,
/* 5258 */ -1,
/* 5259 */ 50041,
/* 5260 */ -5255,
/* 5261 */ -1,
/* 5262 */ 0,
/* 5263 */ -5255,
/* 5264 */ -1,
/* 5265 */ 5269,
/* 5266 */ 50319,
/* 5267 */ -5265,
/* 5268 */ -1,
/* 5269 */ 5273,
/* 5270 */ 50304,
/* 5271 */ -5265,
/* 5272 */ -1,
/* 5273 */ 0,
/* 5274 */ 50370,
/* 5275 */ -5265,
/* 5276 */ -1,
/* 5277 */ 5281,
/* 5278 */ -1,
/* 5279 */ -5277,
/* 5280 */ -1,
/* 5281 */ 0,
/* 5282 */ -5277,
/* 5283 */ -1,
/* 5284 */ 0,
/* 5285 */ 50261,
/* 5286 */ -5284,
/* 5287 */ -1,
/* 5288 */ 0,
/* 5289 */ 50337,
/* 5290 */ 50257,
/* 5291 */ 50058,
/* 5292 */ -1,
/* 5293 */ 50123,
/* 5294 */ -1,
/* 5295 */ 50125,
/* 5296 */ -5288,
/* 5297 */ -1,
/* 5298 */ 5303,
/* 5299 */ -1,
/* 5300 */ -1,
/* 5301 */ -5298,
/* 5302 */ -1,
/* 5303 */ 0,
/* 5304 */ -5298,
/* 5305 */ -1,
/* 5306 */ 5313,
/* 5307 */ 50323,
/* 5308 */ -1,
/* 5309 */ 50257,
/* 5310 */ 50059,
/* 5311 */ -5306,
/* 5312 */ -1,
/* 5313 */ 5320,
/* 5314 */ 50379,
/* 5315 */ -1,
/* 5316 */ -1,
/* 5317 */ 50059,
/* 5318 */ -5306,
/* 5319 */ -1,
/* 5320 */ 5327,
/* 5321 */ 50307,
/* 5322 */ -1,
/* 5323 */ -1,
/* 5324 */ 50059,
/* 5325 */ -5306,
/* 5326 */ -1,
/* 5327 */ 0,
/* 5328 */ 50461,
/* 5329 */ -1,
/* 5330 */ -1,
/* 5331 */ 50037,
/* 5332 */ 50059,
/* 5333 */ -5306,
/* 5334 */ -1,
/* 5335 */ 5339,
/* 5336 */ 50058,
/* 5337 */ -5335,
/* 5338 */ -1,
/* 5339 */ 0,
/* 5340 */ -5335,
/* 5341 */ -1,
/* 5342 */ 5346,
/* 5343 */ 50058,
/* 5344 */ -5342,
/* 5345 */ -1,
/* 5346 */ 0,
/* 5347 */ -5342,
/* 5348 */ -1,
/* 5349 */ 5353,
/* 5350 */ 50058,
/* 5351 */ -5349,
/* 5352 */ -1,
/* 5353 */ 0,
/* 5354 */ -5349,
/* 5355 */ -1,
/* 5356 */ 5360,
/* 5357 */ 50058,
/* 5358 */ -5356,
/* 5359 */ -1,
/* 5360 */ 0,
/* 5361 */ -5356,
/* 5362 */ -1,
/* 5363 */ 0,
/* 5364 */ 50261,
/* 5365 */ -5363,
/* 5366 */ -1,
/* 5367 */ 5371,
/* 5368 */ 50264,
/* 5369 */ -5367,
/* 5370 */ -1,
/* 5371 */ 5375,
/* 5372 */ 50267,
/* 5373 */ -5367,
/* 5374 */ -1,
/* 5375 */ 5379,
/* 5376 */ 50263,
/* 5377 */ -5367,
/* 5378 */ -1,
/* 5379 */ 5383,
/* 5380 */ 50271,
/* 5381 */ -5367,
/* 5382 */ -1,
/* 5383 */ 5387,
/* 5384 */ 50272,
/* 5385 */ -5367,
/* 5386 */ -1,
/* 5387 */ 5391,
/* 5388 */ 50273,
/* 5389 */ -5367,
/* 5390 */ -1,
/* 5391 */ 0,
/* 5392 */ 50469,
/* 5393 */ -5367,
/* 5394 */ -1,
/* 5395 */ 0,
/* 5396 */ 50442,
/* 5397 */ -1,
/* 5398 */ -1,
/* 5399 */ 50262,
/* 5400 */ 50258,
/* 5401 */ 50040,
/* 5402 */ -1,
/* 5403 */ -1,
/* 5404 */ 50041,
/* 5405 */ 50059,
/* 5406 */ -5395,
/* 5407 */ -1,
/* 5408 */ 5416,
/* 5409 */ -1,
/* 5410 */ -1,
/* 5411 */ -1,
/* 5412 */ -1,
/* 5413 */ -1,
/* 5414 */ -5408,
/* 5415 */ -1,
/* 5416 */ 0,
/* 5417 */ -5408,
/* 5418 */ -1,
/* 5419 */ 5424,
/* 5420 */ 50058,
/* 5421 */ -1,
/* 5422 */ -5419,
/* 5423 */ -1,
/* 5424 */ 0,
/* 5425 */ -5419,
/* 5426 */ -1,
/* 5427 */ 5431,
/* 5428 */ 50258,
/* 5429 */ -5427,
/* 5430 */ -1,
/* 5431 */ 0,
/* 5432 */ -5427,
/* 5433 */ -1,
/* 5434 */ 5439,
/* 5435 */ 50061,
/* 5436 */ -1,
/* 5437 */ -5434,
/* 5438 */ -1,
/* 5439 */ 0,
/* 5440 */ -5434,
/* 5441 */ -1,
/* 5442 */ 5451,
/* 5443 */ 50044,
/* 5444 */ -1,
/* 5445 */ -1,
/* 5446 */ -1,
/* 5447 */ -1,
/* 5448 */ -1,
/* 5449 */ -5442,
/* 5450 */ -1,
/* 5451 */ 0,
/* 5452 */ -5442,
/* 5453 */ -1,
/* 5454 */ 5459,
/* 5455 */ 50058,
/* 5456 */ -1,
/* 5457 */ -5454,
/* 5458 */ -1,
/* 5459 */ 0,
/* 5460 */ -5454,
/* 5461 */ -1,
/* 5462 */ 5466,
/* 5463 */ 50258,
/* 5464 */ -5462,
/* 5465 */ -1,
/* 5466 */ 0,
/* 5467 */ -5462,
/* 5468 */ -1,
/* 5469 */ 5474,
/* 5470 */ 50061,
/* 5471 */ -1,
/* 5472 */ -5469,
/* 5473 */ -1,
/* 5474 */ 0,
/* 5475 */ -5469,
/* 5476 */ -1,
/* 5477 */ 5482,
/* 5478 */ 50044,
/* 5479 */ 50490,
/* 5480 */ -5477,
/* 5481 */ -1,
/* 5482 */ 0,
/* 5483 */ -5477,
/* 5484 */ -1,
/* 5485 */ 0,
/* 5486 */ 50442,
/* 5487 */ -1,
/* 5488 */ 50258,
/* 5489 */ 50040,
/* 5490 */ -1,
/* 5491 */ -1,
/* 5492 */ 50041,
/* 5493 */ 50059,
/* 5494 */ -5485,
/* 5495 */ -1,
/* 5496 */ 5504,
/* 5497 */ -1,
/* 5498 */ -1,
/* 5499 */ -1,
/* 5500 */ -1,
/* 5501 */ -1,
/* 5502 */ -5496,
/* 5503 */ -1,
/* 5504 */ 0,
/* 5505 */ -5496,
/* 5506 */ -1,
/* 5507 */ 5512,
/* 5508 */ 50058,
/* 5509 */ -1,
/* 5510 */ -5507,
/* 5511 */ -1,
/* 5512 */ 0,
/* 5513 */ -5507,
/* 5514 */ -1,
/* 5515 */ 5519,
/* 5516 */ 50258,
/* 5517 */ -5515,
/* 5518 */ -1,
/* 5519 */ 0,
/* 5520 */ -5515,
/* 5521 */ -1,
/* 5522 */ 5527,
/* 5523 */ 50061,
/* 5524 */ -1,
/* 5525 */ -5522,
/* 5526 */ -1,
/* 5527 */ 0,
/* 5528 */ -5522,
/* 5529 */ -1,
/* 5530 */ 5539,
/* 5531 */ 50044,
/* 5532 */ -1,
/* 5533 */ -1,
/* 5534 */ -1,
/* 5535 */ -1,
/* 5536 */ -1,
/* 5537 */ -5530,
/* 5538 */ -1,
/* 5539 */ 0,
/* 5540 */ -5530,
/* 5541 */ -1,
/* 5542 */ 5547,
/* 5543 */ 50058,
/* 5544 */ -1,
/* 5545 */ -5542,
/* 5546 */ -1,
/* 5547 */ 0,
/* 5548 */ -5542,
/* 5549 */ -1,
/* 5550 */ 5554,
/* 5551 */ 50258,
/* 5552 */ -5550,
/* 5553 */ -1,
/* 5554 */ 0,
/* 5555 */ -5550,
/* 5556 */ -1,
/* 5557 */ 5562,
/* 5558 */ 50061,
/* 5559 */ -1,
/* 5560 */ -5557,
/* 5561 */ -1,
/* 5562 */ 0,
/* 5563 */ -5557,
/* 5564 */ -1,
/* 5565 */ 5570,
/* 5566 */ 50044,
/* 5567 */ 50490,
/* 5568 */ -5565,
/* 5569 */ -1,
/* 5570 */ 0,
/* 5571 */ -5565,
/* 5572 */ -1,
/* 5573 */ 0,
/* 5574 */ 50442,
/* 5575 */ -1,
/* 5576 */ -1,
/* 5577 */ 50262,
/* 5578 */ 50258,
/* 5579 */ 50059,
/* 5580 */ -5573,
/* 5581 */ -1,
/* 5582 */ 0,
/* 5583 */ 50442,
/* 5584 */ -1,
/* 5585 */ 50258,
/* 5586 */ 50476,
/* 5587 */ -1,
/* 5588 */ 50059,
/* 5589 */ -5582,
/* 5590 */ -1,
/* 5591 */ 0,
/* 5592 */ -1,
/* 5593 */ 50258,
/* 5594 */ -1,
/* 5595 */ 50059,
/* 5596 */ -5591,
/* 5597 */ -1,
/* 5598 */ 5604,
/* 5599 */ 50044,
/* 5600 */ 50258,
/* 5601 */ -1,
/* 5602 */ -5598,
/* 5603 */ -1,
/* 5604 */ 0,
/* 5605 */ -5598,
/* 5606 */ -1,
/* 5607 */ 0,
/* 5608 */ -1,
/* 5609 */ 50258,
/* 5610 */ -1,
/* 5611 */ 50059,
/* 5612 */ -5607,
/* 5613 */ -1,
/* 5614 */ 5620,
/* 5615 */ 50044,
/* 5616 */ 50258,
/* 5617 */ -1,
/* 5618 */ -5614,
/* 5619 */ -1,
/* 5620 */ 0,
/* 5621 */ -5614,
/* 5622 */ -1,
/* 5623 */ 0,
/* 5624 */ -1,
/* 5625 */ 50258,
/* 5626 */ -1,
/* 5627 */ 50059,
/* 5628 */ -5623,
/* 5629 */ -1,
/* 5630 */ 5636,
/* 5631 */ 50044,
/* 5632 */ 50258,
/* 5633 */ -1,
/* 5634 */ -5630,
/* 5635 */ -1,
/* 5636 */ 0,
/* 5637 */ -5630,
/* 5638 */ -1,
/* 5639 */ 5644,
/* 5640 */ -1,
/* 5641 */ 50059,
/* 5642 */ -5639,
/* 5643 */ -1,
/* 5644 */ 5654,
/* 5645 */ 50363,
/* 5646 */ 50040,
/* 5647 */ -1,
/* 5648 */ 50041,
/* 5649 */ -1,
/* 5650 */ 50332,
/* 5651 */ -1,
/* 5652 */ -5639,
/* 5653 */ -1,
/* 5654 */ 5662,
/* 5655 */ 50363,
/* 5656 */ 50040,
/* 5657 */ -1,
/* 5658 */ 50041,
/* 5659 */ -1,
/* 5660 */ -5639,
/* 5661 */ -1,
/* 5662 */ 5674,
/* 5663 */ 50349,
/* 5664 */ 50040,
/* 5665 */ -1,
/* 5666 */ 50059,
/* 5667 */ -1,
/* 5668 */ 50059,
/* 5669 */ -1,
/* 5670 */ 50041,
/* 5671 */ -1,
/* 5672 */ -5639,
/* 5673 */ -1,
/* 5674 */ 5685,
/* 5675 */ 50441,
/* 5676 */ 50040,
/* 5677 */ -1,
/* 5678 */ 50041,
/* 5679 */ 50123,
/* 5680 */ -1,
/* 5681 */ -1,
/* 5682 */ 50125,
/* 5683 */ -5639,
/* 5684 */ -1,
/* 5685 */ 5690,
/* 5686 */ 50299,
/* 5687 */ 50059,
/* 5688 */ -5639,
/* 5689 */ -1,
/* 5690 */ 5695,
/* 5691 */ 50311,
/* 5692 */ 50059,
/* 5693 */ -5639,
/* 5694 */ -1,
/* 5695 */ 5705,
/* 5696 */ 50328,
/* 5697 */ -1,
/* 5698 */ 50464,
/* 5699 */ 50040,
/* 5700 */ -1,
/* 5701 */ 50041,
/* 5702 */ 50059,
/* 5703 */ -5639,
/* 5704 */ -1,
/* 5705 */ 5713,
/* 5706 */ 50464,
/* 5707 */ 50040,
/* 5708 */ -1,
/* 5709 */ 50041,
/* 5710 */ -1,
/* 5711 */ -5639,
/* 5712 */ -1,
/* 5713 */ 5717,
/* 5714 */ -1,
/* 5715 */ -5639,
/* 5716 */ -1,
/* 5717 */ 5723,
/* 5718 */ 50420,
/* 5719 */ -1,
/* 5720 */ 50059,
/* 5721 */ -5639,
/* 5722 */ -1,
/* 5723 */ 0,
/* 5724 */ 50420,
/* 5725 */ 50059,
/* 5726 */ -5639,
/* 5727 */ -1,
/* 5728 */ 5736,
/* 5729 */ 50302,
/* 5730 */ -1,
/* 5731 */ 50058,
/* 5732 */ -1,
/* 5733 */ -1,
/* 5734 */ -5728,
/* 5735 */ -1,
/* 5736 */ 0,
/* 5737 */ -5728,
/* 5738 */ -1,
/* 5739 */ 5743,
/* 5740 */ -1,
/* 5741 */ -5739,
/* 5742 */ -1,
/* 5743 */ 0,
/* 5744 */ -1,
/* 5745 */ -5739,
/* 5746 */ -1,
/* 5747 */ 5752,
/* 5748 */ -1,
/* 5749 */ -1,
/* 5750 */ -5747,
/* 5751 */ -1,
/* 5752 */ 0,
/* 5753 */ -5747,
/* 5754 */ -1,
/* 5755 */ 5761,
/* 5756 */ 50321,
/* 5757 */ 50058,
/* 5758 */ -1,
/* 5759 */ -5755,
/* 5760 */ -1,
/* 5761 */ 0,
/* 5762 */ -5755,
/* 5763 */ -1,
/* 5764 */ 5769,
/* 5765 */ -1,
/* 5766 */ -1,
/* 5767 */ -5764,
/* 5768 */ -1,
/* 5769 */ 0,
/* 5770 */ -5764,
/* 5771 */ -1,
/* 5772 */ 0,
/* 5773 */ 50123,
/* 5774 */ -1,
/* 5775 */ 50125,
/* 5776 */ -5772,
/* 5777 */ -1,
/* 5778 */ 5783,
/* 5779 */ -1,
/* 5780 */ -1,
/* 5781 */ -5778,
/* 5782 */ -1,
/* 5783 */ 0,
/* 5784 */ -5778,
/* 5785 */ -1,
/* 5786 */ 0,
/* 5787 */ 50123,
/* 5788 */ -1,
/* 5789 */ -1,
/* 5790 */ 50125,
/* 5791 */ -5786,
/* 5792 */ -1,
/* 5793 */ 5798,
/* 5794 */ -1,
/* 5795 */ -1,
/* 5796 */ -5793,
/* 5797 */ -1,
/* 5798 */ 5804,
/* 5799 */ 50436,
/* 5800 */ -1,
/* 5801 */ -1,
/* 5802 */ -5793,
/* 5803 */ -1,
/* 5804 */ 0,
/* 5805 */ -5793,
/* 5806 */ -1,
/* 5807 */ 5812,
/* 5808 */ -1,
/* 5809 */ -1,
/* 5810 */ -5807,
/* 5811 */ -1,
/* 5812 */ 0,
/* 5813 */ -5807,
/* 5814 */ -1,
/* 5815 */ 5819,
/* 5816 */ 50256,
/* 5817 */ -5815,
/* 5818 */ -1,
/* 5819 */ 5823,
/* 5820 */ 50259,
/* 5821 */ -5815,
/* 5822 */ -1,
/* 5823 */ 5827,
/* 5824 */ 50257,
/* 5825 */ -5815,
/* 5826 */ -1,
/* 5827 */ 5831,
/* 5828 */ 50453,
/* 5829 */ -5815,
/* 5830 */ -1,
/* 5831 */ 5835,
/* 5832 */ 50341,
/* 5833 */ -5815,
/* 5834 */ -1,
/* 5835 */ 5839,
/* 5836 */ 50391,
/* 5837 */ -5815,
/* 5838 */ -1,
/* 5839 */ 5843,
/* 5840 */ -1,
/* 5841 */ -5815,
/* 5842 */ -1,
/* 5843 */ 5849,
/* 5844 */ 50040,
/* 5845 */ -1,
/* 5846 */ 50041,
/* 5847 */ -5815,
/* 5848 */ -1,
/* 5849 */ 5853,
/* 5850 */ 50258,
/* 5851 */ -5815,
/* 5852 */ -1,
/* 5853 */ 5859,
/* 5854 */ -1,
/* 5855 */ -1,
/* 5856 */ 50258,
/* 5857 */ -5815,
/* 5858 */ -1,
/* 5859 */ 5868,
/* 5860 */ -1,
/* 5861 */ -1,
/* 5862 */ 50258,
/* 5863 */ 50040,
/* 5864 */ -1,
/* 5865 */ 50041,
/* 5866 */ -5815,
/* 5867 */ -1,
/* 5868 */ 5875,
/* 5869 */ 50258,
/* 5870 */ 50040,
/* 5871 */ -1,
/* 5872 */ 50041,
/* 5873 */ -5815,
/* 5874 */ -1,
/* 5875 */ 0,
/* 5876 */ -1,
/* 5877 */ 50064,
/* 5878 */ 50258,
/* 5879 */ -5815,
/* 5880 */ -1,
/* 5881 */ 5885,
/* 5882 */ 50046,
/* 5883 */ -5881,
/* 5884 */ -1,
/* 5885 */ 0,
/* 5886 */ 50491,
/* 5887 */ -5881,
/* 5888 */ -1,
/* 5889 */ 5893,
/* 5890 */ 50046,
/* 5891 */ -5889,
/* 5892 */ -1,
/* 5893 */ 0,
/* 5894 */ 50491,
/* 5895 */ -5889,
/* 5896 */ -1,
/* 5897 */ 5901,
/* 5898 */ 50258,
/* 5899 */ -5897,
/* 5900 */ -1,
/* 5901 */ 5907,
/* 5902 */ -1,
/* 5903 */ 50064,
/* 5904 */ 50258,
/* 5905 */ -5897,
/* 5906 */ -1,
/* 5907 */ 0,
/* 5908 */ -1,
/* 5909 */ -1,
/* 5910 */ 50258,
/* 5911 */ -5897,
/* 5912 */ -1,
/* 5913 */ 5917,
/* 5914 */ 50046,
/* 5915 */ -5913,
/* 5916 */ -1,
/* 5917 */ 0,
/* 5918 */ 50491,
/* 5919 */ -5913,
/* 5920 */ -1,
/* 5921 */ 5926,
/* 5922 */ -1,
/* 5923 */ 50470,
/* 5924 */ -5921,
/* 5925 */ -1,
/* 5926 */ 5931,
/* 5927 */ -1,
/* 5928 */ 50471,
/* 5929 */ -5921,
/* 5930 */ -1,
/* 5931 */ 5940,
/* 5932 */ 50383,
/* 5933 */ 50040,
/* 5934 */ -1,
/* 5935 */ 50044,
/* 5936 */ -1,
/* 5937 */ 50041,
/* 5938 */ -5921,
/* 5939 */ -1,
/* 5940 */ 5949,
/* 5941 */ 50380,
/* 5942 */ 50040,
/* 5943 */ -1,
/* 5944 */ 50044,
/* 5945 */ -1,
/* 5946 */ 50041,
/* 5947 */ -5921,
/* 5948 */ -1,
/* 5949 */ 0,
/* 5950 */ -1,
/* 5951 */ -5921,
/* 5952 */ -1,
/* 5953 */ 5958,
/* 5954 */ 50470,
/* 5955 */ -1,
/* 5956 */ -5953,
/* 5957 */ -1,
/* 5958 */ 5963,
/* 5959 */ 50471,
/* 5960 */ -1,
/* 5961 */ -5953,
/* 5962 */ -1,
/* 5963 */ 5968,
/* 5964 */ 50043,
/* 5965 */ -1,
/* 5966 */ -5953,
/* 5967 */ -1,
/* 5968 */ 5973,
/* 5969 */ 50045,
/* 5970 */ -1,
/* 5971 */ -5953,
/* 5972 */ -1,
/* 5973 */ 5978,
/* 5974 */ 50033,
/* 5975 */ -1,
/* 5976 */ -5953,
/* 5977 */ -1,
/* 5978 */ 5983,
/* 5979 */ 50126,
/* 5980 */ -1,
/* 5981 */ -5953,
/* 5982 */ -1,
/* 5983 */ 5989,
/* 5984 */ 50058,
/* 5985 */ -1,
/* 5986 */ 50058,
/* 5987 */ -5953,
/* 5988 */ -1,
/* 5989 */ 0,
/* 5990 */ -1,
/* 5991 */ -5953,
/* 5992 */ -1,
/* 5993 */ 5999,
/* 5994 */ -1,
/* 5995 */ 50100,
/* 5996 */ -1,
/* 5997 */ -5993,
/* 5998 */ -1,
/* 5999 */ 0,
/* 6000 */ -1,
/* 6001 */ -5993,
/* 6002 */ -1,
/* 6003 */ 6010,
/* 6004 */ 50040,
/* 6005 */ -1,
/* 6006 */ 50041,
/* 6007 */ -1,
/* 6008 */ -6003,
/* 6009 */ -1,
/* 6010 */ 0,
/* 6011 */ -1,
/* 6012 */ -6003,
/* 6013 */ -1,
/* 6014 */ 6020,
/* 6015 */ -1,
/* 6016 */ 50042,
/* 6017 */ -1,
/* 6018 */ -6014,
/* 6019 */ -1,
/* 6020 */ 6026,
/* 6021 */ -1,
/* 6022 */ 50047,
/* 6023 */ -1,
/* 6024 */ -6014,
/* 6025 */ -1,
/* 6026 */ 6032,
/* 6027 */ -1,
/* 6028 */ 50037,
/* 6029 */ -1,
/* 6030 */ -6014,
/* 6031 */ -1,
/* 6032 */ 0,
/* 6033 */ -1,
/* 6034 */ -6014,
/* 6035 */ -1,
/* 6036 */ 6042,
/* 6037 */ -1,
/* 6038 */ 50043,
/* 6039 */ -1,
/* 6040 */ -6036,
/* 6041 */ -1,
/* 6042 */ 6048,
/* 6043 */ -1,
/* 6044 */ 50045,
/* 6045 */ -1,
/* 6046 */ -6036,
/* 6047 */ -1,
/* 6048 */ 0,
/* 6049 */ -1,
/* 6050 */ -6036,
/* 6051 */ -1,
/* 6052 */ 6058,
/* 6053 */ -1,
/* 6054 */ 50472,
/* 6055 */ -1,
/* 6056 */ -6052,
/* 6057 */ -1,
/* 6058 */ 6064,
/* 6059 */ -1,
/* 6060 */ 50473,
/* 6061 */ -1,
/* 6062 */ -6052,
/* 6063 */ -1,
/* 6064 */ 0,
/* 6065 */ -1,
/* 6066 */ -6052,
/* 6067 */ -1,
/* 6068 */ 6074,
/* 6069 */ -1,
/* 6070 */ 50060,
/* 6071 */ -1,
/* 6072 */ -6068,
/* 6073 */ -1,
/* 6074 */ 6080,
/* 6075 */ -1,
/* 6076 */ 50062,
/* 6077 */ -1,
/* 6078 */ -6068,
/* 6079 */ -1,
/* 6080 */ 6086,
/* 6081 */ -1,
/* 6082 */ 50474,
/* 6083 */ -1,
/* 6084 */ -6068,
/* 6085 */ -1,
/* 6086 */ 6092,
/* 6087 */ -1,
/* 6088 */ 50475,
/* 6089 */ -1,
/* 6090 */ -6068,
/* 6091 */ -1,
/* 6092 */ 6098,
/* 6093 */ -1,
/* 6094 */ 50476,
/* 6095 */ -1,
/* 6096 */ -6068,
/* 6097 */ -1,
/* 6098 */ 6104,
/* 6099 */ -1,
/* 6100 */ 50477,
/* 6101 */ -1,
/* 6102 */ -6068,
/* 6103 */ -1,
/* 6104 */ 0,
/* 6105 */ -1,
/* 6106 */ -6068,
/* 6107 */ -1,
/* 6108 */ 6114,
/* 6109 */ -1,
/* 6110 */ 50038,
/* 6111 */ -1,
/* 6112 */ -6108,
/* 6113 */ -1,
/* 6114 */ 0,
/* 6115 */ -1,
/* 6116 */ -6108,
/* 6117 */ -1,
/* 6118 */ 6124,
/* 6119 */ -1,
/* 6120 */ 50094,
/* 6121 */ -1,
/* 6122 */ -6118,
/* 6123 */ -1,
/* 6124 */ 0,
/* 6125 */ -1,
/* 6126 */ -6118,
/* 6127 */ -1,
/* 6128 */ 6134,
/* 6129 */ -1,
/* 6130 */ 50124,
/* 6131 */ -1,
/* 6132 */ -6128,
/* 6133 */ -1,
/* 6134 */ 0,
/* 6135 */ -1,
/* 6136 */ -6128,
/* 6137 */ -1,
/* 6138 */ 6144,
/* 6139 */ -1,
/* 6140 */ 50478,
/* 6141 */ -1,
/* 6142 */ -6138,
/* 6143 */ -1,
/* 6144 */ 0,
/* 6145 */ -1,
/* 6146 */ -6138,
/* 6147 */ -1,
/* 6148 */ 6154,
/* 6149 */ -1,
/* 6150 */ 50479,
/* 6151 */ -1,
/* 6152 */ -6148,
/* 6153 */ -1,
/* 6154 */ 0,
/* 6155 */ -1,
/* 6156 */ -6148,
/* 6157 */ -1,
/* 6158 */ 6166,
/* 6159 */ -1,
/* 6160 */ 50063,
/* 6161 */ -1,
/* 6162 */ 50058,
/* 6163 */ -1,
/* 6164 */ -6158,
/* 6165 */ -1,
/* 6166 */ 0,
/* 6167 */ -1,
/* 6168 */ -6158,
/* 6169 */ -1,
/* 6170 */ 6176,
/* 6171 */ -1,
/* 6172 */ 50061,
/* 6173 */ -1,
/* 6174 */ -6170,
/* 6175 */ -1,
/* 6176 */ 6182,
/* 6177 */ -1,
/* 6178 */ 50481,
/* 6179 */ -1,
/* 6180 */ -6170,
/* 6181 */ -1,
/* 6182 */ 6188,
/* 6183 */ -1,
/* 6184 */ 50489,
/* 6185 */ -1,
/* 6186 */ -6170,
/* 6187 */ -1,
/* 6188 */ 6194,
/* 6189 */ -1,
/* 6190 */ 50480,
/* 6191 */ -1,
/* 6192 */ -6170,
/* 6193 */ -1,
/* 6194 */ 6200,
/* 6195 */ -1,
/* 6196 */ 50482,
/* 6197 */ -1,
/* 6198 */ -6170,
/* 6199 */ -1,
/* 6200 */ 6206,
/* 6201 */ -1,
/* 6202 */ 50483,
/* 6203 */ -1,
/* 6204 */ -6170,
/* 6205 */ -1,
/* 6206 */ 6212,
/* 6207 */ -1,
/* 6208 */ 50484,
/* 6209 */ -1,
/* 6210 */ -6170,
/* 6211 */ -1,
/* 6212 */ 6218,
/* 6213 */ -1,
/* 6214 */ 50485,
/* 6215 */ -1,
/* 6216 */ -6170,
/* 6217 */ -1,
/* 6218 */ 6224,
/* 6219 */ -1,
/* 6220 */ 50487,
/* 6221 */ -1,
/* 6222 */ -6170,
/* 6223 */ -1,
/* 6224 */ 6230,
/* 6225 */ -1,
/* 6226 */ 50488,
/* 6227 */ -1,
/* 6228 */ -6170,
/* 6229 */ -1,
/* 6230 */ 6236,
/* 6231 */ -1,
/* 6232 */ 50486,
/* 6233 */ -1,
/* 6234 */ -6170,
/* 6235 */ -1,
/* 6236 */ 0,
/* 6237 */ -1,
/* 6238 */ -6170,
/* 6239 */ -1,
/* 6240 */ 6246,
/* 6241 */ -1,
/* 6242 */ 50044,
/* 6243 */ -1,
/* 6244 */ -6240,
/* 6245 */ -1,
/* 6246 */ 0,
/* 6247 */ -1,
/* 6248 */ -6240,
/* 6249 */ -1,
/* 6250 */ 0,
/* 6251 */ -1,
/* 6252 */ -6250,
/* 6253 */ -1,
/* 6254 */ 6259,
/* 6255 */ -1,
/* 6256 */ -1,
/* 6257 */ -6254,
/* 6258 */ -1,
/* 6259 */ 0,
/* 6260 */ -6254,
/* 6261 */ -1,
/* 6262 */ 6268,
/* 6263 */ 50044,
/* 6264 */ -1,
/* 6265 */ -1,
/* 6266 */ -6262,
/* 6267 */ -1,
/* 6268 */ 0,
/* 6269 */ -6262,
/* 6270 */ -1,
0
};
extern int yycoordinate[];
int yycoordinate[] = {
0,
/* 1 */ 9999,
/* 2 */ 184005,
/* 3 */ 9999,
/* 4 */ 9999,
/* 5 */ 184005,
/* 6 */ 9999,
/* 7 */ 186003,
/* 8 */ 9999,
/* 9 */ 185003,
/* 10 */ 9999,
/* 11 */ 186004,
/* 12 */ 186011,
/* 13 */ 9999,
/* 14 */ 186011,
/* 15 */ 9999,
/* 16 */ 9999,
/* 17 */ 186011,
/* 18 */ 9999,
/* 19 */ 9999,
/* 20 */ 9999,
/* 21 */ 9999,
/* 22 */ 9999,
/* 23 */ 189010,
/* 24 */ 9999,
/* 25 */ 9999,
/* 26 */ 190010,
/* 27 */ 9999,
/* 28 */ 9999,
/* 29 */ 190008,
/* 30 */ 9999,
/* 31 */ 9999,
/* 32 */ 9999,
/* 33 */ 9999,
/* 34 */ 9999,
/* 35 */ 191013,
/* 36 */ 9999,
/* 37 */ 192005,
/* 38 */ 9999,
/* 39 */ 192015,
/* 40 */ 9999,
/* 41 */ 193005,
/* 42 */ 9999,
/* 43 */ 193012,
/* 44 */ 9999,
/* 45 */ 194005,
/* 46 */ 9999,
/* 47 */ 194015,
/* 48 */ 9999,
/* 49 */ 195005,
/* 50 */ 9999,
/* 51 */ 195016,
/* 52 */ 9999,
/* 53 */ 196005,
/* 54 */ 9999,
/* 55 */ 196015,
/* 56 */ 9999,
/* 57 */ 197005,
/* 58 */ 9999,
/* 59 */ 197013,
/* 60 */ 9999,
/* 61 */ 198005,
/* 62 */ 9999,
/* 63 */ 198014,
/* 64 */ 9999,
/* 65 */ 199005,
/* 66 */ 9999,
/* 67 */ 199012,
/* 68 */ 9999,
/* 69 */ 200005,
/* 70 */ 9999,
/* 71 */ 200013,
/* 72 */ 9999,
/* 73 */ 201005,
/* 74 */ 9999,
/* 75 */ 201011,
/* 76 */ 9999,
/* 77 */ 202005,
/* 78 */ 9999,
/* 79 */ 202014,
/* 80 */ 9999,
/* 81 */ 203005,
/* 82 */ 9999,
/* 83 */ 203014,
/* 84 */ 9999,
/* 85 */ 204005,
/* 86 */ 9999,
/* 87 */ 204015,
/* 88 */ 9999,
/* 89 */ 205005,
/* 90 */ 9999,
/* 91 */ 205016,
/* 92 */ 9999,
/* 93 */ 206005,
/* 94 */ 9999,
/* 95 */ 206014,
/* 96 */ 9999,
/* 97 */ 207005,
/* 98 */ 9999,
/* 99 */ 207013,
/* 100 */ 9999,
/* 101 */ 208005,
/* 102 */ 9999,
/* 103 */ 208015,
/* 104 */ 9999,
/* 105 */ 209005,
/* 106 */ 9999,
/* 107 */ 209016,
/* 108 */ 9999,
/* 109 */ 210005,
/* 110 */ 9999,
/* 111 */ 210017,
/* 112 */ 9999,
/* 113 */ 211005,
/* 114 */ 9999,
/* 115 */ 211017,
/* 116 */ 9999,
/* 117 */ 212005,
/* 118 */ 9999,
/* 119 */ 212017,
/* 120 */ 9999,
/* 121 */ 9999,
/* 122 */ 213011,
/* 123 */ 213017,
/* 124 */ 9999,
/* 125 */ 213009,
/* 126 */ 9999,
/* 127 */ 9999,
/* 128 */ 9999,
/* 129 */ 9999,
/* 130 */ 214027,
/* 131 */ 9999,
/* 132 */ 214058,
/* 133 */ 9999,
/* 134 */ 9999,
/* 135 */ 214008,
/* 136 */ 9999,
/* 137 */ 9999,
/* 138 */ 9999,
/* 139 */ 213012,
/* 140 */ 9999,
/* 141 */ 9999,
/* 142 */ 9999,
/* 143 */ 9999,
/* 144 */ 214036,
/* 145 */ 9999,
/* 146 */ 9999,
/* 147 */ 214036,
/* 148 */ 9999,
/* 149 */ 9999,
/* 150 */ 9999,
/* 151 */ 214059,
/* 152 */ 9999,
/* 153 */ 9999,
/* 154 */ 214059,
/* 155 */ 9999,
/* 156 */ 9999,
/* 157 */ 9999,
/* 158 */ 9999,
/* 159 */ 219005,
/* 160 */ 9999,
/* 161 */ 9999,
/* 162 */ 228009,
/* 163 */ 9999,
/* 164 */ 228005,
/* 165 */ 9999,
/* 166 */ 232005,
/* 167 */ 9999,
/* 168 */ 232018,
/* 169 */ 9999,
/* 170 */ 232009,
/* 171 */ 9999,
/* 172 */ 233005,
/* 173 */ 9999,
/* 174 */ 233018,
/* 175 */ 9999,
/* 176 */ 233009,
/* 177 */ 9999,
/* 178 */ 234005,
/* 179 */ 9999,
/* 180 */ 234010,
/* 181 */ 9999,
/* 182 */ 238005,
/* 183 */ 9999,
/* 184 */ 238019,
/* 185 */ 9999,
/* 186 */ 238010,
/* 187 */ 9999,
/* 188 */ 239005,
/* 189 */ 9999,
/* 190 */ 239019,
/* 191 */ 9999,
/* 192 */ 239010,
/* 193 */ 9999,
/* 194 */ 240005,
/* 195 */ 9999,
/* 196 */ 240019,
/* 197 */ 9999,
/* 198 */ 240010,
/* 199 */ 9999,
/* 200 */ 241005,
/* 201 */ 9999,
/* 202 */ 241019,
/* 203 */ 9999,
/* 204 */ 241010,
/* 205 */ 9999,
/* 206 */ 242005,
/* 207 */ 9999,
/* 208 */ 242019,
/* 209 */ 9999,
/* 210 */ 242010,
/* 211 */ 9999,
/* 212 */ 244005,
/* 213 */ 9999,
/* 214 */ 244010,
/* 215 */ 9999,
/* 216 */ 9999,
/* 217 */ 248009,
/* 218 */ 9999,
/* 219 */ 248005,
/* 220 */ 9999,
/* 221 */ 9999,
/* 222 */ 249009,
/* 223 */ 9999,
/* 224 */ 249005,
/* 225 */ 9999,
/* 226 */ 250005,
/* 227 */ 9999,
/* 228 */ 250010,
/* 229 */ 9999,
/* 230 */ 9999,
/* 231 */ 254009,
/* 232 */ 9999,
/* 233 */ 9999,
/* 234 */ 254005,
/* 235 */ 9999,
/* 236 */ 9999,
/* 237 */ 9999,
/* 238 */ 255010,
/* 239 */ 9999,
/* 240 */ 9999,
/* 241 */ 9999,
/* 242 */ 256013,
/* 243 */ 9999,
/* 244 */ 9999,
/* 245 */ 9999,
/* 246 */ 257014,
/* 247 */ 9999,
/* 248 */ 9999,
/* 249 */ 9999,
/* 250 */ 258014,
/* 251 */ 9999,
/* 252 */ 262005,
/* 253 */ 9999,
/* 254 */ 262021,
/* 255 */ 9999,
/* 256 */ 262037,
/* 257 */ 9999,
/* 258 */ 262010,
/* 259 */ 9999,
/* 260 */ 264005,
/* 261 */ 9999,
/* 262 */ 264021,
/* 263 */ 9999,
/* 264 */ 9999,
/* 265 */ 264041,
/* 266 */ 9999,
/* 267 */ 264010,
/* 268 */ 9999,
/* 269 */ 266005,
/* 270 */ 9999,
/* 271 */ 266021,
/* 272 */ 9999,
/* 273 */ 266010,
/* 274 */ 9999,
/* 275 */ 268005,
/* 276 */ 9999,
/* 277 */ 268010,
/* 278 */ 9999,
/* 279 */ 273005,
/* 280 */ 9999,
/* 281 */ 273021,
/* 282 */ 9999,
/* 283 */ 273037,
/* 284 */ 9999,
/* 285 */ 273010,
/* 286 */ 9999,
/* 287 */ 275005,
/* 288 */ 9999,
/* 289 */ 275021,
/* 290 */ 9999,
/* 291 */ 275010,
/* 292 */ 9999,
/* 293 */ 284003,
/* 294 */ 285003,
/* 295 */ 286003,
/* 296 */ 287003,
/* 297 */ 289003,
/* 298 */ 291003,
/* 299 */ 292003,
/* 300 */ 9999,
/* 301 */ 281003,
/* 302 */ 9999,
/* 303 */ 9999,
/* 304 */ 9999,
/* 305 */ 284011,
/* 306 */ 9999,
/* 307 */ 9999,
/* 308 */ 284005,
/* 309 */ 9999,
/* 310 */ 9999,
/* 311 */ 284005,
/* 312 */ 9999,
/* 313 */ 9999,
/* 314 */ 9999,
/* 315 */ 9999,
/* 316 */ 285009,
/* 317 */ 9999,
/* 318 */ 9999,
/* 319 */ 285009,
/* 320 */ 9999,
/* 321 */ 286004,
/* 322 */ 9999,
/* 323 */ 286012,
/* 324 */ 9999,
/* 325 */ 9999,
/* 326 */ 286012,
/* 327 */ 9999,
/* 328 */ 9999,
/* 329 */ 9999,
/* 330 */ 287009,
/* 331 */ 9999,
/* 332 */ 9999,
/* 333 */ 9999,
/* 334 */ 288012,
/* 335 */ 9999,
/* 336 */ 9999,
/* 337 */ 288012,
/* 338 */ 9999,
/* 339 */ 289004,
/* 340 */ 9999,
/* 341 */ 289010,
/* 342 */ 9999,
/* 343 */ 9999,
/* 344 */ 290013,
/* 345 */ 9999,
/* 346 */ 290011,
/* 347 */ 9999,
/* 348 */ 9999,
/* 349 */ 9999,
/* 350 */ 9999,
/* 351 */ 291005,
/* 352 */ 9999,
/* 353 */ 9999,
/* 354 */ 9999,
/* 355 */ 9999,
/* 356 */ 291036,
/* 357 */ 9999,
/* 358 */ 9999,
/* 359 */ 291036,
/* 360 */ 9999,
/* 361 */ 9999,
/* 362 */ 9999,
/* 363 */ 292013,
/* 364 */ 9999,
/* 365 */ 292007,
/* 366 */ 9999,
/* 367 */ 292007,
/* 368 */ 9999,
/* 369 */ 9999,
/* 370 */ 296009,
/* 371 */ 296007,
/* 372 */ 9999,
/* 373 */ 296007,
/* 374 */ 9999,
/* 375 */ 9999,
/* 376 */ 9999,
/* 377 */ 297016,
/* 378 */ 9999,
/* 379 */ 297029,
/* 380 */ 297007,
/* 381 */ 9999,
/* 382 */ 297007,
/* 383 */ 9999,
/* 384 */ 9999,
/* 385 */ 9999,
/* 386 */ 299016,
/* 387 */ 299007,
/* 388 */ 9999,
/* 389 */ 299007,
/* 390 */ 9999,
/* 391 */ 9999,
/* 392 */ 9999,
/* 393 */ 301007,
/* 394 */ 9999,
/* 395 */ 301007,
/* 396 */ 9999,
/* 397 */ 9999,
/* 398 */ 9999,
/* 399 */ 303013,
/* 400 */ 9999,
/* 401 */ 303027,
/* 402 */ 9999,
/* 403 */ 303007,
/* 404 */ 9999,
/* 405 */ 303007,
/* 406 */ 9999,
/* 407 */ 9999,
/* 408 */ 303007,
/* 409 */ 9999,
/* 410 */ 292014,
/* 411 */ 292024,
/* 412 */ 292018,
/* 413 */ 9999,
/* 414 */ 292018,
/* 415 */ 9999,
/* 416 */ 9999,
/* 417 */ 292018,
/* 418 */ 9999,
/* 419 */ 9999,
/* 420 */ 9999,
/* 421 */ 292025,
/* 422 */ 9999,
/* 423 */ 9999,
/* 424 */ 292025,
/* 425 */ 9999,
/* 426 */ 9999,
/* 427 */ 309008,
/* 428 */ 310005,
/* 429 */ 313006,
/* 430 */ 9999,
/* 431 */ 9999,
/* 432 */ 308004,
/* 433 */ 9999,
/* 434 */ 9999,
/* 435 */ 9999,
/* 436 */ 309009,
/* 437 */ 9999,
/* 438 */ 310007,
/* 439 */ 312007,
/* 440 */ 310016,
/* 441 */ 9999,
/* 442 */ 310016,
/* 443 */ 9999,
/* 444 */ 9999,
/* 445 */ 310016,
/* 446 */ 9999,
/* 447 */ 9999,
/* 448 */ 9999,
/* 449 */ 312008,
/* 450 */ 9999,
/* 451 */ 9999,
/* 452 */ 9999,
/* 453 */ 312034,
/* 454 */ 9999,
/* 455 */ 318003,
/* 456 */ 318017,
/* 457 */ 319005,
/* 458 */ 320008,
/* 459 */ 323005,
/* 460 */ 9999,
/* 461 */ 9999,
/* 462 */ 318003,
/* 463 */ 9999,
/* 464 */ 9999,
/* 465 */ 9999,
/* 466 */ 318008,
/* 467 */ 9999,
/* 468 */ 9999,
/* 469 */ 9999,
/* 470 */ 318014,
/* 471 */ 9999,
/* 472 */ 9999,
/* 473 */ 9999,
/* 474 */ 318018,
/* 475 */ 9999,
/* 476 */ 9999,
/* 477 */ 318018,
/* 478 */ 9999,
/* 479 */ 319006,
/* 480 */ 9999,
/* 481 */ 319010,
/* 482 */ 9999,
/* 483 */ 9999,
/* 484 */ 319010,
/* 485 */ 9999,
/* 486 */ 9999,
/* 487 */ 9999,
/* 488 */ 320018,
/* 489 */ 9999,
/* 490 */ 9999,
/* 491 */ 9999,
/* 492 */ 321012,
/* 493 */ 9999,
/* 494 */ 9999,
/* 495 */ 323009,
/* 496 */ 9999,
/* 497 */ 323007,
/* 498 */ 9999,
/* 499 */ 9999,
/* 500 */ 323007,
/* 501 */ 9999,
/* 502 */ 328003,
/* 503 */ 329003,
/* 504 */ 331003,
/* 505 */ 9999,
/* 506 */ 327038,
/* 507 */ 9999,
/* 508 */ 328005,
/* 509 */ 9999,
/* 510 */ 328009,
/* 511 */ 9999,
/* 512 */ 9999,
/* 513 */ 328009,
/* 514 */ 9999,
/* 515 */ 9999,
/* 516 */ 9999,
/* 517 */ 329013,
/* 518 */ 9999,
/* 519 */ 9999,
/* 520 */ 9999,
/* 521 */ 330012,
/* 522 */ 9999,
/* 523 */ 9999,
/* 524 */ 331007,
/* 525 */ 9999,
/* 526 */ 331005,
/* 527 */ 9999,
/* 528 */ 9999,
/* 529 */ 331005,
/* 530 */ 9999,
/* 531 */ 9999,
/* 532 */ 9999,
/* 533 */ 335012,
/* 534 */ 335024,
/* 535 */ 9999,
/* 536 */ 9999,
/* 537 */ 335004,
/* 538 */ 9999,
/* 539 */ 9999,
/* 540 */ 335029,
/* 541 */ 335025,
/* 542 */ 9999,
/* 543 */ 335025,
/* 544 */ 9999,
/* 545 */ 9999,
/* 546 */ 335025,
/* 547 */ 9999,
/* 548 */ 340003,
/* 549 */ 341003,
/* 550 */ 342003,
/* 551 */ 9999,
/* 552 */ 339027,
/* 553 */ 9999,
/* 554 */ 9999,
/* 555 */ 341008,
/* 556 */ 341004,
/* 557 */ 9999,
/* 558 */ 341004,
/* 559 */ 9999,
/* 560 */ 9999,
/* 561 */ 341004,
/* 562 */ 9999,
/* 563 */ 9999,
/* 564 */ 344005,
/* 565 */ 9999,
/* 566 */ 342011,
/* 567 */ 9999,
/* 568 */ 349007,
/* 569 */ 9999,
/* 570 */ 349013,
/* 571 */ 9999,
/* 572 */ 9999,
/* 573 */ 9999,
/* 574 */ 344007,
/* 575 */ 9999,
/* 576 */ 344007,
/* 577 */ 9999,
/* 578 */ 9999,
/* 579 */ 344007,
/* 580 */ 9999,
/* 581 */ 359003,
/* 582 */ 9999,
/* 583 */ 358003,
/* 584 */ 9999,
/* 585 */ 367003,
/* 586 */ 371003,
/* 587 */ 9999,
/* 588 */ 366003,
/* 589 */ 9999,
/* 590 */ 367007,
/* 591 */ 367014,
/* 592 */ 9999,
/* 593 */ 367007,
/* 594 */ 9999,
/* 595 */ 9999,
/* 596 */ 368011,
/* 597 */ 9999,
/* 598 */ 368007,
/* 599 */ 9999,
/* 600 */ 9999,
/* 601 */ 369011,
/* 602 */ 9999,
/* 603 */ 369007,
/* 604 */ 9999,
/* 605 */ 370007,
/* 606 */ 9999,
/* 607 */ 9999,
/* 608 */ 370012,
/* 609 */ 9999,
/* 610 */ 9999,
/* 611 */ 9999,
/* 612 */ 367008,
/* 613 */ 9999,
/* 614 */ 9999,
/* 615 */ 367008,
/* 616 */ 9999,
/* 617 */ 9999,
/* 618 */ 371009,
/* 619 */ 9999,
/* 620 */ 9999,
/* 621 */ 371005,
/* 622 */ 9999,
/* 623 */ 9999,
/* 624 */ 371005,
/* 625 */ 9999,
/* 626 */ 9999,
/* 627 */ 372016,
/* 628 */ 9999,
/* 629 */ 372014,
/* 630 */ 9999,
/* 631 */ 9999,
/* 632 */ 373016,
/* 633 */ 9999,
/* 634 */ 373014,
/* 635 */ 9999,
/* 636 */ 9999,
/* 637 */ 374012,
/* 638 */ 9999,
/* 639 */ 374010,
/* 640 */ 9999,
/* 641 */ 9999,
/* 642 */ 9999,
/* 643 */ 9999,
/* 644 */ 384009,
/* 645 */ 385005,
/* 646 */ 9999,
/* 647 */ 387009,
/* 648 */ 9999,
/* 649 */ 9999,
/* 650 */ 379009,
/* 651 */ 9999,
/* 652 */ 9999,
/* 653 */ 385011,
/* 654 */ 386005,
/* 655 */ 9999,
/* 656 */ 385007,
/* 657 */ 9999,
/* 658 */ 9999,
/* 659 */ 385007,
/* 660 */ 9999,
/* 661 */ 9999,
/* 662 */ 386011,
/* 663 */ 9999,
/* 664 */ 386007,
/* 665 */ 9999,
/* 666 */ 9999,
/* 667 */ 386007,
/* 668 */ 9999,
/* 669 */ 387010,
/* 670 */ 387018,
/* 671 */ 9999,
/* 672 */ 387018,
/* 673 */ 9999,
/* 674 */ 9999,
/* 675 */ 387018,
/* 676 */ 9999,
/* 677 */ 9999,
/* 678 */ 391015,
/* 679 */ 9999,
/* 680 */ 9999,
/* 681 */ 9999,
/* 682 */ 391013,
/* 683 */ 9999,
/* 684 */ 9999,
/* 685 */ 392011,
/* 686 */ 392018,
/* 687 */ 9999,
/* 688 */ 9999,
/* 689 */ 392009,
/* 690 */ 9999,
/* 691 */ 393005,
/* 692 */ 393028,
/* 693 */ 393035,
/* 694 */ 9999,
/* 695 */ 9999,
/* 696 */ 393005,
/* 697 */ 9999,
/* 698 */ 394005,
/* 699 */ 394028,
/* 700 */ 394035,
/* 701 */ 9999,
/* 702 */ 394050,
/* 703 */ 9999,
/* 704 */ 9999,
/* 705 */ 394005,
/* 706 */ 9999,
/* 707 */ 9999,
/* 708 */ 398013,
/* 709 */ 398020,
/* 710 */ 9999,
/* 711 */ 9999,
/* 712 */ 398011,
/* 713 */ 9999,
/* 714 */ 9999,
/* 715 */ 399015,
/* 716 */ 399022,
/* 717 */ 9999,
/* 718 */ 9999,
/* 719 */ 399013,
/* 720 */ 9999,
/* 721 */ 401005,
/* 722 */ 9999,
/* 723 */ 401016,
/* 724 */ 401023,
/* 725 */ 9999,
/* 726 */ 9999,
/* 727 */ 401005,
/* 728 */ 9999,
/* 729 */ 402005,
/* 730 */ 9999,
/* 731 */ 402015,
/* 732 */ 9999,
/* 733 */ 9999,
/* 734 */ 9999,
/* 735 */ 403015,
/* 736 */ 9999,
/* 737 */ 9999,
/* 738 */ 9999,
/* 739 */ 403009,
/* 740 */ 9999,
/* 741 */ 9999,
/* 742 */ 9999,
/* 743 */ 405015,
/* 744 */ 9999,
/* 745 */ 405030,
/* 746 */ 9999,
/* 747 */ 9999,
/* 748 */ 9999,
/* 749 */ 405009,
/* 750 */ 9999,
/* 751 */ 9999,
/* 752 */ 9999,
/* 753 */ 407015,
/* 754 */ 9999,
/* 755 */ 407030,
/* 756 */ 9999,
/* 757 */ 407046,
/* 758 */ 9999,
/* 759 */ 9999,
/* 760 */ 9999,
/* 761 */ 407009,
/* 762 */ 9999,
/* 763 */ 9999,
/* 764 */ 9999,
/* 765 */ 410015,
/* 766 */ 9999,
/* 767 */ 410030,
/* 768 */ 9999,
/* 769 */ 410046,
/* 770 */ 9999,
/* 771 */ 410062,
/* 772 */ 9999,
/* 773 */ 9999,
/* 774 */ 9999,
/* 775 */ 410009,
/* 776 */ 9999,
/* 777 */ 9999,
/* 778 */ 414013,
/* 779 */ 414020,
/* 780 */ 9999,
/* 781 */ 9999,
/* 782 */ 414011,
/* 783 */ 9999,
/* 784 */ 9999,
/* 785 */ 415012,
/* 786 */ 415019,
/* 787 */ 9999,
/* 788 */ 9999,
/* 789 */ 415010,
/* 790 */ 9999,
/* 791 */ 9999,
/* 792 */ 416012,
/* 793 */ 416019,
/* 794 */ 416032,
/* 795 */ 9999,
/* 796 */ 9999,
/* 797 */ 416010,
/* 798 */ 9999,
/* 799 */ 9999,
/* 800 */ 9999,
/* 801 */ 9999,
/* 802 */ 9999,
/* 803 */ 417006,
/* 804 */ 9999,
/* 805 */ 9999,
/* 806 */ 423008,
/* 807 */ 9999,
/* 808 */ 9999,
/* 809 */ 423006,
/* 810 */ 9999,
/* 811 */ 9999,
/* 812 */ 426009,
/* 813 */ 426016,
/* 814 */ 9999,
/* 815 */ 9999,
/* 816 */ 426007,
/* 817 */ 9999,
/* 818 */ 9999,
/* 819 */ 427009,
/* 820 */ 427016,
/* 821 */ 9999,
/* 822 */ 9999,
/* 823 */ 427007,
/* 824 */ 9999,
/* 825 */ 9999,
/* 826 */ 428010,
/* 827 */ 428017,
/* 828 */ 9999,
/* 829 */ 9999,
/* 830 */ 428008,
/* 831 */ 9999,
/* 832 */ 9999,
/* 833 */ 429011,
/* 834 */ 429018,
/* 835 */ 433019,
/* 836 */ 9999,
/* 837 */ 9999,
/* 838 */ 429009,
/* 839 */ 9999,
/* 840 */ 9999,
/* 841 */ 437011,
/* 842 */ 437018,
/* 843 */ 9999,
/* 844 */ 9999,
/* 845 */ 437009,
/* 846 */ 9999,
/* 847 */ 438005,
/* 848 */ 438018,
/* 849 */ 9999,
/* 850 */ 9999,
/* 851 */ 438016,
/* 852 */ 9999,
/* 853 */ 9999,
/* 854 */ 439010,
/* 855 */ 439017,
/* 856 */ 9999,
/* 857 */ 9999,
/* 858 */ 439008,
/* 859 */ 9999,
/* 860 */ 9999,
/* 861 */ 440011,
/* 862 */ 440018,
/* 863 */ 440028,
/* 864 */ 9999,
/* 865 */ 9999,
/* 866 */ 440009,
/* 867 */ 9999,
/* 868 */ 9999,
/* 869 */ 441010,
/* 870 */ 441017,
/* 871 */ 441027,
/* 872 */ 9999,
/* 873 */ 9999,
/* 874 */ 441008,
/* 875 */ 9999,
/* 876 */ 9999,
/* 877 */ 442012,
/* 878 */ 442019,
/* 879 */ 9999,
/* 880 */ 9999,
/* 881 */ 442010,
/* 882 */ 9999,
/* 883 */ 9999,
/* 884 */ 445013,
/* 885 */ 445020,
/* 886 */ 9999,
/* 887 */ 9999,
/* 888 */ 445011,
/* 889 */ 9999,
/* 890 */ 9999,
/* 891 */ 447012,
/* 892 */ 447019,
/* 893 */ 9999,
/* 894 */ 9999,
/* 895 */ 447010,
/* 896 */ 9999,
/* 897 */ 449005,
/* 898 */ 9999,
/* 899 */ 449013,
/* 900 */ 9999,
/* 901 */ 450005,
/* 902 */ 9999,
/* 903 */ 450014,
/* 904 */ 9999,
/* 905 */ 451005,
/* 906 */ 9999,
/* 907 */ 451016,
/* 908 */ 9999,
/* 909 */ 452005,
/* 910 */ 9999,
/* 911 */ 452017,
/* 912 */ 9999,
/* 913 */ 9999,
/* 914 */ 9999,
/* 915 */ 391016,
/* 916 */ 9999,
/* 917 */ 9999,
/* 918 */ 391016,
/* 919 */ 9999,
/* 920 */ 9999,
/* 921 */ 9999,
/* 922 */ 392012,
/* 923 */ 9999,
/* 924 */ 9999,
/* 925 */ 392012,
/* 926 */ 9999,
/* 927 */ 9999,
/* 928 */ 9999,
/* 929 */ 393015,
/* 930 */ 9999,
/* 931 */ 9999,
/* 932 */ 9999,
/* 933 */ 393025,
/* 934 */ 9999,
/* 935 */ 9999,
/* 936 */ 9999,
/* 937 */ 393029,
/* 938 */ 9999,
/* 939 */ 9999,
/* 940 */ 393029,
/* 941 */ 9999,
/* 942 */ 9999,
/* 943 */ 9999,
/* 944 */ 394015,
/* 945 */ 9999,
/* 946 */ 9999,
/* 947 */ 9999,
/* 948 */ 394025,
/* 949 */ 9999,
/* 950 */ 9999,
/* 951 */ 9999,
/* 952 */ 394029,
/* 953 */ 9999,
/* 954 */ 9999,
/* 955 */ 394029,
/* 956 */ 9999,
/* 957 */ 9999,
/* 958 */ 9999,
/* 959 */ 398014,
/* 960 */ 9999,
/* 961 */ 9999,
/* 962 */ 398014,
/* 963 */ 9999,
/* 964 */ 9999,
/* 965 */ 9999,
/* 966 */ 399016,
/* 967 */ 9999,
/* 968 */ 9999,
/* 969 */ 399016,
/* 970 */ 9999,
/* 971 */ 9999,
/* 972 */ 9999,
/* 973 */ 401006,
/* 974 */ 9999,
/* 975 */ 9999,
/* 976 */ 401006,
/* 977 */ 9999,
/* 978 */ 9999,
/* 979 */ 9999,
/* 980 */ 401017,
/* 981 */ 9999,
/* 982 */ 9999,
/* 983 */ 401017,
/* 984 */ 9999,
/* 985 */ 9999,
/* 986 */ 9999,
/* 987 */ 414014,
/* 988 */ 9999,
/* 989 */ 9999,
/* 990 */ 414014,
/* 991 */ 9999,
/* 992 */ 414021,
/* 993 */ 414057,
/* 994 */ 414026,
/* 995 */ 9999,
/* 996 */ 414026,
/* 997 */ 9999,
/* 998 */ 9999,
/* 999 */ 414026,
/* 1000 */ 9999,
/* 1001 */ 9999,
/* 1002 */ 9999,
/* 1003 */ 414058,
/* 1004 */ 9999,
/* 1005 */ 9999,
/* 1006 */ 414058,
/* 1007 */ 9999,
/* 1008 */ 9999,
/* 1009 */ 9999,
/* 1010 */ 415013,
/* 1011 */ 9999,
/* 1012 */ 9999,
/* 1013 */ 415013,
/* 1014 */ 9999,
/* 1015 */ 415020,
/* 1016 */ 415056,
/* 1017 */ 415025,
/* 1018 */ 9999,
/* 1019 */ 415025,
/* 1020 */ 9999,
/* 1021 */ 9999,
/* 1022 */ 415025,
/* 1023 */ 9999,
/* 1024 */ 9999,
/* 1025 */ 9999,
/* 1026 */ 415057,
/* 1027 */ 9999,
/* 1028 */ 9999,
/* 1029 */ 415057,
/* 1030 */ 9999,
/* 1031 */ 9999,
/* 1032 */ 9999,
/* 1033 */ 416013,
/* 1034 */ 9999,
/* 1035 */ 9999,
/* 1036 */ 416013,
/* 1037 */ 9999,
/* 1038 */ 9999,
/* 1039 */ 416037,
/* 1040 */ 416033,
/* 1041 */ 9999,
/* 1042 */ 416033,
/* 1043 */ 9999,
/* 1044 */ 9999,
/* 1045 */ 416033,
/* 1046 */ 9999,
/* 1047 */ 9999,
/* 1048 */ 9999,
/* 1049 */ 426010,
/* 1050 */ 9999,
/* 1051 */ 9999,
/* 1052 */ 426010,
/* 1053 */ 9999,
/* 1054 */ 9999,
/* 1055 */ 9999,
/* 1056 */ 427010,
/* 1057 */ 9999,
/* 1058 */ 9999,
/* 1059 */ 427010,
/* 1060 */ 9999,
/* 1061 */ 9999,
/* 1062 */ 9999,
/* 1063 */ 428011,
/* 1064 */ 9999,
/* 1065 */ 9999,
/* 1066 */ 428011,
/* 1067 */ 9999,
/* 1068 */ 9999,
/* 1069 */ 9999,
/* 1070 */ 429012,
/* 1071 */ 9999,
/* 1072 */ 9999,
/* 1073 */ 429012,
/* 1074 */ 9999,
/* 1075 */ 429019,
/* 1076 */ 9999,
/* 1077 */ 429024,
/* 1078 */ 9999,
/* 1079 */ 429024,
/* 1080 */ 9999,
/* 1081 */ 9999,
/* 1082 */ 429024,
/* 1083 */ 9999,
/* 1084 */ 9999,
/* 1085 */ 9999,
/* 1086 */ 437012,
/* 1087 */ 9999,
/* 1088 */ 9999,
/* 1089 */ 437012,
/* 1090 */ 9999,
/* 1091 */ 437019,
/* 1092 */ 437029,
/* 1093 */ 9999,
/* 1094 */ 437029,
/* 1095 */ 9999,
/* 1096 */ 9999,
/* 1097 */ 437029,
/* 1098 */ 9999,
/* 1099 */ 9999,
/* 1100 */ 438023,
/* 1101 */ 438019,
/* 1102 */ 9999,
/* 1103 */ 438019,
/* 1104 */ 9999,
/* 1105 */ 9999,
/* 1106 */ 438019,
/* 1107 */ 9999,
/* 1108 */ 9999,
/* 1109 */ 9999,
/* 1110 */ 439011,
/* 1111 */ 9999,
/* 1112 */ 9999,
/* 1113 */ 439011,
/* 1114 */ 9999,
/* 1115 */ 9999,
/* 1116 */ 9999,
/* 1117 */ 440012,
/* 1118 */ 9999,
/* 1119 */ 9999,
/* 1120 */ 440012,
/* 1121 */ 9999,
/* 1122 */ 9999,
/* 1123 */ 9999,
/* 1124 */ 440029,
/* 1125 */ 9999,
/* 1126 */ 9999,
/* 1127 */ 440029,
/* 1128 */ 9999,
/* 1129 */ 9999,
/* 1130 */ 9999,
/* 1131 */ 441011,
/* 1132 */ 9999,
/* 1133 */ 9999,
/* 1134 */ 441011,
/* 1135 */ 9999,
/* 1136 */ 9999,
/* 1137 */ 9999,
/* 1138 */ 441028,
/* 1139 */ 9999,
/* 1140 */ 9999,
/* 1141 */ 441028,
/* 1142 */ 9999,
/* 1143 */ 9999,
/* 1144 */ 9999,
/* 1145 */ 442013,
/* 1146 */ 9999,
/* 1147 */ 9999,
/* 1148 */ 442013,
/* 1149 */ 9999,
/* 1150 */ 442021,
/* 1151 */ 444006,
/* 1152 */ 442027,
/* 1153 */ 9999,
/* 1154 */ 442027,
/* 1155 */ 9999,
/* 1156 */ 9999,
/* 1157 */ 442027,
/* 1158 */ 9999,
/* 1159 */ 9999,
/* 1160 */ 9999,
/* 1161 */ 444007,
/* 1162 */ 9999,
/* 1163 */ 9999,
/* 1164 */ 444007,
/* 1165 */ 9999,
/* 1166 */ 9999,
/* 1167 */ 9999,
/* 1168 */ 445014,
/* 1169 */ 9999,
/* 1170 */ 9999,
/* 1171 */ 445014,
/* 1172 */ 9999,
/* 1173 */ 445022,
/* 1174 */ 446006,
/* 1175 */ 445028,
/* 1176 */ 9999,
/* 1177 */ 445028,
/* 1178 */ 9999,
/* 1179 */ 9999,
/* 1180 */ 445028,
/* 1181 */ 9999,
/* 1182 */ 9999,
/* 1183 */ 9999,
/* 1184 */ 446007,
/* 1185 */ 9999,
/* 1186 */ 9999,
/* 1187 */ 446007,
/* 1188 */ 9999,
/* 1189 */ 9999,
/* 1190 */ 9999,
/* 1191 */ 447013,
/* 1192 */ 9999,
/* 1193 */ 9999,
/* 1194 */ 447013,
/* 1195 */ 9999,
/* 1196 */ 447021,
/* 1197 */ 448006,
/* 1198 */ 447027,
/* 1199 */ 9999,
/* 1200 */ 447027,
/* 1201 */ 9999,
/* 1202 */ 9999,
/* 1203 */ 447027,
/* 1204 */ 9999,
/* 1205 */ 9999,
/* 1206 */ 9999,
/* 1207 */ 448007,
/* 1208 */ 9999,
/* 1209 */ 9999,
/* 1210 */ 448007,
/* 1211 */ 9999,
/* 1212 */ 9999,
/* 1213 */ 9999,
/* 1214 */ 456007,
/* 1215 */ 9999,
/* 1216 */ 9999,
/* 1217 */ 9999,
/* 1218 */ 9999,
/* 1219 */ 457010,
/* 1220 */ 9999,
/* 1221 */ 9999,
/* 1222 */ 9999,
/* 1223 */ 9999,
/* 1224 */ 458009,
/* 1225 */ 9999,
/* 1226 */ 9999,
/* 1227 */ 9999,
/* 1228 */ 9999,
/* 1229 */ 459008,
/* 1230 */ 9999,
/* 1231 */ 465003,
/* 1232 */ 9999,
/* 1233 */ 466005,
/* 1234 */ 468003,
/* 1235 */ 470003,
/* 1236 */ 471005,
/* 1237 */ 482003,
/* 1238 */ 9999,
/* 1239 */ 463032,
/* 1240 */ 9999,
/* 1241 */ 494005,
/* 1242 */ 9999,
/* 1243 */ 494020,
/* 1244 */ 496019,
/* 1245 */ 9999,
/* 1246 */ 494010,
/* 1247 */ 9999,
/* 1248 */ 466007,
/* 1249 */ 9999,
/* 1250 */ 466015,
/* 1251 */ 9999,
/* 1252 */ 467009,
/* 1253 */ 9999,
/* 1254 */ 9999,
/* 1255 */ 467014,
/* 1256 */ 9999,
/* 1257 */ 9999,
/* 1258 */ 467014,
/* 1259 */ 9999,
/* 1260 */ 468004,
/* 1261 */ 9999,
/* 1262 */ 468009,
/* 1263 */ 9999,
/* 1264 */ 9999,
/* 1265 */ 468009,
/* 1266 */ 9999,
/* 1267 */ 9999,
/* 1268 */ 9999,
/* 1269 */ 470006,
/* 1270 */ 9999,
/* 1271 */ 9999,
/* 1272 */ 470006,
/* 1273 */ 9999,
/* 1274 */ 9999,
/* 1275 */ 471011,
/* 1276 */ 471040,
/* 1277 */ 9999,
/* 1278 */ 9999,
/* 1279 */ 471007,
/* 1280 */ 9999,
/* 1281 */ 9999,
/* 1282 */ 471007,
/* 1283 */ 9999,
/* 1284 */ 9999,
/* 1285 */ 9999,
/* 1286 */ 471018,
/* 1287 */ 9999,
/* 1288 */ 9999,
/* 1289 */ 9999,
/* 1290 */ 471026,
/* 1291 */ 9999,
/* 1292 */ 9999,
/* 1293 */ 9999,
/* 1294 */ 471037,
/* 1295 */ 9999,
/* 1296 */ 9999,
/* 1297 */ 483005,
/* 1298 */ 484005,
/* 1299 */ 485005,
/* 1300 */ 486005,
/* 1301 */ 482006,
/* 1302 */ 9999,
/* 1303 */ 482006,
/* 1304 */ 9999,
/* 1305 */ 9999,
/* 1306 */ 482006,
/* 1307 */ 9999,
/* 1308 */ 483007,
/* 1309 */ 9999,
/* 1310 */ 483015,
/* 1311 */ 9999,
/* 1312 */ 9999,
/* 1313 */ 483015,
/* 1314 */ 9999,
/* 1315 */ 484006,
/* 1316 */ 9999,
/* 1317 */ 484011,
/* 1318 */ 9999,
/* 1319 */ 9999,
/* 1320 */ 484011,
/* 1321 */ 9999,
/* 1322 */ 9999,
/* 1323 */ 9999,
/* 1324 */ 485008,
/* 1325 */ 9999,
/* 1326 */ 9999,
/* 1327 */ 485008,
/* 1328 */ 9999,
/* 1329 */ 9999,
/* 1330 */ 486011,
/* 1331 */ 486040,
/* 1332 */ 9999,
/* 1333 */ 9999,
/* 1334 */ 486007,
/* 1335 */ 9999,
/* 1336 */ 9999,
/* 1337 */ 486007,
/* 1338 */ 9999,
/* 1339 */ 9999,
/* 1340 */ 9999,
/* 1341 */ 486018,
/* 1342 */ 9999,
/* 1343 */ 9999,
/* 1344 */ 9999,
/* 1345 */ 486026,
/* 1346 */ 9999,
/* 1347 */ 9999,
/* 1348 */ 9999,
/* 1349 */ 486037,
/* 1350 */ 9999,
/* 1351 */ 496021,
/* 1352 */ 9999,
/* 1353 */ 496026,
/* 1354 */ 9999,
/* 1355 */ 9999,
/* 1356 */ 496026,
/* 1357 */ 9999,
/* 1358 */ 505003,
/* 1359 */ 505015,
/* 1360 */ 9999,
/* 1361 */ 505008,
/* 1362 */ 9999,
/* 1363 */ 9999,
/* 1364 */ 506009,
/* 1365 */ 506020,
/* 1366 */ 9999,
/* 1367 */ 506005,
/* 1368 */ 9999,
/* 1369 */ 9999,
/* 1370 */ 9999,
/* 1371 */ 505016,
/* 1372 */ 9999,
/* 1373 */ 9999,
/* 1374 */ 505016,
/* 1375 */ 9999,
/* 1376 */ 9999,
/* 1377 */ 9999,
/* 1378 */ 506021,
/* 1379 */ 9999,
/* 1380 */ 9999,
/* 1381 */ 506021,
/* 1382 */ 9999,
/* 1383 */ 9999,
/* 1384 */ 510017,
/* 1385 */ 510024,
/* 1386 */ 9999,
/* 1387 */ 510011,
/* 1388 */ 9999,
/* 1389 */ 9999,
/* 1390 */ 9999,
/* 1391 */ 510018,
/* 1392 */ 9999,
/* 1393 */ 9999,
/* 1394 */ 510018,
/* 1395 */ 9999,
/* 1396 */ 9999,
/* 1397 */ 9999,
/* 1398 */ 9999,
/* 1399 */ 519009,
/* 1400 */ 9999,
/* 1401 */ 520009,
/* 1402 */ 9999,
/* 1403 */ 9999,
/* 1404 */ 514006,
/* 1405 */ 9999,
/* 1406 */ 520010,
/* 1407 */ 520019,
/* 1408 */ 9999,
/* 1409 */ 520019,
/* 1410 */ 9999,
/* 1411 */ 9999,
/* 1412 */ 520019,
/* 1413 */ 9999,
/* 1414 */ 9999,
/* 1415 */ 524015,
/* 1416 */ 9999,
/* 1417 */ 9999,
/* 1418 */ 9999,
/* 1419 */ 524013,
/* 1420 */ 9999,
/* 1421 */ 9999,
/* 1422 */ 525017,
/* 1423 */ 525024,
/* 1424 */ 9999,
/* 1425 */ 9999,
/* 1426 */ 525015,
/* 1427 */ 9999,
/* 1428 */ 526005,
/* 1429 */ 9999,
/* 1430 */ 526015,
/* 1431 */ 9999,
/* 1432 */ 9999,
/* 1433 */ 527010,
/* 1434 */ 527017,
/* 1435 */ 9999,
/* 1436 */ 9999,
/* 1437 */ 527008,
/* 1438 */ 9999,
/* 1439 */ 9999,
/* 1440 */ 528010,
/* 1441 */ 528017,
/* 1442 */ 9999,
/* 1443 */ 9999,
/* 1444 */ 528008,
/* 1445 */ 9999,
/* 1446 */ 9999,
/* 1447 */ 529010,
/* 1448 */ 529017,
/* 1449 */ 9999,
/* 1450 */ 9999,
/* 1451 */ 529008,
/* 1452 */ 9999,
/* 1453 */ 9999,
/* 1454 */ 530012,
/* 1455 */ 530019,
/* 1456 */ 9999,
/* 1457 */ 9999,
/* 1458 */ 530010,
/* 1459 */ 9999,
/* 1460 */ 9999,
/* 1461 */ 531009,
/* 1462 */ 531016,
/* 1463 */ 9999,
/* 1464 */ 9999,
/* 1465 */ 531007,
/* 1466 */ 9999,
/* 1467 */ 9999,
/* 1468 */ 532011,
/* 1469 */ 532018,
/* 1470 */ 9999,
/* 1471 */ 9999,
/* 1472 */ 532009,
/* 1473 */ 9999,
/* 1474 */ 9999,
/* 1475 */ 533011,
/* 1476 */ 533018,
/* 1477 */ 9999,
/* 1478 */ 9999,
/* 1479 */ 9999,
/* 1480 */ 533009,
/* 1481 */ 9999,
/* 1482 */ 9999,
/* 1483 */ 534011,
/* 1484 */ 9999,
/* 1485 */ 534022,
/* 1486 */ 9999,
/* 1487 */ 9999,
/* 1488 */ 9999,
/* 1489 */ 534009,
/* 1490 */ 9999,
/* 1491 */ 9999,
/* 1492 */ 535011,
/* 1493 */ 535018,
/* 1494 */ 9999,
/* 1495 */ 9999,
/* 1496 */ 535009,
/* 1497 */ 9999,
/* 1498 */ 9999,
/* 1499 */ 536011,
/* 1500 */ 536018,
/* 1501 */ 9999,
/* 1502 */ 9999,
/* 1503 */ 536009,
/* 1504 */ 9999,
/* 1505 */ 9999,
/* 1506 */ 537014,
/* 1507 */ 537021,
/* 1508 */ 9999,
/* 1509 */ 9999,
/* 1510 */ 537012,
/* 1511 */ 9999,
/* 1512 */ 9999,
/* 1513 */ 538010,
/* 1514 */ 538017,
/* 1515 */ 539005,
/* 1516 */ 9999,
/* 1517 */ 9999,
/* 1518 */ 538008,
/* 1519 */ 9999,
/* 1520 */ 9999,
/* 1521 */ 549012,
/* 1522 */ 549019,
/* 1523 */ 9999,
/* 1524 */ 9999,
/* 1525 */ 549010,
/* 1526 */ 9999,
/* 1527 */ 9999,
/* 1528 */ 556010,
/* 1529 */ 556017,
/* 1530 */ 9999,
/* 1531 */ 9999,
/* 1532 */ 556008,
/* 1533 */ 9999,
/* 1534 */ 9999,
/* 1535 */ 557015,
/* 1536 */ 557022,
/* 1537 */ 9999,
/* 1538 */ 9999,
/* 1539 */ 557013,
/* 1540 */ 9999,
/* 1541 */ 9999,
/* 1542 */ 558009,
/* 1543 */ 558016,
/* 1544 */ 9999,
/* 1545 */ 9999,
/* 1546 */ 558007,
/* 1547 */ 9999,
/* 1548 */ 9999,
/* 1549 */ 559014,
/* 1550 */ 559021,
/* 1551 */ 9999,
/* 1552 */ 9999,
/* 1553 */ 559012,
/* 1554 */ 9999,
/* 1555 */ 9999,
/* 1556 */ 560013,
/* 1557 */ 560020,
/* 1558 */ 9999,
/* 1559 */ 9999,
/* 1560 */ 560011,
/* 1561 */ 9999,
/* 1562 */ 9999,
/* 1563 */ 561009,
/* 1564 */ 561016,
/* 1565 */ 9999,
/* 1566 */ 9999,
/* 1567 */ 561007,
/* 1568 */ 9999,
/* 1569 */ 9999,
/* 1570 */ 562015,
/* 1571 */ 562022,
/* 1572 */ 9999,
/* 1573 */ 9999,
/* 1574 */ 562013,
/* 1575 */ 9999,
/* 1576 */ 9999,
/* 1577 */ 563012,
/* 1578 */ 563019,
/* 1579 */ 9999,
/* 1580 */ 9999,
/* 1581 */ 563010,
/* 1582 */ 9999,
/* 1583 */ 9999,
/* 1584 */ 565011,
/* 1585 */ 565018,
/* 1586 */ 9999,
/* 1587 */ 9999,
/* 1588 */ 565009,
/* 1589 */ 9999,
/* 1590 */ 9999,
/* 1591 */ 567011,
/* 1592 */ 567018,
/* 1593 */ 9999,
/* 1594 */ 9999,
/* 1595 */ 567009,
/* 1596 */ 9999,
/* 1597 */ 9999,
/* 1598 */ 568011,
/* 1599 */ 568018,
/* 1600 */ 9999,
/* 1601 */ 9999,
/* 1602 */ 568009,
/* 1603 */ 9999,
/* 1604 */ 9999,
/* 1605 */ 569014,
/* 1606 */ 569021,
/* 1607 */ 9999,
/* 1608 */ 9999,
/* 1609 */ 569012,
/* 1610 */ 9999,
/* 1611 */ 9999,
/* 1612 */ 570016,
/* 1613 */ 570023,
/* 1614 */ 9999,
/* 1615 */ 9999,
/* 1616 */ 570014,
/* 1617 */ 9999,
/* 1618 */ 9999,
/* 1619 */ 571016,
/* 1620 */ 571023,
/* 1621 */ 9999,
/* 1622 */ 9999,
/* 1623 */ 571014,
/* 1624 */ 9999,
/* 1625 */ 9999,
/* 1626 */ 572014,
/* 1627 */ 572021,
/* 1628 */ 9999,
/* 1629 */ 9999,
/* 1630 */ 572012,
/* 1631 */ 9999,
/* 1632 */ 9999,
/* 1633 */ 573013,
/* 1634 */ 573020,
/* 1635 */ 9999,
/* 1636 */ 9999,
/* 1637 */ 573011,
/* 1638 */ 9999,
/* 1639 */ 9999,
/* 1640 */ 574011,
/* 1641 */ 574018,
/* 1642 */ 9999,
/* 1643 */ 9999,
/* 1644 */ 574009,
/* 1645 */ 9999,
/* 1646 */ 9999,
/* 1647 */ 9999,
/* 1648 */ 575017,
/* 1649 */ 575024,
/* 1650 */ 9999,
/* 1651 */ 9999,
/* 1652 */ 575009,
/* 1653 */ 9999,
/* 1654 */ 9999,
/* 1655 */ 576014,
/* 1656 */ 576021,
/* 1657 */ 9999,
/* 1658 */ 9999,
/* 1659 */ 576012,
/* 1660 */ 9999,
/* 1661 */ 9999,
/* 1662 */ 577010,
/* 1663 */ 577017,
/* 1664 */ 9999,
/* 1665 */ 9999,
/* 1666 */ 577008,
/* 1667 */ 9999,
/* 1668 */ 578005,
/* 1669 */ 9999,
/* 1670 */ 578016,
/* 1671 */ 9999,
/* 1672 */ 579005,
/* 1673 */ 9999,
/* 1674 */ 579014,
/* 1675 */ 9999,
/* 1676 */ 580005,
/* 1677 */ 9999,
/* 1678 */ 580017,
/* 1679 */ 9999,
/* 1680 */ 9999,
/* 1681 */ 9999,
/* 1682 */ 524016,
/* 1683 */ 9999,
/* 1684 */ 9999,
/* 1685 */ 524016,
/* 1686 */ 9999,
/* 1687 */ 9999,
/* 1688 */ 9999,
/* 1689 */ 525018,
/* 1690 */ 9999,
/* 1691 */ 9999,
/* 1692 */ 525018,
/* 1693 */ 9999,
/* 1694 */ 9999,
/* 1695 */ 9999,
/* 1696 */ 527011,
/* 1697 */ 9999,
/* 1698 */ 9999,
/* 1699 */ 527011,
/* 1700 */ 9999,
/* 1701 */ 9999,
/* 1702 */ 9999,
/* 1703 */ 528011,
/* 1704 */ 9999,
/* 1705 */ 9999,
/* 1706 */ 528011,
/* 1707 */ 9999,
/* 1708 */ 9999,
/* 1709 */ 9999,
/* 1710 */ 529011,
/* 1711 */ 9999,
/* 1712 */ 9999,
/* 1713 */ 529011,
/* 1714 */ 9999,
/* 1715 */ 9999,
/* 1716 */ 9999,
/* 1717 */ 530013,
/* 1718 */ 9999,
/* 1719 */ 9999,
/* 1720 */ 530013,
/* 1721 */ 9999,
/* 1722 */ 9999,
/* 1723 */ 9999,
/* 1724 */ 531010,
/* 1725 */ 9999,
/* 1726 */ 9999,
/* 1727 */ 531010,
/* 1728 */ 9999,
/* 1729 */ 9999,
/* 1730 */ 9999,
/* 1731 */ 532012,
/* 1732 */ 9999,
/* 1733 */ 9999,
/* 1734 */ 532012,
/* 1735 */ 9999,
/* 1736 */ 9999,
/* 1737 */ 9999,
/* 1738 */ 533012,
/* 1739 */ 9999,
/* 1740 */ 9999,
/* 1741 */ 533012,
/* 1742 */ 9999,
/* 1743 */ 9999,
/* 1744 */ 9999,
/* 1745 */ 534012,
/* 1746 */ 9999,
/* 1747 */ 9999,
/* 1748 */ 534012,
/* 1749 */ 9999,
/* 1750 */ 9999,
/* 1751 */ 9999,
/* 1752 */ 535012,
/* 1753 */ 9999,
/* 1754 */ 9999,
/* 1755 */ 535012,
/* 1756 */ 9999,
/* 1757 */ 9999,
/* 1758 */ 9999,
/* 1759 */ 536012,
/* 1760 */ 9999,
/* 1761 */ 9999,
/* 1762 */ 536012,
/* 1763 */ 9999,
/* 1764 */ 9999,
/* 1765 */ 9999,
/* 1766 */ 537015,
/* 1767 */ 9999,
/* 1768 */ 9999,
/* 1769 */ 537015,
/* 1770 */ 9999,
/* 1771 */ 9999,
/* 1772 */ 9999,
/* 1773 */ 538011,
/* 1774 */ 9999,
/* 1775 */ 9999,
/* 1776 */ 538011,
/* 1777 */ 9999,
/* 1778 */ 539007,
/* 1779 */ 9999,
/* 1780 */ 539012,
/* 1781 */ 9999,
/* 1782 */ 9999,
/* 1783 */ 539012,
/* 1784 */ 9999,
/* 1785 */ 9999,
/* 1786 */ 9999,
/* 1787 */ 549013,
/* 1788 */ 9999,
/* 1789 */ 9999,
/* 1790 */ 549013,
/* 1791 */ 9999,
/* 1792 */ 9999,
/* 1793 */ 9999,
/* 1794 */ 556011,
/* 1795 */ 9999,
/* 1796 */ 9999,
/* 1797 */ 556011,
/* 1798 */ 9999,
/* 1799 */ 9999,
/* 1800 */ 9999,
/* 1801 */ 557016,
/* 1802 */ 9999,
/* 1803 */ 9999,
/* 1804 */ 557016,
/* 1805 */ 9999,
/* 1806 */ 9999,
/* 1807 */ 9999,
/* 1808 */ 558010,
/* 1809 */ 9999,
/* 1810 */ 9999,
/* 1811 */ 558010,
/* 1812 */ 9999,
/* 1813 */ 9999,
/* 1814 */ 9999,
/* 1815 */ 559015,
/* 1816 */ 9999,
/* 1817 */ 9999,
/* 1818 */ 559015,
/* 1819 */ 9999,
/* 1820 */ 9999,
/* 1821 */ 9999,
/* 1822 */ 560014,
/* 1823 */ 9999,
/* 1824 */ 9999,
/* 1825 */ 560014,
/* 1826 */ 9999,
/* 1827 */ 9999,
/* 1828 */ 9999,
/* 1829 */ 561010,
/* 1830 */ 9999,
/* 1831 */ 9999,
/* 1832 */ 561010,
/* 1833 */ 9999,
/* 1834 */ 561017,
/* 1835 */ 561025,
/* 1836 */ 9999,
/* 1837 */ 561025,
/* 1838 */ 9999,
/* 1839 */ 9999,
/* 1840 */ 561025,
/* 1841 */ 9999,
/* 1842 */ 9999,
/* 1843 */ 9999,
/* 1844 */ 562016,
/* 1845 */ 9999,
/* 1846 */ 9999,
/* 1847 */ 562016,
/* 1848 */ 9999,
/* 1849 */ 9999,
/* 1850 */ 9999,
/* 1851 */ 563013,
/* 1852 */ 9999,
/* 1853 */ 9999,
/* 1854 */ 563013,
/* 1855 */ 9999,
/* 1856 */ 563021,
/* 1857 */ 564006,
/* 1858 */ 563027,
/* 1859 */ 9999,
/* 1860 */ 563027,
/* 1861 */ 9999,
/* 1862 */ 9999,
/* 1863 */ 563027,
/* 1864 */ 9999,
/* 1865 */ 9999,
/* 1866 */ 9999,
/* 1867 */ 564007,
/* 1868 */ 9999,
/* 1869 */ 9999,
/* 1870 */ 564007,
/* 1871 */ 9999,
/* 1872 */ 9999,
/* 1873 */ 9999,
/* 1874 */ 565012,
/* 1875 */ 9999,
/* 1876 */ 9999,
/* 1877 */ 565012,
/* 1878 */ 9999,
/* 1879 */ 565020,
/* 1880 */ 566006,
/* 1881 */ 565026,
/* 1882 */ 9999,
/* 1883 */ 565026,
/* 1884 */ 9999,
/* 1885 */ 9999,
/* 1886 */ 565026,
/* 1887 */ 9999,
/* 1888 */ 9999,
/* 1889 */ 9999,
/* 1890 */ 566007,
/* 1891 */ 9999,
/* 1892 */ 9999,
/* 1893 */ 566007,
/* 1894 */ 9999,
/* 1895 */ 9999,
/* 1896 */ 9999,
/* 1897 */ 567012,
/* 1898 */ 9999,
/* 1899 */ 9999,
/* 1900 */ 567012,
/* 1901 */ 9999,
/* 1902 */ 567019,
/* 1903 */ 567029,
/* 1904 */ 9999,
/* 1905 */ 567029,
/* 1906 */ 9999,
/* 1907 */ 9999,
/* 1908 */ 567029,
/* 1909 */ 9999,
/* 1910 */ 9999,
/* 1911 */ 9999,
/* 1912 */ 568012,
/* 1913 */ 9999,
/* 1914 */ 9999,
/* 1915 */ 568012,
/* 1916 */ 9999,
/* 1917 */ 568020,
/* 1918 */ 568030,
/* 1919 */ 568025,
/* 1920 */ 9999,
/* 1921 */ 568025,
/* 1922 */ 9999,
/* 1923 */ 9999,
/* 1924 */ 568025,
/* 1925 */ 9999,
/* 1926 */ 9999,
/* 1927 */ 9999,
/* 1928 */ 568031,
/* 1929 */ 9999,
/* 1930 */ 9999,
/* 1931 */ 9999,
/* 1932 */ 568035,
/* 1933 */ 9999,
/* 1934 */ 9999,
/* 1935 */ 568035,
/* 1936 */ 9999,
/* 1937 */ 9999,
/* 1938 */ 9999,
/* 1939 */ 569015,
/* 1940 */ 9999,
/* 1941 */ 9999,
/* 1942 */ 569015,
/* 1943 */ 9999,
/* 1944 */ 9999,
/* 1945 */ 9999,
/* 1946 */ 570017,
/* 1947 */ 9999,
/* 1948 */ 9999,
/* 1949 */ 570017,
/* 1950 */ 9999,
/* 1951 */ 9999,
/* 1952 */ 9999,
/* 1953 */ 571017,
/* 1954 */ 9999,
/* 1955 */ 9999,
/* 1956 */ 571017,
/* 1957 */ 9999,
/* 1958 */ 9999,
/* 1959 */ 9999,
/* 1960 */ 572015,
/* 1961 */ 9999,
/* 1962 */ 9999,
/* 1963 */ 572015,
/* 1964 */ 9999,
/* 1965 */ 9999,
/* 1966 */ 9999,
/* 1967 */ 573014,
/* 1968 */ 9999,
/* 1969 */ 9999,
/* 1970 */ 573014,
/* 1971 */ 9999,
/* 1972 */ 9999,
/* 1973 */ 9999,
/* 1974 */ 574012,
/* 1975 */ 9999,
/* 1976 */ 9999,
/* 1977 */ 574012,
/* 1978 */ 9999,
/* 1979 */ 9999,
/* 1980 */ 9999,
/* 1981 */ 575018,
/* 1982 */ 9999,
/* 1983 */ 9999,
/* 1984 */ 575018,
/* 1985 */ 9999,
/* 1986 */ 9999,
/* 1987 */ 9999,
/* 1988 */ 576015,
/* 1989 */ 9999,
/* 1990 */ 9999,
/* 1991 */ 576015,
/* 1992 */ 9999,
/* 1993 */ 9999,
/* 1994 */ 9999,
/* 1995 */ 577011,
/* 1996 */ 9999,
/* 1997 */ 9999,
/* 1998 */ 577011,
/* 1999 */ 9999,
/* 2000 */ 584003,
/* 2001 */ 584014,
/* 2002 */ 9999,
/* 2003 */ 584008,
/* 2004 */ 9999,
/* 2005 */ 9999,
/* 2006 */ 588018,
/* 2007 */ 588028,
/* 2008 */ 9999,
/* 2009 */ 588012,
/* 2010 */ 9999,
/* 2011 */ 9999,
/* 2012 */ 9999,
/* 2013 */ 584015,
/* 2014 */ 9999,
/* 2015 */ 9999,
/* 2016 */ 584015,
/* 2017 */ 9999,
/* 2018 */ 9999,
/* 2019 */ 9999,
/* 2020 */ 588029,
/* 2021 */ 9999,
/* 2022 */ 9999,
/* 2023 */ 588029,
/* 2024 */ 9999,
/* 2025 */ 592005,
/* 2026 */ 592018,
/* 2027 */ 9999,
/* 2028 */ 592010,
/* 2029 */ 9999,
/* 2030 */ 9999,
/* 2031 */ 593009,
/* 2032 */ 593021,
/* 2033 */ 9999,
/* 2034 */ 593005,
/* 2035 */ 9999,
/* 2036 */ 9999,
/* 2037 */ 9999,
/* 2038 */ 592019,
/* 2039 */ 9999,
/* 2040 */ 9999,
/* 2041 */ 592019,
/* 2042 */ 9999,
/* 2043 */ 9999,
/* 2044 */ 9999,
/* 2045 */ 593022,
/* 2046 */ 9999,
/* 2047 */ 9999,
/* 2048 */ 593022,
/* 2049 */ 9999,
/* 2050 */ 9999,
/* 2051 */ 9999,
/* 2052 */ 9999,
/* 2053 */ 600007,
/* 2054 */ 9999,
/* 2055 */ 601007,
/* 2056 */ 9999,
/* 2057 */ 9999,
/* 2058 */ 598009,
/* 2059 */ 9999,
/* 2060 */ 601008,
/* 2061 */ 601017,
/* 2062 */ 9999,
/* 2063 */ 601017,
/* 2064 */ 9999,
/* 2065 */ 9999,
/* 2066 */ 601017,
/* 2067 */ 9999,
/* 2068 */ 9999,
/* 2069 */ 605016,
/* 2070 */ 9999,
/* 2071 */ 9999,
/* 2072 */ 9999,
/* 2073 */ 605014,
/* 2074 */ 9999,
/* 2075 */ 9999,
/* 2076 */ 606015,
/* 2077 */ 606022,
/* 2078 */ 9999,
/* 2079 */ 9999,
/* 2080 */ 606013,
/* 2081 */ 9999,
/* 2082 */ 607005,
/* 2083 */ 9999,
/* 2084 */ 607015,
/* 2085 */ 9999,
/* 2086 */ 9999,
/* 2087 */ 608010,
/* 2088 */ 608017,
/* 2089 */ 9999,
/* 2090 */ 9999,
/* 2091 */ 9999,
/* 2092 */ 608008,
/* 2093 */ 9999,
/* 2094 */ 9999,
/* 2095 */ 609015,
/* 2096 */ 609022,
/* 2097 */ 9999,
/* 2098 */ 9999,
/* 2099 */ 609013,
/* 2100 */ 9999,
/* 2101 */ 9999,
/* 2102 */ 610011,
/* 2103 */ 610019,
/* 2104 */ 9999,
/* 2105 */ 9999,
/* 2106 */ 610009,
/* 2107 */ 9999,
/* 2108 */ 611005,
/* 2109 */ 611028,
/* 2110 */ 611035,
/* 2111 */ 9999,
/* 2112 */ 9999,
/* 2113 */ 611005,
/* 2114 */ 9999,
/* 2115 */ 9999,
/* 2116 */ 612010,
/* 2117 */ 612017,
/* 2118 */ 9999,
/* 2119 */ 9999,
/* 2120 */ 612008,
/* 2121 */ 9999,
/* 2122 */ 9999,
/* 2123 */ 613012,
/* 2124 */ 613019,
/* 2125 */ 9999,
/* 2126 */ 9999,
/* 2127 */ 613010,
/* 2128 */ 9999,
/* 2129 */ 614005,
/* 2130 */ 9999,
/* 2131 */ 614014,
/* 2132 */ 9999,
/* 2133 */ 615005,
/* 2134 */ 9999,
/* 2135 */ 615016,
/* 2136 */ 9999,
/* 2137 */ 616005,
/* 2138 */ 9999,
/* 2139 */ 616017,
/* 2140 */ 9999,
/* 2141 */ 9999,
/* 2142 */ 9999,
/* 2143 */ 605017,
/* 2144 */ 9999,
/* 2145 */ 9999,
/* 2146 */ 605017,
/* 2147 */ 9999,
/* 2148 */ 9999,
/* 2149 */ 9999,
/* 2150 */ 606016,
/* 2151 */ 9999,
/* 2152 */ 9999,
/* 2153 */ 606016,
/* 2154 */ 9999,
/* 2155 */ 9999,
/* 2156 */ 9999,
/* 2157 */ 608011,
/* 2158 */ 9999,
/* 2159 */ 9999,
/* 2160 */ 608011,
/* 2161 */ 9999,
/* 2162 */ 9999,
/* 2163 */ 9999,
/* 2164 */ 609016,
/* 2165 */ 9999,
/* 2166 */ 9999,
/* 2167 */ 609016,
/* 2168 */ 9999,
/* 2169 */ 9999,
/* 2170 */ 9999,
/* 2171 */ 610012,
/* 2172 */ 9999,
/* 2173 */ 9999,
/* 2174 */ 610012,
/* 2175 */ 9999,
/* 2176 */ 610020,
/* 2177 */ 610030,
/* 2178 */ 610025,
/* 2179 */ 9999,
/* 2180 */ 610025,
/* 2181 */ 9999,
/* 2182 */ 9999,
/* 2183 */ 610025,
/* 2184 */ 9999,
/* 2185 */ 9999,
/* 2186 */ 9999,
/* 2187 */ 610031,
/* 2188 */ 9999,
/* 2189 */ 9999,
/* 2190 */ 9999,
/* 2191 */ 610037,
/* 2192 */ 9999,
/* 2193 */ 9999,
/* 2194 */ 610037,
/* 2195 */ 9999,
/* 2196 */ 9999,
/* 2197 */ 9999,
/* 2198 */ 611016,
/* 2199 */ 9999,
/* 2200 */ 9999,
/* 2201 */ 9999,
/* 2202 */ 611025,
/* 2203 */ 9999,
/* 2204 */ 9999,
/* 2205 */ 9999,
/* 2206 */ 611029,
/* 2207 */ 9999,
/* 2208 */ 9999,
/* 2209 */ 611029,
/* 2210 */ 9999,
/* 2211 */ 9999,
/* 2212 */ 9999,
/* 2213 */ 612011,
/* 2214 */ 9999,
/* 2215 */ 9999,
/* 2216 */ 612011,
/* 2217 */ 9999,
/* 2218 */ 9999,
/* 2219 */ 9999,
/* 2220 */ 613013,
/* 2221 */ 9999,
/* 2222 */ 9999,
/* 2223 */ 613013,
/* 2224 */ 9999,
/* 2225 */ 620003,
/* 2226 */ 629003,
/* 2227 */ 9999,
/* 2228 */ 9999,
/* 2229 */ 633007,
/* 2230 */ 9999,
/* 2231 */ 635007,
/* 2232 */ 9999,
/* 2233 */ 636003,
/* 2234 */ 9999,
/* 2235 */ 619037,
/* 2236 */ 9999,
/* 2237 */ 620005,
/* 2238 */ 628005,
/* 2239 */ 620010,
/* 2240 */ 9999,
/* 2241 */ 620010,
/* 2242 */ 9999,
/* 2243 */ 9999,
/* 2244 */ 620010,
/* 2245 */ 9999,
/* 2246 */ 9999,
/* 2247 */ 9999,
/* 2248 */ 628006,
/* 2249 */ 9999,
/* 2250 */ 9999,
/* 2251 */ 628006,
/* 2252 */ 9999,
/* 2253 */ 9999,
/* 2254 */ 9999,
/* 2255 */ 629008,
/* 2256 */ 9999,
/* 2257 */ 9999,
/* 2258 */ 9999,
/* 2259 */ 629017,
/* 2260 */ 9999,
/* 2261 */ 9999,
/* 2262 */ 9999,
/* 2263 */ 630012,
/* 2264 */ 9999,
/* 2265 */ 9999,
/* 2266 */ 9999,
/* 2267 */ 631013,
/* 2268 */ 9999,
/* 2269 */ 635008,
/* 2270 */ 635016,
/* 2271 */ 9999,
/* 2272 */ 635016,
/* 2273 */ 9999,
/* 2274 */ 9999,
/* 2275 */ 635016,
/* 2276 */ 9999,
/* 2277 */ 9999,
/* 2278 */ 636009,
/* 2279 */ 9999,
/* 2280 */ 657008,
/* 2281 */ 9999,
/* 2282 */ 636007,
/* 2283 */ 9999,
/* 2284 */ 636007,
/* 2285 */ 9999,
/* 2286 */ 9999,
/* 2287 */ 636007,
/* 2288 */ 9999,
/* 2289 */ 657009,
/* 2290 */ 657017,
/* 2291 */ 9999,
/* 2292 */ 657017,
/* 2293 */ 9999,
/* 2294 */ 9999,
/* 2295 */ 657017,
/* 2296 */ 9999,
/* 2297 */ 9999,
/* 2298 */ 662015,
/* 2299 */ 9999,
/* 2300 */ 9999,
/* 2301 */ 9999,
/* 2302 */ 662013,
/* 2303 */ 9999,
/* 2304 */ 663005,
/* 2305 */ 9999,
/* 2306 */ 9999,
/* 2307 */ 663009,
/* 2308 */ 9999,
/* 2309 */ 9999,
/* 2310 */ 664016,
/* 2311 */ 664023,
/* 2312 */ 9999,
/* 2313 */ 9999,
/* 2314 */ 664014,
/* 2315 */ 9999,
/* 2316 */ 9999,
/* 2317 */ 665010,
/* 2318 */ 665017,
/* 2319 */ 9999,
/* 2320 */ 9999,
/* 2321 */ 665008,
/* 2322 */ 9999,
/* 2323 */ 9999,
/* 2324 */ 666010,
/* 2325 */ 666020,
/* 2326 */ 666027,
/* 2327 */ 666037,
/* 2328 */ 9999,
/* 2329 */ 9999,
/* 2330 */ 666008,
/* 2331 */ 9999,
/* 2332 */ 9999,
/* 2333 */ 667013,
/* 2334 */ 667020,
/* 2335 */ 668007,
/* 2336 */ 9999,
/* 2337 */ 9999,
/* 2338 */ 667011,
/* 2339 */ 9999,
/* 2340 */ 9999,
/* 2341 */ 669010,
/* 2342 */ 669017,
/* 2343 */ 669027,
/* 2344 */ 9999,
/* 2345 */ 9999,
/* 2346 */ 669008,
/* 2347 */ 9999,
/* 2348 */ 9999,
/* 2349 */ 671010,
/* 2350 */ 671017,
/* 2351 */ 9999,
/* 2352 */ 9999,
/* 2353 */ 671008,
/* 2354 */ 9999,
/* 2355 */ 9999,
/* 2356 */ 672010,
/* 2357 */ 672017,
/* 2358 */ 9999,
/* 2359 */ 9999,
/* 2360 */ 672008,
/* 2361 */ 9999,
/* 2362 */ 9999,
/* 2363 */ 673010,
/* 2364 */ 673017,
/* 2365 */ 9999,
/* 2366 */ 9999,
/* 2367 */ 673008,
/* 2368 */ 9999,
/* 2369 */ 9999,
/* 2370 */ 674010,
/* 2371 */ 674017,
/* 2372 */ 9999,
/* 2373 */ 9999,
/* 2374 */ 674008,
/* 2375 */ 9999,
/* 2376 */ 9999,
/* 2377 */ 675010,
/* 2378 */ 675017,
/* 2379 */ 9999,
/* 2380 */ 9999,
/* 2381 */ 675008,
/* 2382 */ 9999,
/* 2383 */ 9999,
/* 2384 */ 676010,
/* 2385 */ 676017,
/* 2386 */ 9999,
/* 2387 */ 9999,
/* 2388 */ 676008,
/* 2389 */ 9999,
/* 2390 */ 9999,
/* 2391 */ 677010,
/* 2392 */ 677017,
/* 2393 */ 9999,
/* 2394 */ 9999,
/* 2395 */ 677008,
/* 2396 */ 9999,
/* 2397 */ 9999,
/* 2398 */ 678010,
/* 2399 */ 678017,
/* 2400 */ 9999,
/* 2401 */ 9999,
/* 2402 */ 678008,
/* 2403 */ 9999,
/* 2404 */ 9999,
/* 2405 */ 679010,
/* 2406 */ 679017,
/* 2407 */ 9999,
/* 2408 */ 9999,
/* 2409 */ 679008,
/* 2410 */ 9999,
/* 2411 */ 9999,
/* 2412 */ 680010,
/* 2413 */ 680017,
/* 2414 */ 9999,
/* 2415 */ 9999,
/* 2416 */ 680008,
/* 2417 */ 9999,
/* 2418 */ 9999,
/* 2419 */ 681010,
/* 2420 */ 681017,
/* 2421 */ 9999,
/* 2422 */ 9999,
/* 2423 */ 681008,
/* 2424 */ 9999,
/* 2425 */ 9999,
/* 2426 */ 682010,
/* 2427 */ 9999,
/* 2428 */ 9999,
/* 2429 */ 9999,
/* 2430 */ 682008,
/* 2431 */ 9999,
/* 2432 */ 9999,
/* 2433 */ 683011,
/* 2434 */ 683018,
/* 2435 */ 9999,
/* 2436 */ 9999,
/* 2437 */ 683009,
/* 2438 */ 9999,
/* 2439 */ 685005,
/* 2440 */ 9999,
/* 2441 */ 685014,
/* 2442 */ 9999,
/* 2443 */ 686005,
/* 2444 */ 9999,
/* 2445 */ 686017,
/* 2446 */ 9999,
/* 2447 */ 687005,
/* 2448 */ 9999,
/* 2449 */ 687016,
/* 2450 */ 9999,
/* 2451 */ 9999,
/* 2452 */ 9999,
/* 2453 */ 662016,
/* 2454 */ 9999,
/* 2455 */ 9999,
/* 2456 */ 662016,
/* 2457 */ 9999,
/* 2458 */ 9999,
/* 2459 */ 9999,
/* 2460 */ 664017,
/* 2461 */ 9999,
/* 2462 */ 9999,
/* 2463 */ 664017,
/* 2464 */ 9999,
/* 2465 */ 9999,
/* 2466 */ 9999,
/* 2467 */ 665011,
/* 2468 */ 9999,
/* 2469 */ 9999,
/* 2470 */ 665011,
/* 2471 */ 9999,
/* 2472 */ 9999,
/* 2473 */ 9999,
/* 2474 */ 666016,
/* 2475 */ 9999,
/* 2476 */ 9999,
/* 2477 */ 666016,
/* 2478 */ 9999,
/* 2479 */ 9999,
/* 2480 */ 9999,
/* 2481 */ 666021,
/* 2482 */ 9999,
/* 2483 */ 9999,
/* 2484 */ 666021,
/* 2485 */ 9999,
/* 2486 */ 9999,
/* 2487 */ 9999,
/* 2488 */ 666038,
/* 2489 */ 9999,
/* 2490 */ 9999,
/* 2491 */ 666038,
/* 2492 */ 9999,
/* 2493 */ 9999,
/* 2494 */ 9999,
/* 2495 */ 667014,
/* 2496 */ 9999,
/* 2497 */ 9999,
/* 2498 */ 667014,
/* 2499 */ 9999,
/* 2500 */ 9999,
/* 2501 */ 668013,
/* 2502 */ 9999,
/* 2503 */ 9999,
/* 2504 */ 668009,
/* 2505 */ 9999,
/* 2506 */ 9999,
/* 2507 */ 668009,
/* 2508 */ 9999,
/* 2509 */ 9999,
/* 2510 */ 9999,
/* 2511 */ 669011,
/* 2512 */ 9999,
/* 2513 */ 9999,
/* 2514 */ 669011,
/* 2515 */ 9999,
/* 2516 */ 9999,
/* 2517 */ 9999,
/* 2518 */ 669035,
/* 2519 */ 9999,
/* 2520 */ 9999,
/* 2521 */ 669035,
/* 2522 */ 9999,
/* 2523 */ 9999,
/* 2524 */ 9999,
/* 2525 */ 671011,
/* 2526 */ 9999,
/* 2527 */ 9999,
/* 2528 */ 671011,
/* 2529 */ 9999,
/* 2530 */ 9999,
/* 2531 */ 9999,
/* 2532 */ 672011,
/* 2533 */ 9999,
/* 2534 */ 9999,
/* 2535 */ 672011,
/* 2536 */ 9999,
/* 2537 */ 9999,
/* 2538 */ 9999,
/* 2539 */ 673011,
/* 2540 */ 9999,
/* 2541 */ 9999,
/* 2542 */ 673011,
/* 2543 */ 9999,
/* 2544 */ 9999,
/* 2545 */ 9999,
/* 2546 */ 674011,
/* 2547 */ 9999,
/* 2548 */ 9999,
/* 2549 */ 674011,
/* 2550 */ 9999,
/* 2551 */ 9999,
/* 2552 */ 9999,
/* 2553 */ 675011,
/* 2554 */ 9999,
/* 2555 */ 9999,
/* 2556 */ 675011,
/* 2557 */ 9999,
/* 2558 */ 9999,
/* 2559 */ 9999,
/* 2560 */ 676011,
/* 2561 */ 9999,
/* 2562 */ 9999,
/* 2563 */ 676011,
/* 2564 */ 9999,
/* 2565 */ 9999,
/* 2566 */ 9999,
/* 2567 */ 677011,
/* 2568 */ 9999,
/* 2569 */ 9999,
/* 2570 */ 677011,
/* 2571 */ 9999,
/* 2572 */ 9999,
/* 2573 */ 9999,
/* 2574 */ 678011,
/* 2575 */ 9999,
/* 2576 */ 9999,
/* 2577 */ 678011,
/* 2578 */ 9999,
/* 2579 */ 9999,
/* 2580 */ 9999,
/* 2581 */ 679011,
/* 2582 */ 9999,
/* 2583 */ 9999,
/* 2584 */ 679011,
/* 2585 */ 9999,
/* 2586 */ 9999,
/* 2587 */ 9999,
/* 2588 */ 680011,
/* 2589 */ 9999,
/* 2590 */ 9999,
/* 2591 */ 680011,
/* 2592 */ 9999,
/* 2593 */ 9999,
/* 2594 */ 9999,
/* 2595 */ 681011,
/* 2596 */ 9999,
/* 2597 */ 9999,
/* 2598 */ 681011,
/* 2599 */ 9999,
/* 2600 */ 9999,
/* 2601 */ 9999,
/* 2602 */ 682011,
/* 2603 */ 9999,
/* 2604 */ 9999,
/* 2605 */ 682011,
/* 2606 */ 9999,
/* 2607 */ 9999,
/* 2608 */ 9999,
/* 2609 */ 683012,
/* 2610 */ 9999,
/* 2611 */ 9999,
/* 2612 */ 683012,
/* 2613 */ 9999,
/* 2614 */ 683019,
/* 2615 */ 684005,
/* 2616 */ 683024,
/* 2617 */ 9999,
/* 2618 */ 683024,
/* 2619 */ 9999,
/* 2620 */ 9999,
/* 2621 */ 683024,
/* 2622 */ 9999,
/* 2623 */ 9999,
/* 2624 */ 9999,
/* 2625 */ 684006,
/* 2626 */ 9999,
/* 2627 */ 9999,
/* 2628 */ 9999,
/* 2629 */ 684012,
/* 2630 */ 9999,
/* 2631 */ 9999,
/* 2632 */ 684012,
/* 2633 */ 9999,
/* 2634 */ 691003,
/* 2635 */ 9999,
/* 2636 */ 690015,
/* 2637 */ 9999,
/* 2638 */ 691004,
/* 2639 */ 691029,
/* 2640 */ 691009,
/* 2641 */ 9999,
/* 2642 */ 691009,
/* 2643 */ 9999,
/* 2644 */ 9999,
/* 2645 */ 691009,
/* 2646 */ 9999,
/* 2647 */ 9999,
/* 2648 */ 9999,
/* 2649 */ 691030,
/* 2650 */ 9999,
/* 2651 */ 9999,
/* 2652 */ 9999,
/* 2653 */ 691036,
/* 2654 */ 9999,
/* 2655 */ 9999,
/* 2656 */ 691036,
/* 2657 */ 9999,
/* 2658 */ 9999,
/* 2659 */ 9999,
/* 2660 */ 697009,
/* 2661 */ 9999,
/* 2662 */ 9999,
/* 2663 */ 9999,
/* 2664 */ 702005,
/* 2665 */ 9999,
/* 2666 */ 707007,
/* 2667 */ 9999,
/* 2668 */ 9999,
/* 2669 */ 700006,
/* 2670 */ 9999,
/* 2671 */ 9999,
/* 2672 */ 702011,
/* 2673 */ 9999,
/* 2674 */ 702007,
/* 2675 */ 9999,
/* 2676 */ 9999,
/* 2677 */ 702007,
/* 2678 */ 9999,
/* 2679 */ 707008,
/* 2680 */ 707017,
/* 2681 */ 9999,
/* 2682 */ 707017,
/* 2683 */ 9999,
/* 2684 */ 9999,
/* 2685 */ 707017,
/* 2686 */ 9999,
/* 2687 */ 9999,
/* 2688 */ 711014,
/* 2689 */ 9999,
/* 2690 */ 9999,
/* 2691 */ 9999,
/* 2692 */ 711012,
/* 2693 */ 9999,
/* 2694 */ 712004,
/* 2695 */ 9999,
/* 2696 */ 9999,
/* 2697 */ 712004,
/* 2698 */ 9999,
/* 2699 */ 9999,
/* 2700 */ 714012,
/* 2701 */ 714019,
/* 2702 */ 9999,
/* 2703 */ 9999,
/* 2704 */ 714010,
/* 2705 */ 9999,
/* 2706 */ 9999,
/* 2707 */ 716013,
/* 2708 */ 716020,
/* 2709 */ 9999,
/* 2710 */ 9999,
/* 2711 */ 716011,
/* 2712 */ 9999,
/* 2713 */ 9999,
/* 2714 */ 717013,
/* 2715 */ 717022,
/* 2716 */ 717029,
/* 2717 */ 9999,
/* 2718 */ 717045,
/* 2719 */ 9999,
/* 2720 */ 9999,
/* 2721 */ 717011,
/* 2722 */ 9999,
/* 2723 */ 9999,
/* 2724 */ 720013,
/* 2725 */ 720022,
/* 2726 */ 720029,
/* 2727 */ 9999,
/* 2728 */ 720045,
/* 2729 */ 9999,
/* 2730 */ 720062,
/* 2731 */ 9999,
/* 2732 */ 9999,
/* 2733 */ 720011,
/* 2734 */ 9999,
/* 2735 */ 9999,
/* 2736 */ 724012,
/* 2737 */ 725007,
/* 2738 */ 9999,
/* 2739 */ 9999,
/* 2740 */ 724010,
/* 2741 */ 9999,
/* 2742 */ 726005,
/* 2743 */ 726015,
/* 2744 */ 9999,
/* 2745 */ 9999,
/* 2746 */ 9999,
/* 2747 */ 726010,
/* 2748 */ 9999,
/* 2749 */ 9999,
/* 2750 */ 732011,
/* 2751 */ 732019,
/* 2752 */ 9999,
/* 2753 */ 9999,
/* 2754 */ 732009,
/* 2755 */ 9999,
/* 2756 */ 733005,
/* 2757 */ 9999,
/* 2758 */ 733013,
/* 2759 */ 9999,
/* 2760 */ 734005,
/* 2761 */ 9999,
/* 2762 */ 734016,
/* 2763 */ 9999,
/* 2764 */ 735005,
/* 2765 */ 9999,
/* 2766 */ 735014,
/* 2767 */ 9999,
/* 2768 */ 736005,
/* 2769 */ 9999,
/* 2770 */ 736017,
/* 2771 */ 9999,
/* 2772 */ 9999,
/* 2773 */ 9999,
/* 2774 */ 711015,
/* 2775 */ 9999,
/* 2776 */ 9999,
/* 2777 */ 711015,
/* 2778 */ 9999,
/* 2779 */ 9999,
/* 2780 */ 712019,
/* 2781 */ 712026,
/* 2782 */ 713003,
/* 2783 */ 712013,
/* 2784 */ 9999,
/* 2785 */ 712013,
/* 2786 */ 9999,
/* 2787 */ 9999,
/* 2788 */ 712013,
/* 2789 */ 9999,
/* 2790 */ 9999,
/* 2791 */ 9999,
/* 2792 */ 712020,
/* 2793 */ 9999,
/* 2794 */ 9999,
/* 2795 */ 712020,
/* 2796 */ 9999,
/* 2797 */ 9999,
/* 2798 */ 9999,
/* 2799 */ 713004,
/* 2800 */ 9999,
/* 2801 */ 9999,
/* 2802 */ 713004,
/* 2803 */ 9999,
/* 2804 */ 9999,
/* 2805 */ 9999,
/* 2806 */ 714013,
/* 2807 */ 9999,
/* 2808 */ 9999,
/* 2809 */ 714013,
/* 2810 */ 9999,
/* 2811 */ 714020,
/* 2812 */ 714054,
/* 2813 */ 714034,
/* 2814 */ 9999,
/* 2815 */ 714034,
/* 2816 */ 9999,
/* 2817 */ 9999,
/* 2818 */ 714034,
/* 2819 */ 9999,
/* 2820 */ 9999,
/* 2821 */ 9999,
/* 2822 */ 714055,
/* 2823 */ 9999,
/* 2824 */ 9999,
/* 2825 */ 714055,
/* 2826 */ 9999,
/* 2827 */ 9999,
/* 2828 */ 9999,
/* 2829 */ 716014,
/* 2830 */ 9999,
/* 2831 */ 9999,
/* 2832 */ 716014,
/* 2833 */ 9999,
/* 2834 */ 9999,
/* 2835 */ 9999,
/* 2836 */ 717018,
/* 2837 */ 9999,
/* 2838 */ 9999,
/* 2839 */ 717018,
/* 2840 */ 9999,
/* 2841 */ 9999,
/* 2842 */ 9999,
/* 2843 */ 717023,
/* 2844 */ 9999,
/* 2845 */ 9999,
/* 2846 */ 717023,
/* 2847 */ 9999,
/* 2848 */ 9999,
/* 2849 */ 9999,
/* 2850 */ 720018,
/* 2851 */ 9999,
/* 2852 */ 9999,
/* 2853 */ 720018,
/* 2854 */ 9999,
/* 2855 */ 9999,
/* 2856 */ 9999,
/* 2857 */ 720023,
/* 2858 */ 9999,
/* 2859 */ 9999,
/* 2860 */ 720023,
/* 2861 */ 9999,
/* 2862 */ 9999,
/* 2863 */ 9999,
/* 2864 */ 724013,
/* 2865 */ 9999,
/* 2866 */ 9999,
/* 2867 */ 724013,
/* 2868 */ 9999,
/* 2869 */ 725009,
/* 2870 */ 725053,
/* 2871 */ 725014,
/* 2872 */ 9999,
/* 2873 */ 725014,
/* 2874 */ 9999,
/* 2875 */ 9999,
/* 2876 */ 725014,
/* 2877 */ 9999,
/* 2878 */ 9999,
/* 2879 */ 9999,
/* 2880 */ 725054,
/* 2881 */ 9999,
/* 2882 */ 9999,
/* 2883 */ 725054,
/* 2884 */ 9999,
/* 2885 */ 9999,
/* 2886 */ 9999,
/* 2887 */ 726016,
/* 2888 */ 9999,
/* 2889 */ 9999,
/* 2890 */ 726016,
/* 2891 */ 9999,
/* 2892 */ 9999,
/* 2893 */ 9999,
/* 2894 */ 732012,
/* 2895 */ 9999,
/* 2896 */ 9999,
/* 2897 */ 732012,
/* 2898 */ 9999,
/* 2899 */ 732020,
/* 2900 */ 732030,
/* 2901 */ 732025,
/* 2902 */ 9999,
/* 2903 */ 732025,
/* 2904 */ 9999,
/* 2905 */ 9999,
/* 2906 */ 732025,
/* 2907 */ 9999,
/* 2908 */ 9999,
/* 2909 */ 9999,
/* 2910 */ 732031,
/* 2911 */ 9999,
/* 2912 */ 9999,
/* 2913 */ 9999,
/* 2914 */ 732037,
/* 2915 */ 9999,
/* 2916 */ 9999,
/* 2917 */ 732037,
/* 2918 */ 9999,
/* 2919 */ 9999,
/* 2920 */ 9999,
/* 2921 */ 9999,
/* 2922 */ 741007,
/* 2923 */ 9999,
/* 2924 */ 9999,
/* 2925 */ 740007,
/* 2926 */ 9999,
/* 2927 */ 741008,
/* 2928 */ 741018,
/* 2929 */ 9999,
/* 2930 */ 741018,
/* 2931 */ 9999,
/* 2932 */ 9999,
/* 2933 */ 741018,
/* 2934 */ 9999,
/* 2935 */ 9999,
/* 2936 */ 745015,
/* 2937 */ 9999,
/* 2938 */ 9999,
/* 2939 */ 9999,
/* 2940 */ 745013,
/* 2941 */ 9999,
/* 2942 */ 9999,
/* 2943 */ 746013,
/* 2944 */ 746020,
/* 2945 */ 746027,
/* 2946 */ 9999,
/* 2947 */ 9999,
/* 2948 */ 746011,
/* 2949 */ 9999,
/* 2950 */ 9999,
/* 2951 */ 747010,
/* 2952 */ 747017,
/* 2953 */ 747024,
/* 2954 */ 9999,
/* 2955 */ 9999,
/* 2956 */ 747008,
/* 2957 */ 9999,
/* 2958 */ 9999,
/* 2959 */ 748009,
/* 2960 */ 748016,
/* 2961 */ 9999,
/* 2962 */ 748030,
/* 2963 */ 9999,
/* 2964 */ 9999,
/* 2965 */ 748007,
/* 2966 */ 9999,
/* 2967 */ 9999,
/* 2968 */ 750012,
/* 2969 */ 751007,
/* 2970 */ 9999,
/* 2971 */ 9999,
/* 2972 */ 750010,
/* 2973 */ 9999,
/* 2974 */ 9999,
/* 2975 */ 752011,
/* 2976 */ 753007,
/* 2977 */ 9999,
/* 2978 */ 9999,
/* 2979 */ 752009,
/* 2980 */ 9999,
/* 2981 */ 9999,
/* 2982 */ 754012,
/* 2983 */ 754019,
/* 2984 */ 9999,
/* 2985 */ 9999,
/* 2986 */ 754010,
/* 2987 */ 9999,
/* 2988 */ 9999,
/* 2989 */ 756012,
/* 2990 */ 756018,
/* 2991 */ 756029,
/* 2992 */ 757007,
/* 2993 */ 757014,
/* 2994 */ 9999,
/* 2995 */ 9999,
/* 2996 */ 756010,
/* 2997 */ 9999,
/* 2998 */ 9999,
/* 2999 */ 758019,
/* 3000 */ 759007,
/* 3001 */ 9999,
/* 3002 */ 9999,
/* 3003 */ 758017,
/* 3004 */ 9999,
/* 3005 */ 9999,
/* 3006 */ 760011,
/* 3007 */ 760019,
/* 3008 */ 9999,
/* 3009 */ 9999,
/* 3010 */ 760009,
/* 3011 */ 9999,
/* 3012 */ 761005,
/* 3013 */ 9999,
/* 3014 */ 761013,
/* 3015 */ 9999,
/* 3016 */ 762005,
/* 3017 */ 9999,
/* 3018 */ 762014,
/* 3019 */ 9999,
/* 3020 */ 763005,
/* 3021 */ 9999,
/* 3022 */ 763016,
/* 3023 */ 9999,
/* 3024 */ 764005,
/* 3025 */ 9999,
/* 3026 */ 764017,
/* 3027 */ 9999,
/* 3028 */ 9999,
/* 3029 */ 9999,
/* 3030 */ 745016,
/* 3031 */ 9999,
/* 3032 */ 9999,
/* 3033 */ 745016,
/* 3034 */ 9999,
/* 3035 */ 9999,
/* 3036 */ 9999,
/* 3037 */ 746014,
/* 3038 */ 9999,
/* 3039 */ 9999,
/* 3040 */ 746014,
/* 3041 */ 9999,
/* 3042 */ 9999,
/* 3043 */ 9999,
/* 3044 */ 746021,
/* 3045 */ 9999,
/* 3046 */ 9999,
/* 3047 */ 746021,
/* 3048 */ 9999,
/* 3049 */ 9999,
/* 3050 */ 9999,
/* 3051 */ 747011,
/* 3052 */ 9999,
/* 3053 */ 9999,
/* 3054 */ 747011,
/* 3055 */ 9999,
/* 3056 */ 9999,
/* 3057 */ 9999,
/* 3058 */ 747018,
/* 3059 */ 9999,
/* 3060 */ 9999,
/* 3061 */ 747018,
/* 3062 */ 9999,
/* 3063 */ 9999,
/* 3064 */ 9999,
/* 3065 */ 748010,
/* 3066 */ 9999,
/* 3067 */ 9999,
/* 3068 */ 748010,
/* 3069 */ 9999,
/* 3070 */ 9999,
/* 3071 */ 9999,
/* 3072 */ 750013,
/* 3073 */ 9999,
/* 3074 */ 9999,
/* 3075 */ 750013,
/* 3076 */ 9999,
/* 3077 */ 751008,
/* 3078 */ 751020,
/* 3079 */ 751062,
/* 3080 */ 751013,
/* 3081 */ 9999,
/* 3082 */ 751013,
/* 3083 */ 9999,
/* 3084 */ 9999,
/* 3085 */ 751013,
/* 3086 */ 9999,
/* 3087 */ 9999,
/* 3088 */ 9999,
/* 3089 */ 751063,
/* 3090 */ 9999,
/* 3091 */ 9999,
/* 3092 */ 751063,
/* 3093 */ 9999,
/* 3094 */ 9999,
/* 3095 */ 9999,
/* 3096 */ 752012,
/* 3097 */ 9999,
/* 3098 */ 9999,
/* 3099 */ 752012,
/* 3100 */ 9999,
/* 3101 */ 753008,
/* 3102 */ 753020,
/* 3103 */ 753060,
/* 3104 */ 753013,
/* 3105 */ 9999,
/* 3106 */ 753013,
/* 3107 */ 9999,
/* 3108 */ 9999,
/* 3109 */ 753013,
/* 3110 */ 9999,
/* 3111 */ 9999,
/* 3112 */ 9999,
/* 3113 */ 753061,
/* 3114 */ 9999,
/* 3115 */ 9999,
/* 3116 */ 753061,
/* 3117 */ 9999,
/* 3118 */ 9999,
/* 3119 */ 9999,
/* 3120 */ 754013,
/* 3121 */ 9999,
/* 3122 */ 9999,
/* 3123 */ 754013,
/* 3124 */ 9999,
/* 3125 */ 754020,
/* 3126 */ 754054,
/* 3127 */ 754034,
/* 3128 */ 9999,
/* 3129 */ 754034,
/* 3130 */ 9999,
/* 3131 */ 9999,
/* 3132 */ 754034,
/* 3133 */ 9999,
/* 3134 */ 9999,
/* 3135 */ 9999,
/* 3136 */ 754055,
/* 3137 */ 9999,
/* 3138 */ 9999,
/* 3139 */ 754055,
/* 3140 */ 9999,
/* 3141 */ 9999,
/* 3142 */ 9999,
/* 3143 */ 756013,
/* 3144 */ 9999,
/* 3145 */ 9999,
/* 3146 */ 9999,
/* 3147 */ 756030,
/* 3148 */ 9999,
/* 3149 */ 9999,
/* 3150 */ 9999,
/* 3151 */ 757008,
/* 3152 */ 9999,
/* 3153 */ 9999,
/* 3154 */ 757008,
/* 3155 */ 9999,
/* 3156 */ 757016,
/* 3157 */ 757061,
/* 3158 */ 757021,
/* 3159 */ 9999,
/* 3160 */ 757021,
/* 3161 */ 9999,
/* 3162 */ 9999,
/* 3163 */ 757021,
/* 3164 */ 9999,
/* 3165 */ 9999,
/* 3166 */ 9999,
/* 3167 */ 757062,
/* 3168 */ 9999,
/* 3169 */ 9999,
/* 3170 */ 757062,
/* 3171 */ 9999,
/* 3172 */ 9999,
/* 3173 */ 9999,
/* 3174 */ 758020,
/* 3175 */ 9999,
/* 3176 */ 9999,
/* 3177 */ 758020,
/* 3178 */ 9999,
/* 3179 */ 759008,
/* 3180 */ 759054,
/* 3181 */ 759013,
/* 3182 */ 9999,
/* 3183 */ 759013,
/* 3184 */ 9999,
/* 3185 */ 9999,
/* 3186 */ 759013,
/* 3187 */ 9999,
/* 3188 */ 9999,
/* 3189 */ 9999,
/* 3190 */ 759055,
/* 3191 */ 9999,
/* 3192 */ 9999,
/* 3193 */ 759055,
/* 3194 */ 9999,
/* 3195 */ 9999,
/* 3196 */ 9999,
/* 3197 */ 760012,
/* 3198 */ 9999,
/* 3199 */ 9999,
/* 3200 */ 760012,
/* 3201 */ 9999,
/* 3202 */ 760020,
/* 3203 */ 760030,
/* 3204 */ 760025,
/* 3205 */ 9999,
/* 3206 */ 760025,
/* 3207 */ 9999,
/* 3208 */ 9999,
/* 3209 */ 760025,
/* 3210 */ 9999,
/* 3211 */ 9999,
/* 3212 */ 9999,
/* 3213 */ 760031,
/* 3214 */ 9999,
/* 3215 */ 9999,
/* 3216 */ 9999,
/* 3217 */ 760037,
/* 3218 */ 9999,
/* 3219 */ 9999,
/* 3220 */ 760037,
/* 3221 */ 9999,
/* 3222 */ 9999,
/* 3223 */ 9999,
/* 3224 */ 768027,
/* 3225 */ 768038,
/* 3226 */ 9999,
/* 3227 */ 9999,
/* 3228 */ 768076,
/* 3229 */ 9999,
/* 3230 */ 768003,
/* 3231 */ 9999,
/* 3232 */ 9999,
/* 3233 */ 9999,
/* 3234 */ 770029,
/* 3235 */ 770040,
/* 3236 */ 9999,
/* 3237 */ 9999,
/* 3238 */ 770078,
/* 3239 */ 9999,
/* 3240 */ 770005,
/* 3241 */ 9999,
/* 3242 */ 9999,
/* 3243 */ 9999,
/* 3244 */ 772029,
/* 3245 */ 772040,
/* 3246 */ 9999,
/* 3247 */ 9999,
/* 3248 */ 772078,
/* 3249 */ 9999,
/* 3250 */ 772005,
/* 3251 */ 9999,
/* 3252 */ 768039,
/* 3253 */ 9999,
/* 3254 */ 768048,
/* 3255 */ 9999,
/* 3256 */ 9999,
/* 3257 */ 768048,
/* 3258 */ 9999,
/* 3259 */ 770041,
/* 3260 */ 9999,
/* 3261 */ 770050,
/* 3262 */ 9999,
/* 3263 */ 9999,
/* 3264 */ 770050,
/* 3265 */ 9999,
/* 3266 */ 772041,
/* 3267 */ 9999,
/* 3268 */ 772050,
/* 3269 */ 9999,
/* 3270 */ 9999,
/* 3271 */ 772050,
/* 3272 */ 9999,
/* 3273 */ 9999,
/* 3274 */ 777007,
/* 3275 */ 9999,
/* 3276 */ 777003,
/* 3277 */ 9999,
/* 3278 */ 9999,
/* 3279 */ 778009,
/* 3280 */ 9999,
/* 3281 */ 778005,
/* 3282 */ 9999,
/* 3283 */ 9999,
/* 3284 */ 779009,
/* 3285 */ 9999,
/* 3286 */ 779024,
/* 3287 */ 9999,
/* 3288 */ 779005,
/* 3289 */ 9999,
/* 3290 */ 783003,
/* 3291 */ 783015,
/* 3292 */ 9999,
/* 3293 */ 783008,
/* 3294 */ 9999,
/* 3295 */ 9999,
/* 3296 */ 785011,
/* 3297 */ 9999,
/* 3298 */ 9999,
/* 3299 */ 785009,
/* 3300 */ 9999,
/* 3301 */ 9999,
/* 3302 */ 9999,
/* 3303 */ 9999,
/* 3304 */ 787009,
/* 3305 */ 9999,
/* 3306 */ 9999,
/* 3307 */ 9999,
/* 3308 */ 9999,
/* 3309 */ 9999,
/* 3310 */ 789034,
/* 3311 */ 789046,
/* 3312 */ 9999,
/* 3313 */ 789009,
/* 3314 */ 9999,
/* 3315 */ 9999,
/* 3316 */ 791011,
/* 3317 */ 9999,
/* 3318 */ 9999,
/* 3319 */ 9999,
/* 3320 */ 791046,
/* 3321 */ 791058,
/* 3322 */ 9999,
/* 3323 */ 791009,
/* 3324 */ 9999,
/* 3325 */ 9999,
/* 3326 */ 9999,
/* 3327 */ 783025,
/* 3328 */ 9999,
/* 3329 */ 9999,
/* 3330 */ 783025,
/* 3331 */ 9999,
/* 3332 */ 9999,
/* 3333 */ 9999,
/* 3334 */ 789056,
/* 3335 */ 9999,
/* 3336 */ 9999,
/* 3337 */ 789056,
/* 3338 */ 9999,
/* 3339 */ 9999,
/* 3340 */ 9999,
/* 3341 */ 791068,
/* 3342 */ 9999,
/* 3343 */ 9999,
/* 3344 */ 791068,
/* 3345 */ 9999,
/* 3346 */ 9999,
/* 3347 */ 9999,
/* 3348 */ 9999,
/* 3349 */ 797007,
/* 3350 */ 9999,
/* 3351 */ 9999,
/* 3352 */ 796009,
/* 3353 */ 9999,
/* 3354 */ 797008,
/* 3355 */ 797016,
/* 3356 */ 9999,
/* 3357 */ 797016,
/* 3358 */ 9999,
/* 3359 */ 9999,
/* 3360 */ 797016,
/* 3361 */ 9999,
/* 3362 */ 9999,
/* 3363 */ 801015,
/* 3364 */ 9999,
/* 3365 */ 9999,
/* 3366 */ 9999,
/* 3367 */ 801013,
/* 3368 */ 9999,
/* 3369 */ 802005,
/* 3370 */ 9999,
/* 3371 */ 802016,
/* 3372 */ 9999,
/* 3373 */ 803005,
/* 3374 */ 9999,
/* 3375 */ 803014,
/* 3376 */ 9999,
/* 3377 */ 804005,
/* 3378 */ 9999,
/* 3379 */ 804017,
/* 3380 */ 9999,
/* 3381 */ 9999,
/* 3382 */ 9999,
/* 3383 */ 801016,
/* 3384 */ 9999,
/* 3385 */ 9999,
/* 3386 */ 801016,
/* 3387 */ 9999,
/* 3388 */ 9999,
/* 3389 */ 808013,
/* 3390 */ 809005,
/* 3391 */ 9999,
/* 3392 */ 9999,
/* 3393 */ 808011,
/* 3394 */ 9999,
/* 3395 */ 9999,
/* 3396 */ 812011,
/* 3397 */ 813005,
/* 3398 */ 9999,
/* 3399 */ 812009,
/* 3400 */ 9999,
/* 3401 */ 9999,
/* 3402 */ 820014,
/* 3403 */ 821005,
/* 3404 */ 9999,
/* 3405 */ 9999,
/* 3406 */ 820012,
/* 3407 */ 9999,
/* 3408 */ 9999,
/* 3409 */ 9999,
/* 3410 */ 808014,
/* 3411 */ 9999,
/* 3412 */ 9999,
/* 3413 */ 808014,
/* 3414 */ 9999,
/* 3415 */ 9999,
/* 3416 */ 809011,
/* 3417 */ 809023,
/* 3418 */ 809071,
/* 3419 */ 809007,
/* 3420 */ 9999,
/* 3421 */ 809007,
/* 3422 */ 9999,
/* 3423 */ 9999,
/* 3424 */ 810011,
/* 3425 */ 810023,
/* 3426 */ 810070,
/* 3427 */ 810007,
/* 3428 */ 9999,
/* 3429 */ 810007,
/* 3430 */ 9999,
/* 3431 */ 9999,
/* 3432 */ 810007,
/* 3433 */ 9999,
/* 3434 */ 9999,
/* 3435 */ 9999,
/* 3436 */ 809072,
/* 3437 */ 9999,
/* 3438 */ 9999,
/* 3439 */ 809072,
/* 3440 */ 9999,
/* 3441 */ 9999,
/* 3442 */ 9999,
/* 3443 */ 810071,
/* 3444 */ 9999,
/* 3445 */ 9999,
/* 3446 */ 810071,
/* 3447 */ 9999,
/* 3448 */ 9999,
/* 3449 */ 9999,
/* 3450 */ 812012,
/* 3451 */ 9999,
/* 3452 */ 9999,
/* 3453 */ 812012,
/* 3454 */ 9999,
/* 3455 */ 9999,
/* 3456 */ 813011,
/* 3457 */ 813044,
/* 3458 */ 813058,
/* 3459 */ 813007,
/* 3460 */ 9999,
/* 3461 */ 813007,
/* 3462 */ 9999,
/* 3463 */ 9999,
/* 3464 */ 813007,
/* 3465 */ 9999,
/* 3466 */ 813045,
/* 3467 */ 813054,
/* 3468 */ 9999,
/* 3469 */ 813054,
/* 3470 */ 9999,
/* 3471 */ 9999,
/* 3472 */ 813054,
/* 3473 */ 9999,
/* 3474 */ 9999,
/* 3475 */ 9999,
/* 3476 */ 813059,
/* 3477 */ 9999,
/* 3478 */ 9999,
/* 3479 */ 9999,
/* 3480 */ 813063,
/* 3481 */ 9999,
/* 3482 */ 9999,
/* 3483 */ 9999,
/* 3484 */ 820015,
/* 3485 */ 9999,
/* 3486 */ 9999,
/* 3487 */ 820015,
/* 3488 */ 9999,
/* 3489 */ 9999,
/* 3490 */ 821035,
/* 3491 */ 822007,
/* 3492 */ 825011,
/* 3493 */ 821007,
/* 3494 */ 9999,
/* 3495 */ 821007,
/* 3496 */ 9999,
/* 3497 */ 9999,
/* 3498 */ 821007,
/* 3499 */ 9999,
/* 3500 */ 821037,
/* 3501 */ 9999,
/* 3502 */ 9999,
/* 3503 */ 821042,
/* 3504 */ 9999,
/* 3505 */ 9999,
/* 3506 */ 821042,
/* 3507 */ 9999,
/* 3508 */ 822011,
/* 3509 */ 9999,
/* 3510 */ 9999,
/* 3511 */ 822037,
/* 3512 */ 9999,
/* 3513 */ 822017,
/* 3514 */ 9999,
/* 3515 */ 823011,
/* 3516 */ 9999,
/* 3517 */ 823026,
/* 3518 */ 9999,
/* 3519 */ 9999,
/* 3520 */ 823017,
/* 3521 */ 9999,
/* 3522 */ 824011,
/* 3523 */ 9999,
/* 3524 */ 9999,
/* 3525 */ 824037,
/* 3526 */ 9999,
/* 3527 */ 824017,
/* 3528 */ 9999,
/* 3529 */ 9999,
/* 3530 */ 9999,
/* 3531 */ 825012,
/* 3532 */ 9999,
/* 3533 */ 9999,
/* 3534 */ 825012,
/* 3535 */ 9999,
/* 3536 */ 829005,
/* 3537 */ 9999,
/* 3538 */ 829011,
/* 3539 */ 9999,
/* 3540 */ 830005,
/* 3541 */ 9999,
/* 3542 */ 830010,
/* 3543 */ 9999,
/* 3544 */ 9999,
/* 3545 */ 9999,
/* 3546 */ 831011,
/* 3547 */ 9999,
/* 3548 */ 9999,
/* 3549 */ 9999,
/* 3550 */ 9999,
/* 3551 */ 836007,
/* 3552 */ 9999,
/* 3553 */ 9999,
/* 3554 */ 835010,
/* 3555 */ 9999,
/* 3556 */ 836008,
/* 3557 */ 836017,
/* 3558 */ 9999,
/* 3559 */ 836017,
/* 3560 */ 9999,
/* 3561 */ 9999,
/* 3562 */ 836017,
/* 3563 */ 9999,
/* 3564 */ 9999,
/* 3565 */ 840015,
/* 3566 */ 9999,
/* 3567 */ 9999,
/* 3568 */ 9999,
/* 3569 */ 840013,
/* 3570 */ 9999,
/* 3571 */ 9999,
/* 3572 */ 841010,
/* 3573 */ 841017,
/* 3574 */ 9999,
/* 3575 */ 9999,
/* 3576 */ 841008,
/* 3577 */ 9999,
/* 3578 */ 842005,
/* 3579 */ 9999,
/* 3580 */ 9999,
/* 3581 */ 842005,
/* 3582 */ 9999,
/* 3583 */ 9999,
/* 3584 */ 843013,
/* 3585 */ 843020,
/* 3586 */ 9999,
/* 3587 */ 9999,
/* 3588 */ 843011,
/* 3589 */ 9999,
/* 3590 */ 9999,
/* 3591 */ 844014,
/* 3592 */ 844021,
/* 3593 */ 9999,
/* 3594 */ 9999,
/* 3595 */ 844012,
/* 3596 */ 9999,
/* 3597 */ 9999,
/* 3598 */ 845015,
/* 3599 */ 845022,
/* 3600 */ 9999,
/* 3601 */ 9999,
/* 3602 */ 845013,
/* 3603 */ 9999,
/* 3604 */ 9999,
/* 3605 */ 9999,
/* 3606 */ 847049,
/* 3607 */ 9999,
/* 3608 */ 9999,
/* 3609 */ 847009,
/* 3610 */ 9999,
/* 3611 */ 9999,
/* 3612 */ 9999,
/* 3613 */ 848049,
/* 3614 */ 9999,
/* 3615 */ 9999,
/* 3616 */ 848009,
/* 3617 */ 9999,
/* 3618 */ 9999,
/* 3619 */ 9999,
/* 3620 */ 849049,
/* 3621 */ 9999,
/* 3622 */ 9999,
/* 3623 */ 849007,
/* 3624 */ 9999,
/* 3625 */ 9999,
/* 3626 */ 9999,
/* 3627 */ 850049,
/* 3628 */ 9999,
/* 3629 */ 9999,
/* 3630 */ 850010,
/* 3631 */ 9999,
/* 3632 */ 851005,
/* 3633 */ 9999,
/* 3634 */ 851014,
/* 3635 */ 9999,
/* 3636 */ 852005,
/* 3637 */ 9999,
/* 3638 */ 852017,
/* 3639 */ 9999,
/* 3640 */ 9999,
/* 3641 */ 9999,
/* 3642 */ 840016,
/* 3643 */ 9999,
/* 3644 */ 9999,
/* 3645 */ 840016,
/* 3646 */ 9999,
/* 3647 */ 9999,
/* 3648 */ 9999,
/* 3649 */ 841011,
/* 3650 */ 9999,
/* 3651 */ 9999,
/* 3652 */ 841011,
/* 3653 */ 9999,
/* 3654 */ 9999,
/* 3655 */ 842020,
/* 3656 */ 842027,
/* 3657 */ 842070,
/* 3658 */ 842014,
/* 3659 */ 9999,
/* 3660 */ 842014,
/* 3661 */ 9999,
/* 3662 */ 9999,
/* 3663 */ 842014,
/* 3664 */ 9999,
/* 3665 */ 9999,
/* 3666 */ 9999,
/* 3667 */ 842021,
/* 3668 */ 9999,
/* 3669 */ 9999,
/* 3670 */ 842021,
/* 3671 */ 9999,
/* 3672 */ 9999,
/* 3673 */ 9999,
/* 3674 */ 842071,
/* 3675 */ 9999,
/* 3676 */ 9999,
/* 3677 */ 842071,
/* 3678 */ 9999,
/* 3679 */ 9999,
/* 3680 */ 9999,
/* 3681 */ 843014,
/* 3682 */ 9999,
/* 3683 */ 9999,
/* 3684 */ 843014,
/* 3685 */ 9999,
/* 3686 */ 843021,
/* 3687 */ 843062,
/* 3688 */ 843026,
/* 3689 */ 9999,
/* 3690 */ 843026,
/* 3691 */ 9999,
/* 3692 */ 9999,
/* 3693 */ 843026,
/* 3694 */ 9999,
/* 3695 */ 9999,
/* 3696 */ 9999,
/* 3697 */ 843063,
/* 3698 */ 9999,
/* 3699 */ 9999,
/* 3700 */ 843063,
/* 3701 */ 9999,
/* 3702 */ 9999,
/* 3703 */ 9999,
/* 3704 */ 844015,
/* 3705 */ 9999,
/* 3706 */ 9999,
/* 3707 */ 844015,
/* 3708 */ 9999,
/* 3709 */ 9999,
/* 3710 */ 844074,
/* 3711 */ 844030,
/* 3712 */ 9999,
/* 3713 */ 844030,
/* 3714 */ 9999,
/* 3715 */ 9999,
/* 3716 */ 844030,
/* 3717 */ 9999,
/* 3718 */ 9999,
/* 3719 */ 9999,
/* 3720 */ 844075,
/* 3721 */ 9999,
/* 3722 */ 9999,
/* 3723 */ 844075,
/* 3724 */ 9999,
/* 3725 */ 9999,
/* 3726 */ 9999,
/* 3727 */ 845016,
/* 3728 */ 9999,
/* 3729 */ 9999,
/* 3730 */ 845016,
/* 3731 */ 9999,
/* 3732 */ 845023,
/* 3733 */ 845064,
/* 3734 */ 845028,
/* 3735 */ 9999,
/* 3736 */ 845028,
/* 3737 */ 9999,
/* 3738 */ 9999,
/* 3739 */ 845028,
/* 3740 */ 9999,
/* 3741 */ 9999,
/* 3742 */ 9999,
/* 3743 */ 845065,
/* 3744 */ 9999,
/* 3745 */ 9999,
/* 3746 */ 845065,
/* 3747 */ 9999,
/* 3748 */ 847050,
/* 3749 */ 847060,
/* 3750 */ 847058,
/* 3751 */ 9999,
/* 3752 */ 847058,
/* 3753 */ 9999,
/* 3754 */ 9999,
/* 3755 */ 847058,
/* 3756 */ 9999,
/* 3757 */ 9999,
/* 3758 */ 9999,
/* 3759 */ 847061,
/* 3760 */ 9999,
/* 3761 */ 848050,
/* 3762 */ 848060,
/* 3763 */ 848058,
/* 3764 */ 9999,
/* 3765 */ 848058,
/* 3766 */ 9999,
/* 3767 */ 9999,
/* 3768 */ 848058,
/* 3769 */ 9999,
/* 3770 */ 9999,
/* 3771 */ 9999,
/* 3772 */ 848061,
/* 3773 */ 9999,
/* 3774 */ 849050,
/* 3775 */ 849060,
/* 3776 */ 849058,
/* 3777 */ 9999,
/* 3778 */ 849058,
/* 3779 */ 9999,
/* 3780 */ 9999,
/* 3781 */ 849058,
/* 3782 */ 9999,
/* 3783 */ 9999,
/* 3784 */ 9999,
/* 3785 */ 849061,
/* 3786 */ 9999,
/* 3787 */ 850050,
/* 3788 */ 850060,
/* 3789 */ 850058,
/* 3790 */ 9999,
/* 3791 */ 850058,
/* 3792 */ 9999,
/* 3793 */ 9999,
/* 3794 */ 850058,
/* 3795 */ 9999,
/* 3796 */ 9999,
/* 3797 */ 9999,
/* 3798 */ 850061,
/* 3799 */ 9999,
/* 3800 */ 9999,
/* 3801 */ 9999,
/* 3802 */ 856015,
/* 3803 */ 9999,
/* 3804 */ 856030,
/* 3805 */ 9999,
/* 3806 */ 9999,
/* 3807 */ 856009,
/* 3808 */ 9999,
/* 3809 */ 9999,
/* 3810 */ 9999,
/* 3811 */ 858015,
/* 3812 */ 9999,
/* 3813 */ 9999,
/* 3814 */ 858009,
/* 3815 */ 9999,
/* 3816 */ 860005,
/* 3817 */ 860017,
/* 3818 */ 9999,
/* 3819 */ 860011,
/* 3820 */ 9999,
/* 3821 */ 862005,
/* 3822 */ 9999,
/* 3823 */ 862023,
/* 3824 */ 9999,
/* 3825 */ 9999,
/* 3826 */ 862011,
/* 3827 */ 9999,
/* 3828 */ 864005,
/* 3829 */ 864017,
/* 3830 */ 864027,
/* 3831 */ 9999,
/* 3832 */ 9999,
/* 3833 */ 864011,
/* 3834 */ 9999,
/* 3835 */ 867005,
/* 3836 */ 867017,
/* 3837 */ 867027,
/* 3838 */ 9999,
/* 3839 */ 9999,
/* 3840 */ 867011,
/* 3841 */ 9999,
/* 3842 */ 9999,
/* 3843 */ 870012,
/* 3844 */ 9999,
/* 3845 */ 870010,
/* 3846 */ 9999,
/* 3847 */ 9999,
/* 3848 */ 872010,
/* 3849 */ 9999,
/* 3850 */ 872008,
/* 3851 */ 9999,
/* 3852 */ 9999,
/* 3853 */ 9999,
/* 3854 */ 9999,
/* 3855 */ 860019,
/* 3856 */ 9999,
/* 3857 */ 9999,
/* 3858 */ 860019,
/* 3859 */ 9999,
/* 3860 */ 9999,
/* 3861 */ 9999,
/* 3862 */ 864028,
/* 3863 */ 9999,
/* 3864 */ 9999,
/* 3865 */ 9999,
/* 3866 */ 864034,
/* 3867 */ 9999,
/* 3868 */ 9999,
/* 3869 */ 9999,
/* 3870 */ 867028,
/* 3871 */ 9999,
/* 3872 */ 9999,
/* 3873 */ 9999,
/* 3874 */ 867034,
/* 3875 */ 9999,
/* 3876 */ 9999,
/* 3877 */ 9999,
/* 3878 */ 9999,
/* 3879 */ 876038,
/* 3880 */ 9999,
/* 3881 */ 878007,
/* 3882 */ 9999,
/* 3883 */ 9999,
/* 3884 */ 876018,
/* 3885 */ 9999,
/* 3886 */ 878008,
/* 3887 */ 878019,
/* 3888 */ 9999,
/* 3889 */ 878019,
/* 3890 */ 9999,
/* 3891 */ 9999,
/* 3892 */ 878019,
/* 3893 */ 9999,
/* 3894 */ 9999,
/* 3895 */ 884010,
/* 3896 */ 884017,
/* 3897 */ 9999,
/* 3898 */ 9999,
/* 3899 */ 883003,
/* 3900 */ 9999,
/* 3901 */ 9999,
/* 3902 */ 888012,
/* 3903 */ 888019,
/* 3904 */ 9999,
/* 3905 */ 9999,
/* 3906 */ 888010,
/* 3907 */ 9999,
/* 3908 */ 9999,
/* 3909 */ 890015,
/* 3910 */ 9999,
/* 3911 */ 9999,
/* 3912 */ 9999,
/* 3913 */ 890013,
/* 3914 */ 9999,
/* 3915 */ 891005,
/* 3916 */ 9999,
/* 3917 */ 891014,
/* 3918 */ 9999,
/* 3919 */ 892005,
/* 3920 */ 9999,
/* 3921 */ 892016,
/* 3922 */ 9999,
/* 3923 */ 893005,
/* 3924 */ 9999,
/* 3925 */ 893017,
/* 3926 */ 9999,
/* 3927 */ 9999,
/* 3928 */ 9999,
/* 3929 */ 884011,
/* 3930 */ 9999,
/* 3931 */ 9999,
/* 3932 */ 884011,
/* 3933 */ 9999,
/* 3934 */ 884018,
/* 3935 */ 884072,
/* 3936 */ 884024,
/* 3937 */ 9999,
/* 3938 */ 884024,
/* 3939 */ 9999,
/* 3940 */ 9999,
/* 3941 */ 884024,
/* 3942 */ 9999,
/* 3943 */ 9999,
/* 3944 */ 9999,
/* 3945 */ 884073,
/* 3946 */ 9999,
/* 3947 */ 9999,
/* 3948 */ 884073,
/* 3949 */ 9999,
/* 3950 */ 9999,
/* 3951 */ 9999,
/* 3952 */ 888013,
/* 3953 */ 9999,
/* 3954 */ 9999,
/* 3955 */ 888013,
/* 3956 */ 9999,
/* 3957 */ 888020,
/* 3958 */ 888054,
/* 3959 */ 888034,
/* 3960 */ 9999,
/* 3961 */ 888034,
/* 3962 */ 9999,
/* 3963 */ 9999,
/* 3964 */ 888034,
/* 3965 */ 9999,
/* 3966 */ 9999,
/* 3967 */ 9999,
/* 3968 */ 888055,
/* 3969 */ 9999,
/* 3970 */ 9999,
/* 3971 */ 888055,
/* 3972 */ 9999,
/* 3973 */ 9999,
/* 3974 */ 9999,
/* 3975 */ 890016,
/* 3976 */ 9999,
/* 3977 */ 9999,
/* 3978 */ 890016,
/* 3979 */ 9999,
/* 3980 */ 9999,
/* 3981 */ 9999,
/* 3982 */ 9999,
/* 3983 */ 897007,
/* 3984 */ 9999,
/* 3985 */ 9999,
/* 3986 */ 896012,
/* 3987 */ 9999,
/* 3988 */ 897008,
/* 3989 */ 897016,
/* 3990 */ 9999,
/* 3991 */ 897016,
/* 3992 */ 9999,
/* 3993 */ 9999,
/* 3994 */ 897016,
/* 3995 */ 9999,
/* 3996 */ 9999,
/* 3997 */ 903011,
/* 3998 */ 903018,
/* 3999 */ 9999,
/* 4000 */ 9999,
/* 4001 */ 902003,
/* 4002 */ 9999,
/* 4003 */ 9999,
/* 4004 */ 9999,
/* 4005 */ 9999,
/* 4006 */ 907013,
/* 4007 */ 9999,
/* 4008 */ 9999,
/* 4009 */ 908011,
/* 4010 */ 908018,
/* 4011 */ 9999,
/* 4012 */ 9999,
/* 4013 */ 908009,
/* 4014 */ 9999,
/* 4015 */ 9999,
/* 4016 */ 909015,
/* 4017 */ 9999,
/* 4018 */ 9999,
/* 4019 */ 9999,
/* 4020 */ 909013,
/* 4021 */ 9999,
/* 4022 */ 9999,
/* 4023 */ 910012,
/* 4024 */ 910019,
/* 4025 */ 9999,
/* 4026 */ 9999,
/* 4027 */ 910010,
/* 4028 */ 9999,
/* 4029 */ 912005,
/* 4030 */ 9999,
/* 4031 */ 912014,
/* 4032 */ 9999,
/* 4033 */ 913005,
/* 4034 */ 9999,
/* 4035 */ 913016,
/* 4036 */ 9999,
/* 4037 */ 914005,
/* 4038 */ 9999,
/* 4039 */ 914017,
/* 4040 */ 9999,
/* 4041 */ 9999,
/* 4042 */ 9999,
/* 4043 */ 903012,
/* 4044 */ 9999,
/* 4045 */ 9999,
/* 4046 */ 903012,
/* 4047 */ 9999,
/* 4048 */ 903019,
/* 4049 */ 903074,
/* 4050 */ 903025,
/* 4051 */ 9999,
/* 4052 */ 903025,
/* 4053 */ 9999,
/* 4054 */ 9999,
/* 4055 */ 903025,
/* 4056 */ 9999,
/* 4057 */ 9999,
/* 4058 */ 9999,
/* 4059 */ 903075,
/* 4060 */ 9999,
/* 4061 */ 9999,
/* 4062 */ 903075,
/* 4063 */ 9999,
/* 4064 */ 9999,
/* 4065 */ 9999,
/* 4066 */ 908012,
/* 4067 */ 9999,
/* 4068 */ 9999,
/* 4069 */ 908012,
/* 4070 */ 9999,
/* 4071 */ 908019,
/* 4072 */ 908029,
/* 4073 */ 908024,
/* 4074 */ 9999,
/* 4075 */ 908024,
/* 4076 */ 9999,
/* 4077 */ 9999,
/* 4078 */ 908024,
/* 4079 */ 9999,
/* 4080 */ 9999,
/* 4081 */ 9999,
/* 4082 */ 908030,
/* 4083 */ 9999,
/* 4084 */ 9999,
/* 4085 */ 9999,
/* 4086 */ 908036,
/* 4087 */ 9999,
/* 4088 */ 9999,
/* 4089 */ 908036,
/* 4090 */ 9999,
/* 4091 */ 9999,
/* 4092 */ 9999,
/* 4093 */ 909016,
/* 4094 */ 9999,
/* 4095 */ 9999,
/* 4096 */ 909016,
/* 4097 */ 9999,
/* 4098 */ 9999,
/* 4099 */ 9999,
/* 4100 */ 910013,
/* 4101 */ 9999,
/* 4102 */ 9999,
/* 4103 */ 910013,
/* 4104 */ 9999,
/* 4105 */ 910020,
/* 4106 */ 910054,
/* 4107 */ 910034,
/* 4108 */ 9999,
/* 4109 */ 910034,
/* 4110 */ 9999,
/* 4111 */ 9999,
/* 4112 */ 910034,
/* 4113 */ 9999,
/* 4114 */ 9999,
/* 4115 */ 9999,
/* 4116 */ 910055,
/* 4117 */ 9999,
/* 4118 */ 9999,
/* 4119 */ 910055,
/* 4120 */ 9999,
/* 4121 */ 9999,
/* 4122 */ 9999,
/* 4123 */ 9999,
/* 4124 */ 919007,
/* 4125 */ 9999,
/* 4126 */ 9999,
/* 4127 */ 917020,
/* 4128 */ 9999,
/* 4129 */ 919008,
/* 4130 */ 919020,
/* 4131 */ 9999,
/* 4132 */ 919020,
/* 4133 */ 9999,
/* 4134 */ 9999,
/* 4135 */ 919020,
/* 4136 */ 9999,
/* 4137 */ 9999,
/* 4138 */ 924015,
/* 4139 */ 9999,
/* 4140 */ 9999,
/* 4141 */ 9999,
/* 4142 */ 924013,
/* 4143 */ 9999,
/* 4144 */ 925005,
/* 4145 */ 9999,
/* 4146 */ 925015,
/* 4147 */ 9999,
/* 4148 */ 9999,
/* 4149 */ 926010,
/* 4150 */ 926017,
/* 4151 */ 9999,
/* 4152 */ 9999,
/* 4153 */ 9999,
/* 4154 */ 926008,
/* 4155 */ 9999,
/* 4156 */ 9999,
/* 4157 */ 927013,
/* 4158 */ 927020,
/* 4159 */ 9999,
/* 4160 */ 9999,
/* 4161 */ 927011,
/* 4162 */ 9999,
/* 4163 */ 9999,
/* 4164 */ 928014,
/* 4165 */ 928021,
/* 4166 */ 9999,
/* 4167 */ 9999,
/* 4168 */ 928012,
/* 4169 */ 9999,
/* 4170 */ 9999,
/* 4171 */ 929011,
/* 4172 */ 929019,
/* 4173 */ 9999,
/* 4174 */ 9999,
/* 4175 */ 929009,
/* 4176 */ 9999,
/* 4177 */ 930005,
/* 4178 */ 9999,
/* 4179 */ 930016,
/* 4180 */ 9999,
/* 4181 */ 931005,
/* 4182 */ 9999,
/* 4183 */ 931014,
/* 4184 */ 9999,
/* 4185 */ 932005,
/* 4186 */ 9999,
/* 4187 */ 932017,
/* 4188 */ 9999,
/* 4189 */ 9999,
/* 4190 */ 9999,
/* 4191 */ 924016,
/* 4192 */ 9999,
/* 4193 */ 9999,
/* 4194 */ 924016,
/* 4195 */ 9999,
/* 4196 */ 9999,
/* 4197 */ 9999,
/* 4198 */ 926011,
/* 4199 */ 9999,
/* 4200 */ 9999,
/* 4201 */ 926011,
/* 4202 */ 9999,
/* 4203 */ 9999,
/* 4204 */ 9999,
/* 4205 */ 927014,
/* 4206 */ 9999,
/* 4207 */ 9999,
/* 4208 */ 927014,
/* 4209 */ 9999,
/* 4210 */ 9999,
/* 4211 */ 9999,
/* 4212 */ 928015,
/* 4213 */ 9999,
/* 4214 */ 9999,
/* 4215 */ 928015,
/* 4216 */ 9999,
/* 4217 */ 9999,
/* 4218 */ 9999,
/* 4219 */ 929012,
/* 4220 */ 9999,
/* 4221 */ 9999,
/* 4222 */ 929012,
/* 4223 */ 9999,
/* 4224 */ 929020,
/* 4225 */ 929030,
/* 4226 */ 929025,
/* 4227 */ 9999,
/* 4228 */ 929025,
/* 4229 */ 9999,
/* 4230 */ 9999,
/* 4231 */ 929025,
/* 4232 */ 9999,
/* 4233 */ 9999,
/* 4234 */ 9999,
/* 4235 */ 929031,
/* 4236 */ 9999,
/* 4237 */ 9999,
/* 4238 */ 9999,
/* 4239 */ 929037,
/* 4240 */ 9999,
/* 4241 */ 9999,
/* 4242 */ 929037,
/* 4243 */ 9999,
/* 4244 */ 9999,
/* 4245 */ 9999,
/* 4246 */ 9999,
/* 4247 */ 935038,
/* 4248 */ 9999,
/* 4249 */ 938007,
/* 4250 */ 9999,
/* 4251 */ 9999,
/* 4252 */ 935018,
/* 4253 */ 9999,
/* 4254 */ 938008,
/* 4255 */ 938019,
/* 4256 */ 9999,
/* 4257 */ 938019,
/* 4258 */ 9999,
/* 4259 */ 9999,
/* 4260 */ 938019,
/* 4261 */ 9999,
/* 4262 */ 9999,
/* 4263 */ 943015,
/* 4264 */ 9999,
/* 4265 */ 9999,
/* 4266 */ 9999,
/* 4267 */ 943013,
/* 4268 */ 9999,
/* 4269 */ 9999,
/* 4270 */ 944011,
/* 4271 */ 944018,
/* 4272 */ 9999,
/* 4273 */ 9999,
/* 4274 */ 944009,
/* 4275 */ 9999,
/* 4276 */ 9999,
/* 4277 */ 945011,
/* 4278 */ 945018,
/* 4279 */ 9999,
/* 4280 */ 9999,
/* 4281 */ 945009,
/* 4282 */ 9999,
/* 4283 */ 9999,
/* 4284 */ 946011,
/* 4285 */ 946018,
/* 4286 */ 9999,
/* 4287 */ 9999,
/* 4288 */ 946008,
/* 4289 */ 9999,
/* 4290 */ 9999,
/* 4291 */ 947011,
/* 4292 */ 947018,
/* 4293 */ 9999,
/* 4294 */ 9999,
/* 4295 */ 947009,
/* 4296 */ 9999,
/* 4297 */ 9999,
/* 4298 */ 948011,
/* 4299 */ 948018,
/* 4300 */ 9999,
/* 4301 */ 9999,
/* 4302 */ 948008,
/* 4303 */ 9999,
/* 4304 */ 9999,
/* 4305 */ 949015,
/* 4306 */ 949022,
/* 4307 */ 9999,
/* 4308 */ 9999,
/* 4309 */ 949013,
/* 4310 */ 9999,
/* 4311 */ 9999,
/* 4312 */ 951011,
/* 4313 */ 951019,
/* 4314 */ 9999,
/* 4315 */ 9999,
/* 4316 */ 951009,
/* 4317 */ 9999,
/* 4318 */ 9999,
/* 4319 */ 9999,
/* 4320 */ 9999,
/* 4321 */ 9999,
/* 4322 */ 9999,
/* 4323 */ 952008,
/* 4324 */ 9999,
/* 4325 */ 9999,
/* 4326 */ 9999,
/* 4327 */ 955015,
/* 4328 */ 9999,
/* 4329 */ 9999,
/* 4330 */ 955009,
/* 4331 */ 9999,
/* 4332 */ 9999,
/* 4333 */ 9999,
/* 4334 */ 971017,
/* 4335 */ 9999,
/* 4336 */ 971011,
/* 4337 */ 9999,
/* 4338 */ 9999,
/* 4339 */ 9999,
/* 4340 */ 972017,
/* 4341 */ 9999,
/* 4342 */ 972011,
/* 4343 */ 9999,
/* 4344 */ 973005,
/* 4345 */ 9999,
/* 4346 */ 973016,
/* 4347 */ 9999,
/* 4348 */ 974005,
/* 4349 */ 9999,
/* 4350 */ 974014,
/* 4351 */ 9999,
/* 4352 */ 975005,
/* 4353 */ 9999,
/* 4354 */ 975017,
/* 4355 */ 9999,
/* 4356 */ 9999,
/* 4357 */ 9999,
/* 4358 */ 943016,
/* 4359 */ 9999,
/* 4360 */ 9999,
/* 4361 */ 943016,
/* 4362 */ 9999,
/* 4363 */ 9999,
/* 4364 */ 9999,
/* 4365 */ 944012,
/* 4366 */ 9999,
/* 4367 */ 9999,
/* 4368 */ 944012,
/* 4369 */ 9999,
/* 4370 */ 9999,
/* 4371 */ 9999,
/* 4372 */ 945012,
/* 4373 */ 9999,
/* 4374 */ 9999,
/* 4375 */ 945012,
/* 4376 */ 9999,
/* 4377 */ 9999,
/* 4378 */ 9999,
/* 4379 */ 946012,
/* 4380 */ 9999,
/* 4381 */ 9999,
/* 4382 */ 946012,
/* 4383 */ 9999,
/* 4384 */ 9999,
/* 4385 */ 9999,
/* 4386 */ 947012,
/* 4387 */ 9999,
/* 4388 */ 9999,
/* 4389 */ 947012,
/* 4390 */ 9999,
/* 4391 */ 9999,
/* 4392 */ 9999,
/* 4393 */ 948012,
/* 4394 */ 9999,
/* 4395 */ 9999,
/* 4396 */ 948012,
/* 4397 */ 9999,
/* 4398 */ 9999,
/* 4399 */ 9999,
/* 4400 */ 949016,
/* 4401 */ 9999,
/* 4402 */ 9999,
/* 4403 */ 949016,
/* 4404 */ 9999,
/* 4405 */ 949023,
/* 4406 */ 950006,
/* 4407 */ 949028,
/* 4408 */ 9999,
/* 4409 */ 949028,
/* 4410 */ 9999,
/* 4411 */ 9999,
/* 4412 */ 949028,
/* 4413 */ 9999,
/* 4414 */ 9999,
/* 4415 */ 9999,
/* 4416 */ 950007,
/* 4417 */ 9999,
/* 4418 */ 9999,
/* 4419 */ 9999,
/* 4420 */ 950013,
/* 4421 */ 9999,
/* 4422 */ 9999,
/* 4423 */ 950013,
/* 4424 */ 9999,
/* 4425 */ 9999,
/* 4426 */ 9999,
/* 4427 */ 951012,
/* 4428 */ 9999,
/* 4429 */ 9999,
/* 4430 */ 951012,
/* 4431 */ 9999,
/* 4432 */ 951020,
/* 4433 */ 951030,
/* 4434 */ 951025,
/* 4435 */ 9999,
/* 4436 */ 951025,
/* 4437 */ 9999,
/* 4438 */ 9999,
/* 4439 */ 951025,
/* 4440 */ 9999,
/* 4441 */ 9999,
/* 4442 */ 9999,
/* 4443 */ 951031,
/* 4444 */ 9999,
/* 4445 */ 9999,
/* 4446 */ 9999,
/* 4447 */ 951037,
/* 4448 */ 9999,
/* 4449 */ 9999,
/* 4450 */ 951037,
/* 4451 */ 9999,
/* 4452 */ 9999,
/* 4453 */ 9999,
/* 4454 */ 957025,
/* 4455 */ 958006,
/* 4456 */ 970003,
/* 4457 */ 955035,
/* 4458 */ 9999,
/* 4459 */ 955035,
/* 4460 */ 9999,
/* 4461 */ 9999,
/* 4462 */ 955035,
/* 4463 */ 9999,
/* 4464 */ 9999,
/* 4465 */ 958014,
/* 4466 */ 9999,
/* 4467 */ 958010,
/* 4468 */ 9999,
/* 4469 */ 958010,
/* 4470 */ 9999,
/* 4471 */ 9999,
/* 4472 */ 959015,
/* 4473 */ 964010,
/* 4474 */ 959013,
/* 4475 */ 9999,
/* 4476 */ 959013,
/* 4477 */ 9999,
/* 4478 */ 9999,
/* 4479 */ 968013,
/* 4480 */ 968011,
/* 4481 */ 9999,
/* 4482 */ 968011,
/* 4483 */ 9999,
/* 4484 */ 9999,
/* 4485 */ 968011,
/* 4486 */ 9999,
/* 4487 */ 959017,
/* 4488 */ 9999,
/* 4489 */ 959023,
/* 4490 */ 9999,
/* 4491 */ 9999,
/* 4492 */ 9999,
/* 4493 */ 960031,
/* 4494 */ 9999,
/* 4495 */ 960024,
/* 4496 */ 9999,
/* 4497 */ 9999,
/* 4498 */ 9999,
/* 4499 */ 962034,
/* 4500 */ 9999,
/* 4501 */ 962024,
/* 4502 */ 9999,
/* 4503 */ 960032,
/* 4504 */ 9999,
/* 4505 */ 960036,
/* 4506 */ 9999,
/* 4507 */ 9999,
/* 4508 */ 960036,
/* 4509 */ 9999,
/* 4510 */ 962035,
/* 4511 */ 9999,
/* 4512 */ 962039,
/* 4513 */ 9999,
/* 4514 */ 9999,
/* 4515 */ 962039,
/* 4516 */ 9999,
/* 4517 */ 9999,
/* 4518 */ 964014,
/* 4519 */ 9999,
/* 4520 */ 964012,
/* 4521 */ 9999,
/* 4522 */ 9999,
/* 4523 */ 9999,
/* 4524 */ 965023,
/* 4525 */ 9999,
/* 4526 */ 965035,
/* 4527 */ 9999,
/* 4528 */ 965014,
/* 4529 */ 9999,
/* 4530 */ 9999,
/* 4531 */ 967016,
/* 4532 */ 9999,
/* 4533 */ 967014,
/* 4534 */ 9999,
/* 4535 */ 9999,
/* 4536 */ 967014,
/* 4537 */ 9999,
/* 4538 */ 9999,
/* 4539 */ 9999,
/* 4540 */ 970004,
/* 4541 */ 9999,
/* 4542 */ 9999,
/* 4543 */ 970004,
/* 4544 */ 9999,
/* 4545 */ 971018,
/* 4546 */ 971029,
/* 4547 */ 9999,
/* 4548 */ 971029,
/* 4549 */ 9999,
/* 4550 */ 9999,
/* 4551 */ 971029,
/* 4552 */ 9999,
/* 4553 */ 972018,
/* 4554 */ 972029,
/* 4555 */ 9999,
/* 4556 */ 972029,
/* 4557 */ 9999,
/* 4558 */ 9999,
/* 4559 */ 972029,
/* 4560 */ 9999,
/* 4561 */ 9999,
/* 4562 */ 9999,
/* 4563 */ 9999,
/* 4564 */ 980009,
/* 4565 */ 981005,
/* 4566 */ 9999,
/* 4567 */ 982007,
/* 4568 */ 9999,
/* 4569 */ 9999,
/* 4570 */ 978027,
/* 4571 */ 9999,
/* 4572 */ 9999,
/* 4573 */ 981010,
/* 4574 */ 9999,
/* 4575 */ 981008,
/* 4576 */ 9999,
/* 4577 */ 9999,
/* 4578 */ 981008,
/* 4579 */ 9999,
/* 4580 */ 982008,
/* 4581 */ 982021,
/* 4582 */ 9999,
/* 4583 */ 982021,
/* 4584 */ 9999,
/* 4585 */ 9999,
/* 4586 */ 982021,
/* 4587 */ 9999,
/* 4588 */ 9999,
/* 4589 */ 987015,
/* 4590 */ 9999,
/* 4591 */ 9999,
/* 4592 */ 9999,
/* 4593 */ 987013,
/* 4594 */ 9999,
/* 4595 */ 9999,
/* 4596 */ 988009,
/* 4597 */ 988016,
/* 4598 */ 9999,
/* 4599 */ 9999,
/* 4600 */ 988007,
/* 4601 */ 9999,
/* 4602 */ 9999,
/* 4603 */ 989011,
/* 4604 */ 989018,
/* 4605 */ 9999,
/* 4606 */ 9999,
/* 4607 */ 989009,
/* 4608 */ 9999,
/* 4609 */ 990005,
/* 4610 */ 9999,
/* 4611 */ 990015,
/* 4612 */ 9999,
/* 4613 */ 991005,
/* 4614 */ 991028,
/* 4615 */ 991035,
/* 4616 */ 9999,
/* 4617 */ 9999,
/* 4618 */ 991005,
/* 4619 */ 9999,
/* 4620 */ 9999,
/* 4621 */ 992013,
/* 4622 */ 992020,
/* 4623 */ 9999,
/* 4624 */ 9999,
/* 4625 */ 992011,
/* 4626 */ 9999,
/* 4627 */ 9999,
/* 4628 */ 993013,
/* 4629 */ 993020,
/* 4630 */ 9999,
/* 4631 */ 9999,
/* 4632 */ 993007,
/* 4633 */ 9999,
/* 4634 */ 9999,
/* 4635 */ 994013,
/* 4636 */ 994020,
/* 4637 */ 9999,
/* 4638 */ 9999,
/* 4639 */ 994007,
/* 4640 */ 9999,
/* 4641 */ 9999,
/* 4642 */ 995013,
/* 4643 */ 995020,
/* 4644 */ 9999,
/* 4645 */ 9999,
/* 4646 */ 995007,
/* 4647 */ 9999,
/* 4648 */ 9999,
/* 4649 */ 996013,
/* 4650 */ 996020,
/* 4651 */ 9999,
/* 4652 */ 9999,
/* 4653 */ 996007,
/* 4654 */ 9999,
/* 4655 */ 9999,
/* 4656 */ 997013,
/* 4657 */ 997020,
/* 4658 */ 9999,
/* 4659 */ 9999,
/* 4660 */ 997009,
/* 4661 */ 9999,
/* 4662 */ 9999,
/* 4663 */ 9999,
/* 4664 */ 998024,
/* 4665 */ 998031,
/* 4666 */ 9999,
/* 4667 */ 9999,
/* 4668 */ 998010,
/* 4669 */ 9999,
/* 4670 */ 9999,
/* 4671 */ 999013,
/* 4672 */ 999020,
/* 4673 */ 9999,
/* 4674 */ 9999,
/* 4675 */ 999008,
/* 4676 */ 9999,
/* 4677 */ 9999,
/* 4678 */ 1000013,
/* 4679 */ 1000020,
/* 4680 */ 9999,
/* 4681 */ 9999,
/* 4682 */ 1000009,
/* 4683 */ 9999,
/* 4684 */ 9999,
/* 4685 */ 1001013,
/* 4686 */ 1001020,
/* 4687 */ 9999,
/* 4688 */ 9999,
/* 4689 */ 1001008,
/* 4690 */ 9999,
/* 4691 */ 9999,
/* 4692 */ 1002019,
/* 4693 */ 1002026,
/* 4694 */ 1002035,
/* 4695 */ 9999,
/* 4696 */ 1002013,
/* 4697 */ 9999,
/* 4698 */ 9999,
/* 4699 */ 1003012,
/* 4700 */ 1003019,
/* 4701 */ 1003037,
/* 4702 */ 9999,
/* 4703 */ 9999,
/* 4704 */ 1003010,
/* 4705 */ 9999,
/* 4706 */ 9999,
/* 4707 */ 9999,
/* 4708 */ 1005015,
/* 4709 */ 9999,
/* 4710 */ 9999,
/* 4711 */ 9999,
/* 4712 */ 1005009,
/* 4713 */ 9999,
/* 4714 */ 9999,
/* 4715 */ 9999,
/* 4716 */ 1007015,
/* 4717 */ 9999,
/* 4718 */ 1007030,
/* 4719 */ 9999,
/* 4720 */ 9999,
/* 4721 */ 9999,
/* 4722 */ 1007009,
/* 4723 */ 9999,
/* 4724 */ 9999,
/* 4725 */ 9999,
/* 4726 */ 1009015,
/* 4727 */ 9999,
/* 4728 */ 1009030,
/* 4729 */ 9999,
/* 4730 */ 1009046,
/* 4731 */ 9999,
/* 4732 */ 9999,
/* 4733 */ 9999,
/* 4734 */ 1009009,
/* 4735 */ 9999,
/* 4736 */ 9999,
/* 4737 */ 9999,
/* 4738 */ 1012015,
/* 4739 */ 9999,
/* 4740 */ 1012030,
/* 4741 */ 9999,
/* 4742 */ 1012046,
/* 4743 */ 9999,
/* 4744 */ 1012062,
/* 4745 */ 9999,
/* 4746 */ 9999,
/* 4747 */ 9999,
/* 4748 */ 1012009,
/* 4749 */ 9999,
/* 4750 */ 9999,
/* 4751 */ 1016013,
/* 4752 */ 1016020,
/* 4753 */ 9999,
/* 4754 */ 9999,
/* 4755 */ 1016011,
/* 4756 */ 9999,
/* 4757 */ 9999,
/* 4758 */ 1019013,
/* 4759 */ 1019020,
/* 4760 */ 9999,
/* 4761 */ 9999,
/* 4762 */ 1019010,
/* 4763 */ 9999,
/* 4764 */ 9999,
/* 4765 */ 1022011,
/* 4766 */ 1022018,
/* 4767 */ 1023019,
/* 4768 */ 9999,
/* 4769 */ 9999,
/* 4770 */ 1022009,
/* 4771 */ 9999,
/* 4772 */ 9999,
/* 4773 */ 1024012,
/* 4774 */ 1024019,
/* 4775 */ 9999,
/* 4776 */ 9999,
/* 4777 */ 1024010,
/* 4778 */ 9999,
/* 4779 */ 9999,
/* 4780 */ 1027013,
/* 4781 */ 1027020,
/* 4782 */ 9999,
/* 4783 */ 9999,
/* 4784 */ 1027011,
/* 4785 */ 9999,
/* 4786 */ 9999,
/* 4787 */ 1029012,
/* 4788 */ 1029019,
/* 4789 */ 9999,
/* 4790 */ 9999,
/* 4791 */ 1029010,
/* 4792 */ 9999,
/* 4793 */ 9999,
/* 4794 */ 1031011,
/* 4795 */ 1031018,
/* 4796 */ 9999,
/* 4797 */ 1031009,
/* 4798 */ 9999,
/* 4799 */ 1039005,
/* 4800 */ 9999,
/* 4801 */ 1039013,
/* 4802 */ 9999,
/* 4803 */ 1040005,
/* 4804 */ 9999,
/* 4805 */ 1040014,
/* 4806 */ 9999,
/* 4807 */ 1041005,
/* 4808 */ 9999,
/* 4809 */ 1041016,
/* 4810 */ 9999,
/* 4811 */ 1042005,
/* 4812 */ 9999,
/* 4813 */ 1042017,
/* 4814 */ 9999,
/* 4815 */ 9999,
/* 4816 */ 9999,
/* 4817 */ 987016,
/* 4818 */ 9999,
/* 4819 */ 9999,
/* 4820 */ 987016,
/* 4821 */ 9999,
/* 4822 */ 9999,
/* 4823 */ 9999,
/* 4824 */ 988010,
/* 4825 */ 9999,
/* 4826 */ 9999,
/* 4827 */ 988010,
/* 4828 */ 9999,
/* 4829 */ 9999,
/* 4830 */ 9999,
/* 4831 */ 989012,
/* 4832 */ 9999,
/* 4833 */ 9999,
/* 4834 */ 989012,
/* 4835 */ 9999,
/* 4836 */ 9999,
/* 4837 */ 9999,
/* 4838 */ 991015,
/* 4839 */ 9999,
/* 4840 */ 9999,
/* 4841 */ 9999,
/* 4842 */ 991025,
/* 4843 */ 9999,
/* 4844 */ 9999,
/* 4845 */ 9999,
/* 4846 */ 991029,
/* 4847 */ 9999,
/* 4848 */ 9999,
/* 4849 */ 991029,
/* 4850 */ 9999,
/* 4851 */ 9999,
/* 4852 */ 9999,
/* 4853 */ 992014,
/* 4854 */ 9999,
/* 4855 */ 9999,
/* 4856 */ 992014,
/* 4857 */ 9999,
/* 4858 */ 9999,
/* 4859 */ 9999,
/* 4860 */ 993014,
/* 4861 */ 9999,
/* 4862 */ 9999,
/* 4863 */ 993014,
/* 4864 */ 9999,
/* 4865 */ 9999,
/* 4866 */ 9999,
/* 4867 */ 994014,
/* 4868 */ 9999,
/* 4869 */ 9999,
/* 4870 */ 994014,
/* 4871 */ 9999,
/* 4872 */ 9999,
/* 4873 */ 9999,
/* 4874 */ 995014,
/* 4875 */ 9999,
/* 4876 */ 9999,
/* 4877 */ 995014,
/* 4878 */ 9999,
/* 4879 */ 9999,
/* 4880 */ 9999,
/* 4881 */ 996014,
/* 4882 */ 9999,
/* 4883 */ 9999,
/* 4884 */ 996014,
/* 4885 */ 9999,
/* 4886 */ 9999,
/* 4887 */ 9999,
/* 4888 */ 997014,
/* 4889 */ 9999,
/* 4890 */ 9999,
/* 4891 */ 997014,
/* 4892 */ 9999,
/* 4893 */ 9999,
/* 4894 */ 9999,
/* 4895 */ 998025,
/* 4896 */ 9999,
/* 4897 */ 9999,
/* 4898 */ 998025,
/* 4899 */ 9999,
/* 4900 */ 9999,
/* 4901 */ 9999,
/* 4902 */ 999014,
/* 4903 */ 9999,
/* 4904 */ 9999,
/* 4905 */ 999014,
/* 4906 */ 9999,
/* 4907 */ 9999,
/* 4908 */ 9999,
/* 4909 */ 1000014,
/* 4910 */ 9999,
/* 4911 */ 9999,
/* 4912 */ 1000014,
/* 4913 */ 9999,
/* 4914 */ 9999,
/* 4915 */ 9999,
/* 4916 */ 1001014,
/* 4917 */ 9999,
/* 4918 */ 9999,
/* 4919 */ 1001014,
/* 4920 */ 9999,
/* 4921 */ 9999,
/* 4922 */ 9999,
/* 4923 */ 1002020,
/* 4924 */ 9999,
/* 4925 */ 9999,
/* 4926 */ 1002020,
/* 4927 */ 9999,
/* 4928 */ 9999,
/* 4929 */ 9999,
/* 4930 */ 1002036,
/* 4931 */ 9999,
/* 4932 */ 9999,
/* 4933 */ 9999,
/* 4934 */ 1002040,
/* 4935 */ 9999,
/* 4936 */ 9999,
/* 4937 */ 9999,
/* 4938 */ 1003013,
/* 4939 */ 9999,
/* 4940 */ 9999,
/* 4941 */ 1003013,
/* 4942 */ 9999,
/* 4943 */ 9999,
/* 4944 */ 1003042,
/* 4945 */ 1003038,
/* 4946 */ 9999,
/* 4947 */ 1003038,
/* 4948 */ 9999,
/* 4949 */ 9999,
/* 4950 */ 1003038,
/* 4951 */ 9999,
/* 4952 */ 9999,
/* 4953 */ 9999,
/* 4954 */ 1016014,
/* 4955 */ 9999,
/* 4956 */ 9999,
/* 4957 */ 1016014,
/* 4958 */ 9999,
/* 4959 */ 1016021,
/* 4960 */ 1018007,
/* 4961 */ 1016021,
/* 4962 */ 9999,
/* 4963 */ 1016021,
/* 4964 */ 9999,
/* 4965 */ 9999,
/* 4966 */ 1016021,
/* 4967 */ 9999,
/* 4968 */ 1016023,
/* 4969 */ 9999,
/* 4970 */ 1016028,
/* 4971 */ 9999,
/* 4972 */ 1017015,
/* 4973 */ 1017025,
/* 4974 */ 9999,
/* 4975 */ 1017015,
/* 4976 */ 9999,
/* 4977 */ 9999,
/* 4978 */ 9999,
/* 4979 */ 1017016,
/* 4980 */ 9999,
/* 4981 */ 9999,
/* 4982 */ 9999,
/* 4983 */ 1017020,
/* 4984 */ 9999,
/* 4985 */ 9999,
/* 4986 */ 9999,
/* 4987 */ 1018008,
/* 4988 */ 9999,
/* 4989 */ 9999,
/* 4990 */ 1018008,
/* 4991 */ 9999,
/* 4992 */ 9999,
/* 4993 */ 9999,
/* 4994 */ 1019014,
/* 4995 */ 9999,
/* 4996 */ 9999,
/* 4997 */ 1019014,
/* 4998 */ 9999,
/* 4999 */ 1019021,
/* 5000 */ 1021007,
/* 5001 */ 1019021,
/* 5002 */ 9999,
/* 5003 */ 1019021,
/* 5004 */ 9999,
/* 5005 */ 9999,
/* 5006 */ 1019021,
/* 5007 */ 9999,
/* 5008 */ 1019023,
/* 5009 */ 9999,
/* 5010 */ 1019028,
/* 5011 */ 9999,
/* 5012 */ 1020015,
/* 5013 */ 1020025,
/* 5014 */ 9999,
/* 5015 */ 1020015,
/* 5016 */ 9999,
/* 5017 */ 9999,
/* 5018 */ 9999,
/* 5019 */ 1020016,
/* 5020 */ 9999,
/* 5021 */ 9999,
/* 5022 */ 9999,
/* 5023 */ 1020020,
/* 5024 */ 9999,
/* 5025 */ 9999,
/* 5026 */ 9999,
/* 5027 */ 1021008,
/* 5028 */ 9999,
/* 5029 */ 9999,
/* 5030 */ 1021008,
/* 5031 */ 9999,
/* 5032 */ 9999,
/* 5033 */ 9999,
/* 5034 */ 1022012,
/* 5035 */ 9999,
/* 5036 */ 9999,
/* 5037 */ 1022012,
/* 5038 */ 9999,
/* 5039 */ 1022019,
/* 5040 */ 9999,
/* 5041 */ 1022024,
/* 5042 */ 9999,
/* 5043 */ 1022024,
/* 5044 */ 9999,
/* 5045 */ 9999,
/* 5046 */ 1022024,
/* 5047 */ 9999,
/* 5048 */ 9999,
/* 5049 */ 9999,
/* 5050 */ 1024013,
/* 5051 */ 9999,
/* 5052 */ 9999,
/* 5053 */ 1024013,
/* 5054 */ 9999,
/* 5055 */ 1024021,
/* 5056 */ 1026006,
/* 5057 */ 1024027,
/* 5058 */ 9999,
/* 5059 */ 1024027,
/* 5060 */ 9999,
/* 5061 */ 9999,
/* 5062 */ 1024027,
/* 5063 */ 9999,
/* 5064 */ 9999,
/* 5065 */ 9999,
/* 5066 */ 1026007,
/* 5067 */ 9999,
/* 5068 */ 9999,
/* 5069 */ 1026007,
/* 5070 */ 9999,
/* 5071 */ 9999,
/* 5072 */ 9999,
/* 5073 */ 1027014,
/* 5074 */ 9999,
/* 5075 */ 9999,
/* 5076 */ 1027014,
/* 5077 */ 9999,
/* 5078 */ 1027022,
/* 5079 */ 1028006,
/* 5080 */ 1027028,
/* 5081 */ 9999,
/* 5082 */ 1027028,
/* 5083 */ 9999,
/* 5084 */ 9999,
/* 5085 */ 1027028,
/* 5086 */ 9999,
/* 5087 */ 9999,
/* 5088 */ 9999,
/* 5089 */ 1028007,
/* 5090 */ 9999,
/* 5091 */ 9999,
/* 5092 */ 1028007,
/* 5093 */ 9999,
/* 5094 */ 9999,
/* 5095 */ 9999,
/* 5096 */ 1029013,
/* 5097 */ 9999,
/* 5098 */ 9999,
/* 5099 */ 1029013,
/* 5100 */ 9999,
/* 5101 */ 1029021,
/* 5102 */ 1030006,
/* 5103 */ 1029027,
/* 5104 */ 9999,
/* 5105 */ 1029027,
/* 5106 */ 9999,
/* 5107 */ 9999,
/* 5108 */ 1029027,
/* 5109 */ 9999,
/* 5110 */ 9999,
/* 5111 */ 9999,
/* 5112 */ 1030007,
/* 5113 */ 9999,
/* 5114 */ 9999,
/* 5115 */ 1030007,
/* 5116 */ 9999,
/* 5117 */ 9999,
/* 5118 */ 9999,
/* 5119 */ 1031012,
/* 5120 */ 9999,
/* 5121 */ 9999,
/* 5122 */ 1031012,
/* 5123 */ 9999,
/* 5124 */ 1031020,
/* 5125 */ 1038006,
/* 5126 */ 1031020,
/* 5127 */ 9999,
/* 5128 */ 1031020,
/* 5129 */ 9999,
/* 5130 */ 9999,
/* 5131 */ 1031020,
/* 5132 */ 9999,
/* 5133 */ 1032006,
/* 5134 */ 9999,
/* 5135 */ 1032011,
/* 5136 */ 9999,
/* 5137 */ 1037008,
/* 5138 */ 1037018,
/* 5139 */ 9999,
/* 5140 */ 1037008,
/* 5141 */ 9999,
/* 5142 */ 9999,
/* 5143 */ 9999,
/* 5144 */ 1037009,
/* 5145 */ 9999,
/* 5146 */ 9999,
/* 5147 */ 9999,
/* 5148 */ 1037013,
/* 5149 */ 9999,
/* 5150 */ 9999,
/* 5151 */ 9999,
/* 5152 */ 1038007,
/* 5153 */ 9999,
/* 5154 */ 9999,
/* 5155 */ 9999,
/* 5156 */ 1038011,
/* 5157 */ 9999,
/* 5158 */ 1047003,
/* 5159 */ 9999,
/* 5160 */ 1048005,
/* 5161 */ 1050003,
/* 5162 */ 1052003,
/* 5163 */ 1053005,
/* 5164 */ 1064003,
/* 5165 */ 9999,
/* 5166 */ 1045032,
/* 5167 */ 9999,
/* 5168 */ 1077005,
/* 5169 */ 9999,
/* 5170 */ 1077020,
/* 5171 */ 1079019,
/* 5172 */ 9999,
/* 5173 */ 1077010,
/* 5174 */ 9999,
/* 5175 */ 1048007,
/* 5176 */ 9999,
/* 5177 */ 1048015,
/* 5178 */ 9999,
/* 5179 */ 1049009,
/* 5180 */ 9999,
/* 5181 */ 9999,
/* 5182 */ 1049014,
/* 5183 */ 9999,
/* 5184 */ 9999,
/* 5185 */ 1049014,
/* 5186 */ 9999,
/* 5187 */ 1050004,
/* 5188 */ 9999,
/* 5189 */ 1050009,
/* 5190 */ 9999,
/* 5191 */ 9999,
/* 5192 */ 1050009,
/* 5193 */ 9999,
/* 5194 */ 9999,
/* 5195 */ 9999,
/* 5196 */ 1052006,
/* 5197 */ 9999,
/* 5198 */ 9999,
/* 5199 */ 1052006,
/* 5200 */ 9999,
/* 5201 */ 9999,
/* 5202 */ 1053011,
/* 5203 */ 1053040,
/* 5204 */ 9999,
/* 5205 */ 9999,
/* 5206 */ 1053007,
/* 5207 */ 9999,
/* 5208 */ 9999,
/* 5209 */ 1053007,
/* 5210 */ 9999,
/* 5211 */ 9999,
/* 5212 */ 9999,
/* 5213 */ 1053018,
/* 5214 */ 9999,
/* 5215 */ 9999,
/* 5216 */ 9999,
/* 5217 */ 1053026,
/* 5218 */ 9999,
/* 5219 */ 9999,
/* 5220 */ 9999,
/* 5221 */ 1053037,
/* 5222 */ 9999,
/* 5223 */ 9999,
/* 5224 */ 1065005,
/* 5225 */ 1066005,
/* 5226 */ 1067005,
/* 5227 */ 1068005,
/* 5228 */ 1064006,
/* 5229 */ 9999,
/* 5230 */ 1064006,
/* 5231 */ 9999,
/* 5232 */ 9999,
/* 5233 */ 1064006,
/* 5234 */ 9999,
/* 5235 */ 1065007,
/* 5236 */ 9999,
/* 5237 */ 1065015,
/* 5238 */ 9999,
/* 5239 */ 9999,
/* 5240 */ 1065015,
/* 5241 */ 9999,
/* 5242 */ 1066006,
/* 5243 */ 9999,
/* 5244 */ 1066011,
/* 5245 */ 9999,
/* 5246 */ 9999,
/* 5247 */ 1066011,
/* 5248 */ 9999,
/* 5249 */ 9999,
/* 5250 */ 9999,
/* 5251 */ 1067008,
/* 5252 */ 9999,
/* 5253 */ 9999,
/* 5254 */ 1067008,
/* 5255 */ 9999,
/* 5256 */ 9999,
/* 5257 */ 1068011,
/* 5258 */ 1068040,
/* 5259 */ 9999,
/* 5260 */ 9999,
/* 5261 */ 1068007,
/* 5262 */ 9999,
/* 5263 */ 9999,
/* 5264 */ 1068007,
/* 5265 */ 9999,
/* 5266 */ 9999,
/* 5267 */ 9999,
/* 5268 */ 1068018,
/* 5269 */ 9999,
/* 5270 */ 9999,
/* 5271 */ 9999,
/* 5272 */ 1068026,
/* 5273 */ 9999,
/* 5274 */ 9999,
/* 5275 */ 9999,
/* 5276 */ 1068037,
/* 5277 */ 9999,
/* 5278 */ 1079021,
/* 5279 */ 9999,
/* 5280 */ 1079026,
/* 5281 */ 9999,
/* 5282 */ 9999,
/* 5283 */ 1079026,
/* 5284 */ 9999,
/* 5285 */ 9999,
/* 5286 */ 9999,
/* 5287 */ 1083018,
/* 5288 */ 9999,
/* 5289 */ 9999,
/* 5290 */ 9999,
/* 5291 */ 9999,
/* 5292 */ 1086028,
/* 5293 */ 9999,
/* 5294 */ 1092007,
/* 5295 */ 9999,
/* 5296 */ 9999,
/* 5297 */ 1086008,
/* 5298 */ 9999,
/* 5299 */ 1092008,
/* 5300 */ 1092019,
/* 5301 */ 9999,
/* 5302 */ 1092019,
/* 5303 */ 9999,
/* 5304 */ 9999,
/* 5305 */ 1092019,
/* 5306 */ 9999,
/* 5307 */ 9999,
/* 5308 */ 1096016,
/* 5309 */ 9999,
/* 5310 */ 9999,
/* 5311 */ 9999,
/* 5312 */ 1096013,
/* 5313 */ 9999,
/* 5314 */ 9999,
/* 5315 */ 1097016,
/* 5316 */ 1097023,
/* 5317 */ 9999,
/* 5318 */ 9999,
/* 5319 */ 1097012,
/* 5320 */ 9999,
/* 5321 */ 9999,
/* 5322 */ 1098016,
/* 5323 */ 1098023,
/* 5324 */ 9999,
/* 5325 */ 9999,
/* 5326 */ 1098014,
/* 5327 */ 9999,
/* 5328 */ 9999,
/* 5329 */ 1099016,
/* 5330 */ 1099023,
/* 5331 */ 9999,
/* 5332 */ 9999,
/* 5333 */ 9999,
/* 5334 */ 1099010,
/* 5335 */ 9999,
/* 5336 */ 9999,
/* 5337 */ 9999,
/* 5338 */ 1096017,
/* 5339 */ 9999,
/* 5340 */ 9999,
/* 5341 */ 1096017,
/* 5342 */ 9999,
/* 5343 */ 9999,
/* 5344 */ 9999,
/* 5345 */ 1097017,
/* 5346 */ 9999,
/* 5347 */ 9999,
/* 5348 */ 1097017,
/* 5349 */ 9999,
/* 5350 */ 9999,
/* 5351 */ 9999,
/* 5352 */ 1098017,
/* 5353 */ 9999,
/* 5354 */ 9999,
/* 5355 */ 1098017,
/* 5356 */ 9999,
/* 5357 */ 9999,
/* 5358 */ 9999,
/* 5359 */ 1099017,
/* 5360 */ 9999,
/* 5361 */ 9999,
/* 5362 */ 1099017,
/* 5363 */ 9999,
/* 5364 */ 9999,
/* 5365 */ 9999,
/* 5366 */ 1102018,
/* 5367 */ 9999,
/* 5368 */ 9999,
/* 5369 */ 9999,
/* 5370 */ 1105008,
/* 5371 */ 9999,
/* 5372 */ 9999,
/* 5373 */ 9999,
/* 5374 */ 1106009,
/* 5375 */ 9999,
/* 5376 */ 9999,
/* 5377 */ 9999,
/* 5378 */ 1107008,
/* 5379 */ 9999,
/* 5380 */ 9999,
/* 5381 */ 9999,
/* 5382 */ 1108009,
/* 5383 */ 9999,
/* 5384 */ 9999,
/* 5385 */ 9999,
/* 5386 */ 1109007,
/* 5387 */ 9999,
/* 5388 */ 9999,
/* 5389 */ 9999,
/* 5390 */ 1110010,
/* 5391 */ 9999,
/* 5392 */ 9999,
/* 5393 */ 9999,
/* 5394 */ 1111013,
/* 5395 */ 9999,
/* 5396 */ 9999,
/* 5397 */ 1116010,
/* 5398 */ 1116024,
/* 5399 */ 9999,
/* 5400 */ 9999,
/* 5401 */ 9999,
/* 5402 */ 1133003,
/* 5403 */ 1141004,
/* 5404 */ 9999,
/* 5405 */ 9999,
/* 5406 */ 9999,
/* 5407 */ 1115015,
/* 5408 */ 9999,
/* 5409 */ 1133005,
/* 5410 */ 1133021,
/* 5411 */ 1135005,
/* 5412 */ 1135025,
/* 5413 */ 1137005,
/* 5414 */ 9999,
/* 5415 */ 1133013,
/* 5416 */ 9999,
/* 5417 */ 9999,
/* 5418 */ 1133013,
/* 5419 */ 9999,
/* 5420 */ 9999,
/* 5421 */ 1133026,
/* 5422 */ 9999,
/* 5423 */ 1133022,
/* 5424 */ 9999,
/* 5425 */ 9999,
/* 5426 */ 1133022,
/* 5427 */ 9999,
/* 5428 */ 9999,
/* 5429 */ 9999,
/* 5430 */ 1135010,
/* 5431 */ 9999,
/* 5432 */ 9999,
/* 5433 */ 1135010,
/* 5434 */ 9999,
/* 5435 */ 9999,
/* 5436 */ 1135031,
/* 5437 */ 9999,
/* 5438 */ 1135027,
/* 5439 */ 9999,
/* 5440 */ 9999,
/* 5441 */ 1135027,
/* 5442 */ 9999,
/* 5443 */ 9999,
/* 5444 */ 1137010,
/* 5445 */ 1137026,
/* 5446 */ 1139007,
/* 5447 */ 1139027,
/* 5448 */ 1137006,
/* 5449 */ 9999,
/* 5450 */ 1137006,
/* 5451 */ 9999,
/* 5452 */ 9999,
/* 5453 */ 1137006,
/* 5454 */ 9999,
/* 5455 */ 9999,
/* 5456 */ 1137031,
/* 5457 */ 9999,
/* 5458 */ 1137027,
/* 5459 */ 9999,
/* 5460 */ 9999,
/* 5461 */ 1137027,
/* 5462 */ 9999,
/* 5463 */ 9999,
/* 5464 */ 9999,
/* 5465 */ 1139012,
/* 5466 */ 9999,
/* 5467 */ 9999,
/* 5468 */ 1139012,
/* 5469 */ 9999,
/* 5470 */ 9999,
/* 5471 */ 1139033,
/* 5472 */ 9999,
/* 5473 */ 1139029,
/* 5474 */ 9999,
/* 5475 */ 9999,
/* 5476 */ 1139029,
/* 5477 */ 9999,
/* 5478 */ 9999,
/* 5479 */ 9999,
/* 5480 */ 9999,
/* 5481 */ 1141005,
/* 5482 */ 9999,
/* 5483 */ 9999,
/* 5484 */ 1141005,
/* 5485 */ 9999,
/* 5486 */ 9999,
/* 5487 */ 1145010,
/* 5488 */ 9999,
/* 5489 */ 9999,
/* 5490 */ 1152003,
/* 5491 */ 1160004,
/* 5492 */ 9999,
/* 5493 */ 9999,
/* 5494 */ 9999,
/* 5495 */ 1144016,
/* 5496 */ 9999,
/* 5497 */ 1152005,
/* 5498 */ 1152021,
/* 5499 */ 1154005,
/* 5500 */ 1154025,
/* 5501 */ 1156005,
/* 5502 */ 9999,
/* 5503 */ 1152013,
/* 5504 */ 9999,
/* 5505 */ 9999,
/* 5506 */ 1152013,
/* 5507 */ 9999,
/* 5508 */ 9999,
/* 5509 */ 1152026,
/* 5510 */ 9999,
/* 5511 */ 1152022,
/* 5512 */ 9999,
/* 5513 */ 9999,
/* 5514 */ 1152022,
/* 5515 */ 9999,
/* 5516 */ 9999,
/* 5517 */ 9999,
/* 5518 */ 1154010,
/* 5519 */ 9999,
/* 5520 */ 9999,
/* 5521 */ 1154010,
/* 5522 */ 9999,
/* 5523 */ 9999,
/* 5524 */ 1154031,
/* 5525 */ 9999,
/* 5526 */ 1154027,
/* 5527 */ 9999,
/* 5528 */ 9999,
/* 5529 */ 1154027,
/* 5530 */ 9999,
/* 5531 */ 9999,
/* 5532 */ 1156010,
/* 5533 */ 1156026,
/* 5534 */ 1158007,
/* 5535 */ 1158027,
/* 5536 */ 1156006,
/* 5537 */ 9999,
/* 5538 */ 1156006,
/* 5539 */ 9999,
/* 5540 */ 9999,
/* 5541 */ 1156006,
/* 5542 */ 9999,
/* 5543 */ 9999,
/* 5544 */ 1156031,
/* 5545 */ 9999,
/* 5546 */ 1156027,
/* 5547 */ 9999,
/* 5548 */ 9999,
/* 5549 */ 1156027,
/* 5550 */ 9999,
/* 5551 */ 9999,
/* 5552 */ 9999,
/* 5553 */ 1158012,
/* 5554 */ 9999,
/* 5555 */ 9999,
/* 5556 */ 1158012,
/* 5557 */ 9999,
/* 5558 */ 9999,
/* 5559 */ 1158033,
/* 5560 */ 9999,
/* 5561 */ 1158029,
/* 5562 */ 9999,
/* 5563 */ 9999,
/* 5564 */ 1158029,
/* 5565 */ 9999,
/* 5566 */ 9999,
/* 5567 */ 9999,
/* 5568 */ 9999,
/* 5569 */ 1160005,
/* 5570 */ 9999,
/* 5571 */ 9999,
/* 5572 */ 1160005,
/* 5573 */ 9999,
/* 5574 */ 9999,
/* 5575 */ 1164010,
/* 5576 */ 1164024,
/* 5577 */ 9999,
/* 5578 */ 9999,
/* 5579 */ 9999,
/* 5580 */ 9999,
/* 5581 */ 1163016,
/* 5582 */ 9999,
/* 5583 */ 9999,
/* 5584 */ 1197010,
/* 5585 */ 9999,
/* 5586 */ 9999,
/* 5587 */ 1197040,
/* 5588 */ 9999,
/* 5589 */ 9999,
/* 5590 */ 1196016,
/* 5591 */ 9999,
/* 5592 */ 1213003,
/* 5593 */ 9999,
/* 5594 */ 1228003,
/* 5595 */ 9999,
/* 5596 */ 9999,
/* 5597 */ 1212016,
/* 5598 */ 9999,
/* 5599 */ 9999,
/* 5600 */ 9999,
/* 5601 */ 1228004,
/* 5602 */ 9999,
/* 5603 */ 1228004,
/* 5604 */ 9999,
/* 5605 */ 9999,
/* 5606 */ 1228004,
/* 5607 */ 9999,
/* 5608 */ 1243003,
/* 5609 */ 9999,
/* 5610 */ 1260003,
/* 5611 */ 9999,
/* 5612 */ 9999,
/* 5613 */ 1242029,
/* 5614 */ 9999,
/* 5615 */ 9999,
/* 5616 */ 9999,
/* 5617 */ 1260004,
/* 5618 */ 9999,
/* 5619 */ 1260004,
/* 5620 */ 9999,
/* 5621 */ 9999,
/* 5622 */ 1260004,
/* 5623 */ 9999,
/* 5624 */ 1280003,
/* 5625 */ 9999,
/* 5626 */ 1292003,
/* 5627 */ 9999,
/* 5628 */ 9999,
/* 5629 */ 1279016,
/* 5630 */ 9999,
/* 5631 */ 9999,
/* 5632 */ 9999,
/* 5633 */ 1292004,
/* 5634 */ 9999,
/* 5635 */ 1292004,
/* 5636 */ 9999,
/* 5637 */ 9999,
/* 5638 */ 1292004,
/* 5639 */ 9999,
/* 5640 */ 1306003,
/* 5641 */ 9999,
/* 5642 */ 9999,
/* 5643 */ 1306006,
/* 5644 */ 9999,
/* 5645 */ 9999,
/* 5646 */ 9999,
/* 5647 */ 1317012,
/* 5648 */ 9999,
/* 5649 */ 1317025,
/* 5650 */ 9999,
/* 5651 */ 1317045,
/* 5652 */ 9999,
/* 5653 */ 1317006,
/* 5654 */ 9999,
/* 5655 */ 9999,
/* 5656 */ 9999,
/* 5657 */ 1332012,
/* 5658 */ 9999,
/* 5659 */ 1332025,
/* 5660 */ 9999,
/* 5661 */ 1332006,
/* 5662 */ 9999,
/* 5663 */ 9999,
/* 5664 */ 9999,
/* 5665 */ 1341013,
/* 5666 */ 9999,
/* 5667 */ 1341027,
/* 5668 */ 9999,
/* 5669 */ 1341041,
/* 5670 */ 9999,
/* 5671 */ 1342007,
/* 5672 */ 9999,
/* 5673 */ 1341007,
/* 5674 */ 9999,
/* 5675 */ 9999,
/* 5676 */ 9999,
/* 5677 */ 1368016,
/* 5678 */ 9999,
/* 5679 */ 9999,
/* 5680 */ 1378005,
/* 5681 */ 1390005,
/* 5682 */ 9999,
/* 5683 */ 9999,
/* 5684 */ 1368010,
/* 5685 */ 9999,
/* 5686 */ 9999,
/* 5687 */ 9999,
/* 5688 */ 9999,
/* 5689 */ 1439009,
/* 5690 */ 9999,
/* 5691 */ 9999,
/* 5692 */ 9999,
/* 5693 */ 9999,
/* 5694 */ 1443012,
/* 5695 */ 9999,
/* 5696 */ 9999,
/* 5697 */ 1447008,
/* 5698 */ 9999,
/* 5699 */ 9999,
/* 5700 */ 1447033,
/* 5701 */ 9999,
/* 5702 */ 9999,
/* 5703 */ 9999,
/* 5704 */ 1447006,
/* 5705 */ 9999,
/* 5706 */ 9999,
/* 5707 */ 9999,
/* 5708 */ 1456015,
/* 5709 */ 9999,
/* 5710 */ 1456028,
/* 5711 */ 9999,
/* 5712 */ 1456009,
/* 5713 */ 9999,
/* 5714 */ 1467005,
/* 5715 */ 9999,
/* 5716 */ 1467009,
/* 5717 */ 9999,
/* 5718 */ 9999,
/* 5719 */ 1471012,
/* 5720 */ 9999,
/* 5721 */ 9999,
/* 5722 */ 1471010,
/* 5723 */ 9999,
/* 5724 */ 9999,
/* 5725 */ 9999,
/* 5726 */ 9999,
/* 5727 */ 1484010,
/* 5728 */ 9999,
/* 5729 */ 9999,
/* 5730 */ 1378012,
/* 5731 */ 9999,
/* 5732 */ 1380007,
/* 5733 */ 1378010,
/* 5734 */ 9999,
/* 5735 */ 1378010,
/* 5736 */ 9999,
/* 5737 */ 9999,
/* 5738 */ 1378010,
/* 5739 */ 9999,
/* 5740 */ 1378014,
/* 5741 */ 9999,
/* 5742 */ 1378018,
/* 5743 */ 9999,
/* 5744 */ 1378027,
/* 5745 */ 9999,
/* 5746 */ 1378033,
/* 5747 */ 9999,
/* 5748 */ 1380027,
/* 5749 */ 1380009,
/* 5750 */ 9999,
/* 5751 */ 1380009,
/* 5752 */ 9999,
/* 5753 */ 9999,
/* 5754 */ 1380009,
/* 5755 */ 9999,
/* 5756 */ 9999,
/* 5757 */ 9999,
/* 5758 */ 1391007,
/* 5759 */ 9999,
/* 5760 */ 1390013,
/* 5761 */ 9999,
/* 5762 */ 9999,
/* 5763 */ 1390013,
/* 5764 */ 9999,
/* 5765 */ 1391027,
/* 5766 */ 1391009,
/* 5767 */ 9999,
/* 5768 */ 1391009,
/* 5769 */ 9999,
/* 5770 */ 9999,
/* 5771 */ 1391009,
/* 5772 */ 9999,
/* 5773 */ 9999,
/* 5774 */ 1493007,
/* 5775 */ 9999,
/* 5776 */ 9999,
/* 5777 */ 1493003,
/* 5778 */ 9999,
/* 5779 */ 1493025,
/* 5780 */ 1493009,
/* 5781 */ 9999,
/* 5782 */ 1493009,
/* 5783 */ 9999,
/* 5784 */ 9999,
/* 5785 */ 1493009,
/* 5786 */ 9999,
/* 5787 */ 9999,
/* 5788 */ 1498007,
/* 5789 */ 1503007,
/* 5790 */ 9999,
/* 5791 */ 9999,
/* 5792 */ 1497004,
/* 5793 */ 9999,
/* 5794 */ 1498027,
/* 5795 */ 1498009,
/* 5796 */ 9999,
/* 5797 */ 1498009,
/* 5798 */ 9999,
/* 5799 */ 9999,
/* 5800 */ 1499018,
/* 5801 */ 1499016,
/* 5802 */ 9999,
/* 5803 */ 1499016,
/* 5804 */ 9999,
/* 5805 */ 9999,
/* 5806 */ 1499016,
/* 5807 */ 9999,
/* 5808 */ 1503025,
/* 5809 */ 1503009,
/* 5810 */ 9999,
/* 5811 */ 1503009,
/* 5812 */ 9999,
/* 5813 */ 9999,
/* 5814 */ 1503009,
/* 5815 */ 9999,
/* 5816 */ 9999,
/* 5817 */ 9999,
/* 5818 */ 1539010,
/* 5819 */ 9999,
/* 5820 */ 9999,
/* 5821 */ 9999,
/* 5822 */ 1541014,
/* 5823 */ 9999,
/* 5824 */ 9999,
/* 5825 */ 9999,
/* 5826 */ 1543011,
/* 5827 */ 9999,
/* 5828 */ 9999,
/* 5829 */ 9999,
/* 5830 */ 1545013,
/* 5831 */ 9999,
/* 5832 */ 9999,
/* 5833 */ 9999,
/* 5834 */ 1547014,
/* 5835 */ 9999,
/* 5836 */ 9999,
/* 5837 */ 9999,
/* 5838 */ 1549013,
/* 5839 */ 9999,
/* 5840 */ 1551005,
/* 5841 */ 9999,
/* 5842 */ 1551011,
/* 5843 */ 9999,
/* 5844 */ 9999,
/* 5845 */ 1553009,
/* 5846 */ 9999,
/* 5847 */ 9999,
/* 5848 */ 1553005,
/* 5849 */ 9999,
/* 5850 */ 9999,
/* 5851 */ 9999,
/* 5852 */ 1556009,
/* 5853 */ 9999,
/* 5854 */ 1597005,
/* 5855 */ 1597020,
/* 5856 */ 9999,
/* 5857 */ 9999,
/* 5858 */ 1597013,
/* 5859 */ 9999,
/* 5860 */ 1653005,
/* 5861 */ 1653020,
/* 5862 */ 9999,
/* 5863 */ 9999,
/* 5864 */ 1680009,
/* 5865 */ 9999,
/* 5866 */ 9999,
/* 5867 */ 1653013,
/* 5868 */ 9999,
/* 5869 */ 9999,
/* 5870 */ 9999,
/* 5871 */ 1715009,
/* 5872 */ 9999,
/* 5873 */ 9999,
/* 5874 */ 1708009,
/* 5875 */ 9999,
/* 5876 */ 1734005,
/* 5877 */ 9999,
/* 5878 */ 9999,
/* 5879 */ 9999,
/* 5880 */ 1734013,
/* 5881 */ 9999,
/* 5882 */ 9999,
/* 5883 */ 9999,
/* 5884 */ 1597021,
/* 5885 */ 9999,
/* 5886 */ 9999,
/* 5887 */ 9999,
/* 5888 */ 1597033,
/* 5889 */ 9999,
/* 5890 */ 9999,
/* 5891 */ 9999,
/* 5892 */ 1653021,
/* 5893 */ 9999,
/* 5894 */ 9999,
/* 5895 */ 9999,
/* 5896 */ 1653033,
/* 5897 */ 9999,
/* 5898 */ 9999,
/* 5899 */ 9999,
/* 5900 */ 1744007,
/* 5901 */ 9999,
/* 5902 */ 1789005,
/* 5903 */ 9999,
/* 5904 */ 9999,
/* 5905 */ 9999,
/* 5906 */ 1789010,
/* 5907 */ 9999,
/* 5908 */ 1795005,
/* 5909 */ 1795020,
/* 5910 */ 9999,
/* 5911 */ 9999,
/* 5912 */ 1795013,
/* 5913 */ 9999,
/* 5914 */ 9999,
/* 5915 */ 9999,
/* 5916 */ 1795021,
/* 5917 */ 9999,
/* 5918 */ 9999,
/* 5919 */ 9999,
/* 5920 */ 1795033,
/* 5921 */ 9999,
/* 5922 */ 1837005,
/* 5923 */ 9999,
/* 5924 */ 9999,
/* 5925 */ 1837010,
/* 5926 */ 9999,
/* 5927 */ 1839005,
/* 5928 */ 9999,
/* 5929 */ 9999,
/* 5930 */ 1839010,
/* 5931 */ 9999,
/* 5932 */ 9999,
/* 5933 */ 9999,
/* 5934 */ 1841018,
/* 5935 */ 9999,
/* 5936 */ 1841035,
/* 5937 */ 9999,
/* 5938 */ 9999,
/* 5939 */ 1841012,
/* 5940 */ 9999,
/* 5941 */ 9999,
/* 5942 */ 9999,
/* 5943 */ 1843018,
/* 5944 */ 9999,
/* 5945 */ 1843035,
/* 5946 */ 9999,
/* 5947 */ 9999,
/* 5948 */ 1843012,
/* 5949 */ 9999,
/* 5950 */ 1845005,
/* 5951 */ 9999,
/* 5952 */ 1845013,
/* 5953 */ 9999,
/* 5954 */ 9999,
/* 5955 */ 1850018,
/* 5956 */ 9999,
/* 5957 */ 1850016,
/* 5958 */ 9999,
/* 5959 */ 9999,
/* 5960 */ 1852018,
/* 5961 */ 9999,
/* 5962 */ 1852016,
/* 5963 */ 9999,
/* 5964 */ 9999,
/* 5965 */ 1854009,
/* 5966 */ 9999,
/* 5967 */ 1854005,
/* 5968 */ 9999,
/* 5969 */ 9999,
/* 5970 */ 1856009,
/* 5971 */ 9999,
/* 5972 */ 1856005,
/* 5973 */ 9999,
/* 5974 */ 9999,
/* 5975 */ 1858009,
/* 5976 */ 9999,
/* 5977 */ 1858005,
/* 5978 */ 9999,
/* 5979 */ 9999,
/* 5980 */ 1860009,
/* 5981 */ 9999,
/* 5982 */ 1860005,
/* 5983 */ 9999,
/* 5984 */ 9999,
/* 5985 */ 1862009,
/* 5986 */ 9999,
/* 5987 */ 9999,
/* 5988 */ 1862005,
/* 5989 */ 9999,
/* 5990 */ 1864005,
/* 5991 */ 9999,
/* 5992 */ 1864010,
/* 5993 */ 9999,
/* 5994 */ 1869005,
/* 5995 */ 9999,
/* 5996 */ 1869021,
/* 5997 */ 9999,
/* 5998 */ 1869010,
/* 5999 */ 9999,
/* 6000 */ 1871005,
/* 6001 */ 9999,
/* 6002 */ 1871010,
/* 6003 */ 9999,
/* 6004 */ 9999,
/* 6005 */ 1876009,
/* 6006 */ 9999,
/* 6007 */ 1876027,
/* 6008 */ 9999,
/* 6009 */ 1876005,
/* 6010 */ 9999,
/* 6011 */ 1878005,
/* 6012 */ 9999,
/* 6013 */ 1878010,
/* 6014 */ 9999,
/* 6015 */ 1884005,
/* 6016 */ 9999,
/* 6017 */ 1884021,
/* 6018 */ 9999,
/* 6019 */ 1884010,
/* 6020 */ 9999,
/* 6021 */ 1886005,
/* 6022 */ 9999,
/* 6023 */ 1886021,
/* 6024 */ 9999,
/* 6025 */ 1886010,
/* 6026 */ 9999,
/* 6027 */ 1888005,
/* 6028 */ 9999,
/* 6029 */ 1888021,
/* 6030 */ 9999,
/* 6031 */ 1888010,
/* 6032 */ 9999,
/* 6033 */ 1890005,
/* 6034 */ 9999,
/* 6035 */ 1890010,
/* 6036 */ 9999,
/* 6037 */ 1895005,
/* 6038 */ 9999,
/* 6039 */ 1895021,
/* 6040 */ 9999,
/* 6041 */ 1895010,
/* 6042 */ 9999,
/* 6043 */ 1897005,
/* 6044 */ 9999,
/* 6045 */ 1897021,
/* 6046 */ 9999,
/* 6047 */ 1897010,
/* 6048 */ 9999,
/* 6049 */ 1899005,
/* 6050 */ 9999,
/* 6051 */ 1899010,
/* 6052 */ 9999,
/* 6053 */ 1904005,
/* 6054 */ 9999,
/* 6055 */ 1904027,
/* 6056 */ 9999,
/* 6057 */ 1904010,
/* 6058 */ 9999,
/* 6059 */ 1906005,
/* 6060 */ 9999,
/* 6061 */ 1906027,
/* 6062 */ 9999,
/* 6063 */ 1906010,
/* 6064 */ 9999,
/* 6065 */ 1908005,
/* 6066 */ 9999,
/* 6067 */ 1908010,
/* 6068 */ 9999,
/* 6069 */ 1917005,
/* 6070 */ 9999,
/* 6071 */ 1917021,
/* 6072 */ 9999,
/* 6073 */ 1917010,
/* 6074 */ 9999,
/* 6075 */ 1919005,
/* 6076 */ 9999,
/* 6077 */ 1919021,
/* 6078 */ 9999,
/* 6079 */ 1919010,
/* 6080 */ 9999,
/* 6081 */ 1921005,
/* 6082 */ 9999,
/* 6083 */ 1921023,
/* 6084 */ 9999,
/* 6085 */ 1921010,
/* 6086 */ 9999,
/* 6087 */ 1923005,
/* 6088 */ 9999,
/* 6089 */ 1923023,
/* 6090 */ 9999,
/* 6091 */ 1923010,
/* 6092 */ 9999,
/* 6093 */ 1925005,
/* 6094 */ 9999,
/* 6095 */ 1925023,
/* 6096 */ 9999,
/* 6097 */ 1925010,
/* 6098 */ 9999,
/* 6099 */ 1927005,
/* 6100 */ 9999,
/* 6101 */ 1927023,
/* 6102 */ 9999,
/* 6103 */ 1927010,
/* 6104 */ 9999,
/* 6105 */ 1929005,
/* 6106 */ 9999,
/* 6107 */ 1929010,
/* 6108 */ 9999,
/* 6109 */ 1934005,
/* 6110 */ 9999,
/* 6111 */ 1934021,
/* 6112 */ 9999,
/* 6113 */ 1934010,
/* 6114 */ 9999,
/* 6115 */ 1936005,
/* 6116 */ 9999,
/* 6117 */ 1936010,
/* 6118 */ 9999,
/* 6119 */ 1941005,
/* 6120 */ 9999,
/* 6121 */ 1941022,
/* 6122 */ 9999,
/* 6123 */ 1941011,
/* 6124 */ 9999,
/* 6125 */ 1943005,
/* 6126 */ 9999,
/* 6127 */ 1943010,
/* 6128 */ 9999,
/* 6129 */ 1948005,
/* 6130 */ 9999,
/* 6131 */ 1948022,
/* 6132 */ 9999,
/* 6133 */ 1948011,
/* 6134 */ 9999,
/* 6135 */ 1950005,
/* 6136 */ 9999,
/* 6137 */ 1950011,
/* 6138 */ 9999,
/* 6139 */ 1955005,
/* 6140 */ 9999,
/* 6141 */ 1955025,
/* 6142 */ 9999,
/* 6143 */ 1955011,
/* 6144 */ 9999,
/* 6145 */ 1957005,
/* 6146 */ 9999,
/* 6147 */ 1957011,
/* 6148 */ 9999,
/* 6149 */ 1962005,
/* 6150 */ 9999,
/* 6151 */ 1962024,
/* 6152 */ 9999,
/* 6153 */ 1962011,
/* 6154 */ 9999,
/* 6155 */ 1964005,
/* 6156 */ 9999,
/* 6157 */ 1964011,
/* 6158 */ 9999,
/* 6159 */ 1969003,
/* 6160 */ 9999,
/* 6161 */ 1969020,
/* 6162 */ 9999,
/* 6163 */ 1969034,
/* 6164 */ 9999,
/* 6165 */ 1969009,
/* 6166 */ 9999,
/* 6167 */ 2019005,
/* 6168 */ 9999,
/* 6169 */ 2019011,
/* 6170 */ 9999,
/* 6171 */ 2027005,
/* 6172 */ 9999,
/* 6173 */ 2027020,
/* 6174 */ 9999,
/* 6175 */ 2027010,
/* 6176 */ 9999,
/* 6177 */ 2029005,
/* 6178 */ 9999,
/* 6179 */ 2029027,
/* 6180 */ 9999,
/* 6181 */ 2029010,
/* 6182 */ 9999,
/* 6183 */ 2031005,
/* 6184 */ 9999,
/* 6185 */ 2031027,
/* 6186 */ 9999,
/* 6187 */ 2031010,
/* 6188 */ 9999,
/* 6189 */ 2033005,
/* 6190 */ 9999,
/* 6191 */ 2033028,
/* 6192 */ 9999,
/* 6193 */ 2033010,
/* 6194 */ 9999,
/* 6195 */ 2035005,
/* 6196 */ 9999,
/* 6197 */ 2035027,
/* 6198 */ 9999,
/* 6199 */ 2035010,
/* 6200 */ 9999,
/* 6201 */ 2037005,
/* 6202 */ 9999,
/* 6203 */ 2037027,
/* 6204 */ 9999,
/* 6205 */ 2037010,
/* 6206 */ 9999,
/* 6207 */ 2039005,
/* 6208 */ 9999,
/* 6209 */ 2039027,
/* 6210 */ 9999,
/* 6211 */ 2039010,
/* 6212 */ 9999,
/* 6213 */ 2041005,
/* 6214 */ 9999,
/* 6215 */ 2041026,
/* 6216 */ 9999,
/* 6217 */ 2041010,
/* 6218 */ 9999,
/* 6219 */ 2043005,
/* 6220 */ 9999,
/* 6221 */ 2043030,
/* 6222 */ 9999,
/* 6223 */ 2043010,
/* 6224 */ 9999,
/* 6225 */ 2045005,
/* 6226 */ 9999,
/* 6227 */ 2045030,
/* 6228 */ 9999,
/* 6229 */ 2045010,
/* 6230 */ 9999,
/* 6231 */ 2047005,
/* 6232 */ 9999,
/* 6233 */ 2047027,
/* 6234 */ 9999,
/* 6235 */ 2047010,
/* 6236 */ 9999,
/* 6237 */ 2049005,
/* 6238 */ 9999,
/* 6239 */ 2049011,
/* 6240 */ 9999,
/* 6241 */ 2054003,
/* 6242 */ 9999,
/* 6243 */ 2054017,
/* 6244 */ 9999,
/* 6245 */ 2054006,
/* 6246 */ 9999,
/* 6247 */ 2067005,
/* 6248 */ 9999,
/* 6249 */ 2067011,
/* 6250 */ 9999,
/* 6251 */ 2078003,
/* 6252 */ 9999,
/* 6253 */ 2076003,
/* 6254 */ 9999,
/* 6255 */ 2078005,
/* 6256 */ 2105003,
/* 6257 */ 9999,
/* 6258 */ 2078011,
/* 6259 */ 9999,
/* 6260 */ 9999,
/* 6261 */ 2078011,
/* 6262 */ 9999,
/* 6263 */ 9999,
/* 6264 */ 2105009,
/* 6265 */ 2105005,
/* 6266 */ 9999,
/* 6267 */ 2105005,
/* 6268 */ 9999,
/* 6269 */ 9999,
/* 6270 */ 2105005,
0
};
/* only for BIGHASH (see art.c)
extern int DHITS[];
int DHITS[6272];
*/
int TABLE[1329][492];
void init_dirsets() {
TABLE[448][0] = 1;
TABLE[448][376] = 1;
TABLE[448][428] = 1;
TABLE[448][348] = 1;
TABLE[448][346] = 1;
TABLE[448][446] = 1;
TABLE[448][384] = 1;
TABLE[448][368] = 1;
TABLE[448][344] = 1;
TABLE[448][291] = 1;
TABLE[448][329] = 1;
TABLE[448][261] = 1;
TABLE[448][413] = 1;
TABLE[448][306] = 1;
TABLE[448][354] = 1;
TABLE[448][325] = 1;
TABLE[448][417] = 1;
TABLE[448][445] = 1;
TABLE[448][444] = 1;
TABLE[448][337] = 1;
TABLE[448][442] = 1;
TABLE[448][264] = 1;
TABLE[448][267] = 1;
TABLE[448][263] = 1;
TABLE[448][271] = 1;
TABLE[448][272] = 1;
TABLE[448][273] = 1;
TABLE[448][469] = 1;
TABLE[448][431] = 1;
TABLE[448][331] = 1;
TABLE[448][404] = 1;
TABLE[448][327] = 1;
TABLE[448][45] = 1;
TABLE[448][43] = 1;
TABLE[448][40] = 1;
TABLE[448][256] = 1;
TABLE[448][453] = 1;
TABLE[448][341] = 1;
TABLE[448][259] = 1;
TABLE[1][0] = 1;
TABLE[1][446] = 1;
TABLE[1][346] = 1;
TABLE[1][348] = 1;
TABLE[1][428] = 1;
TABLE[1][376] = 1;
TABLE[1][469] = 1;
TABLE[1][273] = 1;
TABLE[1][272] = 1;
TABLE[1][271] = 1;
TABLE[1][263] = 1;
TABLE[1][267] = 1;
TABLE[1][264] = 1;
TABLE[1][442] = 1;
TABLE[1][337] = 1;
TABLE[1][444] = 1;
TABLE[1][445] = 1;
TABLE[1][417] = 1;
TABLE[1][325] = 1;
TABLE[1][354] = 1;
TABLE[1][306] = 1;
TABLE[1][413] = 1;
TABLE[1][261] = 1;
TABLE[1][329] = 1;
TABLE[1][291] = 1;
TABLE[1][344] = 1;
TABLE[1][368] = 1;
TABLE[1][384] = 1;
TABLE[1][43] = 1;
TABLE[1][45] = 1;
TABLE[1][327] = 1;
TABLE[1][404] = 1;
TABLE[1][331] = 1;
TABLE[1][431] = 1;
TABLE[1][259] = 1;
TABLE[1][341] = 1;
TABLE[1][453] = 1;
TABLE[1][256] = 1;
TABLE[1][40] = 1;
TABLE[449][0] = 1;
TABLE[449][376] = 1;
TABLE[449][428] = 1;
TABLE[449][348] = 1;
TABLE[449][346] = 1;
TABLE[449][446] = 1;
TABLE[449][384] = 1;
TABLE[449][368] = 1;
TABLE[449][344] = 1;
TABLE[449][291] = 1;
TABLE[449][329] = 1;
TABLE[449][261] = 1;
TABLE[449][413] = 1;
TABLE[449][306] = 1;
TABLE[449][354] = 1;
TABLE[449][325] = 1;
TABLE[449][417] = 1;
TABLE[449][445] = 1;
TABLE[449][444] = 1;
TABLE[449][337] = 1;
TABLE[449][442] = 1;
TABLE[449][264] = 1;
TABLE[449][267] = 1;
TABLE[449][263] = 1;
TABLE[449][271] = 1;
TABLE[449][272] = 1;
TABLE[449][273] = 1;
TABLE[449][469] = 1;
TABLE[449][431] = 1;
TABLE[449][331] = 1;
TABLE[449][404] = 1;
TABLE[449][327] = 1;
TABLE[449][45] = 1;
TABLE[449][43] = 1;
TABLE[449][40] = 1;
TABLE[449][256] = 1;
TABLE[449][453] = 1;
TABLE[449][341] = 1;
TABLE[449][259] = 1;
TABLE[945][0] = 1;
TABLE[2][376] = 1;
TABLE[3][428] = 1;
TABLE[4][348] = 1;
TABLE[5][384] = 1;
TABLE[6][368] = 1;
TABLE[7][344] = 1;
TABLE[8][291] = 1;
TABLE[9][329] = 1;
TABLE[10][261] = 1;
TABLE[11][446] = 1;
TABLE[11][346] = 1;
TABLE[11][348] = 1;
TABLE[11][428] = 1;
TABLE[11][376] = 1;
TABLE[11][469] = 1;
TABLE[11][273] = 1;
TABLE[11][272] = 1;
TABLE[11][271] = 1;
TABLE[11][263] = 1;
TABLE[11][267] = 1;
TABLE[11][264] = 1;
TABLE[11][442] = 1;
TABLE[11][337] = 1;
TABLE[11][444] = 1;
TABLE[11][445] = 1;
TABLE[11][417] = 1;
TABLE[11][325] = 1;
TABLE[11][354] = 1;
TABLE[11][306] = 1;
TABLE[11][413] = 1;
TABLE[11][261] = 1;
TABLE[11][329] = 1;
TABLE[11][291] = 1;
TABLE[11][344] = 1;
TABLE[11][368] = 1;
TABLE[11][384] = 1;
TABLE[11][0] = 1;
TABLE[11][43] = 1;
TABLE[11][45] = 1;
TABLE[11][327] = 1;
TABLE[11][404] = 1;
TABLE[11][331] = 1;
TABLE[11][431] = 1;
TABLE[11][259] = 1;
TABLE[11][341] = 1;
TABLE[11][453] = 1;
TABLE[11][256] = 1;
TABLE[11][40] = 1;
TABLE[12][413] = 1;
TABLE[13][306] = 1;
TABLE[14][354] = 1;
TABLE[15][325] = 1;
TABLE[16][417] = 1;
TABLE[17][445] = 1;
TABLE[18][444] = 1;
TABLE[19][337] = 1;
TABLE[20][261] = 1;
TABLE[21][442] = 1;
TABLE[22][442] = 1;
TABLE[23][469] = 1;
TABLE[23][273] = 1;
TABLE[23][272] = 1;
TABLE[23][271] = 1;
TABLE[23][263] = 1;
TABLE[23][267] = 1;
TABLE[23][264] = 1;
TABLE[24][442] = 1;
TABLE[25][442] = 1;
TABLE[26][346] = 1;
TABLE[27][446] = 1;
TABLE[450][58] = 1;
TABLE[451][323] = 1;
TABLE[946][257] = 1;
TABLE[452][59] = 1;
TABLE[947][125] = 1;
TABLE[28][36] = 1;
TABLE[29][36] = 1;
TABLE[30][43] = 1;
TABLE[30][45] = 1;
TABLE[30][259] = 1;
TABLE[30][341] = 1;
TABLE[30][453] = 1;
TABLE[30][256] = 1;
TABLE[30][40] = 1;
TABLE[31][43] = 1;
TABLE[31][45] = 1;
TABLE[31][259] = 1;
TABLE[31][341] = 1;
TABLE[31][453] = 1;
TABLE[31][256] = 1;
TABLE[31][40] = 1;
TABLE[32][45] = 1;
TABLE[32][43] = 1;
TABLE[32][40] = 1;
TABLE[32][256] = 1;
TABLE[32][453] = 1;
TABLE[32][341] = 1;
TABLE[32][259] = 1;
TABLE[33][45] = 1;
TABLE[33][43] = 1;
TABLE[33][40] = 1;
TABLE[33][256] = 1;
TABLE[33][453] = 1;
TABLE[33][341] = 1;
TABLE[33][259] = 1;
TABLE[34][45] = 1;
TABLE[34][43] = 1;
TABLE[34][40] = 1;
TABLE[34][256] = 1;
TABLE[34][453] = 1;
TABLE[34][341] = 1;
TABLE[34][259] = 1;
TABLE[35][45] = 1;
TABLE[35][43] = 1;
TABLE[35][40] = 1;
TABLE[35][256] = 1;
TABLE[35][453] = 1;
TABLE[35][341] = 1;
TABLE[35][259] = 1;
TABLE[36][45] = 1;
TABLE[36][43] = 1;
TABLE[36][40] = 1;
TABLE[36][256] = 1;
TABLE[36][453] = 1;
TABLE[36][341] = 1;
TABLE[36][259] = 1;
TABLE[37][45] = 1;
TABLE[37][43] = 1;
TABLE[37][40] = 1;
TABLE[37][256] = 1;
TABLE[37][453] = 1;
TABLE[37][341] = 1;
TABLE[37][259] = 1;
TABLE[38][43] = 1;
TABLE[38][45] = 1;
TABLE[38][259] = 1;
TABLE[38][341] = 1;
TABLE[38][453] = 1;
TABLE[38][256] = 1;
TABLE[38][40] = 1;
TABLE[39][43] = 1;
TABLE[40][45] = 1;
TABLE[41][40] = 1;
TABLE[41][256] = 1;
TABLE[41][453] = 1;
TABLE[41][341] = 1;
TABLE[41][259] = 1;
TABLE[42][40] = 1;
TABLE[43][256] = 1;
TABLE[44][453] = 1;
TABLE[45][341] = 1;
TABLE[46][259] = 1;
TABLE[47][40] = 1;
TABLE[47][256] = 1;
TABLE[47][453] = 1;
TABLE[47][341] = 1;
TABLE[47][259] = 1;
TABLE[48][40] = 1;
TABLE[48][256] = 1;
TABLE[48][453] = 1;
TABLE[48][341] = 1;
TABLE[48][259] = 1;
TABLE[49][40] = 1;
TABLE[49][256] = 1;
TABLE[49][453] = 1;
TABLE[49][341] = 1;
TABLE[49][259] = 1;
TABLE[50][43] = 1;
TABLE[50][45] = 1;
TABLE[50][259] = 1;
TABLE[50][341] = 1;
TABLE[50][453] = 1;
TABLE[50][256] = 1;
TABLE[50][40] = 1;
TABLE[51][40] = 1;
TABLE[51][256] = 1;
TABLE[51][453] = 1;
TABLE[51][341] = 1;
TABLE[51][259] = 1;
TABLE[52][40] = 1;
TABLE[52][256] = 1;
TABLE[52][453] = 1;
TABLE[52][341] = 1;
TABLE[52][259] = 1;
TABLE[53][36] = 1;
TABLE[53][414] = 1;
TABLE[53][314] = 1;
TABLE[53][297] = 1;
TABLE[53][40] = 1;
TABLE[53][453] = 1;
TABLE[53][341] = 1;
TABLE[53][259] = 1;
TABLE[53][256] = 1;
TABLE[53][363] = 1;
TABLE[453][363] = 1;
TABLE[948][40] = 1;
TABLE[948][256] = 1;
TABLE[948][453] = 1;
TABLE[948][341] = 1;
TABLE[948][259] = 1;
TABLE[948][314] = 1;
TABLE[948][297] = 1;
TABLE[948][36] = 1;
TABLE[948][414] = 1;
TABLE[454][256] = 1;
TABLE[949][40] = 1;
TABLE[949][256] = 1;
TABLE[949][453] = 1;
TABLE[949][341] = 1;
TABLE[949][259] = 1;
TABLE[949][314] = 1;
TABLE[949][297] = 1;
TABLE[949][36] = 1;
TABLE[949][414] = 1;
TABLE[455][259] = 1;
TABLE[455][341] = 1;
TABLE[455][453] = 1;
TABLE[455][256] = 1;
TABLE[455][40] = 1;
TABLE[950][314] = 1;
TABLE[950][297] = 1;
TABLE[950][36] = 1;
TABLE[950][414] = 1;
TABLE[456][314] = 1;
TABLE[457][297] = 1;
TABLE[951][36] = 1;
TABLE[951][414] = 1;
TABLE[458][36] = 1;
TABLE[459][414] = 1;
TABLE[460][43] = 1;
TABLE[461][45] = 1;
TABLE[952][466] = 1;
TABLE[952][395] = 1;
TABLE[952][293] = 1;
TABLE[952][59] = 1;
TABLE[952][44] = 1;
TABLE[952][332] = 1;
TABLE[464][466] = 1;
TABLE[465][395] = 1;
TABLE[466][395] = 1;
TABLE[467][395] = 1;
TABLE[468][395] = 1;
TABLE[469][293] = 1;
TABLE[955][59] = 1;
TABLE[955][44] = 1;
TABLE[955][332] = 1;
TABLE[463][43] = 1;
TABLE[463][45] = 1;
TABLE[463][259] = 1;
TABLE[463][341] = 1;
TABLE[463][453] = 1;
TABLE[463][256] = 1;
TABLE[463][40] = 1;
TABLE[954][93] = 1;
TABLE[462][44] = 1;
TABLE[953][45] = 1;
TABLE[953][43] = 1;
TABLE[953][40] = 1;
TABLE[953][256] = 1;
TABLE[953][453] = 1;
TABLE[953][341] = 1;
TABLE[953][259] = 1;
TABLE[953][93] = 1;
TABLE[54][352] = 1;
TABLE[470][58] = 1;
TABLE[473][44] = 1;
TABLE[473][332] = 1;
TABLE[473][363] = 1;
TABLE[473][256] = 1;
TABLE[473][259] = 1;
TABLE[473][341] = 1;
TABLE[473][453] = 1;
TABLE[473][40] = 1;
TABLE[473][297] = 1;
TABLE[473][314] = 1;
TABLE[473][414] = 1;
TABLE[473][36] = 1;
TABLE[956][363] = 1;
TABLE[956][256] = 1;
TABLE[956][259] = 1;
TABLE[956][341] = 1;
TABLE[956][453] = 1;
TABLE[956][40] = 1;
TABLE[956][297] = 1;
TABLE[956][314] = 1;
TABLE[956][414] = 1;
TABLE[956][36] = 1;
TABLE[956][59] = 1;
TABLE[471][44] = 1;
TABLE[472][332] = 1;
TABLE[55][364] = 1;
TABLE[55][353] = 1;
TABLE[474][364] = 1;
TABLE[475][353] = 1;
TABLE[476][58] = 1;
TABLE[957][317] = 1;
TABLE[957][372] = 1;
TABLE[957][300] = 1;
TABLE[957][274] = 1;
TABLE[957][259] = 1;
TABLE[957][256] = 1;
TABLE[477][274] = 1;
TABLE[477][300] = 1;
TABLE[477][372] = 1;
TABLE[477][317] = 1;
TABLE[958][259] = 1;
TABLE[958][256] = 1;
TABLE[478][259] = 1;
TABLE[479][256] = 1;
TABLE[480][396] = 1;
TABLE[959][59] = 1;
TABLE[56][259] = 1;
TABLE[56][256] = 1;
TABLE[56][317] = 1;
TABLE[56][372] = 1;
TABLE[56][300] = 1;
TABLE[56][274] = 1;
TABLE[481][274] = 1;
TABLE[481][300] = 1;
TABLE[481][372] = 1;
TABLE[481][317] = 1;
TABLE[960][259] = 1;
TABLE[960][256] = 1;
TABLE[482][259] = 1;
TABLE[483][256] = 1;
TABLE[484][396] = 1;
TABLE[961][91] = 1;
TABLE[961][466] = 1;
TABLE[961][292] = 1;
TABLE[961][44] = 1;
TABLE[961][259] = 1;
TABLE[961][59] = 1;
TABLE[57][396] = 1;
TABLE[485][44] = 1;
TABLE[962][59] = 1;
TABLE[58][43] = 1;
TABLE[58][45] = 1;
TABLE[58][259] = 1;
TABLE[58][341] = 1;
TABLE[58][453] = 1;
TABLE[58][256] = 1;
TABLE[58][40] = 1;
TABLE[486][44] = 1;
TABLE[963][257] = 1;
TABLE[963][123] = 1;
TABLE[488][257] = 1;
TABLE[489][123] = 1;
TABLE[487][47] = 1;
TABLE[964][44] = 1;
TABLE[964][59] = 1;
TABLE[59][470] = 1;
TABLE[59][471] = 1;
TABLE[59][43] = 1;
TABLE[59][45] = 1;
TABLE[59][33] = 1;
TABLE[59][126] = 1;
TABLE[59][58] = 1;
TABLE[59][380] = 1;
TABLE[59][383] = 1;
TABLE[59][258] = 1;
TABLE[59][40] = 1;
TABLE[59][36] = 1;
TABLE[59][391] = 1;
TABLE[59][341] = 1;
TABLE[59][453] = 1;
TABLE[59][257] = 1;
TABLE[59][259] = 1;
TABLE[59][256] = 1;
TABLE[60][59] = 1;
TABLE[60][44] = 1;
TABLE[60][259] = 1;
TABLE[60][341] = 1;
TABLE[60][453] = 1;
TABLE[60][256] = 1;
TABLE[60][40] = 1;
TABLE[60][45] = 1;
TABLE[60][61] = 1;
TABLE[60][43] = 1;
TABLE[491][40] = 1;
TABLE[491][256] = 1;
TABLE[491][453] = 1;
TABLE[491][341] = 1;
TABLE[491][259] = 1;
TABLE[491][43] = 1;
TABLE[492][45] = 1;
TABLE[493][61] = 1;
TABLE[494][43] = 1;
TABLE[494][45] = 1;
TABLE[494][259] = 1;
TABLE[494][341] = 1;
TABLE[494][453] = 1;
TABLE[494][256] = 1;
TABLE[494][40] = 1;
TABLE[490][43] = 1;
TABLE[965][40] = 1;
TABLE[965][256] = 1;
TABLE[965][453] = 1;
TABLE[965][341] = 1;
TABLE[965][259] = 1;
TABLE[498][40] = 1;
TABLE[966][59] = 1;
TABLE[966][44] = 1;
TABLE[495][383] = 1;
TABLE[496][380] = 1;
TABLE[497][387] = 1;
TABLE[61][384] = 1;
TABLE[500][44] = 1;
TABLE[968][123] = 1;
TABLE[499][44] = 1;
TABLE[967][123] = 1;
TABLE[501][125] = 1;
TABLE[501][323] = 1;
TABLE[501][322] = 1;
TABLE[501][360] = 1;
TABLE[501][361] = 1;
TABLE[501][362] = 1;
TABLE[501][435] = 1;
TABLE[501][419] = 1;
TABLE[501][365] = 1;
TABLE[501][294] = 1;
TABLE[501][292] = 1;
TABLE[501][320] = 1;
TABLE[501][290] = 1;
TABLE[501][377] = 1;
TABLE[501][342] = 1;
TABLE[501][346] = 1;
TABLE[501][426] = 1;
TABLE[501][430] = 1;
TABLE[501][385] = 1;
TABLE[501][432] = 1;
TABLE[501][327] = 1;
TABLE[501][404] = 1;
TABLE[501][352] = 1;
TABLE[501][396] = 1;
TABLE[501][304] = 1;
TABLE[501][370] = 1;
TABLE[501][43] = 1;
TABLE[501][364] = 1;
TABLE[501][353] = 1;
TABLE[501][276] = 1;
TABLE[501][429] = 1;
TABLE[501][373] = 1;
TABLE[501][308] = 1;
TABLE[501][264] = 1;
TABLE[501][267] = 1;
TABLE[501][263] = 1;
TABLE[501][271] = 1;
TABLE[501][272] = 1;
TABLE[501][273] = 1;
TABLE[501][469] = 1;
TABLE[969][125] = 1;
TABLE[62][323] = 1;
TABLE[63][322] = 1;
TABLE[64][370] = 1;
TABLE[64][304] = 1;
TABLE[65][370] = 1;
TABLE[65][304] = 1;
TABLE[66][360] = 1;
TABLE[67][361] = 1;
TABLE[68][362] = 1;
TABLE[68][43] = 1;
TABLE[69][353] = 1;
TABLE[69][364] = 1;
TABLE[70][435] = 1;
TABLE[71][435] = 1;
TABLE[72][435] = 1;
TABLE[73][435] = 1;
TABLE[74][419] = 1;
TABLE[75][365] = 1;
TABLE[76][294] = 1;
TABLE[77][292] = 1;
TABLE[78][292] = 1;
TABLE[79][320] = 1;
TABLE[80][290] = 1;
TABLE[81][377] = 1;
TABLE[82][342] = 1;
TABLE[83][346] = 1;
TABLE[84][276] = 1;
TABLE[85][426] = 1;
TABLE[86][430] = 1;
TABLE[87][385] = 1;
TABLE[88][432] = 1;
TABLE[89][327] = 1;
TABLE[90][404] = 1;
TABLE[91][352] = 1;
TABLE[92][396] = 1;
TABLE[93][308] = 1;
TABLE[93][373] = 1;
TABLE[93][429] = 1;
TABLE[94][469] = 1;
TABLE[94][273] = 1;
TABLE[94][272] = 1;
TABLE[94][271] = 1;
TABLE[94][263] = 1;
TABLE[94][267] = 1;
TABLE[94][264] = 1;
TABLE[502][58] = 1;
TABLE[970][257] = 1;
TABLE[503][58] = 1;
TABLE[971][43] = 1;
TABLE[971][45] = 1;
TABLE[971][259] = 1;
TABLE[971][341] = 1;
TABLE[971][453] = 1;
TABLE[971][256] = 1;
TABLE[971][40] = 1;
TABLE[504][370] = 1;
TABLE[505][304] = 1;
TABLE[506][58] = 1;
TABLE[972][43] = 1;
TABLE[972][45] = 1;
TABLE[972][259] = 1;
TABLE[972][341] = 1;
TABLE[972][453] = 1;
TABLE[972][256] = 1;
TABLE[972][40] = 1;
TABLE[507][370] = 1;
TABLE[508][304] = 1;
TABLE[509][58] = 1;
TABLE[973][43] = 1;
TABLE[973][45] = 1;
TABLE[973][259] = 1;
TABLE[973][341] = 1;
TABLE[973][453] = 1;
TABLE[973][256] = 1;
TABLE[973][40] = 1;
TABLE[510][58] = 1;
TABLE[974][43] = 1;
TABLE[974][45] = 1;
TABLE[974][259] = 1;
TABLE[974][341] = 1;
TABLE[974][453] = 1;
TABLE[974][256] = 1;
TABLE[974][40] = 1;
TABLE[511][58] = 1;
TABLE[975][43] = 1;
TABLE[975][45] = 1;
TABLE[975][259] = 1;
TABLE[975][341] = 1;
TABLE[975][453] = 1;
TABLE[975][256] = 1;
TABLE[975][40] = 1;
TABLE[512][43] = 1;
TABLE[976][362] = 1;
TABLE[513][58] = 1;
TABLE[977][43] = 1;
TABLE[977][45] = 1;
TABLE[977][259] = 1;
TABLE[977][341] = 1;
TABLE[977][453] = 1;
TABLE[977][256] = 1;
TABLE[977][40] = 1;
TABLE[514][58] = 1;
TABLE[978][45] = 1;
TABLE[978][43] = 1;
TABLE[978][40] = 1;
TABLE[978][256] = 1;
TABLE[978][453] = 1;
TABLE[978][341] = 1;
TABLE[978][259] = 1;
TABLE[978][59] = 1;
TABLE[516][43] = 1;
TABLE[516][45] = 1;
TABLE[516][259] = 1;
TABLE[516][341] = 1;
TABLE[516][453] = 1;
TABLE[516][256] = 1;
TABLE[516][40] = 1;
TABLE[980][59] = 1;
TABLE[515][44] = 1;
TABLE[979][45] = 1;
TABLE[979][43] = 1;
TABLE[979][40] = 1;
TABLE[979][256] = 1;
TABLE[979][453] = 1;
TABLE[979][341] = 1;
TABLE[979][259] = 1;
TABLE[979][59] = 1;
TABLE[517][58] = 1;
TABLE[981][45] = 1;
TABLE[981][43] = 1;
TABLE[981][40] = 1;
TABLE[981][256] = 1;
TABLE[981][453] = 1;
TABLE[981][341] = 1;
TABLE[981][259] = 1;
TABLE[981][59] = 1;
TABLE[519][43] = 1;
TABLE[519][45] = 1;
TABLE[519][259] = 1;
TABLE[519][341] = 1;
TABLE[519][453] = 1;
TABLE[519][256] = 1;
TABLE[519][40] = 1;
TABLE[983][59] = 1;
TABLE[518][44] = 1;
TABLE[982][45] = 1;
TABLE[982][43] = 1;
TABLE[982][40] = 1;
TABLE[982][256] = 1;
TABLE[982][453] = 1;
TABLE[982][341] = 1;
TABLE[982][259] = 1;
TABLE[982][59] = 1;
TABLE[520][58] = 1;
TABLE[984][45] = 1;
TABLE[984][43] = 1;
TABLE[984][40] = 1;
TABLE[984][256] = 1;
TABLE[984][453] = 1;
TABLE[984][341] = 1;
TABLE[984][259] = 1;
TABLE[521][44] = 1;
TABLE[985][59] = 1;
TABLE[522][58] = 1;
TABLE[986][43] = 1;
TABLE[986][45] = 1;
TABLE[986][259] = 1;
TABLE[986][341] = 1;
TABLE[986][453] = 1;
TABLE[986][256] = 1;
TABLE[986][40] = 1;
TABLE[523][58] = 1;
TABLE[987][43] = 1;
TABLE[987][45] = 1;
TABLE[987][259] = 1;
TABLE[987][341] = 1;
TABLE[987][453] = 1;
TABLE[987][256] = 1;
TABLE[987][40] = 1;
TABLE[524][58] = 1;
TABLE[988][43] = 1;
TABLE[988][45] = 1;
TABLE[988][259] = 1;
TABLE[988][341] = 1;
TABLE[988][453] = 1;
TABLE[988][256] = 1;
TABLE[988][40] = 1;
TABLE[525][58] = 1;
TABLE[989][43] = 1;
TABLE[989][45] = 1;
TABLE[989][259] = 1;
TABLE[989][341] = 1;
TABLE[989][453] = 1;
TABLE[989][256] = 1;
TABLE[989][40] = 1;
TABLE[526][43] = 1;
TABLE[526][45] = 1;
TABLE[526][259] = 1;
TABLE[526][341] = 1;
TABLE[526][453] = 1;
TABLE[526][256] = 1;
TABLE[526][40] = 1;
TABLE[990][43] = 1;
TABLE[990][45] = 1;
TABLE[990][259] = 1;
TABLE[990][341] = 1;
TABLE[990][453] = 1;
TABLE[990][256] = 1;
TABLE[990][40] = 1;
TABLE[527][58] = 1;
TABLE[991][126] = 1;
TABLE[991][43] = 1;
TABLE[991][45] = 1;
TABLE[991][259] = 1;
TABLE[991][341] = 1;
TABLE[991][453] = 1;
TABLE[991][256] = 1;
TABLE[991][40] = 1;
TABLE[991][59] = 1;
TABLE[528][45] = 1;
TABLE[528][43] = 1;
TABLE[528][126] = 1;
TABLE[528][40] = 1;
TABLE[528][256] = 1;
TABLE[528][453] = 1;
TABLE[528][341] = 1;
TABLE[528][259] = 1;
TABLE[992][59] = 1;
TABLE[529][44] = 1;
TABLE[993][59] = 1;
TABLE[530][58] = 1;
TABLE[994][43] = 1;
TABLE[994][45] = 1;
TABLE[994][259] = 1;
TABLE[994][341] = 1;
TABLE[994][453] = 1;
TABLE[994][256] = 1;
TABLE[994][40] = 1;
TABLE[531][58] = 1;
TABLE[995][43] = 1;
TABLE[995][45] = 1;
TABLE[995][259] = 1;
TABLE[995][341] = 1;
TABLE[995][453] = 1;
TABLE[995][256] = 1;
TABLE[995][40] = 1;
TABLE[532][37] = 1;
TABLE[996][59] = 1;
TABLE[533][58] = 1;
TABLE[997][43] = 1;
TABLE[997][45] = 1;
TABLE[997][259] = 1;
TABLE[997][341] = 1;
TABLE[997][453] = 1;
TABLE[997][256] = 1;
TABLE[997][40] = 1;
TABLE[534][37] = 1;
TABLE[998][59] = 1;
TABLE[535][58] = 1;
TABLE[999][36] = 1;
TABLE[999][59] = 1;
TABLE[537][36] = 1;
TABLE[1001][59] = 1;
TABLE[536][44] = 1;
TABLE[1000][36] = 1;
TABLE[1000][59] = 1;
TABLE[538][58] = 1;
TABLE[1002][36] = 1;
TABLE[1002][59] = 1;
TABLE[540][36] = 1;
TABLE[1004][59] = 1;
TABLE[539][44] = 1;
TABLE[1003][36] = 1;
TABLE[1003][59] = 1;
TABLE[541][58] = 1;
TABLE[1005][36] = 1;
TABLE[1005][59] = 1;
TABLE[543][36] = 1;
TABLE[1007][59] = 1;
TABLE[542][44] = 1;
TABLE[1006][36] = 1;
TABLE[1006][59] = 1;
TABLE[95][274] = 1;
TABLE[96][300] = 1;
TABLE[97][372] = 1;
TABLE[98][317] = 1;
TABLE[99][43] = 1;
TABLE[99][45] = 1;
TABLE[99][259] = 1;
TABLE[99][341] = 1;
TABLE[99][453] = 1;
TABLE[99][256] = 1;
TABLE[99][40] = 1;
TABLE[100][43] = 1;
TABLE[100][45] = 1;
TABLE[100][259] = 1;
TABLE[100][341] = 1;
TABLE[100][453] = 1;
TABLE[100][256] = 1;
TABLE[100][40] = 1;
TABLE[544][259] = 1;
TABLE[544][341] = 1;
TABLE[544][453] = 1;
TABLE[544][256] = 1;
TABLE[544][40] = 1;
TABLE[545][40] = 1;
TABLE[545][256] = 1;
TABLE[545][453] = 1;
TABLE[545][341] = 1;
TABLE[545][259] = 1;
TABLE[1008][259] = 1;
TABLE[1008][341] = 1;
TABLE[1008][453] = 1;
TABLE[1008][256] = 1;
TABLE[1008][316] = 1;
TABLE[1008][40] = 1;
TABLE[1008][287] = 1;
TABLE[1008][44] = 1;
TABLE[1008][59] = 1;
TABLE[546][40] = 1;
TABLE[546][256] = 1;
TABLE[546][453] = 1;
TABLE[546][341] = 1;
TABLE[546][259] = 1;
TABLE[1009][316] = 1;
TABLE[1009][40] = 1;
TABLE[1009][287] = 1;
TABLE[1009][44] = 1;
TABLE[1009][59] = 1;
TABLE[547][316] = 1;
TABLE[1010][40] = 1;
TABLE[1010][287] = 1;
TABLE[1010][44] = 1;
TABLE[1010][59] = 1;
TABLE[551][40] = 1;
TABLE[1011][287] = 1;
TABLE[1011][44] = 1;
TABLE[1011][59] = 1;
TABLE[548][319] = 1;
TABLE[549][304] = 1;
TABLE[550][370] = 1;
TABLE[559][287] = 1;
TABLE[1016][44] = 1;
TABLE[1016][59] = 1;
TABLE[552][259] = 1;
TABLE[552][341] = 1;
TABLE[552][453] = 1;
TABLE[552][256] = 1;
TABLE[552][40] = 1;
TABLE[1012][45] = 1;
TABLE[1012][43] = 1;
TABLE[1012][256] = 1;
TABLE[1012][453] = 1;
TABLE[1012][341] = 1;
TABLE[1012][259] = 1;
TABLE[1012][316] = 1;
TABLE[1012][40] = 1;
TABLE[1012][287] = 1;
TABLE[1012][44] = 1;
TABLE[1012][59] = 1;
TABLE[553][43] = 1;
TABLE[553][45] = 1;
TABLE[553][259] = 1;
TABLE[553][341] = 1;
TABLE[553][453] = 1;
TABLE[553][256] = 1;
TABLE[553][40] = 1;
TABLE[1013][316] = 1;
TABLE[1013][40] = 1;
TABLE[1013][287] = 1;
TABLE[1013][44] = 1;
TABLE[1013][59] = 1;
TABLE[554][316] = 1;
TABLE[1014][40] = 1;
TABLE[1014][287] = 1;
TABLE[1014][44] = 1;
TABLE[1014][59] = 1;
TABLE[558][40] = 1;
TABLE[1015][287] = 1;
TABLE[1015][44] = 1;
TABLE[1015][59] = 1;
TABLE[555][319] = 1;
TABLE[556][304] = 1;
TABLE[557][370] = 1;
TABLE[560][43] = 1;
TABLE[560][45] = 1;
TABLE[560][259] = 1;
TABLE[560][341] = 1;
TABLE[560][453] = 1;
TABLE[560][256] = 1;
TABLE[560][40] = 1;
TABLE[1017][44] = 1;
TABLE[1017][59] = 1;
TABLE[101][43] = 1;
TABLE[101][45] = 1;
TABLE[101][259] = 1;
TABLE[101][341] = 1;
TABLE[101][453] = 1;
TABLE[101][256] = 1;
TABLE[101][40] = 1;
TABLE[102][126] = 1;
TABLE[561][44] = 1;
TABLE[1018][126] = 1;
TABLE[1018][43] = 1;
TABLE[1018][45] = 1;
TABLE[1018][259] = 1;
TABLE[1018][341] = 1;
TABLE[1018][453] = 1;
TABLE[1018][256] = 1;
TABLE[1018][40] = 1;
TABLE[1018][59] = 1;
TABLE[562][44] = 1;
TABLE[1019][126] = 1;
TABLE[1019][43] = 1;
TABLE[1019][45] = 1;
TABLE[1019][259] = 1;
TABLE[1019][341] = 1;
TABLE[1019][453] = 1;
TABLE[1019][256] = 1;
TABLE[1019][40] = 1;
TABLE[1019][59] = 1;
TABLE[103][276] = 1;
TABLE[563][58] = 1;
TABLE[1020][43] = 1;
TABLE[1020][45] = 1;
TABLE[1020][259] = 1;
TABLE[1020][341] = 1;
TABLE[1020][453] = 1;
TABLE[1020][256] = 1;
TABLE[1020][40] = 1;
TABLE[104][368] = 1;
TABLE[564][323] = 1;
TABLE[564][370] = 1;
TABLE[564][425] = 1;
TABLE[564][369] = 1;
TABLE[564][313] = 1;
TABLE[564][447] = 1;
TABLE[564][283] = 1;
TABLE[564][430] = 1;
TABLE[564][400] = 1;
TABLE[564][415] = 1;
TABLE[564][379] = 1;
TABLE[564][312] = 1;
TABLE[564][461] = 1;
TABLE[564][426] = 1;
TABLE[564][392] = 1;
TABLE[564][320] = 1;
TABLE[564][309] = 1;
TABLE[564][402] = 1;
TABLE[564][290] = 1;
TABLE[564][361] = 1;
TABLE[564][432] = 1;
TABLE[564][345] = 1;
TABLE[564][346] = 1;
TABLE[564][357] = 1;
TABLE[564][301] = 1;
TABLE[564][463] = 1;
TABLE[564][462] = 1;
TABLE[564][375] = 1;
TABLE[564][451] = 1;
TABLE[564][310] = 1;
TABLE[564][372] = 1;
TABLE[564][371] = 1;
TABLE[564][351] = 1;
TABLE[564][396] = 1;
TABLE[564][364] = 1;
TABLE[564][353] = 1;
TABLE[564][429] = 1;
TABLE[564][373] = 1;
TABLE[564][308] = 1;
TABLE[564][264] = 1;
TABLE[564][267] = 1;
TABLE[564][263] = 1;
TABLE[564][271] = 1;
TABLE[564][272] = 1;
TABLE[564][273] = 1;
TABLE[564][469] = 1;
TABLE[1021][125] = 1;
TABLE[105][323] = 1;
TABLE[106][370] = 1;
TABLE[107][353] = 1;
TABLE[107][364] = 1;
TABLE[108][425] = 1;
TABLE[109][369] = 1;
TABLE[110][313] = 1;
TABLE[111][447] = 1;
TABLE[112][283] = 1;
TABLE[113][430] = 1;
TABLE[114][430] = 1;
TABLE[115][430] = 1;
TABLE[116][400] = 1;
TABLE[117][415] = 1;
TABLE[118][379] = 1;
TABLE[119][312] = 1;
TABLE[120][461] = 1;
TABLE[121][426] = 1;
TABLE[122][392] = 1;
TABLE[123][320] = 1;
TABLE[124][309] = 1;
TABLE[125][402] = 1;
TABLE[126][290] = 1;
TABLE[127][361] = 1;
TABLE[128][432] = 1;
TABLE[129][345] = 1;
TABLE[130][346] = 1;
TABLE[131][357] = 1;
TABLE[132][301] = 1;
TABLE[133][463] = 1;
TABLE[134][462] = 1;
TABLE[135][375] = 1;
TABLE[136][451] = 1;
TABLE[137][310] = 1;
TABLE[138][372] = 1;
TABLE[139][371] = 1;
TABLE[140][351] = 1;
TABLE[141][308] = 1;
TABLE[141][373] = 1;
TABLE[141][429] = 1;
TABLE[142][396] = 1;
TABLE[143][469] = 1;
TABLE[143][273] = 1;
TABLE[143][272] = 1;
TABLE[143][271] = 1;
TABLE[143][263] = 1;
TABLE[143][267] = 1;
TABLE[143][264] = 1;
TABLE[565][58] = 1;
TABLE[1022][257] = 1;
TABLE[566][58] = 1;
TABLE[1023][43] = 1;
TABLE[1023][45] = 1;
TABLE[1023][259] = 1;
TABLE[1023][341] = 1;
TABLE[1023][453] = 1;
TABLE[1023][256] = 1;
TABLE[1023][40] = 1;
TABLE[567][58] = 1;
TABLE[1024][259] = 1;
TABLE[1024][341] = 1;
TABLE[1024][453] = 1;
TABLE[1024][256] = 1;
TABLE[1024][40] = 1;
TABLE[1024][45] = 1;
TABLE[1024][43] = 1;
TABLE[568][58] = 1;
TABLE[1025][259] = 1;
TABLE[1025][341] = 1;
TABLE[1025][453] = 1;
TABLE[1025][256] = 1;
TABLE[1025][40] = 1;
TABLE[1025][45] = 1;
TABLE[1025][43] = 1;
TABLE[569][58] = 1;
TABLE[1026][43] = 1;
TABLE[1026][45] = 1;
TABLE[1026][259] = 1;
TABLE[1026][341] = 1;
TABLE[1026][453] = 1;
TABLE[1026][256] = 1;
TABLE[1026][40] = 1;
TABLE[570][58] = 1;
TABLE[1027][43] = 1;
TABLE[1027][45] = 1;
TABLE[1027][259] = 1;
TABLE[1027][341] = 1;
TABLE[1027][453] = 1;
TABLE[1027][256] = 1;
TABLE[1027][40] = 1;
TABLE[571][58] = 1;
TABLE[1028][43] = 1;
TABLE[1028][45] = 1;
TABLE[1028][259] = 1;
TABLE[1028][341] = 1;
TABLE[1028][453] = 1;
TABLE[1028][256] = 1;
TABLE[1028][40] = 1;
TABLE[572][58] = 1;
TABLE[1029][43] = 1;
TABLE[1029][45] = 1;
TABLE[1029][259] = 1;
TABLE[1029][341] = 1;
TABLE[1029][453] = 1;
TABLE[1029][256] = 1;
TABLE[1029][40] = 1;
TABLE[573][58] = 1;
TABLE[1030][40] = 1;
TABLE[1030][256] = 1;
TABLE[1030][453] = 1;
TABLE[1030][341] = 1;
TABLE[1030][259] = 1;
TABLE[574][58] = 1;
TABLE[1031][43] = 1;
TABLE[575][58] = 1;
TABLE[1032][43] = 1;
TABLE[1032][45] = 1;
TABLE[1032][259] = 1;
TABLE[1032][341] = 1;
TABLE[1032][453] = 1;
TABLE[1032][256] = 1;
TABLE[1032][40] = 1;
TABLE[576][58] = 1;
TABLE[1033][43] = 1;
TABLE[1033][45] = 1;
TABLE[1033][259] = 1;
TABLE[1033][341] = 1;
TABLE[1033][453] = 1;
TABLE[1033][256] = 1;
TABLE[1033][40] = 1;
TABLE[577][58] = 1;
TABLE[1034][43] = 1;
TABLE[1034][45] = 1;
TABLE[1034][259] = 1;
TABLE[1034][341] = 1;
TABLE[1034][453] = 1;
TABLE[1034][256] = 1;
TABLE[1034][40] = 1;
TABLE[578][58] = 1;
TABLE[1035][43] = 1;
TABLE[1035][45] = 1;
TABLE[1035][259] = 1;
TABLE[1035][341] = 1;
TABLE[1035][453] = 1;
TABLE[1035][256] = 1;
TABLE[1035][40] = 1;
TABLE[579][43] = 1;
TABLE[579][45] = 1;
TABLE[579][259] = 1;
TABLE[579][341] = 1;
TABLE[579][453] = 1;
TABLE[579][256] = 1;
TABLE[579][40] = 1;
TABLE[1036][59] = 1;
TABLE[580][58] = 1;
TABLE[1037][43] = 1;
TABLE[1037][45] = 1;
TABLE[1037][259] = 1;
TABLE[1037][341] = 1;
TABLE[1037][453] = 1;
TABLE[1037][256] = 1;
TABLE[1037][40] = 1;
TABLE[581][58] = 1;
TABLE[1038][43] = 1;
TABLE[1038][45] = 1;
TABLE[1038][259] = 1;
TABLE[1038][341] = 1;
TABLE[1038][453] = 1;
TABLE[1038][256] = 1;
TABLE[1038][40] = 1;
TABLE[582][58] = 1;
TABLE[1039][43] = 1;
TABLE[1039][45] = 1;
TABLE[1039][259] = 1;
TABLE[1039][341] = 1;
TABLE[1039][453] = 1;
TABLE[1039][256] = 1;
TABLE[1039][40] = 1;
TABLE[583][58] = 1;
TABLE[1040][43] = 1;
TABLE[1040][45] = 1;
TABLE[1040][259] = 1;
TABLE[1040][341] = 1;
TABLE[1040][453] = 1;
TABLE[1040][256] = 1;
TABLE[1040][40] = 1;
TABLE[584][58] = 1;
TABLE[1041][43] = 1;
TABLE[1041][45] = 1;
TABLE[1041][259] = 1;
TABLE[1041][341] = 1;
TABLE[1041][453] = 1;
TABLE[1041][256] = 1;
TABLE[1041][40] = 1;
TABLE[585][58] = 1;
TABLE[1042][43] = 1;
TABLE[1042][45] = 1;
TABLE[1042][259] = 1;
TABLE[1042][341] = 1;
TABLE[1042][453] = 1;
TABLE[1042][256] = 1;
TABLE[1042][40] = 1;
TABLE[586][58] = 1;
TABLE[1043][275] = 1;
TABLE[1043][43] = 1;
TABLE[1043][45] = 1;
TABLE[1043][259] = 1;
TABLE[1043][341] = 1;
TABLE[1043][453] = 1;
TABLE[1043][256] = 1;
TABLE[1043][40] = 1;
TABLE[1043][59] = 1;
TABLE[587][45] = 1;
TABLE[587][43] = 1;
TABLE[587][275] = 1;
TABLE[587][40] = 1;
TABLE[587][256] = 1;
TABLE[587][453] = 1;
TABLE[587][341] = 1;
TABLE[587][259] = 1;
TABLE[1044][59] = 1;
TABLE[588][58] = 1;
TABLE[1045][43] = 1;
TABLE[1045][45] = 1;
TABLE[1045][259] = 1;
TABLE[1045][341] = 1;
TABLE[1045][453] = 1;
TABLE[1045][256] = 1;
TABLE[1045][40] = 1;
TABLE[589][58] = 1;
TABLE[1046][36] = 1;
TABLE[1046][59] = 1;
TABLE[591][36] = 1;
TABLE[1048][59] = 1;
TABLE[590][44] = 1;
TABLE[1047][36] = 1;
TABLE[1047][59] = 1;
TABLE[592][58] = 1;
TABLE[1049][36] = 1;
TABLE[1049][59] = 1;
TABLE[594][36] = 1;
TABLE[1051][59] = 1;
TABLE[593][44] = 1;
TABLE[1050][36] = 1;
TABLE[1050][59] = 1;
TABLE[595][58] = 1;
TABLE[1052][126] = 1;
TABLE[1052][43] = 1;
TABLE[1052][45] = 1;
TABLE[1052][259] = 1;
TABLE[1052][341] = 1;
TABLE[1052][453] = 1;
TABLE[1052][256] = 1;
TABLE[1052][40] = 1;
TABLE[1052][59] = 1;
TABLE[596][45] = 1;
TABLE[596][43] = 1;
TABLE[596][126] = 1;
TABLE[596][40] = 1;
TABLE[596][256] = 1;
TABLE[596][453] = 1;
TABLE[596][341] = 1;
TABLE[596][259] = 1;
TABLE[1053][59] = 1;
TABLE[597][58] = 1;
TABLE[1054][45] = 1;
TABLE[1054][43] = 1;
TABLE[1054][40] = 1;
TABLE[1054][256] = 1;
TABLE[1054][453] = 1;
TABLE[1054][341] = 1;
TABLE[1054][259] = 1;
TABLE[1054][59] = 1;
TABLE[600][43] = 1;
TABLE[600][45] = 1;
TABLE[600][259] = 1;
TABLE[600][341] = 1;
TABLE[600][453] = 1;
TABLE[600][256] = 1;
TABLE[600][40] = 1;
TABLE[1056][59] = 1;
TABLE[598][44] = 1;
TABLE[599][124] = 1;
TABLE[1055][45] = 1;
TABLE[1055][43] = 1;
TABLE[1055][40] = 1;
TABLE[1055][256] = 1;
TABLE[1055][453] = 1;
TABLE[1055][341] = 1;
TABLE[1055][259] = 1;
TABLE[1055][59] = 1;
TABLE[601][58] = 1;
TABLE[1057][43] = 1;
TABLE[1057][45] = 1;
TABLE[1057][259] = 1;
TABLE[1057][341] = 1;
TABLE[1057][453] = 1;
TABLE[1057][256] = 1;
TABLE[1057][40] = 1;
TABLE[602][58] = 1;
TABLE[1058][43] = 1;
TABLE[1058][45] = 1;
TABLE[1058][259] = 1;
TABLE[1058][341] = 1;
TABLE[1058][453] = 1;
TABLE[1058][256] = 1;
TABLE[1058][40] = 1;
TABLE[603][58] = 1;
TABLE[1059][43] = 1;
TABLE[1059][45] = 1;
TABLE[1059][259] = 1;
TABLE[1059][341] = 1;
TABLE[1059][453] = 1;
TABLE[1059][256] = 1;
TABLE[1059][40] = 1;
TABLE[604][58] = 1;
TABLE[1060][43] = 1;
TABLE[1060][45] = 1;
TABLE[1060][259] = 1;
TABLE[1060][341] = 1;
TABLE[1060][453] = 1;
TABLE[1060][256] = 1;
TABLE[1060][40] = 1;
TABLE[605][58] = 1;
TABLE[1061][43] = 1;
TABLE[1061][45] = 1;
TABLE[1061][259] = 1;
TABLE[1061][341] = 1;
TABLE[1061][453] = 1;
TABLE[1061][256] = 1;
TABLE[1061][40] = 1;
TABLE[606][58] = 1;
TABLE[1062][43] = 1;
TABLE[1062][45] = 1;
TABLE[1062][259] = 1;
TABLE[1062][341] = 1;
TABLE[1062][453] = 1;
TABLE[1062][256] = 1;
TABLE[1062][40] = 1;
TABLE[607][58] = 1;
TABLE[1063][43] = 1;
TABLE[1063][45] = 1;
TABLE[1063][259] = 1;
TABLE[1063][341] = 1;
TABLE[1063][453] = 1;
TABLE[1063][256] = 1;
TABLE[1063][40] = 1;
TABLE[608][58] = 1;
TABLE[1064][43] = 1;
TABLE[1064][45] = 1;
TABLE[1064][259] = 1;
TABLE[1064][341] = 1;
TABLE[1064][453] = 1;
TABLE[1064][256] = 1;
TABLE[1064][40] = 1;
TABLE[609][58] = 1;
TABLE[1065][36] = 1;
TABLE[144][43] = 1;
TABLE[144][45] = 1;
TABLE[144][259] = 1;
TABLE[144][341] = 1;
TABLE[144][453] = 1;
TABLE[144][256] = 1;
TABLE[144][40] = 1;
TABLE[145][275] = 1;
TABLE[610][44] = 1;
TABLE[1066][275] = 1;
TABLE[1066][43] = 1;
TABLE[1066][45] = 1;
TABLE[1066][259] = 1;
TABLE[1066][341] = 1;
TABLE[1066][453] = 1;
TABLE[1066][256] = 1;
TABLE[1066][40] = 1;
TABLE[1066][59] = 1;
TABLE[611][44] = 1;
TABLE[1067][275] = 1;
TABLE[1067][43] = 1;
TABLE[1067][45] = 1;
TABLE[1067][259] = 1;
TABLE[1067][341] = 1;
TABLE[1067][453] = 1;
TABLE[1067][256] = 1;
TABLE[1067][40] = 1;
TABLE[1067][59] = 1;
TABLE[146][43] = 1;
TABLE[146][45] = 1;
TABLE[146][259] = 1;
TABLE[146][341] = 1;
TABLE[146][453] = 1;
TABLE[146][256] = 1;
TABLE[146][40] = 1;
TABLE[147][126] = 1;
TABLE[612][44] = 1;
TABLE[1068][126] = 1;
TABLE[1068][43] = 1;
TABLE[1068][45] = 1;
TABLE[1068][259] = 1;
TABLE[1068][341] = 1;
TABLE[1068][453] = 1;
TABLE[1068][256] = 1;
TABLE[1068][40] = 1;
TABLE[1068][59] = 1;
TABLE[613][44] = 1;
TABLE[1069][126] = 1;
TABLE[1069][43] = 1;
TABLE[1069][45] = 1;
TABLE[1069][259] = 1;
TABLE[1069][341] = 1;
TABLE[1069][453] = 1;
TABLE[1069][256] = 1;
TABLE[1069][40] = 1;
TABLE[1069][59] = 1;
TABLE[148][344] = 1;
TABLE[614][323] = 1;
TABLE[614][379] = 1;
TABLE[614][385] = 1;
TABLE[614][361] = 1;
TABLE[614][346] = 1;
TABLE[614][467] = 1;
TABLE[614][340] = 1;
TABLE[614][396] = 1;
TABLE[614][364] = 1;
TABLE[614][353] = 1;
TABLE[614][331] = 1;
TABLE[614][443] = 1;
TABLE[614][429] = 1;
TABLE[614][373] = 1;
TABLE[614][308] = 1;
TABLE[614][264] = 1;
TABLE[614][267] = 1;
TABLE[614][263] = 1;
TABLE[614][271] = 1;
TABLE[614][272] = 1;
TABLE[614][273] = 1;
TABLE[614][469] = 1;
TABLE[1070][125] = 1;
TABLE[149][323] = 1;
TABLE[150][379] = 1;
TABLE[151][353] = 1;
TABLE[151][364] = 1;
TABLE[152][385] = 1;
TABLE[153][361] = 1;
TABLE[154][346] = 1;
TABLE[155][443] = 1;
TABLE[155][331] = 1;
TABLE[156][467] = 1;
TABLE[157][340] = 1;
TABLE[158][396] = 1;
TABLE[159][308] = 1;
TABLE[159][373] = 1;
TABLE[159][429] = 1;
TABLE[160][469] = 1;
TABLE[160][273] = 1;
TABLE[160][272] = 1;
TABLE[160][271] = 1;
TABLE[160][263] = 1;
TABLE[160][267] = 1;
TABLE[160][264] = 1;
TABLE[615][58] = 1;
TABLE[1071][257] = 1;
TABLE[616][58] = 1;
TABLE[1072][43] = 1;
TABLE[1072][45] = 1;
TABLE[1072][259] = 1;
TABLE[1072][341] = 1;
TABLE[1072][453] = 1;
TABLE[1072][256] = 1;
TABLE[1072][40] = 1;
TABLE[617][58] = 1;
TABLE[1073][43] = 1;
TABLE[1073][45] = 1;
TABLE[1073][259] = 1;
TABLE[1073][341] = 1;
TABLE[1073][453] = 1;
TABLE[1073][256] = 1;
TABLE[1073][40] = 1;
TABLE[618][58] = 1;
TABLE[1074][43] = 1;
TABLE[1074][45] = 1;
TABLE[1074][259] = 1;
TABLE[1074][341] = 1;
TABLE[1074][453] = 1;
TABLE[1074][256] = 1;
TABLE[1074][40] = 1;
TABLE[619][58] = 1;
TABLE[1075][45] = 1;
TABLE[1075][43] = 1;
TABLE[1075][40] = 1;
TABLE[1075][256] = 1;
TABLE[1075][453] = 1;
TABLE[1075][341] = 1;
TABLE[1075][259] = 1;
TABLE[1075][59] = 1;
TABLE[622][43] = 1;
TABLE[622][45] = 1;
TABLE[622][259] = 1;
TABLE[622][341] = 1;
TABLE[622][453] = 1;
TABLE[622][256] = 1;
TABLE[622][40] = 1;
TABLE[1077][59] = 1;
TABLE[620][124] = 1;
TABLE[621][44] = 1;
TABLE[1076][45] = 1;
TABLE[1076][43] = 1;
TABLE[1076][40] = 1;
TABLE[1076][256] = 1;
TABLE[1076][453] = 1;
TABLE[1076][341] = 1;
TABLE[1076][259] = 1;
TABLE[1076][59] = 1;
TABLE[623][443] = 1;
TABLE[624][331] = 1;
TABLE[625][58] = 1;
TABLE[1078][36] = 1;
TABLE[626][58] = 1;
TABLE[1079][43] = 1;
TABLE[1079][45] = 1;
TABLE[1079][259] = 1;
TABLE[1079][341] = 1;
TABLE[1079][453] = 1;
TABLE[1079][256] = 1;
TABLE[1079][40] = 1;
TABLE[627][58] = 1;
TABLE[1080][259] = 1;
TABLE[1080][341] = 1;
TABLE[1080][453] = 1;
TABLE[1080][256] = 1;
TABLE[1080][40] = 1;
TABLE[1080][45] = 1;
TABLE[1080][43] = 1;
TABLE[161][431] = 1;
TABLE[161][331] = 1;
TABLE[161][404] = 1;
TABLE[161][327] = 1;
TABLE[161][45] = 1;
TABLE[161][43] = 1;
TABLE[161][40] = 1;
TABLE[161][256] = 1;
TABLE[161][453] = 1;
TABLE[161][341] = 1;
TABLE[161][259] = 1;
TABLE[629][43] = 1;
TABLE[629][45] = 1;
TABLE[629][259] = 1;
TABLE[629][341] = 1;
TABLE[629][453] = 1;
TABLE[629][256] = 1;
TABLE[629][40] = 1;
TABLE[1082][431] = 1;
TABLE[1082][331] = 1;
TABLE[1082][404] = 1;
TABLE[1082][327] = 1;
TABLE[628][47] = 1;
TABLE[1081][45] = 1;
TABLE[1081][43] = 1;
TABLE[1081][40] = 1;
TABLE[1081][256] = 1;
TABLE[1081][453] = 1;
TABLE[1081][341] = 1;
TABLE[1081][259] = 1;
TABLE[1081][431] = 1;
TABLE[1081][331] = 1;
TABLE[1081][404] = 1;
TABLE[1081][327] = 1;
TABLE[630][431] = 1;
TABLE[631][331] = 1;
TABLE[632][404] = 1;
TABLE[633][327] = 1;
TABLE[634][323] = 1;
TABLE[634][370] = 1;
TABLE[634][312] = 1;
TABLE[634][296] = 1;
TABLE[634][408] = 1;
TABLE[634][440] = 1;
TABLE[634][412] = 1;
TABLE[634][467] = 1;
TABLE[634][468] = 1;
TABLE[634][330] = 1;
TABLE[634][409] = 1;
TABLE[634][295] = 1;
TABLE[634][374] = 1;
TABLE[634][456] = 1;
TABLE[634][423] = 1;
TABLE[634][315] = 1;
TABLE[634][346] = 1;
TABLE[634][396] = 1;
TABLE[634][264] = 1;
TABLE[634][267] = 1;
TABLE[634][263] = 1;
TABLE[634][271] = 1;
TABLE[634][272] = 1;
TABLE[634][273] = 1;
TABLE[634][469] = 1;
TABLE[634][429] = 1;
TABLE[634][373] = 1;
TABLE[634][308] = 1;
TABLE[634][45] = 1;
TABLE[634][43] = 1;
TABLE[634][40] = 1;
TABLE[634][256] = 1;
TABLE[634][453] = 1;
TABLE[634][341] = 1;
TABLE[634][259] = 1;
TABLE[1083][125] = 1;
TABLE[636][287] = 1;
TABLE[1085][446] = 1;
TABLE[1085][346] = 1;
TABLE[1085][348] = 1;
TABLE[1085][428] = 1;
TABLE[1085][376] = 1;
TABLE[1085][469] = 1;
TABLE[1085][273] = 1;
TABLE[1085][272] = 1;
TABLE[1085][271] = 1;
TABLE[1085][263] = 1;
TABLE[1085][267] = 1;
TABLE[1085][264] = 1;
TABLE[1085][442] = 1;
TABLE[1085][337] = 1;
TABLE[1085][444] = 1;
TABLE[1085][445] = 1;
TABLE[1085][417] = 1;
TABLE[1085][325] = 1;
TABLE[1085][354] = 1;
TABLE[1085][306] = 1;
TABLE[1085][413] = 1;
TABLE[1085][261] = 1;
TABLE[1085][329] = 1;
TABLE[1085][291] = 1;
TABLE[1085][344] = 1;
TABLE[1085][368] = 1;
TABLE[1085][384] = 1;
TABLE[1085][43] = 1;
TABLE[1085][45] = 1;
TABLE[1085][327] = 1;
TABLE[1085][404] = 1;
TABLE[1085][331] = 1;
TABLE[1085][431] = 1;
TABLE[1085][259] = 1;
TABLE[1085][341] = 1;
TABLE[1085][453] = 1;
TABLE[1085][256] = 1;
TABLE[1085][40] = 1;
TABLE[1085][0] = 1;
TABLE[635][323] = 1;
TABLE[635][370] = 1;
TABLE[635][312] = 1;
TABLE[635][296] = 1;
TABLE[635][408] = 1;
TABLE[635][440] = 1;
TABLE[635][412] = 1;
TABLE[635][467] = 1;
TABLE[635][468] = 1;
TABLE[635][330] = 1;
TABLE[635][409] = 1;
TABLE[635][295] = 1;
TABLE[635][374] = 1;
TABLE[635][456] = 1;
TABLE[635][423] = 1;
TABLE[635][315] = 1;
TABLE[635][346] = 1;
TABLE[635][396] = 1;
TABLE[635][264] = 1;
TABLE[635][267] = 1;
TABLE[635][263] = 1;
TABLE[635][271] = 1;
TABLE[635][272] = 1;
TABLE[635][273] = 1;
TABLE[635][469] = 1;
TABLE[635][429] = 1;
TABLE[635][373] = 1;
TABLE[635][308] = 1;
TABLE[635][45] = 1;
TABLE[635][43] = 1;
TABLE[635][40] = 1;
TABLE[635][256] = 1;
TABLE[635][453] = 1;
TABLE[635][341] = 1;
TABLE[635][259] = 1;
TABLE[1084][125] = 1;
TABLE[162][323] = 1;
TABLE[163][43] = 1;
TABLE[163][45] = 1;
TABLE[163][259] = 1;
TABLE[163][341] = 1;
TABLE[163][453] = 1;
TABLE[163][256] = 1;
TABLE[163][40] = 1;
TABLE[164][370] = 1;
TABLE[165][312] = 1;
TABLE[166][296] = 1;
TABLE[167][408] = 1;
TABLE[168][440] = 1;
TABLE[169][412] = 1;
TABLE[170][467] = 1;
TABLE[171][468] = 1;
TABLE[172][330] = 1;
TABLE[173][409] = 1;
TABLE[174][295] = 1;
TABLE[175][374] = 1;
TABLE[176][456] = 1;
TABLE[177][423] = 1;
TABLE[178][423] = 1;
TABLE[179][315] = 1;
TABLE[180][315] = 1;
TABLE[181][346] = 1;
TABLE[182][396] = 1;
TABLE[183][469] = 1;
TABLE[183][273] = 1;
TABLE[183][272] = 1;
TABLE[183][271] = 1;
TABLE[183][263] = 1;
TABLE[183][267] = 1;
TABLE[183][264] = 1;
TABLE[184][308] = 1;
TABLE[184][373] = 1;
TABLE[184][429] = 1;
TABLE[637][58] = 1;
TABLE[1086][257] = 1;
TABLE[638][58] = 1;
TABLE[1087][43] = 1;
TABLE[1087][45] = 1;
TABLE[1087][259] = 1;
TABLE[1087][341] = 1;
TABLE[1087][453] = 1;
TABLE[1087][256] = 1;
TABLE[1087][40] = 1;
TABLE[639][58] = 1;
TABLE[1088][43] = 1;
TABLE[1088][45] = 1;
TABLE[1088][259] = 1;
TABLE[1088][341] = 1;
TABLE[1088][453] = 1;
TABLE[1088][256] = 1;
TABLE[1088][40] = 1;
TABLE[640][305] = 1;
TABLE[1089][58] = 1;
TABLE[1089][43] = 1;
TABLE[1089][45] = 1;
TABLE[1089][259] = 1;
TABLE[1089][341] = 1;
TABLE[1089][453] = 1;
TABLE[1089][256] = 1;
TABLE[1089][40] = 1;
TABLE[641][58] = 1;
TABLE[1090][43] = 1;
TABLE[1090][45] = 1;
TABLE[1090][259] = 1;
TABLE[1090][341] = 1;
TABLE[1090][453] = 1;
TABLE[1090][256] = 1;
TABLE[1090][40] = 1;
TABLE[642][37] = 1;
TABLE[1091][59] = 1;
TABLE[643][58] = 1;
TABLE[1092][43] = 1;
TABLE[1092][45] = 1;
TABLE[1092][259] = 1;
TABLE[1092][341] = 1;
TABLE[1092][453] = 1;
TABLE[1092][256] = 1;
TABLE[1092][40] = 1;
TABLE[644][40] = 1;
TABLE[1093][59] = 1;
TABLE[645][58] = 1;
TABLE[1094][43] = 1;
TABLE[1094][45] = 1;
TABLE[1094][259] = 1;
TABLE[1094][341] = 1;
TABLE[1094][453] = 1;
TABLE[1094][256] = 1;
TABLE[1094][40] = 1;
TABLE[646][401] = 1;
TABLE[1095][59] = 1;
TABLE[647][58] = 1;
TABLE[1096][43] = 1;
TABLE[1096][45] = 1;
TABLE[1096][259] = 1;
TABLE[1096][341] = 1;
TABLE[1096][453] = 1;
TABLE[1096][256] = 1;
TABLE[1096][40] = 1;
TABLE[648][58] = 1;
TABLE[1097][43] = 1;
TABLE[1097][45] = 1;
TABLE[1097][259] = 1;
TABLE[1097][341] = 1;
TABLE[1097][453] = 1;
TABLE[1097][256] = 1;
TABLE[1097][40] = 1;
TABLE[649][58] = 1;
TABLE[1098][43] = 1;
TABLE[1098][45] = 1;
TABLE[1098][259] = 1;
TABLE[1098][341] = 1;
TABLE[1098][453] = 1;
TABLE[1098][256] = 1;
TABLE[1098][40] = 1;
TABLE[650][58] = 1;
TABLE[1099][43] = 1;
TABLE[1099][45] = 1;
TABLE[1099][259] = 1;
TABLE[1099][341] = 1;
TABLE[1099][453] = 1;
TABLE[1099][256] = 1;
TABLE[1099][40] = 1;
TABLE[651][58] = 1;
TABLE[1100][259] = 1;
TABLE[1100][341] = 1;
TABLE[1100][453] = 1;
TABLE[1100][256] = 1;
TABLE[1100][40] = 1;
TABLE[1100][45] = 1;
TABLE[1100][43] = 1;
TABLE[652][58] = 1;
TABLE[1101][43] = 1;
TABLE[1101][45] = 1;
TABLE[1101][259] = 1;
TABLE[1101][341] = 1;
TABLE[1101][453] = 1;
TABLE[1101][256] = 1;
TABLE[1101][40] = 1;
TABLE[653][58] = 1;
TABLE[1102][43] = 1;
TABLE[1102][45] = 1;
TABLE[1102][259] = 1;
TABLE[1102][341] = 1;
TABLE[1102][453] = 1;
TABLE[1102][256] = 1;
TABLE[1102][40] = 1;
TABLE[654][58] = 1;
TABLE[1103][43] = 1;
TABLE[1103][45] = 1;
TABLE[1103][259] = 1;
TABLE[1103][341] = 1;
TABLE[1103][453] = 1;
TABLE[1103][256] = 1;
TABLE[1103][40] = 1;
TABLE[655][58] = 1;
TABLE[1104][36] = 1;
TABLE[656][58] = 1;
TABLE[1105][43] = 1;
TABLE[1105][45] = 1;
TABLE[1105][259] = 1;
TABLE[1105][341] = 1;
TABLE[1105][453] = 1;
TABLE[1105][256] = 1;
TABLE[1105][40] = 1;
TABLE[657][58] = 1;
TABLE[1106][43] = 1;
TABLE[1106][45] = 1;
TABLE[1106][259] = 1;
TABLE[1106][341] = 1;
TABLE[1106][453] = 1;
TABLE[1106][256] = 1;
TABLE[1106][40] = 1;
TABLE[658][58] = 1;
TABLE[1107][274] = 1;
TABLE[659][58] = 1;
TABLE[1108][45] = 1;
TABLE[1108][43] = 1;
TABLE[1108][40] = 1;
TABLE[1108][256] = 1;
TABLE[1108][453] = 1;
TABLE[1108][341] = 1;
TABLE[1108][259] = 1;
TABLE[1108][59] = 1;
TABLE[662][43] = 1;
TABLE[662][45] = 1;
TABLE[662][259] = 1;
TABLE[662][341] = 1;
TABLE[662][453] = 1;
TABLE[662][256] = 1;
TABLE[662][40] = 1;
TABLE[1110][59] = 1;
TABLE[660][44] = 1;
TABLE[661][124] = 1;
TABLE[1109][45] = 1;
TABLE[1109][43] = 1;
TABLE[1109][40] = 1;
TABLE[1109][256] = 1;
TABLE[1109][453] = 1;
TABLE[1109][341] = 1;
TABLE[1109][259] = 1;
TABLE[1109][59] = 1;
TABLE[185][45] = 1;
TABLE[185][43] = 1;
TABLE[185][40] = 1;
TABLE[185][256] = 1;
TABLE[185][453] = 1;
TABLE[185][341] = 1;
TABLE[185][259] = 1;
TABLE[665][43] = 1;
TABLE[665][45] = 1;
TABLE[665][259] = 1;
TABLE[665][341] = 1;
TABLE[665][453] = 1;
TABLE[665][256] = 1;
TABLE[665][40] = 1;
TABLE[663][44] = 1;
TABLE[664][124] = 1;
TABLE[1111][45] = 1;
TABLE[1111][43] = 1;
TABLE[1111][40] = 1;
TABLE[1111][256] = 1;
TABLE[1111][453] = 1;
TABLE[1111][341] = 1;
TABLE[1111][259] = 1;
TABLE[186][261] = 1;
TABLE[187][413] = 1;
TABLE[666][58] = 1;
TABLE[1113][123] = 1;
TABLE[667][125] = 1;
TABLE[667][323] = 1;
TABLE[667][59] = 1;
TABLE[667][355] = 1;
TABLE[667][384] = 1;
TABLE[667][336] = 1;
TABLE[667][427] = 1;
TABLE[667][45] = 1;
TABLE[667][43] = 1;
TABLE[667][346] = 1;
TABLE[667][352] = 1;
TABLE[667][396] = 1;
TABLE[667][276] = 1;
TABLE[667][40] = 1;
TABLE[667][256] = 1;
TABLE[667][453] = 1;
TABLE[667][341] = 1;
TABLE[667][259] = 1;
TABLE[667][429] = 1;
TABLE[667][373] = 1;
TABLE[667][308] = 1;
TABLE[667][264] = 1;
TABLE[667][267] = 1;
TABLE[667][263] = 1;
TABLE[667][271] = 1;
TABLE[667][272] = 1;
TABLE[667][273] = 1;
TABLE[667][469] = 1;
TABLE[1114][125] = 1;
TABLE[188][323] = 1;
TABLE[189][59] = 1;
TABLE[189][276] = 1;
TABLE[190][355] = 1;
TABLE[191][384] = 1;
TABLE[192][336] = 1;
TABLE[193][336] = 1;
TABLE[194][427] = 1;
TABLE[195][43] = 1;
TABLE[195][45] = 1;
TABLE[195][259] = 1;
TABLE[195][341] = 1;
TABLE[195][453] = 1;
TABLE[195][256] = 1;
TABLE[195][40] = 1;
TABLE[196][346] = 1;
TABLE[197][352] = 1;
TABLE[198][308] = 1;
TABLE[198][373] = 1;
TABLE[198][429] = 1;
TABLE[199][396] = 1;
TABLE[200][469] = 1;
TABLE[200][273] = 1;
TABLE[200][272] = 1;
TABLE[200][271] = 1;
TABLE[200][263] = 1;
TABLE[200][267] = 1;
TABLE[200][264] = 1;
TABLE[668][58] = 1;
TABLE[1115][257] = 1;
TABLE[671][276] = 1;
TABLE[1118][59] = 1;
TABLE[669][58] = 1;
TABLE[1116][43] = 1;
TABLE[1116][45] = 1;
TABLE[1116][259] = 1;
TABLE[1116][341] = 1;
TABLE[1116][453] = 1;
TABLE[1116][256] = 1;
TABLE[1116][40] = 1;
TABLE[670][44] = 1;
TABLE[1117][276] = 1;
TABLE[1117][59] = 1;
TABLE[672][58] = 1;
TABLE[1119][343] = 1;
TABLE[1119][435] = 1;
TABLE[1119][281] = 1;
TABLE[1119][59] = 1;
TABLE[674][281] = 1;
TABLE[674][435] = 1;
TABLE[674][343] = 1;
TABLE[1121][59] = 1;
TABLE[673][44] = 1;
TABLE[1120][343] = 1;
TABLE[1120][435] = 1;
TABLE[1120][281] = 1;
TABLE[1120][59] = 1;
TABLE[675][58] = 1;
TABLE[1122][36] = 1;
TABLE[676][306] = 1;
TABLE[1123][58] = 1;
TABLE[1123][36] = 1;
TABLE[677][58] = 1;
TABLE[1124][36] = 1;
TABLE[678][306] = 1;
TABLE[1125][58] = 1;
TABLE[1125][36] = 1;
TABLE[679][58] = 1;
TABLE[1126][36] = 1;
TABLE[680][58] = 1;
TABLE[1127][45] = 1;
TABLE[1127][43] = 1;
TABLE[1127][40] = 1;
TABLE[1127][256] = 1;
TABLE[1127][453] = 1;
TABLE[1127][341] = 1;
TABLE[1127][259] = 1;
TABLE[1127][59] = 1;
TABLE[682][43] = 1;
TABLE[682][45] = 1;
TABLE[682][259] = 1;
TABLE[682][341] = 1;
TABLE[682][453] = 1;
TABLE[682][256] = 1;
TABLE[682][40] = 1;
TABLE[1129][59] = 1;
TABLE[681][44] = 1;
TABLE[1128][45] = 1;
TABLE[1128][43] = 1;
TABLE[1128][40] = 1;
TABLE[1128][256] = 1;
TABLE[1128][453] = 1;
TABLE[1128][341] = 1;
TABLE[1128][259] = 1;
TABLE[1128][59] = 1;
TABLE[683][58] = 1;
TABLE[1130][257] = 1;
TABLE[684][58] = 1;
TABLE[1131][45] = 1;
TABLE[1131][43] = 1;
TABLE[1131][40] = 1;
TABLE[1131][256] = 1;
TABLE[1131][453] = 1;
TABLE[1131][341] = 1;
TABLE[1131][259] = 1;
TABLE[1131][59] = 1;
TABLE[687][43] = 1;
TABLE[687][45] = 1;
TABLE[687][259] = 1;
TABLE[687][341] = 1;
TABLE[687][453] = 1;
TABLE[687][256] = 1;
TABLE[687][40] = 1;
TABLE[1133][59] = 1;
TABLE[685][124] = 1;
TABLE[686][44] = 1;
TABLE[1132][45] = 1;
TABLE[1132][43] = 1;
TABLE[1132][40] = 1;
TABLE[1132][256] = 1;
TABLE[1132][453] = 1;
TABLE[1132][341] = 1;
TABLE[1132][259] = 1;
TABLE[1132][59] = 1;
TABLE[201][306] = 1;
TABLE[688][323] = 1;
TABLE[688][360] = 1;
TABLE[688][377] = 1;
TABLE[688][320] = 1;
TABLE[688][294] = 1;
TABLE[688][424] = 1;
TABLE[688][355] = 1;
TABLE[688][427] = 1;
TABLE[688][406] = 1;
TABLE[688][346] = 1;
TABLE[688][352] = 1;
TABLE[688][396] = 1;
TABLE[688][429] = 1;
TABLE[688][373] = 1;
TABLE[688][308] = 1;
TABLE[688][264] = 1;
TABLE[688][267] = 1;
TABLE[688][263] = 1;
TABLE[688][271] = 1;
TABLE[688][272] = 1;
TABLE[688][273] = 1;
TABLE[688][469] = 1;
TABLE[1134][125] = 1;
TABLE[202][323] = 1;
TABLE[203][360] = 1;
TABLE[204][377] = 1;
TABLE[205][320] = 1;
TABLE[206][294] = 1;
TABLE[207][424] = 1;
TABLE[208][355] = 1;
TABLE[209][427] = 1;
TABLE[210][406] = 1;
TABLE[211][346] = 1;
TABLE[212][352] = 1;
TABLE[213][396] = 1;
TABLE[214][308] = 1;
TABLE[214][373] = 1;
TABLE[214][429] = 1;
TABLE[215][469] = 1;
TABLE[215][273] = 1;
TABLE[215][272] = 1;
TABLE[215][271] = 1;
TABLE[215][263] = 1;
TABLE[215][267] = 1;
TABLE[215][264] = 1;
TABLE[689][58] = 1;
TABLE[1135][257] = 1;
TABLE[690][58] = 1;
TABLE[1136][100] = 1;
TABLE[1136][43] = 1;
TABLE[1136][45] = 1;
TABLE[1136][259] = 1;
TABLE[1136][341] = 1;
TABLE[1136][453] = 1;
TABLE[1136][256] = 1;
TABLE[1136][40] = 1;
TABLE[691][100] = 1;
TABLE[1137][43] = 1;
TABLE[1137][45] = 1;
TABLE[1137][259] = 1;
TABLE[1137][341] = 1;
TABLE[1137][453] = 1;
TABLE[1137][256] = 1;
TABLE[1137][40] = 1;
TABLE[692][58] = 1;
TABLE[1138][100] = 1;
TABLE[1138][43] = 1;
TABLE[1138][45] = 1;
TABLE[1138][259] = 1;
TABLE[1138][341] = 1;
TABLE[1138][453] = 1;
TABLE[1138][256] = 1;
TABLE[1138][40] = 1;
TABLE[693][100] = 1;
TABLE[1139][43] = 1;
TABLE[1139][45] = 1;
TABLE[1139][259] = 1;
TABLE[1139][341] = 1;
TABLE[1139][453] = 1;
TABLE[1139][256] = 1;
TABLE[1139][40] = 1;
TABLE[694][58] = 1;
TABLE[1140][43] = 1;
TABLE[1140][45] = 1;
TABLE[1140][259] = 1;
TABLE[1140][341] = 1;
TABLE[1140][453] = 1;
TABLE[1140][256] = 1;
TABLE[1140][40] = 1;
TABLE[695][58] = 1;
TABLE[1141][45] = 1;
TABLE[1141][43] = 1;
TABLE[1141][40] = 1;
TABLE[1141][256] = 1;
TABLE[1141][453] = 1;
TABLE[1141][341] = 1;
TABLE[1141][259] = 1;
TABLE[1141][59] = 1;
TABLE[697][43] = 1;
TABLE[697][45] = 1;
TABLE[697][259] = 1;
TABLE[697][341] = 1;
TABLE[697][453] = 1;
TABLE[697][256] = 1;
TABLE[697][40] = 1;
TABLE[1143][59] = 1;
TABLE[696][44] = 1;
TABLE[1142][45] = 1;
TABLE[1142][43] = 1;
TABLE[1142][40] = 1;
TABLE[1142][256] = 1;
TABLE[1142][453] = 1;
TABLE[1142][341] = 1;
TABLE[1142][259] = 1;
TABLE[1142][59] = 1;
TABLE[698][58] = 1;
TABLE[1144][45] = 1;
TABLE[1144][43] = 1;
TABLE[1144][40] = 1;
TABLE[1144][256] = 1;
TABLE[1144][453] = 1;
TABLE[1144][341] = 1;
TABLE[1144][259] = 1;
TABLE[1144][59] = 1;
TABLE[700][43] = 1;
TABLE[700][45] = 1;
TABLE[700][259] = 1;
TABLE[700][341] = 1;
TABLE[700][453] = 1;
TABLE[700][256] = 1;
TABLE[700][40] = 1;
TABLE[1146][59] = 1;
TABLE[699][44] = 1;
TABLE[1145][45] = 1;
TABLE[1145][43] = 1;
TABLE[1145][40] = 1;
TABLE[1145][256] = 1;
TABLE[1145][453] = 1;
TABLE[1145][341] = 1;
TABLE[1145][259] = 1;
TABLE[1145][59] = 1;
TABLE[701][58] = 1;
TABLE[1147][343] = 1;
TABLE[1147][435] = 1;
TABLE[1147][281] = 1;
TABLE[1147][59] = 1;
TABLE[703][281] = 1;
TABLE[703][435] = 1;
TABLE[703][343] = 1;
TABLE[1149][59] = 1;
TABLE[702][44] = 1;
TABLE[1148][343] = 1;
TABLE[1148][435] = 1;
TABLE[1148][281] = 1;
TABLE[1148][59] = 1;
TABLE[704][91] = 1;
TABLE[705][93] = 1;
TABLE[706][58] = 1;
TABLE[1150][45] = 1;
TABLE[1150][43] = 1;
TABLE[1150][40] = 1;
TABLE[1150][256] = 1;
TABLE[1150][453] = 1;
TABLE[1150][341] = 1;
TABLE[1150][259] = 1;
TABLE[1150][59] = 1;
TABLE[708][43] = 1;
TABLE[708][45] = 1;
TABLE[708][259] = 1;
TABLE[708][341] = 1;
TABLE[708][453] = 1;
TABLE[708][256] = 1;
TABLE[708][40] = 1;
TABLE[1152][59] = 1;
TABLE[707][44] = 1;
TABLE[1151][45] = 1;
TABLE[1151][43] = 1;
TABLE[1151][40] = 1;
TABLE[1151][256] = 1;
TABLE[1151][453] = 1;
TABLE[1151][341] = 1;
TABLE[1151][259] = 1;
TABLE[1151][59] = 1;
TABLE[709][58] = 1;
TABLE[1153][45] = 1;
TABLE[1153][43] = 1;
TABLE[1153][40] = 1;
TABLE[1153][256] = 1;
TABLE[1153][453] = 1;
TABLE[1153][341] = 1;
TABLE[1153][259] = 1;
TABLE[1153][59] = 1;
TABLE[711][43] = 1;
TABLE[711][45] = 1;
TABLE[711][259] = 1;
TABLE[711][341] = 1;
TABLE[711][453] = 1;
TABLE[711][256] = 1;
TABLE[711][40] = 1;
TABLE[1155][59] = 1;
TABLE[710][44] = 1;
TABLE[1154][45] = 1;
TABLE[1154][43] = 1;
TABLE[1154][40] = 1;
TABLE[1154][256] = 1;
TABLE[1154][453] = 1;
TABLE[1154][341] = 1;
TABLE[1154][259] = 1;
TABLE[1154][59] = 1;
TABLE[712][58] = 1;
TABLE[1156][45] = 1;
TABLE[1156][43] = 1;
TABLE[1156][40] = 1;
TABLE[1156][256] = 1;
TABLE[1156][453] = 1;
TABLE[1156][341] = 1;
TABLE[1156][259] = 1;
TABLE[1156][59] = 1;
TABLE[715][43] = 1;
TABLE[715][45] = 1;
TABLE[715][259] = 1;
TABLE[715][341] = 1;
TABLE[715][453] = 1;
TABLE[715][256] = 1;
TABLE[715][40] = 1;
TABLE[1158][59] = 1;
TABLE[713][124] = 1;
TABLE[714][44] = 1;
TABLE[1157][45] = 1;
TABLE[1157][43] = 1;
TABLE[1157][40] = 1;
TABLE[1157][256] = 1;
TABLE[1157][453] = 1;
TABLE[1157][341] = 1;
TABLE[1157][259] = 1;
TABLE[1157][59] = 1;
TABLE[216][281] = 1;
TABLE[217][435] = 1;
TABLE[218][343] = 1;
TABLE[716][44] = 1;
TABLE[1159][93] = 1;
TABLE[717][44] = 1;
TABLE[1160][93] = 1;
TABLE[718][44] = 1;
TABLE[1161][93] = 1;
TABLE[219][44] = 1;
TABLE[220][44] = 1;
TABLE[221][44] = 1;
TABLE[222][43] = 1;
TABLE[222][45] = 1;
TABLE[222][259] = 1;
TABLE[222][341] = 1;
TABLE[222][453] = 1;
TABLE[222][256] = 1;
TABLE[222][40] = 1;
TABLE[223][339] = 1;
TABLE[224][339] = 1;
TABLE[225][339] = 1;
TABLE[226][339] = 1;
TABLE[719][370] = 1;
TABLE[1162][44] = 1;
TABLE[1162][343] = 1;
TABLE[1162][435] = 1;
TABLE[1162][281] = 1;
TABLE[1162][59] = 1;
TABLE[720][370] = 1;
TABLE[1163][44] = 1;
TABLE[1163][343] = 1;
TABLE[1163][435] = 1;
TABLE[1163][281] = 1;
TABLE[1163][59] = 1;
TABLE[721][370] = 1;
TABLE[1164][44] = 1;
TABLE[1164][343] = 1;
TABLE[1164][435] = 1;
TABLE[1164][281] = 1;
TABLE[1164][59] = 1;
TABLE[227][329] = 1;
TABLE[722][323] = 1;
TABLE[722][396] = 1;
TABLE[722][429] = 1;
TABLE[722][373] = 1;
TABLE[722][308] = 1;
TABLE[722][264] = 1;
TABLE[722][267] = 1;
TABLE[722][263] = 1;
TABLE[722][271] = 1;
TABLE[722][272] = 1;
TABLE[722][273] = 1;
TABLE[722][469] = 1;
TABLE[1165][125] = 1;
TABLE[228][323] = 1;
TABLE[229][308] = 1;
TABLE[229][373] = 1;
TABLE[229][429] = 1;
TABLE[230][396] = 1;
TABLE[231][469] = 1;
TABLE[231][273] = 1;
TABLE[231][272] = 1;
TABLE[231][271] = 1;
TABLE[231][263] = 1;
TABLE[231][267] = 1;
TABLE[231][264] = 1;
TABLE[723][58] = 1;
TABLE[1166][257] = 1;
TABLE[232][308] = 1;
TABLE[233][373] = 1;
TABLE[234][429] = 1;
TABLE[724][58] = 1;
TABLE[1167][42] = 1;
TABLE[1167][59] = 1;
TABLE[726][42] = 1;
TABLE[728][42] = 1;
TABLE[1170][59] = 1;
TABLE[725][44] = 1;
TABLE[1168][42] = 1;
TABLE[1168][59] = 1;
TABLE[727][44] = 1;
TABLE[1169][42] = 1;
TABLE[1169][59] = 1;
TABLE[729][58] = 1;
TABLE[1171][42] = 1;
TABLE[1171][396] = 1;
TABLE[1171][352] = 1;
TABLE[1171][404] = 1;
TABLE[1171][327] = 1;
TABLE[1171][432] = 1;
TABLE[1171][385] = 1;
TABLE[1171][430] = 1;
TABLE[1171][426] = 1;
TABLE[1171][346] = 1;
TABLE[1171][342] = 1;
TABLE[1171][377] = 1;
TABLE[1171][290] = 1;
TABLE[1171][320] = 1;
TABLE[1171][292] = 1;
TABLE[1171][294] = 1;
TABLE[1171][365] = 1;
TABLE[1171][419] = 1;
TABLE[1171][435] = 1;
TABLE[1171][362] = 1;
TABLE[1171][361] = 1;
TABLE[1171][360] = 1;
TABLE[1171][322] = 1;
TABLE[1171][323] = 1;
TABLE[1171][469] = 1;
TABLE[1171][273] = 1;
TABLE[1171][272] = 1;
TABLE[1171][271] = 1;
TABLE[1171][263] = 1;
TABLE[1171][267] = 1;
TABLE[1171][264] = 1;
TABLE[1171][308] = 1;
TABLE[1171][373] = 1;
TABLE[1171][429] = 1;
TABLE[1171][276] = 1;
TABLE[1171][353] = 1;
TABLE[1171][364] = 1;
TABLE[1171][43] = 1;
TABLE[1171][370] = 1;
TABLE[1171][304] = 1;
TABLE[1171][125] = 1;
TABLE[1171][351] = 1;
TABLE[1171][371] = 1;
TABLE[1171][372] = 1;
TABLE[1171][310] = 1;
TABLE[1171][451] = 1;
TABLE[1171][375] = 1;
TABLE[1171][462] = 1;
TABLE[1171][463] = 1;
TABLE[1171][301] = 1;
TABLE[1171][357] = 1;
TABLE[1171][345] = 1;
TABLE[1171][402] = 1;
TABLE[1171][309] = 1;
TABLE[1171][392] = 1;
TABLE[1171][461] = 1;
TABLE[1171][312] = 1;
TABLE[1171][379] = 1;
TABLE[1171][415] = 1;
TABLE[1171][400] = 1;
TABLE[1171][283] = 1;
TABLE[1171][447] = 1;
TABLE[1171][313] = 1;
TABLE[1171][369] = 1;
TABLE[1171][425] = 1;
TABLE[1171][340] = 1;
TABLE[1171][467] = 1;
TABLE[1171][443] = 1;
TABLE[1171][331] = 1;
TABLE[1171][315] = 1;
TABLE[1171][423] = 1;
TABLE[1171][456] = 1;
TABLE[1171][374] = 1;
TABLE[1171][295] = 1;
TABLE[1171][409] = 1;
TABLE[1171][330] = 1;
TABLE[1171][468] = 1;
TABLE[1171][412] = 1;
TABLE[1171][440] = 1;
TABLE[1171][408] = 1;
TABLE[1171][296] = 1;
TABLE[1171][45] = 1;
TABLE[1171][259] = 1;
TABLE[1171][341] = 1;
TABLE[1171][453] = 1;
TABLE[1171][256] = 1;
TABLE[1171][40] = 1;
TABLE[1171][427] = 1;
TABLE[1171][336] = 1;
TABLE[1171][384] = 1;
TABLE[1171][355] = 1;
TABLE[1171][59] = 1;
TABLE[1171][406] = 1;
TABLE[1171][424] = 1;
TABLE[1171][326] = 1;
TABLE[1171][394] = 1;
TABLE[1171][418] = 1;
TABLE[1171][450] = 1;
TABLE[1171][356] = 1;
TABLE[1171][421] = 1;
TABLE[1171][324] = 1;
TABLE[1171][347] = 1;
TABLE[1171][459] = 1;
TABLE[1171][303] = 1;
TABLE[1171][405] = 1;
TABLE[1171][316] = 1;
TABLE[1171][386] = 1;
TABLE[1171][349] = 1;
TABLE[733][42] = 1;
TABLE[1173][396] = 1;
TABLE[1173][352] = 1;
TABLE[1173][404] = 1;
TABLE[1173][327] = 1;
TABLE[1173][432] = 1;
TABLE[1173][385] = 1;
TABLE[1173][430] = 1;
TABLE[1173][426] = 1;
TABLE[1173][346] = 1;
TABLE[1173][342] = 1;
TABLE[1173][377] = 1;
TABLE[1173][290] = 1;
TABLE[1173][320] = 1;
TABLE[1173][292] = 1;
TABLE[1173][294] = 1;
TABLE[1173][365] = 1;
TABLE[1173][419] = 1;
TABLE[1173][435] = 1;
TABLE[1173][362] = 1;
TABLE[1173][361] = 1;
TABLE[1173][360] = 1;
TABLE[1173][322] = 1;
TABLE[1173][323] = 1;
TABLE[1173][469] = 1;
TABLE[1173][273] = 1;
TABLE[1173][272] = 1;
TABLE[1173][271] = 1;
TABLE[1173][263] = 1;
TABLE[1173][267] = 1;
TABLE[1173][264] = 1;
TABLE[1173][308] = 1;
TABLE[1173][373] = 1;
TABLE[1173][429] = 1;
TABLE[1173][276] = 1;
TABLE[1173][353] = 1;
TABLE[1173][364] = 1;
TABLE[1173][43] = 1;
TABLE[1173][370] = 1;
TABLE[1173][304] = 1;
TABLE[1173][125] = 1;
TABLE[1173][351] = 1;
TABLE[1173][371] = 1;
TABLE[1173][372] = 1;
TABLE[1173][310] = 1;
TABLE[1173][451] = 1;
TABLE[1173][375] = 1;
TABLE[1173][462] = 1;
TABLE[1173][463] = 1;
TABLE[1173][301] = 1;
TABLE[1173][357] = 1;
TABLE[1173][345] = 1;
TABLE[1173][402] = 1;
TABLE[1173][309] = 1;
TABLE[1173][392] = 1;
TABLE[1173][461] = 1;
TABLE[1173][312] = 1;
TABLE[1173][379] = 1;
TABLE[1173][415] = 1;
TABLE[1173][400] = 1;
TABLE[1173][283] = 1;
TABLE[1173][447] = 1;
TABLE[1173][313] = 1;
TABLE[1173][369] = 1;
TABLE[1173][425] = 1;
TABLE[1173][340] = 1;
TABLE[1173][467] = 1;
TABLE[1173][443] = 1;
TABLE[1173][331] = 1;
TABLE[1173][315] = 1;
TABLE[1173][423] = 1;
TABLE[1173][456] = 1;
TABLE[1173][374] = 1;
TABLE[1173][295] = 1;
TABLE[1173][409] = 1;
TABLE[1173][330] = 1;
TABLE[1173][468] = 1;
TABLE[1173][412] = 1;
TABLE[1173][440] = 1;
TABLE[1173][408] = 1;
TABLE[1173][296] = 1;
TABLE[1173][45] = 1;
TABLE[1173][259] = 1;
TABLE[1173][341] = 1;
TABLE[1173][453] = 1;
TABLE[1173][256] = 1;
TABLE[1173][40] = 1;
TABLE[1173][427] = 1;
TABLE[1173][336] = 1;
TABLE[1173][384] = 1;
TABLE[1173][355] = 1;
TABLE[1173][59] = 1;
TABLE[1173][406] = 1;
TABLE[1173][424] = 1;
TABLE[1173][326] = 1;
TABLE[1173][394] = 1;
TABLE[1173][418] = 1;
TABLE[1173][450] = 1;
TABLE[1173][356] = 1;
TABLE[1173][421] = 1;
TABLE[1173][324] = 1;
TABLE[1173][347] = 1;
TABLE[1173][459] = 1;
TABLE[1173][303] = 1;
TABLE[1173][405] = 1;
TABLE[1173][316] = 1;
TABLE[1173][386] = 1;
TABLE[1173][349] = 1;
TABLE[730][36] = 1;
TABLE[730][259] = 1;
TABLE[730][341] = 1;
TABLE[730][453] = 1;
TABLE[730][256] = 1;
TABLE[730][40] = 1;
TABLE[730][257] = 1;
TABLE[1172][44] = 1;
TABLE[1172][59] = 1;
TABLE[731][44] = 1;
TABLE[732][59] = 1;
TABLE[734][58] = 1;
TABLE[1174][42] = 1;
TABLE[1174][59] = 1;
TABLE[740][42] = 1;
TABLE[1177][59] = 1;
TABLE[735][40] = 1;
TABLE[735][256] = 1;
TABLE[735][453] = 1;
TABLE[735][341] = 1;
TABLE[735][259] = 1;
TABLE[1175][36] = 1;
TABLE[736][36] = 1;
TABLE[737][36] = 1;
TABLE[738][36] = 1;
TABLE[739][44] = 1;
TABLE[1176][42] = 1;
TABLE[1176][59] = 1;
TABLE[235][36] = 1;
TABLE[236][40] = 1;
TABLE[236][256] = 1;
TABLE[236][453] = 1;
TABLE[236][341] = 1;
TABLE[236][259] = 1;
TABLE[237][257] = 1;
TABLE[238][291] = 1;
TABLE[741][125] = 1;
TABLE[741][323] = 1;
TABLE[741][368] = 1;
TABLE[741][59] = 1;
TABLE[741][419] = 1;
TABLE[741][439] = 1;
TABLE[741][410] = 1;
TABLE[741][334] = 1;
TABLE[741][465] = 1;
TABLE[741][362] = 1;
TABLE[741][367] = 1;
TABLE[741][396] = 1;
TABLE[741][276] = 1;
TABLE[741][264] = 1;
TABLE[741][267] = 1;
TABLE[741][263] = 1;
TABLE[741][271] = 1;
TABLE[741][272] = 1;
TABLE[741][273] = 1;
TABLE[741][469] = 1;
TABLE[1178][125] = 1;
TABLE[239][323] = 1;
TABLE[240][368] = 1;
TABLE[241][59] = 1;
TABLE[241][276] = 1;
TABLE[242][419] = 1;
TABLE[243][439] = 1;
TABLE[244][410] = 1;
TABLE[245][334] = 1;
TABLE[246][465] = 1;
TABLE[247][362] = 1;
TABLE[248][367] = 1;
TABLE[249][396] = 1;
TABLE[250][469] = 1;
TABLE[250][273] = 1;
TABLE[250][272] = 1;
TABLE[250][271] = 1;
TABLE[250][263] = 1;
TABLE[250][267] = 1;
TABLE[250][264] = 1;
TABLE[742][58] = 1;
TABLE[1179][257] = 1;
TABLE[743][58] = 1;
TABLE[1180][36] = 1;
TABLE[746][276] = 1;
TABLE[1183][59] = 1;
TABLE[744][58] = 1;
TABLE[1181][43] = 1;
TABLE[1181][45] = 1;
TABLE[1181][259] = 1;
TABLE[1181][341] = 1;
TABLE[1181][453] = 1;
TABLE[1181][256] = 1;
TABLE[1181][40] = 1;
TABLE[745][44] = 1;
TABLE[1182][276] = 1;
TABLE[1182][59] = 1;
TABLE[747][58] = 1;
TABLE[1184][45] = 1;
TABLE[1184][43] = 1;
TABLE[1184][40] = 1;
TABLE[1184][256] = 1;
TABLE[1184][453] = 1;
TABLE[1184][341] = 1;
TABLE[1184][259] = 1;
TABLE[1184][59] = 1;
TABLE[749][43] = 1;
TABLE[749][45] = 1;
TABLE[749][259] = 1;
TABLE[749][341] = 1;
TABLE[749][453] = 1;
TABLE[749][256] = 1;
TABLE[749][40] = 1;
TABLE[1186][59] = 1;
TABLE[748][44] = 1;
TABLE[1185][45] = 1;
TABLE[1185][43] = 1;
TABLE[1185][40] = 1;
TABLE[1185][256] = 1;
TABLE[1185][453] = 1;
TABLE[1185][341] = 1;
TABLE[1185][259] = 1;
TABLE[1185][59] = 1;
TABLE[750][58] = 1;
TABLE[1187][276] = 1;
TABLE[1187][59] = 1;
TABLE[752][276] = 1;
TABLE[1189][59] = 1;
TABLE[751][44] = 1;
TABLE[1188][276] = 1;
TABLE[1188][59] = 1;
TABLE[753][58] = 1;
TABLE[1190][45] = 1;
TABLE[1190][43] = 1;
TABLE[1190][40] = 1;
TABLE[1190][256] = 1;
TABLE[1190][453] = 1;
TABLE[1190][341] = 1;
TABLE[1190][259] = 1;
TABLE[1190][59] = 1;
TABLE[755][43] = 1;
TABLE[755][45] = 1;
TABLE[755][259] = 1;
TABLE[755][341] = 1;
TABLE[755][453] = 1;
TABLE[755][256] = 1;
TABLE[755][40] = 1;
TABLE[1192][59] = 1;
TABLE[754][44] = 1;
TABLE[1191][45] = 1;
TABLE[1191][43] = 1;
TABLE[1191][40] = 1;
TABLE[1191][256] = 1;
TABLE[1191][453] = 1;
TABLE[1191][341] = 1;
TABLE[1191][259] = 1;
TABLE[1191][59] = 1;
TABLE[757][435] = 1;
TABLE[757][36] = 1;
TABLE[757][366] = 1;
TABLE[757][343] = 1;
TABLE[1193][59] = 1;
TABLE[756][44] = 1;
TABLE[759][435] = 1;
TABLE[759][36] = 1;
TABLE[759][366] = 1;
TABLE[759][343] = 1;
TABLE[1194][59] = 1;
TABLE[758][44] = 1;
TABLE[761][435] = 1;
TABLE[761][36] = 1;
TABLE[761][366] = 1;
TABLE[761][343] = 1;
TABLE[1195][59] = 1;
TABLE[760][44] = 1;
TABLE[763][435] = 1;
TABLE[763][36] = 1;
TABLE[763][366] = 1;
TABLE[763][343] = 1;
TABLE[1196][59] = 1;
TABLE[762][44] = 1;
TABLE[251][435] = 1;
TABLE[252][435] = 1;
TABLE[253][36] = 1;
TABLE[254][36] = 1;
TABLE[255][36] = 1;
TABLE[256][36] = 1;
TABLE[257][366] = 1;
TABLE[258][343] = 1;
TABLE[764][293] = 1;
TABLE[1197][44] = 1;
TABLE[765][47] = 1;
TABLE[766][403] = 1;
TABLE[767][47] = 1;
TABLE[768][403] = 1;
TABLE[259][325] = 1;
TABLE[769][432] = 1;
TABLE[769][355] = 1;
TABLE[769][323] = 1;
TABLE[769][396] = 1;
TABLE[769][429] = 1;
TABLE[769][373] = 1;
TABLE[769][308] = 1;
TABLE[769][264] = 1;
TABLE[769][267] = 1;
TABLE[769][263] = 1;
TABLE[769][271] = 1;
TABLE[769][272] = 1;
TABLE[769][273] = 1;
TABLE[769][469] = 1;
TABLE[1198][125] = 1;
TABLE[260][432] = 1;
TABLE[261][355] = 1;
TABLE[262][323] = 1;
TABLE[263][396] = 1;
TABLE[264][308] = 1;
TABLE[264][373] = 1;
TABLE[264][429] = 1;
TABLE[265][469] = 1;
TABLE[265][273] = 1;
TABLE[265][272] = 1;
TABLE[265][271] = 1;
TABLE[265][263] = 1;
TABLE[265][267] = 1;
TABLE[265][264] = 1;
TABLE[770][58] = 1;
TABLE[1199][36] = 1;
TABLE[1199][59] = 1;
TABLE[772][36] = 1;
TABLE[1201][59] = 1;
TABLE[771][44] = 1;
TABLE[1200][36] = 1;
TABLE[1200][59] = 1;
TABLE[773][58] = 1;
TABLE[1202][343] = 1;
TABLE[1202][435] = 1;
TABLE[1202][281] = 1;
TABLE[1202][59] = 1;
TABLE[775][281] = 1;
TABLE[775][435] = 1;
TABLE[775][343] = 1;
TABLE[1204][59] = 1;
TABLE[774][44] = 1;
TABLE[1203][343] = 1;
TABLE[1203][435] = 1;
TABLE[1203][281] = 1;
TABLE[1203][59] = 1;
TABLE[776][58] = 1;
TABLE[1205][257] = 1;
TABLE[266][354] = 1;
TABLE[777][326] = 1;
TABLE[777][308] = 1;
TABLE[777][346] = 1;
TABLE[777][323] = 1;
TABLE[777][355] = 1;
TABLE[777][396] = 1;
TABLE[777][429] = 1;
TABLE[777][373] = 1;
TABLE[777][264] = 1;
TABLE[777][267] = 1;
TABLE[777][263] = 1;
TABLE[777][271] = 1;
TABLE[777][272] = 1;
TABLE[777][273] = 1;
TABLE[777][469] = 1;
TABLE[1206][125] = 1;
TABLE[267][326] = 1;
TABLE[268][308] = 1;
TABLE[269][346] = 1;
TABLE[270][323] = 1;
TABLE[271][355] = 1;
TABLE[272][396] = 1;
TABLE[273][308] = 1;
TABLE[273][373] = 1;
TABLE[273][429] = 1;
TABLE[274][469] = 1;
TABLE[274][273] = 1;
TABLE[274][272] = 1;
TABLE[274][271] = 1;
TABLE[274][263] = 1;
TABLE[274][267] = 1;
TABLE[274][264] = 1;
TABLE[778][58] = 1;
TABLE[1207][36] = 1;
TABLE[1207][59] = 1;
TABLE[780][36] = 1;
TABLE[1209][59] = 1;
TABLE[779][44] = 1;
TABLE[1208][36] = 1;
TABLE[1208][59] = 1;
TABLE[781][58] = 1;
TABLE[1210][45] = 1;
TABLE[1210][43] = 1;
TABLE[1210][40] = 1;
TABLE[1210][256] = 1;
TABLE[1210][453] = 1;
TABLE[1210][341] = 1;
TABLE[1210][259] = 1;
TABLE[1210][59] = 1;
TABLE[784][43] = 1;
TABLE[784][45] = 1;
TABLE[784][259] = 1;
TABLE[784][341] = 1;
TABLE[784][453] = 1;
TABLE[784][256] = 1;
TABLE[784][40] = 1;
TABLE[1212][59] = 1;
TABLE[782][124] = 1;
TABLE[783][44] = 1;
TABLE[1211][45] = 1;
TABLE[1211][43] = 1;
TABLE[1211][40] = 1;
TABLE[1211][256] = 1;
TABLE[1211][453] = 1;
TABLE[1211][341] = 1;
TABLE[1211][259] = 1;
TABLE[1211][59] = 1;
TABLE[785][58] = 1;
TABLE[1213][257] = 1;
TABLE[786][58] = 1;
TABLE[1214][343] = 1;
TABLE[1214][435] = 1;
TABLE[1214][281] = 1;
TABLE[1214][59] = 1;
TABLE[788][281] = 1;
TABLE[788][435] = 1;
TABLE[788][343] = 1;
TABLE[1216][59] = 1;
TABLE[787][44] = 1;
TABLE[1215][343] = 1;
TABLE[1215][435] = 1;
TABLE[1215][281] = 1;
TABLE[1215][59] = 1;
TABLE[275][445] = 1;
TABLE[789][323] = 1;
TABLE[789][385] = 1;
TABLE[789][402] = 1;
TABLE[789][379] = 1;
TABLE[789][346] = 1;
TABLE[789][429] = 1;
TABLE[789][373] = 1;
TABLE[789][308] = 1;
TABLE[789][396] = 1;
TABLE[789][364] = 1;
TABLE[789][353] = 1;
TABLE[789][264] = 1;
TABLE[789][267] = 1;
TABLE[789][263] = 1;
TABLE[789][271] = 1;
TABLE[789][272] = 1;
TABLE[789][273] = 1;
TABLE[789][469] = 1;
TABLE[1217][125] = 1;
TABLE[276][323] = 1;
TABLE[277][353] = 1;
TABLE[277][364] = 1;
TABLE[278][385] = 1;
TABLE[279][402] = 1;
TABLE[280][379] = 1;
TABLE[281][346] = 1;
TABLE[282][308] = 1;
TABLE[282][373] = 1;
TABLE[282][429] = 1;
TABLE[283][396] = 1;
TABLE[284][469] = 1;
TABLE[284][273] = 1;
TABLE[284][272] = 1;
TABLE[284][271] = 1;
TABLE[284][263] = 1;
TABLE[284][267] = 1;
TABLE[284][264] = 1;
TABLE[790][58] = 1;
TABLE[1218][257] = 1;
TABLE[791][58] = 1;
TABLE[1219][43] = 1;
TABLE[1219][45] = 1;
TABLE[1219][259] = 1;
TABLE[1219][341] = 1;
TABLE[1219][453] = 1;
TABLE[1219][256] = 1;
TABLE[1219][40] = 1;
TABLE[792][58] = 1;
TABLE[1220][43] = 1;
TABLE[1220][45] = 1;
TABLE[1220][259] = 1;
TABLE[1220][341] = 1;
TABLE[1220][453] = 1;
TABLE[1220][256] = 1;
TABLE[1220][40] = 1;
TABLE[793][58] = 1;
TABLE[1221][43] = 1;
TABLE[1221][45] = 1;
TABLE[1221][259] = 1;
TABLE[1221][341] = 1;
TABLE[1221][453] = 1;
TABLE[1221][256] = 1;
TABLE[1221][40] = 1;
TABLE[794][58] = 1;
TABLE[1222][45] = 1;
TABLE[1222][43] = 1;
TABLE[1222][40] = 1;
TABLE[1222][256] = 1;
TABLE[1222][453] = 1;
TABLE[1222][341] = 1;
TABLE[1222][259] = 1;
TABLE[1222][59] = 1;
TABLE[797][43] = 1;
TABLE[797][45] = 1;
TABLE[797][259] = 1;
TABLE[797][341] = 1;
TABLE[797][453] = 1;
TABLE[797][256] = 1;
TABLE[797][40] = 1;
TABLE[1224][59] = 1;
TABLE[795][124] = 1;
TABLE[796][44] = 1;
TABLE[1223][45] = 1;
TABLE[1223][43] = 1;
TABLE[1223][40] = 1;
TABLE[1223][256] = 1;
TABLE[1223][453] = 1;
TABLE[1223][341] = 1;
TABLE[1223][259] = 1;
TABLE[1223][59] = 1;
TABLE[285][417] = 1;
TABLE[798][323] = 1;
TABLE[798][459] = 1;
TABLE[798][347] = 1;
TABLE[798][324] = 1;
TABLE[798][322] = 1;
TABLE[798][426] = 1;
TABLE[798][421] = 1;
TABLE[798][346] = 1;
TABLE[798][356] = 1;
TABLE[798][450] = 1;
TABLE[798][418] = 1;
TABLE[798][394] = 1;
TABLE[798][429] = 1;
TABLE[798][373] = 1;
TABLE[798][308] = 1;
TABLE[798][396] = 1;
TABLE[798][264] = 1;
TABLE[798][267] = 1;
TABLE[798][263] = 1;
TABLE[798][271] = 1;
TABLE[798][272] = 1;
TABLE[798][273] = 1;
TABLE[798][469] = 1;
TABLE[1225][125] = 1;
TABLE[286][323] = 1;
TABLE[287][459] = 1;
TABLE[288][347] = 1;
TABLE[289][324] = 1;
TABLE[290][322] = 1;
TABLE[291][426] = 1;
TABLE[292][421] = 1;
TABLE[293][346] = 1;
TABLE[294][356] = 1;
TABLE[295][450] = 1;
TABLE[296][418] = 1;
TABLE[297][394] = 1;
TABLE[298][308] = 1;
TABLE[298][373] = 1;
TABLE[298][429] = 1;
TABLE[299][396] = 1;
TABLE[300][469] = 1;
TABLE[300][273] = 1;
TABLE[300][272] = 1;
TABLE[300][271] = 1;
TABLE[300][263] = 1;
TABLE[300][267] = 1;
TABLE[300][264] = 1;
TABLE[799][58] = 1;
TABLE[1226][257] = 1;
TABLE[800][58] = 1;
TABLE[1227][36] = 1;
TABLE[801][58] = 1;
TABLE[1228][36] = 1;
TABLE[802][58] = 1;
TABLE[1229][36] = 1;
TABLE[803][58] = 1;
TABLE[1230][43] = 1;
TABLE[1230][45] = 1;
TABLE[1230][259] = 1;
TABLE[1230][341] = 1;
TABLE[1230][453] = 1;
TABLE[1230][256] = 1;
TABLE[1230][40] = 1;
TABLE[804][58] = 1;
TABLE[1231][43] = 1;
TABLE[1231][45] = 1;
TABLE[1231][259] = 1;
TABLE[1231][341] = 1;
TABLE[1231][453] = 1;
TABLE[1231][256] = 1;
TABLE[1231][40] = 1;
TABLE[805][58] = 1;
TABLE[1232][45] = 1;
TABLE[1232][43] = 1;
TABLE[1232][40] = 1;
TABLE[1232][256] = 1;
TABLE[1232][453] = 1;
TABLE[1232][341] = 1;
TABLE[1232][259] = 1;
TABLE[1232][59] = 1;
TABLE[808][43] = 1;
TABLE[808][45] = 1;
TABLE[808][259] = 1;
TABLE[808][341] = 1;
TABLE[808][453] = 1;
TABLE[808][256] = 1;
TABLE[808][40] = 1;
TABLE[1234][59] = 1;
TABLE[806][44] = 1;
TABLE[807][124] = 1;
TABLE[1233][45] = 1;
TABLE[1233][43] = 1;
TABLE[1233][40] = 1;
TABLE[1233][256] = 1;
TABLE[1233][453] = 1;
TABLE[1233][341] = 1;
TABLE[1233][259] = 1;
TABLE[1233][59] = 1;
TABLE[809][58] = 1;
TABLE[1235][45] = 1;
TABLE[1235][43] = 1;
TABLE[1235][40] = 1;
TABLE[1235][256] = 1;
TABLE[1235][453] = 1;
TABLE[1235][341] = 1;
TABLE[1235][259] = 1;
TABLE[1235][59] = 1;
TABLE[812][43] = 1;
TABLE[812][45] = 1;
TABLE[812][259] = 1;
TABLE[812][341] = 1;
TABLE[812][453] = 1;
TABLE[812][256] = 1;
TABLE[812][40] = 1;
TABLE[1237][59] = 1;
TABLE[810][124] = 1;
TABLE[811][44] = 1;
TABLE[1236][45] = 1;
TABLE[1236][43] = 1;
TABLE[1236][40] = 1;
TABLE[1236][256] = 1;
TABLE[1236][453] = 1;
TABLE[1236][341] = 1;
TABLE[1236][259] = 1;
TABLE[1236][59] = 1;
TABLE[825][259] = 1;
TABLE[1243][59] = 1;
TABLE[813][91] = 1;
TABLE[822][466] = 1;
TABLE[823][292] = 1;
TABLE[1241][44] = 1;
TABLE[1241][259] = 1;
TABLE[1241][59] = 1;
TABLE[814][36] = 1;
TABLE[816][414] = 1;
TABLE[818][414] = 1;
TABLE[815][43] = 1;
TABLE[815][45] = 1;
TABLE[815][259] = 1;
TABLE[815][341] = 1;
TABLE[815][453] = 1;
TABLE[815][256] = 1;
TABLE[815][40] = 1;
TABLE[1238][395] = 1;
TABLE[1238][91] = 1;
TABLE[1238][466] = 1;
TABLE[1238][292] = 1;
TABLE[1238][44] = 1;
TABLE[1238][259] = 1;
TABLE[1238][59] = 1;
TABLE[817][43] = 1;
TABLE[817][45] = 1;
TABLE[817][259] = 1;
TABLE[817][341] = 1;
TABLE[817][453] = 1;
TABLE[817][256] = 1;
TABLE[817][40] = 1;
TABLE[1239][395] = 1;
TABLE[1239][91] = 1;
TABLE[1239][466] = 1;
TABLE[1239][292] = 1;
TABLE[1239][44] = 1;
TABLE[1239][259] = 1;
TABLE[1239][59] = 1;
TABLE[819][395] = 1;
TABLE[820][395] = 1;
TABLE[821][395] = 1;
TABLE[1240][91] = 1;
TABLE[1240][466] = 1;
TABLE[1240][292] = 1;
TABLE[1240][44] = 1;
TABLE[1240][259] = 1;
TABLE[1240][59] = 1;
TABLE[824][44] = 1;
TABLE[1242][259] = 1;
TABLE[1242][59] = 1;
TABLE[826][323] = 1;
TABLE[826][459] = 1;
TABLE[826][347] = 1;
TABLE[826][324] = 1;
TABLE[826][322] = 1;
TABLE[826][426] = 1;
TABLE[826][421] = 1;
TABLE[826][346] = 1;
TABLE[826][356] = 1;
TABLE[826][450] = 1;
TABLE[826][418] = 1;
TABLE[826][394] = 1;
TABLE[826][429] = 1;
TABLE[826][373] = 1;
TABLE[826][308] = 1;
TABLE[826][396] = 1;
TABLE[826][264] = 1;
TABLE[826][267] = 1;
TABLE[826][263] = 1;
TABLE[826][271] = 1;
TABLE[826][272] = 1;
TABLE[826][273] = 1;
TABLE[826][469] = 1;
TABLE[1244][396] = 1;
TABLE[1244][308] = 1;
TABLE[1244][373] = 1;
TABLE[1244][429] = 1;
TABLE[1244][394] = 1;
TABLE[1244][418] = 1;
TABLE[1244][450] = 1;
TABLE[1244][356] = 1;
TABLE[1244][346] = 1;
TABLE[1244][421] = 1;
TABLE[1244][426] = 1;
TABLE[1244][322] = 1;
TABLE[1244][324] = 1;
TABLE[1244][347] = 1;
TABLE[1244][459] = 1;
TABLE[1244][323] = 1;
TABLE[1244][469] = 1;
TABLE[1244][273] = 1;
TABLE[1244][272] = 1;
TABLE[1244][271] = 1;
TABLE[1244][263] = 1;
TABLE[1244][267] = 1;
TABLE[1244][264] = 1;
TABLE[1244][125] = 1;
TABLE[827][261] = 1;
TABLE[1245][396] = 1;
TABLE[1245][308] = 1;
TABLE[1245][373] = 1;
TABLE[1245][429] = 1;
TABLE[1245][394] = 1;
TABLE[1245][418] = 1;
TABLE[1245][450] = 1;
TABLE[1245][356] = 1;
TABLE[1245][346] = 1;
TABLE[1245][421] = 1;
TABLE[1245][426] = 1;
TABLE[1245][322] = 1;
TABLE[1245][324] = 1;
TABLE[1245][347] = 1;
TABLE[1245][459] = 1;
TABLE[1245][323] = 1;
TABLE[1245][469] = 1;
TABLE[1245][273] = 1;
TABLE[1245][272] = 1;
TABLE[1245][271] = 1;
TABLE[1245][263] = 1;
TABLE[1245][267] = 1;
TABLE[1245][264] = 1;
TABLE[1245][125] = 1;
TABLE[301][444] = 1;
TABLE[828][349] = 1;
TABLE[1246][123] = 1;
TABLE[829][323] = 1;
TABLE[829][349] = 1;
TABLE[829][386] = 1;
TABLE[829][360] = 1;
TABLE[829][362] = 1;
TABLE[829][320] = 1;
TABLE[829][290] = 1;
TABLE[829][316] = 1;
TABLE[829][405] = 1;
TABLE[829][303] = 1;
TABLE[829][426] = 1;
TABLE[829][430] = 1;
TABLE[829][385] = 1;
TABLE[829][276] = 1;
TABLE[829][294] = 1;
TABLE[829][435] = 1;
TABLE[829][419] = 1;
TABLE[829][365] = 1;
TABLE[829][342] = 1;
TABLE[829][432] = 1;
TABLE[829][327] = 1;
TABLE[829][404] = 1;
TABLE[829][346] = 1;
TABLE[829][352] = 1;
TABLE[829][396] = 1;
TABLE[829][429] = 1;
TABLE[829][373] = 1;
TABLE[829][308] = 1;
TABLE[829][364] = 1;
TABLE[829][353] = 1;
TABLE[829][304] = 1;
TABLE[829][370] = 1;
TABLE[829][264] = 1;
TABLE[829][267] = 1;
TABLE[829][263] = 1;
TABLE[829][271] = 1;
TABLE[829][272] = 1;
TABLE[829][273] = 1;
TABLE[829][469] = 1;
TABLE[1247][125] = 1;
TABLE[302][323] = 1;
TABLE[303][349] = 1;
TABLE[304][386] = 1;
TABLE[305][353] = 1;
TABLE[305][364] = 1;
TABLE[306][370] = 1;
TABLE[306][304] = 1;
TABLE[307][360] = 1;
TABLE[308][362] = 1;
TABLE[309][320] = 1;
TABLE[310][290] = 1;
TABLE[311][316] = 1;
TABLE[312][405] = 1;
TABLE[313][303] = 1;
TABLE[314][426] = 1;
TABLE[315][430] = 1;
TABLE[316][385] = 1;
TABLE[317][276] = 1;
TABLE[318][294] = 1;
TABLE[319][435] = 1;
TABLE[320][435] = 1;
TABLE[321][435] = 1;
TABLE[322][435] = 1;
TABLE[323][419] = 1;
TABLE[324][365] = 1;
TABLE[325][342] = 1;
TABLE[326][432] = 1;
TABLE[327][327] = 1;
TABLE[328][404] = 1;
TABLE[329][346] = 1;
TABLE[330][352] = 1;
TABLE[331][396] = 1;
TABLE[332][308] = 1;
TABLE[332][373] = 1;
TABLE[332][429] = 1;
TABLE[333][469] = 1;
TABLE[333][273] = 1;
TABLE[333][272] = 1;
TABLE[333][271] = 1;
TABLE[333][263] = 1;
TABLE[333][267] = 1;
TABLE[333][264] = 1;
TABLE[830][58] = 1;
TABLE[1248][257] = 1;
TABLE[831][58] = 1;
TABLE[1249][43] = 1;
TABLE[1249][45] = 1;
TABLE[1249][259] = 1;
TABLE[1249][341] = 1;
TABLE[1249][453] = 1;
TABLE[1249][256] = 1;
TABLE[1249][40] = 1;
TABLE[832][58] = 1;
TABLE[1250][43] = 1;
TABLE[1250][45] = 1;
TABLE[1250][259] = 1;
TABLE[1250][341] = 1;
TABLE[1250][453] = 1;
TABLE[1250][256] = 1;
TABLE[1250][40] = 1;
TABLE[833][370] = 1;
TABLE[834][304] = 1;
TABLE[835][58] = 1;
TABLE[1251][43] = 1;
TABLE[1251][61] = 1;
TABLE[1251][45] = 1;
TABLE[1251][40] = 1;
TABLE[1251][256] = 1;
TABLE[1251][453] = 1;
TABLE[1251][341] = 1;
TABLE[1251][259] = 1;
TABLE[1251][59] = 1;
TABLE[836][58] = 1;
TABLE[1252][43] = 1;
TABLE[1252][61] = 1;
TABLE[1252][45] = 1;
TABLE[1252][40] = 1;
TABLE[1252][256] = 1;
TABLE[1252][453] = 1;
TABLE[1252][341] = 1;
TABLE[1252][259] = 1;
TABLE[1252][59] = 1;
TABLE[837][58] = 1;
TABLE[1253][43] = 1;
TABLE[1253][61] = 1;
TABLE[1253][45] = 1;
TABLE[1253][40] = 1;
TABLE[1253][256] = 1;
TABLE[1253][453] = 1;
TABLE[1253][341] = 1;
TABLE[1253][259] = 1;
TABLE[1253][59] = 1;
TABLE[838][58] = 1;
TABLE[1254][43] = 1;
TABLE[1254][61] = 1;
TABLE[1254][45] = 1;
TABLE[1254][40] = 1;
TABLE[1254][256] = 1;
TABLE[1254][453] = 1;
TABLE[1254][341] = 1;
TABLE[1254][259] = 1;
TABLE[1254][59] = 1;
TABLE[839][58] = 1;
TABLE[1255][43] = 1;
TABLE[1255][61] = 1;
TABLE[1255][45] = 1;
TABLE[1255][40] = 1;
TABLE[1255][256] = 1;
TABLE[1255][453] = 1;
TABLE[1255][341] = 1;
TABLE[1255][259] = 1;
TABLE[1255][59] = 1;
TABLE[840][58] = 1;
TABLE[1256][43] = 1;
TABLE[1256][45] = 1;
TABLE[1256][259] = 1;
TABLE[1256][341] = 1;
TABLE[1256][453] = 1;
TABLE[1256][256] = 1;
TABLE[1256][40] = 1;
TABLE[841][58] = 1;
TABLE[1257][43] = 1;
TABLE[1257][61] = 1;
TABLE[1257][45] = 1;
TABLE[1257][40] = 1;
TABLE[1257][256] = 1;
TABLE[1257][453] = 1;
TABLE[1257][341] = 1;
TABLE[1257][259] = 1;
TABLE[1257][59] = 1;
TABLE[842][58] = 1;
TABLE[1258][43] = 1;
TABLE[1258][61] = 1;
TABLE[1258][45] = 1;
TABLE[1258][40] = 1;
TABLE[1258][256] = 1;
TABLE[1258][453] = 1;
TABLE[1258][341] = 1;
TABLE[1258][259] = 1;
TABLE[1258][59] = 1;
TABLE[843][58] = 1;
TABLE[1259][43] = 1;
TABLE[1259][61] = 1;
TABLE[1259][45] = 1;
TABLE[1259][40] = 1;
TABLE[1259][256] = 1;
TABLE[1259][453] = 1;
TABLE[1259][341] = 1;
TABLE[1259][259] = 1;
TABLE[1259][59] = 1;
TABLE[844][58] = 1;
TABLE[1260][43] = 1;
TABLE[1260][61] = 1;
TABLE[1260][45] = 1;
TABLE[1260][40] = 1;
TABLE[1260][256] = 1;
TABLE[1260][453] = 1;
TABLE[1260][341] = 1;
TABLE[1260][259] = 1;
TABLE[1260][59] = 1;
TABLE[845][58] = 1;
TABLE[1261][43] = 1;
TABLE[1261][61] = 1;
TABLE[1261][45] = 1;
TABLE[1261][40] = 1;
TABLE[1261][256] = 1;
TABLE[1261][453] = 1;
TABLE[1261][341] = 1;
TABLE[1261][259] = 1;
TABLE[1261][59] = 1;
TABLE[846][58] = 1;
TABLE[1262][43] = 1;
TABLE[1262][61] = 1;
TABLE[1262][45] = 1;
TABLE[1262][40] = 1;
TABLE[1262][256] = 1;
TABLE[1262][453] = 1;
TABLE[1262][341] = 1;
TABLE[1262][259] = 1;
TABLE[1262][44] = 1;
TABLE[1262][59] = 1;
TABLE[847][44] = 1;
TABLE[848][59] = 1;
TABLE[849][58] = 1;
TABLE[1263][45] = 1;
TABLE[1263][43] = 1;
TABLE[1263][40] = 1;
TABLE[1263][256] = 1;
TABLE[1263][453] = 1;
TABLE[1263][341] = 1;
TABLE[1263][259] = 1;
TABLE[850][44] = 1;
TABLE[1264][59] = 1;
TABLE[851][58] = 1;
TABLE[1265][43] = 1;
TABLE[1265][45] = 1;
TABLE[1265][126] = 1;
TABLE[1265][33] = 1;
TABLE[1265][259] = 1;
TABLE[1265][341] = 1;
TABLE[1265][453] = 1;
TABLE[1265][256] = 1;
TABLE[1265][40] = 1;
TABLE[1265][59] = 1;
TABLE[857][45] = 1;
TABLE[857][43] = 1;
TABLE[857][40] = 1;
TABLE[857][256] = 1;
TABLE[857][453] = 1;
TABLE[857][341] = 1;
TABLE[857][259] = 1;
TABLE[857][33] = 1;
TABLE[857][126] = 1;
TABLE[1267][59] = 1;
TABLE[852][43] = 1;
TABLE[852][45] = 1;
TABLE[852][259] = 1;
TABLE[852][341] = 1;
TABLE[852][453] = 1;
TABLE[852][256] = 1;
TABLE[852][40] = 1;
TABLE[855][126] = 1;
TABLE[855][33] = 1;
TABLE[853][126] = 1;
TABLE[854][33] = 1;
TABLE[856][44] = 1;
TABLE[1266][43] = 1;
TABLE[1266][45] = 1;
TABLE[1266][126] = 1;
TABLE[1266][33] = 1;
TABLE[1266][259] = 1;
TABLE[1266][341] = 1;
TABLE[1266][453] = 1;
TABLE[1266][256] = 1;
TABLE[1266][40] = 1;
TABLE[1266][59] = 1;
TABLE[858][58] = 1;
TABLE[1268][43] = 1;
TABLE[1268][45] = 1;
TABLE[1268][126] = 1;
TABLE[1268][33] = 1;
TABLE[1268][259] = 1;
TABLE[1268][341] = 1;
TABLE[1268][453] = 1;
TABLE[1268][256] = 1;
TABLE[1268][40] = 1;
TABLE[1268][59] = 1;
TABLE[864][45] = 1;
TABLE[864][43] = 1;
TABLE[864][40] = 1;
TABLE[864][256] = 1;
TABLE[864][453] = 1;
TABLE[864][341] = 1;
TABLE[864][259] = 1;
TABLE[864][33] = 1;
TABLE[864][126] = 1;
TABLE[1270][59] = 1;
TABLE[859][43] = 1;
TABLE[859][45] = 1;
TABLE[859][259] = 1;
TABLE[859][341] = 1;
TABLE[859][453] = 1;
TABLE[859][256] = 1;
TABLE[859][40] = 1;
TABLE[862][126] = 1;
TABLE[862][33] = 1;
TABLE[860][126] = 1;
TABLE[861][33] = 1;
TABLE[863][44] = 1;
TABLE[1269][43] = 1;
TABLE[1269][45] = 1;
TABLE[1269][126] = 1;
TABLE[1269][33] = 1;
TABLE[1269][259] = 1;
TABLE[1269][341] = 1;
TABLE[1269][453] = 1;
TABLE[1269][256] = 1;
TABLE[1269][40] = 1;
TABLE[1269][59] = 1;
TABLE[865][58] = 1;
TABLE[1271][43] = 1;
TABLE[1271][45] = 1;
TABLE[1271][259] = 1;
TABLE[1271][341] = 1;
TABLE[1271][453] = 1;
TABLE[1271][256] = 1;
TABLE[1271][40] = 1;
TABLE[866][43] = 1;
TABLE[866][45] = 1;
TABLE[866][259] = 1;
TABLE[866][341] = 1;
TABLE[866][453] = 1;
TABLE[866][256] = 1;
TABLE[866][40] = 1;
TABLE[1272][43] = 1;
TABLE[1272][45] = 1;
TABLE[1272][259] = 1;
TABLE[1272][341] = 1;
TABLE[1272][453] = 1;
TABLE[1272][256] = 1;
TABLE[1272][40] = 1;
TABLE[867][58] = 1;
TABLE[1273][36] = 1;
TABLE[1273][59] = 1;
TABLE[869][36] = 1;
TABLE[1275][59] = 1;
TABLE[868][44] = 1;
TABLE[1274][36] = 1;
TABLE[1274][59] = 1;
TABLE[870][58] = 1;
TABLE[1276][36] = 1;
TABLE[1276][59] = 1;
TABLE[872][36] = 1;
TABLE[1278][59] = 1;
TABLE[871][44] = 1;
TABLE[1277][36] = 1;
TABLE[1277][59] = 1;
TABLE[873][58] = 1;
TABLE[1279][36] = 1;
TABLE[1279][59] = 1;
TABLE[875][36] = 1;
TABLE[1281][59] = 1;
TABLE[874][44] = 1;
TABLE[1280][36] = 1;
TABLE[1280][59] = 1;
TABLE[876][58] = 1;
TABLE[1282][43] = 1;
TABLE[1282][45] = 1;
TABLE[1282][126] = 1;
TABLE[1282][33] = 1;
TABLE[1282][259] = 1;
TABLE[1282][341] = 1;
TABLE[1282][453] = 1;
TABLE[1282][256] = 1;
TABLE[1282][40] = 1;
TABLE[1282][308] = 1;
TABLE[1282][373] = 1;
TABLE[1282][429] = 1;
TABLE[1282][396] = 1;
TABLE[1282][352] = 1;
TABLE[1282][346] = 1;
TABLE[1282][404] = 1;
TABLE[1282][327] = 1;
TABLE[1282][432] = 1;
TABLE[1282][342] = 1;
TABLE[1282][365] = 1;
TABLE[1282][419] = 1;
TABLE[1282][435] = 1;
TABLE[1282][294] = 1;
TABLE[1282][276] = 1;
TABLE[1282][385] = 1;
TABLE[1282][430] = 1;
TABLE[1282][426] = 1;
TABLE[1282][303] = 1;
TABLE[1282][405] = 1;
TABLE[1282][316] = 1;
TABLE[1282][290] = 1;
TABLE[1282][320] = 1;
TABLE[1282][362] = 1;
TABLE[1282][360] = 1;
TABLE[1282][386] = 1;
TABLE[1282][349] = 1;
TABLE[1282][323] = 1;
TABLE[1282][469] = 1;
TABLE[1282][273] = 1;
TABLE[1282][272] = 1;
TABLE[1282][271] = 1;
TABLE[1282][263] = 1;
TABLE[1282][267] = 1;
TABLE[1282][264] = 1;
TABLE[1282][370] = 1;
TABLE[1282][304] = 1;
TABLE[1282][353] = 1;
TABLE[1282][364] = 1;
TABLE[1282][125] = 1;
TABLE[883][45] = 1;
TABLE[883][43] = 1;
TABLE[883][40] = 1;
TABLE[883][256] = 1;
TABLE[883][453] = 1;
TABLE[883][341] = 1;
TABLE[883][259] = 1;
TABLE[883][33] = 1;
TABLE[883][126] = 1;
TABLE[1283][308] = 1;
TABLE[1283][373] = 1;
TABLE[1283][429] = 1;
TABLE[1283][396] = 1;
TABLE[1283][352] = 1;
TABLE[1283][346] = 1;
TABLE[1283][404] = 1;
TABLE[1283][327] = 1;
TABLE[1283][432] = 1;
TABLE[1283][342] = 1;
TABLE[1283][365] = 1;
TABLE[1283][419] = 1;
TABLE[1283][435] = 1;
TABLE[1283][294] = 1;
TABLE[1283][276] = 1;
TABLE[1283][385] = 1;
TABLE[1283][430] = 1;
TABLE[1283][426] = 1;
TABLE[1283][303] = 1;
TABLE[1283][405] = 1;
TABLE[1283][316] = 1;
TABLE[1283][290] = 1;
TABLE[1283][320] = 1;
TABLE[1283][362] = 1;
TABLE[1283][360] = 1;
TABLE[1283][386] = 1;
TABLE[1283][349] = 1;
TABLE[1283][323] = 1;
TABLE[1283][469] = 1;
TABLE[1283][273] = 1;
TABLE[1283][272] = 1;
TABLE[1283][271] = 1;
TABLE[1283][263] = 1;
TABLE[1283][267] = 1;
TABLE[1283][264] = 1;
TABLE[1283][370] = 1;
TABLE[1283][304] = 1;
TABLE[1283][353] = 1;
TABLE[1283][364] = 1;
TABLE[1283][125] = 1;
TABLE[877][43] = 1;
TABLE[877][45] = 1;
TABLE[877][259] = 1;
TABLE[877][341] = 1;
TABLE[877][453] = 1;
TABLE[877][256] = 1;
TABLE[877][40] = 1;
TABLE[880][126] = 1;
TABLE[880][33] = 1;
TABLE[878][126] = 1;
TABLE[879][33] = 1;
TABLE[881][44] = 1;
TABLE[882][59] = 1;
TABLE[334][43] = 1;
TABLE[334][45] = 1;
TABLE[334][259] = 1;
TABLE[334][341] = 1;
TABLE[334][453] = 1;
TABLE[334][256] = 1;
TABLE[334][40] = 1;
TABLE[335][43] = 1;
TABLE[335][45] = 1;
TABLE[335][259] = 1;
TABLE[335][341] = 1;
TABLE[335][453] = 1;
TABLE[335][256] = 1;
TABLE[335][40] = 1;
TABLE[884][259] = 1;
TABLE[884][341] = 1;
TABLE[884][453] = 1;
TABLE[884][256] = 1;
TABLE[884][40] = 1;
TABLE[885][43] = 1;
TABLE[885][45] = 1;
TABLE[885][259] = 1;
TABLE[885][341] = 1;
TABLE[885][453] = 1;
TABLE[885][256] = 1;
TABLE[885][40] = 1;
TABLE[1284][45] = 1;
TABLE[1284][43] = 1;
TABLE[1284][256] = 1;
TABLE[1284][453] = 1;
TABLE[1284][341] = 1;
TABLE[1284][259] = 1;
TABLE[1284][316] = 1;
TABLE[1284][40] = 1;
TABLE[1284][287] = 1;
TABLE[1284][44] = 1;
TABLE[1284][59] = 1;
TABLE[886][43] = 1;
TABLE[886][45] = 1;
TABLE[886][259] = 1;
TABLE[886][341] = 1;
TABLE[886][453] = 1;
TABLE[886][256] = 1;
TABLE[886][40] = 1;
TABLE[1285][316] = 1;
TABLE[1285][40] = 1;
TABLE[1285][287] = 1;
TABLE[1285][44] = 1;
TABLE[1285][59] = 1;
TABLE[887][316] = 1;
TABLE[1286][40] = 1;
TABLE[1286][287] = 1;
TABLE[1286][44] = 1;
TABLE[1286][59] = 1;
TABLE[891][40] = 1;
TABLE[1287][287] = 1;
TABLE[1287][44] = 1;
TABLE[1287][59] = 1;
TABLE[888][319] = 1;
TABLE[889][304] = 1;
TABLE[890][370] = 1;
TABLE[899][287] = 1;
TABLE[1292][44] = 1;
TABLE[1292][59] = 1;
TABLE[892][259] = 1;
TABLE[892][341] = 1;
TABLE[892][453] = 1;
TABLE[892][256] = 1;
TABLE[892][40] = 1;
TABLE[1288][45] = 1;
TABLE[1288][43] = 1;
TABLE[1288][256] = 1;
TABLE[1288][453] = 1;
TABLE[1288][341] = 1;
TABLE[1288][259] = 1;
TABLE[1288][316] = 1;
TABLE[1288][40] = 1;
TABLE[1288][287] = 1;
TABLE[1288][44] = 1;
TABLE[1288][59] = 1;
TABLE[893][43] = 1;
TABLE[893][45] = 1;
TABLE[893][259] = 1;
TABLE[893][341] = 1;
TABLE[893][453] = 1;
TABLE[893][256] = 1;
TABLE[893][40] = 1;
TABLE[1289][316] = 1;
TABLE[1289][40] = 1;
TABLE[1289][287] = 1;
TABLE[1289][44] = 1;
TABLE[1289][59] = 1;
TABLE[894][316] = 1;
TABLE[1290][40] = 1;
TABLE[1290][287] = 1;
TABLE[1290][44] = 1;
TABLE[1290][59] = 1;
TABLE[898][40] = 1;
TABLE[1291][287] = 1;
TABLE[1291][44] = 1;
TABLE[1291][59] = 1;
TABLE[895][319] = 1;
TABLE[896][304] = 1;
TABLE[897][370] = 1;
TABLE[900][43] = 1;
TABLE[900][45] = 1;
TABLE[900][259] = 1;
TABLE[900][341] = 1;
TABLE[900][453] = 1;
TABLE[900][256] = 1;
TABLE[900][40] = 1;
TABLE[1293][44] = 1;
TABLE[1293][59] = 1;
TABLE[336][261] = 1;
TABLE[337][337] = 1;
TABLE[901][323] = 1;
TABLE[901][379] = 1;
TABLE[901][307] = 1;
TABLE[901][461] = 1;
TABLE[1294][125] = 1;
TABLE[338][323] = 1;
TABLE[339][379] = 1;
TABLE[340][307] = 1;
TABLE[341][461] = 1;
TABLE[902][58] = 1;
TABLE[1295][257] = 1;
TABLE[903][58] = 1;
TABLE[1296][43] = 1;
TABLE[1296][45] = 1;
TABLE[1296][259] = 1;
TABLE[1296][341] = 1;
TABLE[1296][453] = 1;
TABLE[1296][256] = 1;
TABLE[1296][40] = 1;
TABLE[904][58] = 1;
TABLE[1297][274] = 1;
TABLE[1297][300] = 1;
TABLE[1297][372] = 1;
TABLE[1297][317] = 1;
TABLE[905][58] = 1;
TABLE[1298][43] = 1;
TABLE[1298][45] = 1;
TABLE[1298][259] = 1;
TABLE[1298][341] = 1;
TABLE[1298][453] = 1;
TABLE[1298][256] = 1;
TABLE[1298][40] = 1;
TABLE[342][261] = 1;
TABLE[343][264] = 1;
TABLE[344][267] = 1;
TABLE[345][263] = 1;
TABLE[346][271] = 1;
TABLE[347][272] = 1;
TABLE[348][273] = 1;
TABLE[349][469] = 1;
TABLE[350][442] = 1;
TABLE[913][264] = 1;
TABLE[913][267] = 1;
TABLE[913][263] = 1;
TABLE[913][271] = 1;
TABLE[913][272] = 1;
TABLE[913][273] = 1;
TABLE[913][469] = 1;
TABLE[1306][44] = 1;
TABLE[1306][41] = 1;
TABLE[906][58] = 1;
TABLE[1299][258] = 1;
TABLE[1299][61] = 1;
TABLE[1299][44] = 1;
TABLE[1299][41] = 1;
TABLE[907][258] = 1;
TABLE[1300][61] = 1;
TABLE[1300][44] = 1;
TABLE[1300][41] = 1;
TABLE[908][61] = 1;
TABLE[1301][44] = 1;
TABLE[1301][41] = 1;
TABLE[912][44] = 1;
TABLE[1305][44] = 1;
TABLE[1305][41] = 1;
TABLE[909][58] = 1;
TABLE[1302][258] = 1;
TABLE[1302][61] = 1;
TABLE[1302][44] = 1;
TABLE[1302][41] = 1;
TABLE[910][258] = 1;
TABLE[1303][61] = 1;
TABLE[1303][44] = 1;
TABLE[1303][41] = 1;
TABLE[911][61] = 1;
TABLE[1304][44] = 1;
TABLE[1304][41] = 1;
TABLE[914][44] = 1;
TABLE[1307][41] = 1;
TABLE[351][442] = 1;
TABLE[922][264] = 1;
TABLE[922][267] = 1;
TABLE[922][263] = 1;
TABLE[922][271] = 1;
TABLE[922][272] = 1;
TABLE[922][273] = 1;
TABLE[922][469] = 1;
TABLE[1315][44] = 1;
TABLE[1315][41] = 1;
TABLE[915][58] = 1;
TABLE[1308][258] = 1;
TABLE[1308][61] = 1;
TABLE[1308][44] = 1;
TABLE[1308][41] = 1;
TABLE[916][258] = 1;
TABLE[1309][61] = 1;
TABLE[1309][44] = 1;
TABLE[1309][41] = 1;
TABLE[917][61] = 1;
TABLE[1310][44] = 1;
TABLE[1310][41] = 1;
TABLE[921][44] = 1;
TABLE[1314][44] = 1;
TABLE[1314][41] = 1;
TABLE[918][58] = 1;
TABLE[1311][258] = 1;
TABLE[1311][61] = 1;
TABLE[1311][44] = 1;
TABLE[1311][41] = 1;
TABLE[919][258] = 1;
TABLE[1312][61] = 1;
TABLE[1312][44] = 1;
TABLE[1312][41] = 1;
TABLE[920][61] = 1;
TABLE[1313][44] = 1;
TABLE[1313][41] = 1;
TABLE[923][44] = 1;
TABLE[1316][41] = 1;
TABLE[352][442] = 1;
TABLE[353][442] = 1;
TABLE[354][264] = 1;
TABLE[354][267] = 1;
TABLE[354][263] = 1;
TABLE[354][271] = 1;
TABLE[354][272] = 1;
TABLE[354][273] = 1;
TABLE[354][469] = 1;
TABLE[924][44] = 1;
TABLE[1317][59] = 1;
TABLE[355][264] = 1;
TABLE[355][267] = 1;
TABLE[355][263] = 1;
TABLE[355][271] = 1;
TABLE[355][272] = 1;
TABLE[355][273] = 1;
TABLE[355][469] = 1;
TABLE[925][44] = 1;
TABLE[1318][59] = 1;
TABLE[356][264] = 1;
TABLE[356][267] = 1;
TABLE[356][263] = 1;
TABLE[356][271] = 1;
TABLE[356][272] = 1;
TABLE[356][273] = 1;
TABLE[356][469] = 1;
TABLE[926][44] = 1;
TABLE[1319][59] = 1;
TABLE[357][470] = 1;
TABLE[357][471] = 1;
TABLE[357][43] = 1;
TABLE[357][45] = 1;
TABLE[357][33] = 1;
TABLE[357][126] = 1;
TABLE[357][58] = 1;
TABLE[357][380] = 1;
TABLE[357][383] = 1;
TABLE[357][258] = 1;
TABLE[357][40] = 1;
TABLE[357][36] = 1;
TABLE[357][391] = 1;
TABLE[357][341] = 1;
TABLE[357][453] = 1;
TABLE[357][257] = 1;
TABLE[357][259] = 1;
TABLE[357][256] = 1;
TABLE[358][363] = 1;
TABLE[359][363] = 1;
TABLE[360][349] = 1;
TABLE[361][441] = 1;
TABLE[362][299] = 1;
TABLE[363][311] = 1;
TABLE[364][328] = 1;
TABLE[365][464] = 1;
TABLE[366][123] = 1;
TABLE[367][420] = 1;
TABLE[368][420] = 1;
TABLE[930][302] = 1;
TABLE[1321][321] = 1;
TABLE[1321][125] = 1;
TABLE[927][43] = 1;
TABLE[927][45] = 1;
TABLE[927][259] = 1;
TABLE[927][341] = 1;
TABLE[927][453] = 1;
TABLE[927][256] = 1;
TABLE[927][40] = 1;
TABLE[928][36] = 1;
TABLE[929][363] = 1;
TABLE[929][349] = 1;
TABLE[929][441] = 1;
TABLE[929][299] = 1;
TABLE[929][311] = 1;
TABLE[929][328] = 1;
TABLE[929][464] = 1;
TABLE[929][420] = 1;
TABLE[929][256] = 1;
TABLE[929][259] = 1;
TABLE[929][257] = 1;
TABLE[929][453] = 1;
TABLE[929][341] = 1;
TABLE[929][391] = 1;
TABLE[929][36] = 1;
TABLE[929][40] = 1;
TABLE[929][258] = 1;
TABLE[929][383] = 1;
TABLE[929][380] = 1;
TABLE[929][58] = 1;
TABLE[929][126] = 1;
TABLE[929][33] = 1;
TABLE[929][45] = 1;
TABLE[929][43] = 1;
TABLE[929][471] = 1;
TABLE[929][470] = 1;
TABLE[929][123] = 1;
TABLE[1320][302] = 1;
TABLE[1320][321] = 1;
TABLE[1320][125] = 1;
TABLE[932][321] = 1;
TABLE[1323][125] = 1;
TABLE[931][363] = 1;
TABLE[931][349] = 1;
TABLE[931][441] = 1;
TABLE[931][299] = 1;
TABLE[931][311] = 1;
TABLE[931][328] = 1;
TABLE[931][464] = 1;
TABLE[931][420] = 1;
TABLE[931][256] = 1;
TABLE[931][259] = 1;
TABLE[931][257] = 1;
TABLE[931][453] = 1;
TABLE[931][341] = 1;
TABLE[931][391] = 1;
TABLE[931][36] = 1;
TABLE[931][40] = 1;
TABLE[931][258] = 1;
TABLE[931][383] = 1;
TABLE[931][380] = 1;
TABLE[931][58] = 1;
TABLE[931][126] = 1;
TABLE[931][33] = 1;
TABLE[931][45] = 1;
TABLE[931][43] = 1;
TABLE[931][471] = 1;
TABLE[931][470] = 1;
TABLE[931][123] = 1;
TABLE[1322][125] = 1;
TABLE[369][123] = 1;
TABLE[933][363] = 1;
TABLE[933][349] = 1;
TABLE[933][441] = 1;
TABLE[933][299] = 1;
TABLE[933][311] = 1;
TABLE[933][328] = 1;
TABLE[933][464] = 1;
TABLE[933][420] = 1;
TABLE[933][256] = 1;
TABLE[933][259] = 1;
TABLE[933][257] = 1;
TABLE[933][453] = 1;
TABLE[933][341] = 1;
TABLE[933][391] = 1;
TABLE[933][36] = 1;
TABLE[933][40] = 1;
TABLE[933][258] = 1;
TABLE[933][383] = 1;
TABLE[933][380] = 1;
TABLE[933][58] = 1;
TABLE[933][126] = 1;
TABLE[933][33] = 1;
TABLE[933][45] = 1;
TABLE[933][43] = 1;
TABLE[933][471] = 1;
TABLE[933][470] = 1;
TABLE[933][123] = 1;
TABLE[1324][125] = 1;
TABLE[370][123] = 1;
TABLE[934][469] = 1;
TABLE[934][273] = 1;
TABLE[934][272] = 1;
TABLE[934][271] = 1;
TABLE[934][263] = 1;
TABLE[934][267] = 1;
TABLE[934][264] = 1;
TABLE[935][436] = 1;
TABLE[1325][420] = 1;
TABLE[1325][464] = 1;
TABLE[1325][328] = 1;
TABLE[1325][311] = 1;
TABLE[1325][299] = 1;
TABLE[1325][441] = 1;
TABLE[1325][349] = 1;
TABLE[1325][363] = 1;
TABLE[1325][123] = 1;
TABLE[1325][470] = 1;
TABLE[1325][471] = 1;
TABLE[1325][43] = 1;
TABLE[1325][45] = 1;
TABLE[1325][33] = 1;
TABLE[1325][126] = 1;
TABLE[1325][58] = 1;
TABLE[1325][380] = 1;
TABLE[1325][383] = 1;
TABLE[1325][258] = 1;
TABLE[1325][40] = 1;
TABLE[1325][36] = 1;
TABLE[1325][391] = 1;
TABLE[1325][341] = 1;
TABLE[1325][453] = 1;
TABLE[1325][257] = 1;
TABLE[1325][259] = 1;
TABLE[1325][256] = 1;
TABLE[1325][125] = 1;
TABLE[936][363] = 1;
TABLE[936][349] = 1;
TABLE[936][441] = 1;
TABLE[936][299] = 1;
TABLE[936][311] = 1;
TABLE[936][328] = 1;
TABLE[936][464] = 1;
TABLE[936][420] = 1;
TABLE[936][256] = 1;
TABLE[936][259] = 1;
TABLE[936][257] = 1;
TABLE[936][453] = 1;
TABLE[936][341] = 1;
TABLE[936][391] = 1;
TABLE[936][36] = 1;
TABLE[936][40] = 1;
TABLE[936][258] = 1;
TABLE[936][383] = 1;
TABLE[936][380] = 1;
TABLE[936][58] = 1;
TABLE[936][126] = 1;
TABLE[936][33] = 1;
TABLE[936][45] = 1;
TABLE[936][43] = 1;
TABLE[936][471] = 1;
TABLE[936][470] = 1;
TABLE[936][123] = 1;
TABLE[1326][125] = 1;
TABLE[371][256] = 1;
TABLE[372][259] = 1;
TABLE[373][257] = 1;
TABLE[374][453] = 1;
TABLE[375][341] = 1;
TABLE[376][391] = 1;
TABLE[377][36] = 1;
TABLE[378][40] = 1;
TABLE[379][258] = 1;
TABLE[380][256] = 1;
TABLE[380][259] = 1;
TABLE[380][257] = 1;
TABLE[380][453] = 1;
TABLE[380][341] = 1;
TABLE[380][391] = 1;
TABLE[380][36] = 1;
TABLE[380][40] = 1;
TABLE[380][258] = 1;
TABLE[381][256] = 1;
TABLE[381][259] = 1;
TABLE[381][257] = 1;
TABLE[381][453] = 1;
TABLE[381][341] = 1;
TABLE[381][391] = 1;
TABLE[381][36] = 1;
TABLE[381][40] = 1;
TABLE[381][258] = 1;
TABLE[382][258] = 1;
TABLE[383][256] = 1;
TABLE[383][259] = 1;
TABLE[383][257] = 1;
TABLE[383][453] = 1;
TABLE[383][341] = 1;
TABLE[383][391] = 1;
TABLE[383][36] = 1;
TABLE[383][40] = 1;
TABLE[383][258] = 1;
TABLE[937][46] = 1;
TABLE[938][491] = 1;
TABLE[939][46] = 1;
TABLE[940][491] = 1;
TABLE[384][258] = 1;
TABLE[385][258] = 1;
TABLE[385][40] = 1;
TABLE[385][36] = 1;
TABLE[385][391] = 1;
TABLE[385][341] = 1;
TABLE[385][453] = 1;
TABLE[385][257] = 1;
TABLE[385][259] = 1;
TABLE[385][256] = 1;
TABLE[386][256] = 1;
TABLE[386][259] = 1;
TABLE[386][257] = 1;
TABLE[386][453] = 1;
TABLE[386][341] = 1;
TABLE[386][391] = 1;
TABLE[386][36] = 1;
TABLE[386][40] = 1;
TABLE[386][258] = 1;
TABLE[941][46] = 1;
TABLE[942][491] = 1;
TABLE[387][258] = 1;
TABLE[387][40] = 1;
TABLE[387][36] = 1;
TABLE[387][391] = 1;
TABLE[387][341] = 1;
TABLE[387][453] = 1;
TABLE[387][257] = 1;
TABLE[387][259] = 1;
TABLE[387][256] = 1;
TABLE[388][258] = 1;
TABLE[388][40] = 1;
TABLE[388][36] = 1;
TABLE[388][391] = 1;
TABLE[388][341] = 1;
TABLE[388][453] = 1;
TABLE[388][257] = 1;
TABLE[388][259] = 1;
TABLE[388][256] = 1;
TABLE[389][383] = 1;
TABLE[390][380] = 1;
TABLE[391][256] = 1;
TABLE[391][259] = 1;
TABLE[391][257] = 1;
TABLE[391][453] = 1;
TABLE[391][341] = 1;
TABLE[391][391] = 1;
TABLE[391][36] = 1;
TABLE[391][40] = 1;
TABLE[391][258] = 1;
TABLE[392][470] = 1;
TABLE[393][471] = 1;
TABLE[394][43] = 1;
TABLE[395][45] = 1;
TABLE[396][33] = 1;
TABLE[397][126] = 1;
TABLE[398][58] = 1;
TABLE[399][256] = 1;
TABLE[399][259] = 1;
TABLE[399][257] = 1;
TABLE[399][453] = 1;
TABLE[399][341] = 1;
TABLE[399][391] = 1;
TABLE[399][36] = 1;
TABLE[399][40] = 1;
TABLE[399][258] = 1;
TABLE[399][383] = 1;
TABLE[399][380] = 1;
TABLE[400][470] = 1;
TABLE[400][471] = 1;
TABLE[400][43] = 1;
TABLE[400][45] = 1;
TABLE[400][33] = 1;
TABLE[400][126] = 1;
TABLE[400][58] = 1;
TABLE[400][380] = 1;
TABLE[400][383] = 1;
TABLE[400][258] = 1;
TABLE[400][40] = 1;
TABLE[400][36] = 1;
TABLE[400][391] = 1;
TABLE[400][341] = 1;
TABLE[400][453] = 1;
TABLE[400][257] = 1;
TABLE[400][259] = 1;
TABLE[400][256] = 1;
TABLE[401][470] = 1;
TABLE[401][471] = 1;
TABLE[401][43] = 1;
TABLE[401][45] = 1;
TABLE[401][33] = 1;
TABLE[401][126] = 1;
TABLE[401][58] = 1;
TABLE[401][380] = 1;
TABLE[401][383] = 1;
TABLE[401][258] = 1;
TABLE[401][40] = 1;
TABLE[401][36] = 1;
TABLE[401][391] = 1;
TABLE[401][341] = 1;
TABLE[401][453] = 1;
TABLE[401][257] = 1;
TABLE[401][259] = 1;
TABLE[401][256] = 1;
TABLE[402][40] = 1;
TABLE[403][256] = 1;
TABLE[403][259] = 1;
TABLE[403][257] = 1;
TABLE[403][453] = 1;
TABLE[403][341] = 1;
TABLE[403][391] = 1;
TABLE[403][36] = 1;
TABLE[403][40] = 1;
TABLE[403][258] = 1;
TABLE[403][383] = 1;
TABLE[403][380] = 1;
TABLE[403][58] = 1;
TABLE[403][126] = 1;
TABLE[403][33] = 1;
TABLE[403][45] = 1;
TABLE[403][43] = 1;
TABLE[403][471] = 1;
TABLE[403][470] = 1;
TABLE[404][256] = 1;
TABLE[404][259] = 1;
TABLE[404][257] = 1;
TABLE[404][453] = 1;
TABLE[404][341] = 1;
TABLE[404][391] = 1;
TABLE[404][36] = 1;
TABLE[404][258] = 1;
TABLE[404][383] = 1;
TABLE[404][380] = 1;
TABLE[404][58] = 1;
TABLE[404][126] = 1;
TABLE[404][33] = 1;
TABLE[404][45] = 1;
TABLE[404][43] = 1;
TABLE[404][471] = 1;
TABLE[404][470] = 1;
TABLE[404][40] = 1;
TABLE[405][256] = 1;
TABLE[405][259] = 1;
TABLE[405][257] = 1;
TABLE[405][453] = 1;
TABLE[405][341] = 1;
TABLE[405][391] = 1;
TABLE[405][36] = 1;
TABLE[405][258] = 1;
TABLE[405][383] = 1;
TABLE[405][380] = 1;
TABLE[405][58] = 1;
TABLE[405][126] = 1;
TABLE[405][33] = 1;
TABLE[405][45] = 1;
TABLE[405][43] = 1;
TABLE[405][471] = 1;
TABLE[405][470] = 1;
TABLE[405][40] = 1;
TABLE[406][256] = 1;
TABLE[406][259] = 1;
TABLE[406][257] = 1;
TABLE[406][453] = 1;
TABLE[406][341] = 1;
TABLE[406][391] = 1;
TABLE[406][36] = 1;
TABLE[406][258] = 1;
TABLE[406][383] = 1;
TABLE[406][380] = 1;
TABLE[406][58] = 1;
TABLE[406][126] = 1;
TABLE[406][33] = 1;
TABLE[406][45] = 1;
TABLE[406][43] = 1;
TABLE[406][471] = 1;
TABLE[406][470] = 1;
TABLE[406][40] = 1;
TABLE[407][40] = 1;
TABLE[407][470] = 1;
TABLE[407][471] = 1;
TABLE[407][43] = 1;
TABLE[407][45] = 1;
TABLE[407][33] = 1;
TABLE[407][126] = 1;
TABLE[407][58] = 1;
TABLE[407][380] = 1;
TABLE[407][383] = 1;
TABLE[407][258] = 1;
TABLE[407][36] = 1;
TABLE[407][391] = 1;
TABLE[407][341] = 1;
TABLE[407][453] = 1;
TABLE[407][257] = 1;
TABLE[407][259] = 1;
TABLE[407][256] = 1;
TABLE[408][40] = 1;
TABLE[408][470] = 1;
TABLE[408][471] = 1;
TABLE[408][43] = 1;
TABLE[408][45] = 1;
TABLE[408][33] = 1;
TABLE[408][126] = 1;
TABLE[408][58] = 1;
TABLE[408][380] = 1;
TABLE[408][383] = 1;
TABLE[408][258] = 1;
TABLE[408][36] = 1;
TABLE[408][391] = 1;
TABLE[408][341] = 1;
TABLE[408][453] = 1;
TABLE[408][257] = 1;
TABLE[408][259] = 1;
TABLE[408][256] = 1;
TABLE[409][40] = 1;
TABLE[409][470] = 1;
TABLE[409][471] = 1;
TABLE[409][43] = 1;
TABLE[409][45] = 1;
TABLE[409][33] = 1;
TABLE[409][126] = 1;
TABLE[409][58] = 1;
TABLE[409][380] = 1;
TABLE[409][383] = 1;
TABLE[409][258] = 1;
TABLE[409][36] = 1;
TABLE[409][391] = 1;
TABLE[409][341] = 1;
TABLE[409][453] = 1;
TABLE[409][257] = 1;
TABLE[409][259] = 1;
TABLE[409][256] = 1;
TABLE[410][256] = 1;
TABLE[410][259] = 1;
TABLE[410][257] = 1;
TABLE[410][453] = 1;
TABLE[410][341] = 1;
TABLE[410][391] = 1;
TABLE[410][36] = 1;
TABLE[410][258] = 1;
TABLE[410][383] = 1;
TABLE[410][380] = 1;
TABLE[410][58] = 1;
TABLE[410][126] = 1;
TABLE[410][33] = 1;
TABLE[410][45] = 1;
TABLE[410][43] = 1;
TABLE[410][471] = 1;
TABLE[410][470] = 1;
TABLE[410][40] = 1;
TABLE[411][256] = 1;
TABLE[411][259] = 1;
TABLE[411][257] = 1;
TABLE[411][453] = 1;
TABLE[411][341] = 1;
TABLE[411][391] = 1;
TABLE[411][36] = 1;
TABLE[411][258] = 1;
TABLE[411][383] = 1;
TABLE[411][380] = 1;
TABLE[411][58] = 1;
TABLE[411][126] = 1;
TABLE[411][33] = 1;
TABLE[411][45] = 1;
TABLE[411][43] = 1;
TABLE[411][471] = 1;
TABLE[411][470] = 1;
TABLE[411][40] = 1;
TABLE[412][256] = 1;
TABLE[412][259] = 1;
TABLE[412][257] = 1;
TABLE[412][453] = 1;
TABLE[412][341] = 1;
TABLE[412][391] = 1;
TABLE[412][36] = 1;
TABLE[412][258] = 1;
TABLE[412][383] = 1;
TABLE[412][380] = 1;
TABLE[412][58] = 1;
TABLE[412][126] = 1;
TABLE[412][33] = 1;
TABLE[412][45] = 1;
TABLE[412][43] = 1;
TABLE[412][471] = 1;
TABLE[412][470] = 1;
TABLE[412][40] = 1;
TABLE[413][40] = 1;
TABLE[413][470] = 1;
TABLE[413][471] = 1;
TABLE[413][43] = 1;
TABLE[413][45] = 1;
TABLE[413][33] = 1;
TABLE[413][126] = 1;
TABLE[413][58] = 1;
TABLE[413][380] = 1;
TABLE[413][383] = 1;
TABLE[413][258] = 1;
TABLE[413][36] = 1;
TABLE[413][391] = 1;
TABLE[413][341] = 1;
TABLE[413][453] = 1;
TABLE[413][257] = 1;
TABLE[413][259] = 1;
TABLE[413][256] = 1;
TABLE[414][40] = 1;
TABLE[414][470] = 1;
TABLE[414][471] = 1;
TABLE[414][43] = 1;
TABLE[414][45] = 1;
TABLE[414][33] = 1;
TABLE[414][126] = 1;
TABLE[414][58] = 1;
TABLE[414][380] = 1;
TABLE[414][383] = 1;
TABLE[414][258] = 1;
TABLE[414][36] = 1;
TABLE[414][391] = 1;
TABLE[414][341] = 1;
TABLE[414][453] = 1;
TABLE[414][257] = 1;
TABLE[414][259] = 1;
TABLE[414][256] = 1;
TABLE[415][40] = 1;
TABLE[415][470] = 1;
TABLE[415][471] = 1;
TABLE[415][43] = 1;
TABLE[415][45] = 1;
TABLE[415][33] = 1;
TABLE[415][126] = 1;
TABLE[415][58] = 1;
TABLE[415][380] = 1;
TABLE[415][383] = 1;
TABLE[415][258] = 1;
TABLE[415][36] = 1;
TABLE[415][391] = 1;
TABLE[415][341] = 1;
TABLE[415][453] = 1;
TABLE[415][257] = 1;
TABLE[415][259] = 1;
TABLE[415][256] = 1;
TABLE[416][40] = 1;
TABLE[416][470] = 1;
TABLE[416][471] = 1;
TABLE[416][43] = 1;
TABLE[416][45] = 1;
TABLE[416][33] = 1;
TABLE[416][126] = 1;
TABLE[416][58] = 1;
TABLE[416][380] = 1;
TABLE[416][383] = 1;
TABLE[416][258] = 1;
TABLE[416][36] = 1;
TABLE[416][391] = 1;
TABLE[416][341] = 1;
TABLE[416][453] = 1;
TABLE[416][257] = 1;
TABLE[416][259] = 1;
TABLE[416][256] = 1;
TABLE[417][40] = 1;
TABLE[417][470] = 1;
TABLE[417][471] = 1;
TABLE[417][43] = 1;
TABLE[417][45] = 1;
TABLE[417][33] = 1;
TABLE[417][126] = 1;
TABLE[417][58] = 1;
TABLE[417][380] = 1;
TABLE[417][383] = 1;
TABLE[417][258] = 1;
TABLE[417][36] = 1;
TABLE[417][391] = 1;
TABLE[417][341] = 1;
TABLE[417][453] = 1;
TABLE[417][257] = 1;
TABLE[417][259] = 1;
TABLE[417][256] = 1;
TABLE[418][40] = 1;
TABLE[418][470] = 1;
TABLE[418][471] = 1;
TABLE[418][43] = 1;
TABLE[418][45] = 1;
TABLE[418][33] = 1;
TABLE[418][126] = 1;
TABLE[418][58] = 1;
TABLE[418][380] = 1;
TABLE[418][383] = 1;
TABLE[418][258] = 1;
TABLE[418][36] = 1;
TABLE[418][391] = 1;
TABLE[418][341] = 1;
TABLE[418][453] = 1;
TABLE[418][257] = 1;
TABLE[418][259] = 1;
TABLE[418][256] = 1;
TABLE[419][40] = 1;
TABLE[419][470] = 1;
TABLE[419][471] = 1;
TABLE[419][43] = 1;
TABLE[419][45] = 1;
TABLE[419][33] = 1;
TABLE[419][126] = 1;
TABLE[419][58] = 1;
TABLE[419][380] = 1;
TABLE[419][383] = 1;
TABLE[419][258] = 1;
TABLE[419][36] = 1;
TABLE[419][391] = 1;
TABLE[419][341] = 1;
TABLE[419][453] = 1;
TABLE[419][257] = 1;
TABLE[419][259] = 1;
TABLE[419][256] = 1;
TABLE[420][256] = 1;
TABLE[420][259] = 1;
TABLE[420][257] = 1;
TABLE[420][453] = 1;
TABLE[420][341] = 1;
TABLE[420][391] = 1;
TABLE[420][36] = 1;
TABLE[420][258] = 1;
TABLE[420][383] = 1;
TABLE[420][380] = 1;
TABLE[420][58] = 1;
TABLE[420][126] = 1;
TABLE[420][33] = 1;
TABLE[420][45] = 1;
TABLE[420][43] = 1;
TABLE[420][471] = 1;
TABLE[420][470] = 1;
TABLE[420][40] = 1;
TABLE[421][256] = 1;
TABLE[421][259] = 1;
TABLE[421][257] = 1;
TABLE[421][453] = 1;
TABLE[421][341] = 1;
TABLE[421][391] = 1;
TABLE[421][36] = 1;
TABLE[421][258] = 1;
TABLE[421][383] = 1;
TABLE[421][380] = 1;
TABLE[421][58] = 1;
TABLE[421][126] = 1;
TABLE[421][33] = 1;
TABLE[421][45] = 1;
TABLE[421][43] = 1;
TABLE[421][471] = 1;
TABLE[421][470] = 1;
TABLE[421][40] = 1;
TABLE[422][40] = 1;
TABLE[422][470] = 1;
TABLE[422][471] = 1;
TABLE[422][43] = 1;
TABLE[422][45] = 1;
TABLE[422][33] = 1;
TABLE[422][126] = 1;
TABLE[422][58] = 1;
TABLE[422][380] = 1;
TABLE[422][383] = 1;
TABLE[422][258] = 1;
TABLE[422][36] = 1;
TABLE[422][391] = 1;
TABLE[422][341] = 1;
TABLE[422][453] = 1;
TABLE[422][257] = 1;
TABLE[422][259] = 1;
TABLE[422][256] = 1;
TABLE[423][40] = 1;
TABLE[423][470] = 1;
TABLE[423][471] = 1;
TABLE[423][43] = 1;
TABLE[423][45] = 1;
TABLE[423][33] = 1;
TABLE[423][126] = 1;
TABLE[423][58] = 1;
TABLE[423][380] = 1;
TABLE[423][383] = 1;
TABLE[423][258] = 1;
TABLE[423][36] = 1;
TABLE[423][391] = 1;
TABLE[423][341] = 1;
TABLE[423][453] = 1;
TABLE[423][257] = 1;
TABLE[423][259] = 1;
TABLE[423][256] = 1;
TABLE[424][256] = 1;
TABLE[424][259] = 1;
TABLE[424][257] = 1;
TABLE[424][453] = 1;
TABLE[424][341] = 1;
TABLE[424][391] = 1;
TABLE[424][36] = 1;
TABLE[424][258] = 1;
TABLE[424][383] = 1;
TABLE[424][380] = 1;
TABLE[424][58] = 1;
TABLE[424][126] = 1;
TABLE[424][33] = 1;
TABLE[424][45] = 1;
TABLE[424][43] = 1;
TABLE[424][471] = 1;
TABLE[424][470] = 1;
TABLE[424][40] = 1;
TABLE[425][256] = 1;
TABLE[425][259] = 1;
TABLE[425][257] = 1;
TABLE[425][453] = 1;
TABLE[425][341] = 1;
TABLE[425][391] = 1;
TABLE[425][36] = 1;
TABLE[425][258] = 1;
TABLE[425][383] = 1;
TABLE[425][380] = 1;
TABLE[425][58] = 1;
TABLE[425][126] = 1;
TABLE[425][33] = 1;
TABLE[425][45] = 1;
TABLE[425][43] = 1;
TABLE[425][471] = 1;
TABLE[425][470] = 1;
TABLE[425][40] = 1;
TABLE[426][40] = 1;
TABLE[426][470] = 1;
TABLE[426][471] = 1;
TABLE[426][43] = 1;
TABLE[426][45] = 1;
TABLE[426][33] = 1;
TABLE[426][126] = 1;
TABLE[426][58] = 1;
TABLE[426][380] = 1;
TABLE[426][383] = 1;
TABLE[426][258] = 1;
TABLE[426][36] = 1;
TABLE[426][391] = 1;
TABLE[426][341] = 1;
TABLE[426][453] = 1;
TABLE[426][257] = 1;
TABLE[426][259] = 1;
TABLE[426][256] = 1;
TABLE[427][40] = 1;
TABLE[427][470] = 1;
TABLE[427][471] = 1;
TABLE[427][43] = 1;
TABLE[427][45] = 1;
TABLE[427][33] = 1;
TABLE[427][126] = 1;
TABLE[427][58] = 1;
TABLE[427][380] = 1;
TABLE[427][383] = 1;
TABLE[427][258] = 1;
TABLE[427][36] = 1;
TABLE[427][391] = 1;
TABLE[427][341] = 1;
TABLE[427][453] = 1;
TABLE[427][257] = 1;
TABLE[427][259] = 1;
TABLE[427][256] = 1;
TABLE[428][256] = 1;
TABLE[428][259] = 1;
TABLE[428][257] = 1;
TABLE[428][453] = 1;
TABLE[428][341] = 1;
TABLE[428][391] = 1;
TABLE[428][36] = 1;
TABLE[428][258] = 1;
TABLE[428][383] = 1;
TABLE[428][380] = 1;
TABLE[428][58] = 1;
TABLE[428][126] = 1;
TABLE[428][33] = 1;
TABLE[428][45] = 1;
TABLE[428][43] = 1;
TABLE[428][471] = 1;
TABLE[428][470] = 1;
TABLE[428][40] = 1;
TABLE[429][256] = 1;
TABLE[429][259] = 1;
TABLE[429][257] = 1;
TABLE[429][453] = 1;
TABLE[429][341] = 1;
TABLE[429][391] = 1;
TABLE[429][36] = 1;
TABLE[429][258] = 1;
TABLE[429][383] = 1;
TABLE[429][380] = 1;
TABLE[429][58] = 1;
TABLE[429][126] = 1;
TABLE[429][33] = 1;
TABLE[429][45] = 1;
TABLE[429][43] = 1;
TABLE[429][471] = 1;
TABLE[429][470] = 1;
TABLE[429][40] = 1;
TABLE[430][40] = 1;
TABLE[430][470] = 1;
TABLE[430][471] = 1;
TABLE[430][43] = 1;
TABLE[430][45] = 1;
TABLE[430][33] = 1;
TABLE[430][126] = 1;
TABLE[430][58] = 1;
TABLE[430][380] = 1;
TABLE[430][383] = 1;
TABLE[430][258] = 1;
TABLE[430][36] = 1;
TABLE[430][391] = 1;
TABLE[430][341] = 1;
TABLE[430][453] = 1;
TABLE[430][257] = 1;
TABLE[430][259] = 1;
TABLE[430][256] = 1;
TABLE[431][256] = 1;
TABLE[431][259] = 1;
TABLE[431][257] = 1;
TABLE[431][453] = 1;
TABLE[431][341] = 1;
TABLE[431][391] = 1;
TABLE[431][36] = 1;
TABLE[431][258] = 1;
TABLE[431][383] = 1;
TABLE[431][380] = 1;
TABLE[431][58] = 1;
TABLE[431][126] = 1;
TABLE[431][33] = 1;
TABLE[431][45] = 1;
TABLE[431][43] = 1;
TABLE[431][471] = 1;
TABLE[431][470] = 1;
TABLE[431][40] = 1;
TABLE[432][256] = 1;
TABLE[432][259] = 1;
TABLE[432][257] = 1;
TABLE[432][453] = 1;
TABLE[432][341] = 1;
TABLE[432][391] = 1;
TABLE[432][36] = 1;
TABLE[432][258] = 1;
TABLE[432][383] = 1;
TABLE[432][380] = 1;
TABLE[432][58] = 1;
TABLE[432][126] = 1;
TABLE[432][33] = 1;
TABLE[432][45] = 1;
TABLE[432][43] = 1;
TABLE[432][471] = 1;
TABLE[432][470] = 1;
TABLE[432][40] = 1;
TABLE[433][258] = 1;
TABLE[433][40] = 1;
TABLE[433][36] = 1;
TABLE[433][391] = 1;
TABLE[433][341] = 1;
TABLE[433][453] = 1;
TABLE[433][257] = 1;
TABLE[433][259] = 1;
TABLE[433][256] = 1;
TABLE[434][258] = 1;
TABLE[434][40] = 1;
TABLE[434][36] = 1;
TABLE[434][391] = 1;
TABLE[434][341] = 1;
TABLE[434][453] = 1;
TABLE[434][257] = 1;
TABLE[434][259] = 1;
TABLE[434][256] = 1;
TABLE[435][258] = 1;
TABLE[435][40] = 1;
TABLE[435][36] = 1;
TABLE[435][391] = 1;
TABLE[435][341] = 1;
TABLE[435][453] = 1;
TABLE[435][257] = 1;
TABLE[435][259] = 1;
TABLE[435][256] = 1;
TABLE[436][258] = 1;
TABLE[436][40] = 1;
TABLE[436][36] = 1;
TABLE[436][391] = 1;
TABLE[436][341] = 1;
TABLE[436][453] = 1;
TABLE[436][257] = 1;
TABLE[436][259] = 1;
TABLE[436][256] = 1;
TABLE[437][258] = 1;
TABLE[437][40] = 1;
TABLE[437][36] = 1;
TABLE[437][391] = 1;
TABLE[437][341] = 1;
TABLE[437][453] = 1;
TABLE[437][257] = 1;
TABLE[437][259] = 1;
TABLE[437][256] = 1;
TABLE[438][258] = 1;
TABLE[438][40] = 1;
TABLE[438][36] = 1;
TABLE[438][391] = 1;
TABLE[438][341] = 1;
TABLE[438][453] = 1;
TABLE[438][257] = 1;
TABLE[438][259] = 1;
TABLE[438][256] = 1;
TABLE[439][258] = 1;
TABLE[439][40] = 1;
TABLE[439][36] = 1;
TABLE[439][391] = 1;
TABLE[439][341] = 1;
TABLE[439][453] = 1;
TABLE[439][257] = 1;
TABLE[439][259] = 1;
TABLE[439][256] = 1;
TABLE[440][258] = 1;
TABLE[440][40] = 1;
TABLE[440][36] = 1;
TABLE[440][391] = 1;
TABLE[440][341] = 1;
TABLE[440][453] = 1;
TABLE[440][257] = 1;
TABLE[440][259] = 1;
TABLE[440][256] = 1;
TABLE[441][258] = 1;
TABLE[441][40] = 1;
TABLE[441][36] = 1;
TABLE[441][391] = 1;
TABLE[441][341] = 1;
TABLE[441][453] = 1;
TABLE[441][257] = 1;
TABLE[441][259] = 1;
TABLE[441][256] = 1;
TABLE[442][258] = 1;
TABLE[442][40] = 1;
TABLE[442][36] = 1;
TABLE[442][391] = 1;
TABLE[442][341] = 1;
TABLE[442][453] = 1;
TABLE[442][257] = 1;
TABLE[442][259] = 1;
TABLE[442][256] = 1;
TABLE[443][258] = 1;
TABLE[443][40] = 1;
TABLE[443][36] = 1;
TABLE[443][391] = 1;
TABLE[443][341] = 1;
TABLE[443][453] = 1;
TABLE[443][257] = 1;
TABLE[443][259] = 1;
TABLE[443][256] = 1;
TABLE[444][40] = 1;
TABLE[444][470] = 1;
TABLE[444][471] = 1;
TABLE[444][43] = 1;
TABLE[444][45] = 1;
TABLE[444][33] = 1;
TABLE[444][126] = 1;
TABLE[444][58] = 1;
TABLE[444][380] = 1;
TABLE[444][383] = 1;
TABLE[444][258] = 1;
TABLE[444][36] = 1;
TABLE[444][391] = 1;
TABLE[444][341] = 1;
TABLE[444][453] = 1;
TABLE[444][257] = 1;
TABLE[444][259] = 1;
TABLE[444][256] = 1;
TABLE[445][470] = 1;
TABLE[445][471] = 1;
TABLE[445][43] = 1;
TABLE[445][45] = 1;
TABLE[445][33] = 1;
TABLE[445][126] = 1;
TABLE[445][58] = 1;
TABLE[445][380] = 1;
TABLE[445][383] = 1;
TABLE[445][258] = 1;
TABLE[445][40] = 1;
TABLE[445][36] = 1;
TABLE[445][391] = 1;
TABLE[445][341] = 1;
TABLE[445][453] = 1;
TABLE[445][257] = 1;
TABLE[445][259] = 1;
TABLE[445][256] = 1;
TABLE[446][256] = 1;
TABLE[446][259] = 1;
TABLE[446][257] = 1;
TABLE[446][453] = 1;
TABLE[446][341] = 1;
TABLE[446][391] = 1;
TABLE[446][36] = 1;
TABLE[446][40] = 1;
TABLE[446][258] = 1;
TABLE[446][383] = 1;
TABLE[446][380] = 1;
TABLE[446][58] = 1;
TABLE[446][126] = 1;
TABLE[446][33] = 1;
TABLE[446][45] = 1;
TABLE[446][43] = 1;
TABLE[446][471] = 1;
TABLE[446][470] = 1;
TABLE[447][41] = 1;
TABLE[447][470] = 1;
TABLE[447][471] = 1;
TABLE[447][43] = 1;
TABLE[447][45] = 1;
TABLE[447][33] = 1;
TABLE[447][126] = 1;
TABLE[447][58] = 1;
TABLE[447][380] = 1;
TABLE[447][383] = 1;
TABLE[447][258] = 1;
TABLE[447][40] = 1;
TABLE[447][36] = 1;
TABLE[447][391] = 1;
TABLE[447][341] = 1;
TABLE[447][453] = 1;
TABLE[447][257] = 1;
TABLE[447][259] = 1;
TABLE[447][256] = 1;
TABLE[944][256] = 1;
TABLE[944][259] = 1;
TABLE[944][257] = 1;
TABLE[944][453] = 1;
TABLE[944][341] = 1;
TABLE[944][391] = 1;
TABLE[944][36] = 1;
TABLE[944][40] = 1;
TABLE[944][258] = 1;
TABLE[944][383] = 1;
TABLE[944][380] = 1;
TABLE[944][58] = 1;
TABLE[944][126] = 1;
TABLE[944][33] = 1;
TABLE[944][45] = 1;
TABLE[944][43] = 1;
TABLE[944][471] = 1;
TABLE[944][470] = 1;
TABLE[1328][41] = 1;
TABLE[943][44] = 1;
TABLE[1327][41] = 1;
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
         case 136: return "Subphrase"; break;
         case 140: return "Subphrase"; break;
         case 148: return "Subphrase"; break;
         case 155: return "res_ref"; break;
         case 165: return "cexpr"; break;
         case 181: return "cexpr2"; break;
         case 215: return "cexpr3"; break;
         case 229: return "cexpr4"; break;
         case 251: return "dice_val"; break;
         case 278: return "dice_only"; break;
         case 292: return "gear_entry"; break;
         case 302: return "Subphrase"; break;
         case 312: return "Subphrase"; break;
         case 320: return "Subphrase"; break;
         case 327: return "Subphrase"; break;
         case 338: return "Subphrase"; break;
         case 347: return "Subphrase"; break;
         case 360: return "Subphrase"; break;
         case 409: return "Subphrase"; break;
         case 418: return "Subphrase"; break;
         case 425: return "gear_desc"; break;
         case 433: return "Subphrase"; break;
         case 437: return "Subphrase"; break;
         case 446: return "Subphrase"; break;
         case 454: return "glyph_entry"; break;
         case 463: return "Subphrase"; break;
         case 471: return "Subphrase"; break;
         case 478: return "Subphrase"; break;
         case 485: return "Subphrase"; break;
         case 493: return "Subphrase"; break;
         case 501: return "glyph_desc"; break;
         case 507: return "Subphrase"; break;
         case 514: return "Subphrase"; break;
         case 522: return "Subphrase"; break;
         case 530: return "event_desc"; break;
         case 538: return "Subphrase"; break;
         case 547: return "event_entry"; break;
         case 553: return "Subphrase"; break;
         case 562: return "Subphrase"; break;
         case 571: return "Subphrase"; break;
         case 580: return "sep_expr"; break;
         case 584: return "mval"; break;
         case 589: return "Subphrase"; break;
         case 609: return "Subphrase"; break;
         case 616: return "Subphrase"; break;
         case 625: return "Subphrase"; break;
         case 640: return "monster_def"; break;
         case 651: return "Subphrase"; break;
         case 660: return "Subphrase"; break;
         case 668: return "Subphrase"; break;
         case 676: return "mon_entry"; break;
         case 912: return "Subphrase"; break;
         case 919: return "Subphrase"; break;
         case 926: return "Subphrase"; break;
         case 934: return "Subphrase"; break;
         case 941: return "Subphrase"; break;
         case 949: return "Subphrase"; break;
         case 956: return "Subphrase"; break;
         case 963: return "Subphrase"; break;
         case 970: return "Subphrase"; break;
         case 977: return "Subphrase"; break;
         case 984: return "Subphrase"; break;
         case 991: return "Subphrase"; break;
         case 1000: return "Subphrase"; break;
         case 1007: return "Subphrase"; break;
         case 1014: return "Subphrase"; break;
         case 1023: return "Subphrase"; break;
         case 1030: return "Subphrase"; break;
         case 1037: return "Subphrase"; break;
         case 1046: return "Subphrase"; break;
         case 1053: return "Subphrase"; break;
         case 1060: return "Subphrase"; break;
         case 1067: return "Subphrase"; break;
         case 1074: return "Subphrase"; break;
         case 1083: return "Subphrase"; break;
         case 1090: return "Subphrase"; break;
         case 1098: return "Subphrase"; break;
         case 1107: return "Subphrase"; break;
         case 1114: return "Subphrase"; break;
         case 1121: return "Subphrase"; break;
         case 1128: return "Subphrase"; break;
         case 1135: return "Subphrase"; break;
         case 1142: return "Subphrase"; break;
         case 1149: return "Subphrase"; break;
         case 1158: return "Subphrase"; break;
         case 1165: return "Subphrase"; break;
         case 1172: return "Subphrase"; break;
         case 1181: return "Subphrase"; break;
         case 1188: return "Subphrase"; break;
         case 1195: return "Subphrase"; break;
         case 1204: return "Subphrase"; break;
         case 1211: return "color"; break;
         case 1230: return "attack_entry"; break;
         case 1247: return "Subphrase"; break;
         case 1259: return "Subphrase"; break;
         case 1266: return "Subphrase"; break;
         case 1273: return "Subphrase"; break;
         case 1283: return "Subphrase"; break;
         case 1295: return "Subphrase"; break;
         case 1307: return "Subphrase"; break;
         case 1314: return "Subphrase"; break;
         case 1321: return "Subphrase"; break;
         case 1328: return "Subphrase"; break;
         case 1338: return "Subphrase"; break;
         case 1350: return "Subphrase"; break;
         case 1357: return "mflag_entry"; break;
         case 1368: return "Subphrase"; break;
         case 1375: return "Subphrase"; break;
         case 1382: return "attrib_entry"; break;
         case 1388: return "Subphrase"; break;
         case 1395: return "item_def"; break;
         case 1405: return "Subphrase"; break;
         case 1413: return "item_entry"; break;
         case 1679: return "Subphrase"; break;
         case 1686: return "Subphrase"; break;
         case 1693: return "Subphrase"; break;
         case 1700: return "Subphrase"; break;
         case 1707: return "Subphrase"; break;
         case 1714: return "Subphrase"; break;
         case 1721: return "Subphrase"; break;
         case 1728: return "Subphrase"; break;
         case 1735: return "Subphrase"; break;
         case 1742: return "Subphrase"; break;
         case 1749: return "Subphrase"; break;
         case 1756: return "Subphrase"; break;
         case 1763: return "Subphrase"; break;
         case 1770: return "Subphrase"; break;
         case 1777: return "Subphrase"; break;
         case 1784: return "Subphrase"; break;
         case 1791: return "Subphrase"; break;
         case 1798: return "Subphrase"; break;
         case 1805: return "Subphrase"; break;
         case 1812: return "Subphrase"; break;
         case 1819: return "Subphrase"; break;
         case 1826: return "Subphrase"; break;
         case 1833: return "Subphrase"; break;
         case 1841: return "Subphrase"; break;
         case 1848: return "Subphrase"; break;
         case 1855: return "Subphrase"; break;
         case 1864: return "Subphrase"; break;
         case 1871: return "Subphrase"; break;
         case 1878: return "Subphrase"; break;
         case 1887: return "Subphrase"; break;
         case 1894: return "Subphrase"; break;
         case 1901: return "Subphrase"; break;
         case 1909: return "Subphrase"; break;
         case 1916: return "Subphrase"; break;
         case 1925: return "Subphrase"; break;
         case 1936: return "Subphrase"; break;
         case 1943: return "Subphrase"; break;
         case 1950: return "Subphrase"; break;
         case 1957: return "Subphrase"; break;
         case 1964: return "Subphrase"; break;
         case 1971: return "Subphrase"; break;
         case 1978: return "Subphrase"; break;
         case 1985: return "Subphrase"; break;
         case 1992: return "Subphrase"; break;
         case 1999: return "arm_entry"; break;
         case 2010: return "Subphrase"; break;
         case 2017: return "Subphrase"; break;
         case 2024: return "iflag_entry"; break;
         case 2035: return "Subphrase"; break;
         case 2042: return "Subphrase"; break;
         case 2049: return "feature_def"; break;
         case 2059: return "Subphrase"; break;
         case 2067: return "feat_entry"; break;
         case 2140: return "Subphrase"; break;
         case 2147: return "Subphrase"; break;
         case 2154: return "Subphrase"; break;
         case 2161: return "Subphrase"; break;
         case 2168: return "Subphrase"; break;
         case 2175: return "Subphrase"; break;
         case 2184: return "Subphrase"; break;
         case 2195: return "Subphrase"; break;
         case 2203: return "Subphrase"; break;
         case 2210: return "Subphrase"; break;
         case 2217: return "Subphrase"; break;
         case 2224: return "effect_def"; break;
         case 2236: return "Subphrase"; break;
         case 2245: return "Subphrase"; break;
         case 2252: return "Subphrase"; break;
         case 2268: return "Subphrase"; break;
         case 2276: return "Subphrase"; break;
         case 2288: return "Subphrase"; break;
         case 2296: return "eff_entry"; break;
         case 2450: return "Subphrase"; break;
         case 2457: return "Subphrase"; break;
         case 2464: return "Subphrase"; break;
         case 2471: return "Subphrase"; break;
         case 2478: return "Subphrase"; break;
         case 2485: return "Subphrase"; break;
         case 2492: return "Subphrase"; break;
         case 2499: return "Subphrase"; break;
         case 2508: return "Subphrase"; break;
         case 2515: return "Subphrase"; break;
         case 2522: return "Subphrase"; break;
         case 2529: return "Subphrase"; break;
         case 2536: return "Subphrase"; break;
         case 2543: return "Subphrase"; break;
         case 2550: return "Subphrase"; break;
         case 2557: return "Subphrase"; break;
         case 2564: return "Subphrase"; break;
         case 2571: return "Subphrase"; break;
         case 2578: return "Subphrase"; break;
         case 2585: return "Subphrase"; break;
         case 2592: return "Subphrase"; break;
         case 2599: return "Subphrase"; break;
         case 2606: return "Subphrase"; break;
         case 2613: return "Subphrase"; break;
         case 2622: return "Subphrase"; break;
         case 2633: return "numlist"; break;
         case 2637: return "Subphrase"; break;
         case 2646: return "Subphrase"; break;
         case 2657: return "object_entry"; break;
         case 2661: return "race_def"; break;
         case 2670: return "Subphrase"; break;
         case 2678: return "Subphrase"; break;
         case 2686: return "race_entry"; break;
         case 2771: return "Subphrase"; break;
         case 2778: return "Subphrase"; break;
         case 2789: return "Subphrase"; break;
         case 2796: return "Subphrase"; break;
         case 2803: return "Subphrase"; break;
         case 2810: return "Subphrase"; break;
         case 2819: return "Subphrase"; break;
         case 2826: return "Subphrase"; break;
         case 2833: return "Subphrase"; break;
         case 2840: return "Subphrase"; break;
         case 2847: return "Subphrase"; break;
         case 2854: return "Subphrase"; break;
         case 2861: return "Subphrase"; break;
         case 2868: return "Subphrase"; break;
         case 2877: return "Subphrase"; break;
         case 2884: return "Subphrase"; break;
         case 2891: return "Subphrase"; break;
         case 2898: return "Subphrase"; break;
         case 2907: return "Subphrase"; break;
         case 2918: return "class_def"; break;
         case 2926: return "Subphrase"; break;
         case 2934: return "class_entry"; break;
         case 3027: return "Subphrase"; break;
         case 3034: return "Subphrase"; break;
         case 3041: return "Subphrase"; break;
         case 3048: return "Subphrase"; break;
         case 3055: return "Subphrase"; break;
         case 3062: return "Subphrase"; break;
         case 3069: return "Subphrase"; break;
         case 3076: return "Subphrase"; break;
         case 3086: return "Subphrase"; break;
         case 3093: return "Subphrase"; break;
         case 3100: return "Subphrase"; break;
         case 3110: return "Subphrase"; break;
         case 3117: return "Subphrase"; break;
         case 3124: return "Subphrase"; break;
         case 3133: return "Subphrase"; break;
         case 3140: return "Subphrase"; break;
         case 3144: return "Subphrase"; break;
         case 3148: return "Subphrase"; break;
         case 3155: return "Subphrase"; break;
         case 3164: return "Subphrase"; break;
         case 3171: return "Subphrase"; break;
         case 3178: return "Subphrase"; break;
         case 3187: return "Subphrase"; break;
         case 3194: return "Subphrase"; break;
         case 3201: return "Subphrase"; break;
         case 3210: return "Subphrase"; break;
         case 3221: return "special_ability"; break;
         case 3251: return "Subphrase"; break;
         case 3258: return "Subphrase"; break;
         case 3265: return "Subphrase"; break;
         case 3272: return "abil_param"; break;
         case 3289: return "abil_level"; break;
         case 3324: return "Subphrase"; break;
         case 3331: return "Subphrase"; break;
         case 3338: return "Subphrase"; break;
         case 3345: return "dungeon_def"; break;
         case 3353: return "Subphrase"; break;
         case 3361: return "dun_entry"; break;
         case 3380: return "Subphrase"; break;
         case 3387: return "dconst_entry"; break;
         case 3407: return "Subphrase"; break;
         case 3414: return "Subphrase"; break;
         case 3433: return "Subphrase"; break;
         case 3440: return "Subphrase"; break;
         case 3447: return "Subphrase"; break;
         case 3454: return "Subphrase"; break;
         case 3465: return "Subphrase"; break;
         case 3473: return "Subphrase"; break;
         case 3481: return "Subphrase"; break;
         case 3488: return "Subphrase"; break;
         case 3499: return "Subphrase"; break;
         case 3507: return "Subphrase"; break;
         case 3528: return "Subphrase"; break;
         case 3535: return "list_entry"; break;
         case 3547: return "artifact_def"; break;
         case 3555: return "Subphrase"; break;
         case 3563: return "arti_entry"; break;
         case 3639: return "Subphrase"; break;
         case 3646: return "Subphrase"; break;
         case 3653: return "Subphrase"; break;
         case 3664: return "Subphrase"; break;
         case 3671: return "Subphrase"; break;
         case 3678: return "Subphrase"; break;
         case 3685: return "Subphrase"; break;
         case 3694: return "Subphrase"; break;
         case 3701: return "Subphrase"; break;
         case 3708: return "Subphrase"; break;
         case 3717: return "Subphrase"; break;
         case 3724: return "Subphrase"; break;
         case 3731: return "Subphrase"; break;
         case 3740: return "Subphrase"; break;
         case 3747: return "Subphrase"; break;
         case 3756: return "Subphrase"; break;
         case 3760: return "Subphrase"; break;
         case 3769: return "Subphrase"; break;
         case 3773: return "Subphrase"; break;
         case 3782: return "Subphrase"; break;
         case 3786: return "Subphrase"; break;
         case 3795: return "Subphrase"; break;
         case 3799: return "art_power"; break;
         case 3851: return "Subphrase"; break;
         case 3859: return "Subphrase"; break;
         case 3867: return "Subphrase"; break;
         case 3875: return "domain_def"; break;
         case 3885: return "Subphrase"; break;
         case 3893: return "domain_entry"; break;
         case 3926: return "Subphrase"; break;
         case 3933: return "Subphrase"; break;
         case 3942: return "Subphrase"; break;
         case 3949: return "Subphrase"; break;
         case 3956: return "Subphrase"; break;
         case 3965: return "Subphrase"; break;
         case 3972: return "Subphrase"; break;
         case 3979: return "god_def"; break;
         case 3987: return "Subphrase"; break;
         case 3995: return "god_entry"; break;
         case 4040: return "Subphrase"; break;
         case 4047: return "Subphrase"; break;
         case 4056: return "Subphrase"; break;
         case 4063: return "Subphrase"; break;
         case 4070: return "Subphrase"; break;
         case 4079: return "Subphrase"; break;
         case 4090: return "Subphrase"; break;
         case 4097: return "Subphrase"; break;
         case 4104: return "Subphrase"; break;
         case 4113: return "Subphrase"; break;
         case 4120: return "terrain_def"; break;
         case 4128: return "Subphrase"; break;
         case 4136: return "terrain_entry"; break;
         case 4188: return "Subphrase"; break;
         case 4195: return "Subphrase"; break;
         case 4202: return "Subphrase"; break;
         case 4209: return "Subphrase"; break;
         case 4216: return "Subphrase"; break;
         case 4223: return "Subphrase"; break;
         case 4232: return "Subphrase"; break;
         case 4243: return "region_def"; break;
         case 4253: return "Subphrase"; break;
         case 4261: return "region_entry"; break;
         case 4355: return "Subphrase"; break;
         case 4362: return "Subphrase"; break;
         case 4369: return "Subphrase"; break;
         case 4376: return "Subphrase"; break;
         case 4383: return "Subphrase"; break;
         case 4390: return "Subphrase"; break;
         case 4397: return "Subphrase"; break;
         case 4404: return "Subphrase"; break;
         case 4413: return "Subphrase"; break;
         case 4424: return "Subphrase"; break;
         case 4431: return "Subphrase"; break;
         case 4440: return "Subphrase"; break;
         case 4451: return "Subphrase"; break;
         case 4463: return "Subphrase"; break;
         case 4486: return "Subphrase"; break;
         case 4502: return "Subphrase"; break;
         case 4509: return "Subphrase"; break;
         case 4516: return "Subphrase"; break;
         case 4537: return "Subphrase"; break;
         case 4544: return "Subphrase"; break;
         case 4552: return "Subphrase"; break;
         case 4560: return "template_def"; break;
         case 4571: return "Subphrase"; break;
         case 4579: return "Subphrase"; break;
         case 4587: return "template_entry"; break;
         case 4814: return "Subphrase"; break;
         case 4821: return "Subphrase"; break;
         case 4828: return "Subphrase"; break;
         case 4835: return "Subphrase"; break;
         case 4843: return "Subphrase"; break;
         case 4850: return "Subphrase"; break;
         case 4857: return "Subphrase"; break;
         case 4864: return "Subphrase"; break;
         case 4871: return "Subphrase"; break;
         case 4878: return "Subphrase"; break;
         case 4885: return "Subphrase"; break;
         case 4892: return "Subphrase"; break;
         case 4899: return "Subphrase"; break;
         case 4906: return "Subphrase"; break;
         case 4913: return "Subphrase"; break;
         case 4920: return "Subphrase"; break;
         case 4927: return "Subphrase"; break;
         case 4935: return "Subphrase"; break;
         case 4942: return "Subphrase"; break;
         case 4951: return "Subphrase"; break;
         case 4958: return "Subphrase"; break;
         case 4967: return "Subphrase"; break;
         case 4976: return "Subphrase"; break;
         case 4984: return "Subphrase"; break;
         case 4991: return "Subphrase"; break;
         case 4998: return "Subphrase"; break;
         case 5007: return "Subphrase"; break;
         case 5016: return "Subphrase"; break;
         case 5024: return "Subphrase"; break;
         case 5031: return "Subphrase"; break;
         case 5038: return "Subphrase"; break;
         case 5047: return "Subphrase"; break;
         case 5054: return "Subphrase"; break;
         case 5063: return "Subphrase"; break;
         case 5070: return "Subphrase"; break;
         case 5077: return "Subphrase"; break;
         case 5086: return "Subphrase"; break;
         case 5093: return "Subphrase"; break;
         case 5100: return "Subphrase"; break;
         case 5109: return "Subphrase"; break;
         case 5116: return "Subphrase"; break;
         case 5123: return "Subphrase"; break;
         case 5132: return "Subphrase"; break;
         case 5141: return "Subphrase"; break;
         case 5149: return "Subphrase"; break;
         case 5157: return "temp_attack_entry"; break;
         case 5174: return "Subphrase"; break;
         case 5186: return "Subphrase"; break;
         case 5193: return "Subphrase"; break;
         case 5200: return "Subphrase"; break;
         case 5210: return "Subphrase"; break;
         case 5222: return "Subphrase"; break;
         case 5234: return "Subphrase"; break;
         case 5241: return "Subphrase"; break;
         case 5248: return "Subphrase"; break;
         case 5255: return "Subphrase"; break;
         case 5265: return "Subphrase"; break;
         case 5277: return "Subphrase"; break;
         case 5284: return "quest_def"; break;
         case 5288: return "flavor_def"; break;
         case 5298: return "Subphrase"; break;
         case 5306: return "flavor_entry"; break;
         case 5335: return "Subphrase"; break;
         case 5342: return "Subphrase"; break;
         case 5349: return "Subphrase"; break;
         case 5356: return "Subphrase"; break;
         case 5363: return "prototype"; break;
         case 5367: return "type_name"; break;
         case 5395: return "s_prototype"; break;
         case 5408: return "Subphrase"; break;
         case 5419: return "Subphrase"; break;
         case 5427: return "Subphrase"; break;
         case 5434: return "Subphrase"; break;
         case 5442: return "Subphrase"; break;
         case 5454: return "Subphrase"; break;
         case 5462: return "Subphrase"; break;
         case 5469: return "Subphrase"; break;
         case 5477: return "Subphrase"; break;
         case 5485: return "sg_prototype"; break;
         case 5496: return "Subphrase"; break;
         case 5507: return "Subphrase"; break;
         case 5515: return "Subphrase"; break;
         case 5522: return "Subphrase"; break;
         case 5530: return "Subphrase"; break;
         case 5542: return "Subphrase"; break;
         case 5550: return "Subphrase"; break;
         case 5557: return "Subphrase"; break;
         case 5565: return "Subphrase"; break;
         case 5573: return "s_declaration"; break;
         case 5582: return "s_object_decl"; break;
         case 5591: return "g_declaration"; break;
         case 5598: return "Subphrase"; break;
         case 5607: return "l_declaration"; break;
         case 5614: return "Subphrase"; break;
         case 5623: return "r_declaration"; break;
         case 5630: return "Subphrase"; break;
         case 5639: return "statement"; break;
         case 5728: return "Subphrase"; break;
         case 5739: return "Subphrase"; break;
         case 5747: return "Subphrase"; break;
         case 5755: return "Subphrase"; break;
         case 5764: return "Subphrase"; break;
         case 5772: return "block"; break;
         case 5778: return "Subphrase"; break;
         case 5786: return "routine"; break;
         case 5793: return "Subphrase"; break;
         case 5807: return "Subphrase"; break;
         case 5815: return "fund_expr"; break;
         case 5881: return "Subphrase"; break;
         case 5889: return "Subphrase"; break;
         case 5897: return "lvalue"; break;
         case 5913: return "Subphrase"; break;
         case 5921: return "expr10"; break;
         case 5953: return "expr20"; break;
         case 5993: return "expr30"; break;
         case 6003: return "expr40"; break;
         case 6014: return "expr50"; break;
         case 6036: return "expr60"; break;
         case 6052: return "expr70"; break;
         case 6068: return "expr80"; break;
         case 6108: return "expr90"; break;
         case 6118: return "expr100"; break;
         case 6128: return "expr110"; break;
         case 6138: return "expr120"; break;
         case 6148: return "expr130"; break;
         case 6158: return "expr140"; break;
         case 6170: return "expr150"; break;
         case 6240: return "expr"; break;
         case 6250: return "param_list"; break;
         case 6254: return "Subphrase"; break;
         case 6262: return "Subphrase"; break;
   }
   else 
      switch(n-50000) {
         case 491: return "MEMBER_OP"; break;
         case 490: return "ELLIPSIS"; break;
         case 489: return "ASSIGN_SUB"; break;
         case 488: return "ASSIGN_RSHIFT"; break;
         case 487: return "ASSIGN_LSHIFT"; break;
         case 486: return "ASSIGN_NEG"; break;
         case 485: return "ASSIGN_OR"; break;
         case 484: return "ASSIGN_AND"; break;
         case 483: return "ASSIGN_MOD"; break;
         case 482: return "ASSIGN_DIV"; break;
         case 481: return "ASSIGN_ADD"; break;
         case 480: return "ASSIGN_MULT"; break;
         case 479: return "OR_OP"; break;
         case 478: return "AND_OP"; break;
         case 477: return "NE_OP"; break;
         case 476: return "EQ_OP"; break;
         case 475: return "LE_OP"; break;
         case 474: return "GE_OP"; break;
         case 473: return "RSHIFT_OP"; break;
         case 472: return "LSHIFT_OP"; break;
         case 471: return "DECREMENT_OP"; break;
         case 470: return "INCREMENT_OP"; break;
         case 469: return "WORD_RECT"; break;
         case 468: return "YVAL"; break;
         case 467: return "XVAL"; break;
         case 466: return "WITH"; break;
         case 465: return "WIELD"; break;
         case 464: return "WHILE"; break;
         case 463: return "WEIGHT_LIM"; break;
         case 462: return "WEIGHT_MOD"; break;
         case 461: return "WEIGHT"; break;
         case 460: return "WEEK"; break;
         case 459: return "WALLS"; break;
         case 458: return "VAR"; break;
         case 457: return "VALUE"; break;
         case 456: return "TVAL"; break;
         case 455: return "TYPE"; break;
         case 454: return "TURNS"; break;
         case 453: return "WORD_TRUE"; break;
         case 452: return "TO"; break;
         case 451: return "TIMEOUT"; break;
         case 450: return "TILES"; break;
         case 449: return "TILE"; break;
         case 448: return "THROWING"; break;
         case 447: return "THREAT"; break;
         case 446: return "TEXT"; break;
         case 445: return "TERRAIN"; break;
         case 444: return "WORD_TEMPLATE"; break;
         case 443: return "WORD_TARGET"; break;
         case 442: return "SYSTEM"; break;
         case 441: return "SWITCH"; break;
         case 440: return "SVAL"; break;
         case 439: return "SUSTAINS"; break;
         case 438: return "STORE"; break;
         case 437: return "STOCK"; break;
         case 436: return "STATIC"; break;
         case 435: return "STATI"; break;
         case 434: return "STARTING"; break;
         case 433: return "START"; break;
         case 432: return "SPELLS"; break;
         case 431: return "SPELL"; break;
         case 430: return "SPEED"; break;
         case 429: return "SPECIALS"; break;
         case 428: return "SLOT"; break;
         case 427: return "SKILLS"; break;
         case 426: return "SIZE"; break;
         case 425: return "SDMG"; break;
         case 424: return "SAVES"; break;
         case 423: return "RVAL"; break;
         case 422: return "ROUTINE"; break;
         case 421: return "ROOMTYPES"; break;
         case 420: return "RETURN"; break;
         case 419: return "RESISTS"; break;
         case 418: return "REGIONS"; break;
         case 417: return "REGION"; break;
         case 416: return "RATING"; break;
         case 415: return "RANGE"; break;
         case 414: return "RANDOM"; break;
         case 413: return "RACE"; break;
         case 412: return "QVAL"; break;
         case 411: return "QUEST"; break;
         case 410: return "QUALITIES"; break;
         case 409: return "PVAL"; break;
         case 408: return "PURPOSE"; break;
         case 407: return "PROP"; break;
         case 406: return "PROFICIENCIES"; break;
         case 405: return "POWER"; break;
         case 404: return "POISON"; break;
         case 403: return "PER"; break;
         case 402: return "PENALTY"; break;
         case 401: return "PARTIAL"; break;
         case 400: return "PARRY"; break;
         case 399: return "PARAMS"; break;
         case 398: return "OPTION"; break;
         case 397: return "OR"; break;
         case 396: return "ON"; break;
         case 395: return "OF"; break;
         case 394: return "OBJECTS"; break;
         case 393: return "OBJECT"; break;
         case 392: return "NUTRITION"; break;
         case 391: return "WORD_NULL"; break;
         case 390: return "NPC"; break;
         case 389: return "NORMAL"; break;
         case 388: return "NONE"; break;
         case 387: return "NEAR"; break;
         case 386: return "MTYPE"; break;
         case 385: return "MOVE"; break;
         case 384: return "MONSTER"; break;
         case 383: return "WORD_MIN"; break;
         case 382: return "MEMBER"; break;
         case 381: return "MELEE"; break;
         case 380: return "WORD_MAX"; break;
         case 379: return "MATERIAL"; break;
         case 378: return "MAP"; break;
         case 377: return "MANA"; break;
         case 376: return "MODULE"; break;
         case 375: return "MAX_SIZE"; break;
         case 374: return "LVAL"; break;
         case 373: return "LISTS"; break;
         case 372: return "LIGHT"; break;
         case 371: return "LIFESPAN"; break;
         case 370: return "WORD_LEVEL"; break;
         case 369: return "LDMG"; break;
         case 368: return "ITEM"; break;
         case 367: return "INVOKE"; break;
         case 366: return "INNATE"; break;
         case 365: return "IMMUNE"; break;
         case 364: return "IMAGE"; break;
         case 363: return "IF"; break;
         case 362: return "HIT"; break;
         case 361: return "HITPOINTS"; break;
         case 360: return "HITDICE"; break;
         case 359: return "HEAVY"; break;
         case 358: return "GUILD"; break;
         case 357: return "GROUP"; break;
         case 356: return "GRID"; break;
         case 355: return "GRANTS"; break;
         case 354: return "GOD"; break;
         case 353: return "GLYPH"; break;
         case 352: return "GEAR"; break;
         case 351: return "FUEL"; break;
         case 350: return "FROM"; break;
         case 349: return "FOR"; break;
         case 348: return "WORD_FILE"; break;
         case 347: return "FLOOR"; break;
         case 346: return "FLAGS"; break;
         case 345: return "FIRES"; break;
         case 344: return "FEATURE"; break;
         case 343: return "FEAT"; break;
         case 342: return "FEATS"; break;
         case 341: return "WORD_FALSE"; break;
         case 340: return "FACTOR"; break;
         case 339: return "EVERY"; break;
         case 338: return "EVENT"; break;
         case 337: return "FLAVOR"; break;
         case 336: return "FAVORED"; break;
         case 335: return "EVAL"; break;
         case 334: return "EQUIP"; break;
         case 333: return "EXPORT"; break;
         case 332: return "ELSE"; break;
         case 331: return "EFFECT"; break;
         case 330: return "DVAL"; break;
         case 329: return "DUNGEON"; break;
         case 328: return "DO"; break;
         case 327: return "DISEASE"; break;
         case 326: return "DOMAINS"; break;
         case 325: return "DOMAIN"; break;
         case 324: return "DOOR"; break;
         case 323: return "WORD_DESC"; break;
         case 322: return "DEPTH"; break;
         case 321: return "DEFAULT"; break;
         case 320: return "DEF"; break;
         case 319: return "WORD_DC"; break;
         case 318: return "DAY"; break;
         case 317: return "DARK"; break;
         case 316: return "DMG"; break;
         case 315: return "CVAL"; break;
         case 314: return "CURSED"; break;
         case 313: return "CRIT"; break;
         case 312: return "COST"; break;
         case 311: return "CONTINUE"; break;
         case 310: return "CTYPE"; break;
         case 309: return "COVERAGE"; break;
         case 308: return "CONSTANTS"; break;
         case 307: return "WORD_COLOR"; break;
         case 306: return "CLASS"; break;
         case 305: return "CHANCE"; break;
         case 304: return "WORD_CR"; break;
         case 303: return "CASTER"; break;
         case 302: return "CASE"; break;
         case 301: return "CAPACITY"; break;
         case 300: return "BRIGHT"; break;
         case 299: return "BREAK"; break;
         case 298: return "BRAWL"; break;
         case 297: return "BLESSED"; break;
         case 296: return "BASE"; break;
         case 295: return "AVAL"; break;
         case 294: return "ATTACK"; break;
         case 293: return "AT"; break;
         case 292: return "AS"; break;
         case 291: return "ARTIFACT"; break;
         case 290: return "ARM"; break;
         case 289: return "ARCHERY"; break;
         case 288: return "APTITUDES"; break;
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
