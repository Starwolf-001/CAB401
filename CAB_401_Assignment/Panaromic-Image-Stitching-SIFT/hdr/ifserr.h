#ifndef IFSE_ERROR

/* Various errors which can occur, and which will be returned through
   the external variable IFSERR    */
#define IFSE_ERROR	 -1     /* Generic error code, when don't have 
				  something better to use */

#define IFSE_BAD_ARG	 010	/* Invalid argument, unspecified.   */
#define IFSE_NULL_HDR	 010
#define IFSE_NULL_HEADER 010	/* passed NULL as an image pointer. */
				/* this may or may not be regarded  */
				/* as an error, depending on the    */
				/* circumstance.		    */

#define IFSE_BAD_HDR	 030
#define IFSE_BAD_HEADER	 030	/* Bad magic number in header,
				  header presumed bad.  Note that TWO bits
				  are set in this error.  If you check for
				  (ifserr & IFSE_BAD_HEADER) you will 
				  also automatically pick up an 
				  IFSE_NULL_HEADER code too. NULL header is
				  sort of a special BAD header.  If you want
				  to treat NULL header as something other than
				  BAD header, then check for IFSE_NULL_HEADER
				  before checking for IFSE_BAD_HEADER */
				
#define IFSE_NO_OPEN	0100	/* Can't open file */
#define IFSE_NOT_IMAGE  0200	/* whatever we read from a file doesn't 
				   appear to be an image.  Not enough read to
				   bother even returning IFS_BAD_HEADER. */
#define IFSE_BAD_NAME   0400	/* Bad filename, error in parsing a name,
				   or similar.  Although this is included
				   under the IO error category, it may be
				   returned by any function which uses some
				   sort of name -- ie, not necessarily a bad
				   filename (although it normally is). */
#define IFSE_IO_ERR	0700	/* Generic I/O error */
				/* As with IFSE_NULL_HEADER and IFSE_BAD_HDR,
				   note that IFSE_NO_OPEN is a form of 
				   IFSE_IO_ERR */
				   
#define IFSE_NO_MEM	01000	/* Can't allocate memory */

#define IFSE_BAD_DTYPE	02000	/* Unrecognized data type (ifsmkh) */
#define IFSE_DATATYPE	02000	/* a synonym */


#define IFSE_BAD_POS	010000	/* Coordinates < 0 or > size of dimension */
#define IFSE_POSITION	010000

#define IFSE_WRONG_NDIM	020000	/* No. of dimensions of image is not right
				  for the requested function (such as
				  calling ifs_exwin on something other than
				  a two-d image. */

#define IFSE_NOT_SUPPORTED 01;

#define IFSE_CANNOT_COMPRESS 011 /* generic error code for failure of 
				   compression routines */
/* Error classes: */
#define IFS_QUIET	01
#define IFS_WARN	010
#define IFS_FATAL	0100

#endif
