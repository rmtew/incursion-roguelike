#define NULL 0
typedef long * yy;
void indentation(yy yyin_1);
#define yyu (-2147483647L)
static yy yynull;
extern yy yyh;
extern yy yyhx;
static yyErr(n,l)
{
    yyAbort(n,"actions", l);
}
extern yy yyglov_LHS_List;
extern yy yyglov_GentleFlag;
extern yy yyglov_NontermList;
extern yy yyglov_PreludeBlock;
yy yyglov_GrammarClass = (yy) yyu;
yyeq_GRAMMARCLASS(t1, t2) yy t1, t2;
{
    switch(t1[0]) {
    case 1: return (t2[0] == 1)
                ;
    case 2: return (t2[0] == 2)
                ;
    }
}
yyPrint_GRAMMARCLASS(t) yy t;
{
    switch(t[0]) {
    case 1: 
        yyTerm("ll1");
        yyNoArgs();
        break;
    case 2: 
        yyTerm("cfg");
        yyNoArgs();
        break;
    }
}
yybroadcast_GRAMMARCLASS(t,In,Out,Handler)
    yy t, In, *Out; int (*Handler) ();
{
    yy A, B;
    A = In;
    if (! Handler(yybroadcast_GRAMMARCLASS, t, In, Out)) {
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
yy yyglov_LabelCount = (yy) yyu;
yy yyglov_LocalNames = (yy) yyu;
yyeq_DEFLIST(t1, t2) yy t1, t2;
{
    switch(t1[0]) {
    case 1: return (t2[0] == 1)
                && yyeq_DEF((yy)t1[1], (yy)t2[1])
                && yyeq_DEFLIST((yy)t1[2], (yy)t2[2])
                ;
    case 2: return (t2[0] == 2)
                ;
    }
}
yyPrint_DEFLIST(t) yy t;
{
    switch(t[0]) {
    case 1: 
        yyTerm("list");
        yyFirstArg();
        yyPrint_DEF((yy)t[1]);
        yyNextArg();
        yyPrint_DEFLIST((yy)t[2]);
        yyEndArgs();
        break;
    case 2: 
        yyTerm("nil");
        yyNoArgs();
        break;
    }
}
yybroadcast_DEFLIST(t,In,Out,Handler)
    yy t, In, *Out; int (*Handler) ();
{
    yy A, B;
    A = In;
    if (! Handler(yybroadcast_DEFLIST, t, In, Out)) {
        switch(t[0]) {
        case 1: 
            yybroadcast_DEF((yy)t[1], A, &B, Handler);
            yybroadcast_DEFLIST((yy)t[2], B, &A, Handler);
            *Out = A;
            break;
        case 2: 
            *Out = A;
            break;
        }
    }
}
yyeq_DEF(t1, t2) yy t1, t2;
{
    switch(t1[0]) {
    case 1: return (t2[0] == 1)
                && yyeq_MODE((yy)t1[1], (yy)t2[1])
                && yyeq_IDENT((yy)t1[2], (yy)t2[2])
                && yyeq_IDENT((yy)t1[3], (yy)t2[3])
                ;
    }
}
yyPrint_DEF(t) yy t;
{
    switch(t[0]) {
    case 1: 
        yyTerm("def");
        yyFirstArg();
        yyPrint_MODE((yy)t[1]);
        yyNextArg();
        yyPrint_IDENT((yy)t[2]);
        yyNextArg();
        yyPrint_IDENT((yy)t[3]);
        yyEndArgs();
        break;
    }
}
yybroadcast_DEF(t,In,Out,Handler)
    yy t, In, *Out; int (*Handler) ();
{
    yy A, B;
    A = In;
    if (! Handler(yybroadcast_DEF, t, In, Out)) {
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
put_actions()
{
    {
        yy yyb;
        yy yy_1;
        yy yyv_Blk;
        yy yy_2;
        yy yy_3_1;
        yy yy_3_2;
        yy yy_3_3;
        yy yy_4_1;
        yy yy_6_1_1;
        yy yy_6_2_1_1;
        yy yy_8;
        yy yyv_Keys;
        yy yy_9;
        yy yy_10_1;
        yyb = yyh;
        yyh += 1; if (yyh > yyhx) yyExtend();
        yy_1 = yyb + 0;
        yy_1[0] = 2;
        yyglov_GrammarClass = yy_1;
        yy_2 = yyglov_PreludeBlock;
        if (yy_2 == (yy) yyu) yyErr(1,39);
        yyv_Blk = yy_2;
        yy_3_1 = ((yy)"");
        yy_3_2 = yyv_Blk;
        yy_3_3 = ((yy)"");
        put_CodeBlock(yy_3_1, yy_3_2, yy_3_3);
        yy_4_1 = ((yy)"#include \"yygram.h\"");
        Put(yy_4_1);
        Nl();
        {
            yy yysb = yyb;
            yy_6_1_1 = yyglov_GrammarClass;
            if (yy_6_1_1 == (yy) yyu) yyErr(1,46);
            if (yy_6_1_1[0] != 2) goto yyfl_1_1_6_1;
            goto yysl_1_1_6;
yyfl_1_1_6_1 : ;
            yy_6_2_1_1 = ((yy)"int yycursym;");
            Put(yy_6_2_1_1);
            goto yysl_1_1_6;
yysl_1_1_6 : ;
            yyb = yysb;
        }
        init_indentation();
        yy_8 = ((yy)1);
        yyglov_LabelCount = yy_8;
        yy_9 = yyglov_LHS_List;
        if (yy_9 == (yy) yyu) yyErr(1,52);
        yyv_Keys = yy_9;
        yy_10_1 = yyv_Keys;
        put_actions_LHSKEYS(yy_10_1);
        fix_code();
        return;
    }
}
put_actions_LHSKEYS(yyin_1)
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
        put_actions_LHSKEYS(yy_1_1);
        yy_2_1 = yyv_Key;
        put_actions_LHSKEY(yy_2_1);
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
    yyErr(2,94);
}
put_actions_LHSKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb = NULL;
        yy yyv_Key = NULL;
        yy yy_0_1 = NULL;
        yy yyv_Lhs = NULL;
        yy yy_1 = NULL;
        yy yy_2_1_1_1 = NULL;
        yy yy_2_1_1_2 = NULL;
        yy yyv_Id = NULL;
        yy yy_2_1_1_2_1 = NULL;
        yy yy_2_1_2_1 = NULL;
        yy yy_2_1_2_2 = NULL;
        yy yy_2_1_2_2_1 = NULL;
        yy yy_2_1_2_2_2 = NULL;
        yy yyv_FP = NULL;
        yy yy_2_1_2_2_3 = NULL;
        yy yy_2_1_4_1 = NULL;
        yy yyv_Str = NULL;
        yy yy_2_1_4_2 = NULL;
        yy yy_2_1_5_1 = NULL;
        yy yy_2_1_6_1 = NULL;
        yy yy_2_1_7_1 = NULL;
        yy yy_2_1_8_1 = NULL;
        yy yy_2_1_10 = NULL;
        yy yy_2_1_11_1 = NULL;
        yy yyv_Clauses = NULL;
        yy yy_2_1_12 = NULL;
        yy yy_2_1_13_1 = NULL;
        yy yy_2_1_15_1 = NULL;
        yy yy_2_1_15_1_1 = NULL;
        yy yy_2_1_16_1 = NULL;
        yy yyv_Prelude = NULL;
        yy yy_2_1_17 = NULL;
        yy yy_2_1_18_1 = NULL;
        yy yy_2_1_18_2 = NULL;
        yy yy_2_1_18_3 = NULL;
        yy yy_2_1_19_1_1 = NULL;
        yy yy_2_1_19_1_3_1 = NULL;
        yy yy_2_1_20_1 = NULL;
        yy yy_2_1_21_1_1 = NULL;
        yy yy_2_1_21_1_3_1 = NULL;
        yy yy_2_1_21_2_1_1 = NULL;
        yy yy_2_1_22_1 = NULL;
        yy yy_2_1_22_1_1 = NULL;
        yy yy_2_1_24_1 = NULL;

        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = (yy) yyv_Key[1];
        if (yy_1 == (yy) yyu) yyErr(1,103);
        yyv_Lhs = yy_1;
        {
            yy yysb = yyb;
            yyb = yyh;
            yyh += 1; if (yyh > yyhx) yyExtend();
            yy_2_1_1_1 = yyv_Lhs;
            yy_2_1_1_2 = yy_2_1_1_1;
            if (yy_2_1_1_2[0] != 1) goto yyfl_5_1_2_1;
            yy_2_1_1_2_1 = ((yy)yy_2_1_1_2[1]);
            yyv_Id = yy_2_1_1_2_1;
            yy_2_1_2_1 = yyv_Id;
            if (! HasMeaning(yy_2_1_2_1, &yy_2_1_2_2)) goto yyfl_5_1_2_1;
            if (yy_2_1_2_2[0] != 2) goto yyfl_5_1_2_1;
            yy_2_1_2_2_1 = ((yy)yy_2_1_2_2[1]);
            yy_2_1_2_2_2 = ((yy)yy_2_1_2_2[2]);
            yy_2_1_2_2_3 = ((yy)yy_2_1_2_2[3]);
            yyv_FP = yy_2_1_2_2_3;
            Nl();
            /* Write the function name. */
            yy_2_1_4_1 = yyv_Id;
            id_to_string(yy_2_1_4_1, &yy_2_1_4_2);
            yyv_Str = yy_2_1_4_2;
            yy_2_1_5_1 = yyv_Str;
            Put(yy_2_1_5_1);
            /* Fill out the function declaration. */
            yy_2_1_6_1 = ((yy)" (");
            Put(yy_2_1_6_1);
            yy_2_1_7_1 = yyv_FP;
            handle_formal_params_1(yy_2_1_7_1);
            yy_2_1_8_1 = ((yy)")");
            Put(yy_2_1_8_1);

            yy_2_1_10 = yyb + 0;
            yy_2_1_10[0] = 2;
            yyglov_LocalNames = yy_2_1_10;
#ifdef WRITE_OLDSTYLE_PARAMETER_TYPES
            yy_2_1_11_1 = yyv_FP;
            handle_formal_params_2(yy_2_1_11_1);
#endif

            yy_2_1_12 = (yy) yyv_Key[3];
            if (yy_2_1_12 == (yy) yyu) yyErr(1,121);
            yyv_Clauses = yy_2_1_12;

            yy_2_1_13_1 = ((yy)" {");
            Put(yy_2_1_13_1);
            Nl();

            yy_2_1_15_1_1 = ((yy)1);
            yy_2_1_15_1 = (yy)(((long)yy_2_1_15_1_1));
            indentation(yy_2_1_15_1);

            yy_2_1_16_1 = yyv_Clauses;
            local_declarations(yy_2_1_16_1);
            yy_2_1_17 = (yy) yyv_Key[2];
            if (yy_2_1_17 == (yy) yyu) yyErr(1,127);
            yyv_Prelude = yy_2_1_17;

            yy_2_1_18_1 = ((yy)"");
            yy_2_1_18_2 = yyv_Prelude;
            yy_2_1_18_3 = ((yy)"");
            put_CodeBlock(yy_2_1_18_1, yy_2_1_18_2, yy_2_1_18_3);
            {
                yy yysb = yyb;
                yy_2_1_19_1_1 = yyglov_GrammarClass;
                if (yy_2_1_19_1_1 == (yy) yyu) yyErr(1,131);
                if (yy_2_1_19_1_1[0] != 2) goto yyfl_5_1_2_1_19_1;
                indent();
                yy_2_1_19_1_3_1 = ((yy)"switch(yyselect()) {");
                Put(yy_2_1_19_1_3_1);
                Nl();
                goto yysl_5_1_2_1_19;
yyfl_5_1_2_1_19_1 : ;
                indent();
                goto yysl_5_1_2_1_19;
yysl_5_1_2_1_19 : ;
                yyb = yysb;
            }
            yy_2_1_20_1 = yyv_Clauses;
            put_actions_for_nonterm_CLAUSEKEYS(yy_2_1_20_1);
            {
                yy yysb = yyb;
                yy_2_1_21_1_1 = yyglov_GrammarClass;
                if (yy_2_1_21_1_1 == (yy) yyu) yyErr(1,140);
                if (yy_2_1_21_1_1[0] != 2) goto yyfl_5_1_2_1_21_1;
                indent();
                yy_2_1_21_1_3_1 = ((yy)"}");
                Put(yy_2_1_21_1_3_1);
                Nl();
                goto yysl_5_1_2_1_21;
yyfl_5_1_2_1_21_1 : ;
                yy_2_1_21_2_1_1 = ((yy)"yysyntaxerror();");
                Put(yy_2_1_21_2_1_1);
                Nl();
                goto yysl_5_1_2_1_21;
yysl_5_1_2_1_21 : ;
                yyb = yysb;
            }
            yy_2_1_22_1_1 = ((yy)1);
            yy_2_1_22_1 = (yy)(-((long)yy_2_1_22_1_1));
            indentation(yy_2_1_22_1);
            indent();
            yy_2_1_24_1 = ((yy)"}");
            Put(yy_2_1_24_1);
            Nl();
            goto yysl_5_1_2;
yyfl_5_1_2_1 : ;
            goto yysl_5_1_2;
yysl_5_1_2 : ;
            yyb = yysb;
        }
        return;
    }
}
put_actions_for_nonterm_CLAUSEKEYS(yyin_1)
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
        if (yy_0_1[0] != 1) goto yyfl_6_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Keys = yy_0_1_1;
        yyv_Key = yy_0_1_2;
        yy_1_1 = yyv_Keys;
        put_actions_for_nonterm_CLAUSEKEYS(yy_1_1);
        yy_2_1 = yyv_Key;
        put_actions_for_nonterm_CLAUSEKEY(yy_2_1);
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
    yyErr(2,153);
}
put_actions_for_nonterm_CLAUSEKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb = NULL;
        yy yyv_ClauseKey = NULL;
        yy yy_0_1 = NULL;
        yy yy_1 = NULL;
        yy yyv_Members = NULL;
        yy yy_1_1 = NULL;
        yy yyv_Number = NULL;
        yy yy_2 = NULL;
        yy yy_3_1_1 = NULL;
        yy yy_3_1_3_1 = NULL;
        yy yy_3_1_4_1 = NULL;
        yy yy_3_1_5_1 = NULL;
        yy yy_3_2_1_1 = NULL;
        yy yy_3_2_2_1 = NULL;
        yy yy_3_2_3_1 = NULL;
        yy yy_4_1 = NULL;
        yy yy_4_1_1 = NULL;
        yy yy_5_1 = NULL;
        yy yy_6_1_1 = NULL;
        yy yy_6_1_3_1 = NULL;
        yy yy_6_1_5_1 = NULL;
        yy yy_6_1_5_1_1 = NULL;
        yy yy_6_2_1_1 = NULL;
        yy yy_6_2_1_1_1 = NULL;
        yy yy_6_2_3_1 = NULL;
        yy_0_1 = yyin_1;
        yyv_ClauseKey = yy_0_1;
        yy_1 = (yy) yyv_ClauseKey[2];
        if (yy_1 == (yy) yyu) yyErr(1,161);
        if (yy_1[0] != 1) goto yyfl_7_1;
        yy_1_1 = ((yy)yy_1[1]);
        yyv_Members = yy_1_1;
        yy_2 = (yy) yyv_ClauseKey[7];
        if (yy_2 == (yy) yyu) yyErr(1,162);
        yyv_Number = yy_2;
        {
            yy yysb = yyb;
            yy_3_1_1 = yyglov_GrammarClass;
            if (yy_3_1_1 == (yy) yyu) yyErr(1,164);
            if (yy_3_1_1[0] != 2) goto yyfl_7_1_3_1;
            indent();
            yy_3_1_3_1 = ((yy)"case ");
            Put(yy_3_1_3_1);
            yy_3_1_4_1 = yyv_Number;
            PutI(yy_3_1_4_1);
            yy_3_1_5_1 = ((yy)": {");
            Put(yy_3_1_5_1);
            Nl();
            goto yysl_7_1_3;
yyfl_7_1_3_1 : ;
            yy_3_2_1_1 = ((yy)"if (TABLE[");
            Put(yy_3_2_1_1);
            yy_3_2_2_1 = yyv_Number;
            PutI(yy_3_2_2_1);
            yy_3_2_3_1 = ((yy)"][yycursym]) {");
            Put(yy_3_2_3_1);
            Nl();
            goto yysl_7_1_3;
yysl_7_1_3 : ;
            yyb = yysb;
        }
        yy_4_1_1 = ((yy)1);
        yy_4_1 = (yy)(((long)yy_4_1_1));
        indentation(yy_4_1);
        yy_5_1 = yyv_Members;
        put_actions_MEMBERLIST(yy_5_1);
        {
            yy yysb = yyb;
            yy_6_1_1 = yyglov_GrammarClass;
            if (yy_6_1_1 == (yy) yyu) yyErr(1,181);
            if (yy_6_1_1[0] != 2) goto yyfl_7_1_6_1;
            indent();
            yy_6_1_3_1 = ((yy)"} break;");
            Put(yy_6_1_3_1);
            Nl();
            yy_6_1_5_1_1 = ((yy)1);
            yy_6_1_5_1 = (yy)(-((long)yy_6_1_5_1_1));
            indentation(yy_6_1_5_1);
            goto yysl_7_1_6;
yyfl_7_1_6_1 : ;
            yy_6_2_1_1_1 = ((yy)1);
            yy_6_2_1_1 = (yy)(-((long)yy_6_2_1_1_1));
            indentation(yy_6_2_1_1);
            indent();
            yy_6_2_3_1 = ((yy)"} else ");
            Put(yy_6_2_3_1);
            goto yysl_7_1_6;
yysl_7_1_6 : ;
            yyb = yysb;
        }
        return;
