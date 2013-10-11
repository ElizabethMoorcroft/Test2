/*--------------------------------------------------------------------------------------------------------
//
//  Animal.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 23/02/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//
// Purpose: Class of animals
//          Animals move around the environment interacting with detectors
//          This section of code details with the specification and movement of animals
--------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------
 //
 //                                        Include statements
 //
 --------------------------------------------------------------------------------------------------------*/

#include <iostream>
#include <math.h>
#include <cmath> 
#include <time.h>
#include "Animal.h"
#include "RandNum.h"
#include "Parameters.h"
#include "Sensors.h"


/*--------------------------------------------------------------------------------------------------------
 //
 //                                        Constructors
 //
 --------------------------------------------------------------------------------------------------------*/
Animal::Animal(){};

Animal::Animal(int a, double i, double j, double k) {
    
    //std::cout <<"Inside Animals"<< std::endl;
    
    //Sets the animal parameters
    identifier = a;
    Current_x = i;
    Current_y = j;
    Current_angle = k;
    Current_angle= RangeAngle(Current_angle);
    
    // When initialising the step number and the current distance travelled will always be set to zero
    step_number = 0;
    Total_distance = 0;
    
    //Set up like this so in the future can be initalized as RV??
    Move_maxangle = CorrWalkMaxAngleChange;
    
    // Updates the location vector
    All_locations.resize(2*NoSteps+5);
    mylocationvector.resize(8);
    locationvectorcount =0;
    
    //Entres the start location into the matrix
    LocationVector(Current_x,Current_y,0,1);

};


/*--------------------------------------------------------------------------------------------------------
 //
 //                                        Set functions
 //
 --------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------
 //
 //                                        Mutiple use functions
 //
 --------------------------------------------------------------------------------------------------------*/

//Needs test
/*----------------------------------------------
 // Updates the vectors which contain the movement
 // of the animals
 ----------------------------------------------*/
void Animal::LocationVector(double locationx, double locationy, int LeaveEntreCode, int End){
    //Records exit of the world
    mylocationvector[0] = identifier;
    mylocationvector[1] = step_number;
    mylocationvector[2] = locationx;
    mylocationvector[3] = locationy;
    mylocationvector[4] = NextAngle;
    mylocationvector[5] = Total_distance;
    mylocationvector[6] = AnimalSpeed;
    mylocationvector[7] = LeaveEntreCode;
    All_locations[locationvectorcount]= mylocationvector;
    locationvectorcount+=1;
};

//Needs test
/*----------------------------------------------
 // Calculations for new x&y locations
 // this uses simple trig and is NOT dependent
 //on the direction of travel
----------------------------------------------*/
double Animal::CalNext_X(double distance){double newx = Current_x + distance*sin(NextAngle); return newx;};
double Animal::CalNext_Y(double distance){double newy = Current_y + distance*cos(NextAngle); return newy;};


// Needs test
/*----------------------------------------------
 // RangeAngle corrects the angle so that the
 // angle is between 0 and 2pi
 ----------------------------------------------*/
double Animal::RangeAngle(double angle){
    if(angle<0){angle += 2*M_PI;} else if(angle>=2*M_PI){angle -= 2*M_PI;};
    return angle;
};

/*--------------------------------------------------------------------------------------------------------
 //
 //                                        New locations caluclations
 //
 --------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------
 // The cover that governs the updating of the 
 // movement of a given animal
 ----------------------------------------------*/
void  Animal::UpdateLocation (double seed){ // a is the number of seconds per step, b is the random seed
    
    //std::cout <<"Inside Update locations"<< std::endl;
    
    // This starts a stream of random numbers used twice
    //  -> To start a new stream of RandNum for the update of movement
    //  -> To start a new stream of RandNum for the probability of changing states in 2 state corr walk (MoveType==2)
    srand(seed);
    std::vector<double> RandomNumberMovement(101);
    for(int i=0; i<101; i++){ RandomNumberMovement[i] = double(rand());};

    //std::cout<<"NewLocation"<<std::endl;
    NewLocation(RandomNumberMovement[0], RandomNumberMovement[500]);
        
    int tempcounter = 0;
    while(tempcounter<1){
                
        //std::cout<< "ENTERED WHILE" << std::endl;
        // If the movement finishes inside the environment
        if(NextX < Sq_MaxX && NextX > Sq_MinX && NextY < Sq_MaxY && NextY > Sq_MinY){
            
            //std::cout<< "If entered" << std::endl;
            
            //Can save the temp location as the current location
            // rewrite current locaion
            Current_x = NextX;
            Current_y = NextY;
            Total_distance += NextDist;
            Current_angle = NextAngle;
                
            //Add to the all locations
            LocationVector(Current_x,Current_y,0,1);
            //std::cout<< "Vectors done" << std::endl;
                
            // The end of movement is in the world so
            // ends the while loop by achieving the condition
            tempcounter = 1;
                
        } //End of the IF loop
            
        // If the movement goes outside of the environment
        // Need to work out which pair of possible boundary it leaves first
        // Each pair of boundaries needs to have different inputs for "LeaveEnterWorld"
        else {
            //std::cout<< "Leaves environment"<<std::endl;
            // If angle between 0 and 90 degrees
            // Then will exit either at top or right boundary
            if(NextAngle>=0 && NextAngle<M_PI/2){LeaveEnterWorld(Sq_MaxY, Sq_MaxX ,Sq_MinY, Sq_MinX); }
            // If angle between 90 and 180 degrees
            // Then will exit either at Bottom or right boundary
            else if(NextAngle>=M_PI/2 && NextAngle<M_PI){LeaveEnterWorld(Sq_MinY, Sq_MaxX ,Sq_MaxY, Sq_MinX);}
            // If angle between 180 and 270 degrees
            //then will exit either at Bottom or right boundary
            else if(NextAngle>=M_PI && NextAngle<1.5*M_PI){LeaveEnterWorld(Sq_MinY, Sq_MinX ,Sq_MaxY, Sq_MaxX);}
            // If angle between 270 and 360 degrees
            //then will exit either at Bottom or right boundary
            else if(NextAngle>=1.5*M_PI && NextAngle<2*M_PI){LeaveEnterWorld(Sq_MaxY, Sq_MinX ,Sq_MinY, Sq_MaxX);}
            // Produces error is the ANGLE is not between 0 and 360
            else {std::cout << "ERROR - Movement angle: "<<NextAngle <<std::endl;};
            
            // Produces error if the Next distance is less than zero
            // But exits loop and continues with the rest of the code
            if(NextDist<0){ std::cout<< "ERROR - Next dist: " << NextDist<<std::endl; exit(EXIT_FAILURE);};
                
        }; // End of Else loop;
            
    }; //End of While loop
    
    //Increases step number by one once the animal finishes moving in the environment
    step_number =step_number+1;
    
};//End of update location


