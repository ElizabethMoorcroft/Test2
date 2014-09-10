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
<<<<<<< HEAD
const int SaveMovement =0; // Saves the movement of the animals for all simulations // 1-saves; 0-doesn't save
const int SaveHR = 0;// Saves the Home range values for each all, all simulations// 1-saves; 0-doesn't save
const int SaveCamera = 1; // Saves the camera locations (constant throughout all simulations) // 1-saves; 0-doesn't save

//Simulation values - #Animals, #Steps, #HR, #CT
const double DensityAnimals = 70*pow(10,-6); // XX*10^-6 Density/m^2  => XX Density/KM^2
const double AverageSizeHR = 1; //Average number of animals per roost/HR
const int NoOfIterations = 1; // No of iterations with the settings

// Time
const int NoRunIn = 0; // Number of steps before cameras are "activated"
const double LengthMonitoring = 60*24*(60*60); //Length of monitoring in seconds. XX*(60*60)secs => XX hrs
=======
const int SaveMovement = 0; // Saves the movement of the animals for all simulations // 1-saves; 0-doesn't save
const int SaveSensor = 1; // Saves the Sensor locations (constant throughout all simulations) // 1-saves; 0-doesn't save

//Simulation values - #Animals, #Iterations
const double DensityAnimals = 0.5*pow(10,-6); // XX*10^-6 Density/m^2  => XX Density/KM^2
const int NoOfIterations = 1; // No of iterations with the settings

//Random number seed
const int Seed =1; // initail start seed - becuase this is set makes result repilicable
>>>>>>> bats

// Length of simulation
   const double LengthMonitoring =   10*24*(60*60); //Length of monitoring in seconds. XX*(60*60)secs => XX hrs
//const double LengthMonitoring = 2*(60*60); //Length of monitoring in seconds. XX*(60*60)secs => XX hrs


//Size of the enviroment
const double Sq_MinX = 0;        // Minimum x value of the environment
const double Sq_MaxX = 7500;    // Maximum x value of the environment
const double Sq_MinY = 0;        // Minimum y value of the environment
const double Sq_MaxY = 7500;    // Maximum y value of the environment
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

<<<<<<< HEAD
// The grid will be a  a standard HR away from the edge of the world
// A value for HR must be entered even if this is not required for movement
const double MaxNoX = 1;
const double MaxNoY = 1;
const double Xgridmin = Sq_MinX + HR_AverageRadius;
const double Ygridmin = Sq_MinY + HR_AverageRadius;
const double Xspace = (Sq_MaxX - Sq_MinX - 2*HR_AverageRadius)/(MaxNoX+1);
const double Yspace = (Sq_MaxY - Sq_MinY - 2*HR_AverageRadius)/(MaxNoY+1);

// Camera width/Radius
const double CameraWidth = 90*M_PI/180; // In Radians. Half width. X°= X*pi/180rads
const double DetectorRadius = 20; // meters

//Radom number seed
const int Seed =1;

//Animal parameters
const double AnimalSpeed = 0.46; // ms^-1
const double ProbChangeMoveState = 0;//Probability of switching from coorelated to non-correlated
const double CorrWalkMaxAngleChange = 0*M_PI/180; // In Radians. Half width. X°= X*pi/180rads
=======
const int LengthSR(1);
const double SensorRadius[LengthSR] = {500};
const int NoSensors = LengthSW*LengthSR;
>>>>>>> bats


//Movement parameters
const double StepLength =   5*60; //Length of each step in seconds *60

// Total number of steps calculated from values above
const int NoSteps = round(LengthMonitoring/StepLength);

#endif