yyfl_7_1 : ;
    }
    yyErr(2,159);
}
put_actions_MEMBERLIST(yyin_1)
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
        put_actions_MEMBER(yy_1_1);
        yy_2_1 = yyv_T;
        put_actions_MEMBERLIST(yy_2_1);
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
    yyErr(2,192);
}
put_actions_MEMBER(yyin_1)
    yy yyin_1;
{
    {
        yy yyb = NULL;
        yy yy_0_1;
        yy yyv_Id;
        yy yy_0_1_1;
        yy yyv_Params;
        yy yy_0_1_2;
        yy yy_0_1_3;
        yy yyv_Pos;
        yy yy_0_1_4;
        yy yy_1_1;
        yy yyv_Str;
        yy yy_1_2;
        yy yy_2_1_1_1;
        yy yy_2_1_1_2;
        yy yyv_N;
        yy yy_2_1_1_2_1;
        yy yyv_FParams;
        yy yy_2_1_1_2_2;
        yy yy_2_1_2_1_1;
        yy yy_2_1_2_1_3_1;
        yy yy_2_1_2_2_2_1;
        yy yy_2_1_2_2_3_1;
        yy yy_2_1_2_2_4_1;
        yy yy_2_1_3_1;
        yy yy_2_1_3_2;
        yy yy_2_1_3_3;
        yy yy_2_1_4_1_1;
        yy yy_2_1_4_2_2_1;
        yy yy_2_2_1_1;
        yy yy_2_2_1_2;
        yy yy_2_2_1_2_1;
        yy yy_2_2_1_2_2;
        yy yyv_Formals;
        yy yy_2_2_1_2_3;
        yy yy_2_2_3_1;
        yy yy_2_2_4_1;
        yy yy_2_2_5_1;
        yy yy_2_2_5_2;
        yy yy_2_2_6_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_9_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yy_0_1_4 = ((yy)yy_0_1[4]);
        yyv_Id = yy_0_1_1;
        yyv_Params = yy_0_1_2;
        yyv_Pos = yy_0_1_4;
        yy_1_1 = yyv_Id;
        id_to_string(yy_1_1, &yy_1_2);
        yyv_Str = yy_1_2;
        {
            yy yysb = yyb;
            yy_2_1_1_1 = yyv_Id;
            if (! HasMeaning(yy_2_1_1_1, &yy_2_1_1_2)) goto yyfl_9_1_2_1;
            if (yy_2_1_1_2[0] != 1) goto yyfl_9_1_2_1;
            yy_2_1_1_2_1 = ((yy)yy_2_1_1_2[1]);
            yy_2_1_1_2_2 = ((yy)yy_2_1_1_2[2]);
            yyv_N = yy_2_1_1_2_1;
            yyv_FParams = yy_2_1_1_2_2;
            {
                yy yysb = yyb;
                yy_2_1_2_1_1 = yyglov_GrammarClass;
                if (yy_2_1_2_1_1 == (yy) yyu) yyErr(1,204);
                if (yy_2_1_2_1_1[0] != 2) goto yyfl_9_1_2_1_2_1;
                indent();
                yy_2_1_2_1_3_1 = ((yy)"get_lexval();");
                Put(yy_2_1_2_1_3_1);
                Nl();
                goto yysl_9_1_2_1_2;
yyfl_9_1_2_1_2_1 : ;
                indent();
                yy_2_1_2_2_2_1 = ((yy)"yycheck(");
                Put(yy_2_1_2_2_2_1);
                yy_2_1_2_2_3_1 = yyv_N;
                PutI(yy_2_1_2_2_3_1);
                yy_2_1_2_2_4_1 = ((yy)");");
                Put(yy_2_1_2_2_4_1);
                Nl();
                goto yysl_9_1_2_1_2;
yysl_9_1_2_1_2 : ;
                yyb = yysb;
            }
            yy_2_1_3_1 = yyv_Params;
            yy_2_1_3_2 = yyv_FParams;
            yy_2_1_3_3 = ((yy)1);
            handle_token_params(yy_2_1_3_1, yy_2_1_3_2, yy_2_1_3_3);
            {
                yy yysb = yyb;
                yy_2_1_4_1_1 = yyglov_GrammarClass;
                if (yy_2_1_4_1_1 == (yy) yyu) yyErr(1,218);
                if (yy_2_1_4_1_1[0] != 2) goto yyfl_9_1_2_1_4_1;
                goto yysl_9_1_2_1_4;
yyfl_9_1_2_1_4_1 : ;
                indent();
                yy_2_1_4_2_2_1 = ((yy)"yycursym = yylex();");
                Put(yy_2_1_4_2_2_1);
                Nl();
                goto yysl_9_1_2_1_4;
yysl_9_1_2_1_4 : ;
                yyb = yysb;
            }
            goto yysl_9_1_2;
yyfl_9_1_2_1 : ;
            yy_2_2_1_1 = yyv_Id;
            if (! HasMeaning(yy_2_2_1_1, &yy_2_2_1_2)) goto yyfl_9_1_2_2;
            if (yy_2_2_1_2[0] != 2) goto yyfl_9_1_2_2;
            yy_2_2_1_2_1 = ((yy)yy_2_2_1_2[1]);
            yy_2_2_1_2_2 = ((yy)yy_2_2_1_2[2]);
            yy_2_2_1_2_3 = ((yy)yy_2_2_1_2[3]);
            yyv_Formals = yy_2_2_1_2_3;
            indent();
            yy_2_2_3_1 = yyv_Str;
            Put(yy_2_2_3_1);
            yy_2_2_4_1 = ((yy)"(");
            Put(yy_2_2_4_1);
            yy_2_2_5_1 = yyv_Params;
            yy_2_2_5_2 = yyv_Formals;
            handle_actual_params(yy_2_2_5_1, yy_2_2_5_2);
            yy_2_2_6_1 = ((yy)");");
            Put(yy_2_2_6_1);
            Nl();
            goto yysl_9_1_2;
yyfl_9_1_2_2 : ;
            goto yyfl_9_1;
yysl_9_1_2 : ;
            yyb = yysb;
        }
        return;
yyfl_9_1 : ;
    }
    {
        yy yyb = NULL;
        yy yy_0_1;
        yy yyv_N;
        yy yy_0_1_1;
        yy yy_1_1_1;
        yy yy_1_1_3_1;
        yy yy_1_2_2_1;
        yy yy_1_2_3_1;
        yy yy_1_2_4_1;
        yy yy_1_2_7_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_9_2;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yyv_N = yy_0_1_1;
        {
            yy yysb = yyb;
            yy_1_1_1 = yyglov_GrammarClass;
            if (yy_1_1_1 == (yy) yyu) yyErr(1,235);
            if (yy_1_1_1[0] != 2) goto yyfl_9_2_1_1;
            indent();
            yy_1_1_3_1 = ((yy)"get_lexval();");
            Put(yy_1_1_3_1);
            Nl();
            goto yysl_9_2_1;
yyfl_9_2_1_1 : ;
            indent();
            yy_1_2_2_1 = ((yy)"yycheck(");
            Put(yy_1_2_2_1);
            yy_1_2_3_1 = yyv_N;
            PutI(yy_1_2_3_1);
            yy_1_2_4_1 = ((yy)");");
            Put(yy_1_2_4_1);
            Nl();
            indent();
            yy_1_2_7_1 = ((yy)"yycursym = yylex();");
            Put(yy_1_2_7_1);
            Nl();
            goto yysl_9_2_1;
yysl_9_2_1 : ;
            yyb = yysb;
        }
        return;
yyfl_9_2 : ;
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
        yy yy_6_1;
        yy yy_9_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 3) goto yyfl_9_3;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yy_0_1_4 = ((yy)yy_0_1[4]);
        yy_0_1_5 = ((yy)yy_0_1[5]);
        if (yy_0_1_1[0] != 1) goto yyfl_9_3;
        yyv_Alternatives = yy_0_1_2;
        yyv_Key = yy_0_1_3;
        yyv_Coordinate = yy_0_1_5;
        indent();
        yy_2_1 = ((yy)"switch (yyselect()) {");
        Put(yy_2_1);
        Nl();
        yy_4_1 = yyv_Alternatives;
        put_actions_ALTERNATIVELIST(yy_4_1);
        indent();
        yy_6_1 = ((yy)"default: ;");
        Put(yy_6_1);
        Nl();
        indent();
        yy_9_1 = ((yy)"}");
        Put(yy_9_1);
        Nl();
        return;
yyfl_9_3 : ;
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
        yy yyv_L;
        yy yy_1_1;
        yy yy_3_1;
        yy yy_5_1;
        yy yy_5_1_1;
        yy yy_7_1;
        yy yy_9_1;
        yy yy_11_1;
        yy yy_12_1;
        yy yy_13_1;
        yy yy_16_1;
        yy yy_18_1;
        yy yy_18_1_1;
        yy yy_20_1;
        yy yy_23_1;
        yy yy_24_1;
        yy yy_25_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 3) goto yyfl_9_4;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yy_0_1_4 = ((yy)yy_0_1[4]);
        yy_0_1_5 = ((yy)yy_0_1[5]);
        if (yy_0_1_1[0] != 2) goto yyfl_9_4;
        yyv_Alternatives = yy_0_1_2;
        yyv_Key = yy_0_1_3;
        yyv_Coordinate = yy_0_1_5;
        UniqueLabel(&yy_1_1);
        yyv_L = yy_1_1;
        indent();
        yy_3_1 = ((yy)"while (1) {");
        Put(yy_3_1);
        Nl();
        yy_5_1_1 = ((yy)1);
        yy_5_1 = (yy)(((long)yy_5_1_1));
        indentation(yy_5_1);
        indent();
        yy_7_1 = ((yy)"switch (yyselect()) {");
        Put(yy_7_1);
        Nl();
        yy_9_1 = yyv_Alternatives;
        put_actions_ALTERNATIVELIST(yy_9_1);
        indent();
        yy_11_1 = ((yy)"default: goto yy");
        Put(yy_11_1);
        yy_12_1 = yyv_L;
        PutI(yy_12_1);
        yy_13_1 = ((yy)";");
        Put(yy_13_1);
        Nl();
        indent();
        yy_16_1 = ((yy)"}");
        Put(yy_16_1);
        Nl();
        yy_18_1_1 = ((yy)1);
        yy_18_1 = (yy)(-((long)yy_18_1_1));
        indentation(yy_18_1);
        indent();
        yy_20_1 = ((yy)"}");
        Put(yy_20_1);
        Nl();
        indent();
        yy_23_1 = ((yy)"yy");
        Put(yy_23_1);
        yy_24_1 = yyv_L;
        PutI(yy_24_1);
        yy_25_1 = ((yy)": ;");
        Put(yy_25_1);
        Nl();
        return;
