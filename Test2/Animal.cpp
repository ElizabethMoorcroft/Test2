//
//  Animal.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 23/02/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <cmath> 
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
    
    
    //Records the movement
    std::vector<double> mylocationvector;
    mylocationvector.push_back (identifier);
    mylocationvector.push_back (step_number);
    mylocationvector.push_back (Current_x);
    mylocationvector.push_back (Current_y);
    mylocationvector.push_back (Current_angle);
    mylocationvector.push_back (Current_distance);
    mylocationvector.push_back (0);
    All_locations.push_back (mylocationvector);
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
            //Sometimes the angle becomes greater than 360 or less than 0
            //Recalculates so that the angle between 0 and 360
            if(TempAngle<0){TempAngle=TempAngle+2*M_PI;}else if(TempAngle>2*M_PI){TempAngle=TempAngle-2*M_PI;};
            
            
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
        
        //std::cout<<Current_angle <<std::endl;

        
        //Add to the all locations
        std::vector<double> mylocationvector;
        mylocationvector.push_back (identifier);
        mylocationvector.push_back (step_number);
        mylocationvector.push_back (TempX);
        mylocationvector.push_back (TempY);
        mylocationvector.push_back (TempAngle);
        mylocationvector.push_back (Current_distance);
        mylocationvector.push_back (0);
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
        //std::cout <<TempDist<<std::endl;
        
        //Based on polar coordinates updates the temp x/y location
        TempX = Current_x + TempDist*sin(TempAngle);
        TempY = Current_y + TempDist*cos(TempAngle);
        
        int tempcounter = 0;
        //std::cout<< "ENTER WHILE" << std::endl;
        while(tempcounter<1){
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
                mylocationvector.push_back (0);
                All_locations.push_back (mylocationvector);
                
                //Ends the while loop by achieving the condition
                tempcounter = 1;
            } //End of the IF loop
            
        // If the movement goes outside of the environment
            else {
                //std::cout<< "Leaves environment"<<std::endl;
            //Have to work out which boundary it leaves first
                
                ///////////////////////////////////////
                // If angle between 0 and 90 degrees //
                ///////////////////////////////////////
                //If angle between 0 and 90 degrees then will exit either at top or right boundary
                if(Current_angle>=0 && Current_angle<M_PI/2){
                    std::cout <<"0-90"<<std::endl;
                    // Caculate the V & H distances to the boudaries
                    double tempDistToTop = Sq_MaxY-Current_y;
                    double tempDistToRight = Sq_MaxX-Current_x;
                    
                    // Calculates the distnace the animal travels to the boudary
                    double tempDistToTopBoundary = std::abs(tempDistToTop/cos(Current_angle));
                    double tempDistToRightBoundary = std::abs(tempDistToRight/sin(Current_angle));
                    
                    // IF the hits the top boundary first
                    if(tempDistToRightBoundary>tempDistToTopBoundary | tempDistToRightBoundary ==0 ){
                        
                        std::cout <<"Top"<<std::endl;
                        //This means that it exits the world at maximum value of y and at the corresponding x value
                        double tempExitsY = Sq_MaxY;
                        double tempExitsX = Current_x+tempDistToTopBoundary*sin(Current_angle);
                        // The current distance travelled
                        Current_distance = Current_distance + tempDistToTopBoundary;

                        
                        //Records the movement 
                        std::vector<double> mylocationvector;
                        mylocationvector.push_back (identifier);
                        mylocationvector.push_back (step_number);
                        mylocationvector.push_back (tempExitsX);
                        mylocationvector.push_back (tempExitsY);
                        mylocationvector.push_back (TempAngle);
                        mylocationvector.push_back (Current_distance);
                        mylocationvector.push_back (0);

                        All_locations.push_back (mylocationvector);
                        
                        // The distance left to travel
                        TempDist = TempDist - tempDistToTopBoundary;
                        
                        //New starting position
                        Current_y = Sq_MinY;
                        Current_x = tempExitsX;
                        
                        //New end locations
                        //Based on polar coordinates updates the temp x/y location
                        TempX = Current_x + TempDist*sin(TempAngle);
                        TempY = Current_y + TempDist*cos(TempAngle);
                        
                        //Records the movement
                        std::vector<double> mylocationvector1;
                        mylocationvector1.push_back (identifier);
                        mylocationvector1.push_back (step_number);
                        mylocationvector1.push_back (Current_x);
                        mylocationvector1.push_back (Current_y);
                        mylocationvector1.push_back (TempAngle);
                        mylocationvector1.push_back (Current_distance);
                        mylocationvector1.push_back (1);
                        All_locations.push_back (mylocationvector1);
                        
                        
                        } //End of IF
                    
                    else if(tempDistToRightBoundary<tempDistToTopBoundary){
                        
                        std::cout << "right "<< std::endl;
                        //This means that it exits the world at maximum value of x and at the corresponding y value
                        double tempExitsX = Sq_MaxX;
                        double tempExitsY = Current_y+tempDistToRightBoundary*cos(Current_angle);
                        
                        // The current distance travelled
                        Current_distance = Current_distance + tempDistToRightBoundary;
                        
                        // The distance left to travel
                        TempDist = TempDist - tempDistToRightBoundary;
                                
                        //Records the movement
                        std::vector<double> mylocationvector;
                        mylocationvector.push_back (identifier);
                        mylocationvector.push_back (step_number);
                        mylocationvector.push_back (tempExitsX);
                        mylocationvector.push_back (tempExitsY);
                        mylocationvector.push_back (TempAngle);
                        mylocationvector.push_back (Current_distance);
                        mylocationvector.push_back (0);
                        All_locations.push_back (mylocationvector);
                        
                        // The distance left to travel
                        TempDist = TempDist - tempDistToRightBoundary;
                        std::cout<<TempDist<<std::endl;
                        
                        //New starting position
                        Current_x = Sq_MinX;
                        Current_y = tempExitsY;
                        
                        //New end locations
                        //Based on polar coordinates updates the temp x/y location
                        TempX = Current_x + TempDist*sin(TempAngle);
                        TempY = Current_y + TempDist*cos(TempAngle);
                        
                        
                        //Records the movement
                        std::vector<double> mylocationvector1;
                        mylocationvector1.push_back (identifier);
                        mylocationvector1.push_back (step_number);
                        mylocationvector1.push_back (Current_x);
                        mylocationvector1.push_back (Current_y);
                        mylocationvector1.push_back (TempAngle);
                        mylocationvector1.push_back (Current_distance);
                        mylocationvector1.push_back (1);
                        All_locations.push_back (mylocationvector1);
                        
                    }; //End of ELSE IF
                } //END OF 0 to 90 degrees IF STATEMENT
                
                
                /////////////////////////////////////////
                // If angle between 90 and 180 degrees //
                /////////////////////////////////////////
                //then will exit either at Bottom or right boundary
                
                else if(Current_angle>=M_PI/2 && Current_angle<M_PI){
                    std::cout <<"90-180"<<std::endl;
                    
                    // Caculate the V & H distances to the boudaries
                    double tempDistToBottom = Current_y - Sq_MinY;
                    double tempDistToRight = Sq_MaxX - Current_x;
                    
                    // Calculates the distnace the animal travels to the boudary
                    double tempDistToBottomBoundary = std::abs(tempDistToBottom/cos(Current_angle));
                    double tempDistToRightBoundary = std::abs(tempDistToRight/sin(Current_angle));
                    // IF the hits the top boundary first
                    
                    if(tempDistToRightBoundary>tempDistToBottomBoundary){
                        //The bat exits the environemnt by the bottom of the world
                        //This means that it exits the world at minimum value of y and at the corresponding x value
                        double tempExitsY = Sq_MinY;
                        double tempExitsX = Current_x + tempDistToBottomBoundary*sin(Current_angle);
                        // The current distance travelled
                        Current_distance = Current_distance + tempDistToBottomBoundary;
                        
                        
                        //Records the movement
                        std::vector<double> mylocationvector;
                        mylocationvector.push_back (identifier);
                        mylocationvector.push_back (step_number);
                        mylocationvector.push_back (tempExitsX);
                        mylocationvector.push_back (tempExitsY);
                        mylocationvector.push_back (TempAngle);
                        mylocationvector.push_back (Current_distance);
                        mylocationvector.push_back (0);
                        All_locations.push_back (mylocationvector);
                        
                        // The distance left to travel
                        TempDist = TempDist - tempDistToBottomBoundary;
                        //std::cout<< "tempDistToBottomBoundary" << tempDistToBottomBoundary<<std::endl;
                        //std::cout<< "Temp dist" << TempDist<<std::endl;
                        
                        
                        //New starting position
                        Current_y = Sq_MaxY;
                        Current_x = tempExitsX;
                        
                        //New end locations
                        //Based on polar coordinates updates the temp x/y location
                        TempX = Current_x + TempDist*sin(TempAngle);
                        TempY = Current_y + TempDist*cos(TempAngle);
                        
                        //Records the movement
                        std::vector<double> mylocationvector1;
                        mylocationvector1.push_back (identifier);
                        mylocationvector1.push_back (step_number);
                        mylocationvector1.push_back (Current_x);
                        mylocationvector1.push_back (Current_y);
                        mylocationvector1.push_back (TempAngle);
                        mylocationvector1.push_back (Current_distance);
                        mylocationvector1.push_back (1);
                        All_locations.push_back (mylocationvector1);
                        
                        
                    } //End of IF
                    
                    else if(tempDistToRightBoundary<tempDistToBottomBoundary){
                        // Hits the Right boundary first
                        //This means that it exits the world at minimum value of x and at the corresponding y value
                        double tempExitsX = Sq_MaxX;
                        double tempExitsY = Current_y +tempDistToRightBoundary*cos(Current_angle);
                        // The current distance travelled
                        Current_distance = Current_distance + tempDistToRightBoundary;
                        // The distance left to travel
                        TempDist = TempDist - tempDistToRightBoundary;
                        
                        //Records the movement
                        std::vector<double> mylocationvector;
                        mylocationvector.push_back (identifier);
                        mylocationvector.push_back (step_number);
                        mylocationvector.push_back (tempExitsX);
                        mylocationvector.push_back (tempExitsY);
                        mylocationvector.push_back (TempAngle);
                        mylocationvector.push_back (Current_distance);
                        mylocationvector.push_back (0);
                        All_locations.push_back (mylocationvector);
                        
                        // The distance left to travel
                        TempDist = TempDist - tempDistToRightBoundary;
                         //std::cout<< "tempDistToRightBoundary" << tempDistToRightBoundary<<std::endl;
                        //std::cout<< "Temp dist" << TempDist<<std::endl;
                        
                        //New starting position
                        Current_x = Sq_MinX;
                        Current_y = tempExitsY;
                        
                        //New end locations
                        //Based on polar coordinates updates the temp x/y location
                        TempX = Current_x + TempDist*sin(TempAngle);
                        TempY = Current_y + TempDist*cos(TempAngle);
                        
                        //Records the movement
                        std::vector<double> mylocationvector1;
                        mylocationvector1.push_back (identifier);
                        mylocationvector1.push_back (step_number);
                        mylocationvector1.push_back (Current_x);
                        mylocationvector1.push_back (Current_y);
                        mylocationvector1.push_back (TempAngle);
                        mylocationvector1.push_back (Current_distance);
                        mylocationvector1.push_back (1);
                        All_locations.push_back (mylocationvector1);
                        
                    }; //End of ELSE IF
                
                
                }//END OF 90 to 180 degrees IF STATEMENT
                
                
                //////////////////////////////////////////
                // If angle between 180 and 270 degrees //
                //////////////////////////////////////////
                // then will exit either at bottom or left boundary
                else if(Current_angle>=M_PI && Current_angle<1.5*M_PI){
                   
                    /*
                    std::cout <<"ID:" <<identifier <<std::endl;
                    std::cout <<"Step Number:" <<step_number<<std::endl;
                    std::cout <<"180-270"<<std::endl;
                    std::cout<< "Start Loop Dist left: "<< TempDist<< std::endl;
                    */
                    
                    // Caculate the V & H distances to the boudaries
                    double tempDistToBottom = Current_y - Sq_MinY;
                    double tempDistToLeft = Current_x - Sq_MinX;
                    
                    // Calculates the distnace the animal travels to the boudary
                    double tempDistToBottomBoundary = std::abs(tempDistToBottom/cos(Current_angle));
                    double tempDistToLeftBoundary = std::abs(tempDistToLeft/sin(Current_angle));
                    // IF the hits the top boundary first
                    
                    if(tempDistToLeftBoundary>tempDistToBottomBoundary){
                        /*
                        std::cout <<"Bottom"<<std::endl;
                         std::cout<< "Start BT Loop Dist left: "<< TempDist<< std::endl;
                        std::cout<< "Dist Left: "<< TempDist<< std::endl;
                        std::cout<< "Dist to Bottom: "<< tempDistToBottomBoundary<< std::endl;
                        std::cout<< "Dist to Left: "<< tempDistToLeftBoundary<< std::endl;
                        */
                        
                        //This means that it exits the world at minimum value of y and at the corresponding x value
                        double tempExitsY = Sq_MinY;
                        double tempExitsX = Current_x +tempDistToBottomBoundary*sin(Current_angle);
                        // The current distance travelled
                        Current_distance = Current_distance + tempDistToBottomBoundary;
                        
                        
                        //Records the movement
                        std::vector<double> mylocationvector;
                        mylocationvector.push_back (identifier);
                        mylocationvector.push_back (step_number);
                        mylocationvector.push_back (tempExitsX);
                        mylocationvector.push_back (tempExitsY);
                        mylocationvector.push_back (TempAngle);
                        mylocationvector.push_back (Current_distance);
                        mylocationvector.push_back (0);
                        All_locations.push_back (mylocationvector);
                          
                        // The distance left to travel
                         TempDist = TempDist - tempDistToBottomBoundary;
  
     
                        //New starting position
                        Current_y = Sq_MaxY;
                        Current_x = tempExitsX;
                        
                        //New end locations
                        //Based on polar coordinates updates the temp x/y location
                        TempX = Current_x + TempDist*sin(TempAngle);
                        TempY = Current_y + TempDist*cos(TempAngle);
 
                        //Records the movement
                        std::vector<double> mylocationvector1;
                        mylocationvector1.push_back (identifier);
                        mylocationvector1.push_back (step_number);
                        mylocationvector1.push_back (Current_x);
                        mylocationvector1.push_back (Current_y);
                        mylocationvector1.push_back (TempAngle);
                        mylocationvector1.push_back (Current_distance);
                        mylocationvector1.push_back (1);
                        All_locations.push_back (mylocationvector1);
                        
                    } //End of IF
                    
                    else if(tempDistToLeftBoundary<tempDistToBottomBoundary){
                        //Hits the left boundary first
                        /*
                        std::cout <<"Left"<<std::endl;
                        std::cout <<"Current x: "<<Current_x<<std::endl;
                        std::cout <<"Current angle: "<<Current_angle<<std::endl;
                        std::cout <<TempDist<<std::endl;
                         */
                        //This means that it exits the world at minimum value of x and at the corresponding y value
                        double tempExitsX = Sq_MinX;
                        double tempExitsY = Current_y + tempDistToLeftBoundary*cos(Current_angle);
                        // The current distance travelled
                        Current_distance = Current_distance + tempDistToLeftBoundary;

                        
                        //Records the movement
                        std::vector<double> mylocationvector;
                        mylocationvector.push_back (identifier);
                        mylocationvector.push_back (step_number);
                        mylocationvector.push_back (tempExitsX);
                        mylocationvector.push_back (tempExitsY);
                        mylocationvector.push_back (TempAngle);
                        mylocationvector.push_back (Current_distance);
                        mylocationvector.push_back (0);
                        All_locations.push_back (mylocationvector);
                        
                        //std::cout<< "Dist Left B4: "<< TempDist<< std::endl;
                        
                        // The distance left to travel
                        TempDist = TempDist - tempDistToLeftBoundary;
                        
                        //std::cout<< "Dist Left Aft: "<< TempDist<< std::endl;

                        //std::cout<< "Dist to Left: "<< tempDistToLeftBoundary<< std::endl;

                        
                        //New starting position
                        Current_x = Sq_MaxX;
                        Current_y = tempExitsY;
                        
                        //New end locations
                        //Based on polar coordinates updates the temp x/y location
                        TempX = Current_x + TempDist*sin(TempAngle);
                        TempY = Current_y + TempDist*cos(TempAngle);
                        
                        //Records the movement
                        std::vector<double> mylocationvector1;
                        mylocationvector1.push_back (identifier);
                        mylocationvector1.push_back (step_number);
                        mylocationvector1.push_back (Current_x);
                        mylocationvector1.push_back (Current_y);
                        mylocationvector1.push_back (TempAngle);
                        mylocationvector1.push_back (Current_distance);
                        mylocationvector1.push_back (1);
                        All_locations.push_back (mylocationvector1);
                        
                    }; //End of ELSE IF
                    
                }//END OF 90 to 180 degrees IF STATEMENT
                
                
                //////////////////////////////////////////
                // If angle between 270 and 360 degrees //
                //////////////////////////////////////////
                
                
                //If angle between 270 and 360 degrees then will exit either at top or left boundary
                else if(Current_angle>=1.5*M_PI && Current_angle<2*M_PI){
                    //std::cout <<"270-360"<<std::endl;
                    // Caculate the V & H distances to the boudaries
                    double tempDistToTop = Sq_MaxY-Current_y;
                    double tempDistToLeft = Current_x-Sq_MinX;
                    
                    // Calculates the distnace the animal travels to the boudary
                    double tempDistToTopBoundary = std::abs(tempDistToTop/cos(Current_angle));
                    double tempDistToLeftBoundary = std::abs(tempDistToLeft/sin(Current_angle));
                    
                    // IF the hits the top boundary first
                    if(tempDistToLeftBoundary>tempDistToTopBoundary){
                        //Hits the Top boundary
                        //This means that it exits the world at maximum value of y and at the corresponding x value
                        double tempExitsY = Sq_MaxY;
                        double tempExitsX = Current_x+tempDistToTopBoundary*sin(Current_angle);
                        // The current distance travelled
                        Current_distance = Current_distance + tempDistToTopBoundary;
                        
                        
                        //Records the movement
                        std::vector<double> mylocationvector;
                        mylocationvector.push_back (identifier);
                        mylocationvector.push_back (step_number);
                        mylocationvector.push_back (tempExitsX);
                        mylocationvector.push_back (tempExitsY);
                        mylocationvector.push_back (TempAngle);
                        mylocationvector.push_back (Current_distance);
                        mylocationvector.push_back (0);
                        All_locations.push_back (mylocationvector);
                        
                        // The distance left to travel
                        TempDist = TempDist - tempDistToTopBoundary;
                        
                        //New starting position
                        Current_y = Sq_MinY;
                        Current_x = tempExitsX;
                        
                        //New end locations
                        //Based on polar coordinates updates the temp x/y location
                        TempX = Current_x + TempDist*sin(TempAngle);
                        TempY = Current_y + TempDist*cos(TempAngle);
                        
                        //Records the movement
                        std::vector<double> mylocationvector1;
                        mylocationvector1.push_back (identifier);
                        mylocationvector1.push_back (step_number);
                        mylocationvector1.push_back (Current_x);
                        mylocationvector1.push_back (Current_y);
                        mylocationvector1.push_back (TempAngle);
                        mylocationvector1.push_back (Current_distance);
                        mylocationvector1.push_back (1);
                        All_locations.push_back (mylocationvector1);
                        
                        
                    } //End of IF
                    
                    else if(tempDistToLeftBoundary<tempDistToTopBoundary){
                        //Hits the left boundary
                        //This means that it exits the world at maximum value of x and at the corresponding y value
                        double tempExitsX = Sq_MinX;
                        double tempExitsY = Current_y+tempDistToLeftBoundary*cos(Current_angle);
                        
                        // The current distance travelled
                        Current_distance = Current_distance + tempDistToLeftBoundary;
                        
                        // The distance left to travel
                        TempDist = TempDist - tempDistToLeftBoundary;
                        
                        //Records the movement
                        std::vector<double> mylocationvector;
                        mylocationvector.push_back (identifier);
                        mylocationvector.push_back (step_number);
                        mylocationvector.push_back (tempExitsX);
                        mylocationvector.push_back (tempExitsY);
                        mylocationvector.push_back (TempAngle);
                        mylocationvector.push_back (Current_distance);
                        mylocationvector.push_back (0);

                        All_locations.push_back (mylocationvector);
                        
                        // The distance left to travel
                        TempDist = TempDist - tempDistToLeftBoundary;
                        
                        //New starting position
                        Current_x = Sq_MaxX;
                        Current_y = tempExitsY;
                        
                        //New end locations
                        //Based on polar coordinates updates the temp x/y location
                        TempX = Current_x + TempDist*sin(TempAngle);
                        TempY = Current_y + TempDist*cos(TempAngle);
                        
                        //Records the movement
                        std::vector<double> mylocationvector1;
                        mylocationvector1.push_back (identifier);
                        mylocationvector1.push_back (step_number);
                        mylocationvector1.push_back (Current_x);
                        mylocationvector1.push_back (Current_y);
                        mylocationvector1.push_back (TempAngle);
                        mylocationvector1.push_back (Current_distance);
                        mylocationvector1.push_back (1);
                        All_locations.push_back (mylocationvector1);
                        
                    }; //End of ELSE IF

                
                
                }
                else {std::cout << "ERROR" <<std::endl;};
                 if(TempDist<=0) {
                     std::cout << "ERROR" <<std::endl;
                     tempcounter=1;};
            }; // End of Else loop;
            //std::cout << "END" <<std::endl;
           
        }; //End of While loop

    }; // End of Home_state=0
   
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