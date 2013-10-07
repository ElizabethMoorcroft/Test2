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


/*-----------------------------------------------------------------------------
//
//                                Constructors
//
-----------------------------------------------------------------------------*/

HomeRange::HomeRange(){};
HomeRange::HomeRange(int a, double SEED){ //identifier, seed

    identifier = a; //The HR id number
    
    /*--------------------*/
    // Random stream ///
    /*--------------------*/
    // 12 numbers created
    // Used to seed other streams
    // Position 1 used to seed stream in 
    srand(SEED);
    std::vector<double> RandomNumberStreamHRlocation(101);
    for(int i=0; i<101; i++){RandomNumberStreamHRlocation[i] =double(rand()); };
    
    //Creates a random number object
    RandNum Number1;
    
    //Creates x and y locations
    Home_X = Number1.AtoBUnif(RandomNumberStreamHRlocation[50], Sq_MinX, Sq_MaxX);
    Home_Y = Number1.AtoBUnif(RandomNumberStreamHRlocation[100], Sq_MinY, Sq_MaxY);
    
}; //END FUNCTION


