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

// Updates the movement
//  - Sets random location/angle
//  - Sets movement values
//  - Updates movement 
AnimalMovement::AnimalMovement( std::vector<Animal*> AllAnimals ,  std::vector<Sensor*> AllSensors , std::ofstream &Captures, std::ofstream &Movement, double randomstart, double randommove, int i, int iterationnumber, double speedvalue, double perchvalue, double maxchangeanglevalue){
    
    //Print out animal number to screen
    std::cout <<"Animal:" << i+1 <<"/" << NoAnimal << std::endl;
    RandNum RandomNumber1;
    srand(randomstart);
    double seed1, seed2, seed3, temp;
    for(int j=0; j<151; j++){
        if(j==50){seed1=double(rand());}
        else if(j==100){seed2=double(rand());}
        else if(j==150){seed3=double(rand());}
        else {temp=double(rand());};
    };
    
    //sets random location
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

}