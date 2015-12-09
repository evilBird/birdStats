//
//  birdBufferStats.c
//  birdStats
//
//  Created by Travis Henspeter on 12/9/15.
//  Copyright © 2015 birdSound. All rights reserved.
//

#include "birdBufferStats.h"
#include "birdNormal.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

int mm_compare_doubles (const void * a, const void * b)
{
    return ( *(double*)a - *(double*)b );
}

double getBufferMin (birdBuffer *buffer)
{
    int n = buffer->n;
    if ( n == 0 ) {
        return 0.0;
    }
    
    double *temp = NULL;
    temp = (double *)(malloc(sizeof(double)*n+1));
    memcpy(temp, buffer->observations, sizeof(double)*n + 1);
    qsort(temp, n, sizeof(double), mm_compare_doubles);
    double result = temp[0];
    free(temp);
    return result;
}

double getBufferMax (birdBuffer *buffer)
{
    int n = buffer->n;
    if ( n == 0 ) {
        return 0.0;
    }
    double *temp = NULL;
    temp = (double *)(malloc(sizeof(double)*n+1));
    memcpy(temp, buffer->observations, sizeof(double)*n + 1);
    qsort(temp, n, sizeof(double), mm_compare_doubles);
    double result = temp[(n-1)];
    free(temp);
    return result;
}

double getBufferMean (birdBuffer *buffer)
{
    int n = buffer->n;
    
    if ( n <= 0 ) {
        return 0.0;
    }
    
    double runningSum = 0.0;
    int i = 0;
    do {
        int index = (buffer->i+i)%n;
        runningSum+=(buffer->observations[index]);
    } while ( ++ i < n );
    
    return (double)(runningSum/(double)(n));
}

double  getBufferWeightedMean (birdBuffer *buffer)
{
    int n = buffer->n;
    
    if ( n <= 0 ) {
        return 0.0;
    }
    
    double runningSumOfObs = 0.0;
    double runningSumOfWeights = 0.0;
    int i = 0;
    do {
        int idx = (( buffer->i - 1 -i ) % n );
        int index = ( idx >= 0 ) ? idx : ( n + idx );
        double obs = buffer->observations[index];
        double w = (double)( n - i )/(double)n;
        runningSumOfObs += ( obs * w );
        runningSumOfWeights += w;
    } while ( ++ i < n );
    
    return (double)(runningSumOfObs/(double)(runningSumOfWeights));
}


double getBufferMedian (birdBuffer *buffer)
{
    int n = buffer->n;
    if ( n <= 0 ) {
        return 0.0;
    }
    
    if ( n < 3) {
        return buffer->observations[n/2];
    }
    
    double *temp = NULL;
    temp = (double *)(malloc(sizeof(double)*n+1));
    memcpy(temp, buffer->observations, sizeof(double)*n + 1);
    qsort(temp, n, sizeof(double), mm_compare_doubles);
    double result = temp[(n/2)];
    free(temp);
    return result;
}

double  getBufferQuartile (birdBuffer *buffer, int q)
{
    int n = buffer->n;
    if ( n <= 0 ) {
        return 0.0;
    }
    
    if ( n < 4) {
        return - 1.0 ; // Error - cannot calculate quartiles without 4 or more observations
    }
    
    if ( q < 0 || q > 4)
    {
        return -1.0; //Error = quartile must be in the range
    }
    
    if ( q == 0 ) {
        return getBufferMin(buffer);
    }else if ( q == 4 ){
        return getBufferMax(buffer);
    }else if (q == 2 ){
        return getBufferMedian(buffer);
    }
    
    double *temp = NULL;
    temp = (double *)(malloc(sizeof(double)*n+1));
    memcpy(temp, buffer->observations, sizeof(double)*n + 1);
    qsort(temp, n, sizeof(double), mm_compare_doubles);
    int idx = (int)floor( (double)n * ( (double)q / 4.0 ) );
    double result = temp[idx];
    free(temp);
    return result;
}

double getBufferSSE (birdBuffer *buffer)
{
    int n = buffer->n;
    if ( n <= 0 ) {
        return 0.0;
    }
    
    double mean = getBufferMean(buffer);
    double sse = 0.0;
    int i = 0;
    do {
        int index = (buffer->i+i)%n;
        double obs = buffer->observations[index];
        double e = obs-mean;
        double se = e*e;
        sse+=se;
    } while ( ++i < n );
    
    return sse;
}

double getBufferMSE (birdBuffer *buffer)
{
    int n = buffer->n;
    if ( n <= 0) {
        return 0.0;
    }
    
    double sse = getBufferSSE(buffer);
    double mse = sse/(double)n;
    return mse;
}

double getBufferSD (birdBuffer *buffer)
{
    int n = buffer->n;
    if ( n <= 1 ) {
        return -1.0; // Error - can't calculate SD with 1 or fewer observations
    }
    
    double sse = getBufferSSE(buffer);
    double result = sqrt(sse)/((double)n - 1.0);
    return result;
}


double getBufferPDF(birdBuffer *buffer, double value)
{
    double mu = getBufferMean(buffer);
    double sigma = getBufferSD(buffer);
    return getPDF(value,mu,sigma);
}

double  compareBuffer1SampStudT(birdBuffer *buffer, double mu)
{
    double mean = getBufferMean(buffer);
    double sd = getBufferSD(buffer);
    return getPDF(mu,mean,sd);
}

double  compareBuffer2SampWelchT(birdBuffer *buffer1, birdBuffer *buffer2)
{
    int n1 = buffer1->n;
    int n2 = buffer2->n;
    if ( n1 <= 1 || n2 <= 1 ) {
        return -1.0; // Error - can't compare buffers with 1 or fewer observations
    }
    
    double num = (getBufferMean(buffer1) - getBufferMean(buffer2));
    double den = sqrt( ( pow( getBufferMSE( buffer1 ) , 2.0 ) / ( double ) n1 ) + ( pow( getBufferMSE( buffer2 ), 2.0 ) / ( double ) n2 ) );
    return (num/den);
}

void  printBufferSummaryStats (birdBuffer *buffer)
{
    int n = buffer->n;
    double max = getBufferMax(buffer);
    double min = getBufferMin(buffer);
    double firstQuartile = getBufferQuartile(buffer, 1);
    double mean = getBufferMean(buffer);
    double median = getBufferMedian(buffer);
    double thirdQuartile = getBufferQuartile(buffer, 3);
    double weightedMean = getBufferWeightedMean(buffer);
    double variance = getBufferMSE(buffer);
    double sse= getBufferSSE(buffer);
    double sd = getBufferSD(buffer);
    printf("\n\nBUFFER SUMMARY STATS:\ntag = %s\nn = %d\nmin = %.2f\n1st quartile = %.2f\nmedian = %.2f\n3rd quartile = %.2f\nmax = %.2f\nmean = %.2f\nweighted mean = %.2f\nSSE = %.2f\nvariance = %.2f\nstandard deviation = %.2f\n\n",buffer->tag,n,min,firstQuartile,median,thirdQuartile,max,mean,weightedMean,sse,variance,sd);
}
