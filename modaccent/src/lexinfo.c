typedef long * yy;
#define yyu (-2147483647L)
static yy yynull;
extern yy yyh;
extern yy yyhx;
static yyErr(n,l)
{
yyAbort(n,"lexinfo", l);
}
extern yy yyglov_LHS_List;
extern yy yyglov_GentleFlag;
extern yy yyglov_NontermList;
extern yy yyglov_PreludeBlock;
yy yyglov_Tokens = (yy) yyu;
yy yyglov_Literals = (yy) yyu;
yyeq_TKNLIST(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_TKN((yy)t1[1], (yy)t2[1])
&& yyeq_TKNLIST((yy)t1[2], (yy)t2[2])
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_TKNLIST(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("list");
yyFirstArg();
yyPrint_TKN((yy)t[1]);
yyNextArg();
yyPrint_TKNLIST((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("nil");
yyNoArgs();
break;
}
}
yybroadcast_TKNLIST(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_TKNLIST, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_TKN((yy)t[1], A, &B, Handler);
yybroadcast_TKNLIST((yy)t[2], B, &A, Handler);
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yyeq_TKN(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_IDENT((yy)t1[1], (yy)t2[1])
&& (t1[2] == t2[2])
;
}
}
yyPrint_TKN(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("token");
yyFirstArg();
yyPrint_IDENT((yy)t[1]);
yyNextArg();
yyPrint_INT((yy)t[2]);
yyEndArgs();
break;
}
}
yybroadcast_TKN(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_TKN, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_IDENT((yy)t[1], A, &B, Handler);
*Out = B;
break;
}
}
}
yyeq_LITLIST(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_LIT((yy)t1[1], (yy)t2[1])
&& yyeq_LITLIST((yy)t1[2], (yy)t2[2])
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_LITLIST(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("list");
yyFirstArg();
yyPrint_LIT((yy)t[1]);
yyNextArg();
yyPrint_LITLIST((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("nil");
yyNoArgs();
break;
}
}
yybroadcast_LITLIST(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_LITLIST, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_LIT((yy)t[1], A, &B, Handler);
yybroadcast_LITLIST((yy)t[2], B, &A, Handler);
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yyeq_LIT(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&&(strcmp((char *) t1[1], (char *) t2[1]) == 0)
&& (t1[2] == t2[2])
;
}
}
yyPrint_LIT(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("literal");
yyFirstArg();
yyPrint_STRING((yy)t[1]);
yyNextArg();
yyPrint_INT((yy)t[2]);
yyEndArgs();
break;
}
}
yybroadcast_LIT(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_LIT, t, In, Out)) {
switch(t[0]) {
case 1: 
*Out = A;
break;
}
}
}
init_lexinfo()
{
{
yy yyb;
yy yy_1;
yy yy_2;
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
yy_1 = yyb + 0;
yy_1[0] = 2;
yyglov_Tokens = yy_1;
yy_2 = yyb + 1;
yy_2[0] = 2;
yyglov_Literals = yy_2;
return;
}
}
enter_token(yyin_1, yyin_2)
yy yyin_1;
yy yyin_2;
{
{
yy yyb;
yy yyv_Id;
yy yy_0_1;
yy yyv_N;
yy yy_0_2;
yy yyv_L;
yy yy_1;
yy yy_2;
yy yy_2_1;
yy yy_2_1_1;
yy yy_2_1_2;
yy yy_2_2;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yyv_Id = yy_0_1;
yyv_N = yy_0_2;
yyb = yyh;
yyh += 6; if (yyh > yyhx) yyExtend();
yy_1 = yyglov_Tokens;
if (yy_1 == (yy) yyu) yyErr(1,59);
yyv_L = yy_1;
yy_2_1_1 = yyv_Id;
yy_2_1_2 = yyv_N;
yy_2_1 = yyb + 3;
yy_2_1[0] = 1;
yy_2_1[1] = ((long)yy_2_1_1);
yy_2_1[2] = ((long)yy_2_1_2);
yy_2_2 = yyv_L;
yy_2 = yyb + 0;
yy_2[0] = 1;
yy_2[1] = ((long)yy_2_1);
yy_2[2] = ((long)yy_2_2);
yyglov_Tokens = yy_2;
return;
}
}
enter_literal(yyin_1, yyin_2)
yy yyin_1;
yy yyin_2;
{
{
yy yyb;
yy yyv_Str;
yy yy_0_1;
yy yyv_N;
yy yy_0_2;
yy yyv_L;
yy yy_1;
yy yy_2;
yy yy_2_1;
yy yy_2_1_1;
yy yy_2_1_2;
yy yy_2_2;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yyv_Str = yy_0_1;
yyv_N = yy_0_2;
yyb = yyh;
yyh += 6; if (yyh > yyhx) yyExtend();
yy_1 = yyglov_Literals;
if (yy_1 == (yy) yyu) yyErr(1,64);
yyv_L = yy_1;
yy_2_1_1 = yyv_Str;
yy_2_1_2 = yyv_N;
yy_2_1 = yyb + 3;
yy_2_1[0] = 1;
yy_2_1[1] = ((long)yy_2_1_1);
yy_2_1[2] = ((long)yy_2_1_2);
yy_2_2 = yyv_L;
yy_2 = yyb + 0;
yy_2[0] = 1;
yy_2[1] = ((long)yy_2_1);
yy_2[2] = ((long)yy_2_2);
yyglov_Literals = yy_2;
return;
}
}
lexinfo()
{
{
yy yyb;
yy yy_1_1;
yy yy_2_1;
yy yy_4_1;
yy yy_6_1;
yy yy_8_1;
yy yy_10_1;
yy yyv_TL;
yy yy_13;
yy yy_14_1_1;
yy yy_14_1_2;
yy yyv_Head;
yy yy_14_1_2_1;
yy yyv_Tail;
yy yy_14_1_2_2;
yy yy_14_2_1;
yy yy_14_2_2;
yy yyv_Id;
yy yy_14_2_2_1;
yy yyv_N;
yy yy_14_2_2_2;
yy yy_14_3_1;
yy yyv_Str;
yy yy_14_3_2;
yy yy_14_4_1;
yy yy_14_5_1;
yy yy_14_6_1;
yy yy_14_7_1;
yy yy_14_9_1;
yy yy_14_9_2;
yy yyv_LL;
yy yy_15;
yy yy_16_1_1;
yy yy_16_1_2;
yy yy_16_1_2_1;
yy yy_16_1_2_2;
yy yy_16_2_1;
yy yy_16_2_2;
yy yy_16_2_2_1;
yy yy_16_2_2_2;
yy yy_16_3_1;
yy yy_16_4_1;
yy yy_16_5_1;
yy yy_16_6_1;
yy yy_16_8_1;
yy yy_16_8_2;
yy_1_1 = ((yy)"yygrammar.h");
OpenOutput(yy_1_1);
yy_2_1 = ((yy)"#ifndef YYSTYPE");
Put(yy_2_1);
Nl();
yy_4_1 = ((yy)"#define YYSTYPE long");
Put(yy_4_1);
Nl();
yy_6_1 = ((yy)"#endif");
Put(yy_6_1);
Nl();
yy_8_1 = ((yy)"extern YYSTYPE yylval;");
Put(yy_8_1);
Nl();
yy_10_1 = ((yy)"extern long yypos;");
Put(yy_10_1);
Nl();
Nl();
yy_13 = yyglov_Tokens;
if (yy_13 == (yy) yyu) yyErr(1,78);
yyv_TL = yy_13;
yysl_10_1_14 : ;
yy_14_1_1 = yyv_TL;
yy_14_1_2 = yy_14_1_1;
if (yy_14_1_2[0] != 1) goto yyfl_10_1_14;
yy_14_1_2_1 = ((yy)yy_14_1_2[1]);
yy_14_1_2_2 = ((yy)yy_14_1_2[2]);
yyv_Head = yy_14_1_2_1;
yyv_Tail = yy_14_1_2_2;
yy_14_2_1 = yyv_Head;
yy_14_2_2 = yy_14_2_1;
if (yy_14_2_2[0] != 1) goto yyfl_10_1_14;
yy_14_2_2_1 = ((yy)yy_14_2_2[1]);
yy_14_2_2_2 = ((yy)yy_14_2_2[2]);
yyv_Id = yy_14_2_2_1;
yyv_N = yy_14_2_2_2;
yy_14_3_1 = yyv_Id;
id_to_string(yy_14_3_1, &yy_14_3_2);
yyv_Str = yy_14_3_2;
yy_14_4_1 = ((yy)"#define ");
Put(yy_14_4_1);
yy_14_5_1 = yyv_Str;
Put(yy_14_5_1);
yy_14_6_1 = ((yy)" ");
Put(yy_14_6_1);
yy_14_7_1 = yyv_N;
PutI(yy_14_7_1);
Nl();
yy_14_9_1 = yyv_Tail;
yyv_TL = yy_14_9_1;
goto yysl_10_1_14;
yyfl_10_1_14 : ;
yy_15 = yyglov_Literals;
if (yy_15 == (yy) yyu) yyErr(1,91);
yyv_LL = yy_15;
yysl_10_1_16 : ;
yy_16_1_1 = yyv_LL;
yy_16_1_2 = yy_16_1_1;
if (yy_16_1_2[0] != 1) goto yyfl_10_1_16;
yy_16_1_2_1 = ((yy)yy_16_1_2[1]);
yy_16_1_2_2 = ((yy)yy_16_1_2[2]);
yyv_Head = yy_16_1_2_1;
yyv_Tail = yy_16_1_2_2;
yy_16_2_1 = yyv_Head;
yy_16_2_2 = yy_16_2_1;
if (yy_16_2_2[0] != 1) goto yyfl_10_1_16;
yy_16_2_2_1 = ((yy)yy_16_2_2[1]);
yy_16_2_2_2 = ((yy)yy_16_2_2[2]);
yyv_Str = yy_16_2_2_1;
yyv_N = yy_16_2_2_2;
yy_16_3_1 = ((yy)"#define yytoken_");
Put(yy_16_3_1);
yy_16_4_1 = yyv_N;
PutI(yy_16_4_1);
yy_16_5_1 = ((yy)" ");
Put(yy_16_5_1);
yy_16_6_1 = yyv_N;
PutI(yy_16_6_1);
Nl();
yy_16_8_1 = yyv_Tail;
yyv_LL = yy_16_8_1;
goto yysl_10_1_16;
yyfl_10_1_16 : ;
CloseOutput();
return;
}
}
lex_printnames()
{
{
yy yyb;
yy yyv_TL;
yy yy_1;
yy yy_2_1_1;
yy yy_2_1_2;
yy yyv_Head;
yy yy_2_1_2_1;
yy yyv_Tail;
yy yy_2_1_2_2;
yy yy_2_2_1;
yy yy_2_2_2;
yy yyv_Id;
yy yy_2_2_2_1;
yy yyv_N;
yy yy_2_2_2_2;
yy yy_2_3_1;
yy yy_2_4_1;
yy yy_2_5_1;
yy yy_2_6_1;
yy yyv_Str;
yy yy_2_6_2;
yy yy_2_7_1;
yy yy_2_8_1;
yy yy_2_10_1;
yy yy_2_10_2;
yy yyv_LL;
yy yy_3;
yy yy_4_1_1;
yy yy_4_1_2;
yy yy_4_1_2_1;
yy yy_4_1_2_2;
yy yy_4_2_1;
yy yy_4_2_2;
yy yy_4_2_2_1;
yy yy_4_2_2_2;
yy yy_4_3_1;
yy yy_4_4_1;
yy yy_4_5_1;
yy yy_4_6_1;
yy yy_4_7_1;
yy yy_4_9_1;
yy yy_4_9_2;
yy_1 = yyglov_Tokens;
if (yy_1 == (yy) yyu) yyErr(1,110);
yyv_TL = yy_1;
yysl_11_1_2 : ;
yy_2_1_1 = yyv_TL;
yy_2_1_2 = yy_2_1_1;
if (yy_2_1_2[0] != 1) goto yyfl_11_1_2;
yy_2_1_2_1 = ((yy)yy_2_1_2[1]);
yy_2_1_2_2 = ((yy)yy_2_1_2[2]);
yyv_Head = yy_2_1_2_1;
yyv_Tail = yy_2_1_2_2;
yy_2_2_1 = yyv_Head;
yy_2_2_2 = yy_2_2_1;
if (yy_2_2_2[0] != 1) goto yyfl_11_1_2;
yy_2_2_2_1 = ((yy)yy_2_2_2[1]);
yy_2_2_2_2 = ((yy)yy_2_2_2[2]);
yyv_Id = yy_2_2_2_1;
yyv_N = yy_2_2_2_2;
yy_2_3_1 = ((yy)"         case ");
Put(yy_2_3_1);
yy_2_4_1 = yyv_N;
PutI(yy_2_4_1);
yy_2_5_1 = ((yy)": return \"");
Put(yy_2_5_1);
yy_2_6_1 = yyv_Id;
id_to_string(yy_2_6_1, &yy_2_6_2);
yyv_Str = yy_2_6_2;
yy_2_7_1 = yyv_Str;
Put(yy_2_7_1);
yy_2_8_1 = ((yy)"\"; break;");
Put(yy_2_8_1);
Nl();
yy_2_10_1 = yyv_Tail;
yyv_TL = yy_2_10_1;
goto yysl_11_1_2;
yyfl_11_1_2 : ;
yy_3 = yyglov_Literals;
if (yy_3 == (yy) yyu) yyErr(1,124);
yyv_LL = yy_3;
yysl_11_1_4 : ;
yy_4_1_1 = yyv_LL;
yy_4_1_2 = yy_4_1_1;
if (yy_4_1_2[0] != 1) goto yyfl_11_1_4;
yy_4_1_2_1 = ((yy)yy_4_1_2[1]);
yy_4_1_2_2 = ((yy)yy_4_1_2[2]);
yyv_Head = yy_4_1_2_1;
yyv_Tail = yy_4_1_2_2;
yy_4_2_1 = yyv_Head;
yy_4_2_2 = yy_4_2_1;
if (yy_4_2_2[0] != 1) goto yyfl_11_1_4;
yy_4_2_2_1 = ((yy)yy_4_2_2[1]);
yy_4_2_2_2 = ((yy)yy_4_2_2[2]);
yyv_Str = yy_4_2_2_1;
yyv_N = yy_4_2_2_2;
yy_4_3_1 = ((yy)"         case ");
Put(yy_4_3_1);
yy_4_4_1 = yyv_N;
PutI(yy_4_4_1);
yy_4_5_1 = ((yy)": return \"");
Put(yy_4_5_1);
yy_4_6_1 = yyv_Str;
Put(yy_4_6_1);
yy_4_7_1 = ((yy)"\"; break;");
Put(yy_4_7_1);
Nl();
yy_4_9_1 = yyv_Tail;
yyv_LL = yy_4_9_1;
goto yysl_11_1_4;
yyfl_11_1_4 : ;
return;
}
}
