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
    
    Move_NonCorr = e;
    Move_speed = f;
    Move_maxangle = g;
    
    Current_x = i;
    Current_y = j;
    Current_angle = k;
    Current_distance = l;
    
    Call_amp = o;
    Call_freq = p;
    Call_width = q;
    
    
    SolidHomeRangeBoundary = r ;  // HR = 1=Y or 0=N
    Home_x = t;
    Home_y = u;
    Home_r = v;
    
    
    //Records the movement
    std::vector<double> mylocationvector;
    mylocationvector.resize(7);
    mylocationvector[0] = identifier;
    mylocationvector[1] = step_number;
    mylocationvector[2] = Current_x;
    mylocationvector[3] = Current_y;
    mylocationvector[4] = Current_angle;
    mylocationvector[5] = Current_distance;
    mylocationvector[6] = 0;
    All_locations.push_back (mylocationvector);
    EndStep_locations.push_back (mylocationvector);
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


/////////////////////
/// NEW LOCATION  ///
/////////////////////

void Animal::NewLocationMT0 (double a, double b){
    
    double seed=a;
    double Dist=b;
    //set up a random number
    RandNum Number1;
    
    // Gas movmeent there is no change in the angle of movemnet
    NextAngle=Current_angle;

    // Calculate a distance value
    NextDist = Number1.PositiveNormal (seed,Dist,Dist/10);
    //std::cout <<TempDist<<std::endl;

    //Based on polar coordinates updates the temp x/y location
    NextX = Current_x + NextDist*sin(NextAngle);
    NextY = Current_y + NextDist*cos(NextAngle);
}

void Animal::NewLocationMT1 (double a, double b, double c){
    
    double seed=a;
    double seed2=b;
    double Dist=c;
    //set up a random number
    RandNum Number1;
    //Correlated random walk
    // Calculate a distance value
    NextDist = Number1.PositiveNormal (seed,Dist,Dist/10);

    //calculates a new random angle of movement
    NextAngle = Number1.AtoRangeBUnif(seed2,Current_angle,Move_maxangle);
    //Sometimes the angle becomes greater than 360 or less than 0
    //Recalculates so that the angle between 0 and 360
    if(NextAngle<0){NextAngle=NextAngle+2*M_PI;}else if(NextAngle>2*M_PI){NextAngle=NextAngle-2*M_PI;};

    //Based on polar coordinates updates the temp x/y location
    NextX = Current_x + NextDist*sin(NextAngle);
    NextY = Current_y + NextDist*cos(NextAngle);
    //Current_angle = NextAngle;
}

void Animal::NewLocationUnCorr (double a, double b, double c){
    
    double seed=a;
    double seed2=b;
    double Dist=c;
    

        
    //set up a random number
    RandNum Number1;
    //Correlated random walk
    // Calculate a distance value
    NextDist = Number1.PositiveNormal (seed,Dist,Dist/10);
        
    //calculates a new random angle of movement
    NextAngle = Number1.AtoBUnif(seed2,0,2*M_PI);
        
        
    //Based on polar coordinates updates the temp x/y location
    NextX = Current_x + NextDist*sin(NextAngle);
    NextY = Current_y + NextDist*cos(NextAngle);

    
    
}

void Animal::NewLocationMT2 (double a, double b, double c){
    
    double seed=a;
    double seed2=b;
    double Dist=c;
    
    if(Move_NonCorr==1){
        //ENTRE CODE IF THE MOVEMENT IS NOT_CORRELATED
        NewLocationUnCorr(seed,seed2,Dist);
        
        }
    else if(Move_NonCorr==0){
        //If MOVEMENT IS CORRELATED THEN CALUCLATE THE MOVEMENT FROM THE MOVETYPE1 CORR-RANDOM WALK
        NewLocationMT1(seed,seed2,Dist);
    }
    
 
}


void Animal::NewLocation (double a, double b, double c){
    
    //Renames variables
    double StepLength = a;
    double seed = b;
    double seed2 = c;
    //std::cout<< seed <<std::endl;

    
    //calculates a new random step distance
    double Dist = Move_speed*StepLength;
    
    if(Movement_type==0){
        
        NewLocationMT0(seed,Dist);
        
    } else if (Movement_type==1){
        NewLocationMT1(seed,seed2,Dist);
        
    } else if (Movement_type==2){
        NewLocationMT2(seed,seed2,Dist);
        
    }
    
}


