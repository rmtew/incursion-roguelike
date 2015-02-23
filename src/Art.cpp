#ifdef DEBUG
/*
  This file is GPLv2, it cannot be compiled into any distributed binaries,
  otherwise it will be a GPL violation.  Ideally we'll become more free
  by removing this restrictively licensed code in future.  It's holding us
  back.
*/
/*
 *   art.c
 *
 *   ACCENT RUNTIME
 *
 *   Copyright (C) 1984, 1999 Friedrich Wilhelm Schroeer
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*============================================================================*/
#include "Incursion.h"
#include <stdlib.h>
#include <stdio.h>
#include "yygram.h"

#pragma warning (disable : 4508)

char* yyprintname(int);
int yyselect(),  yymallocerror();
int yydirset(int,int);
int yyerror(const char*);
int YYSTART();
void get_lexval(), init_lexelem(), first_lexval(), 
  init_dirsets(), next_lexval();
int yylex();

#define PRIVATE static
#define PUBLIC
/*============================================================================*/
/* OPTIONS                                                                    */
/*============================================================================*/

#undef  TRACE
#define TRACE 0
#define PRINTTREE 0
#define WALK 1
#define CHECKVIABLE 1
#define LOOKAHEAD 1
#define DETECTAMBIGUITY 1
#define DYNAMICCYCLECHECK 1
#define HASHING 1
#define BIGHASH 0
#define STATISTICS 0
#define DYNAMICITEMS 1

/*============================================================================*/
/* ITEMS                                                                      */
/*============================================================================*/

#if DYNAMICITEMS
int ITEMLIMIT;
#else
#define ITEMLIMIT    285000
#endif

#define ITEMINCR 285000

# if DYNAMICITEMS
long *dot, *back, *left, *sub;
#else
long dot[ITEMLIMIT], back[ITEMLIMIT], left[ITEMLIMIT], sub[ITEMLIMIT];
#endif
/*
 * An "item" is a quadrupel < D, B, L, S > , where
 *
 * D is a rule currently being processed.
 *   A working point ("dot", written as "*")
 *   devides the right hand side into part already being processed (alpha)
 *   and a part that still needs to be processed (beta)
 *      M : alpha * beta
 *
 * B (back-pointer) is a reference to a list of items in which the
 *   processing of rule rule
 *      D  =  M : M : alpha * beta
 *   startet. This list contains a start item
 *      I' =  < D', B', L', S' >
 *   where
 *      D' =  M : * alpha beta
 *
 * L (left-pointer) 
 *
 * S (sub-pointer)
 *
 * [ documentation to be completed in forthcoming release ....... ]
 *
 * If I is the index of an item < D, B, L, S > then
 *    dot[I]  = D
 *    back[I] = B
 *    left[I] = L
 *    sub[I]  = S
 */

long thislist;
/*
 * current itemlist (index of first item of list)
 */

long last_item;
/*
 * index of last item in item list
 * position last_item+1 is used for sentinel during searching
 * last_item+1 must not exceed ITEMLIMIT-1 (i.e. last_item < ITEMLIMIT-2)
 *
 */

long specialitemadded;
/*
 * an item with index i such that back[i]==thislist
 * has been added to the current item list
 * This requires a rescan of previous items during closure computation
 * (which otherwise is not neccessary)
 */

/*============================================================================*/
/* GRAMMAR ENCODING                                                           */
/*============================================================================*/

extern int yygrammar[];
/*
 * encoded grammar
 * defined in 'yygrammar.c'
 *
 *
 * structure of encoding
 *
 * a rule
 *    [r] M0 : M1 ... Mn
 * with number r, lhs M0, and rhs M1 .. M1
 * is encoded as follows
 *
 *   +-- chain<-+
 *   |   M1*-------> first rule for M1
 *   |   ...    |
 *   |   Mn*    |
 *   |   -M0*---+
 *   |   r
 *   +-> next rule for M0
 *
 * chain : index of start of next rule for M0 (or 0)
 *
 * M1*   : encoding of rhs member Mi (1 <= i <= n)
 * ...     Mi nonterminal: index of start of encoding of first rule for Mi
 * Mn*     Mi terminal: see below
 *
 * -M0*  : negative index of start of encoding of first rule for M0
 *
 * r     : rule number
 *
 * The grammar starts with the rule
 *    [0] YYSTART : UserRoot EOF
 * which is encoded as follows:
 *
 *   1: 0     (chain)
 *   2: 6     (first rule of user root)
 *   3: EOF   (eof token)
 *   4: -1    (points to start of rule)
 *   5: 0     (rule number)
 */

extern int yyannotation[];
/* defined in 'yygrammar.c' */

extern int yycoordinate[];
/* defined in 'yygrammar.c' */

extern int c_length;
 /*
  * length of yygrammar
  * defined in 'yygrammar.c'
  */

# define term_base 50000
# define max_char    255
# define eofsym    term_base

