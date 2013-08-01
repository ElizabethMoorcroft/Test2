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
const double DensityAnimals = 10*pow(10,-6); // XX*10^-6 Density/m^2  => XX Density/KM^2
const double LengthMonitoring = 1.5*(60*60); //Length of monitoring in seconds. XX*(60*60)secs => XX hrs
const double AverageSizeHR = 1; //Average number of animals per roost
const double SpeedCamera  = 30*0.06 ; // => XX m/s =  XX/ 0.06 KM/h  => XX*0.06 m/s =  XX KM/h
const int NoRunIn = 0; // Number of steps before cameras are "activated"
const int NoOfIterations = 1; // No of iterations with the settings
   

//Size of the enviroment
const double Sq_MinX = 0;
const double Sq_MaxX = 7500;
const double Sq_MinY = 0;
const double Sq_MaxY = 7500;

// Centre of the camera circle is in the centre of the environment
const double Cir_CntX = (Sq_MaxX-Sq_MinX)/2+Sq_MinX;
const double Cir_CntY = (Sq_MaxY-Sq_MinY)/2+Sq_MinY;

const double CameraWidth = 90*M_PI/180;

//HR set up value
const double HR_AverageRadius = 1000; //meters
const int HR_SolidBoundaries = 0; // Solid boundaries for HR --- 1 =Yes 0 = No

//Radom number seed
const int Seed =1;

//Animal parameters
const double AnimalSpeed = 6.3; // ms^-1
const int MovementType = 0;
const double ProbChangeMoveState = 0;//Probability of switching from coorelated to non-correlated


//Movement parameters
const double StepLength = 3.5; //Length of each step in seconds
const double StepLengthDist = StepLength*AnimalSpeed;
const double CorrWalkMaxAngleChange = M_PI/36;


//Call parameters
const double Call_halfwidth = 180*M_PI/180;   //Call_halfwidth

//For the attenuation of sound
const double Temp = 25;
const double Hum = 30;
const double Freq = 45.11*pow(10,3);
const double Amp = 126; //Amplitude
const double It = 0;

const int NoSteps = round(LengthMonitoring/StepLength)+ NoRunIn;


const int RandomCameraPlacement =1;

// The number of cameras and the number of steps
// The camera moves around the in a circle switching on and off
// (switching on for 0.35 seconds and off for 3.5 seconds)
// I assume the camera is on for an infinately small length of time during which it can capture
// everything in it's detection area.
// The step length is equal to the length of time the camera is off
// => The number of times the camera switches on is calculated as:
//          length of the study/ step length
// The number of steps is the number of times the camer switches on + the amount of run in time
const int NoCameraTraps = round(LengthMonitoring/StepLength);

// The radius of the camera circle - to calucalte the locations of the cameras
// The circumferance of the circle is: total distnace = total time * camera speed
// The radius is: (circumference/pi)/2 = (circumference/(pi*2))
// => radius = (total time * camera speed/(pi*2))
const double RadiusCameraCircle = (LengthMonitoring*SpeedCamera)/(2*M_PI);

// The angle between camera as seen from the centre of the circle
// Assuming equal distance between the cameras
// Assume the first one is occurs at 0
const double AngleBetweenCameras = (2*M_PI)/(NoCameraTraps-1);



#endif
