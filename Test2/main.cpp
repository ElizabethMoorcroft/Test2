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
#include "Simulation.h"


<<<<<<< HEAD
// Function for creating the directory/Basename of the file
// The directory is the input - but this part of the code automatically
// updates the name with the type of boundaries, type of movement, and density
std::string make_directory( const std::string& directory){
    
    // Updates so that the type of boundary and 
    std::string boundary;
    if(HR_SolidBoundaries==0){boundary=",NoBoundaries";} else{boundary=",Boundaries";}
    double denistyCal = DensityAnimals*pow(10,6);
    int maxseed = NoOfIterations+Seed;
    std::ostringstream result;
    result  << directory << boundary
            << ",Density="      << denistyCal
            << ",Speed="        << AnimalSpeed
            << ",Iterations="   << Seed << "-" << maxseed
            << ",StepLength="   << StepLength
            << ",DetectorRadius=" << DetectorRadius
            << ",CallHalfwidth=" << Call_halfwidth
            << ",CameraHalfwidth=" << CameraWidth
            << ",CorrWalkMaxAngleChange=" <<CorrWalkMaxAngleChange
            << ",ProbChangeMoveState=" << ProbChangeMoveState
        ;
    return result.str();
};

// Base name each file
std::string SaveDirectory = make_directory("/Users/student/Documents/Bats/Simulations/Test");

/// END OF FILE NAMES

