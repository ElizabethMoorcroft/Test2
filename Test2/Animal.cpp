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
#include <time.h>
#include "Animal.h"
#include "RandNum.h"
#include "Parameters.h"




// Constructor
Animal::Animal( ){};

Animal::Animal( int a, int b,  
               double i, double j, double k,
                double t, double u
               ) {
    
    //std::cout <<"Inside Animals"<< std::endl;
    
    identifier = a;
    HomeRange_id = b;
    Current_x = i;
    Current_y = j;
    Current_angle = k;
    if(Current_angle>=2*M_PI){Current_angle=Current_angle-2*M_PI;};
    
     
    
    //When initialising the step number and the current distance travelled
    // will always be set to zero
    step_number = 0;
    Current_distance = 0;
    
    
    // Variables to do with calls
    //Set up like this so in the future can be initalized as RV??
    //Call_amp = Amp;
    //Call_freq = Freq;
    Call_width = Call_halfwidth;
    
    // Variables to do with HR
    SolidHomeRangeBoundary = HR_SolidBoundaries  ;  // HR = 1=Y or 0=N
    Home_x = t;
    Home_y = u;
    Home_r = HR_AverageRadius ;
    
    //Movement variables
    Movement_type = MovementType;
    Move_NonCorr = 0  ;// Always starts in a correlated walk (only really of consequence for Movement type 2)
    //Set up like this so in the future can be initalized as RV??
    Move_speed =AnimalSpeed        ; // Move_speed = f;
    Move_maxangle=CorrWalkMaxAngleChange;//Move_maxangle = g;
    
    //std::cout <<"B4 vectors"<< std::endl;

    EndStep_locations.resize(NoSteps+5);
    All_locations.resize(2*NoSteps+5);
    mylocationvector.resize(8);
    
    // Updates the location vector
    locationvectorcount =0;
    LocationVector(Current_x,Current_y,0,1);
    
    //temp
    timeLE1=0;
    timeLE2=0;
    timeLE3=0;

}


////////////////////////////
/// Weird function things///
////////////////////////////

void Animal::Set_MoveValue (double a, double b) {
    Move_speed = a;
    Move_maxangle = b;
}

void Animal::Set_HRValues (int a, int b, double c, double d, double e) {
    SolidHomeRangeBoundary = a;
    Home_x = c;
    Home_y = d;
    Home_r = e;
}

void Animal::LocationVector(double locationx, double locationy, int LeaveEntreCode, int End){
    //Records exit of the world
    mylocationvector[0] = identifier;
    mylocationvector[1] = step_number;
    mylocationvector[2] = locationx;
    mylocationvector[3] = locationy;
    mylocationvector[4] = Current_angle;
    mylocationvector[5] = Current_distance;
    mylocationvector[6] = Move_speed;
    mylocationvector[7] = LeaveEntreCode;
    All_locations[locationvectorcount]= mylocationvector;
    if(End==1){EndStep_locations[step_number] = mylocationvector;} // Only happens at the end of the step
    locationvectorcount+=1;
}

/////////////////////
/// NEW LOCATION  ///
/////////////////////

// Calculations for new x&y locations
// this uses simple trig and is NOT dependent on the direction of travel
double Animal::CalNext_X(double distance){ double newx = Current_x + distance*sin(NextAngle); return newx;};
double Animal::CalNext_Y(double distance){ double newy = Current_y + distance*cos(NextAngle); return newy;};


// If the animal is moving in  a straight line
//      => Movement Type = 0
void Animal::NewLocationMT0 (double seed){
    
    //set up a random number
    RandNum Number1;

    // Calculate a distance value
    NextDist = Number1.PositiveNormal(seed,StepLengthDist,StepLengthDist/10);
    //std::cout <<TempDist<<std::endl;
    NextAngle = Current_angle;
    
    //Based on polar coordinates updates the temp x/y location
    NextX = CalNext_X(NextDist);
    NextY = CalNext_Y(NextDist);
};


void Animal::NewLocationCorr (double seed, double seed2){
    
    //set up a random number
    RandNum Number1;
    //Correlated random walk
    // Calculate a distance value
    NextDist = Number1.PositiveNormal (seed,StepLengthDist,StepLengthDist/10);

    //calculates a new random angle of movement
    NextAngle = Number1.AtoRangeBUnif(seed2,Current_angle,Move_maxangle);
    //Sometimes the angle becomes greater than 360 or less than 0
    //Recalculates so that the angle between 0 and 360
    if(NextAngle<0){NextAngle+=2*M_PI;}else if(NextAngle>=2*M_PI){NextAngle-=2*M_PI;};

    //Based on polar coordinates updates the temp x/y location
    NextX = CalNext_X(NextDist);
    NextY = CalNext_Y(NextDist);
    //Current_angle = NextAngle;
}