yyfl_9_4 : ;
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
        yy yy_6_1;
        yy yy_9_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 3) goto yyfl_9_5;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yy_0_1_4 = ((yy)yy_0_1[4]);
        yy_0_1_5 = ((yy)yy_0_1[5]);
        if (yy_0_1_1[0] != 3) goto yyfl_9_5;
        yyv_Alternatives = yy_0_1_2;
        yyv_Key = yy_0_1_3;
        yyv_Coordinate = yy_0_1_5;
        indent();
        yy_2_1 = ((yy)"switch (yyselect()) {");
        Put(yy_2_1);
        Nl();
        yy_4_1 = yyv_Alternatives;
        put_actions_ALTERNATIVELIST(yy_4_1);
        indent();
        yy_6_1 = ((yy)"default: printf(\"???\\n\"); exit(1);");
        Put(yy_6_1);
        Nl();
        indent();
        yy_9_1 = ((yy)"}");
        Put(yy_9_1);
        Nl();
        return;
yyfl_9_5 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Blk;
        yy yy_0_1_1;
        yy yy_1_1;
        yy yy_1_2;
        yy yy_1_3;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 4) goto yyfl_9_6;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yyv_Blk = yy_0_1_1;
        yy_1_1 = ((yy)"");
        yy_1_2 = yyv_Blk;
        yy_1_3 = ((yy)"");
        put_CodeBlock(yy_1_1, yy_1_2, yy_1_3);
        return;
yyfl_9_6 : ;
    }
    yyErr(2,198);
}
put_actions_ALTERNATIVELIST(yyin_1)
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
        if (yy_0_1[0] != 1) goto yyfl_10_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_H = yy_0_1_1;
        yyv_T = yy_0_1_2;
        yy_1_1 = yyv_H;
        put_actions_ALTERNATIVE(yy_1_1);
        yy_2_1 = yyv_T;
        put_actions_ALTERNATIVELIST(yy_2_1);
        return;
yyfl_10_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_10_2;
        return;
yyfl_10_2 : ;
    }
    yyErr(2,306);
}
put_actions_ALTERNATIVE(yyin_1)
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
        yy yyv_ClauseKey;
        yy yy_2;
        yy yyv_Number;
        yy yy_3;
        yy yy_4_1;
        yy yy_5_1;
        yy yy_6_1;
        yy yy_8_1;
        yy yy_8_1_1;
        yy yy_9_1;
        yy yy_11_1;
        yy yy_13_1;
        yy yy_13_1_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_11_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yy_0_1_4 = ((yy)yy_0_1[4]);
        yyv_Members = yy_0_1_1;
        yyv_AlternativeKey = yy_0_1_2;
        yyv_Coordinate = yy_0_1_4;
        indent();
        yy_2 = (yy) yyv_AlternativeKey[3];
        if (yy_2 == (yy) yyu) yyErr(1,316);
        yyv_ClauseKey = yy_2;
        yy_3 = (yy) yyv_ClauseKey[7];
        if (yy_3 == (yy) yyu) yyErr(1,317);
        yyv_Number = yy_3;
        yy_4_1 = ((yy)"case ");
        Put(yy_4_1);
        yy_5_1 = yyv_Number;
        PutI(yy_5_1);
        yy_6_1 = ((yy)": {");
        Put(yy_6_1);
        Nl();
        yy_8_1_1 = ((yy)1);
        yy_8_1 = (yy)(((long)yy_8_1_1));
        indentation(yy_8_1);
        yy_9_1 = yyv_Members;
        put_actions_MEMBERLIST(yy_9_1);
        indent();
        yy_11_1 = ((yy)"} break;");
        Put(yy_11_1);
        Nl();
        yy_13_1_1 = ((yy)1);
        yy_13_1 = (yy)(-((long)yy_13_1_1));
        indentation(yy_13_1);
        return;
