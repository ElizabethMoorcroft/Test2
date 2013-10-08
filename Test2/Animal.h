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
    int HomeRange_id;
    int step_number;
    
    int male;
    
    int Move_NonCorr;
    double Move_speed;
    double Move_maxangle;
        
    double Current_x;
    double Current_y;
    double Current_angle;
    double Current_distance;
    
    double NextX;
    double NextY;
    double NextAngle;
    double NextDist;

    double Home_x;
    double Home_y;
    double Home_r;
    
    double Call_width;
    
    int locationvectorcount;
    
    std::vector<double> mylocationvector;
    std::vector<std::vector<double>> All_locations;
    std::vector<std::vector<double>> EndStep_locations;

public:
    Animal();
    Animal( int, int,  
            double, double , double ,
             double, double);
    void Set_MoveValue (double,double);
    void Set_HRValues (int, int, double, double, double );

    
    double RangeAngle(double);
    
    void NewLocation(double,double);
    void LeaveEnterWorld(double , double ,double , double );
    void UpdateLocation (double);
    void LocationVector(double, double,int, int);
    
    double DistToHRCentre();
    
    void setHomeX(double a){Home_x = a;};
    void setHomeY(double a){Home_y = a;};
    
    double getCurrentX(){return  Current_x;};
    double getCurrentY(){return  Current_y;};
    double getID(){return identifier;};
    double getHRID(){return HomeRange_id;};
    double getSpeed(){return Move_speed;};
    
    double CalNext_X(double);
    double CalNext_Y(double);
    
    double getCallWidth(){return Call_width;};
    std::vector<std::vector<double>> getAllLocations(){return All_locations;};
    std::vector<std::vector<double>> getEndStepLocations(){return EndStep_locations;};
    
    
};

#endif
