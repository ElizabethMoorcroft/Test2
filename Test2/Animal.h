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
#include "Sensors.h"
#include "CheckSensor.h"

#ifndef Test2_Animal_h
#define Test2_Animal_h

class Animal {
    
    int identifier;
<<<<<<< HEAD
    int HomeRange_id;
    int step_number;
    
    int male;
=======
>>>>>>> bats
    
    int step_number;
        
    int Move_NonCorr;
    double Move_speed;
    double Move_maxangle;
    double StepLengthDist;
    double perchIndivid;
    
        
    double Current_x;
    double Current_y;
    double Current_angle;
    
    double Total_distance;
    
    double NextX;
    double NextY;
    double NextAngle;
    double NextDist;
<<<<<<< HEAD

    double Home_x;
    double Home_y;
    double Home_r;
    
    double Call_width;
    
    int locationvectorcount;
=======
>>>>>>> bats
    

public:
    Animal();
<<<<<<< HEAD
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
    
=======
    Animal(int, double, double, double, std::ofstream &Movement, std::vector<Sensor*> AllSensors , std::ofstream &Captures, int iterationnumber);
    
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
    
    // Get functions
>>>>>>> bats
    double getCurrentX(){return  Current_x;};
    double getCurrentY(){return  Current_y;};
    double getID(){return identifier;};
    double getTotalDistance(){return Total_distance;};
    double getNextDist(){return NextDist;};
    double getNextAngle(){return NextAngle;};
    double getNextX(){return NextX;};
    double getNextY(){return NextY;};
    
    
    // Calculations
    void NewLocation(double&, double&);
    void LeaveEnterWorld(const double&, const double&, const double&, const double&, std::ofstream &Movement, std::vector<Sensor*> AllSensors , std::ofstream &Captures, int iterationnumber, double, double);
    void UpdateLocation (double, std::ofstream &Movement, std::vector<Sensor*> AllSensors , std::ofstream &Captures, int iterationnumber);
    void LocationVector(double&, double&, double, double, int, int, std::ofstream &Movement, std::vector<Sensor*> AllSensors , std::ofstream &Captures, int iterationnumber);
    double CalNext_X(double);
    double CalNext_Y(double);
<<<<<<< HEAD
    
    double getCallWidth(){return Call_width;};
    std::vector<std::vector<double>> getAllLocations(){return All_locations;};
    std::vector<std::vector<double>> getEndStepLocations(){return EndStep_locations;};
    
=======
    double RangeAngle(double);
>>>>>>> bats
    
};

#endif
