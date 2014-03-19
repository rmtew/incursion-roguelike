/*
 *				C P P 3 . C
 *
 *		    File open and command line options
 *
 * Edit history
 * 13-Nov-84	MM	Split from cpp1.c
 */
#undef DEBUG
#include	<stdio.h>
#include	<ctype.h>
#include	"cppdef.h"
#include	"cpp.h"
#if DEBUG && (HOST == SYS_VMS || HOST == SYS_UNIX)
#include	<signal.h>
extern int	abort();		/* For debugging		*/
#endif

int
openfile(filename)
char		*filename;
/*
 * Open a file, add it to the linked list of open files.
 * This is called only from openfile() above.
 */
{
	register FILE		*fp;

	if ((fp = fopen(filename, "r")) == NULL) {
#if DEBUG
	    perror(filename);
#endif
	    return (FALSE);
	}
	addfile(fp, filename);
	return (TRUE);
}

addfile(fp, filename)
FILE		*fp;			/* Open file pointer		*/
char		*filename;		/* Name of the file		*/
/*
 * Initialize tables for this open file.  This is called from openfile()
 * above (for #include files), and from the entry to cpp to open the main
 * input file.  It calls a common routine, getfile() to build the FILEINFO
 * structure which is used to read characters.  (getfile() is also called
 * to setup a macro replacement.)
 */
{
	register FILEINFO	*file;
	extern FILEINFO		*getfile();

	file = getfile(NBUFF, filename);
	file->fp = fp;			/* Better remember FILE *	*/
	file->buffer[0] = EOS;		/* Initialize for first read	*/
	c_line = 1;			/* Working on line 1 now	*/
	wrongline = TRUE;		/* Force out initial #line	*/
}

setincdirs()
/*
 * Append system-specific directories to the include directory list.
 * Called only when cpp is started.
 */
{
  #ifdef WIN32
	*incend++ = "..\\inc";
  *incend++ = "..\\lib";
	*incend++ = ".\\inc";
  *incend++ = ".\\lib";
  #else
  *incend++ = "../inc";
  *incend++ = "../lib";
  *incend++ = "./inc";
  *incend++ = "./lib";
  #endif
}


initdefines()
/*
 * Initialize the built-in #define's.  There are two flavors:
 * 	#define decus	1		(static definitions)
 *	#define	__FILE__ ??		(dynamic, evaluated by magic)
 * Called only on cpp startup.
 *
 * Note: the built-in static definitions are supressed by the -N option.
 * __LINE__, __FILE__, and __DATE__ are always present.
 */
{
	register char		**pp;
	register char		*tp;
	register DEFBUF		*dp;
	int			i;
	long			tvec;
	extern char		*ctime();

	/*
	 * Predefine the built-in symbols.  Allow the
	 * implementor to pre-define a symbol as "" to
	 * eliminate it.
	 */
	if (nflag == 0) {
	    for (pp = preset; *pp != NULL; pp++) {
		if (*pp[0] != EOS) {
		    dp = defendel(*pp, FALSE);
		    dp->repl = savestring("1");
		    dp->nargs = DEF_NOARGS;
		}
	    }
	}
	/*
	 * The magic pre-defines (__FILE__ and __LINE__ are
	 * initialized with negative argument counts.  expand()
	 * notices this and calls the appropriate routine.
	 * DEF_NOARGS is one greater than the first "magic" definition.
	 */
	if (nflag < 2) {
	    for (pp = magic, i = DEF_NOARGS; *pp != NULL; pp++) {
		dp = defendel(*pp, FALSE);
		dp->nargs = --i;
	    }
#if OK_DATE
	    /*
	     * Define __DATE__ as today's date.
	     */
	    dp = defendel("__DATE__", FALSE);
	    dp->repl = tp = getmem(27);
	    dp->nargs = DEF_NOARGS;
	    time(&tvec);
	    *tp++ = '"';
	    strcpy(tp, ctime(&tvec));
	    tp[24] = '"';			/* Overwrite newline	*/
#endif
	}
}


