//
//  Sensors.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 19/04/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include "Sensors.h"
#include "Parameters.h"
#include "RandNum.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <algorithm> 

Sensor::Sensor(){};
Sensor::Sensor(int Id, double sensorhalfangle, double sensorradius){
    
    /*----------------------------------------------
    // Location of Sensor
    ------------------------------------------------*/
    //Assigning varaibles
    Sensor_identifier = Id;
    radius = sensorradius;
    angle_HalfWidth = sensorhalfangle;

    location_x = (Sq_MaxX- Sq_MinX)/2 + Sq_MinX;
    location_y = (Sq_MaxY- Sq_MinY)/2 + Sq_MinY;
        
    //angle_HalfWidth = SensorWidth;
    angle = 0;// If the turn may wish to change Sensor angles to random directions
    //angle = RangeAngle(angle); // For use  when angle changes
    Sensor_StepOn = 0; // The Sensors are on at all steps!!
    
    // Detector 1 is line represneting the left hand slide of the detector range
    // If the lines of the detcetor were lines of graphs with garident and intercepts, Y=mX+c, then:
    //  - gradient if theta is the ange of the line then, m=1/tan(theta)
    //  - intercpet would be: y-mx=c (where y and x are known)
    g_detector1 = angle-angle_HalfWidth;
    g_detector2 = angle+angle_HalfWidth;

    g_detector1= RangeAngle(g_detector1);
    g_detector2= RangeAngle(g_detector2);

    
    m_detector1 = GradientFromAngle(g_detector1);
    m_detector2 = GradientFromAngle(g_detector2);
    c_detector1  = location_y - location_x*m_detector1;
    c_detector2  = location_y - location_x*m_detector2;
    
    int vh_det1 =0;
    int vh_det2 =0;
    
    if(g_detector1== 0 || g_detector1== M_PI|| g_detector1== 2*M_PI){vh_det1 = 1;} //Line is vertical
    else if(g_detector1 == M_PI/2 || g_detector1== 3*M_PI/2){vh_det1 = 2;} //Line is Horizontal
    
    if(g_detector2== 0 || g_detector2== M_PI|| g_detector2== 2*M_PI){vh_det2 = 1;} //Line is vertical
    else if(g_detector2 == M_PI/2 || g_detector2== 3*M_PI/2){vh_det2 = 2;} //Line is Horizontal
    
    Captures.resize(round(DensityAnimals*((Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY)))*3);
    capturecount=0;
    myvector.resize(11);
};


/*--------------------------------------------------------------------------------------------------------
//
//                                                SETS                                                    
//
--------------------------------------------------------------------------------------------------------*/


void Sensor::Add1StepOn(){Sensor_StepOn=Sensor_StepOn+1;};

void Sensor::ResetStepOn(){Sensor_StepOn=0;};

void Sensor::resetCaptures(){
    capturecount =0;
    Captures.clear();
    Captures.resize(round(DensityAnimals*((Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY)))*3);
};

/*--------------------------------------------------------------------------------------------------------
//
//                                        Mutiple use functions
//
--------------------------------------------------------------------------------------------------------*/

// Needs test
/*----------------------------------------------
 // Some times they values are are the same to 4/5dp
 // But the code doesn't recognise thus
 // This function produces a 1 if the difference is 
 // less than 0.00001 and 0 otherwise.
 // Therefore if two values are approximately equal 
 // then the 1 otherwise 0.
----------------------------------------------*/

bool Sensor::approximatelyequal(double a, double b){
    long double value = sqrt(pow(a-b,2));
    long double epsilon = (1*pow(10,-4));
    return (value<epsilon);
};

// Needs test
/*----------------------------------------------
 // RangeAngle corrects the angle so that the
 // angle is between 0 and 2pi
 ----------------------------------------------*/
double Sensor::RangeAngle(double angle){
    if(angle<0){angle += 2*M_PI;} else if(angle>=2*M_PI){angle -= 2*M_PI;};
    return angle;
};

// Needs test
/*----------------------------------------------
 // Calculates the distance between two points
 //
 ----------------------------------------------*/
double Sensor::DistTwoPoints(double X1, double X2, double Y1, double Y2){
    double distance = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));
    return distance;
};

// Needs test
/*----------------------------------------------
 // Calculates the angle between two points
 // FROM X1 to X2
 ----------------------------------------------*/
double Sensor::AngleTwoPoints(double X1, double X2, double Y1, double Y2){
    // The angle from X1,Y1 to X2,Y2 is atan(d(y)/d(x))
    // Don't want always want theta, I want the baring from north
    double diffx = (X2-X1);
    double diffy = (Y2-Y1);
    // Gives the angle from north
    double theta = atan(diffx/diffy);// - M_PI_2;
    if(diffx<0){theta+= M_PI; if(diffy>0){theta+= M_PI;};}
    //if(diffy<0){theta-= M_PI/2;}
    //Gets rid of negative values
    theta = RangeAngle(theta);
    return theta;
};


// Needs test
/*----------------------------------------------
 // Calculates the gradient of line
 // using the baring from north
 ----------------------------------------------*/
double Sensor::GradientFromAngle(double angle){
    // The Gradient is calculated as = delta(y)/delta(x)
    //  tan(angle) = Opposite/Adjacent = delta (x)/delta(y)
    // therefore gradient = 1/tan(angle)
    double Gradient = 1/tan(angle);
    return(Gradient);
};


// NEEDS TEST
/*-------------------------------------------------
 // Updates captures vectors with new captures
-------------------------------------------------*/
void Sensor::UpdateCaptures(double Individual_ID,double itnumber,double location_x_animal,double location_y_animal,double time, int call, double CamToAnimal, double AnimalToCam , double DistToCam){
    
    /*---------------------------------------------------------------------------------------------------------
     // Check for specific case of interest
     if(Individual_ID== 471 && Sensor_StepOn>212 && Sensor_StepOn<214){
    std::cout<<"Caught!"<<std::endl; //<<Sensor_StepOn << " time: "<< time <<std::endl;
     };
     //----------------------------------------------------------------------------------------------------------*/
    
    myvector[0] = Individual_ID;
    myvector[1] = Sensor_StepOn;
    myvector[2] = Sensor_identifier;
    myvector[3] = itnumber;
    myvector[4] = location_x_animal;
    myvector[5] = location_y_animal;
    myvector[6] = time;
    myvector[7] = call;
    myvector[8] = CamToAnimal;
    myvector[9] = AnimalToCam;
    myvector[10] = DistToCam;
    Captures[capturecount]=myvector;
    capturecount+=1;
};


///////////////////////
///////////////////////

/*----------------------------------------------
 // Intersection between two straight line:
 //     - Vertical line and line at an angle
 //     - Horizontal line and 
 //     - Two lines at an angle
 //     - Vertical line and Circle
 //     - Horizontal line and Circle
 //     - Line at an angle and Circle
 // Tests for all of these are at the bottom 
 // of this script
 //
 // Calculation of the "time" and the angle between 
 // the start location and the intersection of 
 // the two lines.
 // Test at end of script
 ----------------------------------------------*/

double Sensor::VertAndAngleInteraction(double Vert, double m_Angle, double c_Angle){
    //y=mx+c
    std::vector <double> Coord(2);
    double YCoordinate = m_Angle*Vert +c_Angle;
    return(YCoordinate);
};

double Sensor::HorzAndAngleInteraction(double Horz, double m_Angle, double c_Angle){
    // y=mx+c
    // => x = (y-c) /m
    double XCoordinate = (Horz - c_Angle)/m_Angle;
    return(XCoordinate);
};

std::vector <double> Sensor::AngleAndAngleInteraction(double m1_Angle, double c1_Angle, double m2_Angle, double c2_Angle){
    // y=mx+c =>
    // m1*x +c1 = m2*x+c
    // (m1-m2)x = (c2-c1)
    // x = (c2-c1)/(m1-m2)
    std::vector <double> Coord(2);
    double Coordinate1 = (c2_Angle - c1_Angle)/(m1_Angle-m2_Angle);
    double Coordinate2 = m1_Angle*Coordinate1 +c1_Angle;
    Coord[0] = Coordinate1;
    Coord[1] = Coordinate2;
    return(Coord);
};


std::vector <double> Sensor::HorzAndCircInteraction(double Horz, double y_centre, double x_centre, double radius){
    // (y-a)^2+ (x-b)^2 = r^2
    // =>
    //  (x-b)^2 = r^2 - (y-a)^2
    // =>
    // x = (+/-)sqrt (r^2 - (y-a)^2) +b
    std::vector <double> Coord(4);
    long double Coordinate1 = sqrt(pow(radius,2) - pow((Horz - y_centre),2))+ x_centre;
    long double Coordinate2 =  - sqrt(pow(radius,2) - pow((Horz - y_centre),2))+ x_centre;
    Coord[0] = Coordinate1;
    Coord[1] = Horz;
    Coord[2] = Coordinate2;
    Coord[3] = Horz;
    return(Coord);
};

