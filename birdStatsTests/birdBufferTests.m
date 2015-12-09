//
//  birdBufferTests.m
//  birdStats
//
//  Created by Travis Henspeter on 12/9/15.
//  Copyright © 2015 birdSound. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "birdBufferStats.h"

@interface birdBufferTests : XCTestCase

@property (nonatomic)           birdBuffer              birdBuffer;
@property (nonatomic)           int                     n;

@end

@implementation birdBufferTests

- (void)setUp {
    [super setUp];
    _n = 100;
    createBuffer(&_birdBuffer, "birdBuffer",_n);
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
    destroyBuffer(&_birdBuffer);
    _n = 0;
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