/* nonterminals are encoded as
 *    1 ... term_base-1
 * the eof terminal is encoded as
 *    termbase
 * single character terminals are encoded as
 *    term_base+1 ... term_base+max_char
 * named terminals are encoded as
 *    term_base+max_char+1 ...
 *
 * yylex() returns an original token code
 * that must be incremented by term_base
 */

/*============================================================================*/
/* DIRECTOR SETS                                                              */
/*============================================================================*/

long sym;
/* current input token */

long lookaheadsym;
/* next input token */

extern long yypos;
long yypos = 1;
/* this variable must be set by the scanner */

PRIVATE long lookaheadpos;

#if BIGHASH
extern long DHITS[];
long BHITS[ITEMLIMIT];
/*
 * search optimisation
 *
 * DHITS[d] == thislist:
 * there is an item with dot d in the current itemlist
 * (if not: no search neccessary)
 *
 * BHITS[b] == thislist:
 * there is an item with backpointer b in the current itemlist
 * (if not: no search neccessary)
 */
#endif

/*============================================================================*/
/* STATISTICS                                                                 */
/*============================================================================*/

#if STATISTICS
int itemlistcount;
int notviable = 0;
int search = 0;
int nosearch = 0;
int nosearch_b = 0;
int nosearch_h = 0;
/* no_search == 1:
 * no need to call SEARCH because no item with same hash code
 */
int foundsame = 0;
int foundnew = 0;
int completerstep = 0;
int truecompleterstep = 0;
int calls_additem = 0;
int itemsadded = 0;
int rules_rej = 0;
/* rules rejected by 'lookup_dirset' (predictor) */
int tokens_rej = 0;
/* tokens rejected by 'is_viable' (kernel/completer) */
int nonterms_rej = 0;
/* nonterms rejected by 'is_viable' (kernel/completer) */

int GOOD = 0;
/* number of items with dot preceeding a token */
/* that contribute to the next kernel */
int BAD = 0;
/* number of items with dot preceeding a token */
/* that don't contribute to the next kernel */
#endif

/*============================================================================*/
/* PRINT ROUTINES                                                             */
/*============================================================================*/

PRIVATE int print_item(int p)
/*
 * print the item with index p
 */
{
   int i, b, e, l, k;
   i = dot[p];

   T1->CWrite(Format(" %d: ", p));
   if (dot[p] == 0 && sub[p] == 0) {
      printf("[ separator-item ]\n");
      return 0;
   }
   if (i <= 5) {
      b = 1;
   }
   else {
      b = i-1;
      while(yygrammar[b] >= 0) b--;
      b += 2;
   }
   /* b points to the start of the rule */

   e = i;
   while(yygrammar[e] >= 0) e++;
   /* e points to the end of the rule, i.e. the lhscode */

   l = - yygrammar[e];
   /* l is the lhs */

   T1->CWrite(Format("%s :", yyprintname(l)));
   k = b+1;
   /* k points to the first member */
   while(yygrammar[k] > 0) {
      if (k == i) printf(" *");
      /* print member yygrammar[k] */
      if (yygrammar[k] == 5000) {
	 printf(" <EOF>");
      }
      else if (yygrammar[k] > 5000) {
	 if (yygrammar[k] < term_base+max_char+1)
	    { T1->CWrite(Format(" '%c'", yygrammar[k]-term_base)); }
	 else
	    { T1->CWrite(Format(" %s", yyprintname(yygrammar[k]))); }
      }
      else {
	 T1->CWrite(Format(" %s", yyprintname(yygrammar[k])));
      }
      k++;
   }
   if (yygrammar[i] <= 0) printf(" *");
   T1->CWrite(Format(" (back:%d sub:%d left:%d)\n", back[p], sub[p], left[p]));
}

/*----------------------------------------------------------------------------*/

PRIVATE void print_coordinate(int i)
/*
 * print source coordinate (of grammar file) with code i
 * the number encodes both, line and column information
 */
{
   int pos = yycoordinate[i];
   int l = pos / 1000;
   int c = pos % 1000;

   T1->CWrite(Format("line %d, col %d of grammar", l, c));
}

/*----------------------------------------------------------------------------*/

PRIVATE void print_tree(int i)
/*
 * print tree for item with index i
 */
{
   static int indent = 0;
   int k;

   /* rule number if item at end of rule */
   if (yygrammar[dot[i]] < 0 ) {
      /* end of rule */
      for (k = 1; k <= indent; k++) printf("  ");
      T1->CWrite(Format("%s alternative at ", yyprintname(-yygrammar[dot[i]])));
      print_coordinate(dot[i]+1);
      T1->CWrite(" {\n");
      indent++;
   }

   /* left brothers */

   if (left[i]) print_tree(left[i]);

   /* this son */

   if (left[i]) {
      int sym = yygrammar[dot[i]-1];

      if (sym > term_base) {
         for (k = 1; k <= indent; k++) printf("  ");
	 if (sym < term_base+max_char+1)
            T1->CWrite(Format("'%c'\n", yygrammar[dot[i]-1]-term_base));
	 else
	    T1->CWrite(Format("%s\n", yyprintname(sym)));
      }
   }

   /* subtree for this sun */

   if (sub[i])  print_tree(sub[i]);

   if (yygrammar[dot[i]] < 0 ) {
      /* end of rule */
      indent--;
      for (k = 1; k <= indent; k++) T1->CWrite("  ");
      T1->CWrite("}\n");
   }
}

