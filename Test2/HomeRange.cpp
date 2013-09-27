//
//  HomeRange.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 24/02/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include "HomeRange.h"
#include "RandNum.h"
#include "Parameters.h"
#include <iostream>
#include <cstdio>
#include <math.h>
#include <time.h>
#include <vector>

// List of variables
    //a identifier; //The HR id number
    //b type;       //Type of HR
    //c Home_X;  //Location of Home Range X
    //d Home_Y;  //Location of Home Range Y
//If home ranges can occur with a square (TYPE1)
    //e Sq_MinX;
    //f Sq_MaxX;
    //g Sq_MinY;
    //h Sq_MaxY;
//If home ranges can occur with a circle (TYPE2)
    //i Cir_CntX;
    //j Cir_CntY;
    //k Cir_MinRng;
    //l Cir_MaxRng;
//Seeds
    //m seed


HomeRange::HomeRange(){};
HomeRange::HomeRange(int a//identifier; //The HR id number
                     ,double b
                     ,double c
                     ){

    identifier = a; //The HR id number
    recal=0;
    


    
    //Seed for random numbers
    double SEED = b;
    double RadiusCameraCircle = c;

    //std::cout<<"SEED: " <<SEED <<std::endl;
    
    ////////////////////
    // Random stream ///
    ////////////////////
    // 12 numbers created
    // Used to seed other streams
    // Position 1 used to seed stream in 
    srand(SEED);
    std::vector<double> RandomNumberStreamHRlocation(11);
    for(int i=0; i<11; i++){
        RandomNumberStreamHRlocation[i] =double(rand());
    };

    //std::cout<<"percentagemales"<< percentagemales<<" SEX: "<<sex<< std::endl;
    /////////////////////////
    ///Temporary variables///
    /////////////////////////
    
    double temp_x;
    double temp_y;
    //double temp_size;
    
    
    
    //Creates a random number object
    RandNum Number1;
    
    
    /////////////////////////////
    ///No solid HR boundaries ///
    /////////////////////////////
    
    //If HR_SolidBoundaries=0 HRs can be placed anywhere in the range
    if(HR_SolidBoundaries==0){
        //std::cout<<"No home range boundaries" <<std::endl;
        //Creates a random number stream
        //Positions 0 and 5 are used to create RandNum
        srand(RandomNumberStreamHRlocation[5]);
        std::vector<double> RandomNumberStreamHRlocationT0(6);
        for(int i=0; i<6; i++){
            RandomNumberStreamHRlocationT0[i] = double(rand());
        };
        
        //Creates x and y locations
        Home_X = Number1.AtoBUnif(RandomNumberStreamHRlocationT0[0], Sq_MinX, Sq_MaxX);
        Home_Y = Number1.AtoBUnif(RandomNumberStreamHRlocationT0[5], Sq_MinY, Sq_MaxY);
        
        // If there are no HR boundaries there is no need to calculate temp_size
        
    }; //END IF HR_SolidBoundaries==0
    
    
    //////////////////////////
    ///Solid HR boundaries ///
    //////////////////////////
    
    //HR_SolidBoundaries= 1 if the boundaries are solid
    if(HR_SolidBoundaries==1){

        //Creates a random number stream
        //Positions 0 and 5 and 10 are used to create RandNum
        srand(RandomNumberStreamHRlocation[10]);
        std::vector<double> RandomNumberStreamHRlocationT1(11);
        for(int i=0; i<11; i++){
            RandomNumberStreamHRlocationT1[i] = double(rand());
        };
        
        double temp_r;
        double temp_theta;
        
       if(DetectorLayOut==0){
            //std::cout<<"Single detector"<<std::endl;
            temp_r = Number1.AtoBUnif(RandomNumberStreamHRlocationT1[0], 0, HR_AverageRadius + DetectorRadius);
            temp_theta = Number1.AtoBUnif(RandomNumberStreamHRlocationT1[5], 0, 2*M_PI);
            //Tempory x and y locations
            //Using the r*sin(theta) and r*cos(theta) method gives x and y centred around (0,0)
            Home_X = (Sq_MaxX/2) + temp_r*sin(temp_theta);
            Home_Y = (Sq_MaxY/2) + temp_r*cos(temp_theta);
        }
        else if(DetectorLayOut==1){

            GridDetectors(RandomNumberStreamHRlocationT1[0]);
            
        };
    } //END IF HR_SolidBoundaries==1
    

    

}; //END FUNCTION




/*----------------------------------------------------------------------------------------
 //
 // Random home ranges when there is a grid of dectectors
 //
 //
 //
 ----------------------------------------------------------------------------------------*/

void HomeRange::GridDetectors(double SEED){
    
    //Creates a random number object
    RandNum Number1;
    
    // A new stream of random numbers
    srand(SEED);
    std::vector<double> RandomNumberStreamHRGridlocation(11);
    for(int i=0; i<11; i++){
        RandomNumberStreamHRGridlocation[i] =double(rand());
    };
    
    // calculates the Home_X and home_Y
    // between the minumum value that can be reached from a HR centre
    // and the maximum values that can be reached from a HR centre
    Home_X = Number1.AtoBUnif(RandomNumberStreamHRGridlocation[0], Xgridmin - HR_AverageRadius, MaxNoX*Xspace + HR_AverageRadius);
    Home_Y = Number1.AtoBUnif(RandomNumberStreamHRGridlocation[10], Ygridmin - HR_AverageRadius, MaxNoY*Yspace + HR_AverageRadius);

};
