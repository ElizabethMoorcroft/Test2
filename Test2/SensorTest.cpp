//
//  SensorTest.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 14/10/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include "SensorTest.h"
#include "Sensors.h"


////// Sensor::Sensor(int Id, double sensorhalfangle, double sensorradius) /////
/*
//---------------------- Constructor -------------------//
// Test Sensor constructor
 void SensorTest::SensorTest_construct(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 1);
    double id Sensor1.getID();
    if(id!=1){
        std::cout<<"Error! Failed Sensor test - Test construct" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };

//---------------------- Sets / Gets -------------------//
// Test set and get of the angle
 void SensorTest::SensorTest_setgetAngle(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
    Sensor1.setAngle(1);
    double angle Sensor1.getAngle();
    if(angle!=1){
        std::cout<<"Error! Failed Sensor test - Test setgetAngle" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 // Test set and get of the halfangle
 void SensorTest::SensorTest_setgetHalfAngle(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
    Sensor1.setHalfAngle(1);
    double halfangle Sensor1.getHalfAngle();
    if(halfangle!=1){
        std::cout<<"Error! Failed Sensor test - Test setgetHalfAngle" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 
 
  //---------------------- Function: CaptureIndividual -------------------//
void SensorTest::SensorTest_CaptureIndividual1(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
    
    int Estimate;
    //location_x_animal,location_y_animal,Individual_ID,call_halfwidth,move_angle,itnumber,time.
    setXLoc(0);
    setYLoc(0);
    Estimate = Sensor1.CapturesIndividual(0,0,1,M_PI,0,0,0,0); // Animal directly above Sensor
    if(Estimate!=1){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
};


void SensorTest::SensorTest_CaptureIndividual2(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
 
    int Estimate;
    //location_x_animal,location_y_animal,Individual_ID,call_halfwidth,move_angle,itnumber,time.
    setXLoc(0); setYLoc(0);
    Estimate = Sensor1.CapturesIndividual(1,2,1,M_PI,0,0,0,0); // Animal is on boundary and facing away
    if(Estimate!=1){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };

 
 void SensorTest::SensorTest_CaptureIndividual3(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/4, 0);
    setXLoc(0); setYLoc(0);
    int Estimate;
    Sensor1.setAngle(0);
    Sensor1.setHalfAngle(0);
    Estimate = Sensor1.CapturesIndividual(0.5,0.5,1,M_PI,0,0,0,0); // Animal is on within boundary and facing towards, Sensor is facing towards
    if(Estimate!=1){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 void SensorTest::SensorTest_CaptureIndividual4(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/4, 0);
    setXLoc(0); setYLoc(0);
    int Estimate;
    Sensor1.setAngle(0);
    Sensor1.setHalfAngle(0);
    Estimate = CapturesIndividual(sin(angle_HalfWidth),cos(angle_HalfWidth),1,M_PI/4,M_PI+angle_HalfWidth,0,0,0);//Animal is on right edge and facing towards sensor
    if(Estimate!=1){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"4" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 void SensorTest::SensorTest_CaptureIndividual5(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/4, 0);
    setXLoc(0); setYLoc(0);
    int Estimate;
    Sensor1.setAngle(0);
    Sensor1.setHalfAngle(0);
    Estimate = CapturesIndividual(-sin(angle_HalfWidth),cos(angle_HalfWidth),1,M_PI/4,M_PI-angle_HalfWidth,0,0,0);// Animal is on left edge and facing towards Sensor
    if(Estimate!=1){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"5" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 void SensorTest::SensorTest_CaptureIndividual6(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/4, 0);
    setXLoc(0); setYLoc(0);
    int Estimate;
    Sensor1.setAngle(0);
    Sensor1.setHalfAngle(0);
    Estimate = CapturesIndividual(location_x,location_y-0.5,1,M_PI/4,angle,0,0,0); // Animal in range and facing towards Sensor, but Sensor facing away
    if(Estimate!=1){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"6" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 
 
 
 
 //---------------------- Function: Interection of lines -------------------//
 
 // Unit testing for:
 //      - VertAndAngleInteraction
 
 void Sensor::TestVertAndAngleInteraction(){
    // Vertical line X = 0; Angle line: Gradient  = 1, Intercept = 5;
    // The intercept should be at (X= 0, Y=5)
    if(VertAndAngleInteraction(0, 1, 5)!=5){
        std::cout<<"Error! Failed Sensor test - TestVertAndAngleInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Vertical line X = 1; Angle line: Gradient  = -1, Intercept = 5;
    // The intercept should be at (X= 1, Y=4)
    if(VertAndAngleInteraction(1, -1, 5)!=4){
        std::cout<<"Error! Failed Sensor test - TestVertAndAngleInteraction: "<<"2"<< std::endl;
        exit (EXIT_FAILURE);
    };
    // Vertical line X = -1; Angle line: Gradient  = 1, Intercept = 5;
    // The intercept should be at (X= -1, Y=4)
    if(VertAndAngleInteraction(-1, 1, 5)!=4){
        std::cout<<"Error! Failed Sensor test - TestVertAndAngleInteraction: "<<"3"<< std::endl;
        exit (EXIT_FAILURE);
    };
    //IF passed then print to screen
    std::cout<<"Passed! Sensor test - TestVertAndAngleInteraction"<< std::endl;
 };
 

*/

/*
 Sensor1.TestCI();
Sensor1.TestVertAndAngleInteraction();
Sensor1.TestHorzAndAngleInteraction();
Sensor1.TestAngleAndAngleInteraction();
Sensor1.TestHorzAndCircInteraction();
Sensor1.TestVertAndCircInteraction();
Sensor1.TestAngleAndCircInteraction();
Sensor1.TestTimeAndAngleCal();
Sensor1.TestGradientFromAngle();
Sensor1.TestCapturesIntersection();*/