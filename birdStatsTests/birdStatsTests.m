//
//  birdStatsTests.m
//  birdStatsTests
//
//  Created by Travis Henspeter on 12/8/15.
//  Copyright © 2015 birdSound. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "birdBuffer.h"

@interface birdStatsTests : XCTestCase

@property (nonatomic)           birdBuffer              birdBuffer;
@property (nonatomic)           int                     n;

@end

@implementation birdStatsTests

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
        double obs = (double)(i * arc4random_uniform(100));
        addToBuffer(&_birdBuffer, obs);
        if ( i >= offset) {
            runningSum+=obs;
        }
    }
    
    double expectedResult = runningSum/(double)self.n;
    double result = getBufferMean(&_birdBuffer);
    XCTAssertEqual(result, expectedResult);
}

- (void)testBufferGetObs
{
    int idx = 0;
    double expectedResult = -1;
    int offset = self.n*0.4;
    int num = self.n+offset;
    for (int i = 0;i<num;i++) {
        double obs = (double)(i * arc4random_uniform(100));
        addToBuffer(&_birdBuffer, obs);
        if ( ( i - offset + idx ) == idx ) {
            
            expectedResult = obs;
        }
    }
    
    double result = getBufferObs(&_birdBuffer, idx);
    
    XCTAssertEqual(result, expectedResult);
}

- (void)testBufferRecentObs
{
    int idx = 88;
    int offset = self.n*0.4;
    int num = self.n+offset;
    double expectedResult = -1.0;
    for (int i = 0;i<num;i++) {
        double obs = (double)(i * arc4random_uniform(100));
        addToBuffer(&_birdBuffer, obs);
        if ( ( num - 1 - i ) == idx ) {
            expectedResult = obs;
        }
    }
    
    double result = getBufferRecentObs(&_birdBuffer, idx);
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

- (void)testBufferSet
{
    double runningSum = 0;
    int offset = self.n*0.4;
    int num = self.n+offset;
    for (int i = 0;i<num;i++) {
        double obs = (double)(i * arc4random_uniform(100));
        addToBuffer(&_birdBuffer, obs);
        if ( i >= offset) {
            runningSum+=obs;
        }
    }
    
    double expectedResult = runningSum/(double)self.n;
    double result = getBufferMean(&_birdBuffer);
    XCTAssertEqual(result, expectedResult);
    double setTo = -1.0;
    setBuffer(&_birdBuffer, setTo);
    double newMean = getBufferMean(&_birdBuffer);
    XCTAssertEqual(newMean, setTo);
}

- (void)testBufferOrder
{
    int num = self.n+1;
    for ( int i = 0; i < num; i ++ ) {
        double obs = (double)i;
        addToBuffer(&_birdBuffer, obs);
    }
    
    double expectedResult = num-1;
    double result = _birdBuffer.observations[0];
    XCTAssertEqual(result, expectedResult);
}

- (void)testPrintBufferSummaryStats
{
    double runningSum = 0;
    int offset = self.n*0.4;
    int num = self.n+offset;
    for (int i = 0;i<num;i++) {
        double obs = (double)arc4random_uniform(100);
        addToBuffer(&_birdBuffer, obs);
    }
    
    printBufferSummary(&_birdBuffer);
    double pdf = getBufferPDF(&_birdBuffer, 50.0);
}

- (void)testGetPDF
{
    double mean = 10.0;
    double sd = 5.0;
    for (int i = 0; i < self.n; i ++) {
        double rnorm = getRandomNormal(mean, sd);
        NSLog(@"norm: %.2f",rnorm);
        addToBuffer(&_birdBuffer, rnorm);
    }
    
    double mu = getBufferMean(&_birdBuffer);
    double sig = getBufferSD(&_birdBuffer);
    printBufferSummary(&_birdBuffer);
    double pdf = getBufferPDF(&_birdBuffer, 5.0);
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
