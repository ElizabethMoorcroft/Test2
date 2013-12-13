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

CheckSensor::CheckSensor(std::vector<double> Locations, std::vector<double> previousLocations,
                         std::vector<Sensor*> AllSensors,
                         std::ofstream &Movement,
                         int stepcounter, int i, int iterationnumber){
    if(Locations.size()>0){
        
        double currentx = Locations[2];
        double currenty = Locations[3];
        double currentangle = Locations[4];
    
        if(SaveMovement==1){
            Movement<< Locations[0] << //1st column, row "stepcounter"
            "," << Locations[1] << //2nd column, row "stepcounter"
            "," << currentx << //...
            "," << currenty << //...
            "," << currentangle << //...
            "," << Locations[5] << //...
            "," << Locations[6] << //...
            "," << Locations[7] << //8th column, row "stepcounter"
            "," << Locations[8] << //8th column, row "stepcounter"
            "," << iterationnumber <<                  // itertaion number
            "\n";                                      // New line
        }; // END OF IF LOOP FOR SAVING
        
        
        // for each step calcualte past location and current location, and direction when leaving past location
        if(Locations[1]>0){
            double previousx = previousLocations[2];
            double previousy = previousLocations[3];
            // Calcualtes whether the animal is captured
            
            // Only check for the capture if the start and end locations are with a given distance "checkforcapture"
            double checkforcapture =StepLength*0.46*2;
            for(int sensor=0; sensor<NoSensors; sensor++){
               // if(sensor==209){std::cout<<"Sensor1: "<<sensor+1 << "/" << NoSensors <<std::endl;}
                double sensorx = AllSensors[sensor] -> getXloc();
                double sensory = AllSensors[sensor] -> getYloc();
                double sensorradius = AllSensors[sensor] -> getRadius();
                double checkforcaptureplusradius =checkforcapture+sensorradius;
                
               // if(sensor==209){   std::cout<<"Sensor2: "<<sensor+1 << "/" << NoSensors <<std::endl; }

                AllSensors[sensor] -> setStepOn(Locations[1]);
                //if(sensor==209){std::cout<<"Sensor3: "<<sensor+1 << "/" << NoSensors <<std::endl;}

                
                double disttosensorprevious = sqrt(pow(previousx - sensorx,2)+pow(previousy - sensory,2));
                double disttosensorcurrent = sqrt(pow(currentx - sensorx,2)+pow(currenty - sensory,2));
                
                //if(sensor==209){std::cout<<"Sensor4: "<<sensor+1 << "/" << NoSensors <<std::endl;}

                
                if(disttosensorprevious<checkforcaptureplusradius  && disttosensorcurrent<checkforcaptureplusradius ){
                    //if(sensor==209){std::cout<<"Sensor5: "<<sensor+1 << "/" << NoSensors <<std::endl;}
                    //if(sensor==209){std::cout<< "currentx " << currentx << " currenty " <<currenty <<
                    //                         " previousx "<<previousx << " previousy "<< previousy <<
                    //                        " i "<<i<< " CallWidth[0] "<<CallWidth[0] <<
                    //                        " currentangle "<<currentangle <<" iterationnumber "<<iterationnumber<<std::endl;};
                    AllSensors[sensor] ->CapturesIntersection(currentx,currenty,previousx,previousy,i,CallWidth[0],currentangle,iterationnumber);
                    //if(sensor==209){std::cout<<"Sensor6: "<<sensor+1 << "/" << NoSensors <<std::endl;}
                };// End of if distance close to sensor
                
                //if(sensor==209){
                //    std::cout<<"Sensor7: "<<sensor+1 << "/" << NoSensors <<std::endl;
                //}

            }; // END of sensor for loop
        }; // end of if not first step
        };// END of check movement exists
   // std::cout<<"Exit Sensor " <<std::endl;
};