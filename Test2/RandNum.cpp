//
//  RandNum.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 24/02/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include "RandNum.h"
#include <iostream>
#include <vector>
#include <math.h>


//Calcaulte a random variable that is normally distribtued
//But if the value is negative them it recalculates it until +ve
double RandNum::PositiveNormal (double seed, double Mean, double SD){
    
    
    double RV_1=0;
    
    //std::cout<<"Seed inside: " << seed <<std::endl;
    
    int count=2;
    
    //Does the follwing while RV_1 is less than 0
     while(RV_1<=0){
        srand(seed);
        std::vector<double> RandomNumberStreamTemp(count*100);
        for(int i=0; i<(count*100); i++){
             RandomNumberStreamTemp[i] =double (rand());
        };
        // set random seed for the varaibles
        srand(RandomNumberStreamTemp[count*100-1]);
        
        //To generate from a normal distribution we use the following:
        // U~Unif(0,1)
        // Then R ~ exp(lambda) = -log(U)/lambda
        // Then Theta~Unif(0,2*pi)
        // We want:
        // X~Norm(0,1), and, Y~Norm(0,1) where X and Y are independent
        // X =sqrt(R)*sin(Theta)
        // Y =sqrt(R)*cos(Theta)
        
        //Two varaibels from random uniform dist
        double v1 = ((double) rand()/RAND_MAX);
        double v2 = ((double) rand()/RAND_MAX);
        
        //Generate Expoential(1/2) and Uniform(0,2pi) from v1&v2
        double R = -2*log(v1);
        double theta = 2*M_PI*v2;
        
        //Generate a normal from v1/v2
        RV_1 = sqrt(R)*sin(theta);
        /*RV_2 = sqrt(R)*cos(theta);*/
        
        //Converts the RV_1 with a Norm(0,1) to a Norm(b,c)
        RV_1 = RV_1*SD + Mean;
        
        //Adds to the seed number to produce a different random number
        count=count+1;

        
    };

    return (RV_1);
    
}
////////////////////
// End of +veNorm //
////////////////////



//a is the seed value
//b is the current angle
//c is the +/- range
double RandNum::AtoRangeBUnif (double& a,double b,double c){
    
    double Seed= a;
    
    //Sets seed for a random number
    srand(Seed);
    
    // Uniform 0 - 1 random number
    double v1 = ((long double) rand()/RAND_MAX);
    
    // Uniform -1 - 1 random number
    v1 = (v1-0.5)*2;
    
    // Uniform -c - +c random number
    v1 = v1*c;
    
    // Uniform  b+/-c random number
    v1 = v1+b;    
    
    
    //returns value
    return v1;
}
//////////////////////////////
// End of A to Range B Unif //
//////////////////////////////

//a is the seed value
//b is the Min value
//c is the Max value
double RandNum::AtoBUnif (double& Seed,double Min,double Max){
    
    //Sets seed for a random number
    srand(Seed);
    
    // Uniform 0 - 1 random number
    double v1 = ((double) rand()/RAND_MAX);
    
    double range = Max - Min;
    
    // rescale number to be between minumum and maximum
    v1 = range*v1 + Min ;
    
    //returns value
    return v1;
}
////////////////////////
// End of A to B Unif //
////////////////////////
