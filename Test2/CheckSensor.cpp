//
//  CheckSensor.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 21/11/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include "CheckSensor.h"
#include "Sensors.h"
#include "Parameters.h"
#include <vector>

CheckSensor::CheckSensor(){};

CheckSensor::CheckSensor(double currentx,double currenty, double currentangle, double previousx,double previousy,
                         std::vector<Sensor*> AllSensors,
                         std::ofstream &Movement,
                         std::ofstream &Captures,
                         int stepcounter, int i, int iterationnumber,
                         double perchvalue, double speedvalue){

    double checkforcapture =StepLength*(speedvalue/(1-perchvalue))*2;
    for(int sensor=0; sensor<NoSensors; sensor++){
                
        double sensorx = AllSensors[sensor] -> getXloc();
        double sensory = AllSensors[sensor] -> getYloc();
        double sensorradius = AllSensors[sensor] -> getRadius();
        double checkforcaptureplusradius =checkforcapture+sensorradius;
                
        AllSensors[sensor] -> setStepOn(stepcounter);

        double disttosensorprevious = sqrt(pow(previousx - sensorx,2)+pow(previousy - sensory,2));
        double disttosensorcurrent = sqrt(pow(currentx - sensorx,2)+pow(currenty - sensory,2));
                
                
        if(disttosensorprevious<checkforcaptureplusradius  && disttosensorcurrent<checkforcaptureplusradius ){

            AllSensors[sensor] ->CapturesIntersection(currentx,currenty,previousx,previousy,i,currentangle,iterationnumber, Captures);

        };// End of if distance close to sensor
                


    }; // END of sensor for loop
};