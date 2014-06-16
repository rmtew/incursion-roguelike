#define NULL 0
typedef long * yy;
#define yyu (-2147483647L)
static yy yynull;
extern yy yyh;
extern yy yyhx;
static void yyErr(n,l)
{
    yyAbort(n,"encode", l);
}
extern yy yyglov_GentleFlag;
extern yy yyglov_NontermList;
extern yy yyglov_PreludeBlock;
yy yyglov_LHS_List = (yy) yyu;
int yyeq_LHSKEYS(t1, t2)
    yy t1, t2;
{
    switch(t1[0]) {
    case 1: return (t2[0] == 1)
                && yyeq_LHSKEYS((yy)t1[1], (yy)t2[1])
                && yyeq_LHSKEY((yy)t1[2], (yy)t2[2])
                ;
    case 2: return (t2[0] == 2)
                ;
    }
}
void write_nonterms_LHSKEY(yy);
void write_nonterms_LHSKEYS(yy);

yyPrint_LHSKEYS(t) yy t;
{
    switch(t[0]) {
    case 1: 
        yyTerm("keys");
        yyFirstArg();
        yyPrint_LHSKEYS((yy)t[1]);
        yyNextArg();
        yyPrint_LHSKEY((yy)t[2]);
        yyEndArgs();
        break;
    case 2: 
        yyTerm("nil");
        yyNoArgs();
        break;
    }
}
yybroadcast_LHSKEYS(t,In,Out,Handler)
    yy t, In, *Out; int (*Handler) ();
{
    yy A, B;
    A = In;
    if (! Handler(yybroadcast_LHSKEYS, t, In, Out)) {
        switch(t[0]) {
        case 1: 
            yybroadcast_LHSKEYS((yy)t[1], A, &B, Handler);
            *Out = B;
            break;
        case 2: 
            *Out = A;
            break;
        }
    }
}
yyeq_LHSKEY(t1, t2) yy t1, t2;
{
    return t1 == t2;
}
yyPrint_LHSKEY(t) yy t;
{
    yyPrintIndex(t);
}
yyeq_CLAUSEKEYS(t1, t2) yy t1, t2;
{
    switch(t1[0]) {
    case 1: return (t2[0] == 1)
                && yyeq_CLAUSEKEYS((yy)t1[1], (yy)t2[1])
                && yyeq_CLAUSEKEY((yy)t1[2], (yy)t2[2])
                ;
    case 2: return (t2[0] == 2)
                ;
    }
}
yyPrint_CLAUSEKEYS(t) yy t;
{
    switch(t[0]) {
    case 1: 
        yyTerm("clausekeys");
        yyFirstArg();
        yyPrint_CLAUSEKEYS((yy)t[1]);
        yyNextArg();
        yyPrint_CLAUSEKEY((yy)t[2]);
        yyEndArgs();
        break;
    case 2: 
        yyTerm("nil");
        yyNoArgs();
        break;
    }
}
yybroadcast_CLAUSEKEYS(t,In,Out,Handler)
    yy t, In, *Out; int (*Handler) ();
{
    yy A, B;
    A = In;
    if (! Handler(yybroadcast_CLAUSEKEYS, t, In, Out)) {
        switch(t[0]) {
        case 1: 
            yybroadcast_CLAUSEKEYS((yy)t[1], A, &B, Handler);
            *Out = B;
            break;
        case 2: 
            *Out = A;
            break;
        }
    }
}
yyeq_CLAUSEKEY(t1, t2) yy t1, t2;
{
    return t1 == t2;
}
yyPrint_CLAUSEKEY(t) yy t;
{
    yyPrintIndex(t);
}
yyeq_GRAMMARREF(t1, t2) yy t1, t2;
{
    switch(t1[0]) {
    case 1: return (t2[0] == 1)
                && yyeq_MEMBERLIST((yy)t1[1], (yy)t2[1])
                ;
    case 2: return (t2[0] == 2)
                ;
    }
}
yyPrint_GRAMMARREF(t) yy t;
{
    switch(t[0]) {
    case 1: 
        yyTerm("members");
        yyFirstArg();
        yyPrint_MEMBERLIST((yy)t[1]);
        yyEndArgs();
        break;
    case 2: 
        yyTerm("none");
        yyNoArgs();
        break;
    }
}
yybroadcast_GRAMMARREF(t,In,Out,Handler)
    yy t, In, *Out; int (*Handler) ();
{
    yy A, B;
    A = In;
    if (! Handler(yybroadcast_GRAMMARREF, t, In, Out)) {
        switch(t[0]) {
        case 1: 
            yybroadcast_MEMBERLIST((yy)t[1], A, &B, Handler);
            *Out = B;
            break;
        case 2: 
            *Out = A;
            break;
        }
    }
}
yyeq_ELEMS(t1, t2) yy t1, t2;
{
    switch(t1[0]) {
    case 1: return (t2[0] == 1)
                && yyeq_ELEMS((yy)t1[1], (yy)t2[1])
                && yyeq_ELEM((yy)t1[2], (yy)t2[2])
                ;
    case 2: return (t2[0] == 2)
                ;
    }
}
yyPrint_ELEMS(t) yy t;
{
    switch(t[0]) {
    case 1: 
        yyTerm("elems");
        yyFirstArg();
        yyPrint_ELEMS((yy)t[1]);
        yyNextArg();
        yyPrint_ELEM((yy)t[2]);
        yyEndArgs();
        break;
    case 2: 
        yyTerm("nil");
        yyNoArgs();
        break;
    }
}
yybroadcast_ELEMS(t,In,Out,Handler)
    yy t, In, *Out; int (*Handler) ();
{
    yy A, B;
    A = In;
    if (! Handler(yybroadcast_ELEMS, t, In, Out)) {
        switch(t[0]) {
        case 1: 
            yybroadcast_ELEMS((yy)t[1], A, &B, Handler);
            yybroadcast_ELEM((yy)t[2], B, &A, Handler);
            *Out = A;
            break;
        case 2: 
            *Out = A;
            break;
        }
    }
}
yyeq_ELEM(t1, t2) yy t1, t2;
{
    switch(t1[0]) {
    case 1: return (t2[0] == 1)
                && yyeq_IDENT((yy)t1[1], (yy)t2[1])
                && yyeq_MEMBERANNOTATION((yy)t1[2], (yy)t2[2])
                && (t1[3] == t2[3])
                ;
    case 2: return (t2[0] == 2)
                && (t1[1] == t2[1])
                ;
    case 3: return (t2[0] == 3)
                && yyeq_SUBPHRASEKEY((yy)t1[1], (yy)t2[1])
                && yyeq_MEMBERANNOTATION((yy)t1[2], (yy)t2[2])
                && (t1[3] == t2[3])
                ;
    }
}
yyPrint_ELEM(t) yy t;
{
    switch(t[0]) {
    case 1: 
        yyTerm("nonterm");
        yyFirstArg();
        yyPrint_IDENT((yy)t[1]);
        yyNextArg();
        yyPrint_MEMBERANNOTATION((yy)t[2]);
        yyNextArg();
        yyPrint_POS((yy)t[3]);
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
        yyPrint_SUBPHRASEKEY((yy)t[1]);
        yyNextArg();
        yyPrint_MEMBERANNOTATION((yy)t[2]);
        yyNextArg();
        yyPrint_POS((yy)t[3]);
        yyEndArgs();
        break;
    }
}
yybroadcast_ELEM(t,In,Out,Handler)
    yy t, In, *Out; int (*Handler) ();
{
    yy A, B;
    A = In;
    if (! Handler(yybroadcast_ELEM, t, In, Out)) {
        switch(t[0]) {
        case 1: 
            yybroadcast_IDENT((yy)t[1], A, &B, Handler);
            yybroadcast_MEMBERANNOTATION((yy)t[2], B, &A, Handler);
            *Out = A;
            break;
        case 2: 
            *Out = A;
            break;
        case 3: 
            yybroadcast_MEMBERANNOTATION((yy)t[2], A, &B, Handler);
            *Out = B;
            break;
        }
    }
}
yyeq_LHS(t1, t2) yy t1, t2;
{
    switch(t1[0]) {
    case 1: return (t2[0] == 1)
                && yyeq_IDENT((yy)t1[1], (yy)t2[1])
                ;
    case 2: return (t2[0] == 2)
                && yyeq_SUBPHRASEKEY((yy)t1[1], (yy)t2[1])
                ;
    }
}
yyPrint_LHS(t) yy t;
{
    switch(t[0]) {
    case 1: 
        yyTerm("nonterm");
        yyFirstArg();
        yyPrint_IDENT((yy)t[1]);
        yyEndArgs();
        break;
    case 2: 
        yyTerm("subphrase");
        yyFirstArg();
        yyPrint_SUBPHRASEKEY((yy)t[1]);
        yyEndArgs();
        break;
    }
}
yybroadcast_LHS(t,In,Out,Handler)
    yy t, In, *Out; int (*Handler) ();
{
    yy A, B;
    A = In;
    if (! Handler(yybroadcast_LHS, t, In, Out)) {
        switch(t[0]) {
        case 1: 
            yybroadcast_IDENT((yy)t[1], A, &B, Handler);
            *Out = B;
            break;
        case 2: 
            *Out = A;
            break;
        }
    }
}
yy yyglov_term_base = (yy) yyu;
yy yyglov_start_user_grammar = (yy) yyu;
yy yyglov_CurLhs = (yy) yyu;
yy yyglov_CurClause = (yy) yyu;
yyeq_SET(t1, t2) yy t1, t2;
{
    return t1 == t2;
}
yyPrint_SET(t) yy t;
{
    yyPrintOpaque(t);
}
yybroadcast_SET(t,In,Out,Handler)
    yy t, In, *Out; int (*Handler) ();
{
    yy A, B;
    A = In;
    if (! Handler(yybroadcast_SET, t, In, Out)) {
        *Out = In;}
}
yy yyglov_PC = (yy) yyu;
yy yyglov_LastClauseKey = (yy) yyu;
yy yyglov_CurLhsPos = (yy) yyu;
yy yyglov_FieldIndex = (yy) yyu;
yy yyglov_TABLEUPB2 = (yy) yyu;
INIT_ENCODE()
{
    {
        yy yyb = NULL;
        yy yy_1 = NULL;
        yy yy_2 = NULL;
        yy yy_3 = NULL;
        yy yyv_MAXCHAR = NULL;
        yy yy_4_1 = NULL;
        yy yy_5 = NULL;
        yyb = yyh;
        yyh += 1; if (yyh > yyhx) yyExtend();
        yy_1 = ((yy)50000);
        yyglov_term_base = yy_1;
        yy_2 = ((yy)8);
        yyglov_start_user_grammar = yy_2;
        yy_3 = yyb + 0;
        yy_3[0] = 2;
        yyglov_LHS_List = yy_3;
        get_max_char(&yy_4_1);
        yyv_MAXCHAR = yy_4_1;
        yy_5 = yyv_MAXCHAR;
        yyglov_TABLEUPB2 = yy_5;
        init_ana();
        return;
    }
}
ENCODE()
{
    {
        yy yyb = NULL;
        yy yyv_L = NULL;
        yy yy_1 = NULL;
        yy yy_2 = NULL;
        yy yyv_LhsKeys = NULL;
        yy yy_3 = NULL;
        yy yy_4_1 = NULL;
        yy yyv_N = NULL;
        yy yy_5 = NULL;
        yy yy_6_1 = NULL;
        yy yy_8_1 = NULL;
        yy yy_9_1 = NULL;
        yy yy_9_1_1 = NULL;
        yy yy_9_1_2 = NULL;
        yy yy_10_1 = NULL;
        yy yy_12_1 = NULL;
        yy yy_14_1 = NULL;
        yy yy_16_1 = NULL;
        yy yy_18 = NULL;
        yy yy_19_1 = NULL;
        yy yy_20_1 = NULL;
        yy yy_22_1 = NULL;
        yy yy_24_1 = NULL;
        yy yy_26_1 = NULL;
        yy yy_28_1 = NULL;
        yy yy_30 = NULL;
        yy yy_31_1 = NULL;
        yy yy_32_1 = NULL;
        yy yy_34_1 = NULL;
        yy yy_36_1 = NULL;
        yy yy_38_1 = NULL;
        yy yy_40_1 = NULL;
        yy yy_42 = NULL;
        yy yy_43_1 = NULL;
        yy yy_44_1 = NULL;
        yy yy_46_1 = NULL;
        yy yy_49_1 = NULL;
        yy yy_51_1 = NULL;
        yy yy_53_1 = NULL;
        yy yy_54_1 = NULL;
        yy yy_54_1_1 = NULL;
        yy yy_54_1_2 = NULL;
        yy yy_55_1 = NULL;
        yy yy_57_1 = NULL;
        yy yyv_Upb1 = NULL;
        yy yy_59_1 = NULL;
        yy yyv_Upb2 = NULL;
        yy yy_60 = NULL;
        yy yy_61_1 = NULL;
        yy yy_62_1 = NULL;
        yy yy_62_1_1 = NULL;
        yy yy_62_1_2 = NULL;
        yy yy_63_1 = NULL;
        yy yy_64_1 = NULL;
        yy yy_64_1_1 = NULL;
        yy yy_64_1_2 = NULL;
        yy yy_65_1 = NULL;
        yy yy_67_1 = NULL;
        yy yy_69_1 = NULL;
        yy yy_70_1 = NULL;
        yy yy_73_1 = NULL;
        yy yy_75_1 = NULL;
        yy yy_77_1 = NULL;
        yy yy_79_1 = NULL;
        yy yy_81_1 = NULL;
        yy yy_83_1 = NULL;
        yy_1 = yyglov_NontermList;
        if (yy_1 == (yy) yyu) yyErr(1,117);
        yyv_L = yy_1;
        yy_2 = ((yy)1);
        yyglov_PC = yy_2;
        yy_3 = yyglov_LHS_List;
        if (yy_3 == (yy) yyu) yyErr(1,120);
        yyv_LhsKeys = yy_3;
        yy_4_1 = yyv_LhsKeys;
        calc_positions_LHSKEYS(yy_4_1);
        yy_5 = yyglov_PC;
        if (yy_5 == (yy) yyu) yyErr(1,124);
        yyv_N = yy_5;
        yy_6_1 = ((yy)"extern int c_length;");
        Put(yy_6_1);
        Nl();
        yy_8_1 = ((yy)"int c_length = ");
        Put(yy_8_1);
        yy_9_1_1 = yyv_N;
        yy_9_1_2 = ((yy)1);
        yy_9_1 = (yy)(((long)yy_9_1_1)-((long)yy_9_1_2));
        PutI(yy_9_1);
        yy_10_1 = ((yy)";");
        Put(yy_10_1);
        Nl();
        yy_12_1 = ((yy)"extern int yygrammar[];");
        Put(yy_12_1);
        Nl();
        yy_14_1 = ((yy)"int yygrammar[] = {");
        Put(yy_14_1);
        Nl();
        yy_16_1 = ((yy)"0,");
        Put(yy_16_1);
        Nl();
        yy_18 = ((yy)1);
        yyglov_FieldIndex = yy_18;
        yy_19_1 = yyv_LhsKeys;
        write_code_LHSKEYS(yy_19_1);
        yy_20_1 = ((yy)"0");
        Put(yy_20_1);
        Nl();
        yy_22_1 = ((yy)"};");
        Put(yy_22_1);
        Nl();
        yy_24_1 = ((yy)"extern int yyannotation[];");
        Put(yy_24_1);
        Nl();
        yy_26_1 = ((yy)"int yyannotation[] = {");
        Put(yy_26_1);
        Nl();
        yy_28_1 = ((yy)"0,");
        Put(yy_28_1);
        Nl();
        yy_30 = ((yy)1);
        yyglov_FieldIndex = yy_30;
        yy_31_1 = yyv_LhsKeys;
        write_annotation_LHSKEYS(yy_31_1);
        yy_32_1 = ((yy)"0");
        Put(yy_32_1);
        Nl();
        yy_34_1 = ((yy)"};");
        Put(yy_34_1);
        Nl();
        yy_36_1 = ((yy)"extern int yycoordinate[];");
        Put(yy_36_1);
        Nl();
        yy_38_1 = ((yy)"int yycoordinate[] = {");
        Put(yy_38_1);
        Nl();
        yy_40_1 = ((yy)"0,");
        Put(yy_40_1);
        Nl();
        yy_42 = ((yy)1);
        yyglov_FieldIndex = yy_42;
        yy_43_1 = yyv_LhsKeys;
        write_coordinates_LHSKEYS(yy_43_1);
        yy_44_1 = ((yy)"0");
        Put(yy_44_1);
        Nl();
        yy_46_1 = ((yy)"};");
        Put(yy_46_1);
        Nl();
        run_ana();
        yy_49_1 = ((yy)"/* only for BIGHASH (see art.c)");
        Put(yy_49_1);
        Nl();
        yy_51_1 = ((yy)"extern int DHITS[];");
        Put(yy_51_1);
        Nl();
        yy_53_1 = ((yy)"int DHITS[");
        Put(yy_53_1);
        yy_54_1_1 = yyv_N;
        yy_54_1_2 = ((yy)1);
        yy_54_1 = (yy)(((long)yy_54_1_1)+((long)yy_54_1_2));
        PutI(yy_54_1);
        yy_55_1 = ((yy)"];");
        Put(yy_55_1);
        Nl();
        yy_57_1 = ((yy)"*/");
        Put(yy_57_1);
        Nl();
        get_rulecount(&yy_59_1);
        yyv_Upb1 = yy_59_1;
        yy_60 = yyglov_TABLEUPB2;
        if (yy_60 == (yy) yyu) yyErr(1,176);
        yyv_Upb2 = yy_60;
        yy_61_1 = ((yy)"int TABLE[");
        Put(yy_61_1);
        yy_62_1_1 = yyv_Upb1;
        yy_62_1_2 = ((yy)1);
        yy_62_1 = (yy)(((long)yy_62_1_1)+((long)yy_62_1_2));
        PutI(yy_62_1);
        yy_63_1 = ((yy)"][");
        Put(yy_63_1);
        yy_64_1_1 = yyv_Upb2;
        yy_64_1_2 = ((yy)1);
        yy_64_1 = (yy)(((long)yy_64_1_1)+((long)yy_64_1_2));
        PutI(yy_64_1);
        yy_65_1 = ((yy)"];");
        Put(yy_65_1);
        Nl();
        yy_67_1 = ((yy)"init_dirsets() {");
        Put(yy_67_1);
        Nl();
        yy_69_1 = yyv_LhsKeys;
        write_dir_sets_LHSKEYS(yy_69_1);
        yy_70_1 = ((yy)"}");
        Put(yy_70_1);
        Nl();
        Nl();
        yy_73_1 = ((yy)"extern int yydirset();");
        Put(yy_73_1);
        Nl();
        yy_75_1 = ((yy)"int yydirset(i,j)");
        Put(yy_75_1);
        Nl();
        yy_77_1 = ((yy)"   int i,j;");
        Put(yy_77_1);
        Nl();
        yy_79_1 = ((yy)"{");
        Put(yy_79_1);
        Nl();
        yy_81_1 = ((yy)"   return TABLE[i][j];");
        Put(yy_81_1);
        Nl();
        yy_83_1 = ((yy)"}");
        Put(yy_83_1);
        Nl();
        write_transparent();
        write_printnames();
        return;
    }
}
START_RULE(yyin_1, yyin_2, yyin_3, yyin_4, yyin_5, yyin_6, yyin_7)
    yy yyin_1;
