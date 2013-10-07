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
const int SaveMovement =0; // Saves the movement of the animals for all simulations // 1-saves; 0-doesn't save
const int SaveCamera = 1; // Saves the camera locations (constant throughout all simulations) // 1-saves; 0-doesn't save

//Simulation values - #Animals, #Iterations
const double DensityAnimals = 2*pow(10,-6); // XX*10^-6 Density/m^2  => XX Density/KM^2
const int NoOfIterations = 1; // No of iterations with the settings

// Length of simulation
const double LengthMonitoring = 1.5*(60*60); //Length of monitoring in seconds. XX*(60*60)secs => XX hrs

//Size of the enviroment
const double Sq_MinX = 0;       // Minimum x value of the environment
const double Sq_MaxX = 7500;    // Maximum x value of the environment
const double Sq_MinY = 0;       // Minimum y value of the environment
const double Sq_MaxY = 7500;    // Maximum y value of the environment

// Camera width/Radius
const double CameraWidth = 180*M_PI/180; // In Radians. Half width. X°= X*pi/180rads
const double DetectorRadius = 11; // meters

//Random number seed
const int Seed =101; // initail start seed - becuase this is set makes result repilicable

//Movement parameters
const double AnimalSpeed = 10; // ms^-1
const double StepLength = 3.5; //Length of each step in seconds
const double StepLengthDist = StepLength*AnimalSpeed;
const double CorrWalkMaxAngleChange = 0*M_PI/180; // In Radians. Half width. X°= X*pi/180rads

//Call parameters
const double Call_halfwidth = 180*M_PI/180;   //Call_halfwidth. In Radians. X°= X*pi/180rads

// Total number of steps calculated from values above
const int NoSteps = round(LengthMonitoring/StepLength);

#endif
