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
const int LengthMonitoring = 45*(60*60); //Length of monitoring in seconds
const int AverageSizeHR = 1;
const int SpeedCamera  = 2.4; // m/s = 0.06 * KM/h
const int NoRunIn = 10;
const int NoOfIterations = 3;



//Range of animal start locations if no HR boudaries
const int Sq_MinX = 0;
const int Sq_MaxX = 500000;
const int Sq_MinY = 0;
const int Sq_MaxY = 500000;

// Centre of the camera circle is in the centre of the environment
const int Cir_CntX = (Sq_MaxX-Sq_MinX)/2+Sq_MinX;
const int Cir_CntY = (Sq_MaxY-Sq_MinY)/2+Sq_MinY;

const int CameraWidth = M_PI/3;

// Range of animal start locations if HR boudaries - and cicular camera formulations
const int Cir_MinRng = 5;
const int Cir_MaxRng = 50;

//HR set up values
const int HR_AverageRadius = 500;
const int HR_SolidBoundaries = 1; // Solid boundaries for HR --- 1 =Yes 0 = No

//Radom number seed
//std::vector<int> Startseeds;
const int LengthStream = 10;

//Movement parameters
const int StepLength = 3.5; //Length of each step in seconds
const double CorrWalkMaxAngleChange = M_PI/36;

//Animal parameters
const int AnimalSpeed = 5; // ms^-1
const int MovementType =2;
const double ProbChangeMoveState = 0.01;//Probability of switching from coorelated to non-correlated

//Call parameters
const int Call_halfwidth = 45;   //Call_halfwidth

//For the attenuation of sound
const int Temp = 25;
const int Hum = 30;
const int Freq = 50;
const int Amp = 127; //Amplitude
const int It = 0;

#endif