yy yyin_2;
yy yyin_3;
yy yyin_4;
yy yyin_5;
yy yyin_6;
yy yyin_7;
{
    {
        yy yyb = NULL;
        yy yyv_Id = NULL;
        yy yy_0_1 = NULL;
        yy yyv_NKey = NULL;
        yy yy_0_2 = NULL;
        yy yyv_N = NULL;
        yy yy_0_3 = NULL;
        yy yyv_RKey = NULL;
        yy yy_0_4 = NULL;
        yy yyv_Prelude = NULL;
        yy yy_0_5 = NULL;
        yy yyv_Annotation = NULL;
        yy yy_0_6 = NULL;
        yy yyv_Coordinate = NULL;
        yy yy_0_7 = NULL;
        yy yyv_R = NULL;
        yy yy_1 = NULL;
        yy yy_2_1 = NULL;
        yy yyv_Str = NULL;
        yy yy_2_2 = NULL;
        yy yy_3_1 = NULL;
        yy yyv_RuleHandle = NULL;
        yy yy_3_2 = NULL;
        yy yy_4 = NULL;
        yy yy_5_1 = NULL;
        yy yyv_LhsKey = NULL;
        yy yy_5_2 = NULL;
        yy yy_6 = NULL;
        yy yy_7 = NULL;
        yy yyv_ClauseKey = NULL;
        yy yy_9 = NULL;
        yy yy_10 = NULL;
        yy yy_11 = NULL;
        yy yy_12 = NULL;
        yy yyv_Members = NULL;
        yy yy_13 = NULL;
        yy yy_14 = NULL;
        yy yy_14_1 = NULL;
        yy yy_15 = NULL;
        yy yy_16 = NULL;
        yy yy_17 = NULL;
        yy yyv_Clauses = NULL;
        yy yy_18 = NULL;
        yy yy_19 = NULL;
        yy yy_19_1 = NULL;
        yy yy_19_2 = NULL;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yy_0_3 = yyin_3;
        yy_0_4 = yyin_4;
        yy_0_5 = yyin_5;
        yy_0_6 = yyin_6;
        yy_0_7 = yyin_7;
        yyv_Id = yy_0_1;
        yyv_NKey = yy_0_2;
        yyv_N = yy_0_3;
        yyv_RKey = yy_0_4;
        yyv_Prelude = yy_0_5;
        yyv_Annotation = yy_0_6;
        yyv_Coordinate = yy_0_7;
        yyb = yyh;
        yyh += 16; if (yyh > yyhx) yyExtend();
        yy_1 = (yy) yyv_RKey[2];
        if (yy_1 == (yy) yyu) yyErr(1,203);
        yyv_R = yy_1;
        yy_2_1 = yyv_Id;
        id_to_string(yy_2_1, &yy_2_2);
        yyv_Str = yy_2_2;
        yy_3_1 = yyv_N;
        start_rule(yy_3_1, &yy_3_2);
        yyv_RuleHandle = yy_3_2;
        yy_4 = yyv_RuleHandle;
        yyv_RKey[3] = (long) yy_4;
        yy_5_1 = yyv_Id;
        LhsKeyForNonterm(yy_5_1, &yy_5_2);
        yyv_LhsKey = yy_5_2;
        yy_6 = yyv_N;
        yyv_LhsKey[5] = (long) yy_6;
        yy_7 = yyv_LhsKey;
        yyglov_CurLhs = yy_7;
        yyv_ClauseKey = yyb + 0;
        yyb[9] = yyu;
        yyb[8] = yyu;
        yyb[7] = yyu;
        yyb[6] = yyu;
        yyb[5] = yyu;
        yyb[4] = yyu;
        yyb[3] = yyu;
        yyb[2] = yyu;
        yyb[1] = yyu;
        yy_9 = yyb + 10;
        yy_9[0] = 2;
        yyv_ClauseKey[1] = (long) yy_9;
        yy_10 = yyv_ClauseKey;
        yyglov_CurClause = yy_10;
        yy_11 = yyv_RuleHandle;
        yyv_ClauseKey[6] = (long) yy_11;
        yy_12 = yyv_R;
        yyv_ClauseKey[7] = (long) yy_12;
        yy_13 = (yy) yyv_RKey[1];
        if (yy_13 == (yy) yyu) yyErr(1,218);
        yyv_Members = yy_13;
        yy_14_1 = yyv_Members;
        yy_14 = yyb + 11;
        yy_14[0] = 1;
        yy_14[1] = ((long)yy_14_1);
        yyv_ClauseKey[2] = (long) yy_14;
        yy_15 = yyv_Annotation;
        yyv_ClauseKey[4] = (long) yy_15;
        yy_16 = yyv_Coordinate;
        yyv_ClauseKey[5] = (long) yy_16;
        yy_17 = yyv_Prelude;
        yyv_LhsKey[2] = (long) yy_17;
        yy_18 = (yy) yyv_LhsKey[3];
        if (yy_18 == (yy) yyu) yyErr(1,224);
        yyv_Clauses = yy_18;
        yy_19_1 = yyv_Clauses;
        yy_19_2 = yyv_ClauseKey;
        yy_19 = yyb + 13;
        yy_19[0] = 1;
        yy_19[1] = ((long)yy_19_1);
        yy_19[2] = ((long)yy_19_2);
        yyv_LhsKey[3] = (long) yy_19;
        return;
    }
}
END_RULE()
{
    {
        yy yyb;
        return;
    }
}
ADD_NONTERM_MEMBER(yyin_1, yyin_2, yyin_3, yyin_4)
    yy yyin_1;