////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                             ////
///                                 Main code starts                                            ////
///                                                                                             ////
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){
    
    clock_t t = clock();
    //Calculates some values before the before the iteration of loop starts
    int NoCameraTraps = MaxNoX * MaxNoY;    
    double area = (Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY);
    int NoAnimal = floor(DensityAnimals*area);
         
    // The number of the HR's no of expected HR given the number of animals and the average size of HR
    int NoHR = ceil(NoAnimal/AverageSizeHR);
        
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
=======
void simulatevalues(std::vector<double> Values){
    Simulation(Values[0],Values[1],Values[2]);
};

/*--------------------------------------------------------------------------------------------------
//                                                                                             
//                                         Main code starts                                            
//                                                                                             
---------------------------------------------------------------------------------------------------*/

int main(){
    
    clock_t t;  clock_t t2;
    t = clock();
>>>>>>> bats
    
    std::vector<double> Values1(3);
    Values1[0] = 0; Values1[1] = 0.46; Values1[2] =  0*M_PI/180;
    simulatevalues(Values1);
    std::cout<<"Finished" <<std::endl;
    t2 = clock();

<<<<<<< HEAD
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
    std::vector<CameraTrap*> All_CT(NoCameraTraps);
    //Caluclates the location of all the camera traps
    for(int i=0; i<NoCameraTraps; i++){
        //std::cout<<"Camera: "<<i+1 <<"/"<<NoCameraTraps <<std::endl;
        // Calculate the starting position of all Camera traps
        All_CT[i] = new CameraTrap(i);
        if(SaveCamera==1){
            //Saves the locations and the angle of the camera
            Cameras << All_CT[i]->getID() << //1st column
                "," << All_CT[i]->getXloc() << //2nd column
                "," << All_CT[i]->getYloc() << //...
                "," << All_CT[i]->getAngle() << //...
                "," << All_CT[i]->getHalfAngle() << //4th column
                "\n";
        };
    };
    //Closes the csv camera file
    Cameras.close();
    
    // This should not be here - once I know how to test properly then can get ride of this
    std::cout<<"Finish Camera traps"<<std::endl;
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
        //              Home Range locations
        //
        //  - Creates home ranges
        //  - Creates a random number stream to seed home range 
        //    locations
        //  - Creates locations for HR and saves the locations
        -------------------------------------------------------*/
        //Creates a vecter of pointers to Home ranges
        std::vector<HomeRange*> AllHR(NoHR);
        // A list of all the current locations
        std::vector<std::vector<double>> CurrentHRLocation(NoHR);
    
        //List of random number for HRs
        //Uses RandomNumberStream as a seed for the stream
        //Creates 1 Random number for the seed each HR
        srand(RandomNumberStream[0]);
        std::vector<double> RandomNumberStreamHR(NoHR);
        for(int i=0; i<NoHR; i++){RandomNumberStreamHR[i] = rand();};
    
        // For when multiple animals cannot share a home range
        // they have to be allocated seperate ones
        // This is a vector the if the home range is used it is recorded
        // all empty spaces have a value of -1
        std::vector<int> HomeRangeIDused(NoHR);
        for(int i=0; i<NoHR; i++){HomeRangeIDused[i]= -1;};
    
        //Enters data for NoHR home ranges
        for(int i=0; i<NoHR; i++){
            //std::cout<< "HomeRange: " <<i <<"/"<<NoHR<<std::endl;
            AllHR[i] =new HomeRange(i, RandomNumberStreamHR[i]); // ID and Random number
            // if the locations of the HR are saved then this code is initalised
            if(SaveHR==1){
                HomeRangefile<< iterationnumber <<
                        "," << i <<
                        "," << AllHR[i]->getHomeX()<<
                        "," << AllHR[i]->getHomeY()<<
                        "," << AllHR[i]->getHomeSize()<<
                        "\n";
            }; // END OF IF LOOP
        }; // END OF constructor home ranges FOR LOOP
    
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
        std::vector<double> RandomNumberStreamAnimalHR(NoAnimal);
        for(int i=0; i<NoAnimal; i++){ RandomNumberStreamAnimalHR[i]=double(rand()); };
    
        // Random number stream seed for start angle
        srand(RandomNumberStream[50]);
        std::vector<double> RandomNumberStreamAnimalAngle(NoAnimal);
        for(int i=0; i<NoAnimal; i++){RandomNumberStreamAnimalAngle[i]=double(rand());};
    
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
            std::cout <<"Animal:" << i+1 <<"/" << NoAnimal << std::endl;
        
            // Sets seed for a random number so that can choose a home range centre at random
            srand(RandomNumberStreamAnimalHR[i]);
            int   HrId = rand() % NoHR;
        
            // If Home ranges have only 1 animal (or animals to be randomly distributed)
            // Then we need to make sure that each animal is allocated a different home range/start location
            // If HrId already in the vector "HomeRangeIDused" then it needs to be recalulated by,
            // finding the next free HR is allocated instead.
            // Temporary values
            int count_while=1; int HRID_Temp; int HomeRangeIDused_Temp = 0;
            if(AverageSizeHR == 1){
                while(HomeRangeIDused_Temp == 0){
                    count_while +=1;
                    HRID_Temp = 0;
                    // Checks whether "HrId" has been allocated to another animal
                    for(int HRcheck=0; HRcheck<NoHR; HRcheck++){if(HomeRangeIDused[HRcheck]== HrId){HRID_Temp +=1;};}
                    // If it has been allocated then recal
                    if(HRID_Temp !=0){
                        HrId +=1;
                        if(HrId>=NoHR){HrId = 0;};
                        // If it has NOT been allocated then exists the while loop and saves into the vector of allocated HR locations
                    }
                    else{HomeRangeIDused_Temp =1;HomeRangeIDused[i]= HrId;};
                }; // END OF WHILE LOOP
            }; // END OF IF
        
            // the x/y location of the HR
            // All animals start at the centre of there HR
            double xlocation = AllHR[HrId]->getHomeX();
            double ylocation = AllHR[HrId]->getHomeY();
        
            // To choose a start angle, sets up a random number class
            // Uses a radom number from stream RandomNumberStreamAnimalAngle for a random seed
            RandNum Number1;
            double CurrentAngleTemp = Number1.AtoBUnif(RandomNumberStreamAnimalAngle[i],0,(2*M_PI));

            // New animal given start locations - at the centre of the home range
            // ????? Can start anywhere - within HR radius  => Remove need for Run in period ????
            //  Inputs are: ID & HR ID & Starts location (x,y) &  Initial angle & Home range centre (x,y)
            AllAnimals[i] =new Animal(i ,HrId ,xlocation, ylocation, CurrentAngleTemp, xlocation, ylocation);

            /*------------------------------------------------------ 
            // Update location
            ---------------------------------------------------------*/ 
            //Sets seed for a random number
            //Random number stream for the movemnet of the animal
            srand(RandomNumberStreamAnimalMove[i]);
            std::vector<double> RandomNumberCurrentAnimal(NoSteps*100+1);
            for(int i=0; i<NoSteps*100+1; i++){ RandomNumberCurrentAnimal[i]=double(rand()); };
            
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
            std::vector<std::vector<double>> TempAllLocations = AllAnimals[Individual]->getEndStepLocations();
        
            // Variables for calculating captures
            int TimeStepTrap =0;
            double previousx; double previousy;double currentx; double currenty; double currentangle;
            int capturecount; double dist; double callx; double cally;
        
            for(int NoCT=0; NoCT<NoCameraTraps; NoCT++){ //Checks all CT for presence of animal
                //std::cout <<"CT:" << NoCT+1 <<"/" <<NoCameraTraps << std::endl;            
                // The camera only start after the the "Run in period"
                // Then they are only on at the corresponding the time step
                // Camera 0 is only on at NoRunIn, Camera 1 is only on at NoRunIn+1,....
                // The one is added on becuase start counting the number of steps at 0
                for(int time =0; time<NoSteps; time++ ){
                    TimeStepTrap= time+1;
                    if(TempAllLocations[TimeStepTrap].size()>0){
                        previousx = TempAllLocations[TimeStepTrap-1][2];
                        previousy = TempAllLocations[TimeStepTrap-1][3];
                        currentx = TempAllLocations[TimeStepTrap][2];
                        currenty = TempAllLocations[TimeStepTrap][3];
                        currentangle = TempAllLocations[TimeStepTrap][4];
                    
                        // Calcualtes whether the animal is captured
                        capturecount = All_CT[NoCT]->CapturesIntersection(currentx,currenty
                                                                          ,previousx ,previousy
                                                                          ,Individual
                                                                          ,callangle,currentangle
                                                                          ,iterationnumber
                                                                          );
                    
                        if(capturecount>0 && callson==1){
                            // if there have been any captures then it must have past throught the
                            // the area, and if there are no captures then it won't have pasted through at all.
                            // The bats are not constantly calling and therefore
                            // calculate calls throughout the steps which it passes through the camera
                            for(int i=0; i<CallsPerStep; i++){
                                dist = sqrt(pow(previousx-currentx,2)+pow(previousy-currenty,2));
                                callx = previousx + dist*IntervalBetweenCalls*(i+1)*sin(currentangle);
                                cally = previousy + dist*IntervalBetweenCalls*(i+1)*cos(currentangle);
                                All_CT[NoCT]->CapturesIndividual(callx, cally,
                                                                 Individual,
                                                                 callangle, currentangle,
                                                                 iterationnumber,
                                                                 IntervalBetweenCalls*(i+1), 1);
                            };// end of for loop, "i" for the number of calls
                        }; // END OF if loop, calls/no calls
                
                    }; //END OF IF TempAllLocations
                    // Resets the step on to zero because the cameras are used in the next iteration and they need to
                    // the start step on to be zero. 
                    All_CT[NoCT]->Add1StepOn();
                };// END OF FOR time LOOP
            
                /*------------------------------------------------------
                 // Update location
                 ---------------------------------------------------------*/
                /////////////SAVING THE CAPTURES///////////////////
                // Retreaves all of the captures
                std::vector<std::vector<double>> TempCaptures = All_CT[NoCT]->getCaptures();
            
                //STarts looking dor the first entry
                int stepcounter=0;
                // Temp location file is written in csv file
                while(TempCaptures[stepcounter].size()==10){
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
                        "\n";                                  // New line
                
                    stepcounter+=1;
                }; //End of step counter loop
                All_CT[NoCT]->resetCaptures();
            }; // End on camera loop (NoCT)
            // This is done because the StepNo is changed for everytime step and needs to be reset to 0
            for(int NoCT=0; NoCT<NoCameraTraps; NoCT++){All_CT[NoCT]->ResetStepOn();};
        }; //End of Individual loop
        
        /*------------------------------------------------------
         // Destructors - Destructs the vectors of pointers to classes
         ---------------------------------------------------------*/
        // Destructors For Animals
        for(int i=0; i<NoAnimal; i++){delete AllAnimals[i];}
        // Destructors For HomeRange
        for(int i=0; i<NoHR; i++){ delete AllHR[i];}
        
    /*------------------------------------------------------
    //         !!!   END OF ITERATION   !!!!
    ---------------------------------------------------------*/
    };//End of iteration
    
    // Closes files that are open in all iterations
    Captures.close();
    Movement.close();
    HomeRangefile.close();
        
    }; //end if test
    
    // Destructors For Camera Traps
    for(int i=0; i<NoCameraTraps; i++){delete All_CT[i];}
=======
    double seconds = difftime(t2,t);
    std::cout<<"mins " <<seconds/CLOCKS_PER_SEC/60 <<std::endl;
>>>>>>> bats
    
    std::cout<<"Finished" <<std::endl;
    t = clock()- t;
    std::cout<<t/CLOCKS_PER_SEC <<std::endl;
    
    // Return is null
    return 0;
}; 
/*------------------------------------------------------
//                 !!!   END OF CODE   !!!!   
---------------------------------------------------------*/