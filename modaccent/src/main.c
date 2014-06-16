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

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;

char *SourceFileName = NULL;
char *cOutFileName = NULL;
char *hOutFileName = NULL;

void main (int argc, char **argv) {
    size_t outSize = 0;
    char cOutName[] = "yygram.cpp";
    char hOutName[] = "yygram.h";

    if (argc > 4) {
        printf("too many arguments\n");
        exit(1);
    }

    SourceFileName = NULL;

    if (argc >= 2) {
        size_t i;
        SourceFileName = argv[1];
        yyin = fopen (SourceFileName, "r");
        if (yyin == NULL) {
            printf("cannot open '%s'\n", SourceFileName);
            exit(1);
        }
        for (i = 0; i < strlen(SourceFileName); i++) {
            if (SourceFileName[i] == '\\')
                SourceFileName[i] = '/';
        }
    }

    if (argc >= 3) {
        size_t i, n = strlen(argv[2]) + strlen(cOutName) + 10;
        cOutFileName = (char *)alloca(n);
        strncpy(cOutFileName, argv[2], n);
        for (i = 0; i < n; i++) {
            if (cOutFileName [i] == '\\') /* Switch chars. */
                cOutFileName [i] = '/';
        }
        if (cOutFileName[strlen(argv[2])-1] != '/')
            strncat(cOutFileName, "/",1);
        strncat(cOutFileName, cOutName, strlen(cOutName));
    }

    if (argc >= 4) {
        size_t i, n = strlen(argv[3]) + strlen(hOutName) + 10;
        hOutFileName = (char *)alloca(n);
        strncpy(hOutFileName, argv[3], n);
        for (i = 0; i < n; i++) {
            if (hOutFileName [i] == '\\') /* Switch chars. */
                hOutFileName [i] = '/';
        }
        if (hOutFileName[strlen(argv[3])-1] != '/')
            strncat(hOutFileName, "/",1);
        strncat(hOutFileName, hOutName, strlen(hOutName));
    }

    ROOT();

    exit(0);
}

GetSourceFileName(ref_string)
    char **ref_string;
{
    *ref_string = SourceFileName;
}

GetDestCodeFileName(char **ref_string) {
    *ref_string = cOutFileName;
}

GetDestHeaderFileName(char **ref_string) {
    *ref_string = hOutFileName;
}
