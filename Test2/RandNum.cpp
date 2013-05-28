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
long double RandNum::PositiveNormal (double a,double b,double c){
    
    double seed = a;
    double Mean = b;
    double SD = c;
    
    
    long double RV_1;
    /*long double RV_2 = -1;*/
    
    srand(seed);
    std::vector<double> RandomNumberStreamTemp;
    for(int i=0; i<100000; i++){
        double myrandomnumber =  rand();
        RandomNumberStreamTemp.push_back(myrandomnumber);
    };
    
    int count=0;
    
    //Does the follwing while RV_1 is less than 0
    do{
        // set random seed for the varaibles
        srand(RandomNumberStreamTemp[count]);
        
        //To generate from a normal distribution we use the following:
        // U~Unif(0,1)
        // Then R ~ exp(lambda) = -log(U)/lambda
        // Then Theta~Unif(0,2*pi)
        // We want:
        // X~Norm(0,1), and, Y~Norm(0,1) where X and Y are independent
        // X =sqrt(R)*sin(Theta)
        // Y =sqrt(R)*cos(Theta)
        
        //Two varaibels from random uniform dist
        long double v1 = ((long double) rand()/RAND_MAX);
        long double v2 = ((long double) rand()/RAND_MAX);
        
        //Generate Expoential(1/2) and Uniform(0,2pi) from v1&v2
        v1 = -2*log(v1);
        v2 = -2*M_PI*log(v2);
        
        //Generate a normal from v1/v2
        RV_1 = sqrt(v1)*sin(v2);
        /*RV_2 = sqrt(v1)*cos(v2);*/
        
        //Converts the RV_1 with a Norm(0,1) to a Norm(b,c)
        RV_1 = RV_1*SD + Mean;
        
        //Adds to the seed number to produce a different random number
        count=count+1;

        
    } while(RV_1<=0);

    return (RV_1);
    
}
////////////////////
// End of +veNorm //
////////////////////



//a is the seed value
//b is the current angle
//c is the +/- range
long double RandNum::AtoRangeBUnif (double a,double b,double c){
    
    double Seed= a;
    
    //Sets seed for a random number
    srand(Seed);
    
    // Uniform 0 - 1 random number
    long double v1 = ((long double) rand()/RAND_MAX);
    
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
long double RandNum::AtoBUnif (double a,double b,double c){
    
    //Rename variables
    double Seed= a;
    double Min= b;
    double Max= c;
    
    //Sets seed for a random number
    srand(Seed);
    
    // Uniform 0 - 1 random number
    long double v1 = ((long double) rand()/RAND_MAX);
    
    double range = Max - Min;
    
    // rescale number to be between minumum and maximum
    v1 = range*v1 + Min ;
    
    //returns value
    return v1;
}
////////////////////////
// End of A to B Unif //
////////////////////////
