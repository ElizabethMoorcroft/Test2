//
//  Parameters.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 23/05/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#ifndef Test2_Parameters_h
#define Test2_Parameters_h
#include <iostream>
#include <cstdio>
#include <math.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

//////////////////////////////////////////////////
/// Constant parameters used in the simulation ///
//////////////////////////////////////////////////

// Saving settings
const int SaveMovement = 1; // Saves the movement of the animals for all simulations // 1-saves; 0-doesn't save
const int SaveSensor = 1; // Saves the Sensor locations (constant throughout all simulations) // 1-saves; 0-doesn't save

//Simulation values - #Animals, #Iterations
const double DensityAnimals = 1*pow(10,-6); // XX*10^-6 Density/m^2  => XX Density/KM^2
const int NoOfIterations = 1; // No of iterations with the settings

//Random number seed
const int Seed =1; // initail start seed - becuase this is set makes result repilicable

// Length of simulation
   const double LengthMonitoring =   10*24*(60*60); //Length of monitoring in seconds. XX*(60*60)secs => XX hrs
//const double LengthMonitoring = 2*(60*60); //Length of monitoring in seconds. XX*(60*60)secs => XX hrs


//Size of the enviroment
const double Sq_MinX = 0;        // Minimum x value of the environment
const double Sq_MaxX = 750;    // Maximum x value of the environment
const double Sq_MinY = 0;        // Minimum y value of the environment
const double Sq_MaxY = 750;    // Maximum y value of the environment
const double area = (Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY);

// number of animals
const int NoAnimal = round(DensityAnimals*area);

// Sensor width/Radius
//const int LengthSW(7);
//const double SensorWidth[LengthSW] = {  20*M_PI/180, 40*M_PI/180,    // <= 90
//                                        60*M_PI/180, 80*M_PI/180,// >  90 && <=180
//                                        100*M_PI/180 ,   // > 180 && <=270
//                                        160*M_PI/180, 180*M_PI/180 };  // > 270 && <=360

 //Sensor width/Radius
const int LengthSW(1);
const double SensorWidth[LengthSW] = {60*M_PI/180};  // > 270 && <=360 , 100*M_PI/180

const int LengthSR(1);
const double SensorRadius[LengthSR] = {100};
const int NoSensors = LengthSW*LengthSR;


//Movement parameters
const double StepLength =   1; //Length of each step in seconds *60

// Total number of steps calculated from values above
const int NoSteps = round(LengthMonitoring/StepLength);

#endif
