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
    double seed1, seed2, seed3, temp;
    for(int i=0; i<151; i++){
        if(i==50){seed1=double(rand());}
        else if(i==100){seed2=double(rand());}
        else if(i==150){seed3=double(rand());}
        else {temp=double(rand());};
    };
    
    double xlocation = RandomNumber1.AtoBUnif(seed1,Sq_MinX,Sq_MaxX);
    double ylocation = RandomNumber1.AtoBUnif(seed2,Sq_MinY,Sq_MaxY);
    
    
    // To choose a start angle, sets up a random number class
    // Uses a radom number from stream RandomNumberStreamAnimalAngle for a random seed
    double CurrentAngleTemp = RandomNumber1.AtoBUnif(seed3,0,(2*M_PI));
    
    // New animal given start locations - at the centre of the home range
    //  Inputs are: ID & Starts location (x,y) &  Initial angle
    Animal Animal1(i, xlocation, ylocation, CurrentAngleTemp, Movement, AllSensors , Captures, iterationnumber);
    Animal1.setAnimalspeedPerch(speedvalue, perchvalue);
    Animal1.setMove_maxangle(maxchangeanglevalue);
    
    /*------------------------------------------------------
     // Update location
     ---------------------------------------------------------*/
    //Sets seed for a random number
    //Random number stream for the movemnet of the animal
    srand(randommove);
    std::vector<double> RandomNumberCurrentAnimal(NoSteps);
    for(int j=0; j<NoSteps; j++){
        RandomNumberCurrentAnimal[j]=double(rand());
        for(int extra=0; extra<999; extra++){temp=double(rand());};
    };
    
    // For each step, Updates animal location
    for(int j=0; j<NoSteps; j++){
        Animal1.UpdateLocation(RandomNumberCurrentAnimal[j], Movement ,  AllSensors , Captures, iterationnumber);
    }; //End of j loop for Steps

    
    // Creates a temp matrix for "all locations"
    //std::vector<std::vector<double> > TempAllLocations = Animal1.getAllLocations();
    
    //int maxiteration=0;
    
    // Temp location file is written in csv file
    // Each location is a seperate row  - the number of rows = "TempAllLocations.size()"
    //for(int stepcounter=0; stepcounter<TempAllLocations.size(); stepcounter++){
    //    std::vector<double>  locations = TempAllLocations[stepcounter];
    //    std::vector<double>  previouslocations;
   //     if(stepcounter>0){previouslocations = TempAllLocations[stepcounter-1];}else{previouslocations = TempAllLocations[stepcounter];};
   //
    //    if(TempAllLocations[stepcounter].size()>0){ maxiteration=stepcounter;};
        //CheckSensor(locations, previouslocations, AllSensors, Movement, Captures, stepcounter, i, iterationnumber,perchvalue,speedvalue);
        
   // }; // End of for step loop
    
    

}