yy yyin_2;
yy yyin_3;
yy yyin_4;
{
    {
        yy yyb;
        yy yyv_Id;
        yy yy_0_1;
        yy yyv_N;
        yy yy_0_2;
        yy yyv_Annotation;
        yy yy_0_3;
        yy yyv_Coordinate;
        yy yy_0_4;
        yy yy_1_1;
        yy yyv_ClauseKey;
        yy yy_2;
        yy yyv_Elems;
        yy yy_3;
        yy yy_4;
        yy yy_4_1;
        yy yy_4_2;
        yy yy_4_2_1;
        yy yy_4_2_2;
        yy yy_4_2_3;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yy_0_3 = yyin_3;
        yy_0_4 = yyin_4;
        yyv_Id = yy_0_1;
        yyv_N = yy_0_2;
        yyv_Annotation = yy_0_3;
        yyv_Coordinate = yy_0_4;
        yyb = yyh;
        yyh += 7; if (yyh > yyhx) yyExtend();
        yy_1_1 = yyv_N;
        append_nonterm_member(yy_1_1);
        yy_2 = yyglov_CurClause;
        if (yy_2 == (yy) yyu) yyErr(1,234);
        yyv_ClauseKey = yy_2;
        yy_3 = (yy) yyv_ClauseKey[1];
        if (yy_3 == (yy) yyu) yyErr(1,235);
        yyv_Elems = yy_3;
        yy_4_1 = yyv_Elems;
        yy_4_2_1 = yyv_Id;
        yy_4_2_2 = yyv_Annotation;
        yy_4_2_3 = yyv_Coordinate;
        yy_4_2 = yyb + 3;
        yy_4_2[0] = 1;
        yy_4_2[1] = ((long)yy_4_2_1);
        yy_4_2[2] = ((long)yy_4_2_2);
        yy_4_2[3] = ((long)yy_4_2_3);
        yy_4 = yyb + 0;
        yy_4[0] = 1;
        yy_4[1] = ((long)yy_4_1);
        yy_4[2] = ((long)yy_4_2);
        yyv_ClauseKey[1] = (long) yy_4;
        return;
    }
}
ADD_TOKEN_MEMBER(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_N;
        yy yy_0_1;
        yy yy_1_1;
        yy yyv_ClauseKey;
        yy yy_2;
        yy yyv_Elems;
        yy yy_3;
        yy yy_4;
        yy yy_4_1;
        yy yy_4_2;
        yy yy_4_2_1;
        yy_0_1 = yyin_1;
        yyv_N = yy_0_1;
        yyb = yyh;
        yyh += 5; if (yyh > yyhx) yyExtend();
        yy_1_1 = yyv_N;
        append_token_member(yy_1_1);
        yy_2 = yyglov_CurClause;
        if (yy_2 == (yy) yyu) yyErr(1,242);
        yyv_ClauseKey = yy_2;
        yy_3 = (yy) yyv_ClauseKey[1];
        if (yy_3 == (yy) yyu) yyErr(1,243);
        yyv_Elems = yy_3;
        yy_4_1 = yyv_Elems;
        yy_4_2_1 = yyv_N;
        yy_4_2 = yyb + 3;
        yy_4_2[0] = 2;
        yy_4_2[1] = ((long)yy_4_2_1);
        yy_4 = yyb + 0;
        yy_4[0] = 1;
        yy_4[1] = ((long)yy_4_1);
        yy_4[2] = ((long)yy_4_2);
        yyv_ClauseKey[1] = (long) yy_4;
        return;
    }
}
ADD_SUBPHRASE_REFERENCE(yyin_1, yyin_2, yyin_3)
    yy yyin_1;
yy yyin_2;
yy yyin_3;
{
    {
        yy yyb;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_Annotation;
        yy yy_0_2;
        yy yyv_Pos;
        yy yy_0_3;
        yy yyv_N;
        yy yy_1;
        yy yyv_NNo;
        yy yy_2_1;
        yy yy_3_1;
        yy yy_3_1_1;
        yy yy_3_1_2;
        yy yyv_SNo;
        yy yy_3_2;
        yy yy_4_1;
        yy yyv_ClauseKey;
        yy yy_5;
        yy yyv_Elems;
        yy yy_6;
        yy yy_7;
        yy yy_7_1;
        yy yy_7_2;
        yy yy_7_2_1;
        yy yy_7_2_2;
        yy yy_7_2_3;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yy_0_3 = yyin_3;
        yyv_Key = yy_0_1;
        yyv_Annotation = yy_0_2;
        yyv_Pos = yy_0_3;
        yyb = yyh;
        yyh += 7; if (yyh > yyhx) yyExtend();
        yy_1 = (yy) yyv_Key[1];
        if (yy_1 == (yy) yyu) yyErr(1,248);
        yyv_N = yy_1;
        NumberOfNonterms(&yy_2_1);
        yyv_NNo = yy_2_1;
        yy_3_1_1 = yyv_N;
        yy_3_1_2 = yyv_NNo;
        yy_3_1 = (yy)(((long)yy_3_1_1)+((long)yy_3_1_2));
        yy_3_2 = yy_3_1;
        yyv_SNo = yy_3_2;
        yy_4_1 = yyv_SNo;
        append_nonterm_member(yy_4_1);
        yy_5 = yyglov_CurClause;
        if (yy_5 == (yy) yyu) yyErr(1,253);
        yyv_ClauseKey = yy_5;
        yy_6 = (yy) yyv_ClauseKey[1];
        if (yy_6 == (yy) yyu) yyErr(1,254);
        yyv_Elems = yy_6;
        yy_7_1 = yyv_Elems;
        yy_7_2_1 = yyv_Key;
        yy_7_2_2 = yyv_Annotation;
        yy_7_2_3 = yyv_Pos;
        yy_7_2 = yyb + 3;
        yy_7_2[0] = 3;
        yy_7_2[1] = ((long)yy_7_2_1);
        yy_7_2[2] = ((long)yy_7_2_2);
        yy_7_2[3] = ((long)yy_7_2_3);
        yy_7 = yyb + 0;
        yy_7[0] = 1;
        yy_7[1] = ((long)yy_7_1);
        yy_7[2] = ((long)yy_7_2);
        yyv_ClauseKey[1] = (long) yy_7;
        return;
    }
}
START_ALTERNATIVE(yyin_1, yyin_2, yyin_3, yyin_4, yyin_5)
    yy yyin_1;
yy yyin_2;
yy yyin_3;
yy yyin_4;
yy yyin_5;
{
    {
        yy yyb;
        yy yyv_SKey;
        yy yy_0_1;
        yy yyv_AKey;
        yy yy_0_2;
        yy yyv_GrammarRef;
        yy yy_0_3;
        yy yyv_Annotation;
        yy yy_0_4;
        yy yyv_Coordinate;
        yy yy_0_5;
        yy yyv_SubphraseNumber;
        yy yy_1;
        yy yyv_NNont;
        yy yy_2_1;
        yy yy_3_1;
        yy yy_3_1_1;
        yy yy_3_1_2;
        yy yyv_SNo;
        yy yy_3_2;
        yy yyv_N;
        yy yy_4;
        yy yyv_NRules;
        yy yy_5_1;
        yy yy_6_1;
        yy yy_6_1_1;
        yy yy_6_1_2;
        yy yyv_ANo;
        yy yy_6_2;
        yy yy_7_1;
        yy yyv_RuleHandle;
        yy yy_7_2;
        yy yy_8;
        yy yy_9_1;
        yy yyv_LhsKey;
        yy yy_9_2;
        yy yy_10;
        yy yy_11;
        yy yyv_ClauseKey;
        yy yy_13;
        yy yy_14;
        yy yy_15;
        yy yy_16;
        yy yy_17;
        yy yy_18;
        yy yy_19;
        yy yy_20;
        yy yyv_Clauses;
        yy yy_21;
        yy yy_22;
        yy yy_22_1;
        yy yy_22_2;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yy_0_3 = yyin_3;
        yy_0_4 = yyin_4;
        yy_0_5 = yyin_5;
        yyv_SKey = yy_0_1;
        yyv_AKey = yy_0_2;
        yyv_GrammarRef = yy_0_3;
        yyv_Annotation = yy_0_4;
        yyv_Coordinate = yy_0_5;
        yyb = yyh;
        yyh += 14; if (yyh > yyhx) yyExtend();
        yy_1 = (yy) yyv_SKey[1];
        if (yy_1 == (yy) yyu) yyErr(1,260);
        yyv_SubphraseNumber = yy_1;
        NumberOfNonterms(&yy_2_1);
        yyv_NNont = yy_2_1;
        yy_3_1_1 = yyv_SubphraseNumber;
        yy_3_1_2 = yyv_NNont;
        yy_3_1 = (yy)(((long)yy_3_1_1)+((long)yy_3_1_2));
        yy_3_2 = yy_3_1;
        yyv_SNo = yy_3_2;
        yy_4 = (yy) yyv_AKey[2];
        if (yy_4 == (yy) yyu) yyErr(1,263);
        yyv_N = yy_4;
        NumberOfRules(&yy_5_1);
        yyv_NRules = yy_5_1;
        yy_6_1_1 = yyv_N;
        yy_6_1_2 = yyv_NRules;
        yy_6_1 = (yy)(((long)yy_6_1_1)+((long)yy_6_1_2));
        yy_6_2 = yy_6_1;
        yyv_ANo = yy_6_2;
        yy_7_1 = yyv_SNo;
        start_rule(yy_7_1, &yy_7_2);
        yyv_RuleHandle = yy_7_2;
        yy_8 = yyv_RuleHandle;
        yyv_AKey[4] = (long) yy_8;
        yy_9_1 = yyv_SKey;
        LhsKeyForSubphrase(yy_9_1, &yy_9_2);
        yyv_LhsKey = yy_9_2;
        yy_10 = yyv_LhsKey;
        yyglov_CurLhs = yy_10;
        yy_11 = yyv_SNo;
        yyv_LhsKey[5] = (long) yy_11;
        yyv_ClauseKey = yyb + 0;
        yyb[9] = yyu;
        yyb[8] = yyu;
        yyb[7] = yyu;
        yyb[6] = yyu;
        yyb[5] = yyu;
        yyb[4] = yyu;
        yyb[3] = yyu;
        yyb[2] = yyu;
        yyb[1] = yyu;
        yy_13 = yyv_ClauseKey;
        yyv_AKey[3] = (long) yy_13;
        yy_14 = yyb + 10;
        yy_14[0] = 2;
        yyv_ClauseKey[1] = (long) yy_14;
        yy_15 = yyv_ClauseKey;
        yyglov_CurClause = yy_15;
        yy_16 = yyv_RuleHandle;
        yyv_ClauseKey[6] = (long) yy_16;
        yy_17 = yyv_ANo;
        yyv_ClauseKey[7] = (long) yy_17;
        yy_18 = yyv_GrammarRef;
        yyv_ClauseKey[2] = (long) yy_18;
        yy_19 = yyv_Annotation;
        yyv_ClauseKey[4] = (long) yy_19;
        yy_20 = yyv_Coordinate;
        yyv_ClauseKey[5] = (long) yy_20;
        yy_21 = (yy) yyv_LhsKey[3];
        if (yy_21 == (yy) yyu) yyErr(1,283);
        yyv_Clauses = yy_21;
        yy_22_1 = yyv_Clauses;
        yy_22_2 = yyv_ClauseKey;
        yy_22 = yyb + 11;
        yy_22[0] = 1;
        yy_22[1] = ((long)yy_22_1);
        yy_22[2] = ((long)yy_22_2);
        yyv_LhsKey[3] = (long) yy_22;
        return;
    }
}
END_ALTERNATIVE()
{
    {
        yy yyb;
        return;
    }
}
calc_positions_LHSKEYS(yyin_1)
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
        if (yy_0_1[0] != 1) goto yyfl_40_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Keys = yy_0_1_1;
        yyv_Key = yy_0_1_2;
        yy_1_1 = yyv_Keys;
        calc_positions_LHSKEYS(yy_1_1);
        yy_2_1 = yyv_Key;
        calc_positions_LHSKEY(yy_2_1);
        return;
