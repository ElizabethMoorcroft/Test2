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
    
    // Run all tests
    void RunAnimalTests();
    
    // Creating animals
    void test_createanimal(int, double, double, double);
    
    //Gets and Sets
    
    //------------------ Calculations ----------------//    
    void test_LocationVector(double&, double&, int, int);
    
    // Correct entering and leaving of world
    void test_UpdateLocation(double);
    
    // Testing that a new location is created.
    void test_NewLocation1(); // Done
    void test_NewLocation2(); // Done
    
    // Correct entering and leaving of world
    void test_LeaveEnterWorld1(); // Done
    void test_LeaveEnterWorld2(); // Done
    void test_LeaveEnterWorld3(); // Done
    void test_LeaveEnterWorld4(); // Done
    
    // Calculating next locations from angle and distance
    //X
    void test_CalNext_X1(); // Done
    void test_CalNext_X2(); // Done
    void test_CalNext_X3(); // Done
    void test_CalNext_X4(); // Done
    void test_CalNext_X5(); // Done
    void test_CalNext_X6(); // Done
    //Y
    void test_CalNext_Y1(); // Done
    void test_CalNext_Y2(); // Done
    void test_CalNext_Y3(); // Done
    void test_CalNext_Y4(); // Done
    void test_CalNext_Y5(); // Done
    void test_CalNext_Y6(); // Done
    
    // Testing +ve change of angle
    void test_RangeAngle1(); // Done
    void test_RangeAngle2(); // Done
    void test_RangeAngle3(); // Done
    void test_RangeAngle4(); // Done
    
    //whole object
    void test_totaldistance(double); // Done
    void test_changeangle(double);   // Done
    
};

#endif /* defined(__Test2__AnimalTest__) */