/*----------------------------------------------------------------------------*/

#if STATISTICS
PRIVATE void print_statistics()
/*
 * print values of statistics counters
 */
{
   T1->CWrite(Format("itemlistcount =%10d\n", itemlistcount));
   T1->CWrite(Format("notviable     =%10d\n", notviable));
   T1->CWrite(Format("search        =%10d\n", search));
   T1->CWrite(Format("nosearch      =%10d\n", nosearch));
   T1->CWrite(Format("nosearch_b    =%10d\n", nosearch_b));
   T1->CWrite(Format("nosearch_h    =%10d\n", nosearch_h));
   T1->CWrite(Format("foundsame     =%10d\n", foundsame));
   T1->CWrite(Format("foundnew      =%10d\n", foundnew));
   T1->CWrite(Format("completerst   =%10d\n", completerstep));
   T1->CWrite(Format("truecomplst   =%10d\n", truecompleterstep));
   T1->CWrite(Format("calls_additem =%10d\n", calls_additem));
   T1->CWrite(Format("itemsadded    =%10d\n", itemsadded));
   T1->CWrite(Format("rules_rej     =%10d (predictor: lookup_dirset)\n", rules_rej));
   T1->CWrite(Format("tokens_rej    =%10d (kernel/completer: is_viable)\n", tokens_rej));
   T1->CWrite(Format("nonterms_rej  =%10d (kernel/completer: is_viable)\n", nonterms_rej));
   T1->CWrite(Format("GOOD          =%10d\n", GOOD));
   T1->CWrite(Format("BAD           =%10d\n", BAD));
}
#endif

/*============================================================================*/
/* DIRECTOR SETS                                                              */
/*============================================================================*/

PRIVATE int lookup_dirset(long ruleptr)
/*
 * Let 'rule' be the rule be the rule into which 'ruleptr' points.
 * Let 'tkn' be the code of the next token.
 * Return ('tkn' is in the director set of 'rule').
 */
{
   int p;
   int rule;
   int tkn;

   /* find rule number */
   p = ruleptr;
   while (yygrammar[p] >= 0) p++;
   p++;
   rule = yygrammar[p];

   tkn = lookaheadsym-term_base;

   return yydirset(rule, tkn);
}

/*----------------------------------------------------------------------------*/

PRIVATE int is_viable (int d)
/*
 * d is a pointer into the encoded grammar
 * Returns true if the the symbol yygrammar[d] (token or nonterminal)
 * is a valid continuation of the parse:
 * can it start with the current lookahead token?
 * If the symbol is a token it is compared with the lookahead token
 * If symbol is a nonterminal it is checked whether the lookahead token
 * appears in the director sets of the rules for the nonterm of the symbol
 *
 * NOTE: the union of the director sets should be computed statically
 */
{
   if (yygrammar[d] >= term_base) {
      /* token */
      if (yygrammar[d] == lookaheadsym) {
	 return 1;
      }
      else {
#if STATISTICS
	 tokens_rej++;
#endif
	 return 0;
      }
   }

   else if (yygrammar[d] > 0 ) {
      /* nonterm */

      int start;

      start = yygrammar[d];
      /* start points to the first rule for the nonterm */
      do {
	 int p;
	 int rule;

	 p = start+1;
	 while (yygrammar[p] >= 0) {
	    p++;
	 }
	 /* p now points to negative lhs encoding */
	 rule = yygrammar[p+1];
	 if (yydirset(rule, lookaheadsym-term_base)) {
	    return 1;
	 }
	 start = yygrammar[start];
      } while (start);

#if STATISTICS
      nonterms_rej++;
#endif
      return 0;
   }

   else {
      /* end of rule */
      return 1;
   }
}

/*============================================================================*/
/* ERROR MESSAGES                                                             */
/*============================================================================*/

PRIVATE void syntaxerror()
/*
 * Report syntax error and terminate
 */
{
   yyerror("syntax error");
   exit(1);
}

/*----------------------------------------------------------------------------*/

PRIVATE void Abort(char* msg)
/*
 * emit msg and terminate
 */
{
   printf("%s\n", msg);
   exit(1);
}

/*----------------------------------------------------------------------------*/

PRIVATE void table_full()
/*
 * item table full
 */
{
#if DYNAMICITEMS
   ITEMLIMIT += ITEMINCR;

   dot = (long *) realloc(dot, ITEMLIMIT* sizeof(long));
   if (! dot) yymallocerror();
   back = (long *) realloc(back, ITEMLIMIT* sizeof(long));
   if (! back) yymallocerror();
   left = (long *) realloc(left, ITEMLIMIT* sizeof(long));
   if (! left) yymallocerror();
   sub = (long *) realloc(sub, ITEMLIMIT* sizeof(long));
   if (! sub) yymallocerror();
#else
   Abort("fatal error: item table overflow [increase ITEMLIMIT in art.c]\n");
#endif
}

