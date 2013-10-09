//
//  Aniaml.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 23/02/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//
#include <iostream>
#include <cstdio>
#include <math.h>
#include <time.h>
#include <vector>

#ifndef Test2_Animal_h
#define Test2_Animal_h

class Animal {
    
    int identifier;
    
    int step_number;
        
    int Move_NonCorr;
    double Move_speed;
    double Move_maxangle;
        
    double Current_x;
    double Current_y;
    double Current_angle;
    
    double Total_distance;
    
    double NextX;
    double NextY;
    double NextAngle;
    double NextDist;
        
    int locationvectorcount;
    
    std::vector<double> mylocationvector;
    std::vector<std::vector<double>> All_locations;

public:
    Animal();
    Animal(int, double, double, double);
    
    // Get functions
    double getCurrentX(){return  Current_x;};
    double getCurrentY(){return  Current_y;};
    double getID(){return identifier;};
    
    std::vector<std::vector<double>> getAllLocations(){return All_locations;};
    
    // Calculations
    void NewLocation(double, double);
    void LeaveEnterWorld(double, double, double, double);
    void UpdateLocation (double);
    void LocationVector(double, double, int, int);
    double CalNext_X(double);
    double CalNext_Y(double);
    double RangeAngle(double);
    
};

#endif
