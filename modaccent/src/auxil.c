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


POS_to_INT(pos, ref_number)
   long pos;
   long *ref_number;
{
   *ref_number = pos;
}

#define yyLCODE 1000L
#define yyFCODE 1000000000L

POS_to_LineNumber(pos, ref_number)
   long pos;
   long *ref_number;
{
   *ref_number = (pos % yyFCODE) / yyLCODE;
}
