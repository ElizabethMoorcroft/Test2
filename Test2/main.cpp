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
    
    

    
    double T = 273.15+temp ; //Convert celsius to kelvin
    double T01 = 273.16;
    double T0 = 293.15;
    double psat_ps0 = pow(10,-6.8346*pow(T01/T,1.261)+4.6151); // saturation pressure ratio from ISO
    double h = hum*psat_ps0/1;
    // double c0 = 331; // reference sound speed
    // double c_iso = (1+0.16*h)*c0*sqrt(T/T01);
    double taur = T/T0;
    double fr0 = (24+40400*h*(0.02+h)/(0.391+h));
    double frN = pow(taur,-1/2)*(9+280*h*exp(-4.17*(pow(taur,-1/3)-1)));
    double b1 = 0.1068*exp(-3352/T)/(frN+pow(freq,2)/frN);
    double b2 = 0.01275*exp(-2239.1/T)/(fr0+pow(freq,2)/fr0);
    double alpha = 8.686*pow(freq,2)*pow(taur,0.5)*(1.84*pow(10,-11)/1+pow(taur,-3)*(b1+b2));
    double alphatotal = alpha + 6; // # spherical spreading.
    double Attenuation = alphatotal;
    double Radius = (I0-It)/Attenuation;
    
    
    /////////////////////
    //Calculates values//
    /////////////////////
    
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
    
    /*
    //Print statement
    std::cout<< (NoCameraTraps-1) <<std::endl;
    std::cout<< 2*M_PI <<std::endl;
    std::cout<< AngleBetweenCameras <<std::endl;
    */
    
    
    
    
    ///////////////
    //SAVES FILES//
    ///////////////
    
    
    
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
        "," << "Total distance" <<
        "\n";
    
    Captures << "AnimalNumber" <<
        "," << "Time_step" <<
        "," << "CameraID" <<
        "\n";
    
    Cameras << "ID" <<
        "," << "X location" <<
        "," << "Y location" <<
        "," << "Centre Angle" <<
        "," << "Half width angle" <<
        "\n";
    
    Animals << "ID" <<
        "," << "HomeRangeID" <<
        "," << "X location" <<
        "," << "Y location" <<
        "," << "Speed" <<
        "\n";

    HomeRangefile <<"HomeRangeID" <<
        "," << "X location" <<
        "," << "Y location" <<
        "\n";
    
    
    
    /////////////////////////////////////////////////////
    /// Creates the start of the random number stream ///
    /////////////////////////////////////////////////////
    
    //List of random number
    srand(Seed);
    std::vector<double> RandomNumberStream;
    for(int i=0; i<LengthStream; i++){
        double myrandomnumber =  rand();
        RandomNumberStream.push_back(myrandomnumber);
    };
    
    /*
    // Prints off list of Random numbers
     for(int i=0; i<LengthStream; i++){
        std::cout << RandomNumberStream[i] <<std::endl;
    };
    */
    
    
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
    for(int i=0; i<NoHR; i++){
        double myrandomnumber =  rand();
        RandomNumberStreamHR.push_back(myrandomnumber);
    };
    
    
    //Enters data for NoHR home ranges
    for(int i=0; i<NoHR; i++){
        AllHR[i] =new HomeRange(i //identifier; //The HR id number
                                ,0 //type;       //HR have boudaries, 1=Y 0=N
                                ,1 //Home_Shape; // Shape of HR, 1=Circle
                                );
        // Randomly places HR's
        AllHR[i]->HRSetValues(Sq_MinX //Used if type=0, places randomly anywhere
                              ,Sq_MaxX //Used if type=0, places randomly anywhere
                              ,Sq_MinY //Used if type=0, places randomly anywhere
                              ,Sq_MaxY //Used if type=0, places randomly anywhere
                              //Below may need to be altered... due to random radius values
                              ,Cir_CntX // used if type=1, within average radius boudaries MinRng/MaxRng
                              ,Cir_CntY// used if type=1, within average radius boudaries MinRng/MaxRng
                              ,Cir_MinRng//used if type=1, within average radius boudaries MinRng/MaxRng
                              ,Cir_MaxRng//used if type=1, within average radius boudaries MinRng/MaxRng
                              ,HR_AverageRadius //Average radius of HR
                              // Use for all HR
                              ,RandomNumberStreamHR[i] // Seed for random variables
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
    for(int i=0; i<NoAnimal; i++){
        double myrandomnumber =  rand();
        RandomNumberStreamAnimal1.push_back(myrandomnumber);
    };
    
    // Random number stream seed for start angle
    srand(RandomNumberStream[3]);
    std::vector<double> RandomNumberStreamAnimal2;
    for(int i=0; i<NoAnimal; i++){
        double myrandomnumber =  rand();
        RandomNumberStreamAnimal2.push_back(myrandomnumber);
    };
    
    // Random number stream seed for the seed for the movement
    srand(RandomNumberStream[4]);
    std::vector<double> RandomNumberStreamAnimal3;
    for(int i=0; i<NoAnimal; i++){
        double myrandomnumber =  rand();
        RandomNumberStreamAnimal3.push_back(myrandomnumber);
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
        double CurrentAngleTemp = Number1.AtoRangeBUnif(RandomNumberStreamAnimal2[i],0,(2*M_PI));
        
        AllAnimals[i] =new Animal(i                     // identifier
                                  ,HrId                 // HomeRange_id
                                  ,0                    // step_number = 0 for start number
                                  ,1                    // Movement_type
                                  ,1                    // Move_state = e;
                                  ,AnimalSpeed          // Move_speed = f;
                                  ,60                   // Move_maxangle = g;
                                  ,xlocation            // Current_x = i;
                                  ,ylocation            // Current_y = j;
                                  ,CurrentAngleTemp     // Current_angle = k;
                                  ,0                    // Current_distance = l - start of the sim all animals is 0
                                  ,10                   // Call_amp = o;
                                  ,10                   // Call_freq = p;
                                  ,45                   // Call_halfwidth = q;
                                  ,HR_SolidBoundaries   // Home_state = r ;  // HR = 1=Y or 0=N  (HR Boundary Type)
                                  ,xlocation            // Home_x = t;
                                  ,ylocation            // Home_y = u;
                                  ,Radius     // Home_r = v;
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
        for(int i=0; i<NoSteps*NoSteps; i++){
            double myrandomnumber =  rand();
            RandomNumberCurrentAnimal.push_back(myrandomnumber);
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
                    "\n";                                      // New line
            };
            
        
        }; //End of i loop for individual animals
    
    
    std::cout <<"Finish movement" <<std::endl;
    
    ///////////////////////////
    ///Creates camera traps////
    ///////////////////////////
    
    /*
    //List of random number for CTs
    //Uses RandomNumberStream as a seed for the stream
    //Creates 1 Random number for the seed each CT
    srand(RandomNumberStream[4]);
    std::vector<double> RandomNumberStreamCT;
    for(int i=0; i<NoCameraTraps; i++){
        double myrandomnumber =  rand();
        RandomNumberStreamCT.push_back(myrandomnumber);
    };
     */
    
    //Creates a list of pointers to the CTs
    std::vector<CameraTrap*> All_CT(NoCameraTraps);
    for(int i=0; i<NoCameraTraps; i++){
        
        // Calculate the starting position of all Camera traps
        //std::cout<<"Start Camera trap"<<std::endl;
        
        // The cameras start at 3o'clock and move in a anti-clockwise direction
        // This is for the simplicity of calculation
        All_CT[i] = new CameraTrap(i //identifier;
                                   ,RadiusCameraCircle // Radius of circle
                                   , Cir_CntX //Cntre of circle x
                                   , Cir_CntY // Centre of circle y
                                   , AngleBetweenCameras //Angle between cameras
                                   ,100.0 //radius
                                   ,60.0 //half angle
                                   );
        
        //Saves the locations and the angle of the camera
        Cameras << All_CT[i]->getID() << //1st column
            "," << All_CT[i]->getXloc() << //2nd column
            "," << All_CT[i]->getYloc() << //...
            "," << All_CT[i]->getAngle() << //...
            "," << All_CT[i]->getHalfAngle() << //4th column
            "\n";
        };
    

    
    for(int Individual=0; Individual<NoAnimal; Individual++){
        
        
        
        for(int NoCT=0; NoCT<NoCameraTraps; NoCT++){ //Checks all CT for presence of animal
            
            int TimeStepTrap = NoCT+NoRunIn;
            
            std::cout<<"Time Step: "<<TimeStepTrap <<std::endl;
            
            std::vector<std::vector<double>> TempAllLocations = AllAnimals[Individual]->getAllLocations();
            
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

