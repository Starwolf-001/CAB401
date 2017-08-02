/* header file for ifs files. */
/* defines the format for files, and for the in-memory */
/* image header */

/* The blocksize is machine dependent.  Routines such as ifsmkh will normally
   align different data structures (header, data, etc) on block boundaries.
   This is primarily intended to speed things up on disk accesses.  For UNIX
   the blocksize could be set to 1.  Future versions of the IFS routines may
   replace this 'define' with a global variable.  Then the user would be 
   able to change the blocksize whenever he desired.  The latter ability
   might be useful for writing routines to convert a file from the format
   used by one computer to that used by another.
*/

/* 10/29/01 Rajeev Ramanath, multiple modifications to remove ANSI C style function definition */ 

/* 09/22/08 Wesley Snyder, eliminated need for ifssex. All platform dependencies now handled */
/* at run time         */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef	__cplusplus
extern "C" {
#endif
#define BIGENDIAN 1
#define LITTLEENDIAN 2
#define IFS_IEEE 0x10
#ifndef IFS_H
#define IFS_H 1
#define BLOCKSIZE 512
#define IFS_SUCCESS -1
#define IFS_FAILURE 0
#define IFSMGC "IFS"
#define IFSVERSION 60       /* V 6.0 */



/* 256 bytes for IFSHDR version for in memory*/
typedef struct {
char  ifsmgc[4];        /* "magic no." to prove this header OK       */
int   ifsbb;	/* number of bytes/block always (set to BLOCKSIZE)   */
int   ifssoh;	/* block number of start of header, always = 0       */
int   ifssod;	/* block number of start of data,                    */
int   ifssot;	/* block number of start of tail                     */
int   ifstsz;	/* size of tail, in bytes.			     */
char *ifstail;	/* pointer to tail in core.			     */
char  ifsfc[8];	/* file class, not meaningful for unix               */
char  ifsct[8];	/* class type, not meaningful for unix	             */
char  ifsunm[32];	/* user name of creator	                     */
char  ifscdt[32];	/* creation date and time		     */
char  ifscpg[32];	/* id of creating program		     */
char  ifsver[8];	/* version of creating program		     */
char  ifsend;           /* little or big endian (see ifssex.h)       */
char  ifsfpu;           /* type of floating point processor          */
char  ifsrs1[38];	/* reserved for future use		     */
char  ifsdts[16];	/* units of data			     */
float ifsdsc;		/* scaling of data			     */
float ifsdof;		/* offset of data			     */
char  ifsdt[16];	/* data type                                 */
int   dtype;		/* A numeric equivalent to ifsdt, for speed  */
int   ifsbpd;		/* bytes / datum 		 	     */
int   ifsdims;		/* number of dimensions			     */
char * ifsptr; 		/* pointer to in core image array            */
int * ifsdln;           /* pointer to array of dimension lens (in    */
			/* core image only, 0 written to disk)       */
char * userptr;         /* for anything the user wants to use it for */
char   ifsrs3[3];	/* reserved for future use		     */
char   comp;		/* byte used to denote compression mode of data */
                        /* if zero  do not compress                 */
} IFSHDR ;
    /* 256 bytes for IFSHDR on disk. difference is only 32 bits for pointers*/
    typedef struct {
        char  ifsmgc[4];        /* "magic no." to prove this header OK       */
        int   ifsbb;	/* number of bytes/block always (set to BLOCKSIZE)   */
        int   ifssoh;	/* block number of start of header, always = 0       */
        int   ifssod;	/* block number of start of data,                    */
        int   ifssot;	/* block number of start of tail                     */
        int   ifstsz;	/* size of tail, in bytes.			     */
        int   ifstaildisk;	/* place holder pointer to tail.			     */
        char  ifsfc[8];	/* file class, not meaningful for unix               */
        char  ifsct[8];	/* class type, not meaningful for unix	             */
        char  ifsunm[32];	/* user name of creator	                     */
        char  ifscdt[32];	/* creation date and time		     */
        char  ifscpg[32];	/* id of creating program		     */
        char  ifsver[8];	/* version of creating program		     */
        char  ifsend;           /* little or big endian (see ifssex.h)       */
        char  ifsfpu;           /* type of floating point processor          */
        char  ifsrs1[38];	/* reserved for future use		     */
        char  ifsdts[16];	/* units of data			     */
        float ifsdsc;		/* scaling of data			     */
        float ifsdof;		/* offset of data			     */
        char  ifsdt[16];	/* data type                                 */
        int   dtype;		/* A numeric equivalent to ifsdt, for speed  */
        int   ifsbpd;		/* bytes / datum 		 	     */
        int   ifsdims;		/* number of dimensions			     */
        int   ifsptrdisk; 		/* place holder for pointer to  image array            */
        int  ifsdlndisk;           /* place holder for pointer to array of dimension lens (in    */
        /* core image only, 0 written to disk)       */
        int userptrdisk;         /* for anything the user wants to use it for */
        char   ifsrs3[3];	/* reserved for future use		     */
        char   comp;		/* byte used to denote compression mode of data */
        /* if zero  do not compress                 */
    } IFSHDRDisk ;

/* now, the definition of the structure which defines each	*/
/* dimension							*/

typedef struct {
int ifslen;		/* length of this dimension		*/
int ifsrnk;		/* rank of this dimension.  That is     */
			/* the rank defines the order in which  */
			/* each dimension is stored in memory	*/
			/* the dimension with rank = 1 changes  */
			/* the fastest.  In an image sequence,    */
			/* the "columns" dimension changes fastest*/
			/* and therefore has rank 1.  The rows   */
			/* dim has rank two, and the frames dim  */
			/* has rank 3.				*/
char ifsdir[8];		/* direction of this dimension.  strictly*/
			/* for physical interpretation, defines  */
			/* how this dimension is stored		*/
			/* possible options:			*/
			/* FORWARD
			   BACKWARD
			   FB-ALTER
			   BF-ALTER
			   only FORWARD is supported at this time*/
char ifsxun[8];		/* units for this dimension (typical, mils)*/
			/* or pixels				*/
float ifsxsc;		/* scaling for this dimension		*/
float ifsxof;		/* offset for this dimension		*/
char rs4[32];		/* reserved				*/
} IFSDIM ;


typedef IFSHDR * IFSIMG;


/*#ifdef VMS*/
#define bcopy(src,dest,len) memcpy((dest),(src),(len))
/*#endif*/

/* These def's no longer have use, but are defined for historical
 * reasons (easier to change def's here than edit all the source 
 * files using these names) */

#define ifsextn extern
#define ifsglbl

/* declare various functions et al: */

#ifndef IFSlinux
extern  int     errno;   /* Actually, this is system variable, not IFS */
#endif
ifsextn int	ifserr;  /* this should have been uppercase.  oops */
ifsextn int	IFSSLV;
ifsextn int	IFSCHK;
ifsextn int	IFSDEPTH;
ifsextn int	IFSTRACE;
ifsextn char *  (*ifs_alcptr)();
ifsextn char *  (*ifs_realcptr)();
ifsextn int     (*ifs_freeptr)();

int 	*	ifs_ao(IFSHDR *img);
char 	* 	ifsalc(int bytes);
int             ifscfree();
int 		ifschkhd(IFSHDR *hdr, char *text);
int		ifscheckNanInf(IFSHDR *);
void	ifscolormap(IFSIMG,IFSIMG,int);
	
IFSHDR  *   ifsConvertHdr(IFSHDR *hdr, IFSHDR *nhdr);
IFSHDR  *   ifsConvertDims(IFSHDR *hdr, IFSHDR *nhdr);
IFSHDR  *   ifsConvertData(IFSHDR *hdr, IFSHDR *nhdr);
IFSHDR 	*	ifscph(IFSHDR *hdr);

IFSHDR  * 	ifscreate(char *dt, int *dimlen, unsigned int flags, int structsize);
int			ifsdimen(IFSHDR *img, int n);
void		ifsext(int level, char*text);
IFSHDR  * 	ifsexwin(IFSHDR* img,int bottom, int left, int top, int right);
double		ifsfgp(IFSHDR* img,int row,int col);
int			ifsfpp(IFSHDR* img,int row,int col,double val);


IFSHDR	*	ifsfree(IFSHDR* img, unsigned int flags);
char	*	ifsEnv();
char	*	ifsGetFN(char *prompt, FILE* file);
IFSHDR	*	ifsGetHdr();
IFSHDR	*	ifsGetImg(char *FileName, char *Prompt, int SaveTail);
int 		ifsigp(IFSHDR* img,int row,int col);
int			ifsindex(int *dimlen, int *indices,int ndim);
int 		ifsipp(IFSHDR* img,int row,int col,int val);
int 	*	ifslen(IFSHDR *hdr, int *numptr);
int 	*	ifssiz(IFSHDR *hdr);
IFSHDR  *	ifsmkh(int nlines,int pixels,char* dtype);
int 		ifsnpx(IFSHDR* ptr);
FILE	*	ifsOpen(char *FileName, char *Mode, char *Prompt, int Retries);
IFSHDR  *	ifspin(char *FileName);
IFSHDR  *	ifsCVpin(char *FileName);
IFSIMG		ifsplotcontour(char *,float,int ,int ,int );

int 		ifspot(IFSHDR* Img,char *FileName);
int 		ifsCVpot(IFSHDR* Img,char *FileName);
int			ifsprh(FILE* dev, IFSHDR* hdr);
char	*	ifsPrsFN(char *name, int *rc);
int			ifsPutImg(IFSHDR* Image,char *FileName, char *Prompt, int SaveTail);
IFSHDR  *	ifsrdh();
IFSHDR	*	ifsRdHdr(FILE* file);
int			ifsRdData(IFSHDR* hdr, FILE* file);
IFSHDR	*	ifsRdDims(IFSHDR* hdr,FILE*  file);
IFSHDR	*	ifsRdImg(FILE* file, int keeptail);
char    *   ifsrealloc();
IFSHDR  *	ifsrho();
char	*	ifstim();
int			ifsversion();
int			ifsWrImg(IFSHDR* img,FILE* file,int SaveTail);
int			ifswarn(), ifsdie();

 /* begin brg ifs routines */
int         ifsnr(IFSHDR *ip);
int         ifsnc(IFSHDR *ip);
int         ifsnels(IFSHDR *ip);
int         ifsnbytes(IFSHDR *ip);
int         ifsndims(IFSHDR *ip);
 /* end brg ifs routines */

 /* begin hph routines */
int         ifscipp(IFSHDR* img,int row,int col,int val);
int         ifscigp(IFSHDR* img,int row,int col);
int         ifscipp3d(IFSHDR* img,int frame,int row,int col,int val);
int         ifscigp3d(IFSHDR * img, int frame, int row, int col);
int         ifscfpp(IFSHDR* img,int row,int col,double val);
int         ifscfpp3d(IFSHDR* img,int frame,int row,int col,double val);
double      ifscfgp(IFSHDR* img,int row,int col);
double      ifscfgp3d(IFSHDR * img, int frame, int row, int col);
int         ifsipp3d(IFSHDR* img,int frame,int row,int col,int val);
int         ifsigp3d(IFSHDR * img, int frame, int row, int col);
int         ifsfpp3d(IFSHDR* img,int frame,int row,int col,double val);
double      ifsfgp3d(IFSHDR * img, int frame, int row, int col);
double      gaussrand();
double      drand48();
IFSHDR	*	ifsexwin3d(IFSHDR * img, int lframe, int bottom, int left, int hframe, int top, int right);
IFSHDR  *   ifsslice(IFSHDR *img, char *text, int value);
/* end hph routines */
/* misc iptools */
	int HistogramSub(IFSIMG,int);
	int ifsadd(IFSIMG,IFSIMG,IFSIMG);
	int ifsany2any(IFSIMG,IFSIMG);
	int ifsc2mag(IFSIMG,IFSIMG);
	int ifsc2imag(IFSIMG,IFSIMG);
	int ifsc2phase(IFSIMG,IFSIMG);
	int ifsc2real(IFSIMG,IFSIMG);
	int CreateIFSDisplayWindow(IFSIMG,float,int, int);
	int WriteToIFSDisplayWindow(int ,IFSIMG ,int ,float ,int ,int );
	int ifsInsert2Dinto3D(IFSIMG ,IFSIMG ,int );
	int ifsmult(IFSIMG,IFSIMG,IFSIMG);
	int ifsrecip(IFSIMG,IFSIMG);
	int ifssub(IFSIMG,IFSIMG,IFSIMG);
	int ifsvidscale(IFSIMG,IFSIMG,float *,float *, int);
    int zoomupdown(IFSIMG, IFSIMG,int);
	IFSIMG ifsplotcontour(char *,float,int,int,int);
	int MinimumRegion(IFSIMG,IFSIMG,int);
	int ifscfft2d(IFSIMG,int);
/* end of iptools subroutines */
#define IFSERR ifserr

/* ifs macros */

#define ifsgetdim(img,i) ( (IFSDIM *) ( (char *) img + sizeof(IFSHDR) ) + i )

/* Following things are only for internal IFS use */
#ifdef IFSRTN
#      define	PUSH	  ++IFSDEPTH;
#      define	POP(val)  { --IFSDEPTH; return(val); }
#endif

/*
 *  Symbolic names for flag bits for various functions:
 */
/* for ifscreate:	*/
#define IFS_CR_HDR	1
#define IFS_CR_DATA	2
#define IFS_CR_TAIL	0	/* Not meaning at present */
#define IFS_CR_COMPRESS 8
#define IFS_CR_ALL	(IFS_CR_HDR | IFS_CR_DATA | IFS_CR_TAIL | IFS_CR_COMPRESS)

/* for ifsfree:		*/
#define IFS_FR_DATA	IFS_CR_DATA
#define IFS_FR_HDR	IFS_CR_HDR
#define IFS_FR_TAIL	4
#define IFS_FR_ALL	(IFS_FR_DATA | IFS_FR_HDR | IFS_FR_TAIL)

#ifdef OLDIFS

/* THESE ARE SUPERCEDED BY THE CODES IN ifserr.h !!!!!!!!!!!!!!!! */

/* QUIET:  don't display an error message, and don't abort    */
/* #define QUIET 0 */

/* WARN:   display an error message, but don't abort          */
/* #define WARN 100 */

/* FATAL:  display an error message and abort program         */
/* #define FATAL 200 */

typedef struct {
int   ifsbb;	/* number of bytes/block always (set to BLOCKSIZE)   */
int   ifssoh;	/* block number of start of header, always = 0       */
int   ifssod;	/* block number of start of data,                    */
int   ifssot;	/* block number of start of tail                     */
char  ifsfc[8];	/* file class, not meaningful for unix               */
char  ifsct[8];	/* class type, not meaningful for unix	             */
char  ifsunm[32];	/* user name of creator	                     */
char  ifscdt[32];	/* creation date and time		     */
char  ifscpg[32];	/* id of creating program		     */
char  ifsver[8];	/* version of creating program		     */
char  ifsmgc[4];        /* "magic no." to prove this header OK       */
char  ifsrs1[52];	/* reserved for future use		     */
char  ifsdts[8];	/* units of data			     */
float ifsdsc;		/* scaling of data			     */
float ifsdof;		/* offset of data			     */
char ifsrs2[16];	/* reserved for future use		     */
char ifsdt[8];		/* data type, character strings are	     */
			/* defined as follows:			     */
			/* int
			   float
			   complex	( two floats, real, imag)
			   byte		(signed byte)
			   ubyte	(unsigned byte)
			   short	(16 bit integer)
			   */
int ifsbpd;		/* bytes / datum 		 	     */
int ifsdims;		/* number of dimensions			     */
unsigned char * ifsptr; /* pointer to in core image array            */
int * ifsdln;           /* pointer to array of dimension lens (in    */
			/* core image only, 0 written to disk)       */
char * userptr;         /* for anything the user wants to use it for */
char ifsrs3[4];		/* reserved for future use		     */
} OLDIFSHDR;

#endif

#if 0
/* now, the definition of the structure for each dimension	*/
/* 64 bytes for each IFSDIM */
typedef struct {
int ifslen;		/* length of this dimension		*/
int ifsrnk;		/* rank of this dimension.  That is     */
			/* the rank defines the order in which  */
			/* each dimension is stored in memory	*/
			/* the dimension with rank = 1 changes  */
			/* the fastest.  In an image sequence,    */
			/* the "columns" dimension changes fastest*/
			/* and therefore has rank 1.  The rows   */
			/* dim has rank two, and the frames dim  */
			/* has rank 3.				*/
char ifsdir[8];		/* direction of this dimension.  strictly*/
			/* for physical interpretation, defines  */
			/* how this dimension is stored		*/
			/* possible options:			*/
			/* FORWARD
			   BACKWARD
			   FB-ALTER
			   BF-ALTER
			   only FORWARD is supported at this time*/
int dir;		/* Numeric encoding of ifsdir field	*/
char ifsxun[16];	/* units for this dimension (typical, mils)*/
			/* or pixels				*/
float ifsxsc;		/* scaling for this dimension		*/
float ifsxof;		/* offset for this dimension		*/
char rs4[20];		/* reserved				*/
} IFSDIM ;
#endif

#endif
#ifdef	__cplusplus
}
#endif
#define ifs_swap_word(a) ( ((a) << 24) | \
                      (((a) << 8) & 0x00ff0000) | \
                      (((a) >> 8) & 0x0000ff00) | \
        ((unsigned long)(a) >> 24) )

#define ifs_swap_half(a) ( ((a & 0xff) << 8) | ((unsigned short)(a) >> 8) )

