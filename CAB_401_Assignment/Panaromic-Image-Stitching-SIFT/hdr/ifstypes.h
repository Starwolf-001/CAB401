/*
 * types.h:  define a structure used for setting data type.
 */
 
typedef struct
    {
    char * name;	/* The name for this data type. 	*/
    int    size;	/* The 'sizeof' this data type.		*/
    int    dtype;	/* Numeric equivalent to 'name' field   */
			/* If 'dtype == 0', this is a pseudonym,*/
			/* must look further down chain to find */
			/* what the real name, size, etc. are.	*/
    }
    IFS_TYPE ;


#define IFSTI		0x100	/* some form of signed integer   */
#define IFSTF		0x200	/* some form of float/double     */
#define IFSTU		0x400	/* some form of unsigned integer */
#define IFSTC		0x1000  /* a complex number.		 */
#define IFSTS		0x2000  /* user defined structure	 */

/* When specifying IFSTC, you should also specify one of IFSTI/IFSTF/IFSTU */
/* to indicate what format each piece of the complex number is in. Only    */
/* complex numbers with float/double are currently understood though.      */

/* lack of one of the size attributes means default machine size ? */
#define IFST8		0x001	
#define IFST16		0x002	
#define IFST32		0x004
#define IFST64		0x008

#define IFST_8BIT	(IFST8 | IFSTI)
#define IFST_BYTE 	IFST_8BIT
#define IFST_CHAR	IFST_8BIT

#define IFST_U8BIT	(IFST8 | IFSTU)
#define IFST_UBYTE 	IFST_U8BIT
#define IFST_UCHAR	IFST_U8BIT

#define IFST_16BIT	(IFST16 | IFSTI)
#define IFST_U16BIT	(IFST16 | IFSTU)

#define IFST_32BIT	(IFST32 | IFSTI)
/*
Phasing unsigned integer out!!
#define IFST_U32BIT	(IFST32 | IFSTU)
*/

#define IFST_32FLT	(IFST32 | IFSTF)
#define IFST_64FLT	(IFST64 | IFSTF)

/* A '32 complex' is TWO 32 bit floats, similarly for '64 complex' ... */
#define IFST_32CMP	(IFST32 | IFSTF | IFSTC)
#define IFST_64CMP	(IFST64 | IFSTF | IFSTC)

#define IFST_STRUCT	IFSTS

/* The following are machine dependent synonyms */
#define IFST_INT	IFST_32BIT
/* phasing unsigned integer out!
#define IFST_UINT	IFST_U32BIT
*/
#define IFST_LONG	IFST_32BIT
/* phasing unsigned int out!
#define IFST_ULONG	IFST_U32BIT
*/
#define IFST_FLOAT	IFST_32FLT
#define IFST_DOUBLE	IFST_64FLT
#define IFST_SHORT	IFST_16BIT
#define IFST_USHORT	IFST_U16BIT

/* Names for later typecasting (more machine dependent stuff): */

typedef char 		IFSD_8BIT;
typedef unsigned char	IFSD_U8BIT;
typedef short		IFSD_16BIT;
typedef unsigned short	IFSD_U16BIT;
typedef int		IFSD_32BIT;
typedef unsigned int	IFSD_U32BIT;
typedef float		IFSD_32FLT;
typedef double		IFSD_64FLT;
typedef struct
    {
    IFSD_32FLT	real;
    IFSD_32FLT	imag;
    } 			IFSD_32CMP;
typedef struct
    {
    IFSD_64FLT	real;
    IFSD_64FLT	imag;
    }			IFSD_64CMP;


/* Refer to the source file "types.c" to find all the actual names IFS 	*/
/* uses for data types (including synonyms).				*/
