//
//  birdBuffer.c
//  Jedi Trumpet
//
//  Created by Travis Henspeter on 12/2/15.
//  Copyright © 2015 Sonation, Inc. All rights reserved.
//

#include "birdBuffer.h"
#include <string.h>
#include <stdlib.h>

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

void printBuffer (birdBuffer *buffer)
{
    int n = buffer->n;
    int index = getBufferObsIndex(buffer, 0);
    printf("\nPRINT BUFFER (n = %d, first obs index = %d)",n,index);
    for (int i = 0; i < n; i ++) {
        int idx = (index + i)%n;
        double obs = buffer->observations[idx];
        printf("\n%d) %.2f",idx,obs);
    }
    printf("\n");
}