yyfl_40_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_40_2;
        return;
yyfl_40_2 : ;
    }
    yyErr(2,337);
}
calc_positions_LHSKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_Clauses;
        yy yy_1;
        yy yyv_Pos;
        yy yy_2;
        yy yy_3;
        yy yy_4;
        yy yy_5_1;
        yy yyv_LastClause;
        yy yy_6;
        yy yy_7;
        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = (yy) yyv_Key[3];
        if (yy_1 == (yy) yyu) yyErr(1,347);
        yyv_Clauses = yy_1;
        yy_2 = yyglov_PC;
        if (yy_2 == (yy) yyu) yyErr(1,348);
        yyv_Pos = yy_2;
        yy_3 = yyv_Pos;
        yyv_Key[4] = (long) yy_3;
        yy_4 = yyv_Pos;
        yyglov_CurLhsPos = yy_4;
        yy_5_1 = yyv_Clauses;
        calc_positions_CLAUSEKEYS(yy_5_1);
        yy_6 = yyglov_LastClauseKey;
        if (yy_6 == (yy) yyu) yyErr(1,352);
        yyv_LastClause = yy_6;
        yy_7 = ((yy)0);
        yyv_LastClause[9] = (long) yy_7;
        return;
    }
}
calc_positions_CLAUSEKEYS(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Clauses;
        yy yy_0_1_1;
        yy yyv_Clause;
        yy yy_0_1_2;
        yy yy_1_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_42_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Clauses = yy_0_1_1;
        yyv_Clause = yy_0_1_2;
        yy_1_1 = yyv_Clauses;
        calc_positions_CLAUSEKEYS(yy_1_1);
        yy_2_1 = yyv_Clause;
        calc_positions_CLAUSEKEY(yy_2_1);
        return;
yyfl_42_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_42_2;
        return;
yyfl_42_2 : ;
    }
    yyErr(2,355);
}
calc_positions_CLAUSEKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_Pos;
        yy yy_1;
        yy yyv_Elems;
        yy yy_2;
        yy yy_3_1;
        yy yyv_N;
        yy yy_3_2;
        yy yy_4_1;
        yy yy_4_1_1;
        yy yy_4_1_1_1;
        yy yy_4_1_1_2;
        yy yy_4_1_2;
        yy yyv_NextPos;
        yy yy_4_2;
        yy yy_5;
        yy yy_6;
        yy yy_7;
        yy yy_8;
        yy yyv_LhsPos;
        yy yy_9;
        yy yy_10;
        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = yyglov_PC;
        if (yy_1 == (yy) yyu) yyErr(1,363);
        yyv_Pos = yy_1;
        yy_2 = (yy) yyv_Key[1];
        if (yy_2 == (yy) yyu) yyErr(1,364);
        yyv_Elems = yy_2;
        yy_3_1 = yyv_Elems;
        Length(yy_3_1, &yy_3_2);
        yyv_N = yy_3_2;
        yy_4_1_1_1 = yyv_Pos;
        yy_4_1_1_2 = yyv_N;
        yy_4_1_1 = (yy)(((long)yy_4_1_1_1)+((long)yy_4_1_1_2));
        yy_4_1_2 = ((yy)3);
        yy_4_1 = (yy)(((long)yy_4_1_1)+((long)yy_4_1_2));
        yy_4_2 = yy_4_1;
        yyv_NextPos = yy_4_2;
        yy_5 = yyv_NextPos;
        yyglov_PC = yy_5;
        yy_6 = yyv_Key;
        yyglov_LastClauseKey = yy_6;
        yy_7 = yyv_Pos;
        yyv_Key[3] = (long) yy_7;
        yy_8 = yyv_NextPos;
        yyv_Key[9] = (long) yy_8;
        yy_9 = yyglov_CurLhsPos;
        if (yy_9 == (yy) yyu) yyErr(1,371);
        yyv_LhsPos = yy_9;
        yy_10 = yyv_LhsPos;
        yyv_Key[8] = (long) yy_10;
        return;
    }
}
Length(yyin_1, yyout_1)
    yy yyin_1;
yy *yyout_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Elems;
        yy yy_0_1_1;
        yy yyv_Elem;
        yy yy_0_1_2;
        yy yy_0_2;
        yy yy_0_2_1;
        yy yy_0_2_2;
        yy yy_1_1;
        yy yyv_N;
        yy yy_1_2;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_44_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Elems = yy_0_1_1;
        yyv_Elem = yy_0_1_2;
        yy_1_1 = yyv_Elems;
        Length(yy_1_1, &yy_1_2);
        yyv_N = yy_1_2;
        yy_0_2_1 = yyv_N;
        yy_0_2_2 = ((yy)1);
        yy_0_2 = (yy)(((long)yy_0_2_1)+((long)yy_0_2_2));
        *yyout_1 = yy_0_2;
        return;
yyfl_44_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yy_0_2;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_44_2;
        yy_0_2 = ((yy)0);
        *yyout_1 = yy_0_2;
        return;
yyfl_44_2 : ;
    }
    yyErr(2,374);
}
write_code_LHSKEYS(yyin_1)
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
        if (yy_0_1[0] != 1) goto yyfl_45_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Keys = yy_0_1_1;
        yyv_Key = yy_0_1_2;
        yy_1_1 = yyv_Keys;
        write_code_LHSKEYS(yy_1_1);
        yy_2_1 = yyv_Key;
        write_code_LHSKEY(yy_2_1);
        return;
yyfl_45_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_45_2;
        return;
yyfl_45_2 : ;
    }
    yyErr(2,381);
}
write_code_LHSKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_Clauses;
        yy yy_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = (yy) yyv_Key[3];
        if (yy_1 == (yy) yyu) yyErr(1,391);
        yyv_Clauses = yy_1;
        yy_2_1 = yyv_Clauses;
        write_code_CLAUSEKEYS(yy_2_1);
        return;
    }
}
write_code_CLAUSEKEYS(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Clauses;
        yy yy_0_1_1;
        yy yyv_Clause;
        yy yy_0_1_2;
        yy yy_1_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_47_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Clauses = yy_0_1_1;
        yyv_Clause = yy_0_1_2;
        yy_1_1 = yyv_Clauses;
        write_code_CLAUSEKEYS(yy_1_1);
        yy_2_1 = yyv_Clause;
        write_code_CLAUSEKEY(yy_2_1);
        return;
yyfl_47_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_47_2;
        return;
yyfl_47_2 : ;
    }
    yyErr(2,394);
}
write_code_CLAUSEKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_Elems;
        yy yy_1;
        yy yyv_Chain;
        yy yy_2;
        yy yy_3_1;
        yy yy_4_1;
        yy yyv_LhsPos;
        yy yy_5;
        yy yy_6_1;
        yy yy_6_1_1;
        yy yyv_RuleNumber;
        yy yy_7;
        yy yy_8_1;
        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = (yy) yyv_Key[1];
        if (yy_1 == (yy) yyu) yyErr(1,402);
        yyv_Elems = yy_1;
        yy_2 = (yy) yyv_Key[9];
        if (yy_2 == (yy) yyu) yyErr(1,404);
        yyv_Chain = yy_2;
        yy_3_1 = yyv_Chain;
        set_field(yy_3_1);
        yy_4_1 = yyv_Elems;
        write_code_ELEMS(yy_4_1);
        yy_5 = (yy) yyv_Key[8];
        if (yy_5 == (yy) yyu) yyErr(1,409);
        yyv_LhsPos = yy_5;
        yy_6_1_1 = yyv_LhsPos;
        yy_6_1 = (yy)(-((long)yy_6_1_1));
        set_field(yy_6_1);
        yy_7 = (yy) yyv_Key[7];
        if (yy_7 == (yy) yyu) yyErr(1,412);
        yyv_RuleNumber = yy_7;
        yy_8_1 = yyv_RuleNumber;
        set_field(yy_8_1);
        return;
    }
}
write_code_ELEMS(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Elems;
        yy yy_0_1_1;
        yy yyv_Elem;
        yy yy_0_1_2;
        yy yy_1_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_49_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Elems = yy_0_1_1;
        yyv_Elem = yy_0_1_2;
        yy_1_1 = yyv_Elems;
        write_code_ELEMS(yy_1_1);
        yy_2_1 = yyv_Elem;
        write_code_ELEM(yy_2_1);
        return;
yyfl_49_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_49_2;
        return;
yyfl_49_2 : ;
    }
    yyErr(2,415);
}
write_code_ELEM(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Id;
        yy yy_0_1_1;
        yy yy_0_1_2;
        yy yy_0_1_3;
        yy yy_1_1;
        yy yyv_LhsKey;
        yy yy_1_2;
        yy yyv_Pos;
        yy yy_2;
        yy yy_3_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_50_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yyv_Id = yy_0_1_1;
        yy_1_1 = yyv_Id;
        LhsKeyForNonterm(yy_1_1, &yy_1_2);
        yyv_LhsKey = yy_1_2;
        yy_2 = (yy) yyv_LhsKey[4];
        if (yy_2 == (yy) yyu) yyErr(1,424);
        yyv_Pos = yy_2;
        yy_3_1 = yyv_Pos;
        set_field(yy_3_1);
        return;
yyfl_50_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_N;
        yy yy_0_1_1;
        yy yyv_TERMBASE;
        yy yy_1;
        yy yy_2_1;
        yy yy_2_1_1;
        yy yy_2_1_2;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_50_2;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yyv_N = yy_0_1_1;
        yy_1 = yyglov_term_base;
        if (yy_1 == (yy) yyu) yyErr(1,427);
        yyv_TERMBASE = yy_1;
        yy_2_1_1 = yyv_N;
        yy_2_1_2 = yyv_TERMBASE;
        yy_2_1 = (yy)(((long)yy_2_1_1)+((long)yy_2_1_2));
        set_field(yy_2_1);
        return;
yyfl_50_2 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_SKey;
        yy yy_0_1_1;
        yy yy_0_1_2;
        yy yy_0_1_3;
        yy yy_1_1;
        yy yyv_LhsKey;
        yy yy_1_2;
        yy yyv_Pos;
        yy yy_2;
        yy yy_3_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 3) goto yyfl_50_3;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yyv_SKey = yy_0_1_1;
        yy_1_1 = yyv_SKey;
        LhsKeyForSubphrase(yy_1_1, &yy_1_2);
        yyv_LhsKey = yy_1_2;
        yy_2 = (yy) yyv_LhsKey[4];
        if (yy_2 == (yy) yyu) yyErr(1,431);
        yyv_Pos = yy_2;
        yy_3_1 = yyv_Pos;
        set_field(yy_3_1);
        return;