/*----------------------------------------------------------------------------*/

PUBLIC int yymallocerror()
{
   printf("running out of memory\n");
   exit(1);
}

/*============================================================================*/
/* SEARCH OPTIMISATION                                                        */
/*============================================================================*/

#if BIGHASH
PRIVATE init_DHITS()
/*
 * Initialise DHITS
 * (value must be different from possible values of 'thislist')
 */
{ 
   int i;

   for (i=1; i<=c_length; i++) {
      DHITS[i] = 0;
   }
}
#endif

/*----------------------------------------------------------------------------*/

#define HSIZE 1024

#define HASHCODE (b%8+d)%HSIZE

#if HASHING

int hash[HSIZE];
/*
 * hash table to speed up lookup-function
 * if an item with back pointer b and dot d is entered
 * into the current item list
 * an entry with a corresponding hash code is set
 */

/*----------------------------------------------------------------------------*/

#ifdef HASHING
PRIVATE int clearhash ()
/*
 * clear hash table
 */
{
   int i;

   for (i = 0; i<HSIZE; i++) hash[i] = 0;
   return 0;
}
#endif

/*----------------------------------------------------------------------------*/

PRIVATE int hashed(int d, int b)
/*
 * true if there is an entry entry for item with dot d and backpointer b
 */
{
   return hash[HASHCODE];
}

/*----------------------------------------------------------------------------*/

PRIVATE void sethash(int d, int b)
/*
 * set entry for item with dot d and backpointer b
 */
{
   hash[HASHCODE] = 1;
}
#endif


/*============================================================================*/
/* TOKENS                                                                     */
/*============================================================================*/

PRIVATE void readsym ()
/*
 * read next token
 * current token: 'sym'
 * following token: 'lookaheadsym'
 * extend the list of lexical values by calling
 * next_lexval() provided by 'yyactions.c'
 */
{
   long truepos;

   sym = lookaheadsym;
   truepos = lookaheadpos;

   yypos = truepos;
   if (lookaheadsym != 50000 /*EOF*/) {
      /* xxx
      printf("calling yylex()\n");
      */
      lookaheadsym = yylex()+term_base;
      /* xxx
      printf("back from yylex()\n");
      */
      lookaheadpos = yypos;
   }
   else /* printf("was already EOF\n") */ ;
   next_lexval();
   yypos = truepos;
   /* xxxxx */
   /*
   printf("readsym: sym = %d, lookaheadsym = %d\n", sym, lookaheadsym);
   */
}

/*============================================================================*/
/* GRAMMAR                                                                    */
/*============================================================================*/

PRIVATE int getprio(int subptr)

   /* subptr points to an item */
   /* return the prio of corresponding rule */
{
   int i;

   /* find end of rule */
   i = dot[subptr];
   while (yygrammar[i] > 0) i++;
   /* i points to the negative lhs encoding */
   i++;
   /* i now points to the rule number, this is also the index of the prio */

   return yyannotation[i];
}

/*----------------------------------------------------------------------------*/

PRIVATE int getmemberannotation(int i)

   /* i is the index of a member */
   /* return the annotation of the member before the dot */
{
   int grammarindex, annotation;

   grammarindex = dot[i]-1;
   annotation = yyannotation[grammarindex];
   return annotation;
}

/*============================================================================*/
/* AMBIGUITY RESOLUTION                                                       */
/*============================================================================*/

PRIVATE int test_for_cycle(int subtree, int container)
/*
 * return true if
 * the tree to which 'container' (a "subpointer" of an item) points
 * contains the tree to which 'subtree' (also a "subpointer") points
 */
{
   if (container < subtree) {
      /* an earlier item cannot refer a later one */ 
      return 0;
   }
   if (container == subtree) {
      return 1;
   }
   if (sub[container]) {
      if (test_for_cycle(subtree, sub[container]))
         return 1;
   }
   if (left[container]) {
      if (test_for_cycle(subtree, left[container]))
         return 1;
   }
   return 0;
}

/*----------------------------------------------------------------------------*/

