/* A Bison parser, made by GNU Bison 1.875b.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#define NULL 0

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     Ident = 257,
     Char = 258,
     Number = 259,
     CodeBlock = 260,
     yytk_PERCENT_g_BLANK_e_BLANK_n_BLANK_t_BLANK_l_BLANK_e_BLANK = 261,
     yytk_PERCENT_z_BLANK_e_BLANK_r_BLANK_o_BLANK = 262,
     yytk_PERCENT_o_BLANK_u_BLANK_t_BLANK = 263,
     yytk_PERCENT_i_BLANK_n_BLANK = 264,
     yytk_GREATER = 265,
     yytk_LESS = 266,
     yytk_PERCENT_c_BLANK_o_BLANK_n_BLANK_f_BLANK_i_BLANK_l_BLANK_t_BLANK_e_BLANK_r_BLANK = 267,
     yytk_PERCENT_l_BLANK_o_BLANK_n_BLANK_g_BLANK = 268,
     yytk_PERCENT_s_BLANK_h_BLANK_o_BLANK_r_BLANK_t_BLANK = 269,
     yytk_PERCENT_t_BLANK_a_BLANK_i_BLANK_l_BLANK = 270,
     yytk_RPAREN = 271,
     yytk_RPAREN_ASTERISK = 272,
     yytk_RPAREN_QUESTIONM = 273,
     yytk_LPAREN = 274,
     yytk_PERCENT_d_BLANK_i_BLANK_s_BLANK_f_BLANK_i_BLANK_l_BLANK_t_BLANK_e_BLANK_r_BLANK = 275,
     yytk_PERCENT_p_BLANK_r_BLANK_i_BLANK_o_BLANK = 276,
     yytk_BAR = 277,
     yytk_COLON = 278,
     yytk_PERCENT_n_BLANK_o_BLANK_d_BLANK_e_BLANK_f_BLANK_a_BLANK_u_BLANK_l_BLANK_t_BLANK = 279,
     yytk_PERCENT_d_BLANK_e_BLANK_f_BLANK_a_BLANK_u_BLANK_l_BLANK_t_BLANK = 280,
     yytk_COMMA = 281,
     yytk_SEMICOLON = 282,
     yytk_PERCENT_t_BLANK_o_BLANK_k_BLANK_e_BLANK_n_BLANK = 283,
     yytk_PERCENT_p_BLANK_r_BLANK_e_BLANK_l_BLANK_u_BLANK_d_BLANK_e_BLANK = 284
   };
#endif
#define Ident 257
#define Char 258
#define Number 259
#define CodeBlock 260
#define yytk_PERCENT_g_BLANK_e_BLANK_n_BLANK_t_BLANK_l_BLANK_e_BLANK 261
#define yytk_PERCENT_z_BLANK_e_BLANK_r_BLANK_o_BLANK 262
#define yytk_PERCENT_o_BLANK_u_BLANK_t_BLANK 263
#define yytk_PERCENT_i_BLANK_n_BLANK 264
#define yytk_GREATER 265
#define yytk_LESS 266
#define yytk_PERCENT_c_BLANK_o_BLANK_n_BLANK_f_BLANK_i_BLANK_l_BLANK_t_BLANK_e_BLANK_r_BLANK 267
#define yytk_PERCENT_l_BLANK_o_BLANK_n_BLANK_g_BLANK 268
#define yytk_PERCENT_s_BLANK_h_BLANK_o_BLANK_r_BLANK_t_BLANK 269
#define yytk_PERCENT_t_BLANK_a_BLANK_i_BLANK_l_BLANK 270
#define yytk_RPAREN 271
#define yytk_RPAREN_ASTERISK 272
#define yytk_RPAREN_QUESTIONM 273
#define yytk_LPAREN 274
#define yytk_PERCENT_d_BLANK_i_BLANK_s_BLANK_f_BLANK_i_BLANK_l_BLANK_t_BLANK_e_BLANK_r_BLANK 275
#define yytk_PERCENT_p_BLANK_r_BLANK_i_BLANK_o_BLANK 276
#define yytk_BAR 277
#define yytk_COLON 278
#define yytk_PERCENT_n_BLANK_o_BLANK_d_BLANK_e_BLANK_f_BLANK_a_BLANK_u_BLANK_l_BLANK_t_BLANK 279
#define yytk_PERCENT_d_BLANK_e_BLANK_f_BLANK_a_BLANK_u_BLANK_l_BLANK_t_BLANK 280
#define yytk_COMMA 281
#define yytk_SEMICOLON 282
#define yytk_PERCENT_t_BLANK_o_BLANK_k_BLANK_e_BLANK_n_BLANK 283
#define yytk_PERCENT_p_BLANK_r_BLANK_e_BLANK_l_BLANK_u_BLANK_d_BLANK_e_BLANK 284




/* Copy the first part of user declarations.  */
//#line 1 "gen.y"

