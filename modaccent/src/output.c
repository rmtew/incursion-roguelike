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


#define PRIVATE static

#include <stdio.h>

/*--------------------------------------------------------------------*/

#define OutBufSize 6000
#define FlushPos 5000

/*--------------------------------------------------------------------*/

PRIVATE long OutputLineCount = 0;

PRIVATE char OutBuf[OutBufSize];
PRIVATE char *OutBufPtr;
PRIVATE FILE *OutFile;

/*--------------------------------------------------------------------*/

OpenOutput (Name)
   char *Name;
{
   OutFile = fopen(Name, "w");

   if (OutFile == NULL) {
      printf("cannot open %s\n", Name); exit(1);
   }
   OutBufPtr = &OutBuf[0];
   OutputLineCount = 0;
   return 1;
}

/*--------------------------------------------------------------------*/
CloseOutput ()
{
   fwrite(OutBuf, 1, OutBufPtr - &OutBuf[0], OutFile);
   fclose(OutFile);
}

/*--------------------------------------------------------------------*/
Put(Str)
   char *Str;
{
   while(*Str) *OutBufPtr++ = *Str++; 
}

/*--------------------------------------------------------------------*/
PutBlockText(Str)
   char *Str;
{
   while(*Str) {
      if (*Str == '\n') {
	 Nl();
	 Str++;
      }
      else
         *OutBufPtr++ = *Str++; 
   }
}

/*--------------------------------------------------------------------*/
PutI (N)
   long N;
{
   if (N < 0) {
      *OutBufPtr++ = '-';
      N = -N;
   }
   if (N < 10) {
      *OutBufPtr++ = N + '0';
   }
   else if (N < 100) {
      *OutBufPtr++ = N/10 + '0';
      *OutBufPtr++ = N%10 + '0';
   }
   else {
      char buf[50];
      register i;
      register n = N;

      i = 0;

      do {
	 buf[++i] = (n % 10) + '0';
	 n = n / 10;
      } while (n);

      while (i)
	 *OutBufPtr++ = buf[i--];
   }
}

/*--------------------------------------------------------------------*/
Nl ()
{
   if (OutBufPtr > &OutBuf[FlushPos]) {
      fwrite(OutBuf, 1, OutBufPtr - &OutBuf[0], OutFile);
      OutBufPtr = &OutBuf[0];
   }

#ifdef EMIT_CR
   *OutBufPtr++ = '\r';
#endif
   *OutBufPtr++ = '\n';
   OutputLineCount++;
}

/*--------------------------------------------------------------------*/

GetOutputLineCount(ref_n)
   int * ref_n;
{
   *ref_n = OutputLineCount;
}

/*--------------------------------------------------------------------*/
