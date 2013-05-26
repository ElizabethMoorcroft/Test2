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


    
    //Seed for random numbers
    double SEED = b;
    double RadiusCameraCircle = c;
    
    
    ////////////////////
    // Random stream ///
    ////////////////////
    // 12 numbers created
    // Used to seed other streams
    // Position 1 used to seed stream in 
    srand(SEED);
    std::vector<double> RandomNumberStreamHRlocation;
    RandomNumberStreamHRlocation.resize(7);
    for(int i=0; i<7; i++){
        RandomNumberStreamHRlocation[i] =double(rand());
    };
    
    
    /////////////////////////
    ///Temporary variables///
    /////////////////////////
    
    double temp_x;
    double temp_y;
    double temp_size;
    
    
    
    //Creates a random number object
    RandNum Number1;
    
    
    /////////////////////////////
    ///No solid HR boundaries ///
    /////////////////////////////
    
    //If HR_SolidBoundaries=0 HRs can be placed anywhere in the range
    if(HR_SolidBoundaries==0){
        
        //Creates a random number stream
        //Positions 0 and 5 are used to create RandNum
        srand(SEED);
        std::vector<double> RandomNumberStreamHRlocationT0;
        for(int i=0; i<6; i++){
            double myrandomnumber =  rand();
            RandomNumberStreamHRlocationT0.push_back(myrandomnumber);
        };
        
        //Creates x and y locations
        temp_x = Number1.AtoBUnif(RandomNumberStreamHRlocationT0[0], Sq_MinX, Sq_MaxX);
        temp_y = Number1.AtoBUnif(RandomNumberStreamHRlocationT0[5], Sq_MinY, Sq_MaxY);
        
        // If there are no HR boundaries there is no need to calculate temp_size
        temp_size = 0.0;
        
    }; //END IF HR_SolidBoundaries==0
    
    
    //////////////////////////
    ///Solid HR boundaries ///
    //////////////////////////
    
    //HR_SolidBoundaries= 1 if the boundaries are solid
    if(HR_SolidBoundaries==1){

        //Creates a random number stream
        //Positions 0 and 5 and 10 are used to create RandNum
        srand(SEED);
        std::vector<double> RandomNumberStreamHRlocationT1;
        for(int i=0; i<11; i++){
            double myrandomnumber =  rand() ;
            RandomNumberStreamHRlocationT1.push_back(myrandomnumber);
        };
        
    
        //Calculates a random distance from the centre of the trapping circle
        //Calculates a random angle
        //This means that the can simply calculate 2 uniform values rather than recalucalte outside limits
        double MinRadius =0;
        if(RadiusCameraCircle-HR_AverageRadius>0){MinRadius=RadiusCameraCircle-HR_AverageRadius;};
        
        double temp_r = Number1.AtoBUnif(RandomNumberStreamHRlocationT1[0], MinRadius, RadiusCameraCircle+HR_AverageRadius);
        double temp_theta = Number1.AtoBUnif(RandomNumberStreamHRlocationT1[5], 0, 2*M_PI);
        
        //Tempory x and y locations
        //Using the r*sin(theta) and r*cos(theta) method gives x and y centred around (0,0)
        temp_x = temp_r*sin(temp_theta);
        temp_y = temp_r*cos(temp_theta);
        
        //Relocates so that centre is now Cir_CntX & Cir_CntY
        temp_x = Cir_CntX +temp_x;
        temp_y = Cir_CntY +temp_y;
        
        // Calucalte a radom radius size for the HR
        temp_size = Number1.PositiveNormal(RandomNumberStreamHRlocationT1[10],HR_AverageRadius,HR_AverageRadius/10);
        
    } //END IF HR_SolidBoundaries==1
    
    
    /////////////////////////////////
    ///Allocations the x&y values ///
    /////////////////////////////////
    
    //std::cout<<temp_x<<std::endl
    // Assigns the temp values to the permanent values
    Home_X = temp_x;
    Home_Y = temp_y;
    Home_Size = temp_size;
    
    

}; //END FUNCTION