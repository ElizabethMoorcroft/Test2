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
    std::string Move;
    if(MovementType==0){Move=",StraightMove";} else
        if(MovementType==1){Move=",CorrelatedMove";}
        else{Move=",2StateMove";}
    //density is recorded
    double denistyCal = DensityAnimals*pow(10,6);
    
    int maxseed = NoOfIterations+Seed;
    std::ostringstream result;
    result  << directory
            << boundary         << Move
            << ",Density="      << denistyCal
            << ",Speed="        << AnimalSpeed
            << ",Iterations="   << Seed << "-" << maxseed
            << ",StepLength="   << StepLength
            << ",CallHalfwidth=" << Call_halfwidth
            << ",CameraHalfwidth=" << CameraWidth
            << ",Freq="         << Freq ;
    return result.str();
};

// Base name each file
// - The path
// - The species
// - The boundary
// - The movement type
// - The animal denisty
std::string SaveDirectory = make_directory("/Users/student/Documents/Bats/Simulations/SenAna");

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
    


    //int NoSteps = NoCameraTraps + NoRunIn;
    

    
    
    //Tim's code for calucalting the attentuation of sound in order to calucalte the
    // maximum distance, here refered to as Radius, that the detector can detect a bat call
    double T = 273.15+Temp ; //Convert celsius to kelvin
    double T01 = 273.16;
    double T0 = 293.15;
    double psat_ps0 = pow(10,-6.8346*pow(T01/T,1.261)+4.6151); // saturation pressure ratio from ISO
    double h = Hum*psat_ps0/1;
    double taur = T/T0;
    double fr0 = (24+40400*h*(0.02+h)/(0.391+h));
    double frN = pow(taur,-1/2)*(9+280*h*exp(-4.17*(pow(taur,-1/3)-1)));
    double b1 = 0.1068*exp(-3352/T)/(frN+pow(Freq,2)/frN);
    double b2 = 0.01275*exp(-2239.1/T)/(fr0+pow(Freq,2)/fr0);
    double alpha = 8.686*pow(Freq,2)*pow(taur,0.5)*(1.84*pow(10,-11)/1+pow(taur,-3)*(b1+b2));
    double Attenuation = alpha + 6; // # spherical spreading.
    double CallRadius=10;
    for(int i=0;  i<6; i++){ // Newton-raphson
        CallRadius = CallRadius - (Amp-It-Attenuation*CallRadius-20*log10(10*CallRadius))/(-Attenuation-20/(log(10)*CallRadius));
    }

    std::cout<<CallRadius<<std::endl;
    //
    // End of Tim's code
    
    //Calculates the number of animals given the area and the density
    int NoAnimal;
    //If there are no solid Home range boundaries then the animals can start anywhere
    // and then the possible area uses the min and max of the environment
    if(HR_SolidBoundaries==0){
        double area = (Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY);
         NoAnimal = floor(DensityAnimals*area);
    }else if(HR_SolidBoundaries==1){
    // If there is solid HR boundaries then can only detect the animals within
    // one home range radius of a camera
    // If the radius of the camera movement is less than the average home range then there is no hole
    // in the middle of the camera circle
        if(HR_AverageRadius>=RadiusCameraCircle){
            double area = pow(RadiusCameraCircle+HR_AverageRadius,2)*M_PI;
            NoAnimal = floor(DensityAnimals*area);}
        else{
            double area = pow(RadiusCameraCircle+HR_AverageRadius,2)*M_PI - pow(RadiusCameraCircle-HR_AverageRadius,2)*M_PI;
            NoAnimal = floor(DensityAnimals*area);}
        }
    


    
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
    
    // Named such the that the each simulation can be correctly identified
    std::ofstream Movement;
    Movement.open(make_filenamesettings(SaveDirectory, ",Movement", ".csv" ).c_str());
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
    
    if(CallRadius>200){
        std::cout<<"Improbable camera radius. Camera radius= "<<CallRadius<< ". Check input in correct units"<< std::endl;
        exit (EXIT_FAILURE);
    };
    
    // The width of the printed area needs to be greater than the world environment
    if(Cir_CntX+RadiusCameraCircle+HR_AverageRadius>Sq_MaxX){
        std::cout<<"Width of area < Width nessecary, Increase Sq_MaxX"<< std::endl;
        exit (EXIT_FAILURE);
    };
    
    // The height of the printed area needs to be greater than the world environment
    if(Cir_CntY+RadiusCameraCircle+HR_AverageRadius>Sq_MaxY){
        std::cout<<"Height of area < Height nessecary, Increase Sq_MaxY"<< std::endl;
        exit (EXIT_FAILURE);
    };
    
    
    //Test on the camera trap
    int test=0;
    if(test==1){
        std::cout<<"Start Test"<<std::endl;
    int a;
    double b, c, d, e, f, g,h;
    std::ifstream input;
    std::string line;
    input.open("/Users/student/Documents/Bats/CameraTestInputs.dat");
    if (input.is_open()) {
        getline(input, line);
        while (!input.eof()) {
            std::cout<<a<<std::endl;
            input >> a;
            input >> b;
            input >> c;
            input >> d;
            input >> e;
            input >> f;
            input >> g;
            input >> h;
           // for(int i=0; i<NoCameraTraps; i++){
            double tempvarCamerawidth = 89*M_PI/180;
                CameraTrap CT1;
                 CT1 = CameraTrap(1 //identifier;
                                    ,CallRadius //radius
                                     ,60*M_PI/180
                                     ,tempvarCamerawidth //tempCameraWidth
                                    );
            //    std::cout<<i<<std::endl;
                CT1.TestCapturesIndividual(a,b,c,d,e,f,g*tempvarCamerawidth,h);
          //  }
        }
        input.close();
    } // END OF CAMERA CHECK 
    else{std::cout<<"Hello"<<std::endl;}
    };
    
    //Creates a list of pointers to the CTs
    std::vector<CameraTrap*> All_CT(NoCameraTraps);
    //Caluclates the location of all the camera traps
    for(int i=0; i<NoCameraTraps; i++){
        //std::cout<<"Camera: "<<i+1 <<"/"<<NoCameraTraps <<std::endl;
        // Calculate the starting position of all Camera traps
        // The cameras start at 3o'clock and move in a anti-clockwise direction
        // This is for the simplicity of calculation
        All_CT[i] = new CameraTrap(i //identifier;
                                   , CallRadius //radius
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
        << "LengthMonitoring" << ","<< LengthMonitoring << "\n"
        << "AverageSizeHR" << ","<< AverageSizeHR << "\n"
        << "SpeedCamera"  << ","<<  SpeedCamera << "\n"
        << "NoRunIn" << ","<<  NoRunIn << "\n"
        << "NoOfIterations" << ","<< NoOfIterations << "\n"
        << " NoSteps"   << "," << NoSteps << "\n"
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
        << "MovementType" << ","<< MovementType<< "\n"
        << "ProbChangeMoveState" << ","<< ProbChangeMoveState<< "\n"
    //Call parameters
        << "Call_halfwidth" << ","<< Call_halfwidth << "\n"
    //For the attenuation of sound
        << "Temp" << ","<< Temp << "\n"
        << "Hum"  << ","<< Hum<< "\n"
        << "Freq" << ","<< Freq << "\n"
        << "Amp"  << ","<< Amp<< "\n"
        << "It"   << ","<< It << "\n"
    //Calculated values
        <<"CallRadius"<<","<<CallRadius<<"\n";
    //Closes file
     Settings.close();
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // Starts a loop for the rest of the code                                                       //
    // This loop "NoOfIterations" number of times                                                   //
    // Each set of movement traces is saved where the file name ends with the number                //
    // The seed that starts all each simulation is the number of the simulation plus the start seed //
    // This means that can run simulations in blocks 1-10 then 11-20 etc                            //
    //////////////////////////////////////////////////////////////////////////////////////////////////

    for(int iterationnumber=Seed + 0; iterationnumber<Seed + NoOfIterations; iterationnumber++){
        std::cout<<"Iteration: "<<iterationnumber+1<<"/"<<Seed + NoOfIterations<<std::endl;
    
    
    /////////////////////////////////////////////////////
    /// Creates the start of the random number stream ///
    /////////////////////////////////////////////////////
    
    //List of random number
    srand(iterationnumber);
    std::vector<double> RandomNumberStream;
    RandomNumberStream.resize(100);
    for(int i=0; i<100; i++){
        RandomNumberStream[i] = double (rand());
    };
    
    
    
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
    //Creates a vecter of pointers to Home ranges
    std::vector<HomeRange*> AllHR(NoHR);
    
    
    //List of random number for HRs
    //Uses RandomNumberStream as a seed for the stream
    //Creates 1 Random number for the seed each HR
    srand(RandomNumberStream[1]);
    std::vector<double> RandomNumberStreamHR;
    RandomNumberStreamHR.resize(NoHR);
    for(int i=0; i<NoHR; i++){
        RandomNumberStreamHR[i] = double (rand());
    };
    
    
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
    };
    
    /*
    // Closes the HomeRange CSV file
    HomeRangefile.close();
     */
    
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
    
    
    //Creates a vecter of pointers to individuals
    std::vector<Animal*> AllAnimals(NoAnimal);
    
    //List of random number for Animals
    //Uses RandomNumberStream as a seed for the stream
    //Creates 1 Random number for the seed each Animal for the following
    //Random number stream for choosing HR
    srand(RandomNumberStream[2]);
    std::vector<double> RandomNumberStreamAnimal1;
    RandomNumberStreamAnimal1.resize(NoAnimal);
    for(int i=0; i<NoAnimal; i++){
        RandomNumberStreamAnimal1[i]=double(rand());
    };
    
    // Random number stream seed for start angle
    srand(RandomNumberStream[3]);
    std::vector<double> RandomNumberStreamAnimal2;
    RandomNumberStreamAnimal2.resize(NoAnimal);
    for(int i=0; i<NoAnimal; i++){
        RandomNumberStreamAnimal2[i]=double(rand());
    };
    
    // Random number stream seed for the seed for the movement
    srand(RandomNumberStream[4]);
    std::vector<double> RandomNumberStreamAnimal3;
    RandomNumberStreamAnimal3.resize(NoAnimal);
    for(int i=0; i<NoAnimal; i++){
        RandomNumberStreamAnimal3[i]=double(rand());
    };
        
    //std::cout<<"Random numbers"<<sec<<std::endl;
    
    /////////////////////////
    // Create output files //
    /////////////////////////

    /*
    std::ofstream Animals;
    Animals.open(make_filename(SaveDirectory, ",Animals",iterationnumber,".csv" ).c_str());
     */
    /*
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
        double HrId = rand() % NoHR;
        
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

        //Sets seed for a random number
        srand(RandomNumberStreamAnimal3[i]);
        //Random number stream for the movemnet of the animal
        std::vector<double> RandomNumberCurrentAnimal;
        RandomNumberCurrentAnimal.resize(NoSteps*100);
        for(int i=0; i<NoSteps*(100); i++){
            RandomNumberCurrentAnimal[i]=  double(rand());
        };
            // For each step
            for(int j=0; j<NoSteps; j++){
                double count = j*100;
                //Updates animal location
                AllAnimals[i] -> UpdateLocation(RandomNumberCurrentAnimal[count]);
            }; //End of j loop for Steps
        
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
            };

        }; //End of i loop for EACH ANIMALS
    
    //Print "Finish movement" to screen 
    std::cout <<"Finish movement" <<std::endl;
    

    
    //Animals.close();
        
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

        
        // The call width of each individual does not change throughout the simulation
        // Therefore it doesn't need to be called for each camera
        double callangle = AllAnimals[Individual]->getCallWidth();
        std::vector<std::vector<double>> TempAllLocations = AllAnimals[Individual]->getEndStepLocations();

        for(int NoCT=0; NoCT<NoCameraTraps; NoCT++){ //Checks all CT for presence of animal
            
            //std::cout <<"CT:" << NoCT+1 <<"/" <<NoCameraTraps << std::endl;
            
            // The camera only start after the the "Run in period"
            // Then they are only on at the corresponding the time step
            // Camera 0 is only on at NoRunIn, Camera 1 is only on at NoRunIn+1,....
            // The one is added on becuase start counting the number of steps at 0
            int TimeStepTrap = NoCT+NoRunIn+1;

            if(TempAllLocations[TimeStepTrap].size()>0){
                //std::cout<<TempAllLocations.size()<<std::endl;
                //std::cout<<TimeStepTrap<<std::endl;
            //int currentid = TempAllLocations[TimeStepTrap][0];
            double currentx = TempAllLocations[TimeStepTrap][2];
            double currenty = TempAllLocations[TimeStepTrap][3];
            double currentangle = TempAllLocations[TimeStepTrap][4];
            
            /*
            // !!!!Check!!!!
            // Check whether the animal location is matched against the correct CT
            double CameraCT_StepOn  = All_CT[NoCT]->getStepOn();
            if(TempAllLocations[TimeStepTrap][1]!=CameraCT_StepOn){
                std::cout << "Wrong Camera"
                          << TempAllLocations[TimeStepTrap][1]
                          << "!="
                          << CameraCT_StepOn
                          << std::endl;
                exit (EXIT_FAILURE);};
             */
           
            // Calcualtes whether the animal is captured
            All_CT[NoCT]->CapturesIndividual(  currentx
                                             , currenty
                                             , Individual
                                             , callangle
                                             , currentangle
                                             , iterationnumber
                                             );
                
            
            }
         }; // End on camera loop (NoCT)
     }; //End of Individual loop

        
    ///////////////////////////////////////////////////////////////////////////////////////
    ///                         !!!     Destructors         !!!!                        ///
    ///                                                                                 ///
    /// Need to destruct the vectors of pointers to classes                             ///
    ///////////////////////////////////////////////////////////////////////////////////////
    
    // Destructors For Animals
    for(int i=0; i<NoAnimal; i++){
        delete AllAnimals[i];
    }
    
    // Destructors For HomeRange
    for(int i=0; i<NoHR; i++){
        delete AllHR[i];
    }
    
        ///////////////////////////////////////////////////////////////////////////////////////
        ///                         !!!   END OF ITERATION   !!!!                           ///
        ///////////////////////////////////////////////////////////////////////////////////////
    };//End of iteration
    
    
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
        "\n";
    
    //For all camera traps get there capture lists
    //Write that to CSV file
    for(int NoCT=0; NoCT<NoCameraTraps; NoCT++){
        
        // Creates a temp matrix for "all locations"
        std::vector<std::vector<int>> TempCaptures = All_CT[NoCT]->getCaptures();
        int stepcounter=0;
        // Temp location file is written in csv file
        while(TempCaptures[stepcounter].size()>0){
                Captures<< TempCaptures[stepcounter][0] << //1st column, row
                "," << TempCaptures[stepcounter][1] << //2nd column, row "stepcounter"
                "," << TempCaptures[stepcounter][2] << //...
                "," << TempCaptures[stepcounter][3] <<
                "\n";// New line
            stepcounter+=1;
        }; //End of step counter loop
    }; //End of NoCT loop
    
    
    // Closes captures CSV file
    Captures.close();
    
    //Closes the files Movement and Animals
    Movement.close();
    }; //end if test
    // Prints to screem to inform finished calculating captures
    //std::cout <<"Finish calculating captures" <<std::endl;

    
    // Destructors For Camera Traps
    for(int i=0; i<NoCameraTraps; i++){
        delete All_CT[i];
    }
    
    
    t=clock()-tstart;
    std::cout<< "FINISHED :" << float(t)/CLOCKS_PER_SEC <<std::endl;
    return 0;
    
}; 

///////////////////////////////////////////////////////////////////////////////////////////
///                                                                                     ///
///                             !!!   END OF CODE   !!!!                                ///
///                                                                                     ///
///////////////////////////////////////////////////////////////////////////////////////////