typedef long * yy;
#define yyu (-2147483647L)
static yy yynull;
extern yy yyh;
extern yy yyhx;
static yyErr(n,l)
{
yyAbort(n,"bnf", l);
}
/* start */
/* end */
extern yy yyglov_LHS_List;
extern yy yyglov_DefaultSpec;
extern yy yyglov_PreludeBlock;
extern yy yyglov_UniqueRuleNumber;
extern yy yyglov_UniqueAlternativeNumber;
extern yy yyglov_UniqueSubphraseNumber;
extern yy yyglov_AlternativeIndex;
extern yy yyglov_NontermList;
extern yy yyglov_UniqueNontermNumber;
extern yy yyglov_TokenList;
extern yy yyglov_TokenCount;
extern yy yyglov_NextTokenNumber;
extern yy yyglov_GentleFlag;
typedef struct {long attr[2];} yyATTRIBUTES;
#define YYSTYPE yyATTRIBUTES
extern YYSTYPE yylval;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
//#line 178 "gen.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   74

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  35
/* YYNRULES -- Number of rules. */
#define YYNRULES  67
/* YYNRULES -- Number of states. */
#define YYNSTATES  99

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,     2
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     9,    12,    13,    14,    18,    19,    23,
      25,    27,    30,    32,    35,    37,    39,    47,    48,    52,
      54,    57,    60,    61,    64,    67,    68,    71,    72,    76,
      78,    83,    89,    94,    96,    99,   100,   102,   104,   107,
     108,   112,   116,   120,   125,   126,   129,   132,   136,   138,
     141,   143,   147,   148,   152,   154,   156,   159,   162,   163,
     167,   169,   173,   177,   179,   182,   184,   186
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      32,     0,    -1,    34,    65,    33,    35,    38,    -1,    30,
      64,    -1,    -1,    -1,    29,    36,    28,    -1,    -1,    37,
      27,    36,    -1,    37,    -1,     3,    -1,    40,    38,    -1,
      40,    -1,    39,    38,    -1,    26,    -1,    25,    -1,     3,
      50,    24,    41,    44,    42,    28,    -1,    -1,    43,    23,
      42,    -1,    43,    -1,    46,    45,    -1,    30,    64,    -1,
      -1,    22,     5,    -1,    21,     5,    -1,    -1,    47,    46,
      -1,    -1,    49,     3,    55,    -1,     4,    -1,    49,    20,
      58,    19,    -1,    49,    20,    58,    48,    18,    -1,    49,
      20,    59,    17,    -1,    64,    -1,    16,    49,    -1,    -1,
      15,    -1,    14,    -1,    13,     5,    -1,    -1,    12,    53,
      11,    -1,    12,    52,    11,    -1,    12,    51,    11,    -1,
      12,    51,    52,    11,    -1,    -1,    10,    53,    -1,     9,
      53,    -1,    54,    27,    53,    -1,    54,    -1,     3,     3,
      -1,     3,    -1,    12,    56,    11,    -1,    -1,    57,    27,
      56,    -1,    57,    -1,     3,    -1,    60,    61,    -1,    60,
      62,    -1,    -1,    63,    23,    61,    -1,    63,    -1,    63,
       8,    45,    -1,    63,    23,    62,    -1,    63,    -1,    46,
      45,    -1,     6,    -1,     7,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,    62,    62,    81,    98,   111,   142,   153,   159,   169,
     177,   277,   306,   331,   347,   361,   375,   435,   444,   474,
     499,   553,   570,   582,   605,   635,   682,   712,   724,   768,
     789,   844,   907,   962,   983,  1000,  1038,  1052,  1066,  1089,
    1127,  1144,  1161,  1184,  1221,  1233,  1249,  1265,  1295,  1320,
    1360,  1401,  1419,  1438,  1475,  1510,  1538,  1561,  1585,  1602,
    1632,  1708,  1749,  1779,  1855,  1910,  1938,  1953
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "Ident", "Char", "Number", "CodeBlock", 
  "yytk_PERCENT_g_BLANK_e_BLANK_n_BLANK_t_BLANK_l_BLANK_e_BLANK", 
  "yytk_PERCENT_z_BLANK_e_BLANK_r_BLANK_o_BLANK", 
  "yytk_PERCENT_o_BLANK_u_BLANK_t_BLANK", "yytk_PERCENT_i_BLANK_n_BLANK", 
  "yytk_GREATER", "yytk_LESS", 
  "yytk_PERCENT_c_BLANK_o_BLANK_n_BLANK_f_BLANK_i_BLANK_l_BLANK_t_BLANK_e_BLANK_r_BLANK", 
  "yytk_PERCENT_l_BLANK_o_BLANK_n_BLANK_g_BLANK", 
  "yytk_PERCENT_s_BLANK_h_BLANK_o_BLANK_r_BLANK_t_BLANK", 
  "yytk_PERCENT_t_BLANK_a_BLANK_i_BLANK_l_BLANK", "yytk_RPAREN", 
  "yytk_RPAREN_ASTERISK", "yytk_RPAREN_QUESTIONM", "yytk_LPAREN", 
  "yytk_PERCENT_d_BLANK_i_BLANK_s_BLANK_f_BLANK_i_BLANK_l_BLANK_t_BLANK_e_BLANK_r_BLANK", 
  "yytk_PERCENT_p_BLANK_r_BLANK_i_BLANK_o_BLANK", "yytk_BAR", 
  "yytk_COLON", 
  "yytk_PERCENT_n_BLANK_o_BLANK_d_BLANK_e_BLANK_f_BLANK_a_BLANK_u_BLANK_l_BLANK_t_BLANK", 
  "yytk_PERCENT_d_BLANK_e_BLANK_f_BLANK_a_BLANK_u_BLANK_l_BLANK_t_BLANK", 
  "yytk_COMMA", "yytk_SEMICOLON", 
  "yytk_PERCENT_t_BLANK_o_BLANK_k_BLANK_e_BLANK_n_BLANK", 
  "yytk_PERCENT_p_BLANK_r_BLANK_e_BLANK_l_BLANK_u_BLANK_d_BLANK_e_BLANK", 
  "$accept", "ROOT_", "PreludeOption", "init", "token_decl_part", 
  "token_decl_list", "token_decl", "rulelist", "defaultspec", "rule", 
  "FirstAlternativeIndex", "rhslist", "rhs", "RulePrelude", 
  "RuleAnnotation", "memberlist", "member", "LoopAnno", 
  "MemberAnnotation", "FormalParameters", "FormalInParameters", 
  "FormalOutParameters", "FormalParameterList", "FormalParameter", 
  "ActualParameters", "ActualParameterList", "ActualParameter", 
  "optloop_alternativelist0", "alternativelist0", "SetAlternativeIndex", 
  "optloop_alternativelist", "alternativelist", "alternative", 
  "ActionBlock", "GentleOption", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   285,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    31,    32,    33,    33,    34,    35,    35,    36,    36,
      37,    38,    38,    38,    39,    39,    40,    41,    42,    42,
      43,    44,    44,    45,    45,    45,    46,    46,    47,    47,
      47,    47,    47,    47,    48,    48,    49,    49,    49,    49,
      50,    50,    50,    50,    50,    51,    52,    53,    53,    54,
      54,    55,    55,    56,    56,    57,    58,    59,    60,    61,
      61,    61,    62,    62,    63,    64,    65,    65
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     5,     2,     0,     0,     3,     0,     3,     1,
       1,     2,     1,     2,     1,     1,     7,     0,     3,     1,
       2,     2,     0,     2,     2,     0,     2,     0,     3,     1,
       4,     5,     4,     1,     2,     0,     1,     1,     2,     0,
       3,     3,     3,     4,     0,     2,     2,     3,     1,     2,
       1,     3,     0,     3,     1,     1,     2,     2,     0,     3,
       1,     3,     3,     1,     2,     1,     1,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       5,     0,    67,     1,    66,     4,     0,     7,    65,     3,
       0,     0,    10,     0,     9,    44,    15,    14,     2,     0,
      12,     6,     0,     0,     0,    13,    11,     8,    50,     0,
       0,     0,     0,     0,    48,    17,    49,    46,    45,    42,
       0,    41,    40,     0,    22,    43,    47,     0,    27,    21,
      29,     0,    37,    36,     0,    19,    25,    27,     0,    33,
      38,    16,    27,     0,     0,    20,    26,    52,    58,    18,
      24,    23,     0,    28,    35,     0,    27,    55,     0,    54,
      39,    30,     0,    32,    25,    56,    57,    60,    51,     0,
      34,    31,    64,    25,    27,    53,    61,    59,    62
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     1,     7,     2,    11,    13,    14,    18,    19,    20,
      44,    54,    55,    48,    65,    56,    57,    82,    58,    24,
      31,    32,    33,    34,    73,    78,    79,    74,    75,    76,
      85,    86,    87,    59,     5
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -75
static const yysigned_char yypact[] =
{
     -75,    17,    24,   -75,   -75,    12,    31,    14,   -75,   -75,
      41,    -1,   -75,    18,    22,    33,   -75,   -75,   -75,    -1,
      -1,   -75,    41,    23,    26,   -75,   -75,   -75,    44,    45,
      45,     2,    40,    42,    25,   -75,   -75,   -75,   -75,   -75,
      43,   -75,   -75,    45,    27,   -75,   -75,    31,     1,   -75,
     -75,    50,   -75,   -75,    28,    35,     7,     1,     3,   -75,
     -75,   -75,     1,    54,    55,   -75,   -75,    49,   -75,   -75,
     -75,   -75,    59,   -75,    11,    46,     1,   -75,    53,    38,
      21,   -75,    48,   -75,     7,   -75,   -75,    -5,   -75,    59,
     -75,   -75,   -75,     7,     1,   -75,   -75,   -75,   -75
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -75,   -75,   -75,   -75,   -75,    47,   -75,    20,   -75,   -75,
     -75,     5,   -75,   -75,   -74,   -56,   -75,   -75,   -12,   -75,
     -75,    39,   -21,   -75,   -75,   -18,   -75,   -75,   -75,   -75,
     -22,   -20,   -75,    -6,   -75
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -64
static const yysigned_char yytable[] =
{
       9,    66,    15,    93,   -39,    50,    67,     8,    37,    38,
      92,    29,   -63,    39,    51,    52,    53,     3,    94,    96,
      84,   -39,    46,    68,    16,    17,    28,    80,    63,    64,
      81,     4,    29,    30,    51,    52,    53,     8,    84,    25,
      26,    49,     6,    10,    12,    23,    21,    36,    28,    22,
      35,    41,    43,    42,    45,    60,    61,    47,    62,    70,
      71,    72,    77,    83,    88,    89,    91,    69,    90,    27,
      40,    95,    97,     0,    98
};

static const yysigned_char yycheck[] =
{
       6,    57,     3,     8,     3,     4,     3,     6,    29,    30,
      84,     9,    17,    11,    13,    14,    15,     0,    23,    93,
      76,    20,    43,    20,    25,    26,     3,    16,    21,    22,
      19,     7,     9,    10,    13,    14,    15,     6,    94,    19,
      20,    47,    30,    29,     3,    12,    28,     3,     3,    27,
      24,    11,    27,    11,    11,     5,    28,    30,    23,     5,
       5,    12,     3,    17,    11,    27,    18,    62,    80,    22,
      31,    89,    94,    -1,    94
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    32,    34,     0,     7,    65,    30,    33,     6,    64,
      29,    35,     3,    36,    37,     3,    25,    26,    38,    39,
      40,    28,    27,    12,    50,    38,    38,    36,     3,     9,
      10,    51,    52,    53,    54,    24,     3,    53,    53,    11,
      52,    11,    11,    27,    41,    11,    53,    30,    44,    64,
       4,    13,    14,    15,    42,    43,    46,    47,    49,    64,
       5,    28,    23,    21,    22,    45,    46,     3,    20,    42,
       5,     5,    12,    55,    58,    59,    60,     3,    56,    57,
      16,    19,    48,    17,    46,    61,    62,    63,    11,    27,
      49,    18,    45,     8,    23,    56,    45,    61,    62
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
//#line 68 "gen.y"
    {
yy yyv_L;
yy yy_5_1;
yy yy_6_1;
yy_5_1 = (yy)(yyvsp[0].attr[1]);
yyv_L = yy_5_1;
yy_6_1 = yyv_L;
Process(yy_6_1);
yyval.attr[0] = yyvsp[-4].attr[0];
;}
    break;

  case 3:
//#line 84 "gen.y"
    {
yy yyv_Block;
yy yy_2_1;
yy yy_3;
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyv_Block = yy_2_1;
yy_3 = yyv_Block;
yyglov_PreludeBlock = yy_3;
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 4:
//#line 98 "gen.y"
    {
yy yyb;
yy yy_1;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_1 = yyb + 0;
yy_1[0] = 2;
yyglov_PreludeBlock = yy_1;
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 5:
//#line 111 "gen.y"
    {
yy yyb;
yy yy_3;
yy yy_5;
yy yy_6;
yy yy_7;
yy yy_8;
yy yy_9;
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
init_tokenlist();
init_lexinfo();
yy_3 = yyb + 0;
yy_3[0] = 2;
yyglov_NontermList = yy_3;
INIT_ENCODE();
yy_5 = ((yy)0);
yyglov_UniqueNontermNumber = yy_5;
yy_6 = ((yy)0);
yyglov_UniqueRuleNumber = yy_6;
yy_7 = ((yy)0);
yyglov_UniqueSubphraseNumber = yy_7;
yy_8 = ((yy)0);
yyglov_UniqueAlternativeNumber = yy_8;
yy_9 = yyb + 1;
yy_9[0] = 1;
yyglov_DefaultSpec = yy_9;
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 6:
//#line 146 "gen.y"
    {
yyval.attr[0] = yyvsp[-2].attr[0];
;}
    break;

  case 7:
//#line 153 "gen.y"
    {
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 8:
//#line 163 "gen.y"
    {
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 9:
//#line 171 "gen.y"
    {
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 10:
//#line 179 "gen.y"
    {
yy yyb;
yy yyv_I;
yy yy_1_1;
yy yyv_P;
yy yy_2;
yy yyv_N;
yy yy_3_1;
yy yy_4_1;
yy yy_5_1;
yy yyv_Yystype;
yy yy_5_2;
yy yy_6_1;
yy yyv_Dummy;
yy yy_6_2;
yy yy_7_1;
yy yy_7_1_1;
yy yy_7_1_1_1;
yy yy_7_1_1_2;
yy yy_7_1_1_3;
yy yy_7_1_1_4;
yy yy_7_1_2;
yy yyv_L;
yy yy_7_2;
yy yy_8_1;
yy yy_8_2;
yy yy_8_3;
yy yy_8_4;
yy yy_9_1;
yy yy_9_2;
yy yyv_K;
yy yy_10;
yy yy_11_1_1_1;
yy yy_11_1_1_2;
yy yy_11_1_2;
yy_1_1 = (yy)(yyvsp[0].attr[1]);
yy_2 = (yy)(yyvsp[0].attr[0]);
yyb = yyh;
yyh += 10; if (yyh > yyhx) yyExtend();
yyv_I = yy_1_1;
yyv_P = yy_2;
FreshTokenNumber(&yy_3_1);
yyv_N = yy_3_1;
yy_4_1 = yyv_N;
TableUpb2(yy_4_1);
yy_5_1 = ((yy)"YYSTYPE");
string_to_id(yy_5_1, &yy_5_2);
yyv_Yystype = yy_5_2;
yy_6_1 = ((yy)"Dummy");
string_to_id(yy_6_1, &yy_6_2);
yyv_Dummy = yy_6_2;
yy_7_1_1_1 = yyb + 8;
yy_7_1_1_1[0] = 2;
yy_7_1_1_2 = yyv_Yystype;
yy_7_1_1_3 = yyv_Dummy;
yy_7_1_1_4 = yyv_P;
yy_7_1_1 = yyb + 3;
yy_7_1_1[0] = 1;
yy_7_1_1[1] = ((long)yy_7_1_1_1);
yy_7_1_1[2] = ((long)yy_7_1_1_2);
yy_7_1_1[3] = ((long)yy_7_1_1_3);
yy_7_1_1[4] = ((long)yy_7_1_1_4);
yy_7_1_2 = yyb + 9;
yy_7_1_2[0] = 2;
yy_7_1 = yyb + 0;
yy_7_1[0] = 1;
yy_7_1[1] = ((long)yy_7_1_1);
yy_7_1[2] = ((long)yy_7_1_2);
yy_7_2 = yy_7_1;
yyv_L = yy_7_2;
yy_8_1 = yyv_I;
yy_8_2 = yyv_L;
yy_8_3 = yyv_N;
yy_8_4 = yyv_P;
define_token(yy_8_1, yy_8_2, yy_8_3, yy_8_4);
yy_9_1 = yyv_I;
yy_9_2 = yyv_N;
enter_token(yy_9_1, yy_9_2);
yy_10 = yyglov_TokenCount;
if (yy_10 == (yy) yyu) yyErr(1,313);
yyv_K = yy_10;
{
yy yysb = yyb;
yy_11_1_1_1 = yyv_N;
yy_11_1_1_2 = yyv_K;
if ((long)yy_11_1_1_1 <= (long)yy_11_1_1_2) goto yyfl_40_1_11_1;
yy_11_1_2 = yyv_N;
yyglov_TokenCount = yy_11_1_2;
goto yysl_40_1_11;
yyfl_40_1_11_1 : ;
goto yysl_40_1_11;
yysl_40_1_11 : ;
yyb = yysb;
}
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 11:
//#line 280 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_H;
yy yy_1_1;
yy yyv_T;
yy yy_2_1;
yy_1_1 = (yy)(yyvsp[-1].attr[1]);
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yyv_T = yy_2_1;
yy_0_1_1 = yyv_H;
yy_0_1_2 = yyv_T;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 12:
//#line 308 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_H;
yy yy_1_1;
yy_1_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 4; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yy_0_1_1 = yyv_H;
yy_0_1_2 = yyb + 3;
yy_0_1_2[0] = 2;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 13:
//#line 334 "gen.y"
    {
yy yy_0_1;
yy yyv_L;
yy yy_2_1;
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyv_L = yy_2_1;
yy_0_1 = yyv_L;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 14:
//#line 349 "gen.y"
    {
yy yyb;
yy yy_2;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_2 = yyb + 0;
yy_2[0] = 1;
yyglov_DefaultSpec = yy_2;
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 15:
//#line 363 "gen.y"
    {
yy yyb;
yy yy_2;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_2 = yyb + 0;
yy_2[0] = 2;
yyglov_DefaultSpec = yy_2;
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 16:
//#line 383 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yy_0_1_4;
yy yyv_I;
yy yy_1_1;
yy yyv_L;
yy yy_2_1;
yy yyv_Pos;
yy yy_5;
yy yyv_Prelude;
yy yy_6_1;
yy yyv_RhsList;
yy yy_7_1;
yy yy_9_1;
yy yy_9_2;
yy yy_9_3;
yy_1_1 = (yy)(yyvsp[-6].attr[1]);
yy_2_1 = (yy)(yyvsp[-5].attr[1]);
yy_5 = (yy)(yyvsp[-3].attr[0]);
yy_6_1 = (yy)(yyvsp[-2].attr[1]);
yy_7_1 = (yy)(yyvsp[-1].attr[1]);
yyb = yyh;
yyh += 5; if (yyh > yyhx) yyExtend();
yyv_I = yy_1_1;
yyv_L = yy_2_1;
yyv_Pos = yy_5;
yyv_Prelude = yy_6_1;
yyv_RhsList = yy_7_1;
yy_9_1 = yyv_I;
yy_9_2 = yyv_L;
yy_9_3 = yyv_Pos;
define_nonterm(yy_9_1, yy_9_2, yy_9_3);
yy_0_1_1 = yyv_I;
yy_0_1_2 = yyv_RhsList;
yy_0_1_3 = yyv_Prelude;
yy_0_1_4 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 2;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yy_0_1[3] = ((long)yy_0_1_3);
yy_0_1[4] = ((long)yy_0_1_4);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-4].attr[0];
;}
    break;

  case 17:
//#line 435 "gen.y"
    {
yy yy_1;
yy_1 = ((yy)0);
yyglov_AlternativeIndex = yy_1;
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 18:
//#line 448 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_Hd;
yy yy_1_1;
yy yyv_Tl;
yy yy_3_1;
yy_1_1 = (yy)(yyvsp[-2].attr[1]);
yy_3_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yyv_Hd = yy_1_1;
yyv_Tl = yy_3_1;
yy_0_1_1 = yyv_Hd;
yy_0_1_2 = yyv_Tl;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 19:
//#line 476 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_Hd;
yy yy_1_1;
yy_1_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 4; if (yyh > yyhx) yyExtend();
yyv_Hd = yy_1_1;
yy_0_1_1 = yyv_Hd;
yy_0_1_2 = yyb + 3;
yy_0_1_2[0] = 2;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 20:
//#line 502 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yy_0_1_4;
yy yyv_Pos;
yy yy_1;
yy yyv_R;
yy yy_2_1;
yy yyv_Annotation;
yy yy_3_1;
yy yyv_Key;
yy yyv_N;
yy yy_5_1;
yy yy_6;
yy yy_7;
yy_1 = (yy)(yyvsp[-1].attr[0]);
yy_2_1 = (yy)(yyvsp[-1].attr[1]);
yy_3_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 9; if (yyh > yyhx) yyExtend();
yyv_Pos = yy_1;
yyv_R = yy_2_1;
yyv_Annotation = yy_3_1;
yyv_Key = yyb + 5;
yyb[8] = yyu;
yyb[7] = yyu;
yyb[6] = yyu;
FreshRuleNumber(&yy_5_1);
yyv_N = yy_5_1;
yy_6 = yyv_N;
yyv_Key[2] = (long) yy_6;
yy_7 = yyv_R;
yyv_Key[1] = (long) yy_7;
yy_0_1_1 = yyv_R;
yy_0_1_2 = yyv_Key;
yy_0_1_3 = yyv_Annotation;
yy_0_1_4 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yy_0_1[3] = ((long)yy_0_1_3);
yy_0_1[4] = ((long)yy_0_1_4);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 21:
//#line 556 "gen.y"
    {
yy yy_0_1;
yy yyv_Block;
yy yy_2_1;
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyv_Block = yy_2_1;
yy_0_1 = yyv_Block;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 22:
//#line 570 "gen.y"
    {
yy yyb;
yy yy_0_1;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_0_1 = yyb + 0;
yy_0_1[0] = 2;
yyval.attr[1] = ((long)yy_0_1);
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 23:
//#line 585 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yyv_N;
yy yy_2_1;
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
yyv_N = yy_2_1;
NextAlternativeIndex();
yy_0_1_1 = yyv_N;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 24:
//#line 608 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_1_1;
yy yy_0_1_1_1_1;
yy yy_0_1_1_1_2;
yy yyv_N;
yy yy_2_1;
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
yyv_N = yy_2_1;
NextAlternativeIndex();
yy_0_1_1_1_1 = yyv_N;
yy_0_1_1_1_2 = ((yy)1);
yy_0_1_1_1 = (yy)(((long)yy_0_1_1_1_1)+((long)yy_0_1_1_1_2));
yy_0_1_1 = (yy)(-((long)yy_0_1_1_1));
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 25:
//#line 635 "gen.y"
    {
yy yyb = NULL;
yy yy_0_1 = NULL;
yy yy_2_1_1 = NULL;
yy yy_2_1_2_1 = NULL;
yy yyv_Anno = NULL;
yy yy_2_1_2_2 = NULL;
yy yyv_N = NULL;
yy yy_2_2_1 = NULL;
yy yy_2_2_2_1 = NULL;
yy yy_2_2_2_1_1 = NULL;
yy yy_2_2_2_2 = NULL;
NextAlternativeIndex();
{
yy yysb = yyb;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_2_1_1 = yyglov_DefaultSpec;
if (yy_2_1_1 == (yy) yyu) yyErr(1,386);
if (yy_2_1_1[0] != 2) goto yyfl_49_3_2_1;
yy_2_1_2_1 = yyb + 0;
yy_2_1_2_1[0] = 2;
yy_2_1_2_2 = yy_2_1_2_1;
yyv_Anno = yy_2_1_2_2;
goto yysl_49_3_2;
yyfl_49_3_2_1 : ;
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
yy_2_2_1 = yyglov_AlternativeIndex;
if (yy_2_2_1 == (yy) yyu) yyErr(1,389);
yyv_N = yy_2_2_1;
yy_2_2_2_1_1 = yyv_N;
yy_2_2_2_1 = yyb + 0;
yy_2_2_2_1[0] = 1;
yy_2_2_2_1[1] = ((long)yy_2_2_2_1_1);
yy_2_2_2_2 = yy_2_2_2_1;
yyv_Anno = yy_2_2_2_2;
goto yysl_49_3_2;
yysl_49_3_2 : ;
yyb = yysb;
}
yy_0_1 = yyv_Anno;
yyval.attr[1] = ((long)yy_0_1);
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 26:
//#line 685 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_H;
yy yy_1_1;
yy yyv_T;
yy yy_2_1;
yy_1_1 = (yy)(yyvsp[-1].attr[1]);
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yyv_T = yy_2_1;
yy_0_1_1 = yyv_H;
yy_0_1_2 = yyv_T;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 27:
//#line 712 "gen.y"
    {
yy yyb;
yy yy_0_1;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_0_1 = yyb + 0;
yy_0_1[0] = 2;
yyval.attr[1] = ((long)yy_0_1);
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 28:
//#line 728 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yy_0_1_4;
yy yyv_Annotation;
yy yy_1_1;
yy yyv_I;
yy yy_2_1;
yy yyv_Pos;
yy yy_3;
yy yyv_L;
yy yy_4_1;
yy_1_1 = (yy)(yyvsp[-2].attr[1]);
yy_2_1 = (yy)(yyvsp[-1].attr[1]);
yy_3 = (yy)(yyvsp[-1].attr[0]);
yy_4_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 5; if (yyh > yyhx) yyExtend();
yyv_Annotation = yy_1_1;
yyv_I = yy_2_1;
yyv_Pos = yy_3;
yyv_L = yy_4_1;
yy_0_1_1 = yyv_I;
yy_0_1_2 = yyv_L;
yy_0_1_3 = yyv_Annotation;
yy_0_1_4 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yy_0_1[3] = ((long)yy_0_1_3);
yy_0_1[4] = ((long)yy_0_1_4);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-2].attr[0];
;}
    break;

  case 29:
//#line 770 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yyv_I;
yy yy_1_1;
yy_1_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
yyv_I = yy_1_1;
yy_0_1_1 = yyv_I;
yy_0_1 = yyb + 0;
yy_0_1[0] = 2;
yy_0_1[1] = ((long)yy_0_1_1);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 30:
//#line 794 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yy_0_1_4;
yy yy_0_1_5;
yy yyv_Annotation;
yy yy_1_1;
yy yyv_Pos;
yy yy_3;
yy yyv_L;
yy yy_4_1;
yy yyv_Key;
yy yyv_N;
yy yy_7_1;
yy yy_8;
yy_1_1 = (yy)(yyvsp[-3].attr[1]);
yy_3 = (yy)(yyvsp[-2].attr[0]);
yy_4_1 = (yy)(yyvsp[-1].attr[1]);
yyb = yyh;
yyh += 9; if (yyh > yyhx) yyExtend();
yyv_Annotation = yy_1_1;
yyv_Pos = yy_3;
yyv_L = yy_4_1;
yyv_Key = yyb + 7;
yyb[8] = yyu;
FreshSubphraseNumber(&yy_7_1);
yyv_N = yy_7_1;
yy_8 = yyv_N;
yyv_Key[1] = (long) yy_8;
yy_0_1_1 = yyb + 6;
yy_0_1_1[0] = 1;
yy_0_1_2 = yyv_L;
yy_0_1_3 = yyv_Key;
yy_0_1_4 = yyv_Annotation;
yy_0_1_5 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 3;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yy_0_1[3] = ((long)yy_0_1_3);
yy_0_1[4] = ((long)yy_0_1_4);
yy_0_1[5] = ((long)yy_0_1_5);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-2].attr[0];
;}
    break;

  case 31:
//#line 850 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yy_0_1_4;
yy yy_0_1_5;
yy yyv_Annotation;
yy yy_1_1;
yy yyv_Pos;
yy yy_3;
yy yyv_L;
yy yy_4_1;
yy yyv_MemberAnno;
yy yy_5_1;
yy yyv_Key;
yy yyv_N;
yy yy_8_1;
yy yy_9;
yy_1_1 = (yy)(yyvsp[-4].attr[1]);
yy_3 = (yy)(yyvsp[-3].attr[0]);
yy_4_1 = (yy)(yyvsp[-2].attr[1]);
yy_5_1 = (yy)(yyvsp[-1].attr[1]);
yyb = yyh;
yyh += 10; if (yyh > yyhx) yyExtend();
yyv_Annotation = yy_1_1;
yyv_Pos = yy_3;
yyv_L = yy_4_1;
yyv_MemberAnno = yy_5_1;
yyv_Key = yyb + 8;
yyb[9] = yyu;
FreshSubphraseNumber(&yy_8_1);
yyv_N = yy_8_1;
yy_9 = yyv_N;
yyv_Key[1] = (long) yy_9;
yy_0_1_1_1 = yyv_MemberAnno;
yy_0_1_1 = yyb + 6;
yy_0_1_1[0] = 2;
yy_0_1_1[1] = ((long)yy_0_1_1_1);
yy_0_1_2 = yyv_L;
yy_0_1_3 = yyv_Key;
yy_0_1_4 = yyv_Annotation;
yy_0_1_5 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 3;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yy_0_1[3] = ((long)yy_0_1_3);
yy_0_1[4] = ((long)yy_0_1_4);
yy_0_1[5] = ((long)yy_0_1_5);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-3].attr[0];
;}
    break;

  case 32:
//#line 912 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yy_0_1_4;
yy yy_0_1_5;
yy yyv_Annotation;
yy yy_1_1;
yy yyv_Pos;
yy yy_3;
yy yyv_L;
yy yy_4_1;
yy yyv_Key;
yy yyv_N;
yy yy_7_1;
yy yy_8;
yy_1_1 = (yy)(yyvsp[-3].attr[1]);
yy_3 = (yy)(yyvsp[-2].attr[0]);
yy_4_1 = (yy)(yyvsp[-1].attr[1]);
yyb = yyh;
yyh += 9; if (yyh > yyhx) yyExtend();
yyv_Annotation = yy_1_1;
yyv_Pos = yy_3;
yyv_L = yy_4_1;
yyv_Key = yyb + 7;
yyb[8] = yyu;
FreshSubphraseNumber(&yy_7_1);
yyv_N = yy_7_1;
yy_8 = yyv_N;
yyv_Key[1] = (long) yy_8;
yy_0_1_1 = yyb + 6;
yy_0_1_1[0] = 3;
yy_0_1_2 = yyv_L;
yy_0_1_3 = yyv_Key;
yy_0_1_4 = yyv_Annotation;
yy_0_1_5 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 3;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yy_0_1[3] = ((long)yy_0_1_3);
yy_0_1[4] = ((long)yy_0_1_4);
yy_0_1[5] = ((long)yy_0_1_5);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-2].attr[0];
;}
    break;

  case 33:
//#line 964 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yyv_Block;
yy yy_1_1;
yy_1_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
yyv_Block = yy_1_1;
yy_0_1_1 = yyv_Block;
yy_0_1 = yyb + 0;
yy_0_1[0] = 4;
yy_0_1[1] = ((long)yy_0_1_1);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 34:
//#line 986 "gen.y"
    {
yy yy_0_1;
yy yyv_Anno;
yy yy_2_1;
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyv_Anno = yy_2_1;
yy_0_1 = yyv_Anno;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 35:
//#line 1000 "gen.y"
    {
yy yyb = NULL;
yy yy_0_1 = NULL;
yy yy_1_1_1 = NULL;
yy yy_1_1_2_1 = NULL;
yy yyv_Anno = NULL;
yy yy_1_1_2_2 = NULL;
yy yy_1_2_1_1 = NULL;
yy yy_1_2_1_2 = NULL;
{
yy yysb = yyb;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_1_1_1 = yyglov_DefaultSpec;
if (yy_1_1_1 == (yy) yyu) yyErr(1,441);
if (yy_1_1_1[0] != 2) goto yyfl_53_2_1_1;
yy_1_1_2_1 = yyb + 0;
yy_1_1_2_1[0] = 3;
yy_1_1_2_2 = yy_1_1_2_1;
yyv_Anno = yy_1_1_2_2;
goto yysl_53_2_1;
yyfl_53_2_1_1 : ;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_1_2_1_1 = yyb + 0;
yy_1_2_1_1[0] = 1;
yy_1_2_1_2 = yy_1_2_1_1;
yyv_Anno = yy_1_2_1_2;
goto yysl_53_2_1;
yysl_53_2_1 : ;
yyb = yysb;
}
yy_0_1 = yyv_Anno;
yyval.attr[1] = ((long)yy_0_1);
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 36:
//#line 1040 "gen.y"
    {
yy yyb;
yy yy_0_1;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 37:
//#line 1054 "gen.y"
    {
yy yyb;
yy yy_0_1;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_0_1 = yyb + 0;
yy_0_1[0] = 2;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 38:
//#line 1069 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yyv_N;
yy yy_2_1;
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
yyv_N = yy_2_1;
yy_0_1_1 = yyv_N;
yy_0_1 = yyb + 0;
yy_0_1[0] = 4;
yy_0_1[1] = ((long)yy_0_1_1);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 39:
//#line 1089 "gen.y"
    {
yy yyb = NULL;
yy yy_0_1 = NULL;
yy yy_1_1_1 = NULL;
yy yy_1_1_2_1 = NULL;
yy yyv_Anno = NULL;
yy yy_1_1_2_2 = NULL;
yy yy_1_2_1_1 = NULL;
yy yy_1_2_1_2 = NULL;
{
yy yysb = yyb;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_1_1_1 = yyglov_DefaultSpec;
if (yy_1_1_1 == (yy) yyu) yyErr(1,456);
if (yy_1_1_1[0] != 2) goto yyfl_54_4_1_1;
yy_1_1_2_1 = yyb + 0;
yy_1_1_2_1[0] = 3;
yy_1_1_2_2 = yy_1_1_2_1;
yyv_Anno = yy_1_1_2_2;
goto yysl_54_4_1;
yyfl_54_4_1_1 : ;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_1_2_1_1 = yyb + 0;
yy_1_2_1_1[0] = 1;
yy_1_2_1_2 = yy_1_2_1_1;
yyv_Anno = yy_1_2_1_2;
goto yysl_54_4_1;
yysl_54_4_1 : ;
yyb = yysb;
}
yy_0_1 = yyv_Anno;
yyval.attr[1] = ((long)yy_0_1);
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 40:
//#line 1131 "gen.y"
    {
yy yy_0_1;
yy yyv_L;
yy yy_2_1;
yy_2_1 = (yy)(yyvsp[-1].attr[1]);
yyv_L = yy_2_1;
yy_0_1 = yyv_L;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-2].attr[0];
;}
    break;

  case 41:
//#line 1148 "gen.y"
    {
yy yy_0_1;
yy yyv_L;
yy yy_2_1;
yy_2_1 = (yy)(yyvsp[-1].attr[1]);
yyv_L = yy_2_1;
yy_0_1 = yyv_L;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-2].attr[0];
;}
    break;

  case 42:
//#line 1165 "gen.y"
    {
yy yy_0_1;
yy yyv_IL;
yy yy_2_1;
yy yy_4_1;
yy yyv_L;
yy yy_4_2;
yy_2_1 = (yy)(yyvsp[-1].attr[1]);
yyv_IL = yy_2_1;
yy_4_1 = yyv_IL;
convert_to_in_parameters(yy_4_1, &yy_4_2);
yyv_L = yy_4_2;
yy_0_1 = yyv_L;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-2].attr[0];
;}
    break;

  case 43:
//#line 1189 "gen.y"
    {
yy yy_0_1;
yy yyv_IL0;
yy yy_2_1;
yy yyv_OL;
yy yy_3_1;
yy yy_5_1;
yy yyv_IL;
yy yy_5_2;
yy yy_6_1;
yy yy_6_2;
yy yyv_L;
yy yy_6_3;
yy_2_1 = (yy)(yyvsp[-2].attr[1]);
yy_3_1 = (yy)(yyvsp[-1].attr[1]);
yyv_IL0 = yy_2_1;
yyv_OL = yy_3_1;
yy_5_1 = yyv_IL0;
convert_to_in_parameters(yy_5_1, &yy_5_2);
yyv_IL = yy_5_2;
yy_6_1 = yyv_IL;
yy_6_2 = yyv_OL;
append_parameter_lists(yy_6_1, yy_6_2, &yy_6_3);
yyv_L = yy_6_3;
yy_0_1 = yyv_L;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-3].attr[0];
;}
    break;

  case 44:
//#line 1221 "gen.y"
    {
yy yyb;
yy yy_0_1;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_0_1 = yyb + 0;
yy_0_1[0] = 2;
yyval.attr[1] = ((long)yy_0_1);
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 45:
//#line 1236 "gen.y"
    {
yy yy_0_1;
yy yyv_L;
yy yy_2_1;
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyv_L = yy_2_1;
yy_0_1 = yyv_L;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 46:
//#line 1252 "gen.y"
    {
yy yy_0_1;
yy yyv_L;
yy yy_2_1;
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyv_L = yy_2_1;
yy_0_1 = yyv_L;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 47:
//#line 1269 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_H;
yy yy_1_1;
yy yyv_T;
yy yy_3_1;
yy_1_1 = (yy)(yyvsp[-2].attr[1]);
yy_3_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yyv_T = yy_3_1;
yy_0_1_1 = yyv_H;
yy_0_1_2 = yyv_T;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 48:
//#line 1297 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_H;
yy yy_1_1;
yy_1_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 4; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yy_0_1_1 = yyv_H;
yy_0_1_2 = yyb + 3;
yy_0_1_2[0] = 2;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 49:
//#line 1323 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yy_0_1_4;
yy yyv_Type;
yy yy_1_1;
yy yyv_Name;
yy yy_2_1;
yy yyv_Pos;
yy yy_3;
yy_1_1 = (yy)(yyvsp[-1].attr[1]);
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yy_3 = (yy)(yyvsp[0].attr[0]);
yyb = yyh;
yyh += 6; if (yyh > yyhx) yyExtend();
yyv_Type = yy_1_1;
yyv_Name = yy_2_1;
yyv_Pos = yy_3;
yy_0_1_1 = yyb + 5;
yy_0_1_1[0] = 2;
yy_0_1_2 = yyv_Type;
yy_0_1_3 = yyv_Name;
yy_0_1_4 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yy_0_1[3] = ((long)yy_0_1_3);
yy_0_1[4] = ((long)yy_0_1_4);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 50:
//#line 1362 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yy_0_1_4;
yy yyv_Name;
yy yy_1_1;
yy yyv_Pos;
yy yy_2;
yy yy_3_1;
yy yyv_Type;
yy yy_3_2;
yy_1_1 = (yy)(yyvsp[0].attr[1]);
yy_2 = (yy)(yyvsp[0].attr[0]);
yyb = yyh;
yyh += 6; if (yyh > yyhx) yyExtend();
yyv_Name = yy_1_1;
yyv_Pos = yy_2;
yy_3_1 = ((yy)"YYSTYPE");
string_to_id(yy_3_1, &yy_3_2);
yyv_Type = yy_3_2;
yy_0_1_1 = yyb + 5;
yy_0_1_1[0] = 2;
yy_0_1_2 = yyv_Type;
yy_0_1_3 = yyv_Name;
yy_0_1_4 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yy_0_1[3] = ((long)yy_0_1_3);
yy_0_1[4] = ((long)yy_0_1_4);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 51:
//#line 1405 "gen.y"
    {
yy yy_0_1;
yy yyv_L;
yy yy_2_1;
yy_2_1 = (yy)(yyvsp[-1].attr[1]);
yyv_L = yy_2_1;
yy_0_1 = yyv_L;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-2].attr[0];
;}
    break;

  case 52:
//#line 1419 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yyv_Pos;
yy yy_1;
yyGetPos(&yy_1);
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
yyv_Pos = yy_1;
yy_0_1_1 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 2;
yy_0_1[1] = ((long)yy_0_1_1);
yyval.attr[1] = ((long)yy_0_1);
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 53:
//#line 1442 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yyv_H;
yy yy_1_1;
yy yyv_Pos;
yy yy_2;
yy yyv_T;
yy yy_4_1;
yy_1_1 = (yy)(yyvsp[-2].attr[1]);
yy_2 = (yy)(yyvsp[-2].attr[0]);
yy_4_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 4; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yyv_Pos = yy_2;
yyv_T = yy_4_1;
yy_0_1_1 = yyv_H;
yy_0_1_2 = yyv_T;
yy_0_1_3 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yy_0_1[3] = ((long)yy_0_1_3);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 54:
//#line 1477 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_2_1;
yy yy_0_1_3;
yy yyv_H;
yy yy_1_1;
yy yyv_Pos;
yy yy_2;
yy_1_1 = (yy)(yyvsp[0].attr[1]);
yy_2 = (yy)(yyvsp[0].attr[0]);
yyb = yyh;
yyh += 6; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yyv_Pos = yy_2;
yy_0_1_1 = yyv_H;
yy_0_1_2_1 = yyv_Pos;
yy_0_1_2 = yyb + 4;
yy_0_1_2[0] = 2;
yy_0_1_2[1] = ((long)yy_0_1_2_1);
yy_0_1_3 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yy_0_1[3] = ((long)yy_0_1_3);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 55:
//#line 1512 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_Id;
yy yy_1_1;
yy yyv_Pos;
yy yy_2;
yy_1_1 = (yy)(yyvsp[0].attr[1]);
yy_2 = (yy)(yyvsp[0].attr[0]);
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yyv_Id = yy_1_1;
yyv_Pos = yy_2;
yy_0_1_1 = yyv_Id;
yy_0_1_2 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 56:
//#line 1541 "gen.y"
    {
yy yy_0_1;
yy yyv_N;
yy yy_1_1;
yy yyv_L;
yy yy_2_1;
yy yy_3;
yy_1_1 = (yy)(yyvsp[-1].attr[1]);
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyv_N = yy_1_1;
yyv_L = yy_2_1;
yy_3 = yyv_N;
yyglov_AlternativeIndex = yy_3;
yy_0_1 = yyv_L;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 57:
//#line 1564 "gen.y"
    {
yy yy_0_1;
yy yyv_N;
yy yy_1_1;
yy yyv_L;
yy yy_2_1;
yy yy_3;
yy_1_1 = (yy)(yyvsp[-1].attr[1]);
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyv_N = yy_1_1;
yyv_L = yy_2_1;
yy_3 = yyv_N;
yyglov_AlternativeIndex = yy_3;
yy_0_1 = yyv_L;
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 58:
//#line 1585 "gen.y"
    {
yy yy_0_1;
yy yyv_N;
yy yy_1;
yy yy_2;
yy_1 = yyglov_AlternativeIndex;
if (yy_1 == (yy) yyu) yyErr(1,534);
yyv_N = yy_1;
yy_2 = ((yy)0);
yyglov_AlternativeIndex = yy_2;
yy_0_1 = yyv_N;
yyval.attr[1] = ((long)yy_0_1);
yyGetPos(&yyval.attr[0]);
;}
    break;

  case 59:
//#line 1606 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_H;
yy yy_1_1;
yy yyv_T;
yy yy_3_1;
yy_1_1 = (yy)(yyvsp[-2].attr[1]);
yy_3_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yyv_T = yy_3_1;
yy_0_1_1 = yyv_H;
yy_0_1_2 = yyv_T;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 60:
//#line 1634 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_2_1;
yy yy_0_1_2_2;
yy yyv_H;
yy yy_1_1;
yy yyv_Pos;
yy yy_2;
yy yy_3_1_1;
yy yy_3_1_2_1;
yy yyv_Anno;
yy yy_3_1_2_2;
yy yyv_N;
yy yy_3_2_1;
yy yy_3_2_2_1;
yy yy_3_2_2_1_1;
yy yy_3_2_2_1_1_1;
yy yy_3_2_2_1_1_2;
yy yy_3_2_2_2;
yy_1_1 = (yy)(yyvsp[0].attr[1]);
yy_2 = (yy)(yyvsp[0].attr[0]);
yyb = yyh;
yyh += 6; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yyv_Pos = yy_2;
{
yy yysb = yyb;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_3_1_1 = yyglov_DefaultSpec;
if (yy_3_1_1 == (yy) yyu) yyErr(1,544);
if (yy_3_1_1[0] != 2) goto yyfl_67_2_3_1;
yy_3_1_2_1 = yyb + 0;
yy_3_1_2_1[0] = 2;
yy_3_1_2_2 = yy_3_1_2_1;
yyv_Anno = yy_3_1_2_2;
goto yysl_67_2_3;
yyfl_67_2_3_1 : ;
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
yy_3_2_1 = yyglov_AlternativeIndex;
if (yy_3_2_1 == (yy) yyu) yyErr(1,547);
yyv_N = yy_3_2_1;
yy_3_2_2_1_1_1 = yyv_N;
yy_3_2_2_1_1_2 = ((yy)1);
yy_3_2_2_1_1 = (yy)(((long)yy_3_2_2_1_1_1)+((long)yy_3_2_2_1_1_2));
yy_3_2_2_1 = yyb + 0;
yy_3_2_2_1[0] = 1;
yy_3_2_2_1[1] = ((long)yy_3_2_2_1_1);
yy_3_2_2_2 = yy_3_2_2_1;
yyv_Anno = yy_3_2_2_2;
goto yysl_67_2_3;
yysl_67_2_3 : ;
yyb = yysb;
}
yy_0_1_1 = yyv_H;
yy_0_1_2_1 = yyv_Anno;
yy_0_1_2_2 = yyv_Pos;
yy_0_1_2 = yyb + 3;
yy_0_1_2[0] = 2;
yy_0_1_2[1] = ((long)yy_0_1_2_1);
yy_0_1_2[2] = ((long)yy_0_1_2_2);
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 61:
//#line 1712 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_2_1;
yy yy_0_1_2_2;
yy yyv_H;
yy yy_1_1;
yy yyv_Pos;
yy yy_3;
yy yyv_Anno;
yy yy_4_1;
yy_1_1 = (yy)(yyvsp[-2].attr[1]);
yy_3 = (yy)(yyvsp[-1].attr[0]);
yy_4_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 6; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yyv_Pos = yy_3;
yyv_Anno = yy_4_1;
yy_0_1_1 = yyv_H;
yy_0_1_2_1 = yyv_Anno;
yy_0_1_2_2 = yyv_Pos;
yy_0_1_2 = yyb + 3;
yy_0_1_2[0] = 2;
yy_0_1_2[1] = ((long)yy_0_1_2_1);
yy_0_1_2[2] = ((long)yy_0_1_2_2);
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 62:
//#line 1753 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_H;
yy yy_1_1;
yy yyv_T;
yy yy_3_1;
yy_1_1 = (yy)(yyvsp[-2].attr[1]);
yy_3_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yyv_T = yy_3_1;
yy_0_1_1 = yyv_H;
yy_0_1_2 = yyv_T;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 63:
//#line 1781 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_2_1;
yy yy_0_1_2_2;
yy yyv_H;
yy yy_1_1;
yy yyv_Pos;
yy yy_2;
yy yy_3_1_1;
yy yy_3_1_2_1;
yy yyv_Anno;
yy yy_3_1_2_2;
yy yyv_N;
yy yy_3_2_1;
yy yy_3_2_2_1;
yy yy_3_2_2_1_1;
yy yy_3_2_2_1_1_1;
yy yy_3_2_2_1_1_2;
yy yy_3_2_2_2;
yy_1_1 = (yy)(yyvsp[0].attr[1]);
yy_2 = (yy)(yyvsp[0].attr[0]);
yyb = yyh;
yyh += 6; if (yyh > yyhx) yyExtend();
yyv_H = yy_1_1;
yyv_Pos = yy_2;
{
yy yysb = yyb;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_3_1_1 = yyglov_DefaultSpec;
if (yy_3_1_1 == (yy) yyu) yyErr(1,563);
if (yy_3_1_1[0] != 2) goto yyfl_68_2_3_1;
yy_3_1_2_1 = yyb + 0;
yy_3_1_2_1[0] = 2;
yy_3_1_2_2 = yy_3_1_2_1;
yyv_Anno = yy_3_1_2_2;
goto yysl_68_2_3;
yyfl_68_2_3_1 : ;
yyb = yyh;
yyh += 2; if (yyh > yyhx) yyExtend();
yy_3_2_1 = yyglov_AlternativeIndex;
if (yy_3_2_1 == (yy) yyu) yyErr(1,566);
yyv_N = yy_3_2_1;
yy_3_2_2_1_1_1 = yyv_N;
yy_3_2_2_1_1_2 = ((yy)1);
yy_3_2_2_1_1 = (yy)(((long)yy_3_2_2_1_1_1)+((long)yy_3_2_2_1_1_2));
yy_3_2_2_1 = yyb + 0;
yy_3_2_2_1[0] = 1;
yy_3_2_2_1[1] = ((long)yy_3_2_2_1_1);
yy_3_2_2_2 = yy_3_2_2_1;
yyv_Anno = yy_3_2_2_2;
goto yysl_68_2_3;
yysl_68_2_3 : ;
yyb = yysb;
}
yy_0_1_1 = yyv_H;
yy_0_1_2_1 = yyv_Anno;
yy_0_1_2_2 = yyv_Pos;
yy_0_1_2 = yyb + 3;
yy_0_1_2[0] = 2;
yy_0_1_2[1] = ((long)yy_0_1_2_1);
yy_0_1_2[2] = ((long)yy_0_1_2_2);
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 64:
//#line 1858 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yy_0_1_3;
yy yy_0_1_4;
yy yyv_Pos;
yy yy_1;
yy yyv_L;
yy yy_2_1;
yy yyv_Annotation;
yy yy_3_1;
yy yyv_Key;
yy yyv_N;
yy yy_5_1;
yy yy_6;
yy yy_7;
yy_1 = (yy)(yyvsp[-1].attr[0]);
yy_2_1 = (yy)(yyvsp[-1].attr[1]);
yy_3_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 10; if (yyh > yyhx) yyExtend();
yyv_Pos = yy_1;
yyv_L = yy_2_1;
yyv_Annotation = yy_3_1;
yyv_Key = yyb + 5;
yyb[9] = yyu;
yyb[8] = yyu;
yyb[7] = yyu;
yyb[6] = yyu;
FreshAlternativeNumber(&yy_5_1);
yyv_N = yy_5_1;
yy_6 = yyv_N;
yyv_Key[2] = (long) yy_6;
yy_7 = yyv_L;
yyv_Key[1] = (long) yy_7;
yy_0_1_1 = yyv_L;
yy_0_1_2 = yyv_Key;
yy_0_1_3 = yyv_Annotation;
yy_0_1_4 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yy_0_1[3] = ((long)yy_0_1_3);
yy_0_1[4] = ((long)yy_0_1_4);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[-1].attr[0];
;}
    break;

  case 65:
//#line 1912 "gen.y"
    {
yy yyb;
yy yy_0_1;
yy yy_0_1_1;
yy yy_0_1_2;
yy yyv_Pos;
yy yy_1;
yy yyv_Str;
yy yy_2_1;
yy_1 = (yy)(yyvsp[0].attr[0]);
yy_2_1 = (yy)(yyvsp[0].attr[1]);
yyb = yyh;
yyh += 3; if (yyh > yyhx) yyExtend();
yyv_Pos = yy_1;
yyv_Str = yy_2_1;
yy_0_1_1 = yyv_Str;
yy_0_1_2 = yyv_Pos;
yy_0_1 = yyb + 0;
yy_0_1[0] = 1;
yy_0_1[1] = ((long)yy_0_1_1);
yy_0_1[2] = ((long)yy_0_1_2);
yyval.attr[1] = ((long)yy_0_1);
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 66:
//#line 1940 "gen.y"
    {
yy yyb;
yy yy_2;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_2 = yyb + 0;
yy_2[0] = 1;
yyglov_GentleFlag = yy_2;
yyval.attr[0] = yyvsp[0].attr[0];
;}
    break;

  case 67:
//#line 1953 "gen.y"
    {
yy yyb;
yy yy_1;
yyb = yyh;
yyh += 1; if (yyh > yyhx) yyExtend();
yy_1 = yyb + 0;
yy_1[0] = 2;
yyglov_GentleFlag = yy_1;
yyGetPos(&yyval.attr[0]);
;}
    break;


    }

/* Line 999 of yacc.c.  */
//#line 3011 "gen.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}