std::vector <double> Sensor::VertAndCircInteraction(double Vert){
    // (y-a)^2+ (x-b)^2 = r^2
    // =>
    //  (y-a)^2 = r^2 - (x-b)^2
    // =>
    // y = (+/-)sqrt (r^2 - (x-b)^2) +a
    std::vector <double> Coord(4);
    long double Coordinate1 = sqrt(pow(radius,2) - pow((Vert - location_x),2))+ location_y;
    long double Coordinate2 = -sqrt(pow(radius,2) - pow((Vert - location_x),2))+ location_y;
    Coord[0] = Vert;
    Coord[1] = Coordinate1;
    Coord[2] = Vert;
    Coord[3] = Coordinate2;
    return(Coord);
};

std::vector <double> Sensor::AngleAndCircInteraction(double m_Angle, double c_Angle){
    
    std::vector <double> Coord(4);
    //If the line of the animal movement cross the circle part of the sensor circle
    // Using the equation of a circle:
    // (Y-a)^2 +(X-b)^2 = r^2
    // therefore:
    // (mX+c-a)^2 +(X-b)^2 = r^2
    // (mX - A)^2 +(X-b)^2 = r^2 (where A= a-c)
    // (mX)^2 - 2AmX + A^2 + X^2 - 2bX +b^2 = r^2
    // (mX)^2 +X^2  - 2AmX - 2bX +b^2 + A^2 - r^2 = 0
    // (mX)^2 +X^2  - 2AmX - 2bX +b^2 + A^2 - r^2 = 0
    // (m^2+1)X^2 - 2(Am+b)X +(b^2 +A^2 - r^2) = 0
    // FIrst term: (Grad.of.animal^2+1)
    // Second term:  -2* (Grad.of.animal*(intercept.of.animal - y.location.of.Sensor)+ x.location.of.Sensor)
    // Third term: x.location.of.Sensor^2 + (intercept.of.animal - y.location.of.Sensor)^2 +radius^2
    long double circ_term1 = pow(m_Angle,2)+1;
    long double temp_A =  location_y - c_Angle;
    long double circ_term2 = -(m_Angle*temp_A +location_x)*2;
    long double circ_term3 = pow(temp_A,2)+pow(location_x,2) - pow(radius,2);
    
    // The Quadractic formula can be used to solve: (m^2+1)X^2 - 2(Am+b)X +(b^2 +A^2 - r^2) = 0
    // for X and hence for the coordinate of the intercept between circle and line
    // Quaddractic formula:
    //          (-b+-sqrt(b^2-4ac))/2a
    // if the term  (b^2-4ac) is less than zero then there is no iterception between line and circle
    // and there are no solutions
    // can skip the next parts
    long double temp = pow(circ_term2,2)-(4*circ_term1*circ_term3);
    
    if(temp>0 || approximatelyequal(temp,0)){
        if(approximatelyequal(temp,0)){temp=0;} // This needs to be included for tangents
        
        double circ_solsqrt = sqrt(temp);
        double circ_xsol1 = (-circ_term2 - circ_solsqrt)/(2*circ_term1);
        double circ_xsol2 = (-circ_term2 + circ_solsqrt)/(2*circ_term1);
        double circ_ysol1 = m_Angle*circ_xsol1 + c_Angle;
        double circ_ysol2 = m_Angle*circ_xsol2 + c_Angle;
    // Vector to return
    Coord[0] = circ_xsol1;
    Coord[1] = circ_ysol1;
    Coord[2] = circ_xsol2;
    Coord[3] = circ_ysol2;
    }
    return(Coord);
};

// Calculates the "time" when the line movement would intersect with the detector (X and Y)
// This is calculated as the proportion of the total distance moved in 1 step
// Therefore if this value is less than 1, then it's couple be within the movement step
// The angle of the movement has to be calulated as well,
// to make sure that the movement is in the right direction (forward instead of backwards)
std::vector <double> Sensor::TimeAndAngleCal(double Y, double X, double previous_y_animal, double previous_x_animal, double disttotal){
    
    std::vector <double> returnvalues(4);

    double distedge = DistTwoPoints(X,previous_x_animal,Y,previous_y_animal);
    double time = distedge/disttotal;
    //Find the angle
    
    double AngleAnimalCap = AngleTwoPoints(previous_x_animal,X,previous_y_animal,Y);
    
    if((approximatelyequal(Y,previous_y_animal) && approximatelyequal(X,previous_x_animal))
       ||approximatelyequal(AngleAnimalCap,2*M_PI)){AngleAnimalCap=0;};
    if(approximatelyequal(time,0)){time=0;};
    
    returnvalues[0] = time;
    returnvalues[1] = AngleAnimalCap;
    return(returnvalues);
};




//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------------------------------------
                                            Capturing of the animal

// Case of interest
// In the code below there are print statements for "cases of interest", if an individual should be returning 
// a capture at a specific location but is not then by uncommenting these then values will be printed to the 
// screen. The problem can be easily indentified from these.
 
// FROM *main* INPUTS INTO ARE ENTERED INTO: *CapturesIntersection*
//     1 - Calculates the total distance moved
//     2 - Calculates the gradient/ intercept of the movement
//     3 - Enters gradient of movement and detector edge into * SensorAndMovement *
//       to calulate intercepts location on detectcor EDGE 1 (the edge on the left)
//          => which are entered into * CapturesIndividual * to calulate if in Sensor detection zone
//     4 - Does step 3, to edge 2 (the right edge of the detector)
//     5 - Enters gradient of movement and equation of the curve into * SensorCircAndMovement *
//       to calculate intercepts location of the movement and the circular edge of detctor range
//          => which are entered into * CapturesIndividual * to calulate if in Sensor detection zone
//     6 - Enters end location into * CapturesIndividual * to calulate if in Sensor detection zone
//
//  INPUT FROM *CapturesIntersection* go into *SensorCircAndMovement*
//     1 - Initates variables
//     2 - Finds correct type of animal movement (vertical, horizontal, angle), and calcualtes the intercept
//     3 - Calculates the time this occurs and the angle which the animal has to move to achieve this intercept
//     4 - If within the step then the intercept value is put into *CapturesIndividual* to check whether it is
//         a capture and, if appropriate, save the value.
// 
 
// => Captures individual
//         * Are the possible locations of with the Sensor radius?
//         * Are the possible locations within the width of the Sensor?
//         * Is the Sensor within the width of the Animal call?
 
 --------------------------------------------------------------------------------------------------------*/

///////////////////////
///////////////////////
int Sensor::CapturesIntersection(double location_x_animal, double location_y_animal, // Current location
                                     double previous_x_animal, double previous_y_animal, // Previous location
                                     int Individual_ID,                                  // Animal ID
                                     double call_halfwidth, double move_angle,           // Call direction & width
                                     int itnumber){                                      // Iteration number
    int captured = 0;
    double disttotal = DistTwoPoints(previous_x_animal,location_x_animal,previous_y_animal,location_y_animal);
    //double disttotalcam = DistTwoPoints(location_x,location_x_animal,location_y,location_y_animal);

    //if(disttotalcam<20){std::cout<<"Hello"<<std::endl;}
    /*---------------------------------------------------------------------------------------------------------
    // Check for specific case of interest
    if(Individual_ID== 471 && Sensor_StepOn>212 && Sensor_StepOn<214){
        std::cout<<"Start, "<<"location animal: "<<location_x_animal<<", " <<location_y_animal // Current location
                    <<", Past location animal: "<<previous_x_animal<<", "<< previous_y_animal
                    << ", move_angle: " <<move_angle
                    <<", radius: " <<radius
        << std::endl;
    };
    //----------------------------------------------------------------------------------------------------------*/
    
    // finds the total distance travelled between it's new location and it's old location
    
    //If the animal movement was a line on a graph with a gradient and a intercept, Y=mX+c, then:
    //  - gadient would be, m=(change x/change y)
    //  - intercpet would be: y-mx=c (where y and x are known)
    double m_animal  = GradientFromAngle(move_angle);
    double c_animal  = location_y_animal-location_x_animal*m_animal;
    
    if(angle_HalfWidth < M_PI){ // If the Sensor width is 360˚ then the straight line edges are not important
        /*---------------------------------------------------------------------------------------------------------
         // Check for specific case of interest
        if(Individual_ID== 471 && Sensor_StepOn>212 && Sensor_StepOn<214){
         std::cout<<"Dect1, "<<"m_animal: "<< m_animal<<", c_animal: "<< c_animal << "; m_detector1"<< m_detector1<< std::endl;
        };
        //----------------------------------------------------------------------------------------------------------*/
        // Checks for crossing the boundaries for detector 1
        captured += SensorAndMovement(location_x_animal, location_y_animal,
                                  previous_x_animal,previous_y_animal,
                                  Individual_ID,
                                  call_halfwidth, move_angle,
                                  itnumber,
                                  m_animal,c_animal,
                                  m_detector1, c_detector1, g_detector1, vh_det1,
                                  disttotal);
    
        /*---------------------------------------------------------------------------------------------------------
         // Check for specific case of interest
         if(Individual_ID== 0 && Sensor_StepOn>653 && Sensor_StepOn<658){
         std::cout<<"Dect2, "<<"m_animal: "<< m_animal<<", c_animal: "<< c_animal << "; m_detector2"<< m_detector2<< std::endl;
         };
         // ----------------------------------------------------------------------------------------------------------*/
        // Checks for crossing the boundaries for detector 2
        captured += SensorAndMovement(location_x_animal,location_y_animal,
                                  previous_x_animal,previous_y_animal,
                                  Individual_ID,
                                  call_halfwidth, move_angle,
                                  itnumber,
                                  m_animal,c_animal,
                                  m_detector2, c_detector2,g_detector2,vh_det2,
                                  disttotal);
    };
    /*---------------------------------------------------------------------------------------------------------
     // Check for specific case of interest
    if(Individual_ID== 0 && Sensor_StepOn>653 && Sensor_StepOn<658){std::cout<<"Circ"<< std::endl;};
     //----------------------------------------------------------------------------------------------------------*/
    // Checks for crossing the boundaries for circular edge of detector
    captured += SensorCircAndMovement(location_x_animal, location_y_animal,
                                      previous_x_animal, previous_y_animal,
                                      Individual_ID,
                                      call_halfwidth, move_angle,
                                      itnumber,
                                      m_animal, c_animal,
                                      disttotal);
    
    // If there are any captures then return 1 else return 0
    if(captured>=1){return(1);} else {return(0);};
};