yyfl_50_3 : ;
    }
    yyErr(2,421);
}
set_field(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_N;
        yy yy_0_1;
        yy yyv_X;
        yy yy_1;
        yy yy_2_1;
        yy yy_3_1;
        yy yy_4_1;
        yy yy_5_1;
        yy yy_6_1;
        yy yy_8;
        yy yy_8_1;
        yy yy_8_2;
        yy_0_1 = yyin_1;
        yyv_N = yy_0_1;
        yy_1 = yyglov_FieldIndex;
        if (yy_1 == (yy) yyu) yyErr(1,438);
        yyv_X = yy_1;
        yy_2_1 = ((yy)"/* ");
        Put(yy_2_1);
        yy_3_1 = yyv_X;
        PutI(yy_3_1);
        yy_4_1 = ((yy)" */ ");
        Put(yy_4_1);
        yy_5_1 = yyv_N;
        PutI(yy_5_1);
        yy_6_1 = ((yy)",");
        Put(yy_6_1);
        Nl();
        yy_8_1 = yyv_X;
        yy_8_2 = ((yy)1);
        yy_8 = (yy)(((long)yy_8_1)+((long)yy_8_2));
        yyglov_FieldIndex = yy_8;
        return;
    }
}
write_annotation_LHSKEYS(yyin_1)
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
        if (yy_0_1[0] != 1) goto yyfl_53_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Keys = yy_0_1_1;
        yyv_Key = yy_0_1_2;
        yy_1_1 = yyv_Keys;
        write_annotation_LHSKEYS(yy_1_1);
        yy_2_1 = yyv_Key;
        write_annotation_LHSKEY(yy_2_1);
        return;
yyfl_53_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_53_2;
        return;
yyfl_53_2 : ;
    }
    yyErr(2,444);
}
write_annotation_LHSKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_Clauses;
        yy yy_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = (yy) yyv_Key[3];
        if (yy_1 == (yy) yyu) yyErr(1,454);
        yyv_Clauses = yy_1;
        yy_2_1 = yyv_Clauses;
        write_annotation_CLAUSEKEYS(yy_2_1);
        return;
    }
}
write_annotation_CLAUSEKEYS(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Clauses;
        yy yy_0_1_1;
        yy yyv_Clause;
        yy yy_0_1_2;
        yy yy_1_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_55_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Clauses = yy_0_1_1;
        yyv_Clause = yy_0_1_2;
        yy_1_1 = yyv_Clauses;
        write_annotation_CLAUSEKEYS(yy_1_1);
        yy_2_1 = yyv_Clause;
        write_annotation_CLAUSEKEY(yy_2_1);
        return;
yyfl_55_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_55_2;
        return;
yyfl_55_2 : ;
    }
    yyErr(2,457);
}
write_annotation_CLAUSEKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb = NULL;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_Elems;
        yy yy_1;
        yy yyv_Chain;
        yy yy_2;
        yy yy_3_1;
        yy yy_4_1;
        yy yyv_LhsPos;
        yy yy_5;
        yy yy_6_1;
        yy yy_6_1_1;
        yy yyv_Annotation;
        yy yy_7;
        yy yy_8_1_1_1;
        yy yy_8_1_1_2;
        yy yyv_N;
        yy yy_8_1_1_2_1;
        yy yy_8_1_2_1;
        yy yy_8_2_1_1;
        yy yy_8_2_1_2;
        yy yy_8_2_2_1;
        yy yy_8_2_2_1_1;
        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = (yy) yyv_Key[1];
        if (yy_1 == (yy) yyu) yyErr(1,465);
        yyv_Elems = yy_1;
        yy_2 = (yy) yyv_Key[9];
        if (yy_2 == (yy) yyu) yyErr(1,467);
        yyv_Chain = yy_2;
        yy_3_1 = yyv_Chain;
        set_field(yy_3_1);
        yy_4_1 = yyv_Elems;
        write_annotation_ELEMS(yy_4_1);
        yy_5 = (yy) yyv_Key[8];
        if (yy_5 == (yy) yyu) yyErr(1,472);
        yyv_LhsPos = yy_5;
        yy_6_1_1 = yyv_LhsPos;
        yy_6_1 = (yy)(-((long)yy_6_1_1));
        set_field(yy_6_1);
        yy_7 = (yy) yyv_Key[4];
        if (yy_7 == (yy) yyu) yyErr(1,475);
        yyv_Annotation = yy_7;
        {
            yy yysb = yyb;
            yy_8_1_1_1 = yyv_Annotation;
            yy_8_1_1_2 = yy_8_1_1_1;
            if (yy_8_1_1_2[0] != 1) goto yyfl_56_1_8_1;
            yy_8_1_1_2_1 = ((yy)yy_8_1_1_2[1]);
            yyv_N = yy_8_1_1_2_1;
            yy_8_1_2_1 = yyv_N;
            set_field(yy_8_1_2_1);
            goto yysl_56_1_8;
yyfl_56_1_8_1 : ;
            yy_8_2_1_1 = yyv_Annotation;
            yy_8_2_1_2 = yy_8_2_1_1;
            if (yy_8_2_1_2[0] != 2) goto yyfl_56_1_8_2;
            yy_8_2_2_1_1 = ((yy)1);
            yy_8_2_2_1 = (yy)(-((long)yy_8_2_2_1_1));
            set_field(yy_8_2_2_1);
            goto yysl_56_1_8;
yyfl_56_1_8_2 : ;
            goto yyfl_56_1;
yysl_56_1_8 : ;
            yyb = yysb;
        }
        return;
yyfl_56_1 : ;
    }
    yyErr(2,463);
}
write_annotation_ELEMS(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Elems;
        yy yy_0_1_1;
        yy yyv_Elem;
        yy yy_0_1_2;
        yy yy_1_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_57_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Elems = yy_0_1_1;
        yyv_Elem = yy_0_1_2;
        yy_1_1 = yyv_Elems;
        write_annotation_ELEMS(yy_1_1);
        yy_2_1 = yyv_Elem;
        write_annotation_ELEM(yy_2_1);
        return;
yyfl_57_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_57_2;
        return;
yyfl_57_2 : ;
    }
    yyErr(2,484);
}
write_annotation_ELEM(yyin_1)
    yy yyin_1;
{
    {
        yy yyb = NULL;
        yy yy_0_1;
        yy yyv_Id;
        yy yy_0_1_1;
        yy yyv_Annotation;
        yy yy_0_1_2;
        yy yy_0_1_3;
        yy yy_1_1_1_1;
        yy yy_1_1_1_2;
        yy yy_1_1_2_1;
        yy yy_1_2_1_1;
        yy yy_1_2_1_2;
        yy yy_1_2_2_1;
        yy yy_1_3_1_1;
        yy yy_1_3_1_2;
        yy yy_1_3_2_1;
        yy yy_1_4_1_1;
        yy yy_1_4_1_2;
        yy yyv_N;
        yy yy_1_4_1_2_1;
        yy yy_1_4_2_1;
        yy yy_1_4_2_1_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_58_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yyv_Id = yy_0_1_1;
        yyv_Annotation = yy_0_1_2;
        {
            yy yysb = yyb;
            yy_1_1_1_1 = yyv_Annotation;
            yy_1_1_1_2 = yy_1_1_1_1;
            if (yy_1_1_1_2[0] != 1) goto yyfl_58_1_1_1;
            yy_1_1_2_1 = ((yy)1);
            set_field(yy_1_1_2_1);
            goto yysl_58_1_1;
yyfl_58_1_1_1 : ;
            yy_1_2_1_1 = yyv_Annotation;
            yy_1_2_1_2 = yy_1_2_1_1;
            if (yy_1_2_1_2[0] != 2) goto yyfl_58_1_1_2;
            yy_1_2_2_1 = ((yy)2);
            set_field(yy_1_2_2_1);
            goto yysl_58_1_1;
yyfl_58_1_1_2 : ;
            yy_1_3_1_1 = yyv_Annotation;
            yy_1_3_1_2 = yy_1_3_1_1;
            if (yy_1_3_1_2[0] != 3) goto yyfl_58_1_1_3;
            yy_1_3_2_1 = ((yy)0);
            set_field(yy_1_3_2_1);
            goto yysl_58_1_1;
yyfl_58_1_1_3 : ;
            yy_1_4_1_1 = yyv_Annotation;
            yy_1_4_1_2 = yy_1_4_1_1;
            if (yy_1_4_1_2[0] != 4) goto yyfl_58_1_1_4;
            yy_1_4_1_2_1 = ((yy)yy_1_4_1_2[1]);
            yyv_N = yy_1_4_1_2_1;
            yy_1_4_2_1_1 = yyv_N;
            yy_1_4_2_1 = (yy)(-((long)yy_1_4_2_1_1));
            set_field(yy_1_4_2_1);
            goto yysl_58_1_1;
yyfl_58_1_1_4 : ;
            goto yyfl_58_1;
yysl_58_1_1 : ;
            yyb = yysb;
        }
        return;
yyfl_58_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_N;
        yy yy_0_1_1;
        yy yyv_TERMBASE;
        yy yy_1;
        yy yy_2_1;
        yy yy_2_1_1;
        yy yy_2_1_2;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_58_2;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yyv_N = yy_0_1_1;
        yy_1 = yyglov_term_base;
        if (yy_1 == (yy) yyu) yyErr(1,506);
        yyv_TERMBASE = yy_1;
        yy_2_1_1 = yyv_N;
        yy_2_1_2 = yyv_TERMBASE;
        yy_2_1 = (yy)(((long)yy_2_1_1)+((long)yy_2_1_2));
        set_field(yy_2_1);
        return;
yyfl_58_2 : ;
    }
    {
        yy yyb = NULL;
        yy yy_0_1;
        yy yyv_SKey;
        yy yy_0_1_1;
        yy yyv_Annotation;
        yy yy_0_1_2;
        yy yy_0_1_3;
        yy yy_1_1_1_1;
        yy yy_1_1_1_2;
        yy yy_1_1_2_1;
        yy yy_1_2_1_1;
        yy yy_1_2_1_2;
        yy yy_1_2_2_1;
        yy yy_1_3_1_1;
        yy yy_1_3_1_2;
        yy yy_1_3_2_1;
        yy yy_1_4_1_1;
        yy yy_1_4_1_2;
        yy yyv_N;
        yy yy_1_4_1_2_1;
        yy yy_1_4_2_1;
        yy yy_1_4_2_1_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 3) goto yyfl_58_3;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yyv_SKey = yy_0_1_1;
        yyv_Annotation = yy_0_1_2;
        {
            yy yysb = yyb;
            yy_1_1_1_1 = yyv_Annotation;
            yy_1_1_1_2 = yy_1_1_1_1;
            if (yy_1_1_1_2[0] != 1) goto yyfl_58_3_1_1;
            yy_1_1_2_1 = ((yy)1);
            set_field(yy_1_1_2_1);
            goto yysl_58_3_1;
yyfl_58_3_1_1 : ;
            yy_1_2_1_1 = yyv_Annotation;
            yy_1_2_1_2 = yy_1_2_1_1;
            if (yy_1_2_1_2[0] != 2) goto yyfl_58_3_1_2;
            yy_1_2_2_1 = ((yy)2);
            set_field(yy_1_2_2_1);
            goto yysl_58_3_1;
yyfl_58_3_1_2 : ;
            yy_1_3_1_1 = yyv_Annotation;
            yy_1_3_1_2 = yy_1_3_1_1;
            if (yy_1_3_1_2[0] != 3) goto yyfl_58_3_1_3;
            yy_1_3_2_1 = ((yy)0);
            set_field(yy_1_3_2_1);
            goto yysl_58_3_1;
yyfl_58_3_1_3 : ;
            yy_1_4_1_1 = yyv_Annotation;
            yy_1_4_1_2 = yy_1_4_1_1;
            if (yy_1_4_1_2[0] != 4) goto yyfl_58_3_1_4;
            yy_1_4_1_2_1 = ((yy)yy_1_4_1_2[1]);
            yyv_N = yy_1_4_1_2_1;
            yy_1_4_2_1_1 = yyv_N;
            yy_1_4_2_1 = (yy)(-((long)yy_1_4_2_1_1));
            set_field(yy_1_4_2_1);
            goto yysl_58_3_1;
yyfl_58_3_1_4 : ;
            goto yyfl_58_3;
yysl_58_3_1 : ;
            yyb = yysb;
        }
        return;
yyfl_58_3 : ;
    }
    yyErr(2,490);
}
write_coordinates_LHSKEYS(yyin_1)
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
        if (yy_0_1[0] != 1) goto yyfl_59_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Keys = yy_0_1_1;
        yyv_Key = yy_0_1_2;
        yy_1_1 = yyv_Keys;
        write_coordinates_LHSKEYS(yy_1_1);
        yy_2_1 = yyv_Key;
        write_coordinates_LHSKEY(yy_2_1);
        return;
