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
const double DensityAnimals = 20*pow(10,-6); // XX*10^-6 Density/m^2  => XX Density/KM^2
const int NoOfIterations = 10; // No of iterations with the settings

//Random number seed
const int Seed =1; // initail start seed - becuase this is set makes result repilicable

// Length of simulation
const double LengthMonitoring = 0.5*24*(60*60); //Length of monitoring in seconds. XX*(60*60)secs => XX hrs

//Size of the enviroment
const double Sq_MinX = 0;       // Minimum x value of the environment
const double Sq_MaxX = 7500;    // Maximum x value of the environment
const double Sq_MinY = 0;       // Minimum y value of the environment
const double Sq_MaxY = 7500;    // Maximum y value of the environment

// Sensor width/Radius
const int LengthSW(5);
const double SensorWidth[LengthSW] = {05*M_PI/180,15*M_PI/180,25*M_PI/180,35*M_PI/180, 45*M_PI/180};
const int LengthSR(4);
const double SensorRadius[LengthSR] = {1,5,10,20};


//Movement parameters
const double AnimalSpeed = 0.46;//0.46; // ms^-1
const double StepLength = 5*60; //Length of each step in seconds
const double StepLengthDist = StepLength*AnimalSpeed;
const double CorrWalkMaxAngleChange = 0*M_PI/180; // In Radians. Half width. X°= X*pi/180rads

//Call parameters
const int LengthCW(1);
const double CallWidth[LengthCW] = {180*M_PI/180};

// Total number of steps calculated from values above
const int NoSteps = round(LengthMonitoring/StepLength);

#endif