//////////////////////////
/// PERIODIC BOUNDARY  ///
//////////////////////////


void Animal::LeaveEnterWorld(double a, double b , double c, double d){
    
    double YBoundExit =a;
    double XBoundExit =b;
    double YBoundEnter =c;
    double XBoundEnter =d;
    

    // Caculate the V & H distances to the boudaries
    double tempDistToTop = std::abs(YBoundExit-Current_y);
    double tempDistToSide = std::abs(XBoundExit-Current_x);
    
    // Calculates the distnace the animal travels to the boudary
    double tempDistToTopBoundary = std::abs(tempDistToTop/cos(NextAngle));
    double tempDistToSideBoundary = std::abs(tempDistToSide/sin(NextAngle));
    
    double tempExitsY =0;
    double tempExitsX =0;
    
    if(tempDistToSideBoundary>tempDistToTopBoundary | tempDistToSideBoundary ==0 ){
    
        std::cout <<"Top"<<std::endl;
        //This means that it exits the world at maximum value of y and at the corresponding x value
        tempExitsY = YBoundExit;
        tempExitsX = Current_x+tempDistToTopBoundary*sin(NextAngle);
        
        //New starting position
        Current_y = YBoundEnter;
        Current_x = tempExitsX;
        
        // The distance left to travel
        NextDist = NextDist - tempDistToTopBoundary;
    } else{
        std::cout <<"Side"<<std::endl;
        //This means that it exits the world at maximum value of y and at the corresponding x value
        tempExitsX = XBoundExit;
        tempExitsY = Current_y+tempDistToSideBoundary*cos(NextAngle);
        
        //New starting position
        Current_y = tempExitsY;
        Current_x = XBoundEnter;
        Current_angle = NextAngle;
        
        // The distance left to travel
        NextDist = NextDist - tempDistToSideBoundary;
    
    }

    
    
        // The current distance travelled
        Current_distance = Current_distance + tempDistToTopBoundary;
    
    
        //Records exit of the world
        std::vector<double> mylocationvector;
        mylocationvector.resize(7);
        mylocationvector[0] = identifier;
        mylocationvector[1] = step_number;
        mylocationvector[2] = tempExitsX;
        mylocationvector[3] = tempExitsY;
        mylocationvector[4] = NextAngle;
        mylocationvector[5] = Current_distance;
        mylocationvector[6] = 0;
        All_locations.push_back (mylocationvector);
    
    
        //New end locations
        //Based on polar coordinates updates the temp x/y location
        NextX = Current_x + NextDist*sin(NextAngle);
        NextY = Current_y + NextDist*cos(NextAngle);
    
        //Records the etring of the world
        std::vector<double> mylocationvector1;
        mylocationvector1.resize(7);
        mylocationvector1[0] = identifier;
        mylocationvector1[1] = step_number;
        mylocationvector1[2] = Current_x;
        mylocationvector1[3] = Current_y;
        mylocationvector1[4] = NextAngle;
        mylocationvector1[5] = Current_distance;
        mylocationvector1[6] = 1;
        All_locations.push_back (mylocationvector1);
    
}



///////////////////////
/// UPDATE LOCATION ///
///////////////////////