// Uncorrelated movement - Used in type 2 movement (a mixture of correlated and uncorrelated movement).
void Animal::NewLocationUnCorr (double seed, double seed2){
            
    //set up a random number
    RandNum Number1;
    //Correlated random walk
    // Calculate a distance value
    NextDist = Number1.PositiveNormal (seed,StepLengthDist,StepLengthDist/10);
        
    //calculates a new random angle of movement
    NextAngle = Number1.AtoBUnif(seed2,0,2*M_PI);

    //Updates the x and y coordinates
    NextX = CalNext_X(NextDist);
    NextY = CalNext_Y(NextDist);
};

// Movement for when there is a two stage correlated walk
//      => Mocement Type = 2
void Animal::NewLocationMT2 (double seed, double seed2){
    
    if(Move_NonCorr==1){//ENTRE CODE IF THE MOVEMENT IS NOT_CORRELATED
        NewLocationUnCorr(seed,seed2);// END IF
    } // end of if
    else if(Move_NonCorr==0){ //If MOVEMENT IS CORRELATED THEN CALUCLATE THE MOVEMENT FROM THE MOVETYPE1 CORR-RANDOM WALK
        NewLocationCorr(seed,seed2);
    } // END ELSE IF
};


void Animal::NewLocation (double seed, double seed2){
    

    clock_t LE3 =clock();
    
    //calculates a new random step distance
    if(Movement_type==0){
        NewLocationMT0(seed);
    }
    else if (Movement_type==1){
        NewLocationCorr(seed,seed2);
    }
    else if (Movement_type==2){
        NewLocationMT2(seed,seed2);
    };
    
    LE3 =clock()-LE3;
    timeLE3 +=LE3;
};


//////////////////////////
/// PERIODIC BOUNDARY  ///
//////////////////////////


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
        Current_angle = NextAngle;
     
        
        //New starting position
        Current_y = YBoundEnter;
        Current_x = tempExitsX;
        
        // The distance left to travel
        NextDist -= tempDistToTopBoundary;
        // The current distance travelled
        Current_distance += tempDistToTopBoundary;
        
    } else{
        //std::cout <<"Side"<<std::endl;
        //This means that it exits the world at maximum value of y and at the corresponding x value
        tempExitsX = XBoundExit;
        tempExitsY = CalNext_Y(tempDistToSideBoundary);
        
        //New starting position
        Current_y = tempExitsY;
        Current_x = XBoundEnter;
        Current_angle = NextAngle;
        
        // The distance left to travel
        NextDist -=tempDistToSideBoundary;
        // The current distance travelled
        Current_distance += tempDistToSideBoundary;
        //std::cout <<"leaves Side"<<std::endl;
    }
        //Updates the location vector
        LocationVector(tempExitsX,tempExitsY,0,0);
    
        //std::cout <<"vect1 done"<<std::endl;
        //New end locations
        //Based on polar coordinates updates the temp x/y location
        NextX = CalNext_X(NextDist);
        NextY = CalNext_Y(NextDist);
        //std::cout<<All_locations.size()<<std::endl;
        //std::cout<<locationvectorcount<<std::endl;
        //std::cout <<"vect1 done1"<<std::endl;
    
        // Updates the location vector
        LocationVector(Current_x,Current_y,1,0);

    
}



///////////////////////
/// UPDATE LOCATION ///
///////////////////////