PRIVATE void combi_ambiguity(int i, int d, int l, int s)
/*
 * The item with index i
 * and an item with dot d, leftpointer l, and subpointer s
 * introduce a combi ambiguity
 */
{
   if (left[i] != l) {

      /* Combi Ambiguity */

      int left1, left2, sub1, sub2, annotation,
	 selected_left, selected_sub;

#if TRACE
      printf("Combi Ambiguity left[%d] = %d, l = %d\n", i, left[i], l);
#endif

      left1 = left[i];
      sub1 = sub[i];

      left2 = l;
      sub2 = s;

      annotation = getmemberannotation(i);

      if (annotation == 1) {
	 /* %short */

	 if (left1 > left2) {
	    selected_left = left1;
	    selected_sub = sub1;
	 }
	 else {
	    selected_left = left2;
	    selected_sub = sub2;
	 }
      }
      else if (annotation == 2) {
	 /* %long */

	 if (left2 > left1) {
	    selected_left = left1;
	    selected_sub = sub1;
	 }
	 else {
	    selected_left = left2;
	    selected_sub = sub2;
	 }
      }
      else {
	 /* annotation == undef */

	 printf("\n");
	 printf("GRAMMAR DEBUG INFORMATION\n");
	 printf("\n");
	 printf("Grammar ambiguity detected.\n");

	 {
	    int k;
	    k = dot[i];
	    while (yygrammar[k] > 0) k++;
	    printf("There are two different parses\n");
	    printf("for the beginning of ``%s'', alternative at ",
	       yyprintname(-yygrammar[k]));
	    print_coordinate(k+1);
	    printf(",\n");
	 }
	 printf("upto and containing ``%s'' at ", yyprintname(yygrammar[d-1]));
	 print_coordinate(d-1);
	 printf(".\n");

	 printf("\n");
	 printf("PARSE 1\n");
	 printf("-------\n");
	 printf("\n");
	 print_tree(i);

	 /* xxx
	 old_sub = sub[i];
	 old_left = left[i];
	 sub[i] = s;
	 left[i] = l;
	 */
	 printf("\n");
	 printf("PARSE 2\n");
	 printf("-------\n");
	 printf("\n");
	 /* xxx
	 print_tree(i);
	 */
	 print_tree(last_item+1);

	 printf("\n");
	 printf("For ``%s'' at ", yyprintname(yygrammar[d-1]));
	 print_coordinate(d-1);
	 printf(",\n");
	 if (left1 > left2) {
	    printf("use %%short annotation to select first parse,\n");
	    printf("use %%long annotation to select second parse.\n");
	 }
	 else {
	    printf("use %%long annotation to select first parse,\n");
	    printf("use %%short annotation to select second parse.\n");
	 }

	 /* xxx
	 sub[i] = old_sub;
	 left[i] = old_left;
	 */

	 printf("\nEND OF GRAMMAR DEBUG INFORMATION\n\n");

	 yyerror("source text uncovers unhandled grammar ambiguity");
	 exit(1);

	 selected_left = l;
	 selected_sub = s;
      }

      left[i] = selected_left;
      sub[i] = selected_sub;
   }
}

/*----------------------------------------------------------------------------*/

PRIVATE void simple_ambiguity(int i, int d, int l, int s)
/*
 * The item with index i
 * and an item with dot d, leftpointer l, and subpointer s
 * introduce a simple ambiguity
 */
{
   /* Simple Ambiguity */
   T1->SetWin(WIN_SCREEN);
   T1->Clear(); T1->GotoXY(0,0);
   T1->CursorOn();

   int sub1, sub2, prio1, prio2;

#if TRACE
   printf("simple amiguity\n");
   printf("i = %d\n", i);
   printf("last_item+1 = %d\n", last_item+1);
#endif
   sub1 = sub[i];
   sub2 = s;

   prio1 = getprio(sub1);
   prio2 = getprio(sub2);

   if (prio1 == -1 || prio2 == -1) {
      /* undefined prio */

      printf("\n");
      printf("GRAMMAR DEBUG INFORMATION\n");
      printf("\n");
      printf("Grammar ambiguity detected.\n");
      printf
	 ("Two different ``%s'' derivation trees for the same phrase.\n",
	 yyprintname(yygrammar[d-1]));

      printf("\n");
      printf("TREE 1\n");
      printf("------\n");
      printf("\n");
      print_tree(sub[i]);
      printf("\n");
      T1->GetCharRaw();
      T1->Clear(); T1->GotoXY(0,0);
      T1->CursorOn();

      printf("TREE 2\n");
      printf("------\n");
      printf("\n");
      print_tree(s);
      printf("\n");

      if (test_for_cycle(s, sub[i])) {
	 /* not possible */
	 printf("Tree 1 contains tree 2 as subtree.\n");
	 printf
	    ("Use %%prio annotation to select the second tree.\n");
	 printf("An annotation selecting the first tree\n");
	 printf("would not resolve the ambiguity.\n");
      }
      else if (test_for_cycle(sub[i], s)) {
	 printf("Tree 2 contains tree 1 as subtree.\n");
	 printf
	    ("Use %%prio annotation to select the first tree.\n");
	 printf("An annotation selecting the second tree\n");
	 printf("would not resolve the ambiguity.\n");
      }
      else {
	 printf("Use %%prio annotation to select an alternative.\n");
      }

      printf("\nEND OF GRAMMAR DEBUG INFORMATION\n\n");

      yyerror("source text uncovers unhandled grammar ambiguity");
      exit(1);

   }

   else if (prio1 > prio2) {
#if TRACE
      printf("old value wins\n");
#endif
   }
   else {
#if TRACE
      printf("new value wins\n");
      printf("sub[%d] was %d\n", i, sub[i]);
      printf("sub[%d] becomes %d\n", i, s);
#endif

#if DYNAMICCYCLECHECK
      if (s >= i) {
	 if (test_for_cycle(i, s)) {
	    printf("\n");
	    printf("GRAMMAR DEBUG INFORMATION\n");
	    printf("\n");
	    printf("Annotation for ``%s'' allows cyclic derivation.\n",
	       yyprintname(yygrammar[d-1]));
	    printf("\nEND OF GRAMMAR DEBUG INFORMATION\n\n");
	    yyerror("source text uncovers unhandled grammar ambiguity");
	    exit(1);
	 }
      }
#endif
      sub[i] = s;
   }
}

