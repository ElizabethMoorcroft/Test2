//
//  AnimalTest.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 22/10/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#ifndef __Test2__AnimalTest__
#define __Test2__AnimalTest__

#include <iostream>


class AnimalTest {
public:
    
    void test_createanimal(int, double, double, double);
    
    // Calculations
    void test_NewLocation(double&, double&);
    void test_LeaveEnterWorld(const double&, const double&, const double&, const double&);
    void test_UpdateLocation (double);
    void test_LocationVector(double&, double&, int, int);
    double test_CalNext_X(double);
    double test_CalNext_Y(double);
    double test_RangeAngle(double);
    
    //object
    void test_totaldistance();
    void test_changeangle();
    
};

#endif /* defined(__Test2__AnimalTest__) */
