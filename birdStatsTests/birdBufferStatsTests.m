//
//  birdBufferStatsTests.m
//  birdStats
//
//  Created by Travis Henspeter on 12/9/15.
//  Copyright © 2015 birdSound. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "birdBufferStats.h"
#import "birdNormal.h"

@interface birdBufferStatsTests : XCTestCase

@property (nonatomic)           birdBuffer              birdBuffer;
@property (nonatomic)           int                     n;

@end

@implementation birdBufferStatsTests

- (void)setUp {
    [super setUp];
    _n = 100;
    NSString *tag = @"birdStats Tests";
    createBuffer(&_birdBuffer, [tag UTF8String],_n);
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
    destroyBuffer(&_birdBuffer);
    _n = 0;
}

- (void)testBufferMean
{
    double runningSum = 0;
    int offset = self.n*0.4;
    int num = self.n+offset;
    for (int i = 0;i<num;i++) {
        double obs = getRandomNormal(50, 16.666);
        addToBuffer(&_birdBuffer, obs);
        if ( i >= offset) {
            runningSum+=obs;
        }
    }
    printBuffer(&_birdBuffer);
    printBufferSummaryStats(&_birdBuffer);
    double expectedResult = runningSum/(double)self.n;
    double result = getBufferMean(&_birdBuffer);
    if ( result == expectedResult ) {
        NSLog(@"\nmean = %.2f, expected = %.2f",result,expectedResult);
    }
    XCTAssertEqual(result, expectedResult);
}

- (void)testBufferVariance
{
    double runningSum = 0;
    int offset = self.n*0.6;
    int num = self.n+offset;
    double theObs[num];
    for (int i = 0;i<num;i++) {
        double obs = (double)(i * arc4random_uniform(100));
        addToBuffer(&_birdBuffer, obs);
        if ( i >= offset) {
            theObs[(i-offset)] = obs;
            runningSum+=obs;
        }
    }
    
    double mean = getBufferMean(&_birdBuffer);
    double sse = 0.0;
    for (int i = 0; i < self.n; i++) {
        double e = theObs[i]-mean;
        double se = e*e;
        sse+=se;
    }
    
    double expectedSSE = sse;
    double expectedMSE = sse/(double)self.n;
    double actualSSE = getBufferSSE(&_birdBuffer);
    double actualMSE = getBufferMSE(&_birdBuffer);
    XCTAssertEqual(actualSSE, expectedSSE);
    XCTAssertEqual(actualMSE, expectedMSE);
}

-(void)testBufferWeightedMean
{
    int offset = self.n*0.4;
    int num = self.n+offset;
    double sumOfWeights = 0.0;
    double sumOfWeightedObs = 0.0;
    for (int i = 0;i<num;i++) {
        double obs = (double)(i * arc4random_uniform(100));
        addToBuffer(&_birdBuffer, obs);
        if ( i >= offset) {
            int index = ( i - offset + 1 );
            double weight = ((double)index/(double)_n);
            NSLog(@"INDEX: %d, WEIGHT: %.2f",index,weight);
            sumOfWeightedObs += (obs * weight);
            sumOfWeights += weight;
        }
    }
    
    double expectedResult = sumOfWeightedObs/sumOfWeights;
    double result = getBufferWeightedMean(&_birdBuffer);
    XCTAssertEqual(round(result), round(expectedResult));
}

- (void)testBufferMedian
{
    int o = 15;
    int p = _n+o;
    for (int i = o; i < p; i++) {
        double obs = (double)i;
        addToBuffer(&_birdBuffer, obs);
    }
    double expectedResult = (double)(o + (p-o)/2);
    double result = getBufferMedian(&_birdBuffer);
    XCTAssertEqual(result, expectedResult);
}



- (void)testPrintBufferSummaryStats
{
    int offset = self.n*0.4;
    int num = self.n+offset;
    for (int i = 0;i<num;i++) {
        double obs = (double)arc4random_uniform(100);
        addToBuffer(&_birdBuffer, obs);
    }
    
    printBufferSummaryStats(&_birdBuffer);
}

- (void)test1SampleCompare
{
    double mean = 10.0;
    double sd = 5.0;
    for (int i = 0; i < self.n; i ++) {
        double rnorm = getRandomNormal(mean, sd);
        NSLog(@"norm: %.2f",rnorm);
        addToBuffer(&_birdBuffer, rnorm);
    }
    printBufferSummaryStats(&_birdBuffer);
    double pdf = compareBuffer1SampStudT(&_birdBuffer, mean);
    XCTAssertTrue(pdf>0.05);
}

- (void)test2SampleCompare
{
    birdBuffer buffer1, buffer2;
    createBuffer(&buffer1, "buffer1", _n);
    createBuffer(&buffer2, "buffer2", _n);
    
    double mu1,mu2,sig1,sig2;
    mu1 = 100.0;
    sig1 = 20.0;
    mu2 = 110.0;
    sig2 = 30.0;
    
    for (int i = 0; i < self.n; i++) {
        double obs1 = getRandomNormal(mu1, sig1);
        double obs2 = getRandomNormal(mu2, sig2);
        addToBuffer(&buffer1, obs1);
        addToBuffer(&buffer2, obs2);
    }
    
    printBufferSummaryStats(&buffer1);
    printBufferSummaryStats(&buffer2);
    
    double tStat = compareBuffer2SampWelchT(&buffer1, &buffer2);
    NSLog(@"\nCOMPARE BUFFERS T STAT = %.5f",tStat);
}

- (void)testExample {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
