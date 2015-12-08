//
//  birdBuffer.c
//  Jedi Trumpet
//
//  Created by Travis Henspeter on 12/2/15.
//  Copyright © 2015 Sonation, Inc. All rights reserved.
//

#include "birdBuffer.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

void createBuffer (birdBuffer *buffer, char *tag, int max)
{
    buffer->n = 0;
    buffer->i = 0;
    buffer->max = max;
    strcpy(buffer->tag, tag);
    buffer->observations = (double*)(malloc(sizeof(double) * (max+1)));
}

void addToBuffer (birdBuffer *buffer, double obs)
{
    if ( obs != obs ) {
        return;
    }
    int n = buffer->n;
    int max = buffer->max;
    int i = buffer->i;
    buffer->observations[i] = obs;
    n++;
    i++;
    
    if ( n > max ) {
        n = max;
    }
    
    i = i%max;
    buffer->n = n;
    buffer->i = i;
}

int getBufferObsIndex(birdBuffer *buffer, int idx)
{
    int n = buffer->n;
    
    if ( idx >= n ) {
        return -1; //Error - index is beyond bounds of array
    }
    
    int i = buffer->i;
    int firstIndex = ( i == n ) ? 0 : ( i );
    int index = firstIndex+idx;
    return index;
}

double  getBufferObs (birdBuffer *buffer, int idx)
{
    int n = buffer->n;
    int index = getBufferObsIndex(buffer, idx);
    if ( idx >= n || index < 0 || index >= n ) {
        return -1.0; //Error - index is beyond bounds of array
    }
    
    double obs = buffer->observations[index];
    return obs;
}

int getBufferRecentObsIndex(birdBuffer *buffer, int idx)
{
    int n = buffer->n;
    if ( idx >= n || n <= 0 ) {
        return -1; //Error - index is beyond bounds of array, or the array is empty
    }
    
    int i = buffer->i;
    int lastIdx = ( i > 0 ) ? ( i - 1 ) : ( n - 1 );
    int diff = lastIdx-idx;
    int index = ( diff >= 0 ) ? ( diff ) : ( n + diff );
    return index;
}


double  getBufferRecentObs (birdBuffer *buffer, int idx)
{
    int n = buffer->n;
    int index = getBufferRecentObsIndex(buffer, idx);
    
    if ( idx >= n || n <= 0 || index >= n ) {
        return -1.0; //Error - index is beyond bounds of array, or the array is empty
    }
    double obs = buffer->observations[index];
    return obs;
}

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

void setBuffer (birdBuffer *buffer, double setToValue)
{
    int n = buffer->n;
    if (n <= 0){
        return;
    }
    
    int i = 0;
    do {
        int index = (i+buffer->i)%n;
        buffer->observations[index] = setToValue;
    } while (++i<n);
}

void setBufferObs (birdBuffer *buffer, double value, int idx)
{
    
}

double getPDF(double value, double mu, double sigma)
{
    double pdf = (1.0/(sigma * sqrt(2.0*M_PI))) * exp(-(pow((value - mu), 2.0)/pow((2*sigma), 2.0)));
    return pdf;
}

double  getRandomNormal(double mu, double sigma)
{
    double u = (double)arc4random_uniform(1e6) * 1e-6;
    double v = (double)arc4random_uniform(1e6) * 1e-6;
    double x = sqrt(-2.0 * log(u)) * cos(2.0 * M_PI * v);
    double y = sqrt(-2.0 * log(u)) * sin(2.0 * M_PI * v);
    double r = ( arc4random_uniform(100) > 49 ) ? x : y;
    double result = r * sigma + mu;
    return result;
}


double getBufferPDF(birdBuffer *buffer, double value)
{
    double mu = getBufferMean(buffer);
    double sigma = getBufferSD(buffer);
    return getPDF(value,mu,sigma);
}

void  printBufferSummary (birdBuffer *buffer)
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

void zeroBuffer (birdBuffer *buffer)
{
    return setBuffer(buffer, 0.0);
}

void destroyBuffer (birdBuffer *buffer)
{
    if ( buffer->n == 0 ) {
        return;
    }
    buffer->n = 0;
    buffer->i = 0;
    buffer->max = 0;
    free(buffer->observations);
}