yyfl_11_1 : ;
    }
    yyErr(2,312);
}

handle_formal_params_1(yyin_1)
    yy yyin_1;
{
    {
        yy yyb = NULL;
        yy yy_0_1;
        yy yyv_H;
        yy yy_0_1_1;
        yy yyv_T;
        yy yy_0_1_2;
        yy yy_1_1;
        yy yy_2_1;
        yy yy_2_2;
        yy yy_2_2_1;
        yy yy_2_2_2;
        yy yyv_Name;
        yy yy_2_2_3;
        yy yy_2_2_4;
        yy yy_3_1;
        yy yy_3_2;
        yy yy_4_1_1_1;
        yy yy_4_1_1_2;
        yy yy_4_1_2_1;
        yy yy_5_1;

        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_12_1;

        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_H = yy_0_1_1;
        yyv_T = yy_0_1_2;
        yy_1_1 = yyv_H;
#ifdef WRITE_OLDSTYLE_PARAMETER_TYPES
        handle_formal_param_1(yy_1_1);
#else
        handle_formal_param_2(yy_1_1);
#endif
        yy_2_1 = yyv_H;
        yy_2_2 = yy_2_1;
        if (yy_2_2[0] != 1) goto yyfl_12_1;

        yy_2_2_1 = ((yy)yy_2_2[1]);
        yy_2_2_2 = ((yy)yy_2_2[2]);
        yy_2_2_3 = ((yy)yy_2_2[3]);
        yy_2_2_4 = ((yy)yy_2_2[4]);
        yyv_Name = yy_2_2_3;
        yy_3_1 = yyv_Name;
        yy_3_2 = yyv_T;
        check_formal_param(yy_3_1, yy_3_2);
        {
            yy yysb = yyb;
            yy_4_1_1_1 = yyv_T;
            yy_4_1_1_2 = yy_4_1_1_1;
            if (yy_4_1_1_2[0] != 1) goto yyfl_12_1_4_1;
            yy_4_1_2_1 = ((yy)", ");
            Put(yy_4_1_2_1);
            goto yysl_12_1_4;
yyfl_12_1_4_1 : ;
            goto yysl_12_1_4;
yysl_12_1_4 : ;
            yyb = yysb;
        }
        yy_5_1 = yyv_T;
        handle_formal_params_1(yy_5_1);
        return;
yyfl_12_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_12_2;
        return;
yyfl_12_2 : ;
    }
    yyErr(2,357);
}

handle_formal_param_1(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Mode;
        yy yy_0_1_1;
        yy yyv_Type;
        yy yy_0_1_2;
        yy yyv_Name;
        yy yy_0_1_3;
        yy yyv_Pos;
        yy yy_0_1_4;
        yy yy_1_1;
        yy yyv_NStr;
        yy yy_1_2;
        yy yy_2_1;

        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_13_1;

        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yy_0_1_4 = ((yy)yy_0_1[4]);
        yyv_Mode = yy_0_1_1;
        yyv_Type = yy_0_1_2;
        yyv_Name = yy_0_1_3;
        yyv_Pos = yy_0_1_4;

        yy_1_1 = yyv_Name;
        id_to_string(yy_1_1, &yy_1_2);
        yyv_NStr = yy_1_2;
        yy_2_1 = yyv_NStr;
        Put(yy_2_1);

        return;
yyfl_13_1 : ;
    }
    yyErr(2,369);
}

check_formal_param(yyin_1, yyin_2)
    yy yyin_1;
yy yyin_2;
{
    {
        yy yyb = NULL;
        yy yyv_Name1;
        yy yy_0_1;
        yy yy_0_2;
        yy yy_0_2_1;
        yy yy_0_2_1_1;
        yy yy_0_2_1_2;
        yy yyv_Name2;
        yy yy_0_2_1_3;
        yy yyv_Pos;
        yy yy_0_2_1_4;
        yy yyv_Tl;
        yy yy_0_2_2;
        yy yy_1_1_1_1;
        yy yy_1_1_1_2;
        yy yy_1_1_2_1;
        yy yy_1_1_2_2;
        yy yy_1_1_2_3;
        yy yy_1_1_2_4;
        yy yy_2_1;
        yy yy_2_2;

        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yyv_Name1 = yy_0_1;
        if (yy_0_2[0] != 1) goto yyfl_14_1;

        yy_0_2_1 = ((yy)yy_0_2[1]);
        yy_0_2_2 = ((yy)yy_0_2[2]);
        if (yy_0_2_1[0] != 1) goto yyfl_14_1;

        yy_0_2_1_1 = ((yy)yy_0_2_1[1]);
        yy_0_2_1_2 = ((yy)yy_0_2_1[2]);
        yy_0_2_1_3 = ((yy)yy_0_2_1[3]);
        yy_0_2_1_4 = ((yy)yy_0_2_1[4]);
        yyv_Name2 = yy_0_2_1_3;
        yyv_Pos = yy_0_2_1_4;
        yyv_Tl = yy_0_2_2;
        {
            yy yysb = yyb;
            yy_1_1_1_1 = yyv_Name1;
            yy_1_1_1_2 = yyv_Name2;
            if (! yyeq_IDENT(yy_1_1_1_1, yy_1_1_1_2)) goto yyfl_14_1_1_1;
            yy_1_1_2_1 = ((yy)"formal parameter '");
            yy_1_1_2_2 = yyv_Name1;
            yy_1_1_2_3 = ((yy)"' declared more than once");
            yy_1_1_2_4 = yyv_Pos;
            ErrorI(yy_1_1_2_1, yy_1_1_2_2, yy_1_1_2_3, yy_1_1_2_4);
            goto yysl_14_1_1;
yyfl_14_1_1_1 : ;
            goto yysl_14_1_1;
yysl_14_1_1 : ;
            yyb = yysb;
        }
        yy_2_1 = yyv_Name1;
        yy_2_2 = yyv_Tl;
        check_formal_param(yy_2_1, yy_2_2);
        return;
yyfl_14_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yy_0_2;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_2[0] != 2) goto yyfl_14_2;
        return;
yyfl_14_2 : ;
    }
    yyErr(2,374);
}

handle_formal_params_2(yyin_1)
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
        if (yy_0_1[0] != 1) goto yyfl_15_1;

        yyv_H = ((yy)yy_0_1[1]);
        yyv_T = ((yy)yy_0_1[2]);
        handle_formal_param_2(yyv_H);
        handle_formal_params_2(yyv_T);
        return;
yyfl_15_1 : ;
    }
    {
        yy yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_15_2;
        return;
yyfl_15_2 : ;
    }
    yyErr(2,385);
}

handle_formal_param_2(yyin_1)
    yy yyin_1;
{
    {
        yy yyb = NULL;
        yy yy_0_1 = NULL;
        yy yyv_Mode = NULL;
        yy yy_0_1_1 = NULL;
        yy yyv_Type = NULL;
        yy yy_0_1_2 = NULL;
        yy yyv_Name = NULL;
        yy yy_0_1_3 = NULL;
        yy yyv_Pos;
        yy yy_0_1_4;
        yy yy_1_1;
        yy yyv_TStr;
        yy yy_1_2;
        yy yy_2_1;
        yy yy_3_1;
        yy yy_4_1;
        yy yy_5_1_1_1;
        yy yy_5_1_1_2;
        yy yy_5_1_2_1;
        yy yy_6_1;
        yy yyv_NStr;
        yy yy_6_2;
        yy yy_7_1;
        yy yy_8_1;
        yy yy_10_1;
        yy yy_10_2;
        yy yy_10_3;

        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_16_1;

        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yy_0_1_4 = ((yy)yy_0_1[4]);
        yyv_Mode = yy_0_1_1;
        yyv_Type = yy_0_1_2;
        yyv_Name = yy_0_1_3;
        yyv_Pos = yy_0_1_4;

        yy_1_1 = yyv_Type;
        id_to_string(yy_1_1, &yy_1_2);
        yyv_TStr = yy_1_2;

#ifdef WRITE_OLDSTYLE_PARAMETER_TYPES
        yy_2_1 = ((yy)"   ");
        Put(yy_2_1);
#endif
        yy_3_1 = yyv_TStr;
        Put(yy_3_1);
        yy_4_1 = ((yy)" ");
        Put(yy_4_1);
        {
            yy yysb = yyb;
            yy_5_1_1_1 = yyv_Mode;
            yy_5_1_1_2 = yy_5_1_1_1;
            if (yy_5_1_1_2[0] != 2) goto yyfl_16_1_5_1;
            yy_5_1_2_1 = ((yy)"*");
            Put(yy_5_1_2_1);
            goto yysl_16_1_5;
yyfl_16_1_5_1 : ;
            goto yysl_16_1_5;
yysl_16_1_5 : ;
            yyb = yysb;
        }
        yy_6_1 = yyv_Name;
        id_to_string(yy_6_1, &yy_6_2);
        yyv_NStr = yy_6_2;
        yy_7_1 = yyv_NStr;
        Put(yy_7_1);
#ifdef WRITE_OLDSTYLE_PARAMETER_TYPES
        yy_8_1 = ((yy)";");
        Put(yy_8_1);
        Nl();
#endif
        yy_10_1 = yyv_Mode;
        yy_10_2 = yyv_Type;
        yy_10_3 = yyv_Name;
        define_local_name(yy_10_1, yy_10_2, yy_10_3);
        return;
yyfl_16_1 : ;
    }
    yyErr(2,393);
}
handle_actual_params(yyin_1, yyin_2)
    yy yyin_1;
