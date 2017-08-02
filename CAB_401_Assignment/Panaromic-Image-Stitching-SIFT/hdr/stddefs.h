/*********************************************************************\
*                                                                     *
*  hdr.h  -- Mark Lanzo's own flavor of header file.                  *
*                                                                     *
\*********************************************************************/

/*********************************************************************\
*                                                                     *
*  First things defined are symbolic names for various data types.    *
*  These are supposed to be independent of the machine used, as best  *
*  as possible.  Hence, change the define's when running on something *
*  other than Vaxen.                                                  *
*                                                                     *
\*********************************************************************/

#ifndef LANZO_HDR
#define LANZO_HDR 1

/* Ixx types are (signed) integer; xx = number of bits. */
typedef int		I32;
typedef short		I16;
typedef char		I8;

/* Uxx types are unsigned integer */
typedef unsigned int	U32;
typedef unsigned short	U16;
typedef unsigned char	U8;

typedef unsigned char   BYTE;

/* miscellany: */
typedef char * ADDRESS;
typedef char * STRING ;


/* the following definition is either "float" or "double", depending */
/* on which you prefer for floating point stuff.                     */
#define REAL   double

/********************************************************************\
*                                                                    *
* Useful definitions when defining variables and functions:          *
*                                                                    *
\********************************************************************/

/* for functions: */
#define MODULE  static
#define HIDDEN  static


/********************************************************************\
*                                                                    *
* Defines for logical operators and a few bit operations:            *
*                                                                    *
\********************************************************************/

/* I don't want to know how many times I've done "if (a=b)" when I  */
/* meant "if (a==b)"; even when I know better.                      */ 
#define is    ==

/* just to be consistent, I'll include the following , although I   */
/* rarely use 'em since "!=" isn't something I usually screw up.    */

#define isnt  !=
#define isnot !=
/* maybe I should define "aint" :-) */

/* Logical operators: */
#define and   &&
#define or    ||
#define not   !

/* useful macros for bit twiddling:  */
#define SetBits(word, bits) (word |= (bits))
#define ClearBits(word, bits) (word &= ~(bits))
#define SetBit(word, bits) (word |= (bits))
#define ClearBit(word, bits) (word &= ~(bits))


/**************************************************************************\
*                                                                          *
* Block structure stuff.  Any resemblence to pascal or fortran is purely   *
* a figment of your warped imagination!   :-)                              *
*                                                                          *
* These aren't intended to replace their correct C counterparts; but by    *
* judiciously mixing the lowercase and the uppercase forms I can increase  *
* visual cues when looking at printouts and checking to see that all the   *
* braces match up.  Keeping heavy duty logic and loop structures straight  *
* gets to be a real pain when the body of the structure occupies many      *
* pages; and tracking down the misplaced closing brace (which of course    *
* will be in some place which is still syntactically correct) gets to be   *
* even more fun.                                                           *
*                                                                          *
\**************************************************************************/

#define IF(c) if (c) {
#define ENDIF }
#define ELSE } else {
#define ELSEIF(x) } else if (x) {
#define WHILE(c) while(c) {
#define ENDWHILE }
#define ENDFOR }
#define BEGIN {
#define END   }

/* no #define FOR cause 'for' uses semicolons rather than commas, and the */
/* inconsistency of "FOR(x,y,z)" versus "for(x;y;z)" would be confusing.  */
/* Also run into a problem if you use comma operator in "for" statements. */
/* oh well...                                                             */

/*************************************************************************\
*                                                                         *
* common symbolic constants.  Since <stdio.h> and other files may         *
* already define some of these, I'll put each one in `ifdef's             *
*                                                                         *
\*************************************************************************/

#ifndef NULL
#define NULL 0
#endif

/* any non zero value will do for TRUE ... */
/* NOTE!: This will define FALSE also.  if TRUE was already defined, */
/* we're just assuming FALSE was also.                               */
#ifndef TRUE
#define TRUE -1
#define FALSE 0
#endif

/* similarly for ON/OFF                                              */
#ifndef ON
#define ON 1
#define OFF 0
#endif



/*******************************************************************\
*                                                                   *
* Symbolic names for a number of special character constants        *
*                                                                   *
\*******************************************************************/

#define NUL '\0'
#define SOH '\01'    /* ctrl-A     */
#define STX '\02'    /* ctrl-B     */
#define ETX '\03'    /* ctrl-C     */
#define EOT '\04'    /* ctrl-D     */
#define ENQ '\05'    /* ctrl-E     */
#define ACK '\06'    /* ctrl-F     */
#define BEL '\07'    /* ctrl-G     */
#define BS  '\10'    /* ctrl-H     */
#define BSP '\10'    /* ctrl-H     */
#define HT  '\t'     /* ctrl-I     */
#define TAB '\t'     /* ctrl-I     */
#define NL  '\n'     /* ctrl-J     */
#define LF  '\n'     /* ctrl-J,also*/
#define VT  '\v'     /* ctrl-K     */
#define FF  '\f'     /* ctrl-L     */
#define CR  '\r'     /* ctrl-M     */
#define SO  '\16'    /* ctrl-N     */
#define SI  '\17'    /* ctrl-O     */
#define DLE '\20'    /* ctrl-P     */
#define DC1 '\21'    /* ctrl-Q     */
#define DC2 '\22'    /* ctrl-R     */
#define DC3 '\23'    /* ctrl-S     */
#define DC4 '\24'    /* ctrl-T     */
#define NAK '\25'    /* ctrl-U     */
#define SYN '\26'    /* ctrl-V     */
#define ETB '\27'    /* ctrl-W     */
#define CAN '\30'    /* ctrl-X     */
#define EM  '\31'    /* ctrl-Y     */
#define SUB '\32'    /* ctrl-Z     */
#define ESC '\33'    /* ctrl-[ = ESCAPE = ALTMODE */
#define FS  '\34'    /* ctrl-\     */
#define GS  '\35'    /* ctrl-]     */
#define RS  '\36'    /* ctrl-^     */
#define US  '\37'    /* ctrl-_     */
#define DEL '\177'   /* ctrl-? = RUBOUT = DELETE (not BACKSPACE!) */
#define SP  ' '      /* spacebar   */
#define BSLASH '\\'  /* backslash  */

/****************************************************************************\
*                                                                            *
* A handy macro for displaying large quantities of text.  Invoke as follows: *
*                                                                            *
*     static char * message_to_output[] =                                    *
*              {                                                             *
*              "first line of text",                                         *
*              "second line of text",                                        *
*              "more lines of text go here...",                              *
*              "last line of text",                                          *
*              NULL                                                          *
*              } ;                                                           *
*                           (in body of program:)                            *
*     WriteText(file, message_to_output);                                    *
*                                                                            *
* where "file" is any open file pointer (FILE *) such as 'stdout' or 'stderr'*
*                                                                            *
\****************************************************************************/

#ifndef WriteText
#define WriteText(file,text)   \
        {        \
	int _ii;   \
	_ii = 0;   \
	while(text[_ii]) fprintf(file,"%s\n",text[_ii++]);   \
	}
#endif

/* a few more defines added by Katie Boone */
#ifndef AND
#define AND	&&
#endif
#ifndef EQ
#define EQ	==
#endif
#ifndef NE
#define NE	!=
#endif
#ifndef OR
#define OR	||
#endif
#ifndef GT
#define GT	>
#endif
#ifndef LT
#define LT	<
#endif
#ifndef GE
#define GE	>=
#endif
#ifndef LE
#define LE	<=
#endif
#ifndef BOOLEAN
#define BOOLEAN char
#endif
#ifndef MOD
#define MOD %
#endif

#endif
