//
//  main.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 23/02/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//
#include <iostream>
#include <cstdio>
#include <math.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>


//Header files
#include "Animal.h"
#include "AnimalTest.h"
#include "RandNum.h"
#include "Sensors.h"
#include "SensorTest.h"
#include "Parameters.h"
#include "CheckSensor.h"
#include "AnimalMovement.h"
#include "Iteration.h"


/*--------------------------------------------------------------------------------------------------
 //
 //                                              File names
 //
 // 3 functions which are used to create dynamic file names in order to save files.
 // Code needs tiding up a bit
 //
 ---------------------------------------------------------------------------------------------------*/

// Function for creating a dynamic file names for settings
std::string make_filename( const std::string& directory ,const std::string& basename){
    std::ostringstream result;
    result << directory <<basename;
    return result.str();
};

// Function for creating the directory/Basename of the file
// The directory is the input - but this part of the code automatically
// updates the name with the type of boundaries, type of movement, and density
std::string make_directory( const std::string& directory,double perchvalue ,double speedvalue ){
    
    // Updates so that the type of boundary and 
    std::string boundary;
    double denistyCal = DensityAnimals*pow(10,6);
    int maxseed = NoOfIterations+Seed;
    std::ostringstream result;
    result  << directory
            << "Perch"          << perchvalue
            << ",Density="      << denistyCal
            << ",Speed="        << speedvalue
            << ",Iterations="   << Seed << "-" << maxseed
            << ",StepLength="   << StepLength
            << ",CorrWalkMaxAngleChange=" <<CorrWalkMaxAngleChange
        ;
    return result.str();
};


/// END OF FILE NAMES

/*--------------------------------------------------------------------------------------------------
//                                                                                             
//                                         Main code starts                                            
//                                                                                             
---------------------------------------------------------------------------------------------------*/

