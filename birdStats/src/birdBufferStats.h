//
//  birdBufferStats.h
//  birdStats
//
//  Created by Travis Henspeter on 12/9/15.
//  Copyright © 2015 birdSound. All rights reserved.
//

#ifndef birdBufferStats_h
#define birdBufferStats_h

#include <stdio.h>
#include "birdBuffer.h"

double  getBufferQuartile (birdBuffer *buffer, int q);
double  getBufferMin(birdBuffer *buffer);
double  getBufferMax (birdBuffer *buffer);
double  getBufferMean (birdBuffer *buffer);
double  getBufferWeightedMean (birdBuffer *buffer);
double  getBufferMedian (birdBuffer *buffer);
double  getBufferSSE (birdBuffer *buffer);
double  getBufferMSE (birdBuffer *buffer);
double  getBufferSD (birdBuffer *buffer);
double  compareBuffer1SampStudT(birdBuffer *buffer, double mu);
double  compareBuffer2SampWelchT(birdBuffer *buffer1, birdBuffer *buffer2);

void    printBufferSummaryStats (birdBuffer *buffer);

#endif /* birdBufferStats_h */
