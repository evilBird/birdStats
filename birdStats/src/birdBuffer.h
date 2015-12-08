//
//  birdBuffer.h
//  birdStats
//
//  Created by Travis Henspeter on 12/8/15.
//  Copyright © 2015 birdSound, Inc. All rights reserved.
//

#ifndef birdBuffer_h
#define birdBuffer_h

#include <stdio.h>

typedef struct {
    int n;
    int i;
    int max;
    double *observations;
} birdBuffer;

void    createBuffer (birdBuffer *buffer, int max);
void    addToBuffer (birdBuffer *buffer, double obs);
double  getBufferMin(birdBuffer *buffer);
double  getBufferMax (birdBuffer *buffer);
double  getBufferMean (birdBuffer *buffer);
double  getBufferMedian (birdBuffer *buffer);
void    destroyBuffer (birdBuffer *buffer);

#endif /* birdBuffer_h */