yyfl_59_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_59_2;
        return;
yyfl_59_2 : ;
    }
    yyErr(2,525);
}
write_coordinates_LHSKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_Clauses;
        yy yy_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = (yy) yyv_Key[3];
        if (yy_1 == (yy) yyu) yyErr(1,535);
        yyv_Clauses = yy_1;
        yy_2_1 = yyv_Clauses;
        write_coordinates_CLAUSEKEYS(yy_2_1);
        return;
    }
}
write_coordinates_CLAUSEKEYS(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Clauses;
        yy yy_0_1_1;
        yy yyv_Clause;
        yy yy_0_1_2;
        yy yy_1_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_61_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Clauses = yy_0_1_1;
        yyv_Clause = yy_0_1_2;
        yy_1_1 = yyv_Clauses;
        write_coordinates_CLAUSEKEYS(yy_1_1);
        yy_2_1 = yyv_Clause;
        write_coordinates_CLAUSEKEY(yy_2_1);
        return;
yyfl_61_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_61_2;
        return;
yyfl_61_2 : ;
    }
    yyErr(2,538);
}
write_coordinates_CLAUSEKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_Elems;
        yy yy_1;
        yy yyv_Chain;
        yy yy_2;
        yy yy_3_1;
        yy yy_4_1;
        yy yyv_LhsPos;
        yy yy_5;
        yy yy_6_1;
        yy yyv_Coordinate;
        yy yy_7;
        yy yy_8_1;
        yy yyv_N;
        yy yy_8_2;
        yy yy_9_1;
        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = (yy) yyv_Key[1];
        if (yy_1 == (yy) yyu) yyErr(1,546);
        yyv_Elems = yy_1;
        yy_2 = (yy) yyv_Key[9];
        if (yy_2 == (yy) yyu) yyErr(1,548);
        yyv_Chain = yy_2;
        yy_3_1 = ((yy)9999);
        set_field(yy_3_1);
        yy_4_1 = yyv_Elems;
        write_coordinates_ELEMS(yy_4_1);
        yy_5 = (yy) yyv_Key[8];
        if (yy_5 == (yy) yyu) yyErr(1,553);
        yyv_LhsPos = yy_5;
        yy_6_1 = ((yy)9999);
        set_field(yy_6_1);
        yy_7 = (yy) yyv_Key[5];
        if (yy_7 == (yy) yyu) yyErr(1,556);
        yyv_Coordinate = yy_7;
        yy_8_1 = yyv_Coordinate;
        POS_to_INT(yy_8_1, &yy_8_2);
        yyv_N = yy_8_2;
        yy_9_1 = yyv_N;
        set_field(yy_9_1);
        return;
    }
}
write_coordinates_ELEMS(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Elems;
        yy yy_0_1_1;
        yy yyv_Elem;
        yy yy_0_1_2;
        yy yy_1_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_63_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Elems = yy_0_1_1;
        yyv_Elem = yy_0_1_2;
        yy_1_1 = yyv_Elems;
        write_coordinates_ELEMS(yy_1_1);
        yy_2_1 = yyv_Elem;
        write_coordinates_ELEM(yy_2_1);
        return;
yyfl_63_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_63_2;
        return;
yyfl_63_2 : ;
    }
    yyErr(2,560);
}
write_coordinates_ELEM(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Id;
        yy yy_0_1_1;
        yy yyv_Annotation;
        yy yy_0_1_2;
        yy yyv_Coordinate;
        yy yy_0_1_3;
        yy yy_1_1;
        yy yyv_N;
        yy yy_1_2;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_64_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yyv_Id = yy_0_1_1;
        yyv_Annotation = yy_0_1_2;
        yyv_Coordinate = yy_0_1_3;
        yy_1_1 = yyv_Coordinate;
        POS_to_INT(yy_1_1, &yy_1_2);
        yyv_N = yy_1_2;
        yy_2_1 = yyv_N;
        set_field(yy_2_1);
        return;
yyfl_64_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_N;
        yy yy_0_1_1;
        yy yy_1_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_64_2;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yyv_N = yy_0_1_1;
        yy_1_1 = ((yy)9999);
        set_field(yy_1_1);
        return;
yyfl_64_2 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_SKey;
        yy yy_0_1_1;
        yy yy_0_1_2;
        yy yyv_Coordinate;
        yy yy_0_1_3;
        yy yy_1_1;
        yy yyv_N;
        yy yy_1_2;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 3) goto yyfl_64_3;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yy_0_1_3 = ((yy)yy_0_1[3]);
        yyv_SKey = yy_0_1_1;
        yyv_Coordinate = yy_0_1_3;
        yy_1_1 = yyv_Coordinate;
        POS_to_INT(yy_1_1, &yy_1_2);
        yyv_N = yy_1_2;
        yy_2_1 = yyv_N;
        set_field(yy_2_1);
        return;
yyfl_64_3 : ;
    }
    yyErr(2,566);
}
write_dir_sets_LHSKEYS(yyin_1)
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
        if (yy_0_1[0] != 1) goto yyfl_65_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Keys = yy_0_1_1;
        yyv_Key = yy_0_1_2;
        yy_1_1 = yyv_Keys;
        write_dir_sets_LHSKEYS(yy_1_1);
        yy_2_1 = yyv_Key;
        write_dir_sets_LHSKEY(yy_2_1);
        return;
yyfl_65_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_65_2;
        return;
yyfl_65_2 : ;
    }
    yyErr(2,578);
}
write_dir_sets_LHSKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_Clauses;
        yy yy_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = (yy) yyv_Key[3];
        if (yy_1 == (yy) yyu) yyErr(1,588);
        yyv_Clauses = yy_1;
        yy_2_1 = yyv_Clauses;
        write_dir_sets_CLAUSEKEYS(yy_2_1);
        return;
    }
}
write_dir_sets_CLAUSEKEYS(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Clauses;
        yy yy_0_1_1;
        yy yyv_Clause;
        yy yy_0_1_2;
        yy yy_1_1;
        yy yy_2_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_67_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Clauses = yy_0_1_1;
        yyv_Clause = yy_0_1_2;
        yy_1_1 = yyv_Clauses;
        write_dir_sets_CLAUSEKEYS(yy_1_1);
        yy_2_1 = yyv_Clause;
        write_dir_sets_CLAUSEKEY(yy_2_1);
        return;
yyfl_67_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_67_2;
        return;
yyfl_67_2 : ;
    }
    yyErr(2,591);
}
write_dir_sets_CLAUSEKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_K;
        yy yy_1;
        yy yyv_Handle;
        yy yy_2;
        yy yy_3_1;
        yy yyv_Set;
        yy yy_3_2;
        yy yy_4_1;
        yy yy_4_2;
        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = (yy) yyv_Key[7];
        if (yy_1 == (yy) yyu) yyErr(1,599);
        yyv_K = yy_1;
        yy_2 = (yy) yyv_Key[6];
        if (yy_2 == (yy) yyu) yyErr(1,600);
        yyv_Handle = yy_2;
        yy_3_1 = yyv_Handle;
        get_dirset(yy_3_1, &yy_3_2);
        yyv_Set = yy_3_2;
        yy_4_1 = yyv_K;
        yy_4_2 = yyv_Set;
        write_set(yy_4_1, yy_4_2);
        return;
    }
}
write_transparent()
{
    {
        yy yyb;
        yy yyv_LhsKeys;
        yy yy_1;
        yy yy_2_1;
        yy yy_4_1;
        yy yy_6_1;
        yy yy_8_1;
        yy yy_10_1;
        yy yy_10_2;
        yy yy_11_1;
        yy yy_13_1;
        yy_1 = yyglov_LHS_List;
        if (yy_1 == (yy) yyu) yyErr(1,607);
        yyv_LhsKeys = yy_1;
        yy_2_1 = ((yy)"int yytransparent(n)");
        Put(yy_2_1);
        Nl();
        yy_4_1 = ((yy)"   int n;");
        Put(yy_4_1);
        Nl();
        yy_6_1 = ((yy)"{");
        Put(yy_6_1);
        Nl();
        yy_8_1 = ((yy)"   switch(n) {");
        Put(yy_8_1);
        Nl();
        yy_10_1 = yyv_LhsKeys;
        yy_10_2 = ((yy)1);
        write_transparent_LHSKEYS(yy_10_1, yy_10_2);
        yy_11_1 = ((yy)"   }");
        Put(yy_11_1);
        Nl();
        yy_13_1 = ((yy)"}");
        Put(yy_13_1);
        Nl();
        return;
    }
}
write_transparent_LHSKEYS(yyin_1, yyin_2)
    yy yyin_1;
