#define NULL 0
typedef long * yy;
#define yyu (-2147483647L)
static yy yynull;
extern yy yyh;
extern yy yyhx;
static yyErr(n,l)
{
yyAbort(n,"flatten", l);
}
extern yy yyglov_LHS_List;
extern yy yyglov_GentleFlag;
extern yy yyglov_NontermList;
extern yy yyglov_PreludeBlock;
grammarcode_rulelist(yyin_1)
yy yyin_1;
{
{
yy yyb;
yy yy_0_1;
yy yyv_H;
yy yy_0_1_1;
yy yyv_T;
yy yy_0_1_2;
yy yy_1_1;
yy yy_2_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 1) goto yyfl_1_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yyv_H = yy_0_1_1;
yyv_T = yy_0_1_2;
yy_1_1 = yyv_H;
grammarcode_rule(yy_1_1);
yy_2_1 = yyv_T;
grammarcode_rulelist(yy_2_1);
return;
yyfl_1_1 : ;
}
{
yy yyb;
yy yy_0_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 2) goto yyfl_1_2;
return;
yyfl_1_2 : ;
}
yyErr(2,36);
}
grammarcode_rule(yyin_1)
yy yyin_1;
{
{
yy yyb;
yy yyv_Rule;
yy yy_0_1;
yy yy_0_1_1;
yy yyv_L;
yy yy_0_1_1_1;
yy yyv_R;
yy yy_0_1_1_2;
yy yyv_RKey;
yy yy_0_1_1_3;
yy yyv_Prelude;
yy yy_0_1_1_4;
yy yyv_Annotation;
yy yy_0_1_1_5;
yy yyv_Pos;
yy yy_0_1_1_6;
yy yy_1_1;
yy yy_1_2;
yy yyv_N;
yy yy_1_2_1;
yy yyv_NKey;
yy yy_1_2_2;
yy yy_1_2_3;
yy yy_2_1;
yy yy_2_2;
yy yy_2_3;
yy yy_2_4;
yy yy_2_5;
yy yy_2_6;
yy yy_2_7;
yy yyv_RulesForNonterm;
yy yy_3;
yy yy_4;
yy yy_4_1;
yy yy_4_2;
yy yy_5_1;
yy yy_7_1;
yy_0_1 = yyin_1;
yy_0_1_1 = yy_0_1;
yyv_Rule = yy_0_1;
if (yy_0_1_1[0] != 1) goto yyfl_2_1;
yy_0_1_1_1 = ((yy)yy_0_1_1[1]);
yy_0_1_1_2 = ((yy)yy_0_1_1[2]);
yy_0_1_1_3 = ((yy)yy_0_1_1[3]);
yy_0_1_1_4 = ((yy)yy_0_1_1[4]);
yy_0_1_1_5 = ((yy)yy_0_1_1[5]);
yy_0_1_1_6 = ((yy)yy_0_1_1[6]);
yyv_L = yy_0_1_1_1;
yyv_R = yy_0_1_1_2;
yyv_RKey = yy_0_1_1_3;
yyv_Prelude = yy_0_1_1_4;
yyv_Annotation = yy_0_1_1_5;
yyv_Pos = yy_0_1_1_6;
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yy_1_1 = yyv_L;
if (! HasMeaning(yy_1_1, &yy_1_2)) goto yyfl_2_1;
if (yy_1_2[0] != 2) goto yyfl_2_1;
yy_1_2_1 = ((yy)yy_1_2[1]);
yy_1_2_2 = ((yy)yy_1_2[2]);
yy_1_2_3 = ((yy)yy_1_2[3]);
yyv_N = yy_1_2_1;
yyv_NKey = yy_1_2_2;
yy_2_1 = yyv_L;
yy_2_2 = yyv_NKey;
yy_2_3 = yyv_N;
yy_2_4 = yyv_RKey;
yy_2_5 = yyv_Prelude;
yy_2_6 = yyv_Annotation;
yy_2_7 = yyv_Pos;
START_RULE(yy_2_1, yy_2_2, yy_2_3, yy_2_4, yy_2_5, yy_2_6, yy_2_7);
yy_3 = (yy) yyv_NKey[1];
if (yy_3 == (yy) yyu) yyErr(1,56);
yyv_RulesForNonterm = yy_3;
yy_4_1 = yyv_RulesForNonterm;
yy_4_2 = yyv_Rule;
yy_4 = yyb + 0;
yy_4[0] = 1;
yy_4[1] = ((long)yy_4_1);
yy_4[2] = ((long)yy_4_2);
yyv_NKey[1] = (long) yy_4;
yy_5_1 = yyv_R;
grammarcode_memberlist(yy_5_1);
END_RULE();
yy_7_1 = yyv_R;
subphrasecode_memberlist(yy_7_1);
return;
yyfl_2_1 : ;
}
{
yy yyb;
yy yyv_Rule;
yy yy_0_1;
yy yy_0_1_1;
yy yyv_L;
yy yy_0_1_1_1;
yy yyv_RhsList;
yy yy_0_1_1_2;
yy yyv_Prelude;
yy yy_0_1_1_3;
yy yyv_Pos;
yy yy_0_1_1_4;
yy yy_1_1;
yy yy_1_2;
yy yyv_Rhs;
yy yy_1_2_1;
yy yyv_Tl;
yy yy_1_2_2;
yy yy_2_1;
yy yy_2_2;
yy yyv_R;
yy yy_2_2_1;
yy yyv_RKey;
yy yy_2_2_2;
yy yyv_Annotation;
yy yy_2_2_3;
yy yyv_RhsPos;
yy yy_2_2_4;
yy yy_3_1;
yy yy_3_2;
yy yyv_N;
yy yy_3_2_1;
yy yyv_NKey;
yy yy_3_2_2;
yy yy_3_2_3;
yy yy_4_1;
yy yy_4_2;
yy yy_4_3;
yy yy_4_4;
yy yy_4_5;
yy yy_4_6;
yy yy_4_7;
yy yyv_RulesForNonterm;
yy yy_5;
yy yy_6;
yy yy_6_1;
yy yy_6_2;
yy yy_7_1;
yy yy_9_1;
yy yy_10_1;
yy yy_10_2;
yy yy_10_3;
yy_0_1 = yyin_1;
yy_0_1_1 = yy_0_1;
yyv_Rule = yy_0_1;
if (yy_0_1_1[0] != 2) goto yyfl_2_2;
yy_0_1_1_1 = ((yy)yy_0_1_1[1]);
yy_0_1_1_2 = ((yy)yy_0_1_1[2]);
yy_0_1_1_3 = ((yy)yy_0_1_1[3]);
yy_0_1_1_4 = ((yy)yy_0_1_1[4]);
yyv_L = yy_0_1_1_1;
yyv_RhsList = yy_0_1_1_2;
yyv_Prelude = yy_0_1_1_3;
yyv_Pos = yy_0_1_1_4;
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yy_1_1 = yyv_RhsList;
yy_1_2 = yy_1_1;
if (yy_1_2[0] != 1) goto yyfl_2_2;
yy_1_2_1 = ((yy)yy_1_2[1]);
yy_1_2_2 = ((yy)yy_1_2[2]);
yyv_Rhs = yy_1_2_1;
yyv_Tl = yy_1_2_2;
yy_2_1 = yyv_Rhs;
yy_2_2 = yy_2_1;
if (yy_2_2[0] != 1) goto yyfl_2_2;
yy_2_2_1 = ((yy)yy_2_2[1]);
yy_2_2_2 = ((yy)yy_2_2[2]);
yy_2_2_3 = ((yy)yy_2_2[3]);
yy_2_2_4 = ((yy)yy_2_2[4]);
yyv_R = yy_2_2_1;
yyv_RKey = yy_2_2_2;
yyv_Annotation = yy_2_2_3;
yyv_RhsPos = yy_2_2_4;
yy_3_1 = yyv_L;
if (! HasMeaning(yy_3_1, &yy_3_2)) goto yyfl_2_2;
if (yy_3_2[0] != 2) goto yyfl_2_2;
yy_3_2_1 = ((yy)yy_3_2[1]);
yy_3_2_2 = ((yy)yy_3_2[2]);
yy_3_2_3 = ((yy)yy_3_2[3]);
yyv_N = yy_3_2_1;
yyv_NKey = yy_3_2_2;
yy_4_1 = yyv_L;
yy_4_2 = yyv_NKey;
yy_4_3 = yyv_N;
yy_4_4 = yyv_RKey;
yy_4_5 = yyv_Prelude;
yy_4_6 = yyv_Annotation;
yy_4_7 = yyv_RhsPos;
START_RULE(yy_4_1, yy_4_2, yy_4_3, yy_4_4, yy_4_5, yy_4_6, yy_4_7);
yy_5 = (yy) yyv_NKey[1];
if (yy_5 == (yy) yyu) yyErr(1,67);
yyv_RulesForNonterm = yy_5;
yy_6_1 = yyv_RulesForNonterm;
yy_6_2 = yyv_Rule;
yy_6 = yyb + 0;
yy_6[0] = 1;
yy_6[1] = ((long)yy_6_1);
yy_6[2] = ((long)yy_6_2);
yyv_NKey[1] = (long) yy_6;
yy_7_1 = yyv_R;
grammarcode_memberlist(yy_7_1);
END_RULE();
yy_9_1 = yyv_R;
subphrasecode_memberlist(yy_9_1);
yy_10_1 = yyv_L;
yy_10_2 = yyv_Prelude;
yy_10_3 = yyv_Tl;
grammarcode_rhslist(yy_10_1, yy_10_2, yy_10_3);
return;
yyfl_2_2 : ;
}
yyErr(2,51);
}
grammarcode_rhslist(yyin_1, yyin_2, yyin_3)
yy yyin_1;
yy yyin_2;
yy yyin_3;
{
{
yy yyv_L;
yy yy_0_1;
yy yyv_Prelude;
yy yy_0_2;
yy yy_0_3;
yy yyv_Hd;
yy yy_0_3_1;
yy yyv_Tl;
yy yy_0_3_2;
yy yy_1_1;
yy yy_1_2;
yy yy_1_3;
yy yy_2_1;
yy yy_2_2;
yy yy_2_3;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yy_0_3 = yyin_3;
yyv_L = yy_0_1;
yyv_Prelude = yy_0_2;
if (yy_0_3[0] != 1) goto yyfl_3_1;
yy_0_3_1 = ((yy)yy_0_3[1]);
yy_0_3_2 = ((yy)yy_0_3[2]);
yyv_Hd = yy_0_3_1;
yyv_Tl = yy_0_3_2;
yy_1_1 = yyv_L;
yy_1_2 = yyv_Prelude;
yy_1_3 = yyv_Hd;
grammarcode_rhs(yy_1_1, yy_1_2, yy_1_3);
yy_2_1 = yyv_L;
yy_2_2 = yyv_Prelude;
yy_2_3 = yyv_Tl;
grammarcode_rhslist(yy_2_1, yy_2_2, yy_2_3);
return;
yyfl_3_1 : ;
}
{
yy yyv_L;
yy yy_0_1;
yy yyv_Prelude;
yy yy_0_2;
yy yy_0_3;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yy_0_3 = yyin_3;
yyv_L = yy_0_1;
yyv_Prelude = yy_0_2;
if (yy_0_3[0] != 2) goto yyfl_3_2;
return;
yyfl_3_2 : ;
}
yyErr(2,75);
}
grammarcode_rhs(yyin_1, yyin_2, yyin_3)
yy yyin_1;
yy yyin_2;
yy yyin_3;
{
{
yy yyv_L;
yy yy_0_1;
yy yyv_Prelude;
yy yy_0_2;
yy yyv_Rhs;
yy yy_0_3;
yy yy_1_1;
yy yy_1_2;
yy yyv_R;
yy yy_1_2_1;
yy yyv_RKey;
yy yy_1_2_2;
yy yyv_Annotation;
yy yy_1_2_3;
yy yyv_RhsPos;
yy yy_1_2_4;
yy yy_2_1;
yy yy_2_2;
yy yyv_N;
yy yy_2_2_1;
yy yyv_NKey;
yy yy_2_2_2;
yy yy_2_2_3;
yy yy_3_1;
yy yy_3_2;
yy yy_3_3;
yy yy_3_4;
yy yy_3_5;
yy yy_3_6;
yy yy_3_7;
yy yy_4_1;
yy yy_6_1;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yy_0_3 = yyin_3;
yyv_L = yy_0_1;
yyv_Prelude = yy_0_2;
yyv_Rhs = yy_0_3;
yy_1_1 = yyv_Rhs;
yy_1_2 = yy_1_1;
if (yy_1_2[0] != 1) goto yyfl_4_1;
yy_1_2_1 = ((yy)yy_1_2[1]);
yy_1_2_2 = ((yy)yy_1_2[2]);
yy_1_2_3 = ((yy)yy_1_2[3]);
yy_1_2_4 = ((yy)yy_1_2[4]);
yyv_R = yy_1_2_1;
yyv_RKey = yy_1_2_2;
yyv_Annotation = yy_1_2_3;
yyv_RhsPos = yy_1_2_4;
yy_2_1 = yyv_L;
if (! HasMeaning(yy_2_1, &yy_2_2)) goto yyfl_4_1;
if (yy_2_2[0] != 2) goto yyfl_4_1;
yy_2_2_1 = ((yy)yy_2_2[1]);
yy_2_2_2 = ((yy)yy_2_2[2]);
yy_2_2_3 = ((yy)yy_2_2[3]);
yyv_N = yy_2_2_1;
yyv_NKey = yy_2_2_2;
yy_3_1 = yyv_L;
yy_3_2 = yyv_NKey;
yy_3_3 = yyv_N;
yy_3_4 = yyv_RKey;
yy_3_5 = yyv_Prelude;
yy_3_6 = yyv_Annotation;
yy_3_7 = yyv_RhsPos;
START_RULE(yy_3_1, yy_3_2, yy_3_3, yy_3_4, yy_3_5, yy_3_6, yy_3_7);
yy_4_1 = yyv_R;
grammarcode_memberlist(yy_4_1);
END_RULE();
yy_6_1 = yyv_R;
subphrasecode_memberlist(yy_6_1);
return;
yyfl_4_1 : ;
}
yyErr(2,80);
}
grammarcode_memberlist(yyin_1)
yy yyin_1;
{
{
yy yy_0_1;
yy yyv_H;
yy yy_0_1_1;
yy yyv_T;
yy yy_0_1_2;
yy yy_1_1;
yy yy_2_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 1) goto yyfl_5_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yyv_H = yy_0_1_1;
yyv_T = yy_0_1_2;
yy_1_1 = yyv_H;
grammarcode_member(yy_1_1);
yy_2_1 = yyv_T;
grammarcode_memberlist(yy_2_1);
return;
yyfl_5_1 : ;
}
{
yy yy_0_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 2) goto yyfl_5_2;
return;
yyfl_5_2 : ;
}
yyErr(2,92);
}
grammarcode_member(yyin_1)
yy yyin_1;
{
{
yy yyb = NULL;
yy yy_0_1 = NULL;
yy yyv_Id = NULL;
yy yy_0_1_1 = NULL;
yy yyv_Params = NULL;
yy yy_0_1_2 = NULL;
yy yyv_Annotation = NULL;
yy yy_0_1_3 = NULL;
yy yyv_Pos = NULL;
yy yy_0_1_4 = NULL;
yy yy_1_1_1_1 = NULL;
yy yyv_Obj = NULL;
yy yy_1_1_1_2 = NULL;
yy yy_1_1_2_1_1_1 = NULL;
yy yy_1_1_2_1_1_2 = NULL;
yy yyv_N = NULL;
yy yy_1_1_2_1_1_2_1 = NULL;
yy yy_1_1_2_1_1_2_2 = NULL;
yy yy_1_1_2_1_2_1 = NULL;
yy yy_1_1_2_2_1_1 = NULL;
yy yy_1_1_2_2_1_2 = NULL;
yy yy_1_1_2_2_1_2_1 = NULL;
yy yyv_NKey = NULL;
yy yy_1_1_2_2_1_2_2 = NULL;
yy yy_1_1_2_2_1_2_3 = NULL;
yy yy_1_1_2_2_2_1 = NULL;
yy yy_1_1_2_2_2_2 = NULL;
yy yy_1_1_2_2_2_3 = NULL;
yy yy_1_1_2_2_2_4 = NULL;
yy yy_1_2_1_1 = NULL;
yy yy_1_2_1_2 = NULL;
yy yy_1_2_1_3 = NULL;
yy yy_1_2_1_4 = NULL;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 1) goto yyfl_6_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yy_0_1_3 = ((yy)yy_0_1[3]);
yy_0_1_4 = ((yy)yy_0_1[4]);
yyv_Id = yy_0_1_1;
yyv_Params = yy_0_1_2;
yyv_Annotation = yy_0_1_3;
yyv_Pos = yy_0_1_4;
{
yy yysb = yyb;
yy_1_1_1_1 = yyv_Id;
if (! HasMeaning(yy_1_1_1_1, &yy_1_1_1_2)) goto yyfl_6_1_1_1;
yyv_Obj = yy_1_1_1_2;
{
yy yysb = yyb;
yy_1_1_2_1_1_1 = yyv_Obj;
yy_1_1_2_1_1_2 = yy_1_1_2_1_1_1;
if (yy_1_1_2_1_1_2[0] != 1) goto yyfl_6_1_1_1_2_1;
yy_1_1_2_1_1_2_1 = ((yy)yy_1_1_2_1_1_2[1]);
yy_1_1_2_1_1_2_2 = ((yy)yy_1_1_2_1_1_2[2]);
yyv_N = yy_1_1_2_1_1_2_1;
yy_1_1_2_1_2_1 = yyv_N;
ADD_TOKEN_MEMBER(yy_1_1_2_1_2_1);
goto yysl_6_1_1_1_2;
yyfl_6_1_1_1_2_1 : ;
yy_1_1_2_2_1_1 = yyv_Obj;
yy_1_1_2_2_1_2 = yy_1_1_2_2_1_1;
if (yy_1_1_2_2_1_2[0] != 2) goto yyfl_6_1_1_1_2_2;
yy_1_1_2_2_1_2_1 = ((yy)yy_1_1_2_2_1_2[1]);
yy_1_1_2_2_1_2_2 = ((yy)yy_1_1_2_2_1_2[2]);
yy_1_1_2_2_1_2_3 = ((yy)yy_1_1_2_2_1_2[3]);
yyv_N = yy_1_1_2_2_1_2_1;
yyv_NKey = yy_1_1_2_2_1_2_2;
yy_1_1_2_2_2_1 = yyv_Id;
yy_1_1_2_2_2_2 = yyv_N;
yy_1_1_2_2_2_3 = yyv_Annotation;
yy_1_1_2_2_2_4 = yyv_Pos;
ADD_NONTERM_MEMBER(yy_1_1_2_2_2_1, yy_1_1_2_2_2_2, yy_1_1_2_2_2_3, yy_1_1_2_2_2_4);
goto yysl_6_1_1_1_2;
yyfl_6_1_1_1_2_2 : ;
goto yyfl_6_1_1_1;
yysl_6_1_1_1_2 : ;
yyb = yysb;
}
goto yysl_6_1_1;
yyfl_6_1_1_1 : ;
yy_1_2_1_1 = ((yy)"'");
yy_1_2_1_2 = yyv_Id;
yy_1_2_1_3 = ((yy)"' has not been declared");
yy_1_2_1_4 = yyv_Pos;
ErrorI(yy_1_2_1_1, yy_1_2_1_2, yy_1_2_1_3, yy_1_2_1_4);
goto yysl_6_1_1;
yysl_6_1_1 : ;
yyb = yysb;
}
return;
yyfl_6_1 : ;
}
{
yy yyb;
yy yy_0_1;
yy yyv_N;
yy yy_0_1_1;
yy yy_1_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 2) goto yyfl_6_2;
yy_0_1_1 = ((yy)yy_0_1[1]);
yyv_N = yy_0_1_1;
yy_1_1 = yyv_N;
ADD_TOKEN_MEMBER(yy_1_1);
return;
yyfl_6_2 : ;
}
{
yy yy_0_1;
yy yyv_Kind;
yy yy_0_1_1;
yy yyv_L;
yy yy_0_1_2;
yy yyv_Key;
yy yy_0_1_3;
yy yyv_Annotation;
yy yy_0_1_4;
yy yyv_Coordinate;
yy yy_0_1_5;
yy yy_1_1;
yy yy_1_2;
yy yy_1_3;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 3) goto yyfl_6_3;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yy_0_1_3 = ((yy)yy_0_1[3]);
yy_0_1_4 = ((yy)yy_0_1[4]);
yy_0_1_5 = ((yy)yy_0_1[5]);
yyv_Kind = yy_0_1_1;
yyv_L = yy_0_1_2;
yyv_Key = yy_0_1_3;
yyv_Annotation = yy_0_1_4;
yyv_Coordinate = yy_0_1_5;
yy_1_1 = yyv_Key;
yy_1_2 = yyv_Annotation;
yy_1_3 = yyv_Coordinate;
ADD_SUBPHRASE_REFERENCE(yy_1_1, yy_1_2, yy_1_3);
return;
yyfl_6_3 : ;
}
{
yy yy_0_1;
yy yy_0_1_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 4) goto yyfl_6_4;
yy_0_1_1 = ((yy)yy_0_1[1]);
return;
yyfl_6_4 : ;
}
yyErr(2,98);
}
innercode_alternative(yyin_1, yyin_2, yyin_3)
yy yyin_1;
yy yyin_2;
yy yyin_3;
{
{
yy yyb;
yy yy_0_1;
yy yyv_L;
yy yy_0_1_1;
yy yyv_Key;
yy yy_0_1_2;
yy yyv_Annotation;
yy yy_0_1_3;
yy yyv_Pos;
yy yy_0_1_4;
yy yyv_Kind;
yy yy_0_2;
yy yyv_Subphrase;
yy yy_0_3;
yy yyv_N;
yy yy_1;
yy yy_2_1;
yy yy_2_2;
yy yy_2_3;
yy yy_2_3_1;
yy yy_2_4;
yy yy_2_5;
yy yy_3_1;
yy yy_4_1;
yy yy_4_2;
yy yy_4_3;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yy_0_3 = yyin_3;
if (yy_0_1[0] != 1) goto yyfl_7_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yy_0_1_3 = ((yy)yy_0_1[3]);
yy_0_1_4 = ((yy)yy_0_1[4]);
yyv_L = yy_0_1_1;
yyv_Key = yy_0_1_2;
yyv_Annotation = yy_0_1_3;
yyv_Pos = yy_0_1_4;
yyv_Kind = yy_0_2;
yyv_Subphrase = yy_0_3;
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
yy_1 = (yy) yyv_Key[2];
if (yy_1 == (yy) yyu) yyErr(1,125);
yyv_N = yy_1;
yy_2_1 = yyv_Subphrase;
yy_2_2 = yyv_Key;
yy_2_3_1 = yyv_L;
yy_2_3 = yyb + 0;
yy_2_3[0] = 1;
yy_2_3[1] = ((long)yy_2_3_1);
yy_2_4 = yyv_Annotation;
yy_2_5 = yyv_Pos;
START_ALTERNATIVE(yy_2_1, yy_2_2, yy_2_3, yy_2_4, yy_2_5);
yy_3_1 = yyv_L;
grammarcode_memberlist(yy_3_1);
yy_4_1 = yyv_Kind;
yy_4_2 = yyv_Subphrase;
yy_4_3 = yyv_Pos;
innercode_recursive_call(yy_4_1, yy_4_2, yy_4_3);
END_ALTERNATIVE();
return;
yyfl_7_1 : ;
}
yyErr(2,122);
}
innercode_recursive_call(yyin_1, yyin_2, yyin_3)
yy yyin_1;
yy yyin_2;
yy yyin_3;
{
{
yy yy_0_1;
yy yyv_MemberAnno;
yy yy_0_1_1;
yy yyv_Subphrase;
yy yy_0_2;
yy yyv_Coordinate;
yy yy_0_3;
yy yy_1_1;
yy yy_1_2;
yy yy_1_3;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yy_0_3 = yyin_3;
if (yy_0_1[0] != 2) goto yyfl_8_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yyv_MemberAnno = yy_0_1_1;
yyv_Subphrase = yy_0_2;
yyv_Coordinate = yy_0_3;
yy_1_1 = yyv_Subphrase;
yy_1_2 = yyv_MemberAnno;
yy_1_3 = yyv_Coordinate;
ADD_SUBPHRASE_REFERENCE(yy_1_1, yy_1_2, yy_1_3);
return;
yyfl_8_1 : ;
}
{
yy yy_0_1;
yy yy_0_2;
yy yy_0_3;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yy_0_3 = yyin_3;
return;
//yyfl_8_2 : ;
}
yyErr(2,134);
}
innercode_empty_rule(yyin_1, yyin_2, yyin_3, yyin_4)
yy yyin_1;
yy yyin_2;
yy yyin_3;
yy yyin_4;
{
{
yy yyb = NULL;
yy yyv_Kind = NULL;
yy yy_0_1 = NULL;
yy yyv_Subphrase = NULL;
yy yy_0_2 = NULL;
yy yyv_Annotation = NULL;
yy yy_0_3 = NULL;
yy yyv_Pos = NULL;
yy yy_0_4 = NULL;
yy yy_1_1_1_1_1_1 = NULL;
yy yy_1_1_1_1_1_2 = NULL;
yy yy_1_1_1_2_1_1 = NULL;
yy yy_1_1_1_2_1_2 = NULL;
yy yyv_Key = NULL;
yy yyv_N = NULL;
yy yy_1_1_3_1 = NULL;
yy yy_1_1_4 = NULL;
yy yy_1_1_5_1 = NULL;
yy yy_1_1_5_2 = NULL;
yy yy_1_1_5_3 = NULL;
yy yy_1_1_5_4 = NULL;
yy yy_1_1_5_5 = NULL;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yy_0_3 = yyin_3;
yy_0_4 = yyin_4;
yyv_Kind = yy_0_1;
yyv_Subphrase = yy_0_2;
yyv_Annotation = yy_0_3;
yyv_Pos = yy_0_4;
{
yy yysb = yyb;
yyb = yyh;
yyh += 6; if (yyh > yyhx) yyExtend();
{
yy yysb = yyb;
yy_1_1_1_1_1_1 = yyv_Kind;
yy_1_1_1_1_1_2 = yy_1_1_1_1_1_1;
if (yy_1_1_1_1_1_2[0] != 1) goto yyfl_9_1_1_1_1_1;
goto yysl_9_1_1_1_1;
yyfl_9_1_1_1_1_1 : ;
yy_1_1_1_2_1_1 = yyv_Kind;
yy_1_1_1_2_1_2 = yy_1_1_1_2_1_1;
if (yy_1_1_1_2_1_2[0] != 2) goto yyfl_9_1_1_1_1_2;
goto yysl_9_1_1_1_1;
yyfl_9_1_1_1_1_2 : ;
goto yyfl_9_1_1_1;
yysl_9_1_1_1_1 : ;
yyb = yysb;
}
yyv_Key = yyb + 0;
yyb[4] = yyu;
yyb[3] = yyu;
yyb[2] = yyu;
yyb[1] = yyu;
FreshAlternativeNumber(&yy_1_1_3_1);
yyv_N = yy_1_1_3_1;
yy_1_1_4 = yyv_N;
yyv_Key[2] = (long) yy_1_1_4;
yy_1_1_5_1 = yyv_Subphrase;
yy_1_1_5_2 = yyv_Key;
yy_1_1_5_3 = yyb + 5;
yy_1_1_5_3[0] = 2;
yy_1_1_5_4 = yyv_Annotation;
yy_1_1_5_5 = yyv_Pos;
START_ALTERNATIVE(yy_1_1_5_1, yy_1_1_5_2, yy_1_1_5_3, yy_1_1_5_4, yy_1_1_5_5);
END_ALTERNATIVE();
goto yysl_9_1_1;
yyfl_9_1_1_1 : ;
goto yysl_9_1_1;
yysl_9_1_1 : ;
yyb = yysb;
}
return;
}
}
subphrasecode_memberlist(yyin_1)
yy yyin_1;
{
{
yy yy_0_1;
yy yyv_H;
yy yy_0_1_1;
yy yyv_T;
yy yy_0_1_2;
yy yy_1_1;
yy yy_2_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 1) goto yyfl_10_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yyv_H = yy_0_1_1;
yyv_T = yy_0_1_2;
yy_1_1 = yyv_H;
subphrasecode_member(yy_1_1);
yy_2_1 = yyv_T;
subphrasecode_memberlist(yy_2_1);
return;
yyfl_10_1 : ;
}
{
yy yy_0_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 2) goto yyfl_10_2;
return;
yyfl_10_2 : ;
}
yyErr(2,164);
}
subphrasecode_member(yyin_1)
yy yyin_1;
{
{
yy yy_0_1;
yy yyv_Id;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yy_0_1_4;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 1) goto yyfl_11_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yy_0_1_3 = ((yy)yy_0_1[3]);
yy_0_1_4 = ((yy)yy_0_1[4]);
yyv_Id = yy_0_1_1;
return;
yyfl_11_1 : ;
}
{
yy yy_0_1;
yy yyv_N;
yy yy_0_1_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 2) goto yyfl_11_2;
yy_0_1_1 = ((yy)yy_0_1[1]);
yyv_N = yy_0_1_1;
return;
yyfl_11_2 : ;
}
{
yy yy_0_1;
yy yyv_Kind;
yy yy_0_1_1;
yy yyv_L;
yy yy_0_1_2;
yy yyv_SubphraseKey;
yy yy_0_1_3;
yy yyv_Annotation;
yy yy_0_1_4;
yy yyv_Coordinate;
yy yy_0_1_5;
yy yy_1_1;
yy yy_1_2;
yy yy_1_3;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 3) goto yyfl_11_3;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yy_0_1_3 = ((yy)yy_0_1[3]);
yy_0_1_4 = ((yy)yy_0_1[4]);
yy_0_1_5 = ((yy)yy_0_1[5]);
yyv_Kind = yy_0_1_1;
yyv_L = yy_0_1_2;
yyv_SubphraseKey = yy_0_1_3;
yyv_Annotation = yy_0_1_4;
yyv_Coordinate = yy_0_1_5;
yy_1_1 = yyv_L;
yy_1_2 = yyv_Kind;
yy_1_3 = yyv_SubphraseKey;
subphrasecode_alternativelist(yy_1_1, yy_1_2, yy_1_3);
return;
yyfl_11_3 : ;
}
{
yy yy_0_1;
yy yy_0_1_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 4) goto yyfl_11_4;
yy_0_1_1 = ((yy)yy_0_1[1]);
return;
yyfl_11_4 : ;
}
yyErr(2,170);
}
subphrasecode_alternativelist(yyin_1, yyin_2, yyin_3)
yy yyin_1;
yy yyin_2;
yy yyin_3;
{
{
yy yy_0_1;
yy yyv_H;
yy yy_0_1_1;
yy yyv_T;
yy yy_0_1_2;
yy yyv_Kind;
yy yy_0_2;
yy yyv_SubphraseKey;
yy yy_0_3;
yy yy_1_1;
yy yy_1_2;
yy yy_1_3;
yy yy_2_1;
yy yy_3_1;
yy yy_3_2;
yy yy_3_3;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yy_0_3 = yyin_3;
if (yy_0_1[0] != 1) goto yyfl_12_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yyv_H = yy_0_1_1;
yyv_T = yy_0_1_2;
yyv_Kind = yy_0_2;
yyv_SubphraseKey = yy_0_3;
yy_1_1 = yyv_H;
yy_1_2 = yyv_Kind;
yy_1_3 = yyv_SubphraseKey;
innercode_alternative(yy_1_1, yy_1_2, yy_1_3);
yy_2_1 = yyv_H;
subphrasecode_alternative(yy_2_1);
yy_3_1 = yyv_T;
yy_3_2 = yyv_Kind;
yy_3_3 = yyv_SubphraseKey;
subphrasecode_alternativelist(yy_3_1, yy_3_2, yy_3_3);
return;
yyfl_12_1 : ;
}
{
yy yy_0_1;
yy yyv_Annotation;
yy yy_0_1_1;
yy yyv_Pos;
yy yy_0_1_2;
yy yyv_Kind;
yy yy_0_2;
yy yyv_SubphraseKey;
yy yy_0_3;
yy yy_1_1;
yy yy_1_2;
yy yy_1_3;
yy yy_1_4;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yy_0_3 = yyin_3;
if (yy_0_1[0] != 2) goto yyfl_12_2;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yyv_Annotation = yy_0_1_1;
yyv_Pos = yy_0_1_2;
yyv_Kind = yy_0_2;
yyv_SubphraseKey = yy_0_3;
yy_1_1 = yyv_Kind;
yy_1_2 = yyv_SubphraseKey;
yy_1_3 = yyv_Annotation;
yy_1_4 = yyv_Pos;
innercode_empty_rule(yy_1_1, yy_1_2, yy_1_3, yy_1_4);
return;
yyfl_12_2 : ;
}
yyErr(2,177);
}
subphrasecode_alternative(yyin_1)
yy yyin_1;
{
{
yy yy_0_1;
yy yyv_L;
yy yy_0_1_1;
yy yyv_Key;
yy yy_0_1_2;
yy yyv_Annotation;
yy yy_0_1_3;
yy yyv_Pos;
yy yy_0_1_4;
yy yy_1_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 1) goto yyfl_13_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yy_0_1_3 = ((yy)yy_0_1[3]);
yy_0_1_4 = ((yy)yy_0_1[4]);
yyv_L = yy_0_1_1;
yyv_Key = yy_0_1_2;
yyv_Annotation = yy_0_1_3;
yyv_Pos = yy_0_1_4;
yy_1_1 = yyv_L;
subphrasecode_memberlist(yy_1_1);
return;
yyfl_13_1 : ;
}
yyErr(2,186);
}
