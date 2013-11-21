//
//  AnimalMovement.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 21/11/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include "AnimalMovement.h"
#include "CheckSensor.h"
#include "Sensors.h"
#include "Animal.h"
#include "RandNum.h"
#include "Parameters.h"
#include <vector>

AnimalMovement::AnimalMovement(){};

AnimalMovement::AnimalMovement( std::vector<Animal*> AllAnimals ,  std::vector<Sensor*> AllSensors , std::ofstream &Captures, std::ofstream &Movement, double randomstart, double randommove, int i, int iterationnumber, double speedvalue, double perchvalue, double maxchangeanglevalue){
    
    //Print out animal number to screen
    //std::cout <<"Animal:" << i+1 <<"/" << NoAnimal << std::endl;
    RandNum RandomNumber1;
    srand(randomstart);
    std::vector<double> RandomNumberStreamAnimalStartLoc(151);
    for(int j=0; j<151; j++){RandomNumberStreamAnimalStartLoc[j]=double(rand());};
    double xlocation = RandomNumber1.AtoBUnif(RandomNumberStreamAnimalStartLoc[50],Sq_MinX,Sq_MaxX);
    double ylocation = RandomNumber1.AtoBUnif(RandomNumberStreamAnimalStartLoc[100],Sq_MinY,Sq_MaxY);
    
    
    // To choose a start angle, sets up a random number class
    // Uses a radom number from stream RandomNumberStreamAnimalAngle for a random seed
    double CurrentAngleTemp = RandomNumber1.AtoBUnif(RandomNumberStreamAnimalStartLoc[150],0,(2*M_PI));
    
    // New animal given start locations - at the centre of the home range
    //  Inputs are: ID & Starts location (x,y) &  Initial angle
    AllAnimals[i] =new Animal(i, xlocation, ylocation, CurrentAngleTemp);
    AllAnimals[i] -> setAnimalspeedPerch(speedvalue, perchvalue);
    AllAnimals[i] -> setMove_maxangle(maxchangeanglevalue);
    
    /*------------------------------------------------------
     // Update location
     ---------------------------------------------------------*/
    //Sets seed for a random number
    //Random number stream for the movemnet of the animal
    srand(randommove);
    std::vector<double> RandomNumberCurrentAnimal(NoSteps*1000);
    for(int j=0; j<NoSteps*1000; j++){RandomNumberCurrentAnimal[j]=double(rand());};
    
    // For each step, Updates animal location
    double stepcount;
    for(int j=0; j<NoSteps; j++){
        stepcount = j*1000;
        //if(j==0){std::cout<<"NoAnimal: " <<i <<" RandomNumberCurrentAnimal[stepcount]: " <<RandomNumberCurrentAnimal[stepcount]<<std::endl;}
        AllAnimals[i] -> UpdateLocation(RandomNumberCurrentAnimal[stepcount]);
    }; //End of j loop for Steps
    
    // Variables for calculating captures
    //double sensorx; double sensory; double sensorradius;
    //double disttosensorprevious; double disttosensorcurrent;
    
    // Creates a temp matrix for "all locations"
    std::vector<std::vector<double> > TempAllLocations = AllAnimals[i]->getAllLocations();
    
    int maxiteration=0;
    
    // Temp location file is written in csv file
    // Each location is a seperate row  - the number of rows = "TempAllLocations.size()"
    for(int stepcounter=0; stepcounter<TempAllLocations.size(); stepcounter++){
        std::vector<double>  locations = TempAllLocations[stepcounter];
        std::vector<double>  previouslocations = TempAllLocations[stepcounter-1];
        if(TempAllLocations[stepcounter].size()>0){ maxiteration=stepcounter;};
        CheckSensor::CheckSensor(locations, previouslocations, AllSensors, Movement, stepcounter, i, iterationnumber);
        
        /*
         if(TempAllLocations[stepcounter].size()>0){
         maxiteration=stepcounter;
         
         double currentx = TempAllLocations[stepcounter][2];
         double currenty = TempAllLocations[stepcounter][3];
         double currentangle = TempAllLocations[stepcounter][4];
         
         if(SaveMovement==1){
         Movement<< TempAllLocations[stepcounter][0] << //1st column, row "stepcounter"
         "," << TempAllLocations[stepcounter][1] << //2nd column, row "stepcounter"
         "," << currentx << //...
         "," << currenty << //...
         "," << currentangle << //...
         "," << TempAllLocations[stepcounter][5] << //...
         "," << TempAllLocations[stepcounter][6] << //...
         "," << TempAllLocations[stepcounter][7] << //8th column, row "stepcounter"
         "," << TempAllLocations[stepcounter][8] << //8th column, row "stepcounter"
         "," << iterationnumber <<                  // itertaion number
         "\n";                                      // New line
         }; // END OF IF LOOP FOR SAVING
         
         
         // for each step calcualte past location and current location, and direction when leaving past location
         if(TempAllLocations[stepcounter][1]>0){
         double previousx = TempAllLocations[stepcounter-1][2];
         double previousy = TempAllLocations[stepcounter-1][3];
         // Calcualtes whether the animal is captured
         
         // Only check for the capture if the start and end locations are with a given distance "checkforcapture"
         double checkforcapture =StepLength*AnimalSpeed*2;
         for(int sensor=0; sensor<NoSensors; sensor++){
         //std::cout<<"Sensor: "<<sensor << "/" << NoSensors <<std::endl;
         sensorx = AllSensors[sensor] -> getXloc();
         sensory = AllSensors[sensor] -> getYloc();
         sensorradius = AllSensors[sensor] -> getRadius();
         double checkforcaptureplusradius =checkforcapture+sensorradius;
         
         
         AllSensors[sensor] -> setStepOn(TempAllLocations[stepcounter][1]);
         
         disttosensorprevious = sqrt(pow(previousx - sensorx,2)+pow(previousy - sensory,2));
         disttosensorcurrent = sqrt(pow(currentx - sensorx,2)+pow(currenty - sensory,2));
         
         if(disttosensorprevious<checkforcaptureplusradius  && disttosensorcurrent<checkforcaptureplusradius ){
         for(int callsize=0; callsize<LengthCW; callsize++){
         //std::cout<< "CallWidth[callsize]: "<< CallWidth[callsize]<< std::endl;
         
         AllSensors[sensor] ->CapturesIntersection(currentx,currenty,previousx,previousy,i,CallWidth[callsize],currentangle,iterationnumber);
         }; // end of call loop
         };// End of if distance close to sensor
         }; // END of sensor for loop
         }; // end of if not first step
         };// END of check movement exists
         */
    }; // End of for step loop
    
    //std::cout<< TempAllLocations.size()<< std::endl;
    
    Movement<< TempAllLocations[maxiteration][0] << //1st column, row "stepcounter"
    "," << TempAllLocations[maxiteration][1] << //2nd column, row "stepcounter"
    "," << TempAllLocations[maxiteration][2] << //...
    "," << TempAllLocations[maxiteration][3] << //...
    "," << TempAllLocations[maxiteration][4] << //...
    "," << TempAllLocations[maxiteration][5] << //...
    "," << TempAllLocations[maxiteration][6] << //...
    "," << TempAllLocations[maxiteration][7] << //8th column, row "stepcounter"
    "," << TempAllLocations[maxiteration][8] << //8th column, row "stepcounter"
    "," << iterationnumber <<                  // itertaion number
    "\n";                                      // New line
    
    
    /*------------------------------------------------------
     // SAVING THE CAPTURES
     ---------------------------------------------------------*/
    // Retreaves all of the captures
    //std::cout<<"getcaptures"<<std::endl;
    for(int sensor=0; sensor<NoSensors; sensor++){
        std::vector<std::vector<double> > TempCaptures = AllSensors[sensor] -> getCaptures();
        
        //STarts looking dor the first entry
        int stepcounter=0;
        // Temp location file is written in csv file
        while(TempCaptures[stepcounter].size()==10){
            //if(sensorr==5){std::cout<<"stepcounter" << stepcounter <<std::endl;}
            Captures<< TempCaptures[stepcounter][0] << //1st column, row
            "," << TempCaptures[stepcounter][1] << //2nd column, row "stepcounter"
            "," << TempCaptures[stepcounter][2] << //...
            "," << TempCaptures[stepcounter][3] <<
            // "," << TempCaptures[stepcounter][4] <<
            // "," << TempCaptures[stepcounter][5] <<
            // "," << TempCaptures[stepcounter][6] <<
            // "," << TempCaptures[stepcounter][7] <<
            "," << TempCaptures[stepcounter][8] <<
            // "," << TempCaptures[stepcounter][9] <<
            "\n";                                  // New line
            
            stepcounter+=1;
        }; //End of step counter loop
        AllSensors[sensor] -> resetCaptures();
        // This is done because the StepNo is changed for everytime step and needs to be reset to 0
        AllSensors[sensor] -> ResetStepOn();
    }; //END OF FOR LOOP
    //std::cout<<"endcaptures"<<std::endl;

}