/*---------------------------------------------------------------------------------------------------------
                                            SensorCircAndMovement
 
 // Does the animal move cross the circular edge of the detector region?
 // If so, is this within the current step?
 // If so, is it within the range of the detector?
 
 //     1 - Initates variables
 //     2 - Finds correct type of animal movement (vertical, horizontal, angle), and calcualtes the intercept
 //     3 - Calculates the time this occurs and the angle which the animal has to move to achieve this intercept
 //     4 - If within the step then the intercept value is put into *CapturesIndividual* to check whether it is 
 //         a capture and, if appropriate, save the value.
 ----------------------------------------------------------------------------------------------------------*/

int Sensor::SensorCircAndMovement(double location_x_animal, double location_y_animal,   // Current location
                                  double previous_x_animal, double previous_y_animal,       // Previous location
                                  int Individual_ID,                                        // Animal ID
                                  double call_halfwidth, double move_angle,                 // Direction and angle of call
                                  int itnumber,                                             // Iteration number
                                  double m_animal, double c_animal,                         // Gradient/intercept of animal
                                  double disttotal){                                        // Total distance moved
    
    //std::cout<<"SensorCircAndMovement " << "move_angle "<< move_angle<<std::endl;

    // Initial a variable for the total number of times the animal is captured on the circle part of the detector    
    // TandA is for the Time and Angle of the intercept value
    // XandY are the 2 solutions to where the line passes through the circle: (x1,y1,x2,y2)
    int captured =0;
    std::vector<double> TandA(2);
    std::vector<double> XandY(4); 
    
    //Animal has vertical movement
    if(location_x_animal == previous_x_animal){ //std::cout<<"Circ, Vert move"<<std::endl;
        XandY = VertAndCircInteraction(location_x_animal);
    }//END OF VERTICAL ANIMAL MOVEMENT
    
    //Animal has horizontal movement
    else if(location_y_animal == previous_y_animal){ //std::cout<<"Circ, Horz move"<<std::endl;
        XandY = HorzAndCircInteraction(location_y_animal, location_y, location_x, radius);
    }//END OF HORZONTAL ANIMAL MOVEMENT
    
    //Animal has horizontal movement
    else{//std::cout<<"Circ, Angle move"<<std::endl;
        XandY = AngleAndCircInteraction(m_animal, c_animal);
    }//END OF ANGLE ANIMAL MOVEMENT
    
   /*---------------------------------------------------------------------------------------------------------
     // Check for specific case of interest
    if(Individual_ID== 0 && Sensor_StepOn>653 && Sensor_StepOn<658){
        std::cout<<"XandY[0]: "<<XandY[0]<<", XandY[1]: "<<XandY[1]<<std::endl;
        std::cout<<"XandY[2]: "<<XandY[2]<<", XandY[3]: "<<XandY[3]<<std::endl;
     };
     //----------------------------------------------------------------------------------------------------------*/
    
    //--------- CHECK IN "CapturesIndividual" FOR CAPTURE ---------------//
    //Looking at both possible solutions for the line and the circle
    //For each sucessful capture adds a 1 to the "captured" value
    for(int v=0; v<2; v++){ 
        //time and angle of the interscept
        TandA = TimeAndAngleCal(XandY[(v*2)+1], XandY[v*2], previous_y_animal, previous_x_animal, disttotal);
        /*---------------------------------------------------------------------------------------------------------
         // Check for specific case of interest
        if(Individual_ID== 0 && Sensor_StepOn>653 && Sensor_StepOn<658){
        std::cout<<
        "XandY[(v*2)+1]: "<< XandY[(v*2)+1] << " XandY[v*2]: "<<  XandY[v*2]<<
        " previous_y_animal: "<< previous_y_animal << " previous_x_animal: "<<  previous_x_animal<<
        " current_y_animal: "<< location_y_animal << " location_x_animal: "<<  location_x_animal<<
        " m_animal: " << m_animal <<" c_animal: " << c_animal<<
        " TandA[0]: "<<TandA[0]<<", TandA[1]: "<<TandA[1]<< ", move_angle: "<<move_angle<<std::endl;
         };
         //----------------------------------------------------------------------------------------------------------*/
         
        // The time has to be less than the total distance away form starting location
        // the angle of movement from the start location to the capture location has to be the same as the movement angle.
        // It the start location is captured then the time and angle are both zero
        if(((TandA[0]<=1 || approximatelyequal(TandA[0],1)) && approximatelyequal(TandA[1],move_angle))
           ||(TandA[0]==0 && TandA[1]==0)){
            captured += CapturesIndividual(XandY[v*2], XandY[(v*2)+1],
                                       Individual_ID,
                                       call_halfwidth, move_angle,
                                       itnumber,
                                       TandA[0], 
                                       0);
        }; // END OF IF LOOP
    }; // END OF FOR v loop
    //--------- END OF CHECK FOR CAPTURE ---------------//
    
    // Return the total number of captures
    return(captured);
};
////////////////////



/*---------------------------------------------------------------------------------------------------------
                                                    SensorAndMovement
 
// Does the animal move cross the circular edge of the detector region?
// If so, is this within the current step?
// If so, is it within the range of the detector?

//     1 - Initates variables
//     2 - Finds correct type of animal movement (vertical, horizontal, angle), finds the correct type of
//         detector line (vertical, horozontal, angle)and calcualtes the intercept
//     3 - Calculates the time this occurs and the angle which the animal has to move to achieve this intercept
//     4 - If within the step then the intercept value is put into *CapturesIndividual* to check whether it is
//         a capture and, if appropriate, save the value.
 ----------------------------------------------------------------------------------------------------------*/
