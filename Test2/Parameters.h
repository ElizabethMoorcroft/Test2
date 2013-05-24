//
//  Parameters.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 23/05/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#ifndef Test2_Parameters_h
#define Test2_Parameters_h


//////////////////////////////////////////////////
/// Constant parameters used in the simulation ///
//////////////////////////////////////////////////

//Simulation values - #Animals, #Steps, #HR, #CT
const int NoAnimal = 10;
const int LengthMonitoring = 12; //Length of monitoring in seconds
const int AverageSizeHR = 5;
const int SpeedCamera  = 2.4; // m/s = 0.06 * KM/h
const int NoRunIn = 10;

//Range of animal start locations if no HR boudaries
const int Sq_MinX = 0;
const int Sq_MaxX = 100;
const int Sq_MinY = 0;
const int Sq_MaxY = 100;

// Centre of the camera circle
const int Cir_CntX = 0;
const int Cir_CntY = 0;

const int CameraWidth = M_PI/3;

// Range of animal start locations if HR boudaries - and cicular camera formulations
const int Cir_MinRng = 5;
const int Cir_MaxRng = 50;

//HR set up values
const int HR_AverageRadius = 1000;
const int HR_SolidBoundaries = 0; // Solid boundaries for HR --- 1 =Yes 0 = No

//Radom number seed
const int Seed = 1;
const int LengthStream = 10;

//Movement parameters
const int StepLength = 2; //Length of each step in seconds

//Animal parameters
const int AnimalSpeed = 5; // ms^-1


//Call parameters
const int CallFreq = 10;        // Call_freq = p;
const int Call_halfwidth = 45;   //Call_halfwidth

//For the attenuation of sound
const int temp = 25;
const int hum = 30;
const int freq = 50;
const int I0 = 127; //Amplitude
const int It = 0;

#endif