yy yyin_2;
{
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Keys;
        yy yy_0_1_1;
        yy yyv_Key;
        yy yy_0_1_2;
        yy yyv_N;
        yy yy_0_2;
        yy yy_1_1;
        yy yy_1_2;
        yy yy_1_2_1;
        yy yy_1_2_2;
        yy yy_2_1;
        yy yy_2_2;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 1) goto yyfl_70_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Keys = yy_0_1_1;
        yyv_Key = yy_0_1_2;
        yyv_N = yy_0_2;
        yy_1_1 = yyv_Keys;
        yy_1_2_1 = yyv_N;
        yy_1_2_2 = ((yy)1);
        yy_1_2 = (yy)(((long)yy_1_2_1)+((long)yy_1_2_2));
        write_transparent_LHSKEYS(yy_1_1, yy_1_2);
        yy_2_1 = yyv_Key;
        yy_2_2 = yyv_N;
        write_transparent_LHSKEY(yy_2_1, yy_2_2);
        return;
yyfl_70_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yy_0_2;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 2) goto yyfl_70_2;
        return;
yyfl_70_2 : ;
    }
    yyErr(2,616);
}
write_transparent_LHSKEY(yyin_1, yyin_2)
    yy yyin_1;
yy yyin_2;
{
    {
        yy yyb;
        yy yyv_Key;
        yy yy_0_1;
        yy yyv_N;
        yy yy_0_2;
        yy yyv_Lhs;
        yy yy_1;
        yy yyv_Pos;
        yy yy_2;
        yy yy_3_1;
        yy yy_4_1;
        yy yy_5_1;
        yy yyv_NN;
        yy yy_6;
        yy yy_7_1;
        yy yyv_Val;
        yy yy_7_2;
        yy yy_8_1;
        yy yy_9_1;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        yyv_Key = yy_0_1;
        yyv_N = yy_0_2;
        yy_1 = (yy) yyv_Key[1];
        if (yy_1 == (yy) yyu) yyErr(1,626);
        yyv_Lhs = yy_1;
        yy_2 = (yy) yyv_Key[4];
        if (yy_2 == (yy) yyu) yyErr(1,627);
        yyv_Pos = yy_2;
        yy_3_1 = ((yy)"      case ");
        Put(yy_3_1);
        yy_4_1 = yyv_Pos;
        PutI(yy_4_1);
        yy_5_1 = ((yy)": return ");
        Put(yy_5_1);
        yy_6 = (yy) yyv_Key[5];
        if (yy_6 == (yy) yyu) yyErr(1,632);
        yyv_NN = yy_6;
        yy_7_1 = yyv_NN;
        get_transparent(yy_7_1, &yy_7_2);
        yyv_Val = yy_7_2;
        yy_8_1 = yyv_Val;
        PutI(yy_8_1);
        yy_9_1 = ((yy)"; break;");
        Put(yy_9_1);
        Nl();
        return;
    }
}
encode_root_rule()
{
    {
        yy yyb;
        yy yy_1_1;
        yy yyv_STARTUSERGRAMMAR;
        yy yy_2;
        yy yy_3_1;
        yy yyv_TERMBASE;
        yy yy_4;
        yy yy_5_1;
        yy yy_6_1;
        yy yy_6_1_1;
        yy yy_7_1;
        yy yy_8_1;
        yy yy_9_1;
        yy_1_1 = ((yy)0);
        set_field(yy_1_1);
        yy_2 = yyglov_start_user_grammar;
        if (yy_2 == (yy) yyu) yyErr(1,647);
        yyv_STARTUSERGRAMMAR = yy_2;
        yy_3_1 = yyv_STARTUSERGRAMMAR;
        set_field(yy_3_1);
        yy_4 = yyglov_term_base;
        if (yy_4 == (yy) yyu) yyErr(1,649);
        yyv_TERMBASE = yy_4;
        yy_5_1 = yyv_TERMBASE;
        set_field(yy_5_1);
        yy_6_1_1 = ((yy)1);
        yy_6_1 = (yy)(-((long)yy_6_1_1));
        set_field(yy_6_1);
        yy_7_1 = ((yy)0);
        set_field(yy_7_1);
        yy_8_1 = ((yy)1234);
        set_field(yy_8_1);
        yy_9_1 = ((yy)5678);
        set_field(yy_9_1);
        return;
    }
}
LhsKeyForNonterm(yyin_1, yyout_1)
    yy yyin_1;
yy *yyout_1;
{
    {
        yy yyb;
        yy yyv_Id;
        yy yy_0_1;
        yy yy_0_2;
        yy yyv_L;
        yy yy_1;
        yy yy_2_1;
        yy yy_2_2;
        yy yyv_K;
        yy yy_2_3;
        yy_0_1 = yyin_1;
        yyv_Id = yy_0_1;
        yy_1 = yyglov_LHS_List;
        if (yy_1 == (yy) yyu) yyErr(1,662);
        yyv_L = yy_1;
        yy_2_1 = yyv_L;
        yy_2_2 = yyv_Id;
        lookup_Id(yy_2_1, yy_2_2, &yy_2_3);
        yyv_K = yy_2_3;
        yy_0_2 = yyv_K;
        *yyout_1 = yy_0_2;
        return;
    }
}
lookup_Id(yyin_1, yyin_2, yyout_1)
    yy yyin_1;
yy yyin_2;
yy *yyout_1;
{
    {
        yy yyb = NULL;
        yy yy_0_1 = NULL;
        yy yyv_Keys = NULL;
        yy yy_0_1_1 = NULL;
        yy yyv_Key = NULL;
        yy yy_0_1_2 = NULL;
        yy yyv_Id = NULL;
        yy yy_0_2 = NULL;
        yy yy_0_3 = NULL;
        yy yy_1_1_1 = NULL;
        yy yyv_Id0 = NULL;
        yy yy_1_1_1_1 = NULL;
        yy yy_1_1_2_1 = NULL;
        yy yy_1_1_2_2 = NULL;
        yy yy_1_1_3_1 = NULL;
        yy yyv_K = NULL;
        yy yy_1_1_3_2 = NULL;
        yy yy_1_2_1_1 = NULL;
        yy yy_1_2_1_2 = NULL;
        yy yy_1_2_1_3 = NULL;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 1) goto yyfl_76_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Keys = yy_0_1_1;
        yyv_Key = yy_0_1_2;
        yyv_Id = yy_0_2;
        {
            yy yysb = yyb;
            yy_1_1_1 = (yy) yyv_Key[1];
            if (yy_1_1_1 == (yy) yyu) yyErr(1,668);
            if (yy_1_1_1[0] != 1) goto yyfl_76_1_1_1;
            yy_1_1_1_1 = ((yy)yy_1_1_1[1]);
            yyv_Id0 = yy_1_1_1_1;
            yy_1_1_2_1 = yyv_Id;
            yy_1_1_2_2 = yyv_Id0;
            if (! yyeq_IDENT(yy_1_1_2_1, yy_1_1_2_2)) goto yyfl_76_1_1_1;
            yy_1_1_3_1 = yyv_Key;
            yy_1_1_3_2 = yy_1_1_3_1;
            yyv_K = yy_1_1_3_2;
            goto yysl_76_1_1;
yyfl_76_1_1_1 : ;
            yy_1_2_1_1 = yyv_Keys;
            yy_1_2_1_2 = yyv_Id;
            lookup_Id(yy_1_2_1_1, yy_1_2_1_2, &yy_1_2_1_3);
            yyv_K = yy_1_2_1_3;
            goto yysl_76_1_1;
yysl_76_1_1 : ;
            yyb = yysb;
        }
        yy_0_3 = yyv_K;
        *yyout_1 = yy_0_3;
        return;
yyfl_76_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Id;
        yy yy_0_2;
        yy yy_0_3;
        yy yyv_K;
        yy yy_2;
        yy yy_2_1;
        yy yy_3;
        yy yyv_List;
        yy yy_4;
        yy yy_5;
        yy yy_5_1;
        yy yy_5_2;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 2) goto yyfl_76_2;
        yyv_Id = yy_0_2;
        yyb = yyh;
        yyh += 12; if (yyh > yyhx) yyExtend();
        yyv_K = yyb + 0;
        yyb[5] = yyu;
        yyb[4] = yyu;
        yyb[3] = yyu;
        yyb[2] = yyu;
        yyb[1] = yyu;
        yy_2_1 = yyv_Id;
        yy_2 = yyb + 6;
        yy_2[0] = 1;
        yy_2[1] = ((long)yy_2_1);
        yyv_K[1] = (long) yy_2;
        yy_3 = yyb + 8;
        yy_3[0] = 2;
        yyv_K[3] = (long) yy_3;
        yy_4 = yyglov_LHS_List;
        if (yy_4 == (yy) yyu) yyErr(1,679);
        yyv_List = yy_4;
        yy_5_1 = yyv_List;
        yy_5_2 = yyv_K;
        yy_5 = yyb + 9;
        yy_5[0] = 1;
        yy_5[1] = ((long)yy_5_1);
        yy_5[2] = ((long)yy_5_2);
        yyglov_LHS_List = yy_5;
        yy_0_3 = yyv_K;
        *yyout_1 = yy_0_3;
        return;
yyfl_76_2 : ;
    }
    yyErr(2,665);
}
LhsKeyForSubphrase(yyin_1, yyout_1)
    yy yyin_1;
yy *yyout_1;
{
    {
        yy yyb;
        yy yyv_Id;
        yy yy_0_1;
        yy yy_0_2;
        yy yyv_L;
        yy yy_1;
        yy yy_2_1;
        yy yy_2_2;
        yy yyv_K;
        yy yy_2_3;
        yy_0_1 = yyin_1;
        yyv_Id = yy_0_1;
        yy_1 = yyglov_LHS_List;
        if (yy_1 == (yy) yyu) yyErr(1,687);
        yyv_L = yy_1;
        yy_2_1 = yyv_L;
        yy_2_2 = yyv_Id;
        lookup_Subphrase(yy_2_1, yy_2_2, &yy_2_3);
        yyv_K = yy_2_3;
        yy_0_2 = yyv_K;
        *yyout_1 = yy_0_2;
        return;
    }
}
lookup_Subphrase(yyin_1, yyin_2, yyout_1)
    yy yyin_1;
