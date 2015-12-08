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
    char tag[100];
    double *observations; //an array of observations
} birdBuffer;

void    createBuffer (birdBuffer *buffer, char *tag, int max);
void    addToBuffer (birdBuffer *buffer, double obs);
double  getBufferObs (birdBuffer *buffer, int idx); //get the observation at index 'idx' where idx = 0 is the oldest and idx = ( n - 1 ) is the most recent
double  getBufferRecentObs (birdBuffer *buffer, int idx); //get the observation at index 'idx' where idx = 0 is the most recent and ( n - 1 ) is the newest
double  getBufferQuartile (birdBuffer *buffer, int q);
double  getBufferMin(birdBuffer *buffer);
double  getBufferMax (birdBuffer *buffer);
double  getBufferMean (birdBuffer *buffer);
double  getBufferWeightedMean (birdBuffer *buffer);
double  getBufferMedian (birdBuffer *buffer);
double  getBufferSSE (birdBuffer *buffer);
double  getBufferMSE (birdBuffer *buffer);
double  getBufferSD (birdBuffer *buffer);
void    setBuffer (birdBuffer *buffer, double setToValue);
void    setBufferObs (birdBuffer *buffer, double value, int idx); //set the value of the observation at index 'idx' to specified value, where idx = 0 is the oldest and idx = ( n - 1 ) is the most recent
void    zeroBuffer (birdBuffer *buffer);
void    destroyBuffer (birdBuffer *buffer);
void    printBufferSummary (birdBuffer *buffer);
double  getPDF(double value, double mu, double sigma);
double  getRandomNormal(double mu, double sigma);
double  getBufferPDF(birdBuffer *buffer, double value);
#endif /* birdBuffer_h */
