//
//  CameraTraps.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 28/03/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//
#include <iostream>
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
    
    std::vector<std::vector<int>> Captures;
    std::vector<int> myvector;
    
public:
    CameraTrap();
    CameraTrap( int //ID No (Also time step)
               ,double
               );
    CameraTrap( int //ID No (Also time step)
                ,double
                ,double
               ,double);
    std::vector<std::vector<int>> getCaptures(){return Captures;};
    
    double getID(){return CT_identifier;};
    double getStepOn(){return CT_StepOn;};
    double getXloc(){return location_x;};
    double getYloc(){return location_y;};
    double getAngle(){return angle;};
    double getHalfAngle(){return angle_HalfWidth;};
    clock_t gettime(){return time1;};
    
    int CapturesIndividual(double  //x location animal
                            ,double //y location animal
                            ,int // Animal ID
                            ,double //Call angle
                            , double//Movement angle
                           , int //iternation number
                       );
    
    void TestCapturesIndividual(int
                                ,double
                                , double
                                , double
                                , double
                                , double
                                , double
                                , double);
    

    
};

#endif
