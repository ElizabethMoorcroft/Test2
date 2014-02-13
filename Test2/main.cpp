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
    
   /* double speedmatrix[2] = {0.31};
    double anglematrix[4] = {0,45*M_PI/180,90*M_PI/180};
    
    
    for(int simulationnumber1=0; simulationnumber1<1; simulationnumber1++){
        for(int simulationnumber2=0; simulationnumber2<3; simulationnumber2++){
            
            std::cout<< "simulationnumber1: " <<simulationnumber1 <<" simulationnumber2: " <<simulationnumber2 <<std::endl;
            double speedvalue= speedmatrix[simulationnumber1];
            double maxchangeanglevalue = anglematrix[simulationnumber2];
        
            std::vector<double> Values1(3);
            Values1[0] = 0; Values1[1] = speedvalue; Values1[2] =  maxchangeanglevalue;
            simulatevalues(Values1);
            //std::cout<< "finsihed  - simulationnumber1: " <<simulationnumber1 <<" simulationnumber2: " <<simulationnumber2 <<std::endl;

        };
        std::cout<<"Finished" <<std::endl;
    };
    */
    std::vector<double> Values1(3);
    Values1[0] = 0; Values1[1] = 0.5; Values1[2] =  0;
    simulatevalues(Values1);
   /* std::cout<< "Extra1" <<std::endl;
    std::vector<double> Values1(3);
    Values1[0] = 0; Values1[1] = 0.00012; Values1[2] =  0;
    simulatevalues(Values1);
    
    std::cout<<  "Extra2" <<std::endl;
    Values1[0] = 0; Values1[1] = 0.46; Values1[2] =  0;
    simulatevalues(Values1);
    
    std::cout<<  "Extra3" <<std::endl;
    Values1[0] = 0; Values1[1] = 0.15; Values1[2] =  180*M_PI/180;
    simulatevalues(Values1);
    */
    
    std::cout<<"Finished" <<std::endl;
    
    // Return is null
    return 0;
}; 
/*------------------------------------------------------
///        !!!   END OF CODE   !!!!   
---------------------------------------------------------*/