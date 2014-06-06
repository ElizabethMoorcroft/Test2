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
    //if(Locations.size()>0){
        
        //double currentx = Locations[2];
        //double currenty = Locations[3];
        //double currentangle = Locations[4];
        
        
        // for each step calcualte past location and current location, and direction when leaving past location
       // if(Locations[1]>0){
            //double previousx = previousLocations[2];
            //double previousy = previousLocations[3];
            // Calcualtes whether the animal is captured
            
            // Only check for the capture if the start and end locations are with a given distance "checkforcapture"
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

                    AllSensors[sensor] ->CapturesIntersection(currentx,currenty,previousx,previousy,i,CallWidth[0],currentangle,iterationnumber, Captures);

                };// End of if distance close to sensor
                


            }; // END of sensor for loop
    //    }; // end of if not first step
     //   };// END of check movement exists
   // std::cout<<"Exit Sensor " <<std::endl;
};