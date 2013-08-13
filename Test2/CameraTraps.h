//
//  CameraTraps.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 28/03/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//
#include <iostream>
#include "Parameters.h"
#include <cstdio>
#include <math.h>
#include <time.h>
#include <vector>
#ifndef Test2_CameraTraps_h
#define Test2_CameraTraps_h

class CameraTrap {
    
private:
    int CT_identifier;
    int CT_StepOn;
    
    double location_x;
    double location_y;
    
    double radius;
    double angle; // Direction of the camera
    double angle_HalfWidth; // Half width of the camera 
    int capturecount;
    
    clock_t time1;
    
    std::vector<std::vector<double>> Captures;
    std::vector<double> myvector;
    
public:
    CameraTrap();
    CameraTrap( int //ID No (Also time step)
                ,double
                ,double
               ,double);
    std::vector<std::vector<double>> getCaptures(){return Captures;};
    void resetCaptures();
    
    double getID(){return CT_identifier;};
    double getStepOn(){return CT_StepOn;};
    double getXloc(){return location_x;};
    double getYloc(){return location_y;};
    double getAngle(){return angle;};
    double getHalfAngle(){return angle_HalfWidth;};
    clock_t gettime(){return time1;};
    
    void Add1StepOn();
    void ResetStepOn();
    
    int CapturesIndividual(double  //x location animal
                            ,double //y location animal
                            ,int // Animal ID
                            ,double //Call angle
                            , double//Movement angle
                           , int //iternation number
                           ,double //time
                       );
    
    int CapturesIntersection(double  //x location animal
                 ,double //y location animal
                ,double,double //previous x/y
                          ,int // Animal ID
                          ,double //Call angle
                          , double//Movement angle
                          , int //iternation number
                          );
    
    int CameraAndMovement(double,
                        double,
                        double,
                        double,
                        int,
                        double,
                        double,
                        int,
                        double,
                        double,
                        double,
                        double,
                        int,
                        double);
    
    int CameraCircAndMovement(double, //location_x_animal,
                            double, //location_y_animal,
                            double, // previous_x_animal,
                            double, // previous_y_animal,
                            int, // Individual_ID,
                            double, // call_halfwidth,
                            double, // move_angle,
                            int, // itnumber,
                            double, // m_animal,
                            double, // c_animal,
                            double // disttotal
                            );
    
    void TestCapturesIndividual(int
                                ,double
                                , double
                                , double
                                , double
                                , double
                                , double
                                , double);
    
    void TestVertAndAngleInteraction();
    void TestHorzAndAngleInteraction();
    void TestAngleAndAngleInteraction();
    void TestHorzAndCircInteraction();
    void TestVertAndCircInteraction();
    void TestAngleAndCircInteraction();
    
    

    
};

#endif
