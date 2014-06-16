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


#include <stdio.h>

extern FILE *yyin;

char *SourceFileName;

main (argc, argv)
   int argc;
   char **argv;

{
   if (argc > 2) {
      printf("too many arguments\n");
      exit(1);
   }

   if (argc == 2) {
      SourceFileName = argv[1];
      yyin = fopen (SourceFileName, "r");
      if (yyin == NULL) {
	 printf("cannot open '%s'\n", SourceFileName);
	 exit(1);
      }
   }

   ROOT();

   exit(0);
}

GetSourceFileName(ref_string)
   char **ref_string;
{
   *ref_string = SourceFileName;
}