int Sensor::SensorAndMovement(double location_x_animal, double location_y_animal,
                                  double previous_x_animal, double previous_y_animal,
                                  int Individual_ID,
                                  double call_halfwidth, double move_angle,
                                  int itnumber,
                                  double m_animal,double c_animal,
                                  double m_detector, double c_detector,double g_detector, int det,
                                  double disttotal){
    
    //std::cout<<"SensorAndMovement"<<std::endl;
    
    // Initates a count for the number of captures
    // TandA is Time and Angle
    // XandY is an initialised vector for the possible coordinates of capture
    int captured =0;
    std::vector<double> TandA(2);
    std::vector<double> XandY(2);
    // Assuming that there is one possible interaction for 2 lines
    // If the lines are exactly on top of each other then the "capture" occurs
    // at the earliest opportunity, for this we need to initialise values for
    // the min and maximum values of the detector lines
    double mindect;
    double maxdect;
    
    //Animal has vertical movement
    if(location_x_animal == previous_x_animal){ 
        //Horzontal detector - then the solution is simply the y coord from horzontal line and the x coord from the vertical line
        if(det==2){ XandY[0] = previous_x_animal; XandY[1] = location_y; } // END IF DET==2
        // Vertical detector it HAS to have the same x-position as the movement
        // Then the x location is simply the value from both vertical lines
        // The y location is earliest location that the two lines overlap.
        else if(det==1 && location_x==previous_x_animal){
            XandY[0] = previous_x_animal;
            // If the detector is facing up then the min-value = Sensor_loc and the max-value = Sensor_loc+radius
            // otherwise the the min-value = Sensor_loc-radius and the max-value = Sensor_loc
            if(approximatelyequal(g_detector,M_PI)){mindect = location_x-radius; maxdect = location_x;}
            else{mindect=location_x; maxdect=location_x+radius;};
            // If the animal is moving up then I want the minumum value which both detector and movement has:
            // There for I want the max. value of the min. movement and min. detector
            // If the animal is moving down then I want the maxumum value which both detector and movement has:
            // There for I want the min. value of the max. movement and min. detector
            // => Animal moving down
            if(approximatelyequal(move_angle,M_PI)){XandY[1] = std::min(previous_y_animal,maxdect);}
            // => Animal moving up
            else{XandY[1] = std::max(previous_y_animal,mindect);};
        } // END IF DET==1
        // Detector at angle
        // The solution is the vertical line, and the y-coord can be calulated using *VertAndAngleInteraction*
        else{XandY[0] = location_x_animal; XandY[1] = VertAndAngleInteraction(location_x_animal, m_detector, c_detector);};// END ELSE
    }//--------END OF VERTICAL ANIMAL MOVEMENT ---------------//
    
    //Animal has horizontal movement
    else if(location_y_animal == previous_y_animal){
        //Horzontal detector
        // Therefore they must both share the same y-coord
        if(det==2 && location_y== previous_y_animal){
            // The y-coord is simly the value of the y-coord of the animal movement
            XandY[1] = previous_y_animal;
            // I need to idenfy the max. and min. x-value of the detector 
            if(approximatelyequal(g_detector,M_PI/2)){mindect=location_y; maxdect=location_y+radius;}
            else{mindect = location_y-radius; maxdect = location_y;};
            // if the movement is to the left then I want the max. of the min. animal location and the min. detector location
            // if the movement is the right then I need the min. of the max. animal location and the max. detector location
            if(approximatelyequal(move_angle,M_PI/2)){XandY[0] = std::max(previous_x_animal,mindect);}
            else{XandY[0] = std::min(previous_x_animal,maxdect);};
        } // END OF IF DET ==2
        // Vertical detector
        // The x-coord of the intercept is the x-coord of the detector, the y-coord of the movement
        else if(det==1){XandY[0] = location_x; XandY[1] = previous_y_animal;} // END OF IF DET ==1
        // Detector at angle
        // The y-coord of the movement, and the x-coord is calculated by HorzAndAngleInteraction
        else {XandY[0] = HorzAndAngleInteraction(location_y_animal, m_detector, c_detector); XandY[1] = location_y_animal;};// END ELSE
    }//--------END OF HORZONTAL ANIMAL MOVEMENT ---------------//
    
    
    // If the angle of the movement and the detector is the same, then they have to have the same intercept
    else if(approximatelyequal(m_detector,m_animal) && approximatelyequal(c_detector,c_animal)){        
        // Calculation of x location
        // The min./max. value of the detector is calculated
        if(g_detector<M_PI){mindect=location_x; maxdect=location_x+radius*sin(g_detector);}
        else{mindect = location_x+radius*sin(g_detector); maxdect = location_x;};
        // if the movement is to the left then I want the max. of the min. animal location and the min. detector location
        // if the movement is the right then I need the min. of the max. animal location and the max. detector location
        if(move_angle<M_PI){XandY[0] = std::max(previous_x_animal,mindect);}
        else{XandY[0] = std::min(previous_x_animal,maxdect);};
        
        // Calculation of y location
        // The min./max. value of the detector is calculated
        if(g_detector<M_PI/2 ||g_detector>3*M_PI/2 ){mindect=location_y; maxdect=location_y+radius*cos(g_detector);}
        else{mindect = location_y+radius*cos(g_detector); maxdect = location_y;};
        // if the movement is up then I want the max. of the min. animal location and the min. detector location
        // if the movement is down then I need the min. of the max. animal location and the max. detector location
        if(move_angle<M_PI/2 ||move_angle>3*M_PI/2 ){ XandY[1] = std::max(previous_y_animal,mindect);}
        else{XandY[1] = std::min(previous_y_animal,maxdect);};
    }//--------END OF SAME ANGLE ANIMAL MOVEMENT ---------------//
    
    
    else if(m_detector != m_animal){
        //std::cout<<"m_detector != m_animal"<<std::endl;
        //Horzontal detector
        if(det==2){ XandY[0] = HorzAndAngleInteraction(location_y, m_animal, c_animal); XandY[1] = location_y;} //END OF IF DET==2
        // Vertical detector
        else if(det==1){XandY[0] = location_x; XandY[1] = VertAndAngleInteraction(location_x, m_animal, c_animal);} //END OF IF DET==1
        // Angle detctor
        else{XandY =AngleAndAngleInteraction(m_detector, c_detector, m_animal, c_animal);};
        
    }//--------END OF DIFFERENT ANGLE ANIMAL MOVEMENT ---------------//
    else{
        //std::cout<<"Something is very wrong with Sensor::SensorAndMovement " <<  std::endl;
        exit (EXIT_FAILURE);
    };
    
    
    //--------- CHECK IN "CapturesIndividual" FOR CAPTURE ---------------//
    // Inputting into the capture inidvidual function
    //Calucaltes time and angle of the interscept
    TandA = TimeAndAngleCal(XandY[1], XandY[0], previous_y_animal, previous_x_animal, disttotal);
    
    
    //*---------------------------------------------------------------------------------------------------------
    // Check for specific case of interest
    //if(Individual_ID== 0 && Sensor_StepOn>653 && Sensor_StepOn<658){
    //    std::cout<<"move_angle: "<<move_angle<<std::endl;
     //   std::cout<<"XandY[0]: "<<XandY[0] <<", XandY[1]: "<<XandY[1] <<std::endl;
     //   std::cout<<"TandA[0]: "<<TandA[0] <<", TandA[1]: "<<TandA[1] <<std::endl;
    //};
    // ----------------------------------------------------------------------------------------------------------*/
    
    // The time has to be less than the total distance away form starting location
    // the angle of movement from the start location to the capture location has to be the same as the movement angle.
    if((TandA[0]<=1 && approximatelyequal(TandA[1],move_angle))||(TandA[0]==0 && TandA[1]==0)){
        captured += CapturesIndividual(XandY[0],  XandY[1],
                                       Individual_ID,
                                       call_halfwidth, move_angle,
                                       itnumber,
                                       TandA[0],
                                       0);
    }; //--------- END OF CAPTURE CHECK ---------------//

    
    //-------- Return ---------------//
    return(captured);
}; // END OF FUNCTION