/*----------------------------------------------
 // New location works out which movement type 
 // is needed initiating the correct function
 ----------------------------------------------*/
void Animal::NewLocation (double seed, double seed2){

    //set up a random number
    RandNum Number1;
    //Correlated random walk
    // Calculate a distance value
    NextDist = Number1.PositiveNormal (seed,StepLengthDist,StepLengthDist/10);

    //calculates a new random angle of movement
    NextAngle = Number1.AtoRangeBUnif(seed2,Current_angle,Move_maxangle);
    //Sometimes the angle becomes greater than 360 or less than 0
    //Recalculates so that the angle between 0 and 360
    NextAngle = RangeAngle(NextAngle);
    
    //Based on polar coordinates updates the temp x/y location
    NextX = CalNext_X(NextDist);
    NextY = CalNext_Y(NextDist);
    
};


/*--------------------------------------------------------------------------------------------------------
 //
 //                                        Periodic boundaries
 //
 --------------------------------------------------------------------------------------------------------*/


void Animal::LeaveEnterWorld(double YBoundExit, double XBoundExit, double YBoundEnter, double XBoundEnter){
    
    //std::cout <<"Entre LeaveEnterWorld"<<std::endl;
    
    // Caculate the V & H distances to the boudaries
    double tempDistToTop = std::abs(YBoundExit-Current_y);
    double tempDistToSide = std::abs(XBoundExit-Current_x);
    
    // Calculates the distnace the animal travels to the boudary
    double tempDistToTopBoundary = std::abs(tempDistToTop/cos(NextAngle));
    double tempDistToSideBoundary = std::abs(tempDistToSide/sin(NextAngle));
    
    double tempExitsY =0;
    double tempExitsX =0;
    
    if(tempDistToSideBoundary>tempDistToTopBoundary | tempDistToSideBoundary ==0 ){
    
        //std::cout <<"Top"<<std::endl;
        //This means that it exits the world at maximum value of y and at the corresponding x value
        tempExitsY = YBoundExit;
        tempExitsX = CalNext_X(tempDistToTopBoundary);
        
        //New starting position and angle
        Current_y = YBoundEnter;
        Current_x = tempExitsX;
        Current_angle = NextAngle;
        
        // The distance left to travel
        NextDist -= tempDistToTopBoundary;
        // The current distance travelled
        Total_distance += tempDistToTopBoundary;
        
    }// end of exit by the top/bottom of the world 
    else{
        //std::cout <<"Side"<<std::endl;
        //This means that it exits the world at maximum value of y and at the corresponding x value
        tempExitsX = XBoundExit;
        tempExitsY = CalNext_Y(tempDistToSideBoundary);
        
        //New starting position and angle
        Current_y = tempExitsY;
        Current_x = XBoundEnter;
        Current_angle = NextAngle;
        
        // The distance left to travel
        NextDist -=tempDistToSideBoundary;
        // The current distance travelled
        Total_distance += tempDistToSideBoundary;
        //std::cout <<"leaves Side"<<std::endl;
    }; // end of exit by the side of world

    //std::cout <<"vect1 done"<<std::endl;
    //New end locations after the re-entry
    NextX = CalNext_X(NextDist);
    NextY = CalNext_Y(NextDist);
    
    //Updates the location vector with the EXIT location
    LocationVector(tempExitsX,tempExitsY,0,0);
    // Updates the location vector with the RE-ENTRY location
    LocationVector(Current_x,Current_y,1,0);

};