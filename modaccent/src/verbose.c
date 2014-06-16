typedef long * yy;
#define yyu (-2147483647L)
static yy yynull;
extern yy yyh;
extern yy yyhx;
static yyErr(n,l)
{
yyAbort(n,"verbose", l);
}
extern yy yyglov_LHS_List;
extern yy yyglov_GentleFlag;
extern yy yyglov_NontermList;
extern yy yyglov_PreludeBlock;
yy yyglov_IndentationLevel = (yy) yyu;
put_grammar()
{
{
yy yyb;
yy yy_1_1;
yy yyv_Keys;
yy yy_3;
yy yy_4_1;
yy_1_1 = ((yy)"gram.v");
OpenOutput(yy_1_1);
init_indentation();
yy_3 = yyglov_LHS_List;
if (yy_3 == (yy) yyu) yyErr(1,39);
yyv_Keys = yy_3;
yy_4_1 = yyv_Keys;
put_rules_LHSKEYS(yy_4_1);
CloseOutput();
return;
}
}
put_rules_LHSKEYS(yyin_1)
yy yyin_1;
{
{
yy yyb;
yy yy_0_1;
yy yyv_Keys;
yy yy_0_1_1;
yy yyv_Key;
yy yy_0_1_2;
yy yy_1_1;
yy yy_2_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 1) goto yyfl_2_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yyv_Keys = yy_0_1_1;
yyv_Key = yy_0_1_2;
yy_1_1 = yyv_Keys;
put_rules_LHSKEYS(yy_1_1);
yy_2_1 = yyv_Key;
put_rules_LHSKEY(yy_2_1);
return;
yyfl_2_1 : ;
}
{
yy yyb;
yy yy_0_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 2) goto yyfl_2_2;
return;
yyfl_2_2 : ;
}
yyErr(2,43);
}
put_rules_LHSKEY(yyin_1)
yy yyin_1;
{
{
yy yyb;
yy yyv_Key;
yy yy_0_1;
yy yyv_Lhs;
yy yy_1;
yy yy_2_1_1_1;
yy yy_2_1_1_2;
yy yyv_Id;
yy yy_2_1_1_2_1;
yy yy_2_1_2_1;
yy yyv_Str;
yy yy_2_1_2_2;
yy yy_2_1_3_1;
yy yy_2_1_4_1;
yy yyv_Clauses;
yy yy_2_1_6;
yy yy_2_1_7_1;
yy yy_2_1_7_1_1;
yy yy_2_1_8_1;
yy yy_2_1_9_1;
yy yy_2_1_9_1_1;
yy_0_1 = yyin_1;
yyv_Key = yy_0_1;
yy_1 = (yy) yyv_Key[1];
if (yy_1 == (yy) yyu) yyErr(1,52);
yyv_Lhs = yy_1;
{
yy yysb = yyb;
yy_2_1_1_1 = yyv_Lhs;
yy_2_1_1_2 = yy_2_1_1_1;
if (yy_2_1_1_2[0] != 1) goto yyfl_3_1_2_1;
yy_2_1_1_2_1 = ((yy)yy_2_1_1_2[1]);
yyv_Id = yy_2_1_1_2_1;
yy_2_1_2_1 = yyv_Id;
id_to_string(yy_2_1_2_1, &yy_2_1_2_2);
yyv_Str = yy_2_1_2_2;
yy_2_1_3_1 = yyv_Str;
Put(yy_2_1_3_1);
yy_2_1_4_1 = ((yy)":");
Put(yy_2_1_4_1);
Nl();
yy_2_1_6 = (yy) yyv_Key[3];
if (yy_2_1_6 == (yy) yyu) yyErr(1,64);
yyv_Clauses = yy_2_1_6;
yy_2_1_7_1_1 = ((yy)1);
yy_2_1_7_1 = (yy)(((long)yy_2_1_7_1_1));
indentation(yy_2_1_7_1);
yy_2_1_8_1 = yyv_Clauses;
put_rules_for_nonterm_CLAUSEKEYS(yy_2_1_8_1);
yy_2_1_9_1_1 = ((yy)1);
yy_2_1_9_1 = (yy)(-((long)yy_2_1_9_1_1));
indentation(yy_2_1_9_1);
goto yysl_3_1_2;
yyfl_3_1_2_1 : ;
goto yysl_3_1_2;
yysl_3_1_2 : ;
yyb = yysb;
}
return;
}
}
put_rules_for_nonterm_CLAUSEKEYS(yyin_1)
yy yyin_1;
{
{
yy yyb;
yy yy_0_1;
yy yyv_Keys;
yy yy_0_1_1;
yy yyv_Key;
yy yy_0_1_2;
yy yy_1_1;
yy yy_2_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 1) goto yyfl_4_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yyv_Keys = yy_0_1_1;
yyv_Key = yy_0_1_2;
yy_1_1 = yyv_Keys;
put_rules_for_nonterm_CLAUSEKEYS(yy_1_1);
yy_2_1 = yyv_Key;
put_rules_for_nonterm_CLAUSEKEY(yy_2_1);
return;
yyfl_4_1 : ;
}
{
yy yyb;
yy yy_0_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 2) goto yyfl_4_2;
return;
yyfl_4_2 : ;
}
yyErr(2,70);
}
put_rules_for_nonterm_CLAUSEKEY(yyin_1)
yy yyin_1;
{
{
yy yyb;
yy yyv_ClauseKey;
yy yy_0_1;
yy yy_1;
yy yyv_Members;
yy yy_1_1;
yy yy_3_1;
yy yyv_Pos;
yy yy_4;
yy yy_5_1;
yy yy_6_1;
yy yy_7_1;
yy yy_9_1;
yy yy_9_1_1;
yy yy_10_1;
yy yy_11_1;
yy yy_11_1_1;
yy_0_1 = yyin_1;
yyv_ClauseKey = yy_0_1;
yy_1 = (yy) yyv_ClauseKey[2];
if (yy_1 == (yy) yyu) yyErr(1,78);
if (yy_1[0] != 1) goto yyfl_5_1;
yy_1_1 = ((yy)yy_1[1]);
yyv_Members = yy_1_1;
indent();
yy_3_1 = ((yy)"<rule>");
Put(yy_3_1);
yy_4 = (yy) yyv_ClauseKey[3];
if (yy_4 == (yy) yyu) yyErr(1,85);
yyv_Pos = yy_4;
yy_5_1 = ((yy)"  [");
Put(yy_5_1);
yy_6_1 = yyv_Pos;
PutI(yy_6_1);
yy_7_1 = ((yy)"]");
Put(yy_7_1);
Nl();
yy_9_1_1 = ((yy)1);
yy_9_1 = (yy)(((long)yy_9_1_1));
indentation(yy_9_1);
yy_10_1 = yyv_Members;
put_rules_MEMBERLIST(yy_10_1);
yy_11_1_1 = ((yy)1);
yy_11_1 = (yy)(-((long)yy_11_1_1));
indentation(yy_11_1);
return;
yyfl_5_1 : ;
}
yyErr(2,76);
}
put_rules_MEMBERLIST(yyin_1)
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
if (yy_0_1[0] != 1) goto yyfl_6_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yyv_H = yy_0_1_1;
yyv_T = yy_0_1_2;
yy_1_1 = yyv_H;
put_rules_MEMBER(yy_1_1);
yy_2_1 = yyv_T;
put_rules_MEMBERLIST(yy_2_1);
return;
yyfl_6_1 : ;
}
{
yy yyb;
yy yy_0_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 2) goto yyfl_6_2;
return;
yyfl_6_2 : ;
}
yyErr(2,93);
}
put_rules_MEMBER(yyin_1)
yy yyin_1;
{
{
yy yyb;
yy yy_0_1;
yy yyv_Id;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yy_0_1_4;
yy yy_1_1;
yy yyv_Str;
yy yy_1_2;
yy yy_3_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 1) goto yyfl_7_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yy_0_1_3 = ((yy)yy_0_1[3]);
yy_0_1_4 = ((yy)yy_0_1[4]);
yyv_Id = yy_0_1_1;
yy_1_1 = yyv_Id;
id_to_string(yy_1_1, &yy_1_2);
yyv_Str = yy_1_2;
indent();
yy_3_1 = yyv_Str;
Put(yy_3_1);
Nl();
return;
yyfl_7_1 : ;
}
{
yy yyb;
yy yy_0_1;
yy yyv_N;
yy yy_0_1_1;
yy yy_2_1;
yy yy_3_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 2) goto yyfl_7_2;
yy_0_1_1 = ((yy)yy_0_1[1]);
yyv_N = yy_0_1_1;
indent();
yy_2_1 = ((yy)"token-");
Put(yy_2_1);
yy_3_1 = yyv_N;
PutI(yy_3_1);
Nl();
return;
yyfl_7_2 : ;
}
{
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yyv_Alternatives;
yy yy_0_1_2;
yy yyv_Key;
yy yy_0_1_3;
yy yy_0_1_4;
yy yyv_Coordinate;
yy yy_0_1_5;
yy yy_2_1;
yy yy_4_1;
yy yy_4_1_1;
yy yy_5_1;
yy yy_6_1;
yy yy_6_1_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 3) goto yyfl_7_3;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yy_0_1_3 = ((yy)yy_0_1[3]);
yy_0_1_4 = ((yy)yy_0_1[4]);
yy_0_1_5 = ((yy)yy_0_1[5]);
if (yy_0_1_1[0] != 1) goto yyfl_7_3;
yyv_Alternatives = yy_0_1_2;
yyv_Key = yy_0_1_3;
yyv_Coordinate = yy_0_1_5;
indent();
yy_2_1 = ((yy)"<option>");
Put(yy_2_1);
Nl();
yy_4_1_1 = ((yy)1);
yy_4_1 = (yy)(((long)yy_4_1_1));
indentation(yy_4_1);
yy_5_1 = yyv_Alternatives;
put_rules_ALTERNATIVELIST(yy_5_1);
yy_6_1_1 = ((yy)1);
yy_6_1 = (yy)(-((long)yy_6_1_1));
indentation(yy_6_1);
return;
yyfl_7_3 : ;
}
{
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yyv_Alternatives;
yy yy_0_1_2;
yy yyv_Key;
yy yy_0_1_3;
yy yy_0_1_4;
yy yyv_Coordinate;
yy yy_0_1_5;
yy yy_2_1;
yy yy_4_1;
yy yy_4_1_1;
yy yy_5_1;
yy yy_6_1;
yy yy_6_1_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 3) goto yyfl_7_4;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yy_0_1_3 = ((yy)yy_0_1[3]);
yy_0_1_4 = ((yy)yy_0_1[4]);
yy_0_1_5 = ((yy)yy_0_1[5]);
if (yy_0_1_1[0] != 2) goto yyfl_7_4;
yyv_Alternatives = yy_0_1_2;
yyv_Key = yy_0_1_3;
yyv_Coordinate = yy_0_1_5;
indent();
yy_2_1 = ((yy)"<loop>");
Put(yy_2_1);
Nl();
yy_4_1_1 = ((yy)1);
yy_4_1 = (yy)(((long)yy_4_1_1));
indentation(yy_4_1);
yy_5_1 = yyv_Alternatives;
put_rules_ALTERNATIVELIST(yy_5_1);
yy_6_1_1 = ((yy)1);
yy_6_1 = (yy)(-((long)yy_6_1_1));
indentation(yy_6_1);
return;
yyfl_7_4 : ;
}
{
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yyv_Alternatives;
yy yy_0_1_2;
yy yyv_Key;
yy yy_0_1_3;
yy yy_0_1_4;
yy yyv_Coordinate;
yy yy_0_1_5;
yy yy_2_1;
yy yy_4_1;
yy yy_4_1_1;
yy yy_5_1;
yy yy_6_1;
yy yy_6_1_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 3) goto yyfl_7_5;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yy_0_1_3 = ((yy)yy_0_1[3]);
yy_0_1_4 = ((yy)yy_0_1[4]);
yy_0_1_5 = ((yy)yy_0_1[5]);
if (yy_0_1_1[0] != 3) goto yyfl_7_5;
yyv_Alternatives = yy_0_1_2;
yyv_Key = yy_0_1_3;
yyv_Coordinate = yy_0_1_5;
indent();
yy_2_1 = ((yy)"<group>");
Put(yy_2_1);
Nl();
yy_4_1_1 = ((yy)1);
yy_4_1 = (yy)(((long)yy_4_1_1));
indentation(yy_4_1);
yy_5_1 = yyv_Alternatives;
put_rules_ALTERNATIVELIST(yy_5_1);
yy_6_1_1 = ((yy)1);
yy_6_1 = (yy)(-((long)yy_6_1_1));
indentation(yy_6_1);
return;
yyfl_7_5 : ;
}
{
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 4) goto yyfl_7_6;
yy_0_1_1 = ((yy)yy_0_1[1]);
return;
yyfl_7_6 : ;
}
yyErr(2,99);
}
put_rules_ALTERNATIVELIST(yyin_1)
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
if (yy_0_1[0] != 1) goto yyfl_8_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yyv_H = yy_0_1_1;
yyv_T = yy_0_1_2;
yy_1_1 = yyv_H;
put_rules_ALTERNATIVE(yy_1_1);
yy_2_1 = yyv_T;
put_rules_ALTERNATIVELIST(yy_2_1);
return;
yyfl_8_1 : ;
}
{
yy yyb;
yy yy_0_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 2) goto yyfl_8_2;
return;
yyfl_8_2 : ;
}
yyErr(2,135);
}
put_rules_ALTERNATIVE(yyin_1)
yy yyin_1;
{
{
yy yyb;
yy yy_0_1;
yy yyv_Members;
yy yy_0_1_1;
yy yyv_AlternativeKey;
yy yy_0_1_2;
yy yy_0_1_3;
yy yyv_Coordinate;
yy yy_0_1_4;
yy yy_2_1;
yy yyv_ClauseKey;
yy yy_3;
yy yyv_Pos;
yy yy_4;
yy yy_5_1;
yy yy_6_1;
yy yy_7_1;
yy yy_8_1;
yy yyv_N;
yy yy_8_2;
yy yy_9_1;
yy yy_10_1;
yy yy_11_1;
yy yy_13_1;
yy yy_13_1_1;
yy yy_14_1;
yy yy_15_1;
yy yy_15_1_1;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 1) goto yyfl_9_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yy_0_1_3 = ((yy)yy_0_1[3]);
yy_0_1_4 = ((yy)yy_0_1[4]);
yyv_Members = yy_0_1_1;
yyv_AlternativeKey = yy_0_1_2;
yyv_Coordinate = yy_0_1_4;
indent();
yy_2_1 = ((yy)"<alternative>");
Put(yy_2_1);
yy_3 = (yy) yyv_AlternativeKey[3];
if (yy_3 == (yy) yyu) yyErr(1,150);
yyv_ClauseKey = yy_3;
yy_4 = (yy) yyv_ClauseKey[3];
if (yy_4 == (yy) yyu) yyErr(1,151);
yyv_Pos = yy_4;
yy_5_1 = ((yy)"  [");
Put(yy_5_1);
yy_6_1 = yyv_Pos;
PutI(yy_6_1);
yy_7_1 = ((yy)"]");
Put(yy_7_1);
yy_8_1 = yyv_Coordinate;
POS_to_INT(yy_8_1, &yy_8_2);
yyv_N = yy_8_2;
yy_9_1 = ((yy)"  [");
Put(yy_9_1);
yy_10_1 = yyv_N;
PutI(yy_10_1);
yy_11_1 = ((yy)"]");
Put(yy_11_1);
Nl();
yy_13_1_1 = ((yy)1);
yy_13_1 = (yy)(((long)yy_13_1_1));
indentation(yy_13_1);
yy_14_1 = yyv_Members;
put_rules_MEMBERLIST(yy_14_1);
yy_15_1_1 = ((yy)1);
yy_15_1 = (yy)(-((long)yy_15_1_1));
indentation(yy_15_1);
return;
yyfl_9_1 : ;
}
yyErr(2,141);
}
init_indentation()
{
{
yy yyb;
yy yy_1;
yy_1 = ((yy)0);
yyglov_IndentationLevel = yy_1;
return;
}
}
indent()
{
{
yy yyb;
yy yyv_Level;
yy yy_1;
yy yy_2_1;
yy yyv_Count;
yy yy_2_2;
yy yy_3_1_1;
yy yy_3_1_2;
yy yy_3_2_1;
yy yy_3_3_1;
yy yy_3_3_1_1;
yy yy_3_3_1_2;
yy yy_3_3_2;
yy_1 = yyglov_IndentationLevel;
if (yy_1 == (yy) yyu) yyErr(1,174);
yyv_Level = yy_1;
yy_2_1 = ((yy)1);
yy_2_2 = yy_2_1;
yyv_Count = yy_2_2;
yysl_12_1_3 : ;
yy_3_1_1 = yyv_Count;
yy_3_1_2 = yyv_Level;
if ((long)yy_3_1_1 > (long)yy_3_1_2) goto yyfl_12_1_3;
yy_3_2_1 = ((yy)"   ");
Put(yy_3_2_1);
yy_3_3_1_1 = yyv_Count;
yy_3_3_1_2 = ((yy)1);
yy_3_3_1 = (yy)(((long)yy_3_3_1_1)+((long)yy_3_3_1_2));
yyv_Count = yy_3_3_1;
goto yysl_12_1_3;
yyfl_12_1_3 : ;
return;
}
}
indentation(yyin_1)
yy yyin_1;
{
{
yy yyb;
yy yyv_N;
yy yy_0_1;
yy yyv_Level;
yy yy_1;
yy yy_2;
yy yy_2_1;
yy yy_2_2;
yy_0_1 = yyin_1;
yyv_N = yy_0_1;
yy_1 = yyglov_IndentationLevel;
if (yy_1 == (yy) yyu) yyErr(1,184);
yyv_Level = yy_1;
yy_2_1 = yyv_Level;
yy_2_2 = yyv_N;
yy_2 = (yy)(((long)yy_2_1)+((long)yy_2_2));
yyglov_IndentationLevel = yy_2;
return;
}
}
