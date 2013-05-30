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

//Simulation values - #Animals, #Steps, #HR, #CT
const double DensityAnimals = 0.1*pow(10,-6); // XX*10^-6 Density/m^2  => XX Density/KM^2
const double LengthMonitoring = 1.5*(60*60); //Length of monitoring in seconds. XX*(60*60)secs => XX hrs
const double AverageSizeHR = 1; //Average number of animals per roost
const double SpeedCamera  = 30*0.06 ; // => XX m/s =  XX/ 0.06 KM/h  => XX*0.06 m/s =  XX KM/h
const int NoRunIn = 10; // Number of steps before cameras are "activated"
const int NoOfIterations = 1; // No of iterations with the settings


//Size of the enviroment
const double Sq_MinX = 0;
const double Sq_MaxX = 7500;
const double Sq_MinY = 0;
const double Sq_MaxY = 7500;

// Centre of the camera circle is in the centre of the environment
const double Cir_CntX = (Sq_MaxX-Sq_MinX)/2+Sq_MinX;
const double Cir_CntY = (Sq_MaxY-Sq_MinY)/2+Sq_MinY;

const double CameraWidth = M_PI/3;

//HR set up value
const double HR_AverageRadius = 1500; //meters
const int HR_SolidBoundaries = 0; // Solid boundaries for HR --- 1 =Yes 0 = No

//Radom number seed
const int Seed =1;

//Movement parameters
const int StepLength = 3.5; //Length of each step in seconds
const double CorrWalkMaxAngleChange = M_PI/36;

//Animal parameters
const double AnimalSpeed = 6.3; // ms^-1
const int MovementType = 1;
const double ProbChangeMoveState = 0;//Probability of switching from coorelated to non-correlated

//Call parameters
const double Call_halfwidth = M_PI/4;   //Call_halfwidth

//For the attenuation of sound
const double Temp = 25;
const double Hum = 30;
const double Freq = 45.11;
const double Amp = 126; //Amplitude
const double It = 0;

const int NoSteps = round(LengthMonitoring/StepLength)+ NoRunIn;
#endif
