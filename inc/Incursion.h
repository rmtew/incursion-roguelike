/* INCURSION.H -- Copyright (c) 1999-2003 Julian Mensch
     This is the central C++ header file for Incursion, which
   includes in itself all the other second-tier header files. 
*/

#define SYSTEMNAME "Win32"
#define VISION
#define BYTECODE_TRACER

/* A member of the template class is not defined.
   http://msdn.microsoft.com/en-us/library/1k7k17za.aspx
*/
#pragma warning (disable : 4661)
/* posix names differ for c++ iso */
#pragma warning (disable : 4996)

//#define DEBUG_MEMORY_CORRUPTION
//#ifdef DEBUG_MEMORY_CORRUPTION
//#define INTENSIVE_MEMORY_DEBUG
//#endif

#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>




#ifdef DEBUG_MEMORY_CORRUPTION

void * _malloc(size_t sz);
void * _realloc(void *vp, size_t sz);
void   _free(void *vp);
char*  _incursion_strdup(const char *str);
long  array_index(long x,long y);


#define malloc  _malloc
#define free    _free
#define strdup  _incursion_strdup
#define realloc _realloc
#define calloc(a,b) _malloc((a)*(b))
#define I(x,y)  array_index(x,y)
#define array(a,i,s) array_access((void*)a,sizeof(a[0]),i,s)

#else

#define I(x,y) x
#define array(array,index,size) array[index]

#endif

#ifndef WIN32
#undef stricmp
#define stricmp strcasecmp
#endif


#include "Defines.h"
#include "Globals.h"
#include "Base.h"
#include "Target.h"
#include "RComp.h"
#include "Events.h"
#include "Magic.h"
#include "Res.h"
#include "Map.h"
#include "Creature.h"
#include "Item.h"
#include "Feature.h"
#include "Term.h"
#include "Inline.h"