yy yyin_2;
yy *yyout_1;
{
    {
        yy yyb = NULL;
        yy yy_0_1 = NULL;
        yy yyv_Keys = NULL;
        yy yy_0_1_1 = NULL;
        yy yyv_Key = NULL;
        yy yy_0_1_2 = NULL;
        yy yyv_Id = NULL;
        yy yy_0_2 = NULL;
        yy yy_0_3 = NULL;
        yy yy_1_1_1 = NULL;
        yy yyv_Id0 = NULL;
        yy yy_1_1_1_1 = NULL;
        yy yy_1_1_2_1 = NULL;
        yy yy_1_1_2_2 = NULL;
        yy yy_1_1_3_1 = NULL;
        yy yyv_K = NULL;
        yy yy_1_1_3_2 = NULL;
        yy yy_1_2_1_1 = NULL;
        yy yy_1_2_1_2 = NULL;
        yy yy_1_2_1_3 = NULL;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 1) goto yyfl_78_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Keys = yy_0_1_1;
        yyv_Key = yy_0_1_2;
        yyv_Id = yy_0_2;
        {
            yy yysb = yyb;
            yy_1_1_1 = (yy) yyv_Key[1];
            if (yy_1_1_1 == (yy) yyu) yyErr(1,693);
            if (yy_1_1_1[0] != 2) goto yyfl_78_1_1_1;
            yy_1_1_1_1 = ((yy)yy_1_1_1[1]);
            yyv_Id0 = yy_1_1_1_1;
            yy_1_1_2_1 = yyv_Id;
            yy_1_1_2_2 = yyv_Id0;
            if (! yyeq_SUBPHRASEKEY(yy_1_1_2_1, yy_1_1_2_2)) goto yyfl_78_1_1_1;
            yy_1_1_3_1 = yyv_Key;
            yy_1_1_3_2 = yy_1_1_3_1;
            yyv_K = yy_1_1_3_2;
            goto yysl_78_1_1;
yyfl_78_1_1_1 : ;
            yy_1_2_1_1 = yyv_Keys;
            yy_1_2_1_2 = yyv_Id;
            lookup_Subphrase(yy_1_2_1_1, yy_1_2_1_2, &yy_1_2_1_3);
            yyv_K = yy_1_2_1_3;
            goto yysl_78_1_1;
yysl_78_1_1 : ;
            yyb = yysb;
        }
        yy_0_3 = yyv_K;
        *yyout_1 = yy_0_3;
        return;
yyfl_78_1 : ;
    }
    {
        yy yyb;
        yy yy_0_1;
        yy yyv_Id;
        yy yy_0_2;
        yy yy_0_3;
        yy yyv_K;
        yy yy_2;
        yy yy_2_1;
        yy yy_3;
        yy yyv_List;
        yy yy_4;
        yy yy_5;
        yy yy_5_1;
        yy yy_5_2;
        yy_0_1 = yyin_1;
        yy_0_2 = yyin_2;
        if (yy_0_1[0] != 2) goto yyfl_78_2;
        yyv_Id = yy_0_2;
        yyb = yyh;
        yyh += 12; if (yyh > yyhx) yyExtend();
        yyv_K = yyb + 0;
        yyb[5] = yyu;
        yyb[4] = yyu;
        yyb[3] = yyu;
        yyb[2] = yyu;
        yyb[1] = yyu;
        yy_2_1 = yyv_Id;
        yy_2 = yyb + 6;
        yy_2[0] = 2;
        yy_2[1] = ((long)yy_2_1);
        yyv_K[1] = (long) yy_2;
        yy_3 = yyb + 8;
        yy_3[0] = 2;
        yyv_K[3] = (long) yy_3;
        yy_4 = yyglov_LHS_List;
        if (yy_4 == (yy) yyu) yyErr(1,704);
        yyv_List = yy_4;
        yy_5_1 = yyv_List;
        yy_5_2 = yyv_K;
        yy_5 = yyb + 9;
        yy_5[0] = 1;
        yy_5[1] = ((long)yy_5_1);
        yy_5[2] = ((long)yy_5_2);
        yyglov_LHS_List = yy_5;
        yy_0_3 = yyv_K;
        *yyout_1 = yy_0_3;
        return;
yyfl_78_2 : ;
    }
    yyErr(2,690);
}
write_printnames()
{
    {
        yy yyb = NULL;
        yy yyv_LhsKeys = NULL;
        yy yy_1 = NULL;
        yy yy_2_1 = NULL;
        yy yy_4_1 = NULL;
        yy yy_6_1 = NULL;
        yy yyv_TERMBASE = NULL;
        yy yy_8 = NULL;
        yy yy_9_1 = NULL;
        yy yy_10_1 = NULL;
        yy yy_11_1 = NULL;
        yy yy_13_1 = NULL;
        yy yy_15_1 = NULL;
        yy yy_16_1 = NULL;
        yy yy_18_1 = NULL;
        yy yy_20_1 = NULL;
        yy yy_21_1 = NULL;
        yy yy_22_1 = NULL;
        yy yy_25_1 = NULL;
        yy yy_27_1 = NULL;
        yy yy_29_1 = NULL;
        yy_1 = yyglov_LHS_List;
        if (yy_1 == (yy) yyu) yyErr(1,711);
        yyv_LhsKeys = yy_1;
        yy_2_1 = ((yy)"char * yyprintname(n)");
        Put(yy_2_1);
        Nl();
        yy_4_1 = ((yy)"   int n;");
        Put(yy_4_1);
        Nl();
        yy_6_1 = ((yy)"{");
        Put(yy_6_1);
        Nl();
        yy_8 = yyglov_term_base;
        if (yy_8 == (yy) yyu) yyErr(1,715);
        yyv_TERMBASE = yy_8;
        yy_9_1 = ((yy)"   if (n <= ");
        Put(yy_9_1);
        yy_10_1 = yyv_TERMBASE;
        PutI(yy_10_1);
        yy_11_1 = ((yy)")");
        Put(yy_11_1);
        Nl();
        yy_13_1 = ((yy)"      switch(n) {");
        Put(yy_13_1);
        Nl();
        yy_15_1 = yyv_LhsKeys;
        write_nonterms_LHSKEYS(yy_15_1);
        yy_16_1 = ((yy)"   }");
        Put(yy_16_1);
        Nl();
        yy_18_1 = ((yy)"   else ");
        Put(yy_18_1);
        Nl();
        yy_20_1 = ((yy)"      switch(n-");
        Put(yy_20_1);
        yy_21_1 = yyv_TERMBASE;
        PutI(yy_21_1);
        yy_22_1 = ((yy)") {");
        Put(yy_22_1);
        Nl();
        lex_printnames();
        yy_25_1 = ((yy)"      }");
        Put(yy_25_1);
        Nl();
        yy_27_1 = ((yy)"   return \"special_character\";");
        Put(yy_27_1);
        Nl();
        yy_29_1 = ((yy)"}");
        Put(yy_29_1);
        Nl();
        return;
    }
}
void write_nonterms_LHSKEYS(yyin_1)
    yy yyin_1;
{
    {
        yy yyb = NULL;
        yy yy_0_1 = NULL;
        yy yyv_Keys = NULL;
        yy yy_0_1_1 = NULL;
        yy yyv_Key = NULL;
        yy yy_0_1_2 = NULL;
        yy yy_1_1 = NULL;
        yy yy_2_1 = NULL;
        yy_0_1 = yyin_1;
        if (yy_0_1[0] != 1) goto yyfl_80_1;
        yy_0_1_1 = ((yy)yy_0_1[1]);
        yy_0_1_2 = ((yy)yy_0_1[2]);
        yyv_Keys = yy_0_1_1;
        yyv_Key = yy_0_1_2;
        yy_1_1 = yyv_Keys;
        write_nonterms_LHSKEYS(yy_1_1);
        yy_2_1 = yyv_Key;
        write_nonterms_LHSKEY(yy_2_1);
        return;
yyfl_80_1 : ;
    }
    {
        yy yy_0_1 = yyin_1;
        if (yy_0_1[0] != 2) goto yyfl_80_2;
        return;
yyfl_80_2 : ;
    }
    yyErr(2,729);
}
void write_nonterms_LHSKEY(yyin_1)
    yy yyin_1;
{
    {
        yy yyb = NULL;
        yy yyv_Key = NULL;
        yy yy_0_1 = NULL;
        yy yyv_Lhs = NULL;
        yy yy_1 = NULL;
        yy yyv_Pos = NULL;
        yy yy_2 = NULL;
        yy yy_3_1 = NULL;
        yy yy_4_1 = NULL;
        yy yy_5_1 = NULL;
        yy yy_6_1_1_1 = NULL;
        yy yy_6_1_1_2 = NULL;
        yy yyv_Id = NULL;
        yy yy_6_1_1_2_1 = NULL;
        yy yy_6_1_2_1 = NULL;
        yy yyv_Str = NULL;
        yy yy_6_1_2_2 = NULL;
        yy yy_6_1_3_1 = NULL;
        yy yy_6_2_1_1 = NULL;
        yy yy_6_2_1_2 = NULL;
        yy yyv_Sub = NULL;
        yy yy_6_2_1_2_1 = NULL;
        yy yy_6_2_2_1 = NULL;
        yy yy_7_1 = NULL;
        yy_0_1 = yyin_1;
        yyv_Key = yy_0_1;
        yy_1 = (yy) yyv_Key[1];
        if (yy_1 == (yy) yyu) yyErr(1,739);
        yyv_Lhs = yy_1;
        yy_2 = (yy) yyv_Key[4];
        if (yy_2 == (yy) yyu) yyErr(1,740);
        yyv_Pos = yy_2;
        yy_3_1 = ((yy)"         case ");
        Put(yy_3_1);
        yy_4_1 = yyv_Pos;
        PutI(yy_4_1);
        yy_5_1 = ((yy)": return \"");
        Put(yy_5_1);
        {
            yy yysb = yyb;
            yy_6_1_1_1 = yyv_Lhs;
            yy_6_1_1_2 = yy_6_1_1_1;
            if (yy_6_1_1_2[0] != 1) goto yyfl_81_1_6_1;
            yy_6_1_1_2_1 = ((yy)yy_6_1_1_2[1]);
            yyv_Id = yy_6_1_1_2_1;
            yy_6_1_2_1 = yyv_Id;
            id_to_string(yy_6_1_2_1, &yy_6_1_2_2);
            yyv_Str = yy_6_1_2_2;
            yy_6_1_3_1 = yyv_Str;
            Put(yy_6_1_3_1);
            goto yysl_81_1_6;
yyfl_81_1_6_1 : ;
            yy_6_2_1_1 = yyv_Lhs;
            yy_6_2_1_2 = yy_6_2_1_1;
            if (yy_6_2_1_2[0] != 2) goto yyfl_81_1_6_2;
            yy_6_2_1_2_1 = ((yy)yy_6_2_1_2[1]);
            yyv_Sub = yy_6_2_1_2_1;
            yy_6_2_2_1 = ((yy)"Subphrase");
            Put(yy_6_2_2_1);
            goto yysl_81_1_6;
yyfl_81_1_6_2 : ;
            goto yyfl_81_1;
yysl_81_1_6 : ;
            yyb = yysb;
        }
        yy_7_1 = ((yy)"\"; break;");
        Put(yy_7_1);
        Nl();
        return;
yyfl_81_1 : ;
    }
    yyErr(2,736);
}
void TableUpb2(yyin_1)
    yy yyin_1;
{
    {
        yy yyb = NULL;
        yy yyv_N = NULL;
        yy yy_0_1 = NULL;
        yy yyv_Old = NULL;
        yy yy_1 = NULL;
        yy yy_2_1_1_1 = NULL;
        yy yy_2_1_1_2 = NULL;
        yy yy_2_1_2 = NULL;
        yy_0_1 = yyin_1;
        yyv_N = yy_0_1;
        yy_1 = yyglov_TABLEUPB2;
        if (yy_1 == (yy) yyu) yyErr(1,763);
        yyv_Old = yy_1;
        {
            yy yysb = yyb;
            yy_2_1_1_1 = yyv_N;
            yy_2_1_1_2 = yyv_Old;
            if ((long)yy_2_1_1_1 <= (long)yy_2_1_1_2) goto yyfl_84_1_2_1;
            yy_2_1_2 = yyv_N;
            yyglov_TABLEUPB2 = yy_2_1_2;
            goto yysl_84_1_2;
yyfl_84_1_2_1 : ;
            goto yysl_84_1_2;
yysl_84_1_2 : ;
            yyb = yysb;
        }
        return;
    }
}
