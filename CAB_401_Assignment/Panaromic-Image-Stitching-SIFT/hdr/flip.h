#ifndef IFS_H
#include <ifs.h>
#include <ifstypes.h>
#endif
//#include <nicedefs.h>
// the nicedefs are included here explicitly without using a sharp include
/* some macro defs for convenience */

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
#ifndef SUCCESS
#define SUCCESS 1
#endif
#ifndef FAILURE
#define FAILURE 0
#endif

#define CHECKFLOAT(im) if(im->dtype != IFST_32FLT ) return(-1)
#define CHECKSIZE(l1,l2) { \
register int chligg; \
for (chligg = 1; chligg < (*l1 + 1); chligg++) \
if (*(l1 + chligg) != *(l2 + chligg)) return (-2); \
}
#ifdef _cplusplus
extern "C" {
#endif

int curv(IFSIMG,IFSIMG,float);
int flabsolute(IFSIMG,IFSIMG);
int fladds(IFSIMG,IFSIMG,float);
int fladdv(IFSIMG,IFSIMG,IFSIMG);
int flclip(IFSIMG,IFSIMG,float);
int flcurv(IFSIMG,IFSIMG);
int flcp(IFSIMG,IFSIMG);
int fldivs(IFSIMG,IFSIMG,float);
int fldivv(IFSIMG,IFSIMG,IFSIMG);
int flDoG(IFSIMG, IFSIMG, float, int, int);
int fldx(IFSIMG, IFSIMG);
int fldx_back(IFSIMG, IFSIMG);
int fldx_forw(IFSIMG, IFSIMG);
int fldxx(IFSIMG, IFSIMG);
int fldxy(IFSIMG, IFSIMG);
int fldxz(IFSIMG, IFSIMG);
int fldy(IFSIMG, IFSIMG);
int fldy_back(IFSIMG, IFSIMG);
int fldy_forw(IFSIMG, IFSIMG);
int fldyy(IFSIMG, IFSIMG);
int fldyz(IFSIMG, IFSIMG);
int fldz(IFSIMG, IFSIMG);
int fldzz(IFSIMG, IFSIMG);
int flexp(IFSIMG, IFSIMG);
int flGabor (IFSIMG , IFSIMG , float , float , float, float, float, int );
int flinv(IFSIMG, IFSIMG);
int flln(IFSIMG, IFSIMG);
int flLaplace(IFSIMG,IFSIMG,float);
int flvmag(IFSIMG,IFSIMG);
int flmorph(int,IFSIMG,IFSIMG,IFSIMG,int,int,float);
int flmults(IFSIMG, IFSIMG,float);
int flmultv(IFSIMG,IFSIMG,IFSIMG);
int flneg(IFSIMG, IFSIMG);
int flnorm(IFSIMG,float *);
int flone_border(IFSIMG, IFSIMG);
int flpad(IFSIMG, IFSIMG);
int flplanar(IFSIMG, IFSIMG);
int flrec(IFSIMG, IFSIMG);
int flrotate(IFSIMG, IFSIMG,float);

int flshx(IFSIMG,int,IFSIMG);
int flshxy(IFSIMG,int,int,IFSIMG);
int flshy(IFSIMG,int,IFSIMG);
int flsq(IFSIMG, IFSIMG);
int flsqrt(IFSIMG, IFSIMG);
int flsubs(IFSIMG, IFSIMG,float);
int flsubv(IFSIMG, IFSIMG,IFSIMG);
int flthresh(IFSIMG, IFSIMG,float,float);
int flthresh2(IFSIMG, IFSIMG,float,float,float);
int flzero_border(IFSIMG, IFSIMG);


double *dvector(long,long);
float *vector(long,long);
int *ivector(long,long);
int *ivector(long nl, long nh);
double *dvector(long nl, long nh);

double **dmatrix(long nrl, long nrh, long ncl, long nch);
float **matrix(long nrl, long nrh, long ncl, long nch);

void * malloc(size_t);
double ifsinverse(double **a, double **y,int n);
int ifsmatmult(float **x, float **y, float **z, int xrows,int xcols, int yrows, int ycols);
int ifsdmatmult(double **x, double **y, double **z, int xrows,int xcols, int yrows, int ycols);
void transpose(float **v,int rows,int cols,float **vtrans);
void dtranspose(double  **v,int rows,int cols,double  **vtrans);
void jacobi(float **,int,float *,float **,int *);
void printdmatrix(double **,int,int);

void free_ivector(int *v,long nl, long nh);
void free_vector(float *v,long nl, long nh);
void free_dvector(double *v, long nl, long nh);
void free_dmatrix(double **m, long nrl, long nrh, long ncl, long nch);
void free_matrix(float **m, long nrl, long nrh, long ncl, long nch);
#ifdef _cplusplus
 }
#endif

