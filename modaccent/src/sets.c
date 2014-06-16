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
 * sets.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "sets.h"

#define PUBLIC
#define PRIVATE static

struct SET {
   int head;
   set tail;
};

PUBLIC int changed;

PUBLIC void print_set(s)
   set s;
{
   set p;

   p = s;
   printf("{");
   while (p) {
      printf(" %d", p->head);
      p = p->tail;
   }
   printf(" }");
}

PUBLIC void emit_set(s)
   set s;
{
   set p;

   p = s;
   Put("{");
   while (p) {
      extern TokenNumberToString();
      char *str;
      Put(" ");
      TokenNumberToString(p->head, &str);
      Put("'");
      Put(str);
      Put("'");
      p = p->tail;
   }
   Put(" }");
}

PUBLIC void write_set(n, s)
   int n;
   set s;
{
   set p;

   p = s;
   while (p) {
      Put("TABLE[");
      PutI(n);
      Put("][");
      PutI(p->head);
      Put("] = 1;");
      Nl();
      p = p->tail;
   }
}

PUBLIC set empty_set ()
{
   return 0;
}

PUBLIC void into_set_include_elem (ref_s, e)
   set *ref_s;
   int e;
{
   set p;

   /*
   print_set(*ref_s);
   printf(" + %d\n", e);
   */

   p = *ref_s;
   while(p) {
      if (p->head == e) return;
      p = p->tail;
   }

   p = (set) malloc (sizeof (struct SET));
   if (! p) {
      printf("running out of memory\n");
      exit(1);
   }
   p->head = e;
   p->tail = *ref_s;
   *ref_s = p;

   changed = 1;
}

PUBLIC void into_set_include_set (ref_s, x)
   set *ref_s, x;
{
   set p;
   p = x;
   while(p) {
      into_set_include_elem(ref_s, p->head);
      p = p->tail;
   }
}
