//
//  SensorTest.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 14/10/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#ifndef __Test2__SensorTest__
#define __Test2__SensorTest__

#include <iostream>

class SensorTest {
public:
    
    void RunSensorTests();
    
    //---- Constructor -----//
    void SensorTest_construct();
    
    //---- Get / Set -----//
    void SensorTest_setgetAngle();
    void SensorTest_setgetHalfAngle();
    
    // ---- Functions ----//
    void SensorTest_CaptureIndividual1();
    void SensorTest_CaptureIndividual2();
    void SensorTest_CaptureIndividual3();
    void SensorTest_CaptureIndividual4();
    void SensorTest_CaptureIndividual5();
    void SensorTest_CaptureIndividual6();
    
    void TestVertAndAngleInteraction();
    void TestHorzAndAngleInteraction();
    void TestAngleAndAngleInteraction();
    void TestHorzAndCircInteraction();
    void TestVertAndCircInteraction();
    void TestAngleAndCircInteraction();
    
    void TestGradientFromAngle();
    
    void Test_CapturesIntersection1();
    void Test_CapturesIntersection2();
    void Test_CapturesIntersection3();
    void Test_CapturesIntersection4();
    void Test_CapturesIntersection5();
    void Test_CapturesIntersection6();
    void Test_CapturesIntersection7();
    void Test_CapturesIntersection8();
    void Test_CapturesIntersection9();
    void Test_CapturesIntersection10();
    
};

#endif /* defined(__Test2__SensorTest__) */
