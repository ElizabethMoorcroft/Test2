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
const int SaveMovement = 0; // Saves the movement of the animals for all simulations // 1-saves; 0-doesn't save
const int SaveSensor = 1; // Saves the Sensor locations (constant throughout all simulations) // 1-saves; 0-doesn't save

//Simulation values - #Animals, #Iterations
const double DensityAnimals = 0.05*pow(10,-6); // XX*10^-6 Density/m^2  => XX Density/KM^2
const int NoOfIterations = 2; // No of iterations with the settings

//Random number seed
const int Seed =1; // initail start seed - becuase this is set makes result repilicable

// Length of simulation
const double LengthMonitoring = 14*24*(60*60); //Length of monitoring in seconds. XX*(60*60)secs => XX hrs

//Size of the enviroment
const double Sq_MinX = 0;        // Minimum x value of the environment
const double Sq_MaxX = 7500;    // Maximum x value of the environment
const double Sq_MinY = 0;        // Minimum y value of the environment
const double Sq_MaxY = 7500;    // Maximum y value of the environment
const double area = (Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY);

// number of animals
const int NoAnimal = round(DensityAnimals*area);

// Sensor width/Radius
const int LengthSW(21);
const double SensorWidth[LengthSW] = {  0*M_PI/180,
                                       05*M_PI/180, 15*M_PI/180, 25*M_PI/180, 35*M_PI/180, 45*M_PI/180,    // <= 90
                                       55*M_PI/180, 65*M_PI/180, 75*M_PI/180, 85*M_PI/180, 90*M_PI/180,    // >  90 && <=180
                                       95*M_PI/180,105*M_PI/180,115*M_PI/180,125*M_PI/180,135*M_PI/180 ,   // > 180 && <=270
                                      145*M_PI/180,155*M_PI/180,165*M_PI/180,175*M_PI/180,180*M_PI/180 };  // > 270 && <=360
const int LengthSR(11);
const double SensorRadius[LengthSR] = {1,2,5,10,15,20,25,30,50,100,500};
const int NoSensors = LengthSW*LengthSR;


//Movement parameters
const double AnimalSpeed = 0.00012;//0.46; // ms^-1
const double StepLength = 5*60; //Length of each step in seconds
const double CorrWalkMaxAngleChange = 0*M_PI/180; // In Radians. Half width. X°= X*pi/180rads
const double perch=0.0; // Proportion of time spent 'perching'

//Call parameters
const int LengthCW(1);
const double CallWidth[LengthCW] = {180*M_PI/180};

// Total number of steps calculated from values above
const int NoSteps = round(LengthMonitoring/StepLength);

#endif