yy yyin_2;
{
    {
        yy yyb = NULL;
        yy yy_0_1 = NULL;
        yy yyv_H = NULL;
        yy yy_0_1_1 = NULL;
        yy yyv_T = NULL;
        yy yy_0_1_2 = NULL;
        yy yy_0_1_3 = NULL;
        yy yy_0_2 = NULL;
        yy yyv_H2 = NULL;
        yy yy_0_2_1 = NULL;
        yy yyv_T2 = NULL;
        yy yy_0_2_2 = NULL;
        yy yy_1_1 = NULL;
        yy yy_1_2 = NULL;
        yy yy_2_1_1_1 = NULL;
        yy yy_2_1_1_2 = NULL;
        yy yy_2_1_2_1 = NULL;
        yy yy_3_1 = NULL;
        yy yy_3_2 = NULL;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 1) goto yyfl_17_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yyv_H = yy_0_1_1;
        yyv_T = yy_0_1_2;
        if (yy_0_2[0] != 1) goto yyfl_17_1;
        yy_0_2_1 = ((yy)yy_0_2[1]);
        yy_0_2_2 = ((yy)yy_0_2[2]);
        yyv_H2 = yy_0_2_1;
        yyv_T2 = yy_0_2_2;
        yy_1_1 = yyv_H;
        yy_1_2 = yyv_H2;
        handle_actual_param(yy_1_1, yy_1_2);
        {
            yy yysb = yyb;
            yy_2_1_1_1 = yyv_T;
            yy_2_1_1_2 = yy_2_1_1_1;
            if (yy_2_1_1_2[0] != 1) goto yyfl_17_1_2_1;
            yy_2_1_2_1 = ((yy)", ");
            Put(yy_2_1_2_1);
            goto yysl_17_1_2;
yyfl_17_1_2_1 : ;
            goto yysl_17_1_2;
yysl_17_1_2 : ;
            yyb = yysb;
        }
        yy_3_1 = yyv_T;
        yy_3_2 = yyv_T2;
        handle_actual_params(yy_3_1, yy_3_2);
        return;
yyfl_17_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yy_0_2;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 2) goto yyfl_17_2;
        if (yy_0_2[0] != 2) goto yyfl_17_2;
        return;
yyfl_17_2 : ;
    }
    yyErr(2,413);
}
handle_actual_param(yyin_1, yyin_2)
    yy yyin_1;
yy yyin_2;
{
    {
        yy yyb = NULL;
        yy yy_0_1 = NULL;
        yy yyv_Id = NULL;
        yy yy_0_1_1 = NULL;
        yy yy_0_1_2 = NULL;
        yy yy_0_2 = NULL;
        yy yyv_FMode = NULL;
        yy yy_0_2_1 = NULL;
        yy yyv_FType = NULL;
        yy yy_0_2_2 = NULL;
        yy yy_0_2_3 = NULL;
        yy yy_0_2_4 = NULL;
        yy yy_1_1 = NULL;
        yy yyv_AMode = NULL;
        yy yy_1_2 = NULL;
        yy yy_1_3 = NULL;
        yy yy_2_1_1_1 = NULL;
        yy yy_2_1_1_2 = NULL;
        yy yy_2_1_2_1_1_1 = NULL;
        yy yy_2_1_2_1_1_2 = NULL;
        yy yy_2_1_2_2_1_1 = NULL;
        yy yy_2_2_1_1_1_1 = NULL;
        yy yy_2_2_1_1_1_2 = NULL;
        yy yy_2_2_1_1_2_1 = NULL;
        yy yy_3_1 = NULL;
        yy yyv_Str = NULL;
        yy yy_3_2 = NULL;
        yy yy_4_1 = NULL;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 1) goto yyfl_18_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Id = yy_0_1_1;
        if (yy_0_2[0] != 1) goto yyfl_18_1;
        yy_0_2_1 = ((yy)yy_0_2[1]);
        yy_0_2_2 = ((yy)yy_0_2[2]);
        yy_0_2_3 = ((yy)yy_0_2[3]);
        yy_0_2_4 = ((yy)yy_0_2[4]);
        yyv_FMode = yy_0_2_1;
        yyv_FType = yy_0_2_2;
        yy_1_1 = yyv_Id;
        if (! defined_locally(yy_1_1, &yy_1_2, &yy_1_3)) goto yyfl_18_1;
        yyv_AMode = yy_1_2;
        {
            yy yysb = yyb;
            yy_2_1_1_1 = yyv_FMode;
            yy_2_1_1_2 = yy_2_1_1_1;
            if (yy_2_1_1_2[0] != 2) goto yyfl_18_1_2_1;
            {
                yy yysb = yyb;
                yy_2_1_2_1_1_1 = yyv_AMode;
                yy_2_1_2_1_1_2 = yy_2_1_2_1_1_1;
                if (yy_2_1_2_1_1_2[0] != 2) goto yyfl_18_1_2_1_2_1;
                goto yysl_18_1_2_1_2;
yyfl_18_1_2_1_2_1 : ;
                yy_2_1_2_2_1_1 = ((yy)"&");
                Put(yy_2_1_2_2_1_1);
                goto yysl_18_1_2_1_2;
yysl_18_1_2_1_2 : ;
                yyb = yysb;
            }
            goto yysl_18_1_2;
yyfl_18_1_2_1 : ;
                {
                    yy yysb = yyb;
                    yy_2_2_1_1_1_1 = yyv_AMode;
                    yy_2_2_1_1_1_2 = yy_2_2_1_1_1_1;
                    if (yy_2_2_1_1_1_2[0] != 2) goto yyfl_18_1_2_2_1_1;
                    yy_2_2_1_1_2_1 = ((yy)"*");
                    Put(yy_2_2_1_1_2_1);
                    goto yysl_18_1_2_2_1;
yyfl_18_1_2_2_1_1 : ;
                    goto yysl_18_1_2_2_1;
yysl_18_1_2_2_1 : ;
                    yyb = yysb;
                }
                goto yysl_18_1_2;
yysl_18_1_2 : ;
                yyb = yysb;
        }
        yy_3_1 = yyv_Id;
        id_to_string(yy_3_1, &yy_3_2);
        yyv_Str = yy_3_2;
        yy_4_1 = yyv_Str;
        Put(yy_4_1);
        return;
yyfl_18_1 : ;
    }
    yyErr(2,420);
}
handle_token_params(yyin_1, yyin_2, yyin_3)
    yy yyin_1;
yy yyin_2;
yy yyin_3;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_H;
        yy yy_0_1_1;
        yy yyv_T;
        yy yy_0_1_2;
        yy yy_0_1_3;
        yy yy_0_2;
        yy yyv_H2;
        yy yy_0_2_1;
        yy yyv_T2;
        yy yy_0_2_2;
        yy yyv_N;
        yy yy_0_3;
        yy yy_1_1;
        yy yy_1_2;
        yy yy_1_3;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yy_0_3 = yyin_3;
        if (yy_0_1[0] != 1) goto yyfl_19_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yyv_H = yy_0_1_1;
        yyv_T = yy_0_1_2;
        if (yy_0_2[0] != 1) goto yyfl_19_1;
        yy_0_2_1 = ((yy)yy_0_2[1]);
        yy_0_2_2 = ((yy)yy_0_2[2]);
        yyv_H2 = yy_0_2_1;
        yyv_T2 = yy_0_2_2;
        yyv_N = yy_0_3;
        yy_1_1 = yyv_H;
        yy_1_2 = yyv_H2;
        yy_1_3 = yyv_N;
        handle_token_param(yy_1_1, yy_1_2, yy_1_3);
        return;
yyfl_19_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yy_0_2;
        yy yy_0_3;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yy_0_3 = yyin_3;
        if (yy_0_1[0] != 2) goto yyfl_19_2;
        return;
yyfl_19_2 : ;
    }
    yyErr(2,445);
}
handle_token_param(yyin_1, yyin_2, yyin_3)
    yy yyin_1;
yy yyin_2;
yy yyin_3;
{
    {
        yy yyb = NULL;
        yy yy_0_1 = NULL;
        yy yyv_Id = NULL;
        yy yy_0_1_1 = NULL;
        yy yy_0_1_2 = NULL;
        yy yy_0_2 = NULL;
        yy yyv_FMode = NULL;
        yy yy_0_2_1 = NULL;
        yy yyv_FType = NULL;
        yy yy_0_2_2 = NULL;
        yy yy_0_2_3 = NULL;
        yy yy_0_2_4 = NULL;
        yy yyv_N = NULL;
        yy yy_0_3 = NULL;
        yy yy_1_1 = NULL;
        yy yyv_AMode = NULL;
        yy yy_1_2 = NULL;
        yy yy_1_3 = NULL;
        yy yy_3_1_1_1 = NULL;
        yy yy_3_1_1_2 = NULL;
        yy yy_3_1_2_1_1_1 = NULL;
        yy yy_3_1_2_1_1_2 = NULL;
        yy yy_3_1_2_1_2_1 = NULL;
        yy yy_4_1 = NULL;
        yy yyv_IdStr = NULL;
        yy yy_4_2 = NULL;
        yy yy_5_1 = NULL;
        yy yy_6_1 = NULL;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yy_0_3 = yyin_3;
        if (yy_0_1[0] != 1) goto yyfl_20_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Id = yy_0_1_1;
        if (yy_0_2[0] != 1) goto yyfl_20_1;
        yy_0_2_1 = ((yy)yy_0_2[1]);
        yy_0_2_2 = ((yy)yy_0_2[2]);
        yy_0_2_3 = ((yy)yy_0_2[3]);
        yy_0_2_4 = ((yy)yy_0_2[4]);
        yyv_FMode = yy_0_2_1;
        yyv_FType = yy_0_2_2;
        yyv_N = yy_0_3;
        yy_1_1 = yyv_Id;
        if (! defined_locally(yy_1_1, &yy_1_2, &yy_1_3)) goto yyfl_20_1;
        yyv_AMode = yy_1_2;
        indent();
        {
            yy yysb = yyb;
            yy_3_1_1_1 = yyv_FMode;
            yy_3_1_1_2 = yy_3_1_1_1;
            if (yy_3_1_1_2[0] != 2) goto yyfl_20_1_3_1;
            {
                yy yysb = yyb;
                yy_3_1_2_1_1_1 = yyv_AMode;
                yy_3_1_2_1_1_2 = yy_3_1_2_1_1_1;
                if (yy_3_1_2_1_1_2[0] != 2) goto yyfl_20_1_3_1_2_1;
                yy_3_1_2_1_2_1 = ((yy)"*");
                Put(yy_3_1_2_1_2_1);
                goto yysl_20_1_3_1_2;
yyfl_20_1_3_1_2_1 : ;
                goto yysl_20_1_3_1_2;
yysl_20_1_3_1_2 : ;
                yyb = yysb;
            }
            goto yysl_20_1_3;
yyfl_20_1_3_1 : ;
            goto yysl_20_1_3;
yysl_20_1_3 : ;
            yyb = yysb;
        }
        yy_4_1 = yyv_Id;
        id_to_string(yy_4_1, &yy_4_2);
        yyv_IdStr = yy_4_2;
        yy_5_1 = yyv_IdStr;
        Put(yy_5_1);
        yy_6_1 = ((yy)" = yylval;");
        Put(yy_6_1);
        Nl();
        return;
yyfl_20_1 : ;
    }
    yyErr(2,451);
}
UniqueLabel(yyout_1)
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
        yy_1 = yyglov_LabelCount;
        if (yy_1 == (yy) yyu) yyErr(1,480);
        yyv_N = yy_1;
        yy_2_1 = yyv_N;
        yy_2_2 = ((yy)1);
        yy_2 = (yy)(((long)yy_2_1)+((long)yy_2_2));
        yyglov_LabelCount = yy_2;
        yy_0_1 = yyv_N;
        *yyout_1 = yy_0_1;
        return;
    }
}
local_declarations(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_CLAUSEKEYS;
        yy yy_0_1;
        yy yy_1_1;
        yy_0_1 = yyin_1;
        yyv_CLAUSEKEYS = yy_0_1;
        yy_1_1 = yyv_CLAUSEKEYS;
        local_declarations_CLAUSEKEYS(yy_1_1);
        return;
    }
}
local_declarations_CLAUSEKEYS(yyin_1)
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
        if (yy_0_1[0] != 1) goto yyfl_24_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Keys = yy_0_1_1;
        yyv_Key = yy_0_1_2;
        yy_1_1 = yyv_Keys;
        local_declarations_CLAUSEKEYS(yy_1_1);
        yy_2_1 = yyv_Key;
        local_declarations_CLAUSEKEY(yy_2_1);
        return;