//-----------------------------------------------------------
// Captures individual
// Are the possible locations of with the Sensor radius?
// Are the possible locations within the width of the Sensor?
// Is the Sensor within the width of the Animal call?
//-----------------------------------------------------------
int Sensor::CapturesIndividual(double location_x_animal,
                                   double location_y_animal,
                                   int Individual_ID,
                                   double call_halfwidth,
                                   double move_angle,
                                   int itnumber,
                                   double time,
                                   int call
                                   ){
    //if(Individual_ID== 471 && Sensor_StepOn>212 && Sensor_StepOn<214){std::cout<<"InCap, radius" << radius <<std::endl;};
    
    int captured=0;    
    double AngleFromSensor = 0;
    
    // If on the exact same spot as the Sensor assume it will be captured
    if(approximatelyequal(location_x_animal,location_x) && approximatelyequal(location_y_animal,location_y)){
        UpdateCaptures(Individual_ID,itnumber,location_x_animal,location_y_animal,time,call,0,0,0);
        captured = 1;
    }
    // If not at the exact same location then check whether it is
    else{
        // Calculates the distance between the Sensor and the animal
        double diff_animal_Sensor = DistTwoPoints(location_x_animal,location_x, location_y_animal, location_y);
        
        /*---------------------------------------------------------------------------------------------------------
         // Check for specific case of interest
        if(Individual_ID== 471 && Sensor_StepOn>212 && Sensor_StepOn<214){
            std::cout<<"Distance between animal & Sensor: "<<diff_animal_Sensor <<", Radius: "<<radius <<std::endl;
         };
        // ----------------------------------------------------------------------------------------------------------*/
        
        // If the animal is less than a radius distance away from the detector then it is enters the if statement
        if(approximatelyequal(diff_animal_Sensor,radius)|| diff_animal_Sensor<=radius){
            

        /*---------------------------------------------------------------------------------------------------------
         // Check for specific case of interest
            if(Individual_ID== 0 && Sensor_StepOn>653 && Sensor_StepOn<658){std::cout<< "RADIUS"<<std::endl;}
        //----------------------------------------------------------------------------------------------------------*/
               
            // If in range, is it in the angle?
            // Can use the AngleTwoPoints to calculates the angle 
            // FROM Sensor TO the animal
            AngleFromSensor = AngleTwoPoints(location_x, location_x_animal, location_y, location_y_animal);
            double AngleFromSensorCentre = AngleFromSensor - angle;
            if(AngleFromSensorCentre>M_PI){AngleFromSensorCentre-=2*M_PI;};
            // If the Sensor is a circle
            if(angle_HalfWidth == M_PI){
                //std::cout<<"ello"<<std::endl;
                double AngleFromAnimalCentre = RangeAngle(AngleFromSensor+M_PI);
                if(AngleFromAnimalCentre>M_PI){AngleFromAnimalCentre-=2*M_PI;};
                UpdateCaptures(Individual_ID,itnumber,location_x_animal,location_y_animal,time,call,AngleFromSensorCentre,AngleFromAnimalCentre,diff_animal_Sensor);
            };
            /*---------------------------------------------------------------------------------------------------------
             // Check for specific case of interest
            if(Individual_ID== 0 && Sensor_StepOn>653 && Sensor_StepOn<658){
                std::cout<< "location_x_animal: "<<location_x_animal<<std::endl;
                std::cout<< "location_y_animal: "<<location_y_animal<<std::endl;
                std::cout<< "Sensor location_x: "<<location_x<<std::endl;
                std::cout<< "Sensor location_y: "<<location_y<<std::endl;
                std::cout<< "Angle of Sensor: "<<angle<<std::endl;
                std::cout<< "Movement angle: "<<move_angle<<std::endl;
                std::cout<< "Min. sensor angle: "<<g_detector1<<std::endl;
                std::cout<< "Max. sensor angle: "<<g_detector2<<std::endl;
                std::cout<<"Angle from sensor to Animal: "<<AngleFromSensor <<std::endl;
               std::cout<<"Atan(x/y): "<<atan((location_x-location_x_animal)/(location_y- location_y_animal))<<std::endl;
             };
            //----------------------------------------------------------------------------------------------------------*/
        
            // If the angle is between min and max possible angle
            // But becuase things have been corrected lines for <0 & >2pi then it can be more complicated
            //  - IF the angle of the dect1 line is less than zero then the "AngleFromSensor" needs to be less than angle of dect2
            //      (between 0 and dect2)
            //  - IF the angle of the dect1 line is less than zero then the "AngleFromSensor" can also be greater than angle of dect1
            //      (between dect1 and 2pi)
            //  - "AngleFromSensor" could be equal to the angle of dect1
            //  - "AngleFromSensor" could be equal to the angle of dect2
            //  - "AngleFromSensor" could be between the angle of dect1 and the angle of dect2
            if((g_detector1>g_detector2 && AngleFromSensor <= g_detector2) ||
               (g_detector1>g_detector2 && AngleFromSensor >= g_detector1) ||
               approximatelyequal(g_detector1, AngleFromSensor)  ||
               approximatelyequal(g_detector2, AngleFromSensor)   ||
               (AngleFromSensor >= g_detector1 && AngleFromSensor <= g_detector2)){
                
                
                //Calcaulates the angle from the Animal to the detector as a bearing from north
                // Trig. shows that this is angle from "a to b" =  180 + "b to a"
                double AngleFromAnimal = AngleFromSensor+M_PI;
                // If greater than 2pi then correct (should not be less than zero!)
                AngleFromAnimal = RangeAngle(AngleFromAnimal);
                double AngleFromAnimalCentre = AngleFromAnimal;
                if(AngleFromAnimalCentre>M_PI){AngleFromAnimalCentre-=2*M_PI;};
                // initates values for min and max call angles

                /*---------------------------------------------------------------------------------------------------------
                 // Check for specific case of interest
                if(Individual_ID== 0 && Sensor_StepOn>653 && Sensor_StepOn<658){std::cout<<"In Sensor angle"<<std::endl;}
                //----------------------------------------------------------------------------------------------------------*/
                
                // If the animal is identifiable regardless of its direction (we say if has a 360 call)
                // in this case we can say it's captures as it is in the sensor range
                if(call_halfwidth==M_PI){
                    /*---------------------------------------------------------------------------------------------------------
                     // Check for specific case of interest
                    if(Individual_ID== 0 && Sensor_StepOn>653 && Sensor_StepOn<658){std::cout<<"Captured with a 360 call"<<std::endl;};
                     //----------------------------------------------------------------------------------------------------------*/
                    // Call the update captures function and let captured ==1.
                    UpdateCaptures(Individual_ID,itnumber,location_x_animal,location_y_animal,time,call,AngleFromSensorCentre, AngleFromAnimalCentre,diff_animal_Sensor);
                    captured = 1;
                }
                
                // If the animal is only identiable is certain possitions then have to take this into account
                else{
                    
                    double Min_Animalangle = move_angle-call_halfwidth;
                    double Max_Animalangle = move_angle+call_halfwidth;
                    
                    // Corrects to be between 0 and 2pi
                    Min_Animalangle = RangeAngle(Min_Animalangle);
                    
                    /*---------------------------------------------------------------------------------------------------------
                     // Check for specific case of interest
                    if(Individual_ID== 0 && Sensor_StepOn>653 && Sensor_StepOn<658){
                            std::cout<< "Min Animalangle: "<<Min_Animalangle<<std::endl;
                            std::cout<< "Max Animalangle: "<<Max_Animalangle<<std::endl;
                            std::cout<<"AngleFromAnimal: "<<AngleFromAnimal <<std::endl;
                     };
                    // ----------------------------------------------------------------------------------------------------------*/
            
                    // If the angle is between min and max possible angle
                    // But becuase things have been corrected lines for <0 & >2pi then it can be more complicated
                    //  - IF the min call angle is less than zero then the "AngleFromAnimal" can be be less than max call angle
                    //      (between 0 and max call angle)
                    //  - IF the min call angle is less than zero then the "AngleFromAnimal" can also be greater than min call angle
                    //      (between min call angle and 2pi)
                    //  - "AngleFromAnimal" could be equal to the angle of min call angle
                    //  - "AngleFromAnimal" could be equal to the angle of max call angle
                    //  - "AngleFromAnimal" could be between the min and max call angle
                    if((Min_Animalangle>Max_Animalangle && AngleFromAnimal <= Max_Animalangle) || 
                       (Min_Animalangle>Max_Animalangle && AngleFromAnimal >= Min_Animalangle) ||
                       approximatelyequal(Min_Animalangle, AngleFromAnimal) ||
                       approximatelyequal(Max_Animalangle, AngleFromAnimal) ||
                       (AngleFromAnimal >= Min_Animalangle && AngleFromAnimal <= Max_Animalangle)){
                        
                        /*---------------------------------------------------------------------------------------------------------
                         // Check for specific case of interest
                        if(Individual_ID== 0 && Sensor_StepOn>653 && Sensor_StepOn<658){std::cout<< "IN Animal ANGEL"<<std::endl;};
                        //----------------------------------------------------------------------------------------------------------*/
                        // If it's in the possible angle then record in vector
                        UpdateCaptures(Individual_ID,itnumber,location_x_animal,location_y_animal,time,call,AngleFromSensorCentre, AngleFromAnimalCentre,diff_animal_Sensor);
                        captured = 1;           
                    };//End of IF  - "detector in the width of the call"
                }; // End of ELSE  - "Not a 360 call"
            }; //End of IF - "in width of detector"
        }; //End of "radius" IF
    }; //END of else - not directly on same spot as Sensor
    
    /*-------- RETURN CAPTURED --------*/
    return(captured);
    
}; // End of function


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------------------------------
//
//                                           Tests
//
// - Test for ALL Interections functions
//      * TestVertAndAngleInteraction
//      * TestHorzAndAngleInteraction
//      * TestAngleAndAngleInteraction
//      * TestHorzAndCircInteraction
//      * TestVertAndCircInteraction
//      * TestAngleAndCircInteraction
//
// - TEST FOR TIME ANGLE calulations
// - TestGradientFromAngle
 --------------------------------------------------------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//--------TEST FOR INTERSECTION FUNCTIONS---------------//

