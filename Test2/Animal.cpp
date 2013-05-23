//
//  Animal.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 23/02/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "Animal.h"
#include "RandNum.h"
#include "Parameters.h"




// Constructor
Animal::Animal( ){};

Animal::Animal( int a, int b, int c, int d,
               double e, double f, double g,
               double i, double j, double k, double l,
               double o, double p, double q,
               int r,  double t, double u, double v
               ) {
    
    identifier = a;
    HomeRange_id = b;
    step_number = c;
    Movement_type = d;
    
    Move_state = e;
    Move_speed = f;
    Move_maxangle = g;
    
    Current_x = i;
    Current_y = j;
    Current_angle = k;
    Current_distance = l;
    
    Call_amp = o;
    Call_freq = p;
    Call_width = q;
    
    
    Home_state = r ;  // HR = 1=Y or 0=N
    Home_x = t;
    Home_y = u;
    Home_r = v;
    
                }


////////////////////////////
/// Weird function things///
////////////////////////////

void Animal::Set_MoveValue (double a, double b) {
    Move_speed = a;
    Move_maxangle = b;
    
}

void Animal::Set_HRValues (int a, int b, double c, double d, double e) {
    Home_state = a;
    Home_x = c;
    Home_y = d;
    Home_r = e;
}


///////////////////////
/// UPDATE LOCATION ///
///////////////////////

void Animal::UpdateLocation (double a, double b){ // a is the number of seconds per step, b is the random seed
    
    //Renames variables
    double StepLength = a;
    double seed = b;
    //std::cout<< seed <<std::endl;
    
    //List of random number
    srand(seed);
    std::vector<double> RandomNumberUpdateMovement;
    for(int i=0; i<100000; i++){ //Do I need to increase the max value??
        double myrandomnumber =  rand();
        RandomNumberUpdateMovement.push_back(myrandomnumber);
    };
    
    
    /*
    std::cout << "ENTER UPDATE" <<std::endl;
    std::cout << Home_state <<std::endl;
     */
    
    //set up a random number
    RandNum Number1;
    
    //Sets up temporary X and Y values
    double TempX;
    double TempY;
    double TempAngle;
    double TempDist;
    double TempDistToHR = Home_r+1;
    int i = 0; //index is the value within side the desired range
    int count = 0; // Number of times the loop has been looped through
    
    
    //calculates a new random step distance
    double Dist = Move_speed*StepLength;
    
    ////////////////////////////
    /// HR bounding movement ///
    ////////////////////////////
    if(Home_state==1){
        while(i==0){
            
            // Calculate a distance value
            TempDist = Number1.PositiveNormal (RandomNumberUpdateMovement[count],Dist,Dist/10);
        
            //calculates a new random angle of movement
            TempAngle = Number1.AtoRangeBUnif(RandomNumberUpdateMovement[count+99],Current_angle,Move_maxangle);
    
            //Based on polar coordinates updates the temp x/y location
            TempX = Current_x + TempDist*sin(TempAngle);
            TempY = Current_y + TempDist*cos(TempAngle);
            
        
            //**
            //Need to include a Formal Test new location inside the HR??
            //**
            
            
            // Distance from centre of hr to animal
            double TempDistToHR =sqrt(pow(TempX-Home_x,2)+pow(TempX-Home_x,2));
            
            
            /*
            std::cout << TempDistToHR <<std::endl;
            std::cout << Home_r  <<std::endl;
            std::cout << TempX <<std::endl;
            std::cout << TempY <<std::endl;
            */
            
            count = count +1;
            
            //While loop wasn't working
            //if new location is inside the home range then let 
            if(TempDistToHR<Home_r){ i=i+1;}
            
        } //while (TempDistToHR > Home_r);
        
        // rewrite current locaion
        Current_x = TempX;
        Current_y = TempY;
        Current_angle = TempAngle;
        Current_distance = Current_distance + TempDist;
        
        //Add to the all locations
        std::vector<double> mylocationvector;
        mylocationvector.push_back (identifier);
        mylocationvector.push_back (step_number);
        mylocationvector.push_back (TempX);
        mylocationvector.push_back (TempY);
        mylocationvector.push_back (TempAngle);
        mylocationvector.push_back (Current_distance);
        All_locations.push_back (mylocationvector);
    }
    
    
    ////////////////////////////////
    /// HR NOT bounding movement ///
    ////////////////////////////////
    if(Home_state==0){
        
        // Gas movmeent there is no change in the angle of movemnet
        TempAngle=Current_angle;
        
        TempDistToHR=0;
        
        
        // Calculate a distance value
        TempDist = Number1.PositiveNormal (RandomNumberUpdateMovement[count],Dist,Dist/10);
        
        
        //Based on polar coordinates updates the temp x/y location
        TempX = Current_x + TempDist*sin(TempAngle);
        TempY = Current_y + TempDist*cos(TempAngle);
        
        // If the movement finishes inside the environment
        if(TempX < Sq_MaxX && TempX > Sq_MinX && TempY < Sq_MaxY && TempY > Sq_MinY){
            //Can save the temp location as the current location
            // rewrite current locaion
            Current_x = TempX;
            Current_y = TempY;
            Current_angle = TempAngle;
            Current_distance = Current_distance + TempDist;
            
            //Add to the all locations
            std::vector<double> mylocationvector;
            mylocationvector.push_back (identifier);
            mylocationvector.push_back (step_number);
            mylocationvector.push_back (TempX);
            mylocationvector.push_back (TempY);
            mylocationvector.push_back (TempAngle);
            mylocationvector.push_back (Current_distance);
            All_locations.push_back (mylocationvector);
        }
        //
        else {
        
        
        }
        /*
         ENTRE PERIODIC BOUNDARY MOVEMENT 
        */
    }
   
    //std::cout << "Out of loop "<< TempDistToHR <<std::endl;
    
    /*
    //Print to test values
    std::cout << TempX <<std::endl;
    std::cout << TempY <<std::endl;
    std::cout << TempAngle <<std::endl;
    std::cout << TempDist <<std::endl;
    */
    
    
    


    
    step_number =step_number+1;
}