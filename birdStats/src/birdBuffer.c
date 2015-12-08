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

void createBuffer (birdBuffer *buffer, int max)
{
    buffer->n = 0;
    buffer->i = 0;
    buffer->max = max;
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
    
    if ( n == 0 ) {
        return 0.0;
    }
    
    double runningSum = 0.0;
    while (n--) {
        runningSum+=(buffer->observations[n]);
    }
    return (double)(runningSum/(double)(buffer->n));
}

double getBufferMedian (birdBuffer *buffer)
{
    int n = buffer->n;
    if ( n == 0 ) {
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