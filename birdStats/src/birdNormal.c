//
//  birdNormal.c
//  birdStats
//
//  Created by Travis Henspeter on 12/9/15.
//  Copyright © 2015 birdSound. All rights reserved.
//

#include "birdNormal.h"
#include <math.h>
#include <stdlib.h>

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

