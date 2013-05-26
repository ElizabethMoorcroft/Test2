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
    
    double location_x;
    double location_y;
    
    double radius;
    double angle;
    double angle_HalfWidth;
    
    std::vector<std::vector<int>> Captures;
    
public:
    CameraTrap();
    CameraTrap(int //ID No (Also time step)
               , double //Angle between cameras
               , double , double
               );
    std::vector<std::vector<int>> getCaptures(){return Captures;};
    
    double getID(){return CT_identifier;};
    double getXloc(){return location_x;};
    double getYloc(){return location_y;};
    double getAngle(){return angle;};
    double getHalfAngle(){return angle_HalfWidth;};
    
    void CapturesIndividual(double  //x location animal
                            ,double //y location animal
                            ,int // Animal ID
                            ,int //Camera #
                            ,int // step
                            ,double //Call angle
                            , double//Movement angle
                       );
    
};

#endif
