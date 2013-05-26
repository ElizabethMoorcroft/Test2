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
#include "Animal.h"
#include "RandNum.h"
#include "HomeRange.h"
#include "CameraTraps.h"
#include "Parameters.h"


// Function for creating a dynamic file names
std::string make_filename( const std::string& basename, int index, const std::string& ext )
{
    std::ostringstream result;
    result << basename << index << ext;
    return result.str();
}

///////////////////////
///Main code starts////
///////////////////////

int main(){

    
    
    
    /////////////////////
    //Calculates values//
    /////////////////////
    
    double area = (Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY);
    int NoAnimal = floor(DensityAnimals*area);
    
    int NoCameraTraps = round(LengthMonitoring/StepLength);
    int NoSteps = NoCameraTraps + NoRunIn;
    
    // The radius of the camera circle - to calucalte the locations of the cameras
    // The circumferance of the circle is: total distnace = total time * camera speed
    // The radius is: (circumference/pi)/2 = (circumference/(pi*2))
    // => radius = (total time * camera speed/(pi*2))
    double RadiusCameraCircle = (LengthMonitoring*SpeedCamera)/(2*M_PI);
    
    // The angle between camera as seen from the centre of the circle
    // Assuming equal distance between the cameras
    // Assume the first one is occurs at 0
    double AngleBetweenCameras = (2*M_PI)/(NoCameraTraps-1);
    
    // The number of the HR's no of expected HR given the number of animals and the average size of HR
    int NoHR = NoAnimal/AverageSizeHR;
    
    //Tim's code for calucalting the attentuation of sound in order to calucalte the
    // maximum distance, here refered to as Radius, that the detector can detect a bat call
    double T = 273.15+temp ; //Convert celsius to kelvin
    double T01 = 273.16;
    double T0 = 293.15;
    double psat_ps0 = pow(10,-6.8346*pow(T01/T,1.261)+4.6151); // saturation pressure ratio from ISO
    double h = hum*psat_ps0/1;
    double taur = T/T0;
    double fr0 = (24+40400*h*(0.02+h)/(0.391+h));
    double frN = pow(taur,-1/2)*(9+280*h*exp(-4.17*(pow(taur,-1/3)-1)));
    double b1 = 0.1068*exp(-3352/T)/(frN+pow(freq,2)/frN);
    double b2 = 0.01275*exp(-2239.1/T)/(fr0+pow(freq,2)/fr0);
    double alpha = 8.686*pow(freq,2)*pow(taur,0.5)*(1.84*pow(10,-11)/1+pow(taur,-3)*(b1+b2));
    double Attenuation = alpha + 6; // # spherical spreading.
    double CallRadius = (Amp-It)/Attenuation;
    //
    // End of Tim's code
    
    
    
    ///////////////////////
    /// !!! WARNINGS!!! ///
    ///////////////////////
    
    //Number of animals needs to be greater than zero
    if(NoAnimal==0){
        std::cout<<"No of Animals = "<<NoAnimal<< ", Increase density"<< std::endl;
        exit (EXIT_FAILURE);
    };
    
    //Number of Home ranges needs to be greater than zero
    if(NoHR==0){
        std::cout<<"No of roosts = "<<NoHR<< ", Decrease number of bats per roost"<< std::endl;
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

    

    
    /////////////////////////
    // Create output files //
    /////////////////////////
    
    
    
    //Write movement to a file
    std::ofstream Movement(make_filename( "/Users/student/Documents/Bats/Move", 3, ".csv" ).c_str());
    std::ofstream Captures("/Users/student/Documents/Bats/Captures.csv");
    std::ofstream Cameras("/Users/student/Documents/Bats/Cameras.csv");
    std::ofstream Animals("/Users/student/Documents/Bats/Animals.csv");
    std::ofstream HomeRangefile("/Users/student/Documents/Bats/HomeRange.csv");

    
    //Creates a header for the file
    //This is not necessary but helps
    Movement << "AnimalNumber" <<
        "," << "StepNumber" <<
        "," << "Xlocation" <<
        "," << "Ylocation" <<
        "," << "Angle" <<
        "," << "TotalDistance" <<
        "," << "Re-enterWorld" <<
        "\n";
    
    Captures << "AnimalNumber" <<
        "," << "Time_step" <<
        "," << "CameraID" <<
        "\n";
    
    Cameras << "ID" <<
        "," << "X location" <<
        "," << "Y location" <<
        "," << "CentreAngle" <<
        "," << "HalfWidthAngle" <<
        "," << "Camera Speed" <<
        "\n";
    
    Animals << "ID" <<
        "," << "HomeRangeID" <<
        "," << "XLocation" <<
        "," << "YLocation" <<
        "," << "Speed" <<
        "\n";

    HomeRangefile <<"HomeRangeID" <<
        "," << "XLocation" <<
        "," << "YLocation" <<
        "\n";
    
    
    
    /////////////////////////////////////////////////////
    /// Creates the start of the random number stream ///
    /////////////////////////////////////////////////////
    
    //List of random number
    srand(Seed);
    std::vector<double> RandomNumberStream;
    RandomNumberStream.resize(LengthStream);
    for(int i=0; i<LengthStream; i++){
        RandomNumberStream[i] = double (rand());
    };
    
    
    
    ///////////////////////////
    /// Creates home ranges ///
    ///////////////////////////
    
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
    
    HomeRangefile << AllHR[i]->getHomeID() << //1st column
        "," << AllHR[i]->getHomeX() << //...
        "," << AllHR[i]->getHomeY()  << //...
        "\n";
    
    };

    
    //////////////////////
    ///Creates animals////
    //////////////////////
    
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
    
    // Enters data
    for(int i=0; i<NoAnimal; i++){
        
        //Sets seed for a random number
        // So that can choose a home range centre at random
        srand(RandomNumberStreamAnimal1[i]);
        double HrId = rand() % NoHR;
        
        // the x/y location of the HR
        // All animals start at the centre of there HR
        double xlocation = AllHR[HrId]->getHomeX();
        double ylocation = AllHR[HrId]->getHomeY();

        
        // To choose a start angle, sets up a random number class
        // Uses a radom number from stream RandomNumberStreamAnimal2 for a random seed
        RandNum Number1;
        double CurrentAngleTemp = Number1.AtoBUnif(RandomNumberStreamAnimal2[i],0,(2*M_PI));
        std::cout <<CurrentAngleTemp <<std::endl;
        
        AllAnimals[i] =new Animal(i                     // identifier
                                  ,HrId                 // HomeRange_id
                                  ,0                    // step_number = 0 for start number
                                  ,MovementType         // Movement_type
                                  ,0                    // Move_state = e;
                                  ,AnimalSpeed          // Move_speed = f;
                                  ,CorrWalkMaxAngleChange//Move_maxangle = g;
                                  ,xlocation            // Current_x = i;
                                  ,ylocation            // Current_y = j;
                                  ,CurrentAngleTemp     // Current_angle = k;
                                  ,0                    // Current_distance = l - start of the sim all animals is 0
                                  ,10                   // Call_amp = o;
                                  ,10                   // Call_freq = p;
                                  ,45                   // Call_halfwidth = q;
                                  ,HR_SolidBoundaries   // SolidHomeRangeBoundary = r ;  // HR = 1=Y or 0=N
                                  ,xlocation            // Home_x = t;
                                  ,ylocation            // Home_y = u;
                                  ,HR_AverageRadius     // Home_r = v;
                                  );
        
        //Saves each individual animals 
        Animals  << AllAnimals[i]->getID() << //1st column
            "," << AllAnimals[i]->getHRID() << //2nd column
            "," << AllAnimals[i]->getCurrentX() << //...
            "," << AllAnimals[i]->getCurrentY()  << //...
            "," << AllAnimals[i]->getSpeed()  << //...
            "\n";
        
        ///////////////////////
        /// Update location ///
        ///////////////////////
        
        //Sets seed for a random number
        srand(RandomNumberStreamAnimal3[i]);
        //Random number stream for the movemnet of the animal
        std::vector<double> RandomNumberCurrentAnimal;
        RandomNumberCurrentAnimal.resize(NoSteps*NoSteps);
        for(int i=0; i<NoSteps*NoSteps; i++){
            RandomNumberCurrentAnimal[i]=  double(rand());
        };
        
            for(int j=0; j<NoSteps; j++){
                std::cout <<"Step" <<j << std::endl;
                double count = (j)*NoSteps;
                 //std::cout << count << std::endl;
                 //std::cout << RandomNumberCurrentAnimal[count] << std::endl;
                
                //Updates animal location
                AllAnimals[i] -> UpdateLocation(StepLength,RandomNumberCurrentAnimal[count]);
            }; //End of j loop for Steps
        
        // Creates a temp matrix for "all locations"
        std::vector<std::vector<double>> TempAllLocations = AllAnimals[i]->getAllLocations();
            
            // Temp location file is written in csv file
            for(int stepcounter=0; stepcounter<TempAllLocations.size(); stepcounter++){
                Movement  << TempAllLocations[stepcounter][0] << //1st column, row "stepcounter"
                    "," << TempAllLocations[stepcounter][1] << //2nd column, row "stepcounter"
                    "," << TempAllLocations[stepcounter][2] << //...
                    "," << TempAllLocations[stepcounter][3] << //...
                    "," << TempAllLocations[stepcounter][4] << //...
                    "," << TempAllLocations[stepcounter][5] << //5th column, row "stepcounter"
                    "," << TempAllLocations[stepcounter][6] << //6th column, row "stepcounter"
                "\n";                                      // New line
            };
            
        
        }; //End of i loop for individual animals
    
    
    std::cout <<"Finish movement" <<std::endl;
    
    ///////////////////////////
    ///Creates camera traps////
    ///////////////////////////
    
    
    //List of random number for CTs
    //Uses RandomNumberStream as a seed for the stream
    //Creates 1 Random number for the seed each CT
    srand(RandomNumberStream[4]);
    std::vector<double> RandomNumberStreamCT;
    RandomNumberStreamCT.resize(NoCameraTraps);
    for(int i=0; i<NoCameraTraps; i++){
        RandomNumberStreamCT[i] =double (rand());
    };
     
    
    //Creates a list of pointers to the CTs
    std::vector<CameraTrap*> All_CT(NoCameraTraps);
    for(int i=0; i<NoCameraTraps; i++){
        
        // Calculate the starting position of all Camera traps
        //std::cout<<"Start Camera trap"<<std::endl;
        
        // The cameras start at 3o'clock and move in a anti-clockwise direction
        // This is for the simplicity of calculation
        All_CT[i] = new CameraTrap(i //identifier;
                                   , RadiusCameraCircle // Radius of circle
                                   , AngleBetweenCameras //Angle between cameras
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
    

    // For each individual check each camera location and see whether they were captured
    //Each camera is only at a their location for one time interval
    for(int Individual=0; Individual<NoAnimal; Individual++){
        
        for(int NoCT=0; NoCT<NoCameraTraps; NoCT++){ //Checks all CT for presence of animal
            
            int TimeStepTrap = NoCT+NoRunIn;
            
            std::cout<<"Time Step: "<<TimeStepTrap <<std::endl;
            
            std::vector<std::vector<double>> TempAllLocations = AllAnimals[Individual]->getEndStepLocations();
            
            double currentid = TempAllLocations[TimeStepTrap][0];
            double currentx = TempAllLocations[TimeStepTrap][2];
            double currenty = TempAllLocations[TimeStepTrap][3];
            double currentangle = TempAllLocations[TimeStepTrap][4];
            double callangle = AllAnimals[Individual]->getCallWidth();
            
            std::cout<<"Current ID: "<< currentid <<std::endl;
            

            
            All_CT[NoCT]->CapturesIndividual(currentx
                                             ,currenty
                                             ,currentid
                                             ,NoCT
                                             ,TimeStepTrap
                                             ,callangle
                                             ,currentangle );
            
            
            
         }; // End on NoCT loop
     }; //End of Individual loop
    
    
    
    
    ///////////////////////////////
    /// Saves the capture lists ///
    ///////////////////////////////
    
    //For all camera traps get there capture lists
    //Write that to CSV file
    for(int NoCT=0; NoCT<NoCameraTraps; NoCT++){ 

        // Creates a temp matrix for "all locations"
        std::vector<std::vector<int>> TempCaptures = All_CT[NoCT]->getCaptures();
    
    
        // Temp location file is written in csv file
        //std::cout<<"Current ID1: " << TempCaptures[0]<<std::endl;
        for(int stepcounter=0; stepcounter<TempCaptures.size(); stepcounter++){
            Captures << TempCaptures[stepcounter][0] << //1st column, row
                "," << TempCaptures[stepcounter][1] << //2nd column, row "stepcounter"
                "," << TempCaptures[stepcounter][2] << //...
                "\n";                                  // New line
        }; //End of step counter loop
    }; //End of NoCT loop
    
     std::cout <<"Finish Camera" <<std::endl;
    
    
    
    
    //////////////////
    ///CLOSES FILES///
    //////////////////
    Movement.close();
    Captures.close();
    Animals.close();
    HomeRangefile.close();
    Cameras.close();

    
    std::cout <<"Closed" <<std::endl;
    
    
    ///////////////////
    /// Destructors ///
    ///////////////////
    
    // Destructors For Animals 
    for(int i=0; i<NoAnimal; i++){
        delete AllAnimals[i];
    }
    std::cout <<"Destruct" <<std::endl;
    
     // Destructors For Camera Traps
    for(int i=0; i<NoCameraTraps; i++){
        delete All_CT[i];
    }
     
    std::cout <<"Destruct" <<std::endl;
    
    //////////////////
    // END OF CODE ///
    //////////////////

    return 0;
};

