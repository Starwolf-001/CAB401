/*
 *  ifsmatrix.h
 *  
 *
 *  Created by Wesley Snyder on 11/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
double ifsinverse(double **, double **,int );
void printmatrix(double **,int ,int );
int *ivector(long nl,long );
double *dvector(long ,long );
void free_ivector(int *,long , long );
void free_vector(float *,long , long );
void free_dmatrix(double **, long , long , long , long );
double **dmatrix(long , long , long , long );
int ifsmatmult(double **, double **, double **, int x,int , int , int );
void transpose(double **,int ,int ,double **);
#define NR_END