void Animal::UpdateLocation (double a, double b){ // a is the number of seconds per step, b is the random seed
    
    double StepLength = a;
    double seed = b;
    
    //List of random number
    srand(seed);
    std::vector<double> RandomNumberMovement;
    for(int i=0; i<101; i++){ //Do I need to increase the max value??
        double myrandomnumber =  rand();
        RandomNumberMovement.push_back(myrandomnumber);
    };
    
    
    //List of random number
    srand(RandomNumberMovement[0]);
    std::vector<double> RandomNumberUpdateMovement;
    for(int i=0; i<100000; i++){ //Do I need to increase the max value??
        double myrandomnumber =  rand();
        RandomNumberUpdateMovement.push_back(myrandomnumber);
    };
    

    
    ////////////////////////////
    /// HR bounding movement ///
    ////////////////////////////
    if(SolidHomeRangeBoundary==1){
        
        int i=0;
        int count=0;

        std::cout<<"Solid"<<std::endl;
        while(i==0){
            if(count<1000){
                std::cout<<"Corr" << count<<std::endl;
                NewLocation(StepLength, RandomNumberUpdateMovement[count], RandomNumberUpdateMovement[count+100]);
            
                // Distance from centre of hr to animal
                double TempDistToHR =sqrt(pow(NextX-Home_x,2)+pow(NextY-Home_y,2));
            
                count = count +1;
            
                //While loop wasn't working
                //if new location is inside the home range then let
                if(TempDistToHR<Home_r){ i=i+1;}
            }
            else {
                std::cout<<"UnCorr"<< count<<std::endl;
                NewLocationUnCorr(RandomNumberUpdateMovement[count], RandomNumberUpdateMovement[count+100], Move_speed*StepLength);
                
                double TempDistToHR =sqrt(pow(NextX-Home_x,2)+pow(NextY-Home_y,2));
                
                count = count +1;
                
                //While loop wasn't working
                //if new location is inside the home range then let
                if(TempDistToHR<Home_r){ i=i+1;}
            }
        } //End of While loop
        
        // rewrite current locaion
        Current_x = NextX;
        Current_y = NextY;
        Current_angle = NextAngle;
        Current_distance = Current_distance + NextDist;
        
        //std::cout<<Current_angle <<std::endl;

        
        //Add to the all locations
        std::vector<double> mylocationvector;
        mylocationvector.resize(7);
        mylocationvector[0] =identifier;
        mylocationvector[1] =step_number;
        mylocationvector[2] =Current_x;
        mylocationvector[3] =Current_y;
        mylocationvector[4] =NextAngle;
        mylocationvector[5] =Current_distance;
        mylocationvector[6] =0;
        All_locations.push_back (mylocationvector);
        EndStep_locations.push_back (mylocationvector);
    } //END OF SOLID BOUNDARIES
    
    
    ////////////////////////////////
    /// HR NOT bounding movement ///
    ////////////////////////////////
    if(SolidHomeRangeBoundary==0){
        
        //std::cout<<"NoSolid"<<std::endl;
        
            NewLocation(StepLength, RandomNumberUpdateMovement[0], RandomNumberUpdateMovement[100]);
        
        int tempcounter = 0;
        //std::cout<< "ENTER WHILE" << std::endl;
        while(tempcounter<1){
            // If the movement finishes inside the environment
            if(NextX < Sq_MaxX && NextX > Sq_MinX && NextY < Sq_MaxY && NextY > Sq_MinY){
                //Can save the temp location as the current location
                // rewrite current locaion
                Current_x = NextX;
                Current_y = NextY;
                Current_angle = NextAngle;
                Current_distance = Current_distance + NextDist;
                
                //Add to the all locations
                std::vector<double> mylocationvector;
                mylocationvector.resize(7);
                mylocationvector[0] = identifier;
                mylocationvector[1] = step_number;
                mylocationvector[2] = NextX;
                mylocationvector[3] = NextY;
                mylocationvector[4] = NextAngle;
                mylocationvector[5] = Current_distance;
                mylocationvector[6] = 0;
                All_locations.push_back (mylocationvector);
                EndStep_locations.push_back (mylocationvector);
                
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
                else {std::cout << "ERROR - ANGLE" <<std::endl;};
                
                // Produces error if the Next distance is less than zero
                // But exits loop and continues with the rest of the code
                 if(NextDist<0) {
                    std::cout<< "ERROR: Next dist" << NextDist<<std::endl;
                    tempcounter=1;};
            }; // End of Else loop;
            //std::cout << "END" <<std::endl;
           
        }; //End of While loop

    }; // End of SolidHomeRangeBoundary=0
   
    //Increases step number by one once the animal finishes moving in the environment
    step_number =step_number+1;
    
    if(Movement_type==2){
        //Movement type 2 switches between correlated and uncorrelated movement
    
        if(Move_NonCorr==1){ 
            //Changes the movement from non-correlated to correlated for the next step
            srand(RandomNumberMovement[100]);
            double ChangeMoveState = double(rand())/double(RAND_MAX);
            if(ChangeMoveState < ProbChangeMoveState){
                Move_NonCorr=0;}
        } else { 
            //Changes the movement from correlated to non-correlated for the next step
            srand(RandomNumberMovement[100]);
            double ChangeMoveState = double(rand())/double(RAND_MAX);
            if(ChangeMoveState < ProbChangeMoveState){
                Move_NonCorr=1;}
        }
    }
}//End of update