yyfl_24_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_24_2;
        return;
yyfl_24_2 : ;
    }
    yyErr(2,490);
}
local_declarations_CLAUSEKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_ClauseKey;
        yy yy_0_1;
        yy yy_1;
        yy yyv_Members;
        yy yy_1_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        yyv_ClauseKey = yy_0_1;
        yy_1 = (yy) yyv_ClauseKey[2];
        if (yy_1 == (yy) yyu) yyErr(1,498);
        if (yy_1[0] != 1) goto yyfl_25_1;
        yy_1_1 = ((yy)yy_1[1]);
        yyv_Members = yy_1_1;
        yy_2_1 = yyv_Members;
        {
            extern local_declarations_sweep();
            yybroadcast_MEMBERLIST(yy_2_1, 0, &yynull, local_declarations_sweep);
        }
        return;
yyfl_25_1 : ;
    }
    yyErr(2,496);
}
int local_declarations_sweep(yytp, yyin_1, yyin_2, yyout_1 )
    long yytp;
yy yyin_1, yyin_2, *yyout_1;
{
    {
        extern yybroadcast_MEMBER();
        /*=== Sweep MEMBER ===*/
        if (yytp == (long) yybroadcast_MEMBER) {
            {
                yy yyb = NULL;
                yy yy_0_1 = NULL;
                yy yyv_Id = NULL;
                yy yy_0_1_1 = NULL;
                yy yyv_ActParams = NULL;
                yy yy_0_1_2 = NULL;
                yy yy_0_1_3 = NULL;
                yy yyv_Pos = NULL;
                yy yy_0_1_4 = NULL;
                yy yy_1_1 = NULL;
                yy yyv_Meaning = NULL;
                yy yy_1_2 = NULL;
                yy yy_2_1_1_1 = NULL;
                yy yy_2_1_1_2 = NULL;
                yy yy_2_1_1_2_1 = NULL;
                yy yy_2_1_1_2_2 = NULL;
                yy yyv_FormalParams = NULL;
                yy yy_2_1_1_2_3 = NULL;
                yy yy_2_1_2_1 = NULL;
                yy yy_2_1_2_2 = NULL;
                yy yy_2_2_1_1 = NULL;
                yy yy_2_2_1_2 = NULL;
                yy yy_2_2_1_2_1 = NULL;
                yy yy_2_2_1_2_2 = NULL;
                yy yy_2_2_2_1 = NULL;
                yy_0_1 = yyin_1;
                if (yy_0_1[0] != 1) goto yyfl_26_10001;
                yy_0_1_1 = ((yy)yy_0_1[1]);
                yy_0_1_2 = ((yy)yy_0_1[2]);
                yy_0_1_3 = ((yy)yy_0_1[3]);
                yy_0_1_4 = ((yy)yy_0_1[4]);
                yyv_Id = yy_0_1_1;
                yyv_ActParams = yy_0_1_2;
                yyv_Pos = yy_0_1_4;
                yy_1_1 = yyv_Id;
                if (! HasMeaning(yy_1_1, &yy_1_2)) goto yyfl_26_10001;
                yyv_Meaning = yy_1_2;
                {
                    yy yysb = yyb;
                    yy_2_1_1_1 = yyv_Meaning;
                    yy_2_1_1_2 = yy_2_1_1_1;
                    if (yy_2_1_1_2[0] != 2) goto yyfl_26_10001_2_1;
                    yy_2_1_1_2_1 = ((yy)yy_2_1_1_2[1]);
                    yy_2_1_1_2_2 = ((yy)yy_2_1_1_2[2]);
                    yy_2_1_1_2_3 = ((yy)yy_2_1_1_2[3]);
                    yyv_FormalParams = yy_2_1_1_2_3;
                    yy_2_1_2_1 = yyv_FormalParams;
                    yy_2_1_2_2 = yyv_ActParams;
                    process_formal_and_actual_params(yy_2_1_2_1, yy_2_1_2_2);
                    goto yysl_26_10001_2;
yyfl_26_10001_2_1 : ;
                    yy_2_2_1_1 = yyv_Meaning;
                    yy_2_2_1_2 = yy_2_2_1_1;
                    if (yy_2_2_1_2[0] != 1) goto yyfl_26_10001_2_2;
                    yy_2_2_1_2_1 = ((yy)yy_2_2_1_2[1]);
                    yy_2_2_1_2_2 = ((yy)yy_2_2_1_2[2]);
                    yyv_FormalParams = yy_2_2_1_2_2;
                    yy_2_2_2_1 = yyv_ActParams;
                    process_token_param(yy_2_2_2_1);
                    goto yysl_26_10001_2;
yyfl_26_10001_2_2 : ;
                    goto yyfl_26_10001;
yysl_26_10001_2 : ;
                    yyb = yysb;
                }
                *yyout_1 = yyin_2;
                return 1;
yyfl_26_10001 : ;
            }
            return 0;
        }
        /*===*/
        else return 0;
    }
}
process_formal_and_actual_params(yyin_1, yyin_2)
    yy yyin_1;
yy yyin_2;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_FHd;
        yy yy_0_1_1;
        yy yyv_FTl;
        yy yy_0_1_2;
        yy yy_0_2;
        yy yyv_AHd;
        yy yy_0_2_1;
        yy yyv_ATl;
        yy yy_0_2_2;
        yy yyv_Pos;
        yy yy_0_2_3;
        yy yy_1_1;
        yy yy_1_2;
        yy yy_2_1;
        yy yy_2_2;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 1) goto yyfl_27_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_FHd = yy_0_1_1;
        yyv_FTl = yy_0_1_2;
        if (yy_0_2[0] != 1) goto yyfl_27_1;
        yy_0_2_1 = ((yy)yy_0_2[1]);
        yy_0_2_2 = ((yy)yy_0_2[2]);
        yy_0_2_3 = ((yy)yy_0_2[3]);
        yyv_AHd = yy_0_2_1;
        yyv_ATl = yy_0_2_2;
        yyv_Pos = yy_0_2_3;
        yy_1_1 = yyv_FHd;
        yy_1_2 = yyv_AHd;
        process_formal_and_actual(yy_1_1, yy_1_2);
        yy_2_1 = yyv_FTl;
        yy_2_2 = yyv_ATl;
        process_formal_and_actual_params(yy_2_1, yy_2_2);
        return;
yyfl_27_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yy_0_2;
        yy yy_0_2_1;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 2) goto yyfl_27_2;
        if (yy_0_2[0] != 2) goto yyfl_27_2;
        yy_0_2_1 = ((yy)yy_0_2[1]);
        return;
yyfl_27_2 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yy_0_2;
        yy yy_0_2_1;
        yy yy_0_2_2;
        yy yyv_Pos;
        yy yy_0_2_3;
        yy yy_1_1;
        yy yy_1_2;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 2) goto yyfl_27_3;
        if (yy_0_2[0] != 1) goto yyfl_27_3;
        yy_0_2_1 = ((yy)yy_0_2[1]);
        yy_0_2_2 = ((yy)yy_0_2[2]);
        yy_0_2_3 = ((yy)yy_0_2[3]);
        yyv_Pos = yy_0_2_3;
        yy_1_1 = ((yy)"too many actual parameters");
        yy_1_2 = yyv_Pos;
        Error(yy_1_1, yy_1_2);
        return;
yyfl_27_3 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yy_0_1_1;
        yy yy_0_1_2;
        yy yy_0_2;
        yy yyv_Pos;
        yy yy_0_2_1;
        yy yy_1_1;
        yy yy_1_2;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 1) goto yyfl_27_4;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        if (yy_0_2[0] != 2) goto yyfl_27_4;
        yy_0_2_1 = ((yy)yy_0_2[1]);
        yyv_Pos = yy_0_2_1;
        yy_1_1 = ((yy)"too few actual parameters");
        yy_1_2 = yyv_Pos;
        Error(yy_1_1, yy_1_2);
        return;
yyfl_27_4 : ;
    }
    yyErr(2,512);
}
process_formal_and_actual(yyin_1, yyin_2)
    yy yyin_1;
