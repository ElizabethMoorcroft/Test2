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
std::string make_directory( const std::string& directory){
    
    // Updates so that the type of boundary and 
    std::string boundary;
    double denistyCal = DensityAnimals*pow(10,6);
    int maxseed = NoOfIterations+Seed;
    std::ostringstream result;
    result  << directory
            << "Perch"          << perch
            << ",Density="      << denistyCal
            << ",Speed="        << AnimalSpeed
            << ",Iterations="   << Seed << "-" << maxseed
            << ",StepLength="   << StepLength
            << ",CorrWalkMaxAngleChange=" <<CorrWalkMaxAngleChange
        ;
    return result.str();
};

// Base name each file
std::string SaveDirectory = make_directory("/Users/student/Documents/Bats/Simulations/Run23Oct2013");

/// END OF FILE NAMES

/*--------------------------------------------------------------------------------------------------
//                                                                                             
//                                         Main code starts                                            
//                                                                                             
---------------------------------------------------------------------------------------------------*/

int main(){
    
    //Clocks total run time
    //clock_t t = clock();
    
    //Calculates some values before the before the iteration of loop starts
    double area = (Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY);
    int NoAnimal = round(DensityAnimals*area);
    
    /*-------------------------------------------------------
     // Creates files for saving - Including headings
     --------------------------------------------------------*/
    //Creates file for Sensors (CSV file) and writes in the header
    std::ofstream Sensors;
    Sensors.open(make_filename(SaveDirectory, ",Sensors.csv" ).c_str());
    if(SaveSensor==1){
        Sensors << "ID" <<
            "," << "X location" <<
            "," << "Y location" <<
            "," << "CentreAngle" <<
            "," << "HalfWidthAngle" <<
            "," << "Radius" <<
            "\n";
    } else {Sensors << "Not saved";}
    
    //Creates file for Captures (CSV file) and writes in the header
    std::ofstream CapturesNotRef;
    std::ofstream &Captures = CapturesNotRef;
    Captures.open(make_filename(SaveDirectory, ",Captures.csv" ).c_str());
    Captures << "AnimalNumber" <<
        "," << "Time_step" <<
        "," << "SensorID" <<
        "," << "Iteration number" <<
       // "," << "X location" << //5
       // "," << "Ylocation" << //6
       // "," << "Percentage time within step" << //7
       // "," << "Angle from centre of sensor to animal" << //8
        "," << "Angle from animal to sensor" <<
       // "," << "Distance from animal to sensor" << //10
        "\n";
    
    //Creates file for Movement (CSV file) and writes in the header
    std::ofstream MovementNotRef;
    std::ofstream &Movement = MovementNotRef;
    Movement.open(make_filename(SaveDirectory, ",Movement.csv" ).c_str());
   // if(SaveMovement==1){
        //Creates a header for the file
        //This is not necessary but helps
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
   // } else {Movement << "Not saved";}
    
    // Saves the settings used
    std::ofstream Settings;
    Settings.open(make_filename(SaveDirectory,",Settings.csv").c_str());
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
    clock_t t = clock();

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
        std::cout<<"Iteration: "<<iterationnumber<<"/"<<Seed + NoOfIterations -1 <<std::endl;
    
        /*-------------------------------------------------------
         // START OF RANDOM NUMBER STREAM
         // 
         // Creates the start of the random number stream 
         // This is started with the iteration number
         // This makes each run completely replicable
         //
         // This stream of numbers 4 other streams
         //  - RandomNumberStreamHR (Finding HR locations)
         //  - RandomNumberStreamAnimalHR (matching Animal and HR)
         //  - RandomNumberStreamAnimalAngle (random direction)
         //  - RandomNumberStreamAnimalMove (Seed for movement)
        --------------------------------------------------------*/
        srand(iterationnumber);
        std::vector<double> RandomNumberStream(101);
        for(int i=0; i<101; i++){RandomNumberStream[i] = double (rand());};
     
        /*------------------------------------------------------  
        //              Creation of animals and movement
        //
        // Creates animals
        // Simulates their movement
        // (Saves this to files)
        //
        //  - Vector of pointers to Animals
        //  - 3 Random streams created using seeds from initail stream
        //        * Stream for choosing a suitable HOME RANGE
        //        * Stream for choosing a suitable START ANGLE
        //        * Stream for seeding a new stream for all subsequent MOVEMENT
        //  - Creates CSV documents with save animal parameters
        //  - Creates CSV documents with save animal movements
        //  - Allocates start location, simulates movement                                     
        ---------------------------------------------------------*/ 
        //Creates a vecter of pointers to individuals
        std::vector<Animal*> AllAnimals(NoAnimal);
    
        //List of random number for Animals
        //Uses RandomNumberStream as a seed for the stream
        //Creates 1 Random number for the seed each Animal for the following
        //Random number stream for choosing HR
        srand(RandomNumberStream[25]);
        std::vector<double> RandomNumberStreamAnimalStart(pow(NoAnimal,2));
        for(int i=0; i<pow(NoAnimal,2); i++){ RandomNumberStreamAnimalStart[i]=double(rand()); };

    
        // Random number stream seed for the seed for the movement
        srand(RandomNumberStream[75]);
        std::vector<double> RandomNumberStreamAnimalMove(pow(NoAnimal,2));
        for(int i=0; i<pow(NoAnimal,2); i++){RandomNumberStreamAnimalMove[i]=double(rand());};
        
        /*------------------------------------------------------
         // Creating  animals and animal movement
         //
         //     - Allocates HR
         //         * if there is 1 animal per HR checks there are
         //             no HR allocated twice
         //     - Calculates a random angle 
         //     - Creates animal
         //     - Updates location
         //     - Saves movement
         ---------------------------------------------------------*/
        for(int i=0; i<NoAnimal; i++){
            double randomstart = RandomNumberStreamAnimalStart[i*NoAnimal];
            double randommove = RandomNumberStreamAnimalMove[i*NoAnimal];
            AnimalMovement::AnimalMovement( AllAnimals , AllSensors ,  Captures,  Movement, randomstart,  randommove, i, iterationnumber);
                        
        }; //End of Individual loop
        
        /*------------------------------------------------------
         // Destructors - Destructs the vectors of pointers to classes
         ---------------------------------------------------------*/
        // Destructors For Animals
        for(int i=0; i<NoAnimal; i++){delete AllAnimals[i];}
        
    /*------------------------------------------------------
    //         !!!   END OF ITERATION   !!!!
    ---------------------------------------------------------*/
    };//End of iteration
    
    // Closes files that are open in all iterations
    Captures.close();
    Movement.close();
        
    //}; //end if test
    
    std::cout<<"Finished" <<std::endl;
        
    t = clock() - t;
    std::cout<<((float)t)/CLOCKS_PER_SEC<<std::endl;
    
    // Return is null
    return 0;
}; 
/*------------------------------------------------------
///        !!!   END OF CODE   !!!!   
---------------------------------------------------------*/