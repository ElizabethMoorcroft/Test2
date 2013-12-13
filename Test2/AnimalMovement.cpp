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
    Animal Animal1(i, xlocation, ylocation, CurrentAngleTemp);
    Animal1.setAnimalspeedPerch(speedvalue, perchvalue);
    Animal1.setMove_maxangle(maxchangeanglevalue);
    
    /*------------------------------------------------------
     // Update location
     ---------------------------------------------------------*/
    //Sets seed for a random number
    //Random number stream for the movemnet of the animal
    srand(randommove);
    std::vector<double> RandomNumberCurrentAnimal(NoSteps*1000);
    for(int j=0; j<NoSteps*1000; j++){ RandomNumberCurrentAnimal[j]=double(rand());};
    
    // For each step, Updates animal location
    double stepcount;
    for(int j=0; j<NoSteps; j++){
        stepcount = j*1000;
        //std::cout<<"NoStep: "<<j<<"/"<<NoSteps <<std::endl;
        //if(j==0){std::cout<<"NoAnimal: " <<i <<" RandomNumberCurrentAnimal[stepcount]: " <<RandomNumberCurrentAnimal[stepcount]<<std::endl;}
        Animal1.UpdateLocation(RandomNumberCurrentAnimal[stepcount]);
        //std::cout<<"END UPDATE MOVEMENT " <<std::endl;
    }; //End of j loop for Steps
    //std::cout<<"finished no of steps"<<std::endl;

    
    // Creates a temp matrix for "all locations"
    std::vector<std::vector<double> > TempAllLocations = Animal1.getAllLocations();
    
    int maxiteration=0;
    
    // Temp location file is written in csv file
    // Each location is a seperate row  - the number of rows = "TempAllLocations.size()"
    for(int stepcounter=0; stepcounter<TempAllLocations.size(); stepcounter++){
        //std::cout<<"stepcounter: "<< stepcounter<<std::endl;
        std::vector<double>  locations = TempAllLocations[stepcounter];
        std::vector<double>  previouslocations;
        if(stepcounter>0){previouslocations = TempAllLocations[stepcounter-1];}else{previouslocations = TempAllLocations[stepcounter];};
        
        if(TempAllLocations[stepcounter].size()>0){ maxiteration=stepcounter;};
        //std::cout<<"check sensors: "<< stepcounter <<"/" <<TempAllLocations.size() <<std::endl;
        CheckSensor(locations, previouslocations, AllSensors, Movement, stepcounter, i, iterationnumber);
        
    }; // End of for step loop
    
    //std::cout<< TempAllLocations.size()<< std::endl;
    
    //std::cout<<"movement save"<<std::endl;

    
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
                "," << TempCaptures[stepcounter][4] <<
                "," << TempCaptures[stepcounter][5] <<
                "," << TempCaptures[stepcounter][6] <<
                "," << TempCaptures[stepcounter][7] <<
                "," << TempCaptures[stepcounter][8] <<
                "," << TempCaptures[stepcounter][9] <<
                "\n";                                  // New line
            
            stepcounter+=1;
        }; //End of step counter loop
        AllSensors[sensor] -> resetCaptures();
        // This is done because the StepNo is changed for everytime step and needs to be reset to 0
        AllSensors[sensor] -> ResetStepOn();
    }; //END OF FOR LOOP
    //std::cout<<"endcaptures"<<std::endl;
    //std::cout <<"Finished Animal:" << i+1 <<"/" << NoAnimal << std::endl;
}