yy yyin_2;
{
    {
        yy yyb = NULL;
        yy yy_0_1 = NULL;
        yy yyv_FMode = NULL;
        yy yy_0_1_1 = NULL;
        yy yyv_FType = NULL;
        yy yy_0_1_2 = NULL;
        yy yyv_FName = NULL;
        yy yy_0_1_3 = NULL;
        yy yy_0_1_4 = NULL;
        yy yy_0_2 = NULL;
        yy yyv_AName = NULL;
        yy yy_0_2_1 = NULL;
        yy yyv_Pos = NULL;
        yy yy_0_2_2 = NULL;
        yy yy_1_1_1_1 = NULL;
        yy yyv_AMode = NULL;
        yy yy_1_1_1_2 = NULL;
        yy yyv_AType = NULL;
        yy yy_1_1_1_3 = NULL;
        yy yy_1_1_2_1_1_1 = NULL;
        yy yy_1_1_2_1_1_2 = NULL;
        yy yy_1_1_2_1_2_1 = NULL;
        yy yy_1_1_2_1_2_2 = NULL;
        yy yy_1_1_2_1_2_3 = NULL;
        yy yy_1_1_2_1_2_4 = NULL;
        yy yy_1_2_1_1 = NULL;
        yy yy_1_2_1_2 = NULL;
        yy yy_1_2_1_3 = NULL;
        yy yy_1_2_2_1 = NULL;
        yy yy_1_2_3_1 = NULL;
        yy yyv_FTypeStr = NULL;
        yy yy_1_2_3_2 = NULL;
        yy yy_1_2_4_1 = NULL;
        yy yy_1_2_5_1 = NULL;
        yy yy_1_2_6_1 = NULL;
        yy yyv_ANameStr = NULL;
        yy yy_1_2_6_2 = NULL;
        yy yy_1_2_7_1 = NULL;
        yy yy_1_2_8_1 = NULL;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 1) goto yyfl_28_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yy_0_1_4 = ((yy)yy_0_1[4]);
        yyv_FMode = yy_0_1_1;
        yyv_FType = yy_0_1_2;
        yyv_FName = yy_0_1_3;
        if (yy_0_2[0] != 1) goto yyfl_28_1;
        yy_0_2_1 = ((yy)yy_0_2[1]);
        yy_0_2_2 = ((yy)yy_0_2[2]);
        yyv_AName = yy_0_2_1;
        yyv_Pos = yy_0_2_2;
        {
            yy yysb = yyb;
            yy_1_1_1_1 = yyv_AName;
            if (! defined_locally(yy_1_1_1_1, &yy_1_1_1_2, &yy_1_1_1_3)) goto yyfl_28_1_1_1;
            yyv_AMode = yy_1_1_1_2;
            yyv_AType = yy_1_1_1_3;
            {
                yy yysb = yyb;
                yy_1_1_2_1_1_1 = yyv_FType;
                yy_1_1_2_1_1_2 = yyv_AType;
                if (yyeq_IDENT(yy_1_1_2_1_1_1, yy_1_1_2_1_1_2)) goto yyfl_28_1_1_1_2_1;
                yy_1_1_2_1_2_1 = ((yy)"'");
                yy_1_1_2_1_2_2 = yyv_AName;
                yy_1_1_2_1_2_3 = ((yy)"' has inconsistent type");
                yy_1_1_2_1_2_4 = yyv_Pos;
                ErrorI(yy_1_1_2_1_2_1, yy_1_1_2_1_2_2, yy_1_1_2_1_2_3, yy_1_1_2_1_2_4);
                goto yysl_28_1_1_1_2;
yyfl_28_1_1_1_2_1 : ;
                goto yysl_28_1_1_1_2;
yysl_28_1_1_1_2 : ;
                yyb = yysb;
            }
            goto yysl_28_1_1;
yyfl_28_1_1_1 : ;
            yyb = yyh;
            yyh += 1; if (yyh > yyhx) yyExtend();
            yy_1_2_1_1 = yyb + 0;
            yy_1_2_1_1[0] = 3;
            yy_1_2_1_2 = yyv_FType;
            yy_1_2_1_3 = yyv_AName;
            define_local_name(yy_1_2_1_1, yy_1_2_1_2, yy_1_2_1_3);
            yy_1_2_2_1 = ((yy)"   ");
            Put(yy_1_2_2_1);
            yy_1_2_3_1 = yyv_FType;
            id_to_string(yy_1_2_3_1, &yy_1_2_3_2);
            yyv_FTypeStr = yy_1_2_3_2;
            yy_1_2_4_1 = yyv_FTypeStr;
            Put(yy_1_2_4_1);
            yy_1_2_5_1 = ((yy)" ");
            Put(yy_1_2_5_1);
            yy_1_2_6_1 = yyv_AName;
            id_to_string(yy_1_2_6_1, &yy_1_2_6_2);
            yyv_ANameStr = yy_1_2_6_2;
            yy_1_2_7_1 = yyv_ANameStr;
            Put(yy_1_2_7_1);
            yy_1_2_8_1 = ((yy)";");
            Put(yy_1_2_8_1);
            Nl();
            goto yysl_28_1_1;
yysl_28_1_1 : ;
            yyb = yysb;
        }
        return;
yyfl_28_1 : ;
    }
    yyErr(2,522);
}
process_token_param(yyin_1)
    yy yyin_1;
{
    {
        yy yyb = NULL;
        yy yy_0_1 = NULL;
        yy yy_0_1_1 = NULL;
        yy yyv_AName = NULL;
        yy yy_0_1_1_1 = NULL;
        yy yyv_Pos = NULL;
        yy yy_0_1_1_2 = NULL;
        yy yy_0_1_2 = NULL;
        yy yy_0_1_2_1 = NULL;
        yy yy_0_1_3 = NULL;
        yy yy_1_1 = NULL;
        yy yyv_FType = NULL;
        yy yy_1_2 = NULL;
        yy yy_2_1_1_1 = NULL;
        yy yyv_AMode = NULL;
        yy yy_2_1_1_2 = NULL;
        yy yyv_AType = NULL;
        yy yy_2_1_1_3 = NULL;
        yy yy_2_1_2_1_1_1 = NULL;
        yy yy_2_1_2_1_1_2 = NULL;
        yy yy_2_1_2_1_2_1 = NULL;
        yy yy_2_1_2_1_2_2 = NULL;
        yy yy_2_1_2_1_2_3 = NULL;
        yy yy_2_1_2_1_2_4 = NULL;
        yy yy_2_2_1_1 = NULL;
        yy yy_2_2_1_2 = NULL;
        yy yy_2_2_1_3 = NULL;
        yy yy_2_2_2_1 = NULL;
        yy yy_2_2_3_1 = NULL;
        yy yyv_FTypeStr = NULL;
        yy yy_2_2_3_2 = NULL;
        yy yy_2_2_4_1 = NULL;
        yy yy_2_2_5_1 = NULL;
        yy yy_2_2_6_1 = NULL;
        yy yyv_ANameStr = NULL;
        yy yy_2_2_6_2 = NULL;
        yy yy_2_2_7_1 = NULL;
        yy yy_2_2_8_1 = NULL;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_29_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        if (yy_0_1_1[0] != 1) goto yyfl_29_1;
        yy_0_1_1_1 = ((yy)yy_0_1_1[1]);
        yy_0_1_1_2 = ((yy)yy_0_1_1[2]);
        yyv_AName = yy_0_1_1_1;
        yyv_Pos = yy_0_1_1_2;
        if (yy_0_1_2[0] != 2) goto yyfl_29_1;
        yy_0_1_2_1 = ((yy)yy_0_1_2[1]);
        yy_1_1 = ((yy)"YYSTYPE");
        string_to_id(yy_1_1, &yy_1_2);
        yyv_FType = yy_1_2;
        {
            yy yysb = yyb;
            yy_2_1_1_1 = yyv_AName;
            if (! defined_locally(yy_2_1_1_1, &yy_2_1_1_2, &yy_2_1_1_3)) goto yyfl_29_1_2_1;
            yyv_AMode = yy_2_1_1_2;
            yyv_AType = yy_2_1_1_3;
            {
                yy yysb = yyb;
                yy_2_1_2_1_1_1 = yyv_FType;
                yy_2_1_2_1_1_2 = yyv_AType;
                if (yyeq_IDENT(yy_2_1_2_1_1_1, yy_2_1_2_1_1_2)) goto yyfl_29_1_2_1_2_1;
                yy_2_1_2_1_2_1 = ((yy)"'");
                yy_2_1_2_1_2_2 = yyv_AName;
                yy_2_1_2_1_2_3 = ((yy)"' has inconsistent type");
                yy_2_1_2_1_2_4 = yyv_Pos;
                ErrorI(yy_2_1_2_1_2_1, yy_2_1_2_1_2_2, yy_2_1_2_1_2_3, yy_2_1_2_1_2_4);
                goto yysl_29_1_2_1_2;
yyfl_29_1_2_1_2_1 : ;
                goto yysl_29_1_2_1_2;
yysl_29_1_2_1_2 : ;
                yyb = yysb;
            }
            goto yysl_29_1_2;
yyfl_29_1_2_1 : ;
            yyb = yyh;
            yyh += 1; if (yyh > yyhx) yyExtend();
            yy_2_2_1_1 = yyb + 0;
            yy_2_2_1_1[0] = 3;
            yy_2_2_1_2 = yyv_FType;
            yy_2_2_1_3 = yyv_AName;
            define_local_name(yy_2_2_1_1, yy_2_2_1_2, yy_2_2_1_3);
            yy_2_2_2_1 = ((yy)"   ");
            Put(yy_2_2_2_1);
            yy_2_2_3_1 = yyv_FType;
            id_to_string(yy_2_2_3_1, &yy_2_2_3_2);
            yyv_FTypeStr = yy_2_2_3_2;
            yy_2_2_4_1 = yyv_FTypeStr;
            Put(yy_2_2_4_1);
            yy_2_2_5_1 = ((yy)" ");
            Put(yy_2_2_5_1);
            yy_2_2_6_1 = yyv_AName;
            id_to_string(yy_2_2_6_1, &yy_2_2_6_2);
            yyv_ANameStr = yy_2_2_6_2;
            yy_2_2_7_1 = yyv_ANameStr;
            Put(yy_2_2_7_1);
            yy_2_2_8_1 = ((yy)";");
            Put(yy_2_2_8_1);
            Nl();
            goto yysl_29_1_2;
yysl_29_1_2 : ;
            yyb = yysb;
        }
        return;
yyfl_29_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_29_2;
        return;
yyfl_29_2 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yy_0_1_1;
        yy yyv_Name;
        yy yy_0_1_1_1;
        yy yyv_Pos;
        yy yy_0_1_1_2;
        yy yy_0_1_2;
        yy yy_0_1_2_1;
        yy yyv_Name2;
        yy yy_0_1_2_1_1;
        yy yyv_Pos2;
        yy yy_0_1_2_1_2;
        yy yyv_Tl;
        yy yy_0_1_2_2;
        yy yy_0_1_2_3;
        yy yy_0_1_3;
        yy yy_1_1;
        yy yy_1_2;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_29_3;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        if (yy_0_1_1[0] != 1) goto yyfl_29_3;
        yy_0_1_1_1 = ((yy)yy_0_1_1[1]);
        yy_0_1_1_2 = ((yy)yy_0_1_1[2]);
        yyv_Name = yy_0_1_1_1;
        yyv_Pos = yy_0_1_1_2;
        if (yy_0_1_2[0] != 1) goto yyfl_29_3;
        yy_0_1_2_1 = ((yy)yy_0_1_2[1]);
        yy_0_1_2_2 = ((yy)yy_0_1_2[2]);
        yy_0_1_2_3 = ((yy)yy_0_1_2[3]);
        if (yy_0_1_2_1[0] != 1) goto yyfl_29_3;
        yy_0_1_2_1_1 = ((yy)yy_0_1_2_1[1]);
        yy_0_1_2_1_2 = ((yy)yy_0_1_2_1[2]);
        yyv_Name2 = yy_0_1_2_1_1;
        yyv_Pos2 = yy_0_1_2_1_2;
        yyv_Tl = yy_0_1_2_2;
        yy_1_1 = ((yy)"Too many parameters for token");
        yy_1_2 = yyv_Pos;
        Error(yy_1_1, yy_1_2);
        return;
yyfl_29_3 : ;
    }
    yyErr(2,544);
}

