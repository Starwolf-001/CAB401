/*
 *  resample.h
 *  Geodesic
 *
 *  Created by Wesley Snyder on 9/21/07.
 *  Copyright 2007 __University of Western Australia__. All rights reserved.
 *
 */
struct rs 
{	
	double *rho;		/* array of distances between point pairs */
	double *t;		/* arc length in the new shape*/
	double *s;		/* arc length in the old shape*/
	int *k;			/* index */
};
