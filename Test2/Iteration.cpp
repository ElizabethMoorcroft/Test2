//
//  Iteration.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 21/11/2013.
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


#include "Iteration.h"
#include "Animal.h"
#include "AnimalMovement.h"
#include "CheckSensor.h"
#include "Sensors.h"
#include "Animal.h"
#include "RandNum.h"
#include "Parameters.h"


Iteration::Iteration(){};

Iteration::Iteration(std::vector<Sensor*> AllSensors, std::ofstream &Captures, std::ofstream &Movement, int iterationnumber, double speedvalue, double perchvalue, double maxchangeanglevalue){
    std::cout<<"Iteration: "<<iterationnumber<<"/"<<Seed + NoOfIterations -1 <<std::endl;
    
    /*-------------------------------------------------------
     // START OF RANDOM NUMBER STREAM
     //
     // Creates the start of the random number stream
     // This is started with the iteration number
     // This makes each run completely replicable
     //
     // This stream of numbers 2 other streams
     //  - RandomNumberStreamAnimalStart (start location)
     //  - RandomNumberStreamAnimalMove (Seed for movement)
     --------------------------------------------------------*/
    srand(iterationnumber);
    double seedstart, seedmove, temp;
    for(int i=0; i<76; i++){
        if(i==25){seedstart=double(rand());}
        else if(i==75){seedmove=double(rand());}
        else {temp=double(rand());};
    };
    
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
    srand(seedstart);
    std::vector<double> RandomNumberStreamAnimalStart(NoAnimal);
    for(int i=0; i<NoAnimal; i++){
        RandomNumberStreamAnimalStart[i]=double(rand());
        for(int j=0; j<NoAnimal;j++){temp=double(rand());}
    };
    
    
    // Random number stream seed for the seed for the movement
    srand(seedmove);
    std::vector<double> RandomNumberStreamAnimalMove(NoAnimal);
    for(int i=0; i<NoAnimal; i++){
        RandomNumberStreamAnimalMove[i]=double(rand());
        for(int j=0; j<NoAnimal;j++){temp=double(rand());}
    };
    
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
        //std::cout<<"Animals numbers: " <<i+1<<"/" <<NoAnimal <<" Random number "<< RandomNumberStreamAnimalMove[i] <<std::endl;
        double randomstart = RandomNumberStreamAnimalStart[i];
        double randommove = RandomNumberStreamAnimalMove[i];
        
        AnimalMovement( AllAnimals , AllSensors ,  Captures,  Movement, randomstart,  randommove, i, iterationnumber, speedvalue, perchvalue, maxchangeanglevalue);
        
    }; //End of Individual loop
    
};