/*============================================================================*/
/* EARLEY                                                                     */
/*============================================================================*/

PRIVATE void SEARCH(long d, long b, long l, long s)
/*
 * An item with dot d, backpointer b, leftpointer l, subpointer s
 * has been prliminary added to the current list at position
 * last_item+1 (sentinel)
 * if there is no other item with dot d and backpointer b
 * make this item permanent
 * otherwise, if the old item has a different backpointer/subpointer
 * then an ambiguity is detected
 * if the backpointer/subpointer is the same
 * the new item is already present
 */
{
   register long i;

   i = thislist;

   while ((dot[i]!=d)||(back[i]!=b)) i++;

   if (i==last_item+1) {
      last_item++;
      if (last_item==(ITEMLIMIT-2)) table_full();
#if HASHING
      sethash(d, b);
#endif
#if STATISTICS
      foundnew++;
#endif
   }
   else {
#if STATISTICS
      foundsame++;
#endif

#if DETECTAMBIGUITY
      if (left[i] != l)
	 combi_ambiguity(i, d, l, s);
      else if (sub[i] != s)
	 simple_ambiguity(i, d, l, s);

#endif

   }
}

/*----------------------------------------------------------------------------*/

PRIVATE void additem (long d, long b, long l, long s)
/*
 * add an item with dot d, backpointer b, leftpointer l, and subpointer s
 * to the current item list
 * if there is already an item with dot d and backpointer b
 * then the grammar is ambigous (see 'SEARCH', which actually adds the item)
 *
 * hash optimization:
 * if there is no item with the same hash code for d and b
 * it is not neccessary to invoke SEARCH because the item is unique in the
 * current list
 */
{
#if STATISTICS || TRACE
   int old = last_item;
#endif

#if STATISTICS
   calls_additem++;
#endif

   /* sentinel */
   dot [ last_item+1 ] = d;
   back[ last_item+1 ] = b;
   left[ last_item+1 ] = l;
   sub [ last_item+1 ] = s;

#if TRACE
   print_item(last_item+1);
#endif

#if HASHING
   if (! hashed(d, b)) {
#if STATISTICS
      nosearch_h++;
#endif
#if BIGHASH
      DHITS[d] = thislist;
      BHITS[b] = thislist;
#endif
      last_item++; if (last_item==(ITEMLIMIT-2)) table_full();
      sethash(d, b);
   }
   else {
#endif
#if BIGHASH
      if (DHITS[d] == thislist) {
	 /* there may be already an item with code d */
	 if (BHITS[b] == thislist) {
	    /* there may be already an item with code b */
#if STATISTICS
	    search++;
#endif
	    SEARCH(d, b, l, s);
	 }
	 else
	 {
	    /* no search necessary: no item with code b entered til now */

#if STATISTICS
	    nosearch_b++;
#endif
	    BHITS[b] = thislist;
	    last_item++; if (last_item==(ITEMLIMIT-2)) table_full();
#if HASHING
	    sethash(d, b);
#endif
	 }
      }
      else {
	 /* no search necessary: no item with code d entered til now */

#if STATISTICS
	 nosearch++;
#endif
	 DHITS[d] = thislist;
	 BHITS[b] = thislist;
	 last_item++; if (last_item==(ITEMLIMIT-2)) table_full();
#if HASHING
	 sethash(d, b);
#endif
      }
#else
#if STATISTICS
      search++;
#endif
      SEARCH(d, b, l, s);
#endif
#if HASHING
   }
#endif

#if STATISTICS
   if (last_item > old) {
      itemsadded++;
   }
#endif
#if TRACE
   if (last_item <= old) {
      printf(" [already inserted]\n");
   }
#endif
}

/*----------------------------------------------------------------------------*/

PRIVATE void kernel (long prevlist)
/*
 * compute the kernel of the next item list
 * prevlist points to the previous list
 *
 * KERNEL[i] =
 *    {
 *       < N : alpha yygrammar[i] * beta, B,I,0 >
 *    |
 *       < N : alpha * yygrammar[i] beta, B,_,_ > is in IL[i-1] and has index I
 *    }
 */
{
   long i;
#if TRACE
   printf("kernel:\n");
#endif
   i = prevlist;
   while (dot[i]) {
      if (yygrammar[dot[i]] >= term_base ) {
         if (yygrammar[dot[i]] == sym) {
#if CHECKVIABLE
	    if (is_viable(dot[i]+1))
#endif
	    {
	       additem(dot[i]+1,back[i],i,0);
	    }
#if STATISTICS
	    GOOD++;
#endif
         }
	 else {
#if STATISTICS
	    BAD++;
#endif
	 }
      }
      i++;
   }
}