void Animal::UpdateLocation (double seed){ // a is the number of seconds per step, b is the random seed

    
    //std::cout <<"Inside Update locations"<< std::endl;
    //List of random number
    
    srand(seed);
    std::vector<double> RandomNumberMovement;
    RandomNumberMovement.resize(101);
    for(int i=0; i<101; i++){ //Do I need to increase the max value??
        RandomNumberMovement[i] = double(rand());
    };
    
    
    //List of random number
    srand(RandomNumberMovement[0]);
    std::vector<double> RandomNumberUpdateMovement;
    RandomNumberUpdateMovement.resize(1000);
    for(int i=0; i<1000; i++){ //Do I need to increase the max value??
        RandomNumberUpdateMovement[i] = double(rand());
    };
    

    
    ////////////////////////////
    /// HR bounding movement ///
    ////////////////////////////
    if(SolidHomeRangeBoundary==1){

        int count=0;
        double TempDistToHR =Home_r+1;

        while(TempDistToHR >Home_r){
            if(count<200){
                NewLocation(RandomNumberUpdateMovement[count], RandomNumberUpdateMovement[count+100]);
            
                // Distance from centre of hr to animal
                //double
                TempDistToHR =sqrt(pow(NextX-Home_x,2)+pow(NextY-Home_y,2));
            
                count +=1;
            }
            else {
                NewLocationUnCorr(RandomNumberUpdateMovement[count], RandomNumberUpdateMovement[count+100]);
                
                //double
                TempDistToHR =sqrt(pow(NextX-Home_x,2)+pow(NextY-Home_y,2));
                
                count +=1;
                
            } //END OF ELSE
        } //End of While loop (i loop)
        
        // rewrite current location
        Current_x = NextX;
        Current_y = NextY;
        Current_angle = NextAngle;
        Current_distance = Current_distance + NextDist;
        
        //std::cout<<Current_angle <<std::endl;
        
        //Add to the all locations
          LocationVector(Current_x,Current_y,0,1);

         
    } //END OF SOLID BOUNDARIES
    
    
    ////////////////////////////////
    /// HR NOT bounding movement ///
    ////////////////////////////////
    if(SolidHomeRangeBoundary==0){
        
        //std::cout<<"NoSolid"<<std::endl;
        
        NextAngle = Current_angle;
            NewLocation(RandomNumberUpdateMovement[0], RandomNumberUpdateMovement[100]);
        
        int tempcounter = 0;
        //std::cout<< "ENTER WHILE" << std::endl;
        while(tempcounter<1){
            
            //std::cout<< "ENTERED WHILE" << std::endl;
            // If the movement finishes inside the environment
            if(NextX < Sq_MaxX && NextX > Sq_MinX && NextY < Sq_MaxY && NextY > Sq_MinY){
                
                //std::cout<< "If entered" << std::endl;
                
                clock_t LE2 =clock();
                //Can save the temp location as the current location
                // rewrite current locaion
                Current_x = NextX;
                Current_y = NextY;
                Current_distance += NextDist;
                
                //Add to the all locations
                LocationVector(Current_x,Current_y,0,1);
                //std::cout<< "Vectors done" << std::endl;
                
                //Ends the while loop by achieving the condition
                tempcounter = 1;
                
                LE2 =clock()-LE2;
                timeLE2 +=LE2;
                 //std::cout<<"ENDNoSolid"<<std::endl;
            } //End of the IF loop
            
        // If the movement goes outside of the environment
            else {
                clock_t LE1 =clock();
                //std::cout<< "Leaves environment"<<std::endl;
            //Have to work out which boundary it leaves first
                
                ///////////////////////////////////////
                // If angle between 0 and 90 degrees //
                ///////////////////////////////////////
                //If angle between 0 and 90 degrees then will exit either at top or right boundary
                if(NextAngle>=0 && NextAngle<M_PI/2){
                    //std::cout <<"0-90"<<std::endl;
                    LeaveEnterWorld(Sq_MaxY, Sq_MaxX ,Sq_MinY, Sq_MinX);
                } //END OF 0 to 90 degrees IF STATEMENT
                
                
                /////////////////////////////////////////
                // If angle between 90 and 180 degrees //
                /////////////////////////////////////////
                //then will exit either at Bottom or right boundary

                else if(NextAngle>=M_PI/2 && NextAngle<M_PI){
                    //std::cout <<"90-180"<<std::endl;
                    LeaveEnterWorld(Sq_MinY, Sq_MaxX ,Sq_MaxY, Sq_MinX);
                    
                }//END OF 90 to 180 degrees IF STATEMENT

                /////////////////////////////////////////
                // If angle between 180 and 270 degrees //
                /////////////////////////////////////////
                //then will exit either at Bottom or right boundary
                
                else if(NextAngle>=M_PI && NextAngle<1.5*M_PI){
                    //std::cout <<"180-270"<<std::endl;
                    LeaveEnterWorld(Sq_MinY, Sq_MinX ,Sq_MaxY, Sq_MaxX);
                    
                }//END OF 180 to 270 degrees IF STATEMENT
                
                /////////////////////////////////////////
                // If angle between 270 and 360 degrees //
                /////////////////////////////////////////
                //then will exit either at Bottom or right boundary
                
                else if(NextAngle>=1.5*M_PI && NextAngle<2*M_PI){
                    //std::cout <<"270-360"<<std::endl;
                    LeaveEnterWorld(Sq_MaxY, Sq_MinX ,Sq_MinY, Sq_MaxX);
                
                }//END OF 270 to 360 degrees IF STATEMENT
                
                
                //////////////////////////
                // Two possible errors  //
                //////////////////////////
                // Produces error is the ANGLE is not between 0 and 360
                else {std::cout << "ERROR - ANGLE: "<<NextAngle <<std::endl;};
                
                // Produces error if the Next distance is less than zero
                // But exits loop and continues with the rest of the code
                 if(NextDist<0) {
                    std::cout<< "ERROR: Next dist" << NextDist<<std::endl;
                    tempcounter=1;};
                
                LE1 =clock()-LE1;
                timeLE1 +=LE1;
                
            }; // End of Else loop;
           
        }; //End of While loop
    

        
    
    }; // End of SolidHomeRangeBoundary=0
   
    
    //Increases step number by one once the animal finishes moving in the environment
    step_number =step_number+1;
    // IF the movement is a two state movement model then 
    if(Movement_type==2){
        //Movement type 2 switches between correlated and uncorrelated movement
        srand(RandomNumberMovement[100]);
        double ChangeMoveState = double(rand())/double(RAND_MAX);
        if(ChangeMoveState < ProbChangeMoveState){
            if(Move_NonCorr==1){Move_NonCorr=0;} else{Move_NonCorr=1;}
            //Changes the movement from non-correlated to correlated for the next step
            // OR Changes the movement from correlated to non-correlated for the next step
        }
    }
//tupdate= clock() - tupdate;
  //  std::cout<< float(tupdate)/CLOCKS_PER_SEC<<std::endl;
}//End of update