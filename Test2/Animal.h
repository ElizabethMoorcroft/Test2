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
    double StepLengthDist;

        
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
    std::vector<std::vector<double> > All_locations;

public:
    Animal();
    Animal(int, double, double, double);
    
    //Set functions
    void setMove_speed(double a);
    void setAnimalspeedPerch(double a, double b);
    void setMove_maxangle(double a){Move_maxangle=a;};
    void setNextAngle(double a){NextAngle=a;};
    void setNextDist(double a){NextDist=a;};
    void setCurrentX(double a){Current_x = a;};
    void setCurrentY(double a){Current_y = a;};
    void setNextX(double a){Current_x = a;};
    void setNextY(double a){Current_y = a;};
    void setlargelocationvector(double a){All_locations.resize(10*a);}
    
    // Get functions
    double getCurrentX(){return  Current_x;};
    double getCurrentY(){return  Current_y;};
    double getID(){return identifier;};
    double getTotalDistance(){return Total_distance;};
    double getNextDist(){return NextDist;};
    double getNextAngle(){return NextAngle;};
    double getNextX(){return NextX;};
    double getNextY(){return NextY;};
    
    std::vector<std::vector<double> > getAllLocations(){return All_locations;};
    
    // Calculations
    void NewLocation(double&, double&);
    void LeaveEnterWorld(const double&, const double&, const double&, const double&);
    void UpdateLocation (double);
    void LocationVector(double&, double&, int, int);
    double CalNext_X(double);
    double CalNext_Y(double);
    double RangeAngle(double);
    
};

#endif
