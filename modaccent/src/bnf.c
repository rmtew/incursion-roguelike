#define NULL 0
typedef long * yy;
#define yyu (-2147483647L)
static yy yynull;
extern yy yyh;
extern yy yyhx;
static yyErr(n,l)
{
yyAbort(n,"bnf", l);
}
extern yy yyglov_LHS_List;
yyeq_DEFAULTSPEC(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_DEFAULTSPEC(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("default");
yyNoArgs();
break;
case 2: 
yyTerm("nodefault");
yyNoArgs();
break;
}
}
yybroadcast_DEFAULTSPEC(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A;
A = In;
if (! Handler(yybroadcast_DEFAULTSPEC, t, In, Out)) {
switch(t[0]) {
case 1: 
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yy yyglov_DefaultSpec = (yy) yyu;
yyeq_ACTIONBLOCK(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&&(strcmp((char *) t1[1], (char *) t2[1]) == 0)
&& (t1[2] == t2[2])
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_ACTIONBLOCK(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("action");
yyFirstArg();
yyPrint_STRING((yy)t[1]);
yyNextArg();
yyPrint_POS((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("none");
yyNoArgs();
break;
}
}
yybroadcast_ACTIONBLOCK(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_ACTIONBLOCK, t, In, Out)) {
switch(t[0]) {
case 1: 
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yy yyglov_PreludeBlock = (yy) yyu;
yyeq_RULELIST(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_RULE((yy)t1[1], (yy)t2[1])
&& yyeq_RULELIST((yy)t1[2], (yy)t2[2])
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_RULELIST(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("list");
yyFirstArg();
yyPrint_RULE((yy)t[1]);
yyNextArg();
yyPrint_RULELIST((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("nil");
yyNoArgs();
break;
}
}
yybroadcast_RULELIST(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_RULELIST, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_RULE((yy)t[1], A, &B, Handler);
yybroadcast_RULELIST((yy)t[2], B, &A, Handler);
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yyeq_RULE(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_IDENT((yy)t1[1], (yy)t2[1])
&& yyeq_MEMBERLIST((yy)t1[2], (yy)t2[2])
&& yyeq_RULEKEY((yy)t1[3], (yy)t2[3])
&& yyeq_ACTIONBLOCK((yy)t1[4], (yy)t2[4])
&& yyeq_RULEANNOTATION((yy)t1[5], (yy)t2[5])
&& (t1[6] == t2[6])
;
case 2: return (t2[0] == 2)
&& yyeq_IDENT((yy)t1[1], (yy)t2[1])
&& yyeq_RHSLIST((yy)t1[2], (yy)t2[2])
&& yyeq_ACTIONBLOCK((yy)t1[3], (yy)t2[3])
&& (t1[4] == t2[4])
;
}
}
yyPrint_RULE(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("xxrule");
yyFirstArg();
yyPrint_IDENT((yy)t[1]);
yyNextArg();
yyPrint_MEMBERLIST((yy)t[2]);
yyNextArg();
yyPrint_RULEKEY((yy)t[3]);
yyNextArg();
yyPrint_ACTIONBLOCK((yy)t[4]);
yyNextArg();
yyPrint_RULEANNOTATION((yy)t[5]);
yyNextArg();
yyPrint_POS((yy)t[6]);
yyEndArgs();
break;
case 2: 
yyTerm("rule");
yyFirstArg();
yyPrint_IDENT((yy)t[1]);
yyNextArg();
yyPrint_RHSLIST((yy)t[2]);
yyNextArg();
yyPrint_ACTIONBLOCK((yy)t[3]);
yyNextArg();
yyPrint_POS((yy)t[4]);
yyEndArgs();
break;
}
}
yybroadcast_RULE(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_RULE, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_IDENT((yy)t[1], A, &B, Handler);
yybroadcast_MEMBERLIST((yy)t[2], B, &A, Handler);
yybroadcast_ACTIONBLOCK((yy)t[4], A, &B, Handler);
yybroadcast_RULEANNOTATION((yy)t[5], B, &A, Handler);
*Out = A;
break;
case 2: 
yybroadcast_IDENT((yy)t[1], A, &B, Handler);
yybroadcast_RHSLIST((yy)t[2], B, &A, Handler);
yybroadcast_ACTIONBLOCK((yy)t[3], A, &B, Handler);
*Out = B;
break;
}
}
}
yyeq_RHSLIST(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_RHS((yy)t1[1], (yy)t2[1])
&& yyeq_RHSLIST((yy)t1[2], (yy)t2[2])
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_RHSLIST(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("list");
yyFirstArg();
yyPrint_RHS((yy)t[1]);
yyNextArg();
yyPrint_RHSLIST((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("nil");
yyNoArgs();
break;
}
}
yybroadcast_RHSLIST(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_RHSLIST, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_RHS((yy)t[1], A, &B, Handler);
yybroadcast_RHSLIST((yy)t[2], B, &A, Handler);
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yyeq_RHS(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_MEMBERLIST((yy)t1[1], (yy)t2[1])
&& yyeq_RULEKEY((yy)t1[2], (yy)t2[2])
&& yyeq_RULEANNOTATION((yy)t1[3], (yy)t2[3])
&& (t1[4] == t2[4])
;
}
}
yyPrint_RHS(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("rhs");
yyFirstArg();
yyPrint_MEMBERLIST((yy)t[1]);
yyNextArg();
yyPrint_RULEKEY((yy)t[2]);
yyNextArg();
yyPrint_RULEANNOTATION((yy)t[3]);
yyNextArg();
yyPrint_POS((yy)t[4]);
yyEndArgs();
break;
}
}
yybroadcast_RHS(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_RHS, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_MEMBERLIST((yy)t[1], A, &B, Handler);
yybroadcast_RULEANNOTATION((yy)t[3], B, &A, Handler);
*Out = A;
break;
}
}
}
yyeq_RULEANNOTATION(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& (t1[1] == t2[1])
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_RULEANNOTATION(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("prio");
yyFirstArg();
yyPrint_INT((yy)t[1]);
yyEndArgs();
break;
case 2: 
yyTerm("undefruleanno");
yyNoArgs();
break;
}
}
yybroadcast_RULEANNOTATION(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_RULEANNOTATION, t, In, Out)) {
switch(t[0]) {
case 1: 
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yyeq_MEMBERLIST(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_MEMBER((yy)t1[1], (yy)t2[1])
&& yyeq_MEMBERLIST((yy)t1[2], (yy)t2[2])
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_MEMBERLIST(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("list");
yyFirstArg();
yyPrint_MEMBER((yy)t[1]);
yyNextArg();
yyPrint_MEMBERLIST((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("nil");
yyNoArgs();
break;
}
}
yybroadcast_MEMBERLIST(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_MEMBERLIST, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_MEMBER((yy)t[1], A, &B, Handler);
yybroadcast_MEMBERLIST((yy)t[2], B, &A, Handler);
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yyeq_MEMBER(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_IDENT((yy)t1[1], (yy)t2[1])
&& yyeq_ACTPARAMLIST((yy)t1[2], (yy)t2[2])
&& yyeq_MEMBERANNOTATION((yy)t1[3], (yy)t2[3])
&& (t1[4] == t2[4])
;
case 2: return (t2[0] == 2)
&& (t1[1] == t2[1])
;
case 3: return (t2[0] == 3)
&& yyeq_SUBPHRASEKIND((yy)t1[1], (yy)t2[1])
&& yyeq_ALTERNATIVELIST((yy)t1[2], (yy)t2[2])
&& yyeq_SUBPHRASEKEY((yy)t1[3], (yy)t2[3])
&& yyeq_MEMBERANNOTATION((yy)t1[4], (yy)t2[4])
&& (t1[5] == t2[5])
;
case 4: return (t2[0] == 4)
&& yyeq_ACTIONBLOCK((yy)t1[1], (yy)t2[1])
;
}
}
yyPrint_MEMBER(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("call");
yyFirstArg();
yyPrint_IDENT((yy)t[1]);
yyNextArg();
yyPrint_ACTPARAMLIST((yy)t[2]);
yyNextArg();
yyPrint_MEMBERANNOTATION((yy)t[3]);
yyNextArg();
yyPrint_POS((yy)t[4]);
yyEndArgs();
break;
case 2: 
yyTerm("token");
yyFirstArg();
yyPrint_INT((yy)t[1]);
yyEndArgs();
break;
case 3: 
yyTerm("subphrase");
yyFirstArg();
yyPrint_SUBPHRASEKIND((yy)t[1]);
yyNextArg();
yyPrint_ALTERNATIVELIST((yy)t[2]);
yyNextArg();
yyPrint_SUBPHRASEKEY((yy)t[3]);
yyNextArg();
yyPrint_MEMBERANNOTATION((yy)t[4]);
yyNextArg();
yyPrint_POS((yy)t[5]);
yyEndArgs();
break;
case 4: 
yyTerm("sem");
yyFirstArg();
yyPrint_ACTIONBLOCK((yy)t[1]);
yyEndArgs();
break;
}
}
yybroadcast_MEMBER(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_MEMBER, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_IDENT((yy)t[1], A, &B, Handler);
yybroadcast_ACTPARAMLIST((yy)t[2], B, &A, Handler);
yybroadcast_MEMBERANNOTATION((yy)t[3], A, &B, Handler);
*Out = B;
break;
case 2: 
*Out = A;
break;
case 3: 
yybroadcast_SUBPHRASEKIND((yy)t[1], A, &B, Handler);
yybroadcast_ALTERNATIVELIST((yy)t[2], B, &A, Handler);
yybroadcast_MEMBERANNOTATION((yy)t[4], A, &B, Handler);
*Out = B;
break;
case 4: 
yybroadcast_ACTIONBLOCK((yy)t[1], A, &B, Handler);
*Out = B;
break;
}
}
}
yyeq_MEMBERANNOTATION(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
;
case 2: return (t2[0] == 2)
;
case 3: return (t2[0] == 3)
;
case 4: return (t2[0] == 4)
&& (t1[1] == t2[1])
;
}
}
yyPrint_MEMBERANNOTATION(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("short");
yyNoArgs();
break;
case 2: 
yyTerm("long");
yyNoArgs();
break;
case 3: 
yyTerm("undefmemberanno");
yyNoArgs();
break;
case 4: 
yyTerm("user");
yyFirstArg();
yyPrint_INT((yy)t[1]);
yyEndArgs();
break;
}
}
yybroadcast_MEMBERANNOTATION(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_MEMBERANNOTATION, t, In, Out)) {
switch(t[0]) {
case 1: 
*Out = A;
break;
case 2: 
*Out = A;
break;
case 3: 
*Out = A;
break;
case 4: 
*Out = A;
break;
}
}
}
yyeq_SUBPHRASEKIND(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
;
case 2: return (t2[0] == 2)
&& yyeq_MEMBERANNOTATION((yy)t1[1], (yy)t2[1])
;
case 3: return (t2[0] == 3)
;
}
}
yyPrint_SUBPHRASEKIND(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("option");
yyNoArgs();
break;
case 2: 
yyTerm("loop");
yyFirstArg();
yyPrint_MEMBERANNOTATION((yy)t[1]);
yyEndArgs();
break;
case 3: 
yyTerm("alternatives");
yyNoArgs();
break;
}
}
yybroadcast_SUBPHRASEKIND(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_SUBPHRASEKIND, t, In, Out)) {
switch(t[0]) {
case 1: 
*Out = A;
break;
case 2: 
yybroadcast_MEMBERANNOTATION((yy)t[1], A, &B, Handler);
*Out = B;
break;
case 3: 
*Out = A;
break;
}
}
}
yyeq_ALTERNATIVELIST(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_ALTERNATIVE((yy)t1[1], (yy)t2[1])
&& yyeq_ALTERNATIVELIST((yy)t1[2], (yy)t2[2])
;
case 2: return (t2[0] == 2)
&& yyeq_RULEANNOTATION((yy)t1[1], (yy)t2[1])
&& (t1[2] == t2[2])
;
}
}
yyPrint_ALTERNATIVELIST(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("list");
yyFirstArg();
yyPrint_ALTERNATIVE((yy)t[1]);
yyNextArg();
yyPrint_ALTERNATIVELIST((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("nil");
yyFirstArg();
yyPrint_RULEANNOTATION((yy)t[1]);
yyNextArg();
yyPrint_POS((yy)t[2]);
yyEndArgs();
break;
}
}
yybroadcast_ALTERNATIVELIST(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_ALTERNATIVELIST, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_ALTERNATIVE((yy)t[1], A, &B, Handler);
yybroadcast_ALTERNATIVELIST((yy)t[2], B, &A, Handler);
*Out = A;
break;
case 2: 
yybroadcast_RULEANNOTATION((yy)t[1], A, &B, Handler);
*Out = B;
break;
}
}
}
yyeq_ALTERNATIVE(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_MEMBERLIST((yy)t1[1], (yy)t2[1])
&& yyeq_ALTERNATIVEKEY((yy)t1[2], (yy)t2[2])
&& yyeq_RULEANNOTATION((yy)t1[3], (yy)t2[3])
&& (t1[4] == t2[4])
;
}
}
yyPrint_ALTERNATIVE(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("alternative");
yyFirstArg();
yyPrint_MEMBERLIST((yy)t[1]);
yyNextArg();
yyPrint_ALTERNATIVEKEY((yy)t[2]);
yyNextArg();
yyPrint_RULEANNOTATION((yy)t[3]);
yyNextArg();
yyPrint_POS((yy)t[4]);
yyEndArgs();
break;
}
}
yybroadcast_ALTERNATIVE(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_ALTERNATIVE, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_MEMBERLIST((yy)t[1], A, &B, Handler);
yybroadcast_RULEANNOTATION((yy)t[3], B, &A, Handler);
*Out = A;
break;
}
}
}
yyeq_FORMPARAMLIST(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_FORMPARAM((yy)t1[1], (yy)t2[1])
&& yyeq_FORMPARAMLIST((yy)t1[2], (yy)t2[2])
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_FORMPARAMLIST(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("list");
yyFirstArg();
yyPrint_FORMPARAM((yy)t[1]);
yyNextArg();
yyPrint_FORMPARAMLIST((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("nil");
yyNoArgs();
break;
}
}
yybroadcast_FORMPARAMLIST(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_FORMPARAMLIST, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_FORMPARAM((yy)t[1], A, &B, Handler);
yybroadcast_FORMPARAMLIST((yy)t[2], B, &A, Handler);
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yyeq_FORMPARAM(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_MODE((yy)t1[1], (yy)t2[1])
&& yyeq_IDENT((yy)t1[2], (yy)t2[2])
&& yyeq_IDENT((yy)t1[3], (yy)t2[3])
&& (t1[4] == t2[4])
;
}
}
yyPrint_FORMPARAM(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("param");
yyFirstArg();
yyPrint_MODE((yy)t[1]);
yyNextArg();
yyPrint_IDENT((yy)t[2]);
yyNextArg();
yyPrint_IDENT((yy)t[3]);
yyNextArg();
yyPrint_POS((yy)t[4]);
yyEndArgs();
break;
}
}
yybroadcast_FORMPARAM(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_FORMPARAM, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_MODE((yy)t[1], A, &B, Handler);
yybroadcast_IDENT((yy)t[2], B, &A, Handler);
yybroadcast_IDENT((yy)t[3], A, &B, Handler);
*Out = B;
break;
}
}
}
yyeq_MODE(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
;
case 2: return (t2[0] == 2)
;
case 3: return (t2[0] == 3)
;
}
}
yyPrint_MODE(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("xxin");
yyNoArgs();
break;
case 2: 
yyTerm("out");
yyNoArgs();
break;
case 3: 
yyTerm("none");
yyNoArgs();
break;
}
}
yybroadcast_MODE(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_MODE, t, In, Out)) {
switch(t[0]) {
case 1: 
*Out = A;
break;
case 2: 
*Out = A;
break;
case 3: 
*Out = A;
break;
}
}
}
yyeq_ACTPARAMLIST(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_ACTPARAM((yy)t1[1], (yy)t2[1])
&& yyeq_ACTPARAMLIST((yy)t1[2], (yy)t2[2])
&& (t1[3] == t2[3])
;
case 2: return (t2[0] == 2)
&& (t1[1] == t2[1])
;
}
}
yyPrint_ACTPARAMLIST(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("list");
yyFirstArg();
yyPrint_ACTPARAM((yy)t[1]);
yyNextArg();
yyPrint_ACTPARAMLIST((yy)t[2]);
yyNextArg();
yyPrint_POS((yy)t[3]);
yyEndArgs();
break;
case 2: 
yyTerm("nil");
yyFirstArg();
yyPrint_POS((yy)t[1]);
yyEndArgs();
break;
}
}
yybroadcast_ACTPARAMLIST(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_ACTPARAMLIST, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_ACTPARAM((yy)t[1], A, &B, Handler);
yybroadcast_ACTPARAMLIST((yy)t[2], B, &A, Handler);
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yyeq_ACTPARAM(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_IDENT((yy)t1[1], (yy)t2[1])
&& (t1[2] == t2[2])
;
}
}
yyPrint_ACTPARAM(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("id");
yyFirstArg();
yyPrint_IDENT((yy)t[1]);
yyNextArg();
yyPrint_POS((yy)t[2]);
yyEndArgs();
break;
}
}
yybroadcast_ACTPARAM(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_ACTPARAM, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_IDENT((yy)t[1], A, &B, Handler);
*Out = B;
break;
}
}
}
yyeq_RULEKEY(t1, t2) yy t1, t2;
{
return t1 == t2;
}
yyPrint_RULEKEY(t) yy t;
{
yyPrintIndex(t);
}
yyeq_ALTERNATIVEKEY(t1, t2) yy t1, t2;
{
return t1 == t2;
}
yyPrint_ALTERNATIVEKEY(t) yy t;
{
yyPrintIndex(t);
}
yyeq_SUBPHRASEKEY(t1, t2) yy t1, t2;
{
return t1 == t2;
}
yyPrint_SUBPHRASEKEY(t) yy t;
{
yyPrintIndex(t);
}
yyeq_NONTERMKEY(t1, t2) yy t1, t2;
{
return t1 == t2;
}
yyPrint_NONTERMKEY(t) yy t;
{
yyPrintIndex(t);
}
yyeq_RULES(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_RULES((yy)t1[1], (yy)t2[1])
&& yyeq_RULE((yy)t1[2], (yy)t2[2])
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_RULES(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("rules");
yyFirstArg();
yyPrint_RULES((yy)t[1]);
yyNextArg();
yyPrint_RULE((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("nil");
yyNoArgs();
break;
}
}
yybroadcast_RULES(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_RULES, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_RULES((yy)t[1], A, &B, Handler);
yybroadcast_RULE((yy)t[2], B, &A, Handler);
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yy yyglov_UniqueRuleNumber = (yy) yyu;
yy yyglov_UniqueAlternativeNumber = (yy) yyu;
yy yyglov_UniqueSubphraseNumber = (yy) yyu;
yy yyglov_AlternativeIndex = (yy) yyu;
yy yyglov_NontermList = (yy) yyu;
yyeq_NONTERMS(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_NONTERMS((yy)t1[1], (yy)t2[1])
&& yyeq_NONTERMKEY((yy)t1[2], (yy)t2[2])
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_NONTERMS(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("nonterms");
yyFirstArg();
yyPrint_NONTERMS((yy)t[1]);
yyNextArg();
yyPrint_NONTERMKEY((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("nil");
yyNoArgs();
break;
}
}
yybroadcast_NONTERMS(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_NONTERMS, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_NONTERMS((yy)t[1], A, &B, Handler);
*Out = B;
break;
case 2: 
*Out = A;
break;
}
}
}
yyeq_OBJECT(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& (t1[1] == t2[1])
&& yyeq_FORMPARAMLIST((yy)t1[2], (yy)t2[2])
;
case 2: return (t2[0] == 2)
&& (t1[1] == t2[1])
&& yyeq_NONTERMKEY((yy)t1[2], (yy)t2[2])
&& yyeq_FORMPARAMLIST((yy)t1[3], (yy)t2[3])
;
}
}
yyPrint_OBJECT(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("tokenobj");
yyFirstArg();
yyPrint_INT((yy)t[1]);
yyNextArg();
yyPrint_FORMPARAMLIST((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("nontermobj");
yyFirstArg();
yyPrint_INT((yy)t[1]);
yyNextArg();
yyPrint_NONTERMKEY((yy)t[2]);
yyNextArg();
yyPrint_FORMPARAMLIST((yy)t[3]);
yyEndArgs();
break;
}
}
yybroadcast_OBJECT(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_OBJECT, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_FORMPARAMLIST((yy)t[2], A, &B, Handler);
*Out = B;
break;
case 2: 
yybroadcast_FORMPARAMLIST((yy)t[3], A, &B, Handler);
*Out = B;
break;
}
}
}
yy yyglov_UniqueNontermNumber = (yy) yyu;
yyeq_IDENT(t1, t2) yy t1, t2;
{
return t1 == t2;
}
yyPrint_IDENT(t) yy t;
{
yyPrintOpaque(t);
}
yybroadcast_IDENT(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_IDENT, t, In, Out)) {
*Out = In;}
}
yy yyglov_TokenList = (yy) yyu;
yy yyglov_TokenCount = (yy) yyu;
yy yyglov_NextTokenNumber = (yy) yyu;
yyeq_TOKENLIST(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&& yyeq_TOKENENTRY((yy)t1[1], (yy)t2[1])
&& yyeq_TOKENLIST((yy)t1[2], (yy)t2[2])
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_TOKENLIST(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("list");
yyFirstArg();
yyPrint_TOKENENTRY((yy)t[1]);
yyNextArg();
yyPrint_TOKENLIST((yy)t[2]);
yyEndArgs();
break;
case 2: 
yyTerm("nil");
yyNoArgs();
break;
}
}
yybroadcast_TOKENLIST(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_TOKENLIST, t, In, Out)) {
switch(t[0]) {
case 1: 
yybroadcast_TOKENENTRY((yy)t[1], A, &B, Handler);
yybroadcast_TOKENLIST((yy)t[2], B, &A, Handler);
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yyeq_TOKENENTRY(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
&&(strcmp((char *) t1[1], (char *) t2[1]) == 0)
&& (t1[2] == t2[2])
;
}
}
yyPrint_TOKENENTRY(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("token");
yyFirstArg();
yyPrint_STRING((yy)t[1]);
yyNextArg();
yyPrint_INT((yy)t[2]);
yyEndArgs();
break;
}
}
yybroadcast_TOKENENTRY(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_TOKENENTRY, t, In, Out)) {
switch(t[0]) {
case 1: 
*Out = A;
break;
}
}
}
yyeq_Bool(t1, t2) yy t1, t2;
{
switch(t1[0]) {
case 1: return (t2[0] == 1)
;
case 2: return (t2[0] == 2)
;
}
}
yyPrint_Bool(t) yy t;
{
switch(t[0]) {
case 1: 
yyTerm("yes");
yyNoArgs();
break;
case 2: 
yyTerm("no");
yyNoArgs();
break;
}
}
yybroadcast_Bool(t,In,Out,Handler)
yy t, In, *Out; int (*Handler) ();
{
yy A, B;
A = In;
if (! Handler(yybroadcast_Bool, t, In, Out)) {
switch(t[0]) {
case 1: 
*Out = A;
break;
case 2: 
*Out = A;
break;
}
}
}
yy yyglov_GentleFlag = (yy) yyu;
int yyparse_rc = 0;
ROOT()
{
extern char *THIS_RUNTIME_SYSTEM;
char *GENTLE = "Gentle 3.0 01100401 (C) 1992, 1997";
if (strcmp(THIS_RUNTIME_SYSTEM, GENTLE))
{ printf("INCONSISTENT GENTLE RUNTIME SYSTEM\n"); exit(1); }
yyExtend();
yyparse_rc = yyparse();
}
Process(yyin_1)
yy yyin_1;
{
{
yy yyb;
yy yyv_L;
yy yy_0_1;
yy yy_1_1_1_1;
yy yy_1_1_1_2;
yy yy_1_1_1_2_1;
yy yyv_UserRoot;
yy yy_1_1_1_2_1_1;
yy yy_1_1_1_2_1_2;
yy yy_1_1_1_2_1_3;
yy yy_1_1_1_2_1_4;
yy yy_1_1_1_2_1_5;
yy yyv_Pos;
yy yy_1_1_1_2_1_6;
yy yy_1_1_1_2_2;
yy yy_1_2_1_1;
yy yy_1_2_1_2;
yy yy_1_2_1_2_1;
yy yy_1_2_1_2_1_1;
yy yy_1_2_1_2_1_2;
yy yy_1_2_1_2_1_3;
yy yy_1_2_1_2_1_4;
yy yy_1_2_1_2_2;
yy yy_2_1;
yy yy_2_2;
yy yyv_SR;
yy yy_2_3;
yy yy_3_1;
yy yy_3_2;
yy yy_3_2_1;
yy yy_3_2_2;
yy yyv_Formals;
yy yy_3_2_3;
yy yy_4_1_1_1;
yy yy_4_1_1_2;
yy yy_4_1_2_1;
yy yy_4_1_2_2;
yy yy_4_1_2_3;
yy yy_4_1_2_4;
yy yy_5_1;
yy yy_5_1_1;
yy yy_5_1_2;
yy yyv_L2;
yy yy_5_2;
yy yy_6_1;
yy yy_8_1;
yy_0_1 = yyin_1;
yyv_L = yy_0_1;
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
{
yy yysb = yyb;
yy_1_1_1_1 = yyv_L;
yy_1_1_1_2 = yy_1_1_1_1;
if (yy_1_1_1_2[0] != 1) goto yyfl_2_1_1_1;
yy_1_1_1_2_1 = ((yy)yy_1_1_1_2[1]);
yy_1_1_1_2_2 = ((yy)yy_1_1_1_2[2]);
if (yy_1_1_1_2_1[0] != 1) goto yyfl_2_1_1_1;
yy_1_1_1_2_1_1 = ((yy)yy_1_1_1_2_1[1]);
yy_1_1_1_2_1_2 = ((yy)yy_1_1_1_2_1[2]);
yy_1_1_1_2_1_3 = ((yy)yy_1_1_1_2_1[3]);
yy_1_1_1_2_1_4 = ((yy)yy_1_1_1_2_1[4]);
yy_1_1_1_2_1_5 = ((yy)yy_1_1_1_2_1[5]);
yy_1_1_1_2_1_6 = ((yy)yy_1_1_1_2_1[6]);
yyv_UserRoot = yy_1_1_1_2_1_1;
yyv_Pos = yy_1_1_1_2_1_6;
goto yysl_2_1_1;
yyfl_2_1_1_1 : ;
yy_1_2_1_1 = yyv_L;
yy_1_2_1_2 = yy_1_2_1_1;
if (yy_1_2_1_2[0] != 1) goto yyfl_2_1_1_2;
yy_1_2_1_2_1 = ((yy)yy_1_2_1_2[1]);
yy_1_2_1_2_2 = ((yy)yy_1_2_1_2[2]);
if (yy_1_2_1_2_1[0] != 2) goto yyfl_2_1_1_2;
yy_1_2_1_2_1_1 = ((yy)yy_1_2_1_2_1[1]);
yy_1_2_1_2_1_2 = ((yy)yy_1_2_1_2_1[2]);
yy_1_2_1_2_1_3 = ((yy)yy_1_2_1_2_1[3]);
yy_1_2_1_2_1_4 = ((yy)yy_1_2_1_2_1[4]);
yyv_UserRoot = yy_1_2_1_2_1_1;
yyv_Pos = yy_1_2_1_2_1_4;
goto yysl_2_1_1;
yyfl_2_1_1_2 : ;
goto yyfl_2_1;
yysl_2_1_1 : ;
yyb = yysb;
}
yy_2_1 = yyv_UserRoot;
yy_2_2 = yyv_Pos;
StartRule(yy_2_1, yy_2_2, &yy_2_3);
yyv_SR = yy_2_3;
yy_3_1 = yyv_UserRoot;
if (! HasMeaning(yy_3_1, &yy_3_2)) goto yyfl_2_1;
if (yy_3_2[0] != 2) goto yyfl_2_1;
yy_3_2_1 = ((yy)yy_3_2[1]);
yy_3_2_2 = ((yy)yy_3_2[2]);
yy_3_2_3 = ((yy)yy_3_2[3]);
yyv_Formals = yy_3_2_3;
{
yy yysb = yyb;
yy_4_1_1_1 = yyv_Formals;
yy_4_1_1_2 = yy_4_1_1_1;
if (yy_4_1_1_2[0] != 1) goto yyfl_2_1_4_1;
yy_4_1_2_1 = ((yy)"Startsymbol ('");
yy_4_1_2_2 = yyv_UserRoot;
yy_4_1_2_3 = ((yy)"') can not have parameters");
yy_4_1_2_4 = yyv_Pos;
ErrorI(yy_4_1_2_1, yy_4_1_2_2, yy_4_1_2_3, yy_4_1_2_4);
goto yysl_2_1_4;
yyfl_2_1_4_1 : ;
goto yysl_2_1_4;
yysl_2_1_4 : ;
yyb = yysb;
}
yy_5_1_1 = yyv_SR;
yy_5_1_2 = yyv_L;
yy_5_1 = yyb + 0;
yy_5_1[0] = 1;
yy_5_1[1] = ((long)yy_5_1_1);
yy_5_1[2] = ((long)yy_5_1_2);
yy_5_2 = yy_5_1;
yyv_L2 = yy_5_2;
yy_6_1 = yyv_L2;
grammarcode_rulelist(yy_6_1);
lexinfo();
GetDestCodeFileName(&yy_8_1);
OpenOutput(yy_8_1);
put_actions();
ENCODE();
CloseOutput();
return;
yyfl_2_1 : ;
}
yyErr(2,96);
}
StartRule(yyin_1, yyin_2, yyout_1)
yy yyin_1;
yy yyin_2;
yy *yyout_1;
{
{
yy yyb;
yy yyv_UserRoot;
yy yy_0_1;
yy yyv_Pos;
yy yy_0_2;
yy yy_0_3;
yy yy_1_1;
yy yyv_Id;
yy yy_1_2;
yy yy_2_1;
yy yy_2_1_1;
yy yy_2_1_1_1;
yy yy_2_1_1_2;
yy yy_2_1_1_2_1;
yy yy_2_1_1_3;
yy yy_2_1_1_4;
yy yy_2_1_2;
yy yy_2_1_2_1;
yy yy_2_1_2_1_1;
yy yy_2_1_2_2;
yy yyv_Members;
yy yy_2_2;
yy yyv_Key;
yy yyv_N;
yy yy_4_1;
yy yy_5;
yy yy_6;
yy yy_7_1;
yy yy_7_2;
yy yy_7_3;
yy yy_8_1;
yy yy_8_1_1;
yy yy_8_1_2;
yy yy_8_1_3;
yy yy_8_1_4;
yy yy_8_1_5;
yy yy_8_1_5_1;
yy yy_8_1_6;
yy yyv_StartRule;
yy yy_8_2;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yyv_UserRoot = yy_0_1;
yyv_Pos = yy_0_2;
yyb = yyh;
yyh += 32; if (yyh > yyhx) yyExtend();
yy_1_1 = ((yy)"YYSTART");
string_to_id(yy_1_1, &yy_1_2);
yyv_Id = yy_1_2;
yy_2_1_1_1 = yyv_UserRoot;
yy_2_1_1_2_1 = yyv_Pos;
yy_2_1_1_2 = yyb + 8;
yy_2_1_1_2[0] = 2;
yy_2_1_1_2[1] = ((long)yy_2_1_1_2_1);
yy_2_1_1_3 = yyb + 10;
yy_2_1_1_3[0] = 3;
yy_2_1_1_4 = yyv_Pos;
yy_2_1_1 = yyb + 3;
yy_2_1_1[0] = 1;
yy_2_1_1[1] = ((long)yy_2_1_1_1);
yy_2_1_1[2] = ((long)yy_2_1_1_2);
yy_2_1_1[3] = ((long)yy_2_1_1_3);
yy_2_1_1[4] = ((long)yy_2_1_1_4);
yy_2_1_2_1_1 = ((yy)0);
yy_2_1_2_1 = yyb + 14;
yy_2_1_2_1[0] = 2;
yy_2_1_2_1[1] = ((long)yy_2_1_2_1_1);
yy_2_1_2_2 = yyb + 16;
yy_2_1_2_2[0] = 2;
yy_2_1_2 = yyb + 11;
yy_2_1_2[0] = 1;
yy_2_1_2[1] = ((long)yy_2_1_2_1);
yy_2_1_2[2] = ((long)yy_2_1_2_2);
yy_2_1 = yyb + 0;
yy_2_1[0] = 1;
yy_2_1[1] = ((long)yy_2_1_1);
yy_2_1[2] = ((long)yy_2_1_2);
yy_2_2 = yy_2_1;
yyv_Members = yy_2_2;
yyv_Key = yyb + 17;
yyb[20] = yyu;
yyb[19] = yyu;
yyb[18] = yyu;
FreshRuleNumber(&yy_4_1);
yyv_N = yy_4_1;
yy_5 = yyv_N;
yyv_Key[2] = (long) yy_5;
yy_6 = yyv_Members;
yyv_Key[1] = (long) yy_6;
yy_7_1 = yyv_Id;
yy_7_2 = yyb + 21;
yy_7_2[0] = 2;
yy_7_3 = yyv_Pos;
define_nonterm(yy_7_1, yy_7_2, yy_7_3);
yy_8_1_1 = yyv_Id;
yy_8_1_2 = yyv_Members;
yy_8_1_3 = yyv_Key;
yy_8_1_4 = yyb + 29;
yy_8_1_4[0] = 2;
yy_8_1_5_1 = ((yy)0);
yy_8_1_5 = yyb + 30;
yy_8_1_5[0] = 1;
yy_8_1_5[1] = ((long)yy_8_1_5_1);
yy_8_1_6 = yyv_Pos;
yy_8_1 = yyb + 22;
yy_8_1[0] = 1;
yy_8_1[1] = ((long)yy_8_1_1);
yy_8_1[2] = ((long)yy_8_1_2);
yy_8_1[3] = ((long)yy_8_1_3);
yy_8_1[4] = ((long)yy_8_1_4);
yy_8_1[5] = ((long)yy_8_1_5);
yy_8_1[6] = ((long)yy_8_1_6);
yy_8_2 = yy_8_1;
yyv_StartRule = yy_8_2;
yy_0_3 = yyv_StartRule;
*yyout_1 = yy_0_3;
return;
}
}
FreshRuleNumber(yyout_1)
yy *yyout_1;
{
{
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_N;
yy yy_1;
yy yy_2;
yy yy_2_1;
yy yy_2_2;
yy_1 = yyglov_UniqueRuleNumber;
if (yy_1 == (yy) yyu) yyErr(1,260);
yyv_N = yy_1;
yy_2_1 = yyv_N;
yy_2_2 = ((yy)1);
yy_2 = (yy)(((long)yy_2_1)+((long)yy_2_2));
yyglov_UniqueRuleNumber = yy_2;
yy_0_1_1 = yyv_N;
yy_0_1_2 = ((yy)1);
yy_0_1 = (yy)(((long)yy_0_1_1)+((long)yy_0_1_2));
*yyout_1 = yy_0_1;
return;
}
}
FreshAlternativeNumber(yyout_1)
yy *yyout_1;
{
{
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_N;
yy yy_1;
yy yy_2;
yy yy_2_1;
yy yy_2_2;
yy_1 = yyglov_UniqueAlternativeNumber;
if (yy_1 == (yy) yyu) yyErr(1,267);
yyv_N = yy_1;
yy_2_1 = yyv_N;
yy_2_2 = ((yy)1);
yy_2 = (yy)(((long)yy_2_1)+((long)yy_2_2));
yyglov_UniqueAlternativeNumber = yy_2;
yy_0_1_1 = yyv_N;
yy_0_1_2 = ((yy)1);
yy_0_1 = (yy)(((long)yy_0_1_1)+((long)yy_0_1_2));
*yyout_1 = yy_0_1;
return;
}
}
FreshSubphraseNumber(yyout_1)
yy *yyout_1;
{
{
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_N;
yy yy_1;
yy yy_2;
yy yy_2_1;
yy yy_2_2;
yy_1 = yyglov_UniqueSubphraseNumber;
if (yy_1 == (yy) yyu) yyErr(1,274);
yyv_N = yy_1;
yy_2_1 = yyv_N;
yy_2_2 = ((yy)1);
yy_2 = (yy)(((long)yy_2_1)+((long)yy_2_2));
yyglov_UniqueSubphraseNumber = yy_2;
yy_0_1_1 = yyv_N;
yy_0_1_2 = ((yy)1);
yy_0_1 = (yy)(((long)yy_0_1_1)+((long)yy_0_1_2));
*yyout_1 = yy_0_1;
return;
}
}
FreshTokenNumber(yyout_1)
yy *yyout_1;
{
{
yy yyb;
yy yy_0_1;
yy yyv_N;
yy yy_1;
yy yy_2;
yy yy_2_1;
yy yy_2_2;
yy_1 = yyglov_NextTokenNumber;
if (yy_1 == (yy) yyu) yyErr(1,321);
yyv_N = yy_1;
yy_2_1 = yyv_N;
yy_2_2 = ((yy)1);
yy_2 = (yy)(((long)yy_2_1)+((long)yy_2_2));
yyglov_NextTokenNumber = yy_2;
yy_0_1 = yyv_N;
*yyout_1 = yy_0_1;
return;
}
}
NextAlternativeIndex()
{
{
yy yyb;
yy yyv_N;
yy yy_1;
yy yy_2;
yy yy_2_1;
yy yy_2_2;
yy_1 = yyglov_AlternativeIndex;
if (yy_1 == (yy) yyu) yyErr(1,395);
yyv_N = yy_1;
yy_2_1 = yyv_N;
yy_2_2 = ((yy)1);
yy_2 = (yy)(((long)yy_2_1)+((long)yy_2_2));
yyglov_AlternativeIndex = yy_2;
return;
}
}
define_token(yyin_1, yyin_2, yyin_3, yyin_4)
yy yyin_1;
yy yyin_2;
yy yyin_3;
yy yyin_4;
{
{
yy yyb = NULL;
yy yyv_Id = NULL;
yy yy_0_1 = NULL;
yy yyv_Params = NULL;
yy yy_0_2 = NULL;
yy yyv_N = NULL;
yy yy_0_3 = NULL;
yy yyv_Pos = NULL;
yy yy_0_4 = NULL;
yy yy_1_1_1_1 = NULL;
yy yyv_Obj = NULL;
yy yy_1_1_1_2 = NULL;
yy yy_1_1_2_1 = NULL;
yy yy_1_1_2_2 = NULL;
yy yy_1_1_2_3 = NULL;
yy yy_1_1_2_4 = NULL;
yy yy_1_2_1_1 = NULL;
yy yy_1_2_1_2 = NULL;
yy yy_1_2_1_2_1 = NULL;
yy yy_1_2_1_2_2 = NULL;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yy_0_3 = yyin_3;
yy_0_4 = yyin_4;
yyv_Id = yy_0_1;
yyv_Params = yy_0_2;
yyv_N = yy_0_3;
yyv_Pos = yy_0_4;
{
yy yysb = yyb;
yy_1_1_1_1 = yyv_Id;
if (! HasMeaning(yy_1_1_1_1, &yy_1_1_1_2)) goto yyfl_79_1_1_1;
yyv_Obj = yy_1_1_1_2;
yy_1_1_2_1 = ((yy)"'");
yy_1_1_2_2 = yyv_Id;
yy_1_1_2_3 = ((yy)"' already defined");
yy_1_1_2_4 = yyv_Pos;
ErrorI(yy_1_1_2_1, yy_1_1_2_2, yy_1_1_2_3, yy_1_1_2_4);
goto yysl_79_1_1;
yyfl_79_1_1_1 : ;
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yy_1_2_1_1 = yyv_Id;
yy_1_2_1_2_1 = yyv_N;
yy_1_2_1_2_2 = yyv_Params;
yy_1_2_1_2 = yyb + 0;
yy_1_2_1_2[0] = 1;
yy_1_2_1_2[1] = ((long)yy_1_2_1_2_1);
yy_1_2_1_2[2] = ((long)yy_1_2_1_2_2);
DefMeaning(yy_1_2_1_1, yy_1_2_1_2);
goto yysl_79_1_1;
yysl_79_1_1 : ;
yyb = yysb;
}
return;
}
}
define_nonterm(yyin_1, yyin_2, yyin_3)
yy yyin_1;
yy yyin_2;
yy yyin_3;
{
{
yy yyb = NULL;
yy yyv_Id = NULL;
yy yy_0_1 = NULL;
yy yyv_FormalParams = NULL;
yy yy_0_2 = NULL;
yy yyv_Pos = NULL;
yy yy_0_3 = NULL;
yy yy_1_1_1_1 = NULL;
yy yyv_Obj = NULL;
yy yy_1_1_1_2 = NULL;
yy yy_1_1_2_1_1_1 = NULL;
yy yy_1_1_2_1_1_2 = NULL;
yy yy_1_1_2_1_2_1 = NULL;
yy yy_1_1_2_1_2_2 = NULL;
yy yy_1_1_2_1_2_3 = NULL;
yy yy_1_1_2_1_2_4 = NULL;
yy yy_1_1_2_2_1_1 = NULL;
yy yy_1_1_2_2_1_2 = NULL;
yy yy_1_1_2_2_1_3 = NULL;
yy yy_1_1_2_2_1_4 = NULL;
yy yyv_N = NULL;
yy yy_1_2_1 = NULL;
yy yy_1_2_2 = NULL;
yy yy_1_2_2_1 = NULL;
yy yy_1_2_2_2 = NULL;
yy yyv_NKey = NULL;
yy yy_1_2_4 = NULL;
yy yy_1_2_5_1 = NULL;
yy yy_1_2_5_2 = NULL;
yy yy_1_2_5_2_1 = NULL;
yy yy_1_2_5_2_1_1 = NULL;
yy yy_1_2_5_2_1_2 = NULL;
yy yy_1_2_5_2_2 = NULL;
yy yy_1_2_5_2_3 = NULL;
yy yyv_L = NULL;
yy yy_1_2_6 = NULL;
yy yy_1_2_7 = NULL;
yy yy_1_2_7_1 = NULL;
yy yy_1_2_7_2 = NULL;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
yy_0_3 = yyin_3;
yyv_Id = yy_0_1;
yyv_FormalParams = yy_0_2;
yyv_Pos = yy_0_3;
{
yy yysb = yyb;
yy_1_1_1_1 = yyv_Id;
if (! HasMeaning(yy_1_1_1_1, &yy_1_1_1_2)) goto yyfl_80_1_1_1;
yyv_Obj = yy_1_1_1_2;
{
yy yysb = yyb;
yy_1_1_2_1_1_1 = yyv_Obj;
yy_1_1_2_1_1_2 = yy_1_1_2_1_1_1;
if (yy_1_1_2_1_1_2[0] != 1) goto yyfl_80_1_1_1_2_1;
yy_1_1_2_1_2_1 = ((yy)"nonterminal '");
yy_1_1_2_1_2_2 = yyv_Id;
yy_1_1_2_1_2_3 = ((yy)"' already defined as token");
yy_1_1_2_1_2_4 = yyv_Pos;
ErrorI(yy_1_1_2_1_2_1, yy_1_1_2_1_2_2, yy_1_1_2_1_2_3, yy_1_1_2_1_2_4);
goto yysl_80_1_1_1_2;
yyfl_80_1_1_1_2_1 : ;
yy_1_1_2_2_1_1 = ((yy)"nonterminal '");
yy_1_1_2_2_1_2 = yyv_Id;
yy_1_1_2_2_1_3 = ((yy)"' defined by more than one rule");
yy_1_1_2_2_1_4 = yyv_Pos;
ErrorI(yy_1_1_2_2_1_1, yy_1_1_2_2_1_2, yy_1_1_2_2_1_3, yy_1_1_2_2_1_4);
goto yysl_80_1_1_1_2;
yysl_80_1_1_1_2 : ;
yyb = yysb;
}
goto yysl_80_1_1;
yyfl_80_1_1_1 : ;
yyb = yyh;
yyh += 11; if (yyh > yyhx) yyExtend();
yy_1_2_1 = yyglov_UniqueNontermNumber;
if (yy_1_2_1 == (yy) yyu) yyErr(1,623);
yyv_N = yy_1_2_1;
yy_1_2_2_1 = yyv_N;
yy_1_2_2_2 = ((yy)1);
yy_1_2_2 = (yy)(((long)yy_1_2_2_1)+((long)yy_1_2_2_2));
yyglov_UniqueNontermNumber = yy_1_2_2;
yyv_NKey = yyb + 0;
yyb[2] = yyu;
yyb[1] = yyu;
yy_1_2_4 = yyb + 3;
yy_1_2_4[0] = 2;
yyv_NKey[1] = (long) yy_1_2_4;
yy_1_2_5_1 = yyv_Id;
yy_1_2_5_2_1_1 = yyv_N;
yy_1_2_5_2_1_2 = ((yy)1);
yy_1_2_5_2_1 = (yy)(((long)yy_1_2_5_2_1_1)+((long)yy_1_2_5_2_1_2));
yy_1_2_5_2_2 = yyv_NKey;
yy_1_2_5_2_3 = yyv_FormalParams;
yy_1_2_5_2 = yyb + 4;
yy_1_2_5_2[0] = 2;
yy_1_2_5_2[1] = ((long)yy_1_2_5_2_1);
yy_1_2_5_2[2] = ((long)yy_1_2_5_2_2);
yy_1_2_5_2[3] = ((long)yy_1_2_5_2_3);
DefMeaning(yy_1_2_5_1, yy_1_2_5_2);
yy_1_2_6 = yyglov_NontermList;
if (yy_1_2_6 == (yy) yyu) yyErr(1,628);
yyv_L = yy_1_2_6;
yy_1_2_7_1 = yyv_L;
yy_1_2_7_2 = yyv_NKey;
yy_1_2_7 = yyb + 8;
yy_1_2_7[0] = 1;
yy_1_2_7[1] = ((long)yy_1_2_7_1);
yy_1_2_7[2] = ((long)yy_1_2_7_2);
yyglov_NontermList = yy_1_2_7;
goto yysl_80_1_1;
yysl_80_1_1 : ;
yyb = yysb;
}
return;
}
}
init_tokenlist()
{
{
yy yyb;
yy yy_1;
yy yy_2;
yy yyv_MAXCHAR;
yy yy_3_1;
yy yy_4;
yy yy_4_1;
yy yy_4_2;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_1 = yyb + 0;
yy_1[0] = 2;
yyglov_TokenList = yy_1;
yy_2 = ((yy)0);
yyglov_TokenCount = yy_2;
get_max_char(&yy_3_1);
yyv_MAXCHAR = yy_3_1;
yy_4_1 = yyv_MAXCHAR;
yy_4_2 = ((yy)1);
yy_4 = (yy)(((long)yy_4_1)+((long)yy_4_2));
yyglov_NextTokenNumber = yy_4;
return;
}
}
TokenNumberToString(yyin_1, yyout_1)
yy yyin_1;
yy *yyout_1;
{
{
yy yyb = NULL;
yy yyv_N = NULL;
yy yy_0_1 = NULL;
yy yy_0_2 = NULL;
yy yyv_L = NULL;
yy yy_1 = NULL;
yy yy_2_1 = NULL;
yy yy_2_2 = NULL;
yy yyv_S = NULL;
yy yy_2_3 = NULL;
yy_0_1 = yyin_1;
yyv_N = yy_0_1;
yy_1 = yyglov_TokenList;
if (yy_1 == (yy) yyu) yyErr(1,669);
yyv_L = yy_1;
yy_2_1 = yyv_L;
yy_2_2 = yyv_N;
lookup_tokenstring(yy_2_1, yy_2_2, &yy_2_3);
yyv_S = yy_2_3;
yy_0_2 = yyv_S;
*yyout_1 = yy_0_2;
return;
}
}
lookup_tokenstring(yyin_1, yyin_2, yyout_1)
yy yyin_1;
yy yyin_2;
yy *yyout_1;
{
{
yy yyb = NULL;
yy yy_0_1 = NULL;
yy yy_0_1_1 = NULL;
yy yyv_S0 = NULL;
yy yy_0_1_1_1 = NULL;
yy yyv_N0 = NULL;
yy yy_0_1_1_2 = NULL;
yy yyv_Tl = NULL;
yy yy_0_1_2 = NULL;
yy yyv_N = NULL;
yy yy_0_2 = NULL;
yy yy_0_3 = NULL;
yy yy_1_1_1_1 = NULL;
yy yy_1_1_1_2 = NULL;
yy yy_1_1_2_1 = NULL;
yy yyv_S = NULL;
yy yy_1_1_2_2 = NULL;
yy yy_1_2_1_1 = NULL;
yy yy_1_2_1_2 = NULL;
yy yy_1_2_1_3 = NULL;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
if (yy_0_1[0] != 1) goto yyfl_93_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
if (yy_0_1_1[0] != 1) goto yyfl_93_1;
yy_0_1_1_1 = ((yy)yy_0_1_1[1]);
yy_0_1_1_2 = ((yy)yy_0_1_1[2]);
yyv_S0 = yy_0_1_1_1;
yyv_N0 = yy_0_1_1_2;
yyv_Tl = yy_0_1_2;
yyv_N = yy_0_2;
{
yy yysb = yyb;
yy_1_1_1_1 = yyv_N0;
yy_1_1_1_2 = yyv_N;
if ((long)yy_1_1_1_1 != (long)yy_1_1_1_2) goto yyfl_93_1_1_1;
yy_1_1_2_1 = yyv_S0;
yy_1_1_2_2 = yy_1_1_2_1;
yyv_S = yy_1_1_2_2;
goto yysl_93_1_1;
yyfl_93_1_1_1 : ;
yy_1_2_1_1 = yyv_Tl;
yy_1_2_1_2 = yyv_N;
lookup_tokenstring(yy_1_2_1_1, yy_1_2_1_2, &yy_1_2_1_3);
yyv_S = yy_1_2_1_3;
goto yysl_93_1_1;
yysl_93_1_1 : ;
yyb = yysb;
}
yy_0_3 = yyv_S;
*yyout_1 = yy_0_3;
return;
yyfl_93_1 : ;
}
yyErr(2,672);
}
NumberOfRules(yyout_1)
yy *yyout_1;
{
{
yy yyb;
yy yy_0_1;
yy yyv_N;
yy yy_1;
yy_1 = yyglov_UniqueRuleNumber;
if (yy_1 == (yy) yyu) yyErr(1,685);
yyv_N = yy_1;
yy_0_1 = yyv_N;
*yyout_1 = yy_0_1;
return;
}
}
NumberOfNonterms(yyout_1)
yy *yyout_1;
{
{
yy yyb;
yy yy_0_1;
yy yyv_N;
yy yy_1;
yy_1 = yyglov_UniqueNontermNumber;
if (yy_1 == (yy) yyu) yyErr(1,689);
yyv_N = yy_1;
yy_0_1 = yyv_N;
*yyout_1 = yy_0_1;
return;
}
}
convert_to_in_parameters(yyin_1, yyout_1)
yy yyin_1;
yy *yyout_1;
{
{
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_1_1;
yy yyv_T;
yy yy_0_1_1_2;
yy yyv_N;
yy yy_0_1_1_3;
yy yyv_P;
yy yy_0_1_1_4;
yy yyv_T1;
yy yy_0_1_2;
yy yy_0_2;
yy yy_0_2_1;
yy yy_0_2_1_1;
yy yy_0_2_1_2;
yy yy_0_2_1_3;
yy yy_0_2_1_4;
yy yy_0_2_2;
yy yy_1_1;
yy yyv_T2;
yy yy_1_2;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 1) goto yyfl_96_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
if (yy_0_1_1[0] != 1) goto yyfl_96_1;
yy_0_1_1_1 = ((yy)yy_0_1_1[1]);
yy_0_1_1_2 = ((yy)yy_0_1_1[2]);
yy_0_1_1_3 = ((yy)yy_0_1_1[3]);
yy_0_1_1_4 = ((yy)yy_0_1_1[4]);
yyv_T = yy_0_1_1_2;
yyv_N = yy_0_1_1_3;
yyv_P = yy_0_1_1_4;
yyv_T1 = yy_0_1_2;
yyb = yyh;
yyh += 9; if (yyh > yyhx) yyExtend();
yy_1_1 = yyv_T1;
convert_to_in_parameters(yy_1_1, &yy_1_2);
yyv_T2 = yy_1_2;
yy_0_2_1_1 = yyb + 8;
yy_0_2_1_1[0] = 1;
yy_0_2_1_2 = yyv_T;
yy_0_2_1_3 = yyv_N;
yy_0_2_1_4 = yyv_P;
yy_0_2_1 = yyb + 3;
yy_0_2_1[0] = 1;
yy_0_2_1[1] = ((long)yy_0_2_1_1);
yy_0_2_1[2] = ((long)yy_0_2_1_2);
yy_0_2_1[3] = ((long)yy_0_2_1_3);
yy_0_2_1[4] = ((long)yy_0_2_1_4);
yy_0_2_2 = yyv_T2;
yy_0_2 = yyb + 0;
yy_0_2[0] = 1;
yy_0_2[1] = ((long)yy_0_2_1);
yy_0_2[2] = ((long)yy_0_2_2);
*yyout_1 = yy_0_2;
return;
yyfl_96_1 : ;
}
{
yy yyb;
yy yy_0_1;
yy yy_0_2;
yy_0_1 = yyin_1;
if (yy_0_1[0] != 2) goto yyfl_96_2;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_0_2 = yyb + 0;
yy_0_2[0] = 2;
*yyout_1 = yy_0_2;
return;
yyfl_96_2 : ;
}
yyErr(2,693);
}
append_parameter_lists(yyin_1, yyin_2, yyout_1)
yy yyin_1;
yy yyin_2;
yy *yyout_1;
{
{
yy yyb;
yy yy_0_1;
yy yyv_H;
yy yy_0_1_1;
yy yyv_T;
yy yy_0_1_2;
yy yyv_L;
yy yy_0_2;
yy yy_0_3;
yy yy_0_3_1;
yy yy_0_3_2;
yy yy_1_1;
yy yy_1_2;
yy yyv_TL;
yy yy_1_3;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
if (yy_0_1[0] != 1) goto yyfl_97_1;
yy_0_1_1 = ((yy)yy_0_1[1]);
yy_0_1_2 = ((yy)yy_0_1[2]);
yyv_H = yy_0_1_1;
yyv_T = yy_0_1_2;
yyv_L = yy_0_2;
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yy_1_1 = yyv_T;
yy_1_2 = yyv_L;
append_parameter_lists(yy_1_1, yy_1_2, &yy_1_3);
yyv_TL = yy_1_3;
yy_0_3_1 = yyv_H;
yy_0_3_2 = yyv_TL;
yy_0_3 = yyb + 0;
yy_0_3[0] = 1;
yy_0_3[1] = ((long)yy_0_3_1);
yy_0_3[2] = ((long)yy_0_3_2);
*yyout_1 = yy_0_3;
return;
yyfl_97_1 : ;
}
{
yy yyb;
yy yy_0_1;
yy yyv_L;
yy yy_0_2;
yy yy_0_3;
yy_0_1 = yyin_1;
yy_0_2 = yyin_2;
if (yy_0_1[0] != 2) goto yyfl_97_2;
yyv_L = yy_0_2;
yy_0_3 = yyv_L;
*yyout_1 = yy_0_3;
return;
yyfl_97_2 : ;
}
yyErr(2,699);
}
