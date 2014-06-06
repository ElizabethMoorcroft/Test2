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
#include "CheckSensor.h"


/*--------------------------------------------------------------------------------------------------------
 //
 //                                        Constructors
 //
 --------------------------------------------------------------------------------------------------------*/
Animal::Animal(){};

Animal::Animal(int a, double i, double j, double k, std::ofstream &Movement, std::vector<Sensor*> AllSensors , std::ofstream &Captures, int iterationnumber) {
    
    //std::cout <<"Inside Animals"<< std::endl;
    
    //Sets the animal parameters
    identifier = a;
    Current_x = i;
    Current_y = j;
    Current_angle = k;
    Current_angle= RangeAngle(Current_angle);
    StepLengthDist = StepLength*Move_speed;
    
    // When initialising the step number and the current distance travelled will always be set to zero
    step_number = 0;
    Total_distance = 0;
    
    //Set up like this so in the future can be initalized as RV??
    
    // Updates the location vector
    All_locations.resize(10*NoSteps);
    mylocationvector.resize(9);
    locationvectorcount =0;
    
    //Entres the start location into the matrix
    LocationVector(Current_x,Current_y,Current_x,Current_y,0,1, Movement, AllSensors , Captures, iterationnumber);
    
    //std::cout<<"Move speed " <<Move_speed<< std::endl;
};


/*--------------------------------------------------------------------------------------------------------
 //
 //                                        Set functions
 //
 --------------------------------------------------------------------------------------------------------*/
//Set functions
void Animal::setMove_speed(double a){
    Move_speed = a;
    StepLengthDist = StepLength*Move_speed;
};

