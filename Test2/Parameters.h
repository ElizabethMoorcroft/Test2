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
const int SaveMovement =1; // Saves the movement of the animals for all simulations // 1-saves; 0-doesn't save
const int SaveHR = 0;// Saves the Home range values for each all, all simulations// 1-saves; 0-doesn't save
const int SaveCamera = 1; // Saves the camera locations (constant throughout all simulations) // 1-saves; 0-doesn't save

//Simulation values - #Animals, #Steps, #HR, #CT
const double DensityAnimals = 1*pow(10,-6); // XX*10^-6 Density/m^2  => XX Density/KM^2
const double AverageSizeHR = 1; //Average number of animals per roost/HR
const int NoOfIterations = 10; // No of iterations with the settings

// Time
const int NoRunIn = 0; // Number of steps before cameras are "activated"
const double LengthMonitoring = 1.5*(60*60); //Length of monitoring in seconds. XX*(60*60)secs => XX hrs


//HR set up value
const double HR_AverageRadius = 1000; //meters
const int HR_SolidBoundaries = 0; // Solid boundaries for HR --- 1 =Yes 0 = No

//Size of the enviroment
const double Sq_MinX = 0;
const double Sq_MaxX = 7500;
const double Sq_MinY = 0;
const double Sq_MaxY = 7500;


// Call rates
const int callson = 0;
const double CallsPerStep = 3.5*60;
const double IntervalBetweenCalls = 1/CallsPerStep;

// Centre of the camera circle is in the centre of the environment
const double DetectorLayOut = 0; // 0 is a single stationary; 1 is a grid;

// The grid will be a  a standard HR away from the edge of the world
// A value for HR must be entered even if this is not required for movement
const double MaxNoX = 2;
const double MaxNoY = 1;
const double Xgridmin = Sq_MinX + HR_AverageRadius;
const double Ygridmin = Sq_MinY + HR_AverageRadius;
const double Xspace = (Sq_MaxX - Sq_MinX - 2*HR_AverageRadius)/(MaxNoX+1);
const double Yspace = (Sq_MaxY - Sq_MinY - 2*HR_AverageRadius)/(MaxNoY+1);

// Camera width/Radius
const double CameraWidth = 180*M_PI/180; // In Radians. Half width. X°= X*pi/180rads
const double DetectorRadius = 11; // meters

//Radom number seed
const int Seed =1;

//Animal parameters
const double AnimalSpeed = 10; // ms^-1
const double ProbChangeMoveState = 0;//Probability of switching from coorelated to non-correlated
const double CorrWalkMaxAngleChange = 0*M_PI/180; // In Radians. Half width. X°= X*pi/180rads


//Movement parameters
const double StepLength = 3.5; //Length of each step in seconds
const double StepLengthDist = StepLength*AnimalSpeed;


//Call parameters
const double Call_halfwidth = 180*M_PI/180;   //Call_halfwidth. In Radians. X°= X*pi/180rads

// Total number of steps calculated from values above
const int NoSteps = round(LengthMonitoring/StepLength)+ NoRunIn;




#endif
