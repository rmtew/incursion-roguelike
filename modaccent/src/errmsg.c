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
 * errmsg.c
 */


#define PRIVATE static

/*--------------------------------------------------------------------*/
/* Source Positions                                                   */
/*--------------------------------------------------------------------*/

#define yyLCODE 1000L

long yypos = (yyLCODE+1);

PRIVATE long yyLineCount = 1;

/*--------------------------------------------------------------------*/
void yyGetPos(ref_pos)
   long *ref_pos;
{
   *ref_pos = yypos-1;
}

/*--------------------------------------------------------------------*/

void yyPosToNextLine()
{
   yyLineCount++;
   yypos = yyLineCount*yyLCODE+1;
}

/*--------------------------------------------------------------------*/

PRIVATE  yyLineAtPos(pos)
   long pos;
{
   long l;
   l = pos / yyLCODE;
   return l;
}

/*--------------------------------------------------------------------*/

PRIVATE yyColAtPos(pos)
   long pos;
{
   long c;
   c = pos % yyLCODE;
   return c;
}

/*--------------------------------------------------------------------*/
/* Error Messages                                                     */
/*--------------------------------------------------------------------*/

void Error(msg, pos)
   char *msg;
   long pos;
{
   printf("line %d, col %d: %s\n",
      yyLineAtPos(pos), yyColAtPos(pos), msg);
   exit(1);
}

/*--------------------------------------------------------------------*/

yyerror(msg)
   char *msg;
{
   long pos;

   yyGetPos(& pos);
   Error(msg, pos);
}

/*--------------------------------------------------------------------*/