// Unit testing for:
//      - VertAndAngleInteraction
void Sensor::TestVertAndAngleInteraction(){
    // Vertical line X = 0; Angle line: Gradient  = 1, Intercept = 5;
    // The intercept should be at (X= 0, Y=5)
    if(VertAndAngleInteraction(0, 1, 5)!=5){
        std::cout<<"Error! Failed Sensor test - TestVertAndAngleInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Vertical line X = 1; Angle line: Gradient  = -1, Intercept = 5;
    // The intercept should be at (X= 1, Y=4)
    if(VertAndAngleInteraction(1, -1, 5)!=4){
        std::cout<<"Error! Failed Sensor test - TestVertAndAngleInteraction: "<<"2"<< std::endl;
        exit (EXIT_FAILURE);
    };
    // Vertical line X = -1; Angle line: Gradient  = 1, Intercept = 5;
    // The intercept should be at (X= -1, Y=4)
    if(VertAndAngleInteraction(-1, 1, 5)!=4){
        std::cout<<"Error! Failed Sensor test - TestVertAndAngleInteraction: "<<"3"<< std::endl;
        exit (EXIT_FAILURE);
    };
    //IF passed then print to screen
    std::cout<<"Passed! Sensor test - TestVertAndAngleInteraction"<< std::endl;
};

// Unit testing for:
//      - TestHorzAndAngleInteraction
void Sensor::TestHorzAndAngleInteraction(){
    // Horizontal line Y = 0; Angle line: Gradient  = 1, Intercept = 5;
    // The intercept should be at (X= -5, Y=0)
    if(HorzAndAngleInteraction(0, 1, 5)!=-5){
        std::cout<<"Error! Failed Sensor test - TestHorzAndAngleInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Horizontal line Y = 1; Angle line: Gradient  = -1, Intercept = 5;
    // The intercept should be at (X= -5, Y=1)
    if(HorzAndAngleInteraction(1, -1, 5)!=4){
        std::cout<<"Error! Failed Sensor test - TestHorzAndAngleInteraction: "<<"2"<< std::endl;
        exit (EXIT_FAILURE);
    };
    // Horizontal line Y = 1; Angle line: Gradient  = 2, Intercept = 5;
    // The intercept should be at (X= 2, Y=1)
    if(HorzAndAngleInteraction(1, 2, 5)!=-2){
        std::cout<<"Error! Failed Sensor test - TestHorzAndAngleInteraction: "<<"3"<< std::endl;
        exit (EXIT_FAILURE);
    };
    //IF passed then print to screen
    std::cout<<"Passed! Sensor test - TestHorzAndAngleInteraction "<< std::endl;
};// END OF UNIT TEST 


// Unit testing for:
//      - TestHorzAndAngleInteraction
void Sensor::TestAngleAndAngleInteraction(){
    // Initate vector for results of the AngleAndAngleInteraction
    std::vector <double> Estimate(2);
    // Line 1: G1 = 1, I1 = 0 ; Line2: G2 = -1, I2 = 0;
    // The intercept should be at (X= 0, Y=0)
    Estimate = AngleAndAngleInteraction(1, 0, -1, 0);
    if(Estimate[0]!=0 && Estimate[1]!=0){
        std::cout<<"Error! Failed Sensor test - TestAngleAndAngleInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Line 1: G1 = 1, I1 = 0 ; Line2: G2 = 2, I2 = 0;
    // The intercept should be at (X= 0, Y=0)
    Estimate = AngleAndAngleInteraction(1,0, 2, 0);
    if(Estimate[0]!=0 && Estimate[1]!=0){
        std::cout<<"Error! Failed Sensor test - TestAngleAndAngleInteraction: "<<"2"<< std::endl;
        exit (EXIT_FAILURE);
    };
    // Line 1: G1 = 1, I1 = 10 ; Line2: G2 = -1, I2 = 5;
    // The intercept should be at (X= -2.5, Y=7)
    Estimate = AngleAndAngleInteraction(1,10, -1, 5);
    if(Estimate[0]!=-2.5 && Estimate[1]!=7){
        std::cout<<"Error! Failed Sensor test - TestAngleAndAngleInteraction: "<<"3"<< std::endl;
        exit (EXIT_FAILURE);
    };
    //IF passed then print to screen
    std::cout<<"Passed! Sensor test - TestAngleAndAngleInteraction"<< std::endl;
}; // END OF UNIT TEST 

// Unit testing for:
//      - TestHorzAndCircInteraction
void Sensor::TestHorzAndCircInteraction(){
    // Initate vector for results of the HorzAndCircInteraction
    std::vector <double> Estimate(4);
    // Horz: Y = 1; Circle: Xcentre = 0, Ycentre = 0, Radius = 1;
    // should be tangent to the top of the circle
    // The intercept should be at (X=0, Y=1) and (X=0, Y=1) 
    Estimate = HorzAndCircInteraction(1, 0, 0, 1); // Line tangent to the top of the circle, coordinates (0,1)
    if((Estimate[0]!=0 || Estimate[1]!=1) || (Estimate[2]!=0 || Estimate[3]!=1) ){
        std::cout<<"Error! Failed Sensor test - TestHorzAndCircInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Horz: Y = 1; Circle: Xcentre = 0, Ycentre = 0, Radius = 1;
    // should be tangent to the top of the circle
    // The intercept should be at (X=-1, Y=0) and (X=1, Y=0)
    Estimate = HorzAndCircInteraction(0, 0, 0, 1);
    // If neither (-1,0,1,0) and (1,0,-1,0) are true then problem with code
    if((Estimate[0]!=-1 || Estimate[1]!=0 || Estimate[2]!=1 || Estimate[3]!=0)
       && (Estimate[0]!=1 || Estimate[1]!=0 || Estimate[2]!=-1 || Estimate[3]!=0)){
        std::cout<< Estimate[0] <<" " <<Estimate[1] << " "<<std::endl;
        std::cout<<"Error! Failed Sensor test - TestHorzAndCircInteraction: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Horz: Y = 1; Circle: Xcentre = 0.5, Ycentre = 0, Radius = 1;
    // should be tangent to the top of the circle
    // The intercept should be at (X=-Sqrt(3)/2, Y=0.5 and X=Sqrt(3)/2, Y=0.5)
    Estimate = HorzAndCircInteraction(0.5, 0, 0, 1); // Line crosses top part of the circle, coordinates (+/-sqrt(0.75),0.5)
    double temp = sqrt(1-pow(0.5,2));
    // If neither (-sqrt(.75),0,sqrt(.75),0) and (sqrt(.75),0,-sqrt(.75),0) are true then problem with code
    if((Estimate[0]!=-temp || Estimate[1]!=0.5 || Estimate[2]!=temp || Estimate[3]!=0.5)
       && (Estimate[0]!=temp || Estimate[1]!=0.5 || Estimate[2]!=-temp || Estimate[3]!=0.5)){
        //std::cout<< temp << " "<<std::endl;
        //std::cout<< Estimate[0] <<" " <<Estimate[1] << " "<<std::endl;
        std::cout<<"Error! Failed Sensor test - TestHorzAndCircInteraction: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    };
    //IF passed then print to screen
    std::cout<<"Passed! Sensor test - TestHorzAndCircInteraction"<< std::endl;
}; // END OF UNIT TEST 


// THIS NEEDS TO BE CHANGED TO CALCULATE ESTIMATE TO BE 4//
// Unit testing for:
//      - TestVertAndCircInteraction
void Sensor::TestVertAndCircInteraction(){
    // Initate vector for results of the VertAndCircInteraction
    std::vector <double> Estimate(2);
    
    Estimate = VertAndCircInteraction(1);
    if(Estimate[0]!=0 && Estimate[1]!=0){
        std::cout<<"Error! Failed Sensor test - TestVertAndCircInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
    Estimate = VertAndCircInteraction(0);
    if((Estimate[0]!=-1 && Estimate[1]!=1)&&(Estimate[0]!=1 && Estimate[1]!=-1)){
        std::cout<<"Error! Failed Sensor test - TestVertAndCircInteraction: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
    Estimate = VertAndCircInteraction(0.5);
    double temp = sqrt(1-pow(0.5,2));
    if((Estimate[0]!=-temp && Estimate[1]!=temp)&&(Estimate[0]!=temp && Estimate[1]!=-temp)){
        std::cout<<"Error! Failed Sensor test - TestVertAndCircInteraction: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    };
    //IF passed then print to screen
    std::cout<<"Passed! Sensor test - TestVertAndCircInteraction"<< std::endl;
}; // END OF UNIT TEST 


void Sensor::TestAngleAndCircInteraction(){
    std::vector <double> Estimate(4);
    
    Estimate = AngleAndCircInteraction(1,0);
    double temp = cos(M_PI/4);
    if(((approximatelyequal(Estimate[0],-temp) && (approximatelyequal(Estimate[2],-temp)))
            &&
         (approximatelyequal(Estimate[2],temp) && (approximatelyequal(Estimate[3],temp))))
        &&
       ((approximatelyequal(Estimate[0],temp) && (approximatelyequal(Estimate[2],temp)))
        &&
        (approximatelyequal(Estimate[2],-temp) && (approximatelyequal(Estimate[3],-temp))))){
        std::cout<< temp<<std::endl;
        std::cout<< Estimate[0]<<std::endl;
        std::cout<<"Error! Failed Sensor test - TestAngleAndCircInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    Estimate = AngleAndCircInteraction(-1,0);
    if(((approximatelyequal(Estimate[0],-temp) && (approximatelyequal(Estimate[2],temp)))
        &&
        (approximatelyequal(Estimate[2],temp) && (approximatelyequal(Estimate[3],-temp))))
       &&
       ((approximatelyequal(Estimate[0],temp) && (approximatelyequal(Estimate[2],-temp)))
        &&
        (approximatelyequal(Estimate[2],-temp) && (approximatelyequal(Estimate[3],temp))))){
        std::cout<<"Error! Failed Sensor test - TestAngleAndCircInteraction: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    Estimate = AngleAndCircInteraction(1, sqrt(2));
    temp = 1/sqrt(2);
    if(((approximatelyequal(Estimate[0],-temp) && (approximatelyequal(Estimate[2],temp)))
        &&
        (approximatelyequal(Estimate[2],temp) && (approximatelyequal(Estimate[3],-temp))))
       &&
       ((approximatelyequal(Estimate[0],temp) && (approximatelyequal(Estimate[2],-temp)))
        &&
        (approximatelyequal(Estimate[2],-temp) && (approximatelyequal(Estimate[3],temp))))
       ){
        std::cout<< Estimate[0] <<" " << (1/sqrt(2))<<std::endl;
        std::cout<< Estimate[0] <<" " << Estimate[1] <<" "<< Estimate[2] <<" " << Estimate[3] <<" "<<std::endl;
        std::cout<<"Error! Failed Sensor test - TestAngleAndCircInteraction: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    }
    std::cout<<"Passed! Sensor test - TestAngleAndCircInteraction"<< std::endl;
};

//--------END OF TESTS FOR INTERSECTION  ---------------//


//--------TIME ANGLE TEST  ---------------//
void Sensor::TestTimeAndAngleCal(){
    std::vector <double> Estimate(2);
    // Vertical movement: intersept is at half way & directly below
    Estimate = TimeAndAngleCal(0,0,1,0,2);
    if(Estimate[0]!=0.5 || Estimate[1]!=M_PI){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Vertical movement: intersept is at half way & directly above
    Estimate = TimeAndAngleCal(1,0,0,0,2);
    if(Estimate[0]!=0.5 || Estimate[1]!=0){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Angle movement: intersept is at half way & at 45 degree
    Estimate = TimeAndAngleCal(1/sqrt(2),1/sqrt(2),0,0,2);
    if(approximatelyequal(Estimate[0],0.5)!=1 || approximatelyequal(Estimate[1],M_PI/4)!=1){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Angle movement: intersept is at half way & at 225 degree
    Estimate = TimeAndAngleCal(-1/sqrt(2),-1/sqrt(2),0,0,2);
    if(approximatelyequal(Estimate[0],0.5)!=1 || approximatelyequal(Estimate[1],5*M_PI/4)!=1){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"4" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Angle movement: intersept is at 100% of the distance & at 45 degree
    Estimate = TimeAndAngleCal(sqrt(2),sqrt(2),0,0,2);
    if(approximatelyequal(Estimate[0],1)!=1 || approximatelyequal(Estimate[1],M_PI/4)!=1){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"5" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Angle movement: intersept is at 200% of the distance & at 45 degree
    Estimate = TimeAndAngleCal(2*sqrt(2),2*sqrt(2),0,0,2);
    if(approximatelyequal(Estimate[0],2)!=1 || approximatelyequal(Estimate[1],M_PI/4)!=1){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"6" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Angle movement: intersept is at 0% of the distance & at 45 degree
    Estimate = TimeAndAngleCal(0,0,0,0,2);
    if(approximatelyequal(Estimate[0],0)!=1 || approximatelyequal(Estimate[1],0)!=1){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"7" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // IF passed print to screen
    std::cout<<"Passed! Sensor test - TestTimeAndAngleCal"<< std::endl;
};
/*---------- END OF TIME ANGLE TEST -----------------*/


/*---------- Test Gradient from angle -----------------*/
void Sensor::TestGradientFromAngle(){
    // Initate double for results
    double Estimate;
    //
    Estimate = GradientFromAngle(0); // Vertical "upwards" line
    if(isinf(Estimate)!=1){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    Estimate = GradientFromAngle(M_PI); // Vertical "downwards" line
    if(isinf(-1*Estimate)!=1 && Estimate>pow(-10,12)){ // Does not produce neg inf, but produces very low number
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    };
    Estimate = GradientFromAngle(M_PI/2); // Horizontal "left" line
    if(Estimate==0){ 
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    };
    Estimate = GradientFromAngle(3*M_PI/2); // Horizontal "right" line
    if(Estimate==0){ 
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"4" <<std::endl;
        exit (EXIT_FAILURE);
    };
    Estimate = GradientFromAngle(M_PI/4); // Line of the identity (Quadrant1)
    if(Estimate==1){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"5" <<std::endl;
        exit (EXIT_FAILURE);
    };
    Estimate = GradientFromAngle(3*M_PI/4); // Line of the negative identity (Quadrant2)
    if(Estimate==-1){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"6" <<std::endl;
        exit (EXIT_FAILURE);
    };
    Estimate = GradientFromAngle(5*M_PI/4); // Line of the identity (Quadrant3)
    if(Estimate==1){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"7" <<std::endl;
        exit (EXIT_FAILURE);
    };
    Estimate = GradientFromAngle(7*M_PI/4); // Line of the negative identity (Quadrant4)
    if(Estimate==-1){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"8" <<std::endl;
        exit (EXIT_FAILURE);
    }
    // If passed print to screen
    std::cout<<"Passed! Sensor test - TestGradientFromAngle"<< std::endl;
};
/*End of test for Gradient from angle*/




//--------TEST FOR CAPTURING WITHIN RADIUS AND ANGLE  ---------------//

/*-----------Test Movement Capture thing--------------*/


//NEEDS completing
// Is the Sensor lines at a angles and is the movement also at an angle?

void Sensor::TestCapturesIntersection(){
    
    int loc1;
    int loc2;
    double temp;
    
    //-------------------------------------------------//
    // Vertical movement through the centre of sector
    // Vert movement Angle Sensor
    loc1 =0; loc2 =0;
    resetCaptures();
    /*Angle Sensor and vertical movement. Should get captured:
     //  - At detector1 line (X=0,Y=0.5,T=0)
     //  - At detector2 line  (X=0,Y=0,T=0)
     //  - At Circle line (X=0 ,Y=1 ,T=1)
     //  - At end of the step (X=0,Y=1,T=1) */
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    g_detector1 = 7*M_PI/4; g_detector2 =angle + angle_HalfWidth;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0; vh_det1 =0; vh_det2 =0;
    //std::cout<<"Test CapInter"<< std::endl;
    CapturesIntersection(0,1,0,0, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         0,  // move_angle,
                         1); // IT
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"1a" <<std::endl;exit (EXIT_FAILURE);};
            
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", " <<Captures[i][6]<<std::endl;
             
            if(Captures[i][4] ==0 && Captures[i][5] ==0){loc1 += 1;};
            if(Captures[i][4] ==0 && Captures[i][5] ==1){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"1b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< Captures[i][6] <<std::endl;
            };
        };
    };
     //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc1<< std::endl;
     if(loc1!=2 || loc2!=2){
         std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"1c" <<std::endl;
         exit (EXIT_FAILURE);
     };
    
    //-------------------------------------------------//
    // Horizontal movement through the centre of sector
    // Horz movement Angle Sensor
    resetCaptures();
    /// this brings up ever  - because something somewhere needs an approx.equal///
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    g_detector1 = 7*M_PI/4; g_detector2 =angle + angle_HalfWidth;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0; vh_det1 =0; vh_det2 =0;
    //std::cout<<"Test CapInter: 2"<< std::endl;
     /*Angle Sensor and Horizontal movement. Should get captured:
     //  - At detector1 line (X=-0.5,Y=0.5,T=0)
     //  - At detector2 line  (X=0.5,Y=0.5,T=1)
     //  - Not at Circle line (X=0 ,Y= ,T= )
     //  - At end of the step (X=0.5,Y=0.5,T=1) */
    CapturesIntersection(0.5,0.5,-0.5,0.5, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         M_PI/2,  // move_angle,
                         1); // IT
    loc1 =0; loc2 =0;
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"2a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(Captures[i][4] ==0.5 && Captures[i][5] ==0.5){loc1 += 1;};
            if(Captures[i][4] ==-0.5 && Captures[i][5] ==0.5){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"2b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< Captures[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=1){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"2c" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    //-------------------------------------------------//
    // Angle movement through the centre of sector
    // Angle movement Angle Sensor
    resetCaptures();
    /*Angle Sensor and angle movement. Should get captured:
     //  - At detector1 line (X=-0.5,Y=0.5,T=0)
     //  - Not at detector2 line  (X= ,Y= ,T= )
     //  - At Circle line (X=0 ,Y=1 ,T=1) 
     //  - At end of the step (X=0 ,Y=1 ,T=1) */
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    g_detector1 = 7*M_PI/4; g_detector2 =angle + angle_HalfWidth;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0; vh_det1 =0; vh_det2 =0;
    //std::cout<<"Test CapInter: 3"<< std::endl;
    temp= 1/sqrt(2);
    CapturesIntersection(0,1,-0.5,0.5,  // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         M_PI/4,  // move_angle,
                         1); // IT
    loc1 =0; loc2 =0;
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"3a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(Captures[i][4] ==-0.5 && Captures[i][5] ==0.5){loc1 += 1;};
            if(Captures[i][4] ==0 && Captures[i][5] ==1){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"3b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< Captures[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=1 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"3c" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    
    //-------------------------------------------------//
    // Horizontal movement through top of the sector, through both edges and the circle twice
    resetCaptures();
    /*Angle Sensor and horz movement. Should get captured:
     //  - At detector1 line (X=-1/sqrt(2),Y=1/sqrt(2),T=0)
     //  - At detector2 line  (X=1/sqrt(2),Y=1/sqrt(2),T=1)
     //  - At Circle line (X=1/sqrt(2),Y=1/sqrt(2),T=1) && (X=-1/sqrt(2),Y=1/sqrt(2),T=0)
     //  - At end of the step (X=1/sqrt(2),Y=1/sqrt(2), T=1) */
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    g_detector1 = 7*M_PI/4; g_detector2 =angle + angle_HalfWidth;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0; vh_det1 =0; vh_det2 =0;
    //std::cout<<"Test CapInter: 4"<< std::endl;
    temp= 1/sqrt(2);
    CapturesIntersection(temp,temp,-temp,temp, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         M_PI/2,  // move_angle,
                         1); // IT
    loc1 =0; loc2 =0;
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"4a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<"Captures " <<std::endl;
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(approximatelyequal(Captures[i][4],-temp)==1 && approximatelyequal(Captures[i][5],temp)==1){loc1 += 1;};
            if(approximatelyequal(Captures[i][4],temp)==1 && approximatelyequal(Captures[i][5],temp)==1){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"4b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< Captures[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=3){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"4c" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    
    //-------------------------------------------------//
    // Angle movement through on edge of sector
    resetCaptures();
    /*Angle Sensor and same-angle movement. Should get captured:
     //  - At detector1 line (X=0,Y=0,T=0)
     //  - At detector2 (same angle) line  (X=0,Y=0,T=0)
     //  - At Circle line (X=1/sqrt(2),Y=1/sqrt(2), T=1)
     //  - At end of the step (X=1/sqrt(2),Y=1/sqrt(2), T=1) */
    /// this brings up ever  - because something somewhere needs an approx.equal///
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    g_detector1 = 7*M_PI/4; g_detector2 =angle + angle_HalfWidth;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0; vh_det1 =0; vh_det2 =0;
    //std::cout<<"Test CapInter: Number 5"<< std::endl;
    CapturesIntersection(temp,temp,0,0, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         M_PI/4,  // move_angle,
                         1); // IT
    loc1 =0; loc2 =0;
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"5a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(approximatelyequal(Captures[i][4],0) ==1 && approximatelyequal(Captures[i][5],0) ==1 ){loc1 += 1;};
            if(approximatelyequal(Captures[i][4],temp) ==1 && approximatelyequal(Captures[i][5],temp) ==1 ){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"5b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< Captures[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"5c" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    
    
    //-------------------------------------------------//
    // Angle movement through on edge of sector
    resetCaptures();
    /*Angle Sensor and same-angle movement. Should get captured:
    //  - At detector1 (same angle) line (X=0,Y=0,T=0)
    //  - At detector2 line  (X=0,Y=0,T=0)
    //  - At Circle line (X=-1/sqrt(2),Y=1/sqrt(2), T=1)
    //  - At end of the step (X=-1/sqrt(2),Y=1/sqrt(2), T=1) */
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    g_detector1 = 7*M_PI/4; g_detector2 =angle + angle_HalfWidth;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0; vh_det1 =0; vh_det2 =0;
    //std::cout<<"Test CapInter: Number 6"<< std::endl;
    CapturesIntersection(-temp,temp,0,0, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         7*M_PI/4,  // move_angle,
                         1); // IT
    loc1 =0; loc2 =0;
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"6a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(approximatelyequal(Captures[i][4],0) ==1 && approximatelyequal(Captures[i][5],0) ==1 ){loc1 += 1;};
            if(approximatelyequal(Captures[i][4],-temp) ==1 && approximatelyequal(Captures[i][5],temp) ==1 ){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"6b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< Captures[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"6c" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    //-------------------------------------------------//
    // Vert movement Vert Sensor
    // Vert movement Horz Sensor
    resetCaptures();
    /// this brings up ever  - because something somewhere needs an approx.equal///
    angle = M_PI/4; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    g_detector1 = angle - angle_HalfWidth; g_detector2 =angle + angle_HalfWidth;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0; vh_det1 =1; vh_det2 =2;
    //std::cout<<"Test CapInter: Number 7"<< std::endl;
    /*Vertical/horzontal Sensor and veritcal movement. Should get captured:
    //  - AT horzontal detector2 line (X=0,Y=0,T=0)
    //  - At vertical detcector1 line  (X=0,Y=0,T=0) 
    //  - At Circle line (X=0,Y=1, T=1)
    //  - At end of the step (X=0,Y=1, T=1) */
    CapturesIntersection(0,1,0,0, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         0,  // move_angle,
                         1); // IT
    loc1 =0; loc2 =0;
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"7a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(approximatelyequal(Captures[i][4],0) ==1 && approximatelyequal(Captures[i][5],0) ==1 ){loc1 += 1;};
            if(approximatelyequal(Captures[i][4],0) ==1 && approximatelyequal(Captures[i][5],1) ==1 ){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"7b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< Captures[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"7c" <<std::endl;
        exit (EXIT_FAILURE);
    };

    //-------------------------------------------------//
    // Horz movement Vert Sensor
    // Horz movement Horz Sensor
    resetCaptures();
    /// this brings up ever  - because something somewhere needs an approx.equal///
    angle = M_PI/4; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    g_detector1 = angle - angle_HalfWidth; g_detector2 =angle + angle_HalfWidth;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0; vh_det1 =1; vh_det2 =2;
    //std::cout<<"Test CapInter: Number 8"<< std::endl;
    /*Vertical/horzontal Sensor and horiz movement. Should get captured:
     //  - AT horzontal detector2 line (X=0,Y=0,T=0)
     //  - At vertical detcector1 line  (X=0,Y=0,T=0)
     //  - At Circle line (X=1,Y=0, T=1)
     //  - At end of the step (X=1,Y=0, T=1) */
    CapturesIntersection(1,0,0,0, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         M_PI/2,  // move_angle,
                         1); // IT
    loc1 =0; loc2 =0;
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"8a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(approximatelyequal(Captures[i][4],0) ==1 && approximatelyequal(Captures[i][5],0) ==1 ){loc1 += 1;};
            if(approximatelyequal(Captures[i][4],1) ==1 && approximatelyequal(Captures[i][5],0) ==1 ){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"8b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< Captures[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"8c" <<std::endl;
        exit (EXIT_FAILURE);
    };

    

    //-------------------------------------------------//
    // Angle movement Vert Sensor
    // Angle movement Horz Sensor
    resetCaptures();
    /// this brings up ever  - because something somewhere needs an approx.equal///
    angle = M_PI/4; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    g_detector1 = angle - angle_HalfWidth; g_detector2 =angle + angle_HalfWidth;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0; vh_det1 =1; vh_det2 =2;
    //std::cout<<"Test CapInter: Number 9"<< std::endl;
    /*Vertical/horzontal Sensor and angle movement. Should get captured:
     //  - AT horzontal detector1 line (X=0,Y=0.5,T=0)
     //  - At vertical detcector1 line  (X=0.5,Y=0,T=1)
     //  - NOT at Circle line (X= ,Y=, T=)
     //  - At end of the step (X=0.5,Y=0, T=1) */
    CapturesIntersection(0.5,0,0,0.5, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         3*M_PI/4,  // move_angle,
                         1); // IT
    loc1 =0; loc2 =0;
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"9a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(approximatelyequal(Captures[i][4],0) ==1 && approximatelyequal(Captures[i][5],0.5) ==1 ){loc1 += 1;};
            if(approximatelyequal(Captures[i][4],0.5) ==1 && approximatelyequal(Captures[i][5],0) ==1 ){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"9b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< Captures[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=1 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"9c" <<std::endl;
        exit (EXIT_FAILURE);
    };

    
    //-------------------------------------------------//
    // Angle movement not near Sensor
    loc1 =0; loc2 =0;
    /*Angle Sensor and vertical movement. Should get NO captured:
     //  - At detector1 line (X= ,Y= ,T=)
     //  - At detector2 line  (X= ,Y= ,T=)
     //  - At Circle line (X= ,Y= ,T=)
     //  - At end of the step (X= ,Y= ,T=) */
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    g_detector1 = 7*M_PI/4; g_detector2 =angle + angle_HalfWidth;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0; vh_det1 =0; vh_det2 =0;
    resetCaptures();
    //std::cout<<"Test CapInter"<< std::endl;
    CapturesIntersection(100,100,99,99, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         M_PI/4,  // move_angle,
                         1); // IT
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // Should have No Captures
            std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"10a " << Captures.size() <<std::endl;
            exit (EXIT_FAILURE);
        };
    };
    
    std::cout<<"Passed! Sensor test - TestSensorAndMovement"<< std::endl;
};

