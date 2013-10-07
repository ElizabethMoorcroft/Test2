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
#include "RandNum.h"
#include "CameraTraps.h"
#include "Parameters.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              FILE NAMES                                            ///
//                                                                                                    ///
// 3 functions which are used to create dynamic file names in order to save files.                    ///
// Code needs tiding up a bit                                                                         ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

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
            << ",Density="      << denistyCal
            << ",Speed="        << AnimalSpeed
            << ",Iterations="   << Seed << "-" << maxseed
            << ",StepLength="   << StepLength
            << ",DetectorRadius=" << DetectorRadius
            << ",CallHalfwidth=" << Call_halfwidth
            << ",CameraHalfwidth=" << CameraWidth
            << ",CorrWalkMaxAngleChange=" <<CorrWalkMaxAngleChange
        ;
    return result.str();
};

// Base name each file
std::string SaveDirectory = make_directory("/Users/student/Documents/Bats/Simulations/Bats");

/// END OF FILE NAMES

/*--------------------------------------------------------------------------------------------------
//                                                                                             
//                                         Main code starts                                            
//                                                                                             
---------------------------------------------------------------------------------------------------*/

int main(){
    
    //Clocks total run time
    clock_t t = clock();
    
    //Calculates some values before the before the iteration of loop starts
    double area = (Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY);
    int NoAnimal = floor(DensityAnimals*area);
         
    /*-------------------------------------------------------
     // Creates files for saving - Including headings
     --------------------------------------------------------*/
    //Creates file for Cameras (CSV file) and writes in the header
    std::ofstream Cameras;
    Cameras.open(make_filename(SaveDirectory, ",Cameras.csv" ).c_str());
    if(SaveCamera==1){
        Cameras << "ID" <<
            "," << "X location" <<
            "," << "Y location" <<
            "," << "CentreAngle" <<
            "," << "HalfWidthAngle" <<
            "," << "Camera Speed" <<
            "\n";
        ;
    } else {Cameras << "Not saved";}
    
    //Creates file for Captures (CSV file) and writes in the header
    std::ofstream Captures;
    Captures.open(make_filename(SaveDirectory, ",Captures.csv" ).c_str());
    Captures << "AnimalNumber" <<
        "," << "Time_step" <<
        "," << "CameraID" <<
        "," << "Iteration number" <<
        "," << "X location" <<
        "," << "Ylocation" <<
        "," << "Percentage time within step" <<
        "," << "Call" <<
        "," << "Angle from centre of camera to bat" <<
        "," << "Angle from bat to camera" <<
        "," << "Distance from animal to camera" <<
        "\n";
    
    //Creates file for Movement (CSV file) and writes in the header
    std::ofstream Movement;
    Movement.open(make_filename(SaveDirectory, ",Movement.csv" ).c_str());
    if(SaveMovement==1){
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
            "," << "Iternation number" <<
            "\n";
    } else {Movement << "Not saved";}
    
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
             << "Seed" << "," << Seed << "\n"
             << "Sq_MinX" << ","<<  Sq_MinX << "\n"
             << "Sq_MaxX" << ","<<  Sq_MaxX << "\n"
             << "Sq_MinY" << ","<<  Sq_MinY << "\n"
             << "Sq_MaxY" << ","<<  Sq_MaxY << "\n"
             << "CameraWidth" << "," << CameraWidth<< "\n"
             << "StepLength" << ","<< StepLength << "\n"
             << "CorrWalkMaxAngleChange" << ","<<   CorrWalkMaxAngleChange<< "\n"
             << "AnimalSpeed" << ","<< AnimalSpeed<< "\n"
             << "Call_halfwidth" << ","<< Call_halfwidth << "\n"
             << "DetectorRadius"<<","<<DetectorRadius<<"\n";
    //Closes file
    Settings.close();
    
    ///////////////////////////////////////////////////////////////////////////////////////
    ///                                 !!! WARNINGS!!!                                 ///
    /// Will automatically stop the simulation if certain conditions they               ///
    /// aren't satisfied.                                                               ///
    /// Includes a limited number of tests on the following functions:                  ///
    ///         *CapturesIndividual                                                     ///
    ///////////////////////////////////////////////////////////////////////////////////////
   
    //Number of animals needs to be greater than zero
    if(NoAnimal<=0){
        std::cout<<"No of Animals = "<<NoAnimal<< ", Increase density"<< std::endl; exit (EXIT_FAILURE);
    };
    
    // No steps
    if(NoSteps==0){
        std::cout<<"No steps, Increase Length of monitoring"<< std::endl; exit (EXIT_FAILURE);
    };
    
    //Test on the camera trap
    ///////////THIS NEEDS UPDATING!!!!
    int test=0;
    if(test==1){
        CameraTrap CT1;
        CT1 = CameraTrap(1); // ID
        CT1.TestCI();
        CT1.TestVertAndAngleInteraction();
        CT1.TestHorzAndAngleInteraction();
        CT1.TestAngleAndAngleInteraction();
        CT1.TestHorzAndCircInteraction();
        CT1.TestVertAndCircInteraction();
        CT1.TestAngleAndCircInteraction();
        CT1.TestTimeAndAngleCal();
        CT1.TestGradientFromAngle();
        CT1.TestCapturesIntersection();
    };
    
    ///////////////////////////////////////////////////////////////////////////////////////
    //                          CALCULATES LOCATION OF CAMERA TRAPS                     ///
    //                                                                                  ///
    // Creates Camera traps as they do not change location when the movement changes    ///
    //  - only after the set up parameters change (change in length of study, etc)      ///
    // Saves the locations in a CSV file                                                ///
    ///////////////////////////////////////////////////////////////////////////////////////
    //Creates a list of pointers to the CTs
    CameraTrap CT(0);
    if(SaveCamera==1){
        //Saves the locations and the angle of the camera
        Cameras << CT.getID() << //1st column
            "," << CT.getXloc() << //2nd column
            "," << CT.getYloc() << //...
            "," << CT.getAngle() << //...
            "," << CT.getHalfAngle() << //4th column
            "\n";
    };
    //Closes the csv camera file
    Cameras.close();
    
    // This should not be here - once I know how to test properly then can get ride of this
    //std::cout<<"Finish Camera traps"<<std::endl;
    if(test==0)
    {
        
    /*------------------------------------------------------------------------------------------------
    //                                      Iteration Loop
    //
    // Starts a loop for the rest of the code, this loop "NoOfIterations" number of times
    // Each set of movement traces is saved where the file name ends with the number
    // The seed that starts all each simulation is the number of the simulation plus the start seed
    // This means that can run simulations in blocks 1-10 then 11-20 etc                            
    ------------------------------------------------------------------------------------------------*/
    for(int iterationnumber=Seed; iterationnumber<Seed + NoOfIterations; iterationnumber++){
        std::cout<<"Iteration: "<<iterationnumber<<"/"<<Seed + NoOfIterations<<std::endl;
    
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
        std::vector<double> RandomNumberStreamAnimalStart(NoAnimal);
        for(int i=0; i<NoAnimal; i++){ RandomNumberStreamAnimalStart[i]=double(rand()); };

    
        // Random number stream seed for the seed for the movement
        srand(RandomNumberStream[75]);
        std::vector<double> RandomNumberStreamAnimalMove(NoAnimal);
        for(int i=0; i<NoAnimal; i++){RandomNumberStreamAnimalMove[i]=double(rand());};
        
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
            //Print out animal number to screen
            //std::cout <<"Animal:" << i+1 <<"/" << NoAnimal << std::endl;
            RandNum RandomNumber1;
            srand(RandomNumberStreamAnimalStart[i]);
            std::vector<double> RandomNumberStreamAnimalStartLoc(151);
            for(int i=0; i<151; i++){RandomNumberStreamAnimalStartLoc[i]=double(rand());};
            double xlocation = RandomNumber1.AtoBUnif(RandomNumberStreamAnimalStartLoc[50],Sq_MinX,Sq_MaxX);
            double ylocation = RandomNumber1.AtoBUnif(RandomNumberStreamAnimalStartLoc[100],Sq_MinY,Sq_MaxY);
        
            // To choose a start angle, sets up a random number class
            // Uses a radom number from stream RandomNumberStreamAnimalAngle for a random seed
            double CurrentAngleTemp = RandomNumber1.AtoBUnif(RandomNumberStreamAnimalStartLoc[150],0,(2*M_PI));

            // New animal given start locations - at the centre of the home range
            //  Inputs are: ID & Starts location (x,y) &  Initial angle
            AllAnimals[i] =new Animal(i, xlocation, ylocation, CurrentAngleTemp);

            /*------------------------------------------------------ 
            // Update location
            ---------------------------------------------------------*/ 
            //Sets seed for a random number
            //Random number stream for the movemnet of the animal
            srand(RandomNumberStreamAnimalMove[i]);
            std::vector<double> RandomNumberCurrentAnimal(NoSteps*100);
            
            // For each step, Updates animal location
            double stepcount;
            for(int j=0; j<NoSteps; j++){
                stepcount = j*100;
                AllAnimals[i] -> UpdateLocation(RandomNumberCurrentAnimal[stepcount]);
            }; //End of j loop for Steps
            
            // If the movement is to be saved in the csv file then entre this loop
            if(SaveMovement==1){
                //std::cout <<"Update all locations"<< std::endl;
                // Creates a temp matrix for "all locations"
                std::vector<std::vector<double>> TempAllLocations = AllAnimals[i]->getAllLocations();
                // Temp location file is written in csv file
                // Each location is a seperate row  - the number of rows = "TempAllLocations.size()"
                for(int stepcounter=0; stepcounter<TempAllLocations.size(); stepcounter++){
                    if(TempAllLocations[stepcounter].size()>0){
                        Movement<< TempAllLocations[stepcounter][0] << //1st column, row "stepcounter"
                            "," << TempAllLocations[stepcounter][1] << //2nd column, row "stepcounter"
                            "," << TempAllLocations[stepcounter][2] << //...
                            "," << TempAllLocations[stepcounter][3] << //...
                            "," << TempAllLocations[stepcounter][4] << //...
                            "," << TempAllLocations[stepcounter][5] << //...
                            "," << TempAllLocations[stepcounter][6] << //...
                            "," << TempAllLocations[stepcounter][7] << //8th column, row "stepcounter"
                            "," << iterationnumber <<                  // itertaion number
                            "\n";                                      // New line
                    }; //END of STEPCOUNTER LOOP
                }; //END OF FOR LOOP
            }; // END OF IF LOOP FOR SAVING
        }; //End of i loop for EACH ANIMALS
    
        /*--------------------------------------------------------------
        //                         CAPTURES
        //
        // Calcualtes whether any animals have been captured by cameras
        //  - There is no random numbers used here
        //  - Cycles through each time step for each individual and 
        //     checks whether they are captured
        //  - Saves the Animal ID, Camera ID and Time Step in a CSV file                       
        --------------------------------------------------------------*/
        // For each individual check each camera location and see whether they were captured
        //Each camera is only at a their location for one time interval
        for(int Individual=0; Individual<NoAnimal; Individual++){
            //std::cout <<"Animal:" << Individual+1 <<"/" << NoAnimal << std::endl;

            // The call width of each individual does not change throughout the simulation
            // Therefore it doesn't need to be called for each camera
            double callangle = AllAnimals[Individual]->getCallWidth();
            std::vector<std::vector<double>> TempAllLocations = AllAnimals[Individual]->getAllLocations();
        
            // Variables for calculating captures
            int TimeStepTrap =0;
            double previousx; double previousy;double currentx; double currenty; double currentangle;
            int capturecount;
             
            // The camera only start after the the "Run in period"
            // Then they are only on at the corresponding the time step
            // Camera 0 is only on at NoRunIn, Camera 1 is only on at NoRunIn+1,....
            // The one is added on becuase start counting the number of steps at 0
            for(int time =0; time<NoSteps; time++ ){
                //std::cout <<"Animal:" << Individual+1 <<"/" << NoAnimal << " Time: "<< time<<std::endl;
                TimeStepTrap= time+1;
                if(TempAllLocations[TimeStepTrap].size()>0 && TempAllLocations[TimeStepTrap][7]==0){
                        previousx = TempAllLocations[TimeStepTrap-1][2];
                        previousy = TempAllLocations[TimeStepTrap-1][3];
                        currentx = TempAllLocations[TimeStepTrap][2];
                        currenty = TempAllLocations[TimeStepTrap][3];
                        currentangle = TempAllLocations[TimeStepTrap][4];
                    
                    // Calcualtes whether the animal is captured
                    capturecount = CT.CapturesIntersection(currentx,currenty,previousx,previousy,Individual,callangle,currentangle,iterationnumber);
                
                    }; //END OF IF TempAllLocations
                    // Resets the step on to zero because the cameras are used in the next iteration and they need to
                    // the start step on to be zero. 
                    CT.Add1StepOn();
            };// END OF FOR time LOOP
            
            /*------------------------------------------------------
            // SAVING THE CAPTURES
            ---------------------------------------------------------*/
            // Retreaves all of the captures
            std::vector<std::vector<double>> TempCaptures = CT.getCaptures();
            
            //STarts looking dor the first entry
            int stepcounter=0;
            // Temp location file is written in csv file
            while(TempCaptures[stepcounter].size()==11){
                    Captures<< TempCaptures[stepcounter][0] << //1st column, row
                        "," << TempCaptures[stepcounter][1] << //2nd column, row "stepcounter"
                        "," << TempCaptures[stepcounter][2] << //...
                        "," << TempCaptures[stepcounter][3] <<
                        "," << TempCaptures[stepcounter][4] <<
                        "," << TempCaptures[stepcounter][5] <<
                        "," << TempCaptures[stepcounter][6] <<
                        "," << TempCaptures[stepcounter][7] <<
                        "," << TempCaptures[stepcounter][8] <<
                        "," << TempCaptures[stepcounter][9] <<
                        "," << TempCaptures[stepcounter][10] <<
                        "\n";                                  // New line
                
                    stepcounter+=1;
                }; //End of step counter loop
            CT.resetCaptures();
            // This is done because the StepNo is changed for everytime step and needs to be reset to 0
            CT.ResetStepOn();
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
        
    }; //end if test
    
    std::cout<<"Finished" <<std::endl;
        
    t = clock() - t;
    std::cout<<((float)t)/CLOCKS_PER_SEC<<std::endl;
    
    // Return is null
    return 0;
}; 
/*------------------------------------------------------
///        !!!   END OF CODE   !!!!   
---------------------------------------------------------*/