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
    int n; //current number of observations
    int i; //index at which the next observation will be added
    int max; //maximum number of observations
    char tag[100]; //identifier string
    double *observations; //an array of observations
} birdBuffer;

void    createBuffer (birdBuffer *buffer, char *tag, int max);
void    addToBuffer (birdBuffer *buffer, double obs);
void    setBuffer (birdBuffer *buffer, double setToValue);
void    zeroBuffer (birdBuffer *buffer);
void    destroyBuffer (birdBuffer *buffer);

double  getBufferObs (birdBuffer *buffer, int idx); //get the observation at index 'idx' where idx = 0 is the oldest and idx = ( n - 1 ) is the most recent
double  getBufferRecentObs (birdBuffer *buffer, int idx); //get the observation at index 'idx' where idx = 0 is the most recent and ( n - 1 ) is the newest
void    printBuffer (birdBuffer *buffer);



#endif /* birdBuffer_h */
