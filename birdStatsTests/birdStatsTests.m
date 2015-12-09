//
//  birdStatsTests.m
//  birdStatsTests
//
//  Created by Travis Henspeter on 12/8/15.
//  Copyright © 2015 birdSound. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "birdBufferStats.h"
#import "birdNormal.h"

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