define_local_name(yyin_1, yyin_2, yyin_3)
    yy yyin_1;
    yy yyin_2;
    yy yyin_3;
{
    {
        yy yyb;
        yy yyv_Mode;
        yy yy_0_1;
        yy yyv_Type;
        yy yy_0_2;
        yy yyv_Name;
        yy yy_0_3;
        yy yyv_L;
        yy yy_1;
        yy yy_2;
        yy yy_2_1;
        yy yy_2_1_1;
        yy yy_2_1_2;
        yy yy_2_1_3;
        yy yy_2_2;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yy_0_3 = yyin_3;
        yyv_Mode = yy_0_1;
        yyv_Type = yy_0_2;
        yyv_Name = yy_0_3;
        yyb = yyh;
        yyh += 7; if (yyh > yyhx) yyExtend();
        yy_1 = yyglov_LocalNames;
        if (yy_1 == (yy) yyu) yyErr(1,580);
        yyv_L = yy_1;
        yy_2_1_1 = yyv_Mode;
        yy_2_1_2 = yyv_Type;
        yy_2_1_3 = yyv_Name;
        yy_2_1 = yyb + 3;
        yy_2_1[0] = 1;
        yy_2_1[1] = ((long)yy_2_1_1);
        yy_2_1[2] = ((long)yy_2_1_2);
        yy_2_1[3] = ((long)yy_2_1_3);
        yy_2_2 = yyv_L;
        yy_2 = yyb + 0;
        yy_2[0] = 1;
        yy_2[1] = ((long)yy_2_1);
        yy_2[2] = ((long)yy_2_2);
        yyglov_LocalNames = yy_2;
        return;
    }
}
int defined_locally(yyin_1, yyout_1, yyout_2)
    yy yyin_1;
yy *yyout_1;
yy *yyout_2;
{
    {
        yy yyb;
        yy yyv_Name;
        yy yy_0_1;
        yy yy_0_2;
        yy yy_0_3;
        yy yyv_L;
        yy yy_1;
        yy yy_2_1;
        yy yy_2_2;
        yy yyv_Mode;
        yy yy_2_3;
        yy yyv_Type;
        yy yy_2_4;
        yy_0_1 = yyin_1;
        yyv_Name = yy_0_1;
        yy_1 = yyglov_LocalNames;
        if (yy_1 == (yy) yyu) yyErr(1,585);
        yyv_L = yy_1;
        yy_2_1 = yyv_L;
        yy_2_2 = yyv_Name;
        if (! defined_locally_h(yy_2_1, yy_2_2, &yy_2_3, &yy_2_4)) goto yyfl_34_1;
        yyv_Mode = yy_2_3;
        yyv_Type = yy_2_4;
        yy_0_2 = yyv_Mode;
        yy_0_3 = yyv_Type;
        *yyout_1 = yy_0_2;
        *yyout_2 = yy_0_3;
        return 1;
yyfl_34_1 : ;
    }
    return 0;
}
int defined_locally_h(yyin_1, yyin_2, yyout_1, yyout_2)
    yy yyin_1;
yy yyin_2;
yy *yyout_1;
yy *yyout_2;
{
    {
        yy yyb = NULL;
        yy yy_0_1 = NULL;
        yy yy_0_1_1 = NULL;
        yy yyv_M = NULL;
        yy yy_0_1_1_1 = NULL;
        yy yyv_T = NULL;
        yy yy_0_1_1_2 = NULL;
        yy yyv_N = NULL;
        yy yy_0_1_1_3 = NULL;
        yy yyv_Tail = NULL;
        yy yy_0_1_2 = NULL;
        yy yyv_Name = NULL;
        yy yy_0_2 = NULL;
        yy yy_0_3 = NULL;
        yy yy_0_4 = NULL;
        yy yy_1_1_1_1 = NULL;
        yy yy_1_1_1_2 = NULL;
        yy yy_1_1_2_1 = NULL;
        yy yyv_Mode = NULL;
        yy yy_1_1_2_2 = NULL;
        yy yy_1_1_3_1 = NULL;
        yy yyv_Type = NULL;
        yy yy_1_1_3_2 = NULL;
        yy yy_1_2_1_1 = NULL;
        yy yy_1_2_1_2 = NULL;
        yy yy_1_2_1_3 = NULL;
        yy yy_1_2_1_4 = NULL;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 1) goto yyfl_35_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        if (yy_0_1_1[0] != 1) goto yyfl_35_1;
        yy_0_1_1_1 = ((yy)yy_0_1_1[1]);
        yy_0_1_1_2 = ((yy)yy_0_1_1[2]);
        yy_0_1_1_3 = ((yy)yy_0_1_1[3]);
        yyv_M = yy_0_1_1_1;
        yyv_T = yy_0_1_1_2;
        yyv_N = yy_0_1_1_3;
        yyv_Tail = yy_0_1_2;
        yyv_Name = yy_0_2;
        {
            yy yysb = yyb;
            yy_1_1_1_1 = yyv_N;
            yy_1_1_1_2 = yyv_Name;
            if (! yyeq_IDENT(yy_1_1_1_1, yy_1_1_1_2)) goto yyfl_35_1_1_1;
            yy_1_1_2_1 = yyv_M;
            yy_1_1_2_2 = yy_1_1_2_1;
            yyv_Mode = yy_1_1_2_2;
            yy_1_1_3_1 = yyv_T;
            yy_1_1_3_2 = yy_1_1_3_1;
            yyv_Type = yy_1_1_3_2;
            goto yysl_35_1_1;
yyfl_35_1_1_1 : ;
            yy_1_2_1_1 = yyv_Tail;
            yy_1_2_1_2 = yyv_Name;
            if (! defined_locally_h(yy_1_2_1_1, yy_1_2_1_2, &yy_1_2_1_3, &yy_1_2_1_4)) goto yyfl_35_1_1_2;
            yyv_Mode = yy_1_2_1_3;
            yyv_Type = yy_1_2_1_4;
            goto yysl_35_1_1;
yyfl_35_1_1_2 : ;
            goto yyfl_35_1;
yysl_35_1_1 : ;
            yyb = yysb;
        }
        yy_0_3 = yyv_Mode;
        yy_0_4 = yyv_Type;
        *yyout_1 = yy_0_3;
        *yyout_2 = yy_0_4;
        return 1;
yyfl_35_1 : ;
    }
    return 0;
}
put_CodeBlock(yyin_1, yyin_2, yyin_3)
    yy yyin_1;
yy yyin_2;
yy yyin_3;
{
    {
        yy yyb;
        yy yyv_Str1;
        yy yy_0_1;
        yy yy_0_2;
        yy yyv_BlkTxt;
        yy yy_0_2_1;
        yy yyv_BlkPos;
        yy yy_0_2_2;
        yy yyv_Str2;
        yy yy_0_3;
        yy yy_1_1;
        yy yy_2_1;
        yy yyv_N1;
        yy yy_2_2;
        yy yy_3_1;
        yy yyv_Name;
        yy yy_4_1;
        yy yy_5_1;
        yy yy_6_1;
        yy yy_7_1;
        yy yy_9_1;
        yy yy_10_1;
        yy yy_11_1;
        yy yyv_N2;
        yy yy_13_1;
        yy yy_14_1;
        yy yy_15_1;
        yy yy_15_1_1;
        yy yy_15_1_2;
        yy yy_16_1;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yy_0_3 = yyin_3;
        yyv_Str1 = yy_0_1;
        if (yy_0_2[0] != 1) goto yyfl_36_1;
        yy_0_2_1 = ((yy)yy_0_2[1]);
        yy_0_2_2 = ((yy)yy_0_2[2]);
        yyv_BlkTxt = yy_0_2_1;
        yyv_BlkPos = yy_0_2_2;
        yyv_Str2 = yy_0_3;
        yy_1_1 = ((yy)"#line ");
        Put(yy_1_1);
        yy_2_1 = yyv_BlkPos;
        POS_to_LineNumber(yy_2_1, &yy_2_2);
        yyv_N1 = yy_2_2;
        yy_3_1 = yyv_N1;
        PutI(yy_3_1);
        GetSourceFileName(&yy_4_1);
        yyv_Name = yy_4_1;
        yy_5_1 = ((yy)" \"");
        Put(yy_5_1);
        yy_6_1 = yyv_Name;
        Put(yy_6_1);
        yy_7_1 = ((yy)"\"");
        Put(yy_7_1);
        Nl();
        yy_9_1 = yyv_Str1;
        Put(yy_9_1);
        yy_10_1 = yyv_BlkTxt;
        PutBlockText(yy_10_1);
        yy_11_1 = yyv_Str2;
        Put(yy_11_1);
        Nl();
        GetOutputLineCount(&yy_13_1);
        yyv_N2 = yy_13_1;
        yy_14_1 = ((yy)"# line ");
        Put(yy_14_1);
        yy_15_1_1 = yyv_N2;
        yy_15_1_2 = ((yy)2);
        yy_15_1 = (yy)(((long)yy_15_1_1)+((long)yy_15_1_2));
        PutI(yy_15_1);
        yy_16_1 = ((yy)" \"yygram.cpp\"");
        Put(yy_16_1);
        Nl();
        return;
yyfl_36_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yy_0_2;
        yy yy_0_3;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yy_0_3 = yyin_3;
        if (yy_0_2[0] != 2) goto yyfl_36_2;
        return;
yyfl_36_2 : ;
    }
    yyErr(2,600);
}
