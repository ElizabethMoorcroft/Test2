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
#include "HomeRange.h"
#include "CameraTraps.h"
#include "Parameters.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              FILE NAMES                                            ///
//                                                                                                    ///
// 3 functions which are used to create dynamic file names in order to save files.                    ///
//                                                                                                    ///
// Code needs tiding up a bit                                                                         ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////


// Function for creating a dynamic file names for movement/captures/HRs
std::string make_filename( const std::string& directoryBasename,const std::string& type, int index, const std::string& ext ){
    std::ostringstream result;
    result << directoryBasename << type << index << ext;
    return result.str();
}

// Function for creating a dynamic file names for settings
std::string make_filenamesettings( const std::string& directory ,const std::string& basename, const std::string& ext ){
    std::ostringstream result;
    result << directory <<basename << ext;
    return result.str();
};

// Function for creating the directory/Basename of the file
// The directory is the input - but this part of the code automatically
// updates the name with the type of boundaries, type of movement, and density
std::string make_directory( const std::string& directory){
    
    // Updates so that the type of boundary and 
    std::string boundary;
    if(HR_SolidBoundaries==0){boundary=",NoBoundaries";} else{boundary=",Boundaries";}
    // Updates so that the type of boundary and
    //std::string Move;
    //if(MovementType==0){Move=",StraightMove";} else
    //    if(MovementType==1){Move=",CorrelatedMove";}
    //    else{Move=",2StateMove";}
    //density is recorded
    double denistyCal = DensityAnimals*pow(10,6);
    
    int maxseed = NoOfIterations+Seed;
    std::ostringstream result;
    result  << directory
      //      << boundary         << Move
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
// - The path
// - The species
// - The boundary
// - The movement type
// - The animal denisty
std::string SaveDirectory = make_directory("/Users/student/Documents/Bats/Simulations/Temporary");

/// END OF FILE NAMES


////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                             ////
///                                                                                             ////
///                                 Main code starts                                            ////
///                                                                                             ////
///                                                                                             ////
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){
    
    clock_t tstart;
    clock_t t;
    tstart=clock();
    /////////////////////////////////////////////////////////////////////////
    //Calculates some values before the before the iteration of loop starts//
    /////////////////////////////////////////////////////////////////////////
    
    int NoCameraTraps = 0;
    if(DetectorLayOut == 0 | DetectorLayOut == 2){NoCameraTraps = NoSteps -NoRunIn;}
    else if(DetectorLayOut == 1){NoCameraTraps = MaxNoX *MaxNoY;}
    //else if(DetectorLayOut == 0){NoCameraTraps =1;}
    else{std::cout<<"Error! DetectorLayOut has not been specified."<< std::endl;
        exit (EXIT_FAILURE);}
    
    // The radius of the camera circle - to calucalte the locations of the cameras
    // The circumferance of the circle is: total distnace = total time * camera speed
    // The radius is: (circumference/pi)/2 = (circumference/(pi*2))
    // => radius = (total time * camera speed/(pi*2))
    double RadiusCameraCircle = (LengthMonitoring*SpeedCamera)/(2*M_PI);
    
    //Calculates the number of animals given the area and the density
    int NoAnimal;
    double area;
    //If there are no solid Home range boundaries then the animals can start anywhere
    // and then the possible area uses the min and max of the environment
    if(HR_SolidBoundaries==0){
         area = (Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY);
         NoAnimal = floor(DensityAnimals*area);
    }else if(HR_SolidBoundaries==1){
        if(DetectorLayOut==2){
            // If there is solid HR boundaries then can only detect the animals within
            // one home range radius of a camera
            // If the radius of the camera movement is less than the average home range then there is no hole
            // in the middle of the camera circle
            if(HR_AverageRadius>=RadiusCameraCircle){
                area = pow(RadiusCameraCircle+HR_AverageRadius,2)*M_PI;
                NoAnimal = floor(DensityAnimals*area);}
            else{
                area = pow(RadiusCameraCircle+HR_AverageRadius,2)*M_PI - pow(RadiusCameraCircle-HR_AverageRadius,2)*M_PI;
                NoAnimal = floor(DensityAnimals*area);
            } // End of Else
        } // End of transect
        else if (DetectorLayOut==1){// Grid of detectors
            // This puts them in a square, should be more of a blob shape
            area = ((Sq_MaxX-Sq_MinX)-HR_AverageRadius) *((Sq_MaxY-Sq_MinY)- HR_AverageRadius);
            NoAnimal = floor(DensityAnimals*area);
        } //end of grid of detectors
        else if(DetectorLayOut==0){
            area = pow(HR_AverageRadius,2)*M_PI;
            NoAnimal = floor(DensityAnimals*area);
        };
        
         
    }// END OF IF HRBOUNDARIES
    


    
    // The number of the HR's no of expected HR given the number of animals and the average size of HR
    int NoHR = ceil(NoAnimal/AverageSizeHR);
    
    std::cout<<"finishcal"<<std::endl;
    ///////////////////////////////////////////////////////////////////////////////////////
    //                          CALCULATES LOCATION OF CAMERA TRAPS                     ///
    //                                                                                  ///
    // Creates Camera traps as they do not change location when the movement changes    ///
    //  - only after the set up parameters change (change in length of study, etc)      ///
    // Saves the locations in a CSV file                                                ///
    ///////////////////////////////////////////////////////////////////////////////////////

    //Creates a CSV file and writes in the header
    std::ofstream Cameras;
    Cameras.open(make_filenamesettings(SaveDirectory, ",Cameras", ".csv" ).c_str());
    Cameras << "ID" <<
    "," << "X location" <<
    "," << "Y location" <<
    "," << "CentreAngle" <<
    "," << "HalfWidthAngle" <<
    "," << "Camera Speed" <<
    "\n";
    
    /* ----------------------------------------------------------------------------*/
    
        // Named such the that the each simulation can be correctly identified
        std::ofstream Movement;
        Movement.open(make_filenamesettings(SaveDirectory, ",Movement", ".csv" ).c_str());
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
    /*---------------------------------------------------------------------------------*/
    
    

    
    ///////////////////////////////////////////////////////////////////////////////////////
    ///                                 !!! WARNINGS!!!                                 ///
    /// Will automatically stop the simulation if certain conditions they               ///
    /// aren't satisfied.                                                               ///
    /// Includes a limited number of tests on the following functions:                  ///
    ///         *CapturesIndividual                                                     ///
    ///////////////////////////////////////////////////////////////////////////////////////
    
   
    //Number of animals needs to be greater than zero
    if(NoAnimal<=0){
        std::cout<<"No of Animals = "<<NoAnimal<< ", Increase density"<< std::endl;
        exit (EXIT_FAILURE);
    };
    
    //Number of Home ranges needs to be greater than zero
    if(NoHR<=0){
        std::cout<<"No of roosts = "<<NoHR<< ", Decrease number of bats per roost"<< std::endl;
        exit (EXIT_FAILURE);
    };
    
    if(DetectorRadius>200){
        std::cout<<"Improbable camera radius. Camera radius= "<<DetectorRadius<< ". Check input in correct units"<< std::endl;
        exit (EXIT_FAILURE);
    };
    /*
    // The width of the printed area needs to be greater than the world environment
    if(Cir_CntX+RadiusCameraCircle+HR_AverageRadius>Sq_MaxX){
        std::cout<<Cir_CntX <<","<<RadiusCameraCircle<<"," <<HR_AverageRadius << std::endl;
        std::cout<<"Width of area < Width nessecary, Increase Sq_MaxX"<< std::endl;
        exit (EXIT_FAILURE);
    };
    
    // The height of the printed area needs to be greater than the world environment
    if(Cir_CntY+RadiusCameraCircle+HR_AverageRadius>Sq_MaxY){
        std::cout<<"Height of area < Height nessecary, Increase Sq_MaxY"<< std::endl;
        exit (EXIT_FAILURE);
    };
    */
    // No steps
    if(NoSteps==0){
        std::cout<<"No steps, Increase Length of monitoring"<< std::endl;
        exit (EXIT_FAILURE);
    };
    
    
    //Test on the camera trap
    ///////////THIS NEEDS UPDATING!!!!
    int test=0;
    if(test==1){
        CameraTrap CT1;
        CT1 = CameraTrap(1,1,0,0 ); // ID, radius,2*Transect only varaibles
        
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
    

    
    // The angle between camera as seen from the centre of the circle
    // Assuming equal distance between the cameras
    // Assume the first one is occurs at 0
    double AngleBetweenCameras = (2*M_PI)/(NoCameraTraps-1);
    
    
    //Creates a list of pointers to the CTs
    std::vector<CameraTrap*> All_CT(NoCameraTraps);
    //Caluclates the location of all the camera traps
    for(int i=0; i<NoCameraTraps; i++){
        //std::cout<<"Camera: "<<i+1 <<"/"<<NoCameraTraps <<std::endl;
        //std::cout<<DetectorLayOut<<std::endl;
        // Calculate the starting position of all Camera traps
        // The cameras start at 3o'clock and move in a anti-clockwise direction
        // This is for the simplicity of calculation
        All_CT[i] = new CameraTrap(i //identifier;
                                   , DetectorRadius //radius
                                   , RadiusCameraCircle //FOR TRANSECTS
                                   , AngleBetweenCameras //FOR TRANSECTS
                                   );
        
        //Saves the locations and the angle of the camera
        Cameras << All_CT[i]->getID() << //1st column
        "," << All_CT[i]->getXloc() << //2nd column
        "," << All_CT[i]->getYloc() << //...
        "," << All_CT[i]->getAngle() << //...
        "," << All_CT[i]->getHalfAngle() << //4th column
        "," << SpeedCamera << //5th column
        "\n";
    };
    //Closes the csv camera file
    Cameras.close();
    
    std::cout<<"Finish Camera traps"<<std::endl;
    if(test==0)
    {
    ////////////////////////////////////////////////////////////////////////////////////////
    //                                Saves settings                                      //
    //                                                                                    //
    // Save the settings - from which the results of the simulations will be exactly      //
    // replicable                                                                         //
    ////////////////////////////////////////////////////////////////////////////////////////
    
    //Creates the file 
    std::ofstream Settings;
    Settings.open(make_filenamesettings(SaveDirectory,"Settings", ".csv").c_str());

    //Simulation values - #Animals, #Steps, #HR, #CT
    Settings
        << "DensityAnimals" << ","<< DensityAnimals << "\n"
        << "NoOfAnimals" << "," << NoAnimal << "\n"
        << "Area"<< "," << area << "\n"
        << "LengthMonitoring" << "," << LengthMonitoring << "\n"
        << "AverageSizeHR" << "," << AverageSizeHR << "\n"
        << "SpeedCamera"  << "," <<  SpeedCamera << "\n"
        << "NoRunIn" << "," <<  NoRunIn << "\n"
        << "NoOfIterations" << "," << NoOfIterations << "\n"
        << "NoSteps"   << "," << NoSteps << "\n"
    //Radom number seed
        << "Seed" << "," << Seed << "\n"
    //Size of the enviroment
        << "Sq_MinX" << ","<<  Sq_MinX << "\n"
        << "Sq_MaxX" << ","<<  Sq_MaxX << "\n"
        << "Sq_MinY" << ","<<  Sq_MinY << "\n"
        << "Sq_MaxY" << ","<<  Sq_MaxY << "\n"
    //Camera settings
        << "CameraWidth" << "," << CameraWidth<< "\n"
    //HR set up value
        << "HR_AverageRadius" << ","<< HR_AverageRadius << "\n"
        << "HR_SolidBoundaries"  << ","<< HR_SolidBoundaries << "\n"
    //Movement parameters
        << "StepLength" << ","<< StepLength << "\n"
        << "CorrWalkMaxAngleChange" << ","<<   CorrWalkMaxAngleChange<< "\n"
    //Animal parameters
        << "AnimalSpeed" << ","<< AnimalSpeed<< "\n"
      //  << "MovementType" << ","<< MovementType<< "\n"
        << "ProbChangeMoveState" << ","<< ProbChangeMoveState<< "\n"
    //Call parameters
        << "Call_halfwidth" << ","<< Call_halfwidth << "\n"
    //For the attenuation of sound
    /*
        << "Temp" << ","<< Temp << "\n"
        << "Hum"  << ","<< Hum<< "\n"
        << "Freq" << ","<< Freq << "\n"
        << "Amp"  << ","<< Amp<< "\n"
        << "It"   << ","<< It << "\n"
     */
    //Calculated values
        <<"DetectorRadius"<<","<<DetectorRadius<<"\n";
    //Closes file
     Settings.close();
    
    ///////////////////////////////
    /// Saves the capture lists ///
    ///////////////////////////////
    // Creates file and saves it in with name refering the simulation number in the "SaveDirectory"
    std::ofstream Captures;
    Captures.open(make_filenamesettings(SaveDirectory, ",Captures",".csv" ).c_str());
    // Header for the file
    Captures << "AnimalNumber" <<
        "," << "Time_step" <<
        "," << "CameraID" <<
        "," << "Iteration number" <<
        "," << "X location" <<
        "," << "Ylocation" <<
        "," << "Percentage time within step" <<
        "," << "Call" <<
        "\n";
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // Starts a loop for the rest of the code                                                       //
    // This loop "NoOfIterations" number of times                                                   //
    // Each set of movement traces is saved where the file name ends with the number                //
    // The seed that starts all each simulation is the number of the simulation plus the start seed //
    // This means that can run simulations in blocks 1-10 then 11-20 etc                            //
    //////////////////////////////////////////////////////////////////////////////////////////////////

    for(int iterationnumber=Seed + 0; iterationnumber<Seed + NoOfIterations; iterationnumber++){
        std::cout<<"Iteration: "<<iterationnumber<<"/"<<Seed + NoOfIterations<<std::endl;
    
    
    /////////////////////////////////////////////////////
    /// Creates the start of the random number stream ///
    /////////////////////////////////////////////////////
    
    //List of random number
    srand(iterationnumber);
    std::vector<double> RandomNumberStream(100);
    for(int i=0; i<100; i++){RandomNumberStream[i] = double (rand());};
        //std::cout<<"Seeded random numbers"<<std::endl;
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////
    ///                                     Home Range locations                            ///
    ///                                                                                     ///
    ///  - Creates File for the home range locations to be saved in                         ///
    ///  - Creates home ranges                                                              ///
    ///  - Creates a random number stream to seed home range locations                      ///
    ///  - Creates locations for HR and saves the locations                                 ///
    ///////////////////////////////////////////////////////////////////////////////////////////
    
    //Creates files
    // Saves it with a name which is a composite of Save Directory (craeted above), "HomeRange",
    //      the iteration number which is actually the starting seed.
    // Gives the file a header
    /*
    std::ofstream HomeRangefile;
    HomeRangefile.open(make_filename(SaveDirectory, ",HomeRange",iterationnumber,".csv").c_str());
    HomeRangefile   <<"HomeRangeID" <<
                "," << "XLocation" <<
                "," << "YLocation" <<
                "\n";
        
    */
    //std::cout<<"HomeRange"<<std::endl;
    //Creates a vecter of pointers to Home ranges
    std::vector<HomeRange*> AllHR(NoHR);
    
    
    //List of random number for HRs
    //Uses RandomNumberStream as a seed for the stream
    //Creates 1 Random number for the seed each HR
    srand(RandomNumberStream[1]);
    std::vector<double> RandomNumberStreamHR(NoHR);
    for(int i=0; i<NoHR; i++){ RandomNumberStreamHR[i] = double (rand()); };
    
    //std::cout<<"HR RandNum seeded"<<std::endl;
        
    // For when the max number of satrt location is 1
    std::vector<int> HomeRangeIDused(NoHR);
    for(int i=0; i<NoHR; i++){HomeRangeIDused[i]= NoHR+100;};
        
    //Enters data for NoHR home ranges
    for(int i=0; i<NoHR; i++){
        AllHR[i] =new HomeRange(i //identifier; //The HR id number
                              , RandomNumberStreamHR[i] // Seed for random variables
                              , RadiusCameraCircle
                              );
    
        /*
        HomeRangefile << AllHR[i]->getHomeID() << //1st column
            "," << AllHR[i]->getHomeX() << //...
            "," << AllHR[i]->getHomeY()  << //...
            "\n";
         */
    }; // END OF FOR LOOP
    
    /*
    // Closes the HomeRange CSV file
    HomeRangefile.close();
     */
    //std::cout<<"HR complete"<<std::endl;
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///                                     ANIMALS                                         ////
    /// Creates animals - simulates their movement - and saves this to files                ////
    ///                                                                                     ////
    ///  - Vector of pointers to Animals                                                    ////
    ///  - 3 Random streams created using seeds from initail stream                         ////
    ///        * Stream for choosing a suitable HOME RANGE                                  ////
    ///        * Stream for choosing a suitable START ANGLE                                 ////
    ///        * Stream for seeding a new stream for all subsequent MOVEMENT                ////
    ///  - Creates CSV documents with save animal parameters                                ////
    ///  - Creates CSV documents with save animal movements                                 ////
    ///  - Allocates start location, simulates movement                                     ////
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    //std::cout<<"Animals"<<std::endl;
    //Creates a vecter of pointers to individuals
    std::vector<Animal*> AllAnimals(NoAnimal);
    
    //List of random number for Animals
    //Uses RandomNumberStream as a seed for the stream
    //Creates 1 Random number for the seed each Animal for the following
    //Random number stream for choosing HR
    srand(RandomNumberStream[2]);
    std::vector<double> RandomNumberStreamAnimal1(NoAnimal);
    for(int i=0; i<NoAnimal; i++){ RandomNumberStreamAnimal1[i]=double(rand()); };
    
    // Random number stream seed for start angle
    srand(RandomNumberStream[3]);
    std::vector<double> RandomNumberStreamAnimal2(NoAnimal);
    for(int i=0; i<NoAnimal; i++){RandomNumberStreamAnimal2[i]=double(rand());};
    
    // Random number stream seed for the seed for the movement
    srand(RandomNumberStream[4]);
    std::vector<double> RandomNumberStreamAnimal3(NoAnimal);
    for(int i=0; i<NoAnimal; i++){RandomNumberStreamAnimal3[i]=double(rand());};
        
    //std::cout<<"Random numbers"<<std::endl;
    
    /////////////////////////
    // Create output files //
    /////////////////////////

    /*
    std::ofstream Animals;
    Animals.open(make_filename(SaveDirectory, ",Animals",iterationnumber,".csv" ).c_str());
    Animals << "ID" <<
        "," << "HomeRangeID" <<
        "," << "XLocation" <<
        "," << "YLocation" <<
        "," << "Speed" <<
        "\n";
     */
    ////////////////////////////////////////////
    // Creating  animals and animal movement ///
    ////////////////////////////////////////////
    for(int i=0; i<NoAnimal; i++){
        //Print out animal number to screen
        
        //std::cout <<"Animal:" << i+1 <<"/" << NoAnimal << std::endl;
        
        // Sets seed for a random number
        // So that can choose a home range centre at random
        srand(RandomNumberStreamAnimal1[i]);
        int HrId = rand() % NoHR;
        
        // If Home ranges have only 1 animal (or animals to be randomly distributed)
        // Then we need to make sure that each animal is allocated a different home range/start location
        // If HrId already in the vector "HomeRangeIDused" then it needs to be recalulated by,
        // finding the next free HR is allocated instead.
        int count_while=1;
        int HRID_Temp;
        int HomeRangeIDused_Temp = 0;
        if(AverageSizeHR == 1){
            while(HomeRangeIDused_Temp == 0){
                count_while +=1;
                HRID_Temp =0;
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
        }; //
        
        
        //if(i+1==NoHR){std::cout<<"hello"<<std::endl;}
            
        // the x/y location of the HR
        // All animals start at the centre of there HR
        double xlocation = AllHR[HrId]->getHomeX();
        double ylocation = AllHR[HrId]->getHomeY();

        
        // To choose a start angle, sets up a random number class
        // Uses a radom number from stream RandomNumberStreamAnimal2 for a random seed
        //RandNum Number1;
        //double CurrentAngleTemp = Number1.AtoBUnif(RandomNumberStreamAnimal2[i],0,(2*M_PI));
        srand(RandomNumberStreamAnimal2[i]);
        double CurrentAngleTemp = (double(rand())/RAND_MAX)*2*M_PI;
        
        //std::cout <<"Create animal"<< std::endl;
        //std::cout <<"HrId "<< HrId << std::endl;

        // New animal given start locations - at the centre of the home range
        // ????? Can start anywhere - within HR radius  => Remove need for Run in period ????
        AllAnimals[i] =new Animal(i                     // identifier
                                  ,HrId                 // HomeRange_id
                                  ,xlocation            // Starts at the roost location
                                  ,ylocation            // Starts at the roost location
                                  ,CurrentAngleTemp     // Starts with random angle
                                  ,xlocation            // Home_x = t;
                                  ,ylocation            // Home_y = u;
                                  );
        
        /*
        //Saves each individual animals 
        Animals << AllAnimals[i]->getID() << //1st column
            "," << AllAnimals[i]->getHRID() << //2nd column
            "," << AllAnimals[i]->getCurrentX() << //...
            "," << AllAnimals[i]->getCurrentY()  << //...
            "," << AllAnimals[i]->getSpeed()  << //...
            "\n";
        */
        ///////////////////////
        /// Update location ///
        ///////////////////////
        
        //std::cout <<"Update location"<< std::endl;
        //Sets seed for a random number
        srand(RandomNumberStreamAnimal3[i]);
        //Random number stream for the movemnet of the animal
        std::vector<double> RandomNumberCurrentAnimal(NoSteps*100);
        for(int i=0; i<NoSteps*(100); i++){ RandomNumberCurrentAnimal[i]=  double(rand());};
            // For each step
            for(int j=0; j<NoSteps; j++){
                //std::cout <<"Update step"<< std::endl;
                double count = j*100;
                //Updates animal location
                AllAnimals[i] -> UpdateLocation(RandomNumberCurrentAnimal[count]);
            }; //End of j loop for Steps
        
        /* ------------------------------------------------------------------*/
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
                    "," << TempAllLocations[stepcounter][5] << //5th column, row "stepcounter"
                    "," << TempAllLocations[stepcounter][6] << //6th column, row "stepcounter"
                    "," << TempAllLocations[stepcounter][7] << //7th column, row "stepcounter"
                    "," << iterationnumber <<
                "\n";                                      // New line
                    
            }; //END of STEPCOUNTER LOOP
            }; //END OF FOR LOOP
        };
        /*-------------------------------------------------------------- */
        
        }; //End of i loop for EACH ANIMALS
    
        
    //Print "Finish movement" to screen 
    //std::cout <<"Finish movement" <<std::endl;
    
    //Animals.close();
    
    // Check that all animals are allocated different HR's etc
    //for(int i=0; i<NoHR; i++){std::cout<<"Home Range ID used: "<<HomeRangeIDused[i]<<std::endl;};
        
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///                         CAPTURES                                                    ////
    ///                                                                                     ////
    /// Calcualtes whether any animals have been captured by cameras                        ////
    ///  - There is no random numbers used here                                             ////
    ///  - Cycles through each time step for each individual and checks whether they are    ////
    ///     captured                                                                        ////
    ///  - Saves the Animal ID, Camera ID and Time Step in a CSV file                       ////
    ////////////////////////////////////////////////////////////////////////////////////////////
    


    // For each individual check each camera location and see whether they were captured
    //Each camera is only at a their location for one time interval
    for(int Individual=0; Individual<NoAnimal; Individual++){
        //std::cout <<"Animal:" << Individual+1 <<"/" << NoAnimal << std::endl;
        //std::cout<< "No. of cameras: " << NoCameraTraps <<std::endl;
        // The call width of each individual does not change throughout the simulation
        // Therefore it doesn't need to be called for each camera
        double callangle = AllAnimals[Individual]->getCallWidth();
        std::vector<std::vector<double>> TempAllLocations = AllAnimals[Individual]->getEndStepLocations();
        
        int TimeStepTrap =0;
        double previousx;
        double previousy;
        double currentx;
        double currenty;
        double currentangle;
        int capturecount;
        double dist;
        double callx;
        double cally;
        
        for(int NoCT=0; NoCT<NoCameraTraps; NoCT++){ //Checks all CT for presence of animal
            //std::cout <<"CT:" << NoCT+1 <<"/" <<NoCameraTraps << std::endl;

            if(DetectorLayOut == 0 | DetectorLayOut ==2){
                // The camera only start after the the "Run in period"
                // Then they are only on at the corresponding the time step
                // Camera 0 is only on at NoRunIn, Camera 1 is only on at NoRunIn+1,....
                // The one is added on becuase start counting the number of steps at 0
                TimeStepTrap = NoCT+NoRunIn+1;
                //std::cout <<"CT:" << NoCT+1 <<"/" <<NoCameraTraps << std::endl;
                if(TempAllLocations[TimeStepTrap].size()>0){
                    previousx = TempAllLocations[TimeStepTrap-1][2];
                    previousy = TempAllLocations[TimeStepTrap-1][3];
                    currentx = TempAllLocations[TimeStepTrap][2];
                    currenty = TempAllLocations[TimeStepTrap][3];
                    currentangle = TempAllLocations[TimeStepTrap][4];
                    
                    // Calcualtes whether the animal is captured
                    capturecount = All_CT[NoCT]->CapturesIntersection(currentx
                                                       ,currenty
                                                       ,previousx
                                                       ,previousy
                                                       ,Individual
                                                       ,callangle
                                                       ,currentangle
                                                       ,iterationnumber
                                                       );
                    
                    
                    if(capturecount>0 && callson==1){
                        // The bats are not constantly calling and therefore need to assume that calls have with a given 
                        for(int i=0; i<CallsPerStep; i++){
                            dist = sqrt(pow(previousx-currentx,2)+pow(previousy-currenty,2));
                            callx = previousx + dist*IntervalBetweenCalls*(i+1)*sin(currentangle);
                            cally = previousy + dist*IntervalBetweenCalls*(i+1)*cos(currentangle);
                            All_CT[NoCT]->CapturesIndividual(callx,
                                                             cally,
                                                             Individual,
                                                             callangle,
                                                             currentangle,
                                                             iterationnumber,
                                                             IntervalBetweenCalls*(i+1),
                                                             1);
                        };
                    }; // END OF if
                
                } //END OF IF TempAllLocations
            }
        // If the detector layout is a grid, then every detector needs to be checked at every time step.
        // This invloves an extra loop.
        else if( DetectorLayOut == 1){
            //std::cout<<"Inside loop1 "<<Individual<<" "<<NoCT<<" "<< All_CT[NoCT]->getStepOn()<<std::endl;
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
                    capturecount = All_CT[NoCT]->CapturesIntersection(currentx
                                                       ,currenty
                                                       ,previousx
                                                       ,previousy
                                                       ,Individual
                                                       ,callangle
                                                       ,currentangle
                                                       ,iterationnumber
                                                       );
                    
                    if(capturecount>0 && callson==1){ // if there have been any captures then the
                        // The bats are not constantly calling and therefore
                        for(int i=0; i<CallsPerStep; i++){
                            dist = sqrt(pow(previousx-currentx,2)+pow(previousy-currenty,2));
                            callx = previousx + dist*IntervalBetweenCalls*(i+1)*sin(currentangle);
                            cally = previousy + dist*IntervalBetweenCalls*(i+1)*cos(currentangle);
                            All_CT[NoCT]->CapturesIndividual(callx,
                                                             cally,
                                                             Individual,
                                                             callangle,
                                                             currentangle,
                                                             iterationnumber,
                                                             IntervalBetweenCalls*(i+1),
                                                             1);
                        };// end of for
                    }; // END OF if
                
                } //END OF IF TempAllLocations
                All_CT[NoCT]->Add1StepOn();
            }// END OF FOR time LOOP
            }// END ELSE IF Detctlayout ==1
            
            
            /////////////SAVING THE CAPTURES///////////////////
            // Retreaves all of the captures
            std::vector<std::vector<double>> TempCaptures = All_CT[NoCT]->getCaptures();
            
            //STarts looking dor the first entry
            int stepcounter=0;
            //std::cout<<"Length of the first entry ="<<TempCaptures[stepcounter].size()<<std::endl;
            // Temp location file is written in csv file
            while(TempCaptures[stepcounter].size()==8){
                //std::cout<<"Length of the current entry  = "<<TempCaptures[stepcounter].size()<<std::endl;
                Captures<< TempCaptures[stepcounter][0] << //1st column, row
                "," << TempCaptures[stepcounter][1] << //2nd column, row "stepcounter"
                "," << TempCaptures[stepcounter][2] << //...
                "," << TempCaptures[stepcounter][3] <<
                "," << TempCaptures[stepcounter][4] <<
                "," << TempCaptures[stepcounter][5] <<
                "," << TempCaptures[stepcounter][6] <<
                "," << TempCaptures[stepcounter][7] <<
                "\n";// New line
                
                stepcounter+=1;
                //std::cout<<"next entry"<<stepcounter<<std::endl;
            }; //End of step counter loop
            All_CT[NoCT]->resetCaptures();
            
         }; // End on camera loop (NoCT)
        // This is done because the StepNo is changed for everytime step and needs to be reset to 0
        if(DetectorLayOut == 1){
            for(int NoCT=0; NoCT<NoCameraTraps; NoCT++){All_CT[NoCT]->ResetStepOn();};
        };
     }; //End of Individual loop
        
    
        
    ///////////////////////////////////////////////////////////////////////////////////////
    ///                         !!!     Destructors         !!!!                        ///
    ///                                                                                 ///
    /// Need to destruct the vectors of pointers to classes                             ///
    ///////////////////////////////////////////////////////////////////////////////////////
    
    // Destructors For Animals
    for(int i=0; i<NoAnimal; i++){delete AllAnimals[i];}
    
    // Destructors For HomeRange
    for(int i=0; i<NoHR; i++){ delete AllHR[i];}
    
        ///////////////////////////////////////////////////////////////////////////////////////
        ///                         !!!   END OF ITERATION   !!!!                           ///
        ///////////////////////////////////////////////////////////////////////////////////////
    };//End of iteration
    
    
    // Closes captures CSV file
    Captures.close();
    
    //Closes the files Movement files
    //TO BE UNCOMMENTED WITH MOVEMENT SECTION!!
    Movement.close();
        
    }; //end if test
    // Prints to screem to inform finished calculating captures
    //std::cout <<"Finish calculating captures" <<std::endl;

    
    // Destructors For Camera Traps
    for(int i=0; i<NoCameraTraps; i++){ delete All_CT[i];}
    
    
    t=clock()-tstart;
    std::cout<< "Time taken to finish: " << float(t)/CLOCKS_PER_SEC <<std::endl;
    return 0;
    
}; 

///////////////////////////////////////////////////////////////////////////////////////////
///                                                                                     ///
///                             !!!   END OF CODE   !!!!                                ///
///                                                                                     ///
///////////////////////////////////////////////////////////////////////////////////////////
