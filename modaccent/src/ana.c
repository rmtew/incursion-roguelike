/*
 * ACCENT 
 * 
 * A Compiler Compiler for the Entire Class of Context-Free Languages
 * 
 * Copyright (C) 1999  Friedrich Wilhelm Schroeer
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


/*
 * ana.c
 */

/*----------------------------------------------------------------------------*/

#include "sets.h"

#define PUBLIC
#define PRIVATE static

/*----------------------------------------------------------------------------*/

PRIVATE void compute_transparent();
PRIVATE void compute_first ();
PRIVATE void compute_follow ();
PRIVATE void compute_dir ();
PRIVATE void process_rhs();

/*----------------------------------------------------------------------------*/
/* Grammar Definition                                                         */
/*----------------------------------------------------------------------------*/

#define max_char 255

typedef struct MEMBERLIST *memberlist;
typedef struct RULELIST *rulelist;

typedef int member;

struct MEMBERLIST {
   member head;
   memberlist tail;
};

struct RULELIST {
   memberlist head;
   rulelist tail;
};

PRIVATE int n_of_nonterms = 0;
PRIVATE rulelist cur_rule_list = 0;
PRIVATE rulelist last_rule_list_ptr;
PRIVATE memberlist last_member_ptr;
PRIVATE int rulecount = 0;

/*----------------------------------------------------------------------------*/

PRIVATE mallocerror()
{
   printf("running out of memory\n");
   exit(1);
}

/*----------------------------------------------------------------------------*/

PUBLIC get_rulecount(ref_n)
   int *ref_n;
{
   *ref_n = rulecount;
}

/*----------------------------------------------------------------------------*/

PUBLIC start_rule(n, ref_r)
   int n;
   int *ref_r;

{
   rulelist lst;
   memberlist r;

   if (n > n_of_nonterms) n_of_nonterms = n;

   r = (memberlist) malloc (sizeof(struct MEMBERLIST));
   if (! r) mallocerror();
   last_member_ptr = r;

   r->head = n;
   r->tail = 0;

   lst = (rulelist) malloc (sizeof (struct RULELIST));
   if (! lst) mallocerror();
   lst->head = r;
   lst->tail = 0;

   if (cur_rule_list == 0) {
      cur_rule_list = lst;
      last_rule_list_ptr = lst;
   }
   else {
      last_rule_list_ptr->tail = lst;
      last_rule_list_ptr = lst;
   }

   rulecount++;

   *ref_r = rulecount;
}

/*----------------------------------------------------------------------------*/

PRIVATE append_member(n)
{
   memberlist lst;

   lst = (memberlist) malloc (sizeof (struct MEMBERLIST));
   if (! lst) mallocerror();
   lst->tail = 0;
   lst->head = n;

   last_member_ptr->tail = lst;
   last_member_ptr = lst;
}

PUBLIC append_nonterm_member(n)
{
   append_member(n);
}

PUBLIC append_token_member(n)
{
   append_member(- n);
}

/*----------------------------------------------------------------------------*/

PRIVATE print_grammar()
{
   rulelist r;
   memberlist m;

   r = cur_rule_list;
   while (r) {
      m = r->head;
      r = r-> tail;

      printf("rule\n");
      printf("lhs = %d\n", m->head);
      m = m->tail;

      while(m) {
	 printf("member = %d\n", m->head);
	 m = m->tail;
      }
   }
}

/*----------------------------------------------------------------------------*/
/* LL Analysis                                                                */
/*----------------------------------------------------------------------------*/


PRIVATE int *TRANSPARENT;
PRIVATE set *FIRST;
PRIVATE set *FOLLOW;
PRIVATE set *DIRSET;

PRIVATE allocate_arrays()
{
   TRANSPARENT = (int *) malloc (sizeof(int)*n_of_nonterms+1);
   if (! TRANSPARENT) mallocerror();
   FIRST = (set *) malloc (sizeof(set)*n_of_nonterms+1);
   if (! FIRST) mallocerror();
   FOLLOW = (set *) malloc (sizeof(set)*n_of_nonterms+1);
   if (! FOLLOW) mallocerror();
   DIRSET = (set *) malloc (sizeof(set)*rulecount+1);
   if (! DIRSET) mallocerror();
}

PRIVATE set right_context;
PRIVATE int true_change;

/*----------------------------------------------------------------------------*/

PUBLIC init_ana()
{
   ;
}

/*----------------------------------------------------------------------------*/

PUBLIC run_ana()
{
   allocate_arrays();

   compute_transparent();
   compute_first();
   compute_follow();
   compute_dir();
}


/*----------------------------------------------------------------------------*/