/*----------------------------------------------------------------------------*/

PRIVATE void predictor (long item)
/*
 * predictor step for item 'item'
 *
 * PREDICTOR:
 * The dot is before a nonterm
 * add the rules for that nonterm (with the dot at the beginning)
 *
 * If
 *    < N : alpha * M beta, B,L,S >
 * is in IL
 * then add
 *    < M : * gamma, B',0,0 >
 * if there is not yet an an item with the first two components
 * and there is a rule N : gamma
 * B' is a reference to IL[i]
 */
{
   long ruleptr;
  
   ruleptr = yygrammar[dot[item]];
#if TRACE
   printf("predictor for item %d:\n", item);
#endif
   do {
      int old = last_item;

#if ! LOOKAHEAD
      /* (1) ORIGINAL VERSION */
      additem(ruleptr+1,thislist,0,0);
      if ((back[last_item]==thislist) && (last_item>old)) specialitemadded=1;
#else
      /* (2) IMPROVEMENT
	 add test: is current symbol (lookaheadsym)
	 in director set of that rule ?
      */
         if (lookup_dirset(ruleptr)) {
            additem(ruleptr+1,thislist,0,0);
	    if ((back[last_item]==thislist) && (last_item>old))
	       specialitemadded=1;
	 }
	 else {
#if TRACE
            printf("rejected by lookup_dirset(%d)\n", ruleptr);
#endif
#if STATISTICS
	    rules_rej++;
#endif
	 }
#endif

      ruleptr=yygrammar[ruleptr];

   } while (ruleptr);
}

/*----------------------------------------------------------------------------*/

PRIVATE void completer (long item)
/*
 * completer step for item 'item'
 *
 * COMPLETER
 * The dot is at the end of a rule
 * add the item that triggered this rule to be included
 * where the dot is advanced after the nonterm
 *
 * If
 *    < M : gamma * , B,L,S >
 * is in CL (with index ITEM)
 * and there is an item
 *    < N : alpha * M beta, B',L',S' >
 * in the item list to which the back-pointer B refers (with index I)
 * then add
 *    < N : alpha M * beta, B',I,ITEM >
 * if there is not yet an item with the first two components
 */

{
   long lhs, old;
   register int i;
   int dot_i;

#if TRACE
	    printf("completer for item %d:\n", item);
#endif

   lhs=-yygrammar[dot[item]];
   i=back[item];

   /* loop over all items in earlier item list */
   dot[last_item+1] = 0; /* sentinel */
   while (dot_i = dot[i]) {

      if (yygrammar[/*dot[i]*/dot_i]==lhs) {

#if CHECKVIABLE
	 if (is_viable(/*dot[i]*/dot_i+1))
#endif
	 {
            old=last_item;
	    additem(/*dot[i]*/dot_i+1,back[i],i,item);
            dot[last_item+1] = 0; /* sentinel */
	    if ((back[i]==thislist) && (last_item>old)) specialitemadded=1;
	 }
      }

      i++;
   }
}

/*----------------------------------------------------------------------------*/

PRIVATE void closure ()
/*
 * compute closure for the kernel of the current item list
 *
 * CLOSURE
 * apply PREDICTOR and COMPLETOR
 * as long as there are changes
 */
{
   long i;
#if TRACE
   printf("closure\n");
#endif
   do {
      specialitemadded=0;
      i=thislist;
      while (i<=last_item) {
         if (yygrammar[dot[i]]<0) completer(i);
         else if (yygrammar[dot[i]]<term_base) predictor(i);
         i++;
      }
#if TRACE
      if (specialitemadded) printf("next closure iteration\n");
#endif
  } while (specialitemadded);
}

/*----------------------------------------------------------------------------*/

PRIVATE void initial_itemlist()
/*
 * compute initial item list
 * its kernel is given by the item
 *    YYSTART : * UserRoot EOF
 * for which the closure is computed
 *
 */
{

#if DYNAMICITEMS
   ITEMLIMIT = ITEMINCR;

   dot = (long *) malloc(ITEMLIMIT* sizeof(long));
   if (! dot) yymallocerror();
   back = (long *) malloc(ITEMLIMIT* sizeof(long));
   if (! back) yymallocerror();
   left = (long *) malloc(ITEMLIMIT* sizeof(long));
   if (! left) yymallocerror();
   sub = (long *) malloc(ITEMLIMIT* sizeof(long));
   if (! sub) yymallocerror();
#endif

#if STATISTICS
  itemlistcount = 0;
#endif
  thislist=1;
#if HASHING
  clearhash();
#endif
#if TRACE
  printf("initial itemlist:\n");
#endif
  additem(2,1,0,0); /*  YYSTART : * UserRoot EOF  */
  closure();
#if TRACE
  printf("end of initial itemlist\n");
#endif
  additem(0,0,0,0); /* terminator */
}

