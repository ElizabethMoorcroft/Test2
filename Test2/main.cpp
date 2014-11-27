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
    
    std::vector<double> Values1(3);
    Values1[0] = 0; Values1[1] = 0.46; Values1[2] =  0*M_PI/180;
    simulatevalues(Values1);
    
    Values1[0] = 0; Values1[1] = 0.46; Values1[2] =  180*M_PI/180;
    simulatevalues(Values1);

    
    std::cout<<"Finished" <<std::endl;
    t2 = clock();

    double seconds = difftime(t2,t);
    std::cout<<"mins " <<seconds/CLOCKS_PER_SEC/60 <<std::endl;
    
    // Return is null
    return 0;
}; 
/*------------------------------------------------------
///        !!!   END OF CODE   !!!!   
---------------------------------------------------------*/