PRIVATE void compute_transparent()
{
   int i;

   for (i = 1; i <= n_of_nonterms; i++) {
      TRANSPARENT[i] = 0;
   }

   do {
      rulelist rl;
      memberlist ml;
      member lhs;

      changed = 0;

      /* for all rules */
      rl = cur_rule_list;
      while (rl) {
	 ml = rl->head;
	 rl = rl->tail;
	 lhs = ml->head;

	 if (! TRANSPARENT[lhs]) {
	    if (ml-> tail == 0) { /* empty rhs */
	       TRANSPARENT[lhs] = 1;
	       changed = 1;
	    }
	    else {
	       /* for all members of rhs */
	       ml = ml->tail;
	       while (ml) {
		  member m;
		  int is_still_trans = 1;

		  m = ml->head;
		  if (m <= 0) { /* m is token */
		     is_still_trans = 0;
		     break;
		  }
		  if (TRANSPARENT[m]) {
		     ; /* continue with rhs */
		  }
		  else {
		     is_still_trans = 0;
		     break;
		  }

		  if (is_still_trans) {
		     TRANSPARENT[lhs] = 1;
		     changed = 1;
		  }

		  ml = ml->tail;
	       }
	    }
	 }
      }

   } while(changed);

}

/*----------------------------------------------------------------------------*/

PRIVATE void compute_first ()
{
   int i;

   /* for all nonterms FIRST = empty_set */

   for (i = 1; i <= n_of_nonterms; i++) {
      FIRST[i] = empty_set();
   }

   do {
      rulelist rl;
      memberlist ml;
      member lhs, m;

      changed = 0;
      
      /* for all rules */
      rl = cur_rule_list;
      while (rl) {
	 ml = rl->head;
	 rl = rl->tail;

	 lhs = ml->head;
	 ml = ml->tail;
	 while(ml) {
	    m = ml->head;
	    if (m <= 0) { /* m is a token */
	       into_set_include_elem(&FIRST[lhs], - m);
	       break;
	    }
	    into_set_include_set(&FIRST[lhs], FIRST[m]);
	    if (TRANSPARENT[m]) {
	       ; /* continue */
	    }
	    else {
	       break;
	    }

	    ml = ml->tail;
	 }
      }
   } while (changed);

}

/*----------------------------------------------------------------------------*/

PRIVATE void compute_follow ()
{
   int i;

   /* for all nonterms FOLLOW = empty_set */

   for (i = 1; i <= n_of_nonterms; i++) {
      FOLLOW[i] = empty_set();
   }

   do {
      rulelist rl;
      memberlist ml;
      member lhs;
      memberlist rhs;

      true_change = 0;
      
      /* for all rules */
      rl = cur_rule_list;
      while (rl) {
	 ml = rl->head;
	 rl = rl->tail;

	 lhs = ml->head;
	 rhs = ml->tail;
	 right_context = empty_set();
	 into_set_include_set(&right_context, FOLLOW[lhs]);
	 process_rhs(rhs);

      }
   } while (true_change);

}

/*----------------------------------------------------------------------------*/

/* called by compute_follow */

PRIVATE void process_rhs(ml)
   memberlist ml;
{
   member m;

   if (ml == 0) return;

   process_rhs(ml->tail);
   m = ml->head;

   if (m <= 0) { /* m is a token */
      right_context = empty_set();
      into_set_include_elem(&right_context, - m);
   }
   else { /* m is a nonterm */
      changed = 0;
      into_set_include_set(&FOLLOW[m], right_context);
      if (changed) true_change = 1;

      if (TRANSPARENT[m]) {
	 into_set_include_set(&right_context, FIRST[m]);
      }
      else {
	 right_context = empty_set();
	 into_set_include_set(&right_context, FIRST[m]);
      }
   }
}

/*----------------------------------------------------------------------------*/

PRIVATE void compute_dir ()
{
   rulelist rl;
   memberlist ml;
   member lhs;
   member m;

   set cur;
   int still_transparent;
   int ruleindex;

   rl = cur_rule_list;
   ruleindex = 0;
   while (rl) {
      ml = rl->head;
      rl = rl->tail;

      cur = empty_set();

      still_transparent = 1;

      lhs = ml->head;
      ml = ml->tail;
      while (ml) {
	 m = ml->head;
	 ml = ml->tail;

	 if (m <= 0) { /* m is a token */
	    into_set_include_elem(&cur, - m);
            ruleindex++;
            DIRSET[ruleindex] = cur;
	    still_transparent = 0;
	    break;
	 }

	 into_set_include_set(&cur, FIRST[m]);
	 if (TRANSPARENT[m]) {
	    ; /* continue */
	 }
	 else {
            ruleindex++;
            DIRSET[ruleindex] = cur;
	    still_transparent = 0;
	    break;
	 }
      }
      if (still_transparent) {
	 into_set_include_set(&cur, FOLLOW[lhs]);
         ruleindex++;
         DIRSET[ruleindex] = cur;
      }

   }
}

/*----------------------------------------------------------------------------*/

PUBLIC get_dirset(n, ref_s)
   int n;
   set *ref_s;
{
   *ref_s = DIRSET[n];
}

/*----------------------------------------------------------------------------*/

PUBLIC get_transparent(n, ref_val)
   int n;
   int *ref_val;
{
   *ref_val = TRANSPARENT[n];
}
/*----------------------------------------------------------------------------*/

PUBLIC get_max_char(ref_n)
   int *ref_n;
{
   *ref_n = max_char;
}

/*----------------------------------------------------------------------------*/