//Set functions
void Animal::setAnimalspeedPerch(double a, double b){
    Move_speed = a/(1-b);
    StepLengthDist = StepLength*Move_speed;
    perchIndivid=b;
};




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
void Animal::LocationVector(double& locationx, double& locationy, double pLocationx, double pLocationy, int LeaveEntreCode, int End, std::ofstream &Movement, std::vector<Sensor*> AllSensors , std::ofstream &Captures, int iterationnumber){
    //Records exit of the world

    if(SaveMovement==1 || iterationnumber==NoOfIterations){
        Movement<< identifier << //1st column, row "stepcounter"
        "," << step_number << //2nd column, row "stepcounter"
        "," << locationx << //...
        "," << locationy << //...
        "," << NextAngle << //...
        "," << Total_distance << //...
        "," << Move_speed << //...
        "," << LeaveEntreCode << //8th column, row "stepcounter"
        "," << NextDist << //8th column, row "stepcounter"
        "," << iterationnumber <<                  // itertaion number
        "\n";
    };
    
    CheckSensor(locationx, locationy, NextAngle,
                pLocationx, pLocationy,
                AllSensors,
                Movement,
                Captures,
                step_number,
                identifier,
                iterationnumber,
                perchIndivid,
                Move_speed);
    
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
void  Animal::UpdateLocation (double seed, std::ofstream &Movement, std::vector<Sensor*> AllSensors , std::ofstream &Captures, int iterationnumber){ // a is the number of seconds per step, b is the random seed
    
    //std::cout <<"Inside Update locations: "<< seed<< std::endl;
    
    double previousx = Current_x;
    double previousy = Current_y;
    
    // This starts a stream of random numbers used twice
    //  -> To start a new stream of RandNum for the update of movement
    //  -> To start a new stream of RandNum for the probability of changing states in 2 state corr walk (MoveType==2)
    srand(seed);
    double seed1, seed2, seed3, temp;
    for(int i=0; i<101; i++){
        if(i==0){seed1=double(rand());}
        else if(i==50){seed2=double(rand());}
        else if(i==100){seed3=double(rand());}
        else {temp=double(rand());};
    };
    
    double probofperch=0;
    
    if(perchIndivid!=0){
        //std::cout<<"Perching"<<std::endl;
        RandNum Number1;
        probofperch = Number1.AtoBUnif(seed3,0,1);
    };
    if(probofperch<(1-perchIndivid)){
        //std::cout<<"NewLocation"<<std::endl;
        NewLocation(seed1, seed2);
    }else{
        NextX= Current_x;
        NextY= Current_y;
        NextDist = 0;
        NextAngle = RangeAngle(Current_angle);
    }
    
    int tempcounter = 0;
    while(tempcounter<1){
                
        //std::cout<< "ENTERED WHILE" << std::endl;
        // If the movement finishes inside the environment
        if(NextX < Sq_MaxX && NextX > Sq_MinX && NextY < Sq_MaxY && NextY > Sq_MinY){
            
            
            //Can save the temp location as the current location
            // rewrite current locaion
            Current_x = NextX;
            Current_y = NextY;
            Total_distance += NextDist;
            Current_angle = RangeAngle(NextAngle);
            
            
            //Add to the all locations
            LocationVector(Current_x,Current_y, previousx, previousy, 0,1, Movement, AllSensors , Captures, iterationnumber);
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
            if(NextAngle>=0 && NextAngle<M_PI/2){LeaveEnterWorld(Sq_MaxY, Sq_MaxX ,Sq_MinY, Sq_MinX, Movement, AllSensors , Captures, iterationnumber, previousx, previousy); }
            // If angle between 90 and 180 degrees
            // Then will exit either at Bottom or right boundary
            else if(NextAngle>=M_PI/2 && NextAngle<M_PI){LeaveEnterWorld(Sq_MinY, Sq_MaxX ,Sq_MaxY, Sq_MinX, Movement, AllSensors , Captures, iterationnumber, previousx, previousy);}
            // If angle between 180 and 270 degrees
            //then will exit either at Bottom or right boundary
            else if(NextAngle>=M_PI && NextAngle<1.5*M_PI){LeaveEnterWorld(Sq_MinY, Sq_MinX ,Sq_MaxY, Sq_MaxX, Movement, AllSensors , Captures, iterationnumber, previousx, previousy);}
            // If angle between 270 and 360 degrees
            //then will exit either at Bottom or right boundary
            else if(NextAngle>=1.5*M_PI && NextAngle<2*M_PI){LeaveEnterWorld(Sq_MaxY, Sq_MinX ,Sq_MinY, Sq_MaxX, Movement, AllSensors , Captures, iterationnumber, previousx, previousy);}
            // Produces error is the ANGLE is not between 0 and 360
            else {std::cout << "ERROR - Movement angle: "<<NextAngle <<std::endl;  exit(EXIT_FAILURE);};
            
            // Produces error if the Next distance is less than zero
            // But exits loop and continues with the rest of the code
            if(NextDist<0){ std::cout<< "ERROR - Next dist: " << NextDist<<" StepLengthDist " <<StepLengthDist<< std::endl; exit(EXIT_FAILURE);};
                
        }; // End of Else loop;
            
    }; //End of While loop
    
    //Increases step number by one once the animal finishes moving in the environment
    step_number =step_number+1;
    
};//End of update location


/*----------------------------------------------
 // New location works out which movement type 
 // is needed initiating the correct function
 ----------------------------------------------*/
void Animal::NewLocation (double& seed, double& seed2){
    
    //set up a random number
    RandNum Number1;
    //Correlated random walk
    // Calculate a distance value
    NextDist = Number1.PositiveNormal(seed,StepLengthDist,StepLengthDist/10);
    
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


void Animal::LeaveEnterWorld(const double& YBoundExit, const double& XBoundExit, const double& YBoundEnter, const double& XBoundEnter, std::ofstream &Movement, std::vector<Sensor*> AllSensors , std::ofstream &Captures, int iterationnumber, double pLocationx, double pLocationy){
    
    // Caculate the V & H distances to the boudaries
    double tempDistToTop = std::fabs(YBoundExit-Current_y);
    double tempDistToSide = std::fabs(XBoundExit-Current_x);
    
    // Calculates the distnace the animal travels to the boudary
    double tempDistToTopBoundary = std::fabs(tempDistToTop/cos(NextAngle));
    double tempDistToSideBoundary = std::fabs(tempDistToSide/sin(NextAngle));


    
    double tempExitsY =0;
    double tempExitsX =0;
    
    
    if(tempDistToSideBoundary>tempDistToTopBoundary | tempDistToSideBoundary ==0 ){
    
        //This means that it exits the world at maximum value of y and at the corresponding x value
        tempExitsY = YBoundExit;
        tempExitsX = CalNext_X(tempDistToTopBoundary);
        
        //New starting position and angle
        Current_y = YBoundEnter;
        Current_x = tempExitsX;
        Current_angle = NextAngle;
        // The distance left to travel
        //tempdisttrav = tempDistToTopBoundary;
        // The distance left to travel
        NextDist -=tempDistToTopBoundary;
        // The current distance travelled
        Total_distance += tempDistToTopBoundary;
        
    }// end of exit by the top/bottom of the world 
    else{
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

    }; // end of exit by the side of world

    //New end locations after the re-entry
    NextX = CalNext_X(NextDist);
    NextY = CalNext_Y(NextDist);
    
    //Updates the location vector with the EXIT location
    LocationVector(tempExitsX,tempExitsY,pLocationx,pLocationy,0,0, Movement, AllSensors , Captures, iterationnumber);
    // Updates the location vector with the RE-ENTRY location
    LocationVector(Current_x,Current_y,pLocationx,pLocationy,1,0, Movement, AllSensors , Captures, iterationnumber);
    
};