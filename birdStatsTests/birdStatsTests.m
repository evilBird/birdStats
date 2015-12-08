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
    _n = 10;
    createBuffer(&_birdBuffer, _n);
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
    for (int i = 0;i<self.n;i++) {
        double obs = (double)(i * arc4random_uniform(100));
        addToBuffer(&_birdBuffer, obs);
        runningSum+=obs;
    }
    
    double expectedResult = runningSum/(double)self.n;
    double result = getBufferMean(&_birdBuffer);
    XCTAssertEqual(result, expectedResult);
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