/*----------------------------------------------------------------------------*/

PRIVATE void next_itemlist()
/*
 * compute next item list:
 * kernel and closure
 */
{
   long prevlist;

#if STATISTICS
   itemlistcount++;
#endif

#if HASHING
   clearhash();
#endif

   prevlist=thislist;
   thislist=last_item+1;

#if BIGHASH
   BHITS[thislist] = -1;
#endif

   kernel(prevlist);
   if (last_item<thislist) syntaxerror();
   closure();
#if TRACE
   printf("end of itemlist\n");
#endif
   additem(0,0,0,0);
}

/*----------------------------------------------------------------------------*/

PRIVATE void itemlist_sequence()
/*
 * compute the sequence of item lists:
 * initial_itemlist
 * and then next_itemlist for each input token
 */
{
   last_item=0;
   initial_itemlist();
   do {
      readsym();
      next_itemlist();
   } while (sym != eofsym);
}

/*============================================================================*/
/* RETURN LEFTPARSE STEP BY STEP                                              */
/*============================================================================*/

/*
 * when the recognizer has terminated
 * leftpointers and sub pointers represent the parse tree
 * (starting with the item YYSTART : UserRoot EOF * )
 * the function 'yyselect' returns the rule number
 * one after each other in the order of a left derivation
 * as required by the tree walker implemented in yyactions.c
 * it uses a stack to keep track of items that need to be processed later
 */

#define STACKINCR 200
int STACKSIZE;
int *stack;
int stptr = 0;

void *x_realloc(void*,size_t,size_t,size_t);

/*----------------------------------------------------------------------------*/

PRIVATE void push(int n)
/*
 * push item index n onto the stack
 */
{
   if (stptr == STACKSIZE-2) {
      STACKSIZE += STACKINCR;
      stack = (int *) x_realloc(stack, 1, STACKSIZE,STACKSIZE-STACKINCR);
      if (! stack) yymallocerror();
   }
   stack[stptr++] = n;
}

/*----------------------------------------------------------------------------*/

PRIVATE int pop()
/*
 * pop an item index from the stack and return the value
 */
{
   stptr--;
   return stack[stptr];
}

/*----------------------------------------------------------------------------*/

PRIVATE void init_stack()
/*
 * push the initial item index on the stack
 * it represents the item
 *    YYSTART : UserRoot EOF *
 * i.e. the root of the derivation tree
 */
{
   STACKSIZE = STACKINCR;
   stack = (int *) malloc (sizeof(int) * STACKSIZE);
   if (! stack) yymallocerror();

   push(thislist);
}

/*----------------------------------------------------------------------------*/

PUBLIC int yyselect()
/*
 * return the next rule number (for the left-derivation)
 *
 * this function is called by the generated tree walker
 *
 * the stack always contains pointers to items that still must be processed
 * to produce a left-derivation
 * the top element must be processed first
 * if the dot appears inside a rule
 *    M : alpha N * beta
 * then the items representing the parse for beta are already on the stack
 * and the items representing alpha N must be pushed
 * they are represented by the sub pointer of the item
 * (corresponding to the parse for N)
 * and the leftpointer of the item
 * (corresponding to the parse for alpha)
 * if the item has the form
 *    M : gamma *
 * (i.e. the rule has been completed)
 * first the items representing gamma are pushed
 * then the rule number is returned
 * subsequent calls will process the items
 * representing gamma
 * 
 */
{
   int i;
   while (1) {
      i = pop();
      if (sub[i])  push(sub[i]);
      if (left[i]) push(left[i]);
      if (yygrammar[dot[i]] < 0 ) {
	    return yygrammar[dot[i]+1];
      }
   }
}

/*============================================================================*/
/* MAIN FUNCTION YYPARSE                                                      */
/*============================================================================*/

PUBLIC int yyparse()
/*
 * main function of the parser
 *
 * this function is called by the user's program
 *
 * compute the sequence of item lists
 * which implictely contain the parse tree
 * and then invokes the generated tree walker YYSTART
 * which in turn calls yyselect() to obtain the rule numbers
 * in the order of a left derivation
 */
{
#if BIGHASH
   init_DHITS();
#endif
   init_dirsets();
   lookaheadsym = yylex()+term_base;
   lookaheadpos = yypos;
   first_lexval();
   itemlist_sequence();

#if TRACE
   printf("END OF RECOGNIZER\n");
#endif

#if DYNAMICITEMS
   free(back);
#endif

#if PRINTTREE
   print_tree(left[thislist]);
#endif

#if WALK
   init_stack();
   init_lexelem();
   YYSTART();
#endif

#if DYNAMICITEMS
free(dot);
free(left);
free(sub);
#endif

#if STATISTICS
   print_statistics();
#endif

   return 0;
}

/*================================================================ THE END ===*/
#endif