int main(){
    
    double perchmatrix[2] = {0,0.5};
    double speedmatrix[2] = {0.00012,0.46};
    double anglematrix[4] = {0,45,90,180};
    
    
    for(int simulationnumber1=0; simulationnumber1<sizeof(perchmatrix); simulationnumber1++){
    for(int simulationnumber2=0; simulationnumber2<sizeof(speedmatrix); simulationnumber2++){
    for(int simulationnumber3=0; simulationnumber3<sizeof(anglematrix); simulationnumber3++){
        
        double perchvalue= perchmatrix[simulationnumber1];
        double speedvalue= speedmatrix[simulationnumber2];
        double maxchangeanglevalue = anglematrix[simulationnumber3];
        
        std::string savevalue = make_directory("/Users/student/Documents/Bats/Simulations/Run23Oct2013", perchvalue, speedvalue);
        std::string returnvalue = make_directory("/Users/student/Dropbox/SimulationReached", perchvalue, speedvalue);
        
        
        /*-------------------------------------------------------
         // Creates files for saving - Including headings
         --------------------------------------------------------*/
        //Creates file for Sensors (CSV file) and writes in the header
        std::ofstream Sensors;
        Sensors.open(make_filename(savevalue, ",Sensors.csv" ).c_str());
        Sensors << "ID" <<
            "," << "X location" <<
            "," << "Y location" <<
            "," << "CentreAngle" <<
            "," << "HalfWidthAngle" <<
            "," << "Radius" <<
            "\n";
    
        //Creates file for Captures (CSV file) and writes in the header
        std::ofstream CapturesNotRef;
        std::ofstream &Captures = CapturesNotRef;
        Captures.open(make_filename(savevalue, ",Captures.csv" ).c_str());
        Captures << "AnimalNumber" <<
            "," << "Time_step" <<
            "," << "SensorID" <<
            "," << "Iteration number" <<
            "," << "Angle from animal to sensor" <<
            "\n";
    
        //Creates file for Movement (CSV file) and writes in the header
        std::ofstream MovementNotRef;
        std::ofstream &Movement = MovementNotRef;
        Movement.open(make_filename(savevalue, ",Movement.csv" ).c_str());
        Movement << "AnimalNumber" <<
            "," << "StepNumber" <<
            "," << "Xlocation" <<
            "," << "Ylocation" <<
            "," << "Angle" <<
            "," << "TotalDistance" <<
            "," << "Speed" <<
            "," << "Re-enterWorld" <<
            "," << "Dist" <<
            "," << "Iternation number" <<
            "\n";
    
        // Saves the settings used
        std::ofstream Settings;
        Settings.open(make_filename(savevalue,",Settings.csv").c_str());
        //Simulation values - #Animals, #Steps,  #CT
        Settings << "DensityAnimals" << ","<< DensityAnimals << "\n"
             << "NoOfAnimals" << "," << NoAnimal << "\n"
             << "Area"<< "," << area << "\n"
             << "LengthMonitoring" << "," << LengthMonitoring << "\n"
             << "NoOfIterations" << "," << NoOfIterations << "\n"
             << "NoSteps"   << "," << NoSteps << "\n"
             << "StepLength"   << "," << StepLength << "\n"
             << "Seed" << "," << Seed << "\n"
             << "Sq_MinX" << ","<<  Sq_MinX << "\n"
             << "Sq_MaxX" << ","<<  Sq_MaxX << "\n"
             << "Sq_MinY" << ","<<  Sq_MinY << "\n"
             << "Sq_MaxY" << ","<<  Sq_MaxY << "\n"
             << "CorrWalkMaxAngleChange" << ","<<   CorrWalkMaxAngleChange<< "\n"
             << "AnimalSpeed" << ","<< AnimalSpeed<< "\n"
            ;
        //Closes file
        Settings.close();
    
        /*--------------------------------------------------------------------------------------------------
         //
         //                                 !!! WARNINGS && TESTS !!!
         //
         // Will automatically stop the simulation if certain conditions they aren't satisfied.
         // Runs automated unit tests on:
         //     * Animal
         //     * Sensor
         //
         ---------------------------------------------------------------------------------------------------*/

   
        //Number of animals needs to be greater than zero
        if(NoAnimal<=0){
            std::cout<<"No of Animals = "<<NoAnimal<< ", Increase density"<< std::endl; exit (EXIT_FAILURE);
        };
        // No steps
        if(NoSteps==0){
            std::cout<<"No steps, Increase Length of monitoring"<< std::endl; exit (EXIT_FAILURE);
        };
        
        //Tests
        //SensorTest ST1;
        //ST1.RunSensorTests();
        //AnimalTest AT1;
        //AT1.RunAnimalTests();
    
        std::cout<<"Passed all tests and checks"<< std::endl;
        //Clocks total run time

        /*--------------------------------------------------------------------------------------------------
         //                                          Creating sensors
        //
         // Creates Sensors as they do not change location when the movement changes
         //  - only after the set up parameters change (change in length of study, etc)
         // Saves the locations in a CSV file
         ---------------------------------------------------------------------------------------------------*/
        //Creates a list of pointers to the Sensors
    
        std::vector<Sensor*> AllSensors(NoSensors);
    
        int sensorcount(0);
        for(int sensor1 =0; sensor1<LengthSW; sensor1 ++){
            for(int sensor=0; sensor<LengthSR; sensor++){
            
                AllSensors[sensorcount] =new Sensor(sensorcount,SensorWidth[sensor1],SensorRadius[sensor]);
    
    
                if(SaveSensor==1){
                //Saves the locations and the angle of the Sensor
                Sensors << AllSensors[sensorcount] -> getID() << //1st column
                    "," << AllSensors[sensorcount] -> getXloc() << //2nd column
                    "," << AllSensors[sensorcount] -> getYloc() << //...
                    "," << AllSensors[sensorcount] -> getAngle() << //...
                    "," << AllSensors[sensorcount] -> getHalfAngle() << //4th column
                    "," << AllSensors[sensorcount] -> getRadius() << //4th column
                    "\n";
                sensorcount +=1;
                };
            };
        };
        //Closes the csv Sensor file
        Sensors.close();
    
        /*------------------------------------------------------------------------------------------------
         //                                      Iteration Loop
         //
         // Starts a loop for the rest of the code, this loop "NoOfIterations" number of times
         // Each set of movement traces is saved where the file name ends with the number
         // The seed that starts all each simulation is the number of the simulation plus the start seed
         // This means that can run simulations in blocks 1-10 then 11-20 etc
         ------------------------------------------------------------------------------------------------*/
        for(int iterationnumber=Seed; iterationnumber<Seed + NoOfIterations; iterationnumber++){
        
            Iteration::Iteration(AllSensors, Captures, Movement, iterationnumber, speedvalue, perchvalue, maxchangeanglevalue);
        
        };//End of iteration
    
        // Closes files that are open in all iterations
        Captures.close();
        Movement.close();
        CapturesNotRef.close();
        MovementNotRef.close();
        
        std::ofstream Return;
        Return.open(make_filename(returnvalue, ",finished.csv" ).c_str());
        Return.close();

    
    };};};
    std::cout<<"Finished" <<std::endl;
    
    
    // Return is null
    return 0;
}; 
/*------------------------------------------------------
///        !!!   END OF CODE   !!!!   
---------------------------------------------------------*/