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
    //location_x = 1000;
    //location_y = 6000;
    
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
    
    vh_det1 =0;
    vh_det2 =0;
    
    if(g_detector1== 0 || g_detector1== M_PI|| g_detector1== 2*M_PI){vh_det1 = 1;} //Line is vertical
    else if(g_detector1 == M_PI/2 || g_detector1== 3*M_PI/2){vh_det1 = 2;} //Line is Horizontal
    
    if(g_detector2== 0 || g_detector2== M_PI|| g_detector2== 2*M_PI){vh_det2 = 1;} //Line is vertical
    else if(g_detector2 == M_PI/2 || g_detector2== 3*M_PI/2){vh_det2 = 2;} //Line is Horizontal
    
    Captures.resize(round(DensityAnimals*((Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY)))*3);
    

    
    capturecount=0;
    myvector.resize(10);
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

void Sensor::setXLoc(double a){
    location_x = a;
    c_detector1  = location_y - location_x*m_detector1;
    c_detector2  = location_y - location_x*m_detector2;
};

void Sensor::setYLoc(double a){
    location_y = a;
    c_detector1  = location_y - location_x*m_detector1;
    c_detector2  = location_y - location_x*m_detector2;
};

void Sensor::setAngle(double a){
    
    angle = a;
    
    // REcaluclates the angles/gradients of the lines
    g_detector1 = angle-angle_HalfWidth;
    g_detector2 = angle+angle_HalfWidth;
    
    g_detector1= RangeAngle(g_detector1);
    g_detector2= RangeAngle(g_detector2);
    
    m_detector1 = GradientFromAngle(g_detector1);
    m_detector2 = GradientFromAngle(g_detector2);
    c_detector1  = location_y - location_x*m_detector1;
    c_detector2  = location_y - location_x*m_detector2;
    
    vh_det1 =0;
    vh_det2 =0;
    
    if(g_detector1== 0 || g_detector1== M_PI|| g_detector1== 2*M_PI){vh_det1 = 1;} //Line is vertical
    else if(g_detector1 == M_PI/2 || g_detector1== 3*M_PI/2){vh_det1 = 2;} //Line is Horizontal
    
    if(g_detector2== 0 || g_detector2== M_PI|| g_detector2== 2*M_PI){vh_det2 = 1;} //Line is vertical
    else if(g_detector2 == M_PI/2 || g_detector2== 3*M_PI/2){vh_det2 = 2;} //Line is Horizontal


};
void Sensor::setHalfAngle(double a){
    
    angle_HalfWidth = a;
    
    // REcaluclates the angles/gradients of the line
    g_detector1 = angle-angle_HalfWidth;
    g_detector2 = angle+angle_HalfWidth;

    g_detector1= RangeAngle(g_detector1);
    g_detector2= RangeAngle(g_detector2);
    
    m_detector1 = GradientFromAngle(g_detector1);
    m_detector2 = GradientFromAngle(g_detector2);
    c_detector1  = location_y - location_x*m_detector1;
    c_detector2  = location_y - location_x*m_detector2;
    
    vh_det1 =0;
    vh_det2 =0;
    
    if(g_detector1== 0 || g_detector1== M_PI|| g_detector1== 2*M_PI){vh_det1 = 1;} //Line is vertical
    else if(g_detector1 == M_PI/2 || g_detector1== 3*M_PI/2){vh_det1 = 2;} //Line is Horizontal
    
    if(g_detector2== 0 || g_detector2== M_PI|| g_detector2== 2*M_PI){vh_det2 = 1;} //Line is vertical
    else if(g_detector2 == M_PI/2 || g_detector2== 3*M_PI/2){vh_det2 = 2;} //Line is Horizontal
    
    

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

/*----------------------------------------------
 // RangeAngle corrects the angle so that the
 // angle is between 0 and 2pi
 ----------------------------------------------*/
double Sensor::RangeAngleMinus(double angle){
    if(angle<(-1*M_PI)){angle += 2*M_PI;} else if(angle>=M_PI){angle -= 2*M_PI;};
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
   // if(Sensor_identifier==0 && Sensor_StepOn>17654 && Sensor_StepOn<17656){std::cout<< "diffx: " <<diffx <<", diffy: " <<diffy <<std::endl;}
    // Gives the angle from north
    double theta = atan(diffx/diffy);// - M_PI_2;
   // if(Sensor_identifier==0 && Sensor_StepOn>17654 && Sensor_StepOn<17656){std::cout<< "theta: " <<theta  <<std::endl;}

    if(diffy<0){theta+= M_PI; }//if(diffy>0){theta+= M_PI;};}
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
void Sensor::UpdateCaptures(int Individual_ID,int itnumber,double location_x_animal,double location_y_animal,double time, int call, double CamToAnimal, double AnimalToCam , double DistToCam,  std::ofstream &Captures){
    
    /*---------------------------------------------------------------------------------------------------------
     // Check for specific case of interest
    if(Sensor_identifier == 3 && Individual_ID== 0 && Sensor_StepOn>430 && Sensor_StepOn<432){
    std::cout<<"Caught!"<<std::endl; //<<Sensor_StepOn << " time: "<< time <<std::endl;
     };
     //----------------------------------------------------------------------------------------------------------*/
    
    //std::cout<<"Caught! "<< capturecount <<"/"<< Captures.size() <<" " << Sensor_identifier<<std::endl;
    myvector[0] = Individual_ID;
    myvector[1] = Sensor_StepOn;
    myvector[2] = Sensor_identifier;
    myvector[3] = itnumber;
    myvector[4] = location_x_animal;
    myvector[5] = location_y_animal;
    myvector[6] = time;
    myvector[7] = CamToAnimal;
    myvector[8] = AnimalToCam;
    myvector[9] = DistToCam;
    //Captures[capturecount]=myvector;
    //capturecount+=1;
    Captures << Individual_ID <<","
             << Sensor_StepOn <<","
             << Sensor_identifier <<","
             << itnumber << ","
             << location_x_animal <<","
             << location_y_animal <<","
             << time <<","
             << CamToAnimal <<","
             <<AnimalToCam <<","
            << DistToCam
            << "\n";
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
    //std::vector <double> Coord(2);
    double YCoordinate = m_Angle*Vert +c_Angle;
    return(YCoordinate);
};

double Sensor::HorzAndAngleInteraction(double Horz, double m_Angle, double c_Angle){
    // y=mx+c
    // => x = (y-c) /m
    // std::cout<<" Horz: " <<Horz << " c_Angle: " <<c_Angle << " m_Angle: " << m_Angle<<std::endl;
    double XCoordinate = (Horz - c_Angle)/m_Angle;
    return(XCoordinate);
};

std::vector <double> Sensor::AngleAndAngleInteraction(double m1_Angle, double c1_Angle, double m2_Angle, double c2_Angle){
    // y=mx+c =>
    // m1*x +c1 = m2*x+c
    // (m1-m2)x = (c2-c1)
    // x = (c2-c1)/(m1-m2)
    //if(Sensor_identifier==209){std::cout<<"hello - inside angle/angle" <<std::endl;}
    
    double Coordinate1 = (c2_Angle - c1_Angle)/(m1_Angle-m2_Angle);
    //if(Sensor_identifier==209){std::cout<<"Coordinate1 "<<Coordinate1  <<std::endl;}

    double Coordinate2 = m1_Angle*Coordinate1 +c1_Angle;
    //if(Sensor_identifier==209){std::cout<<"Coordinate2 "<<Coordinate2  <<std::endl;}
    
    std::vector <double> Coord(4);
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
    double Coordinate1 = sqrt(pow(radius,2) - pow((Vert - location_x),2))+ location_y;
    double Coordinate2 = -sqrt(pow(radius,2) - pow((Vert - location_x),2))+ location_y;
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
    double circ_term1 = pow(m_Angle,2)+1;
    double temp_A =  location_y - c_Angle;
    double circ_term2 = -(m_Angle*temp_A +location_x)*2;
    double circ_term3 = pow(temp_A,2)+pow(location_x,2) - pow(radius,2);
    
    // The Quadractic formula can be used to solve: (m^2+1)X^2 - 2(Am+b)X +(b^2 +A^2 - r^2) = 0
    // for X and hence for the coordinate of the intercept between circle and line
    // Quaddractic formula:
    //          (-b+-sqrt(b^2-4ac))/2a
    // if the term  (b^2-4ac) is less than zero then there is no iterception between line and circle
    // and there are no solutions
    // can skip the next parts
    double temp = pow(circ_term2,2)-(4*circ_term1*circ_term3);
    
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
void Sensor::CapturesIntersection(double location_x_animal, double location_y_animal,   // Current locations
                                  double previous_x_animal, double previous_y_animal,   // Previous locations
                                  int Individual_ID,                                    // Animal ID
                                  double move_angle,                                    // Call direction & width
                                  int itnumber,                                         // Iteration number
                                  std::ofstream &Captures){
    
    double currentlocx =location_x_animal;
    double currentlocy =location_y_animal;
    double previouslocx =previous_x_animal;
    double previouslocy =previous_y_animal;
    double move_angle_ref = move_angle;
    int Individual_ID_ref = Individual_ID;
    
    double disttotal = DistTwoPoints(previouslocx,currentlocx,previouslocy,currentlocy);
    
    //If the animal movement was a line on a graph with a gradient and a intercept, Y=mX+c, then:
    //  - gadient would be, m=(change x/change y)
    //  - intercpet would be: y-mx=c (where y and x are known)
    double m_animal  = GradientFromAngle(move_angle_ref);
    double c_animal  = currentlocy-currentlocx*m_animal;
    
    // Checks the beginning of the step
    CapturesIndividual(previouslocx,previouslocy,
                                   Individual_ID,
                                   move_angle,
                                   itnumber,
                                   0,
                                   0,
                                   Captures
                                   );
    
    if(angle_HalfWidth < M_PI){ // If the Sensor width is 360˚ then the straight line edges are not important
        // Checks for crossing the boundaries for detector 1
       SensorAndMovement(currentlocx, currentlocy,
                                  previouslocx,previouslocy,
                                  Individual_ID_ref,
                                   move_angle_ref,
                                  itnumber,
                                  m_animal,c_animal,
                                  m_detector1, c_detector1, g_detector1, vh_det1,
                                  disttotal,
                                 Captures);
        // Checks for crossing the boundaries for detector 2
       SensorAndMovement(currentlocx,currentlocy,
                                  previouslocx ,previouslocy,
                                  Individual_ID_ref,
                                  move_angle_ref,
                                  itnumber,
                                  m_animal,c_animal,
                                  m_detector2, c_detector2,g_detector2,vh_det2,
                                      disttotal,
                                      Captures);
    };
    // Checks for crossing the boundaries for circular edge of detector
    SensorCircAndMovement(currentlocx,currentlocy,
                                      previouslocx ,previouslocy,
                                      Individual_ID_ref,
                                       move_angle_ref,
                                      itnumber,
                                      m_animal, c_animal,
                                      disttotal,
                                      Captures);
    // Checks for crossing the boundaries for circular edge of detector
    CapturesIndividual(currentlocx,currentlocy,
                                   Individual_ID,
                                   move_angle,
                                   itnumber,
                                   1,
                                   0,
                                   Captures);
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

void Sensor::SensorCircAndMovement(double location_x_animal, double location_y_animal,   // Current location
                                  double previous_x_animal, double previous_y_animal,       // Previous location
                                  int Individual_ID,                                        // Animal ID
                                   double move_angle,                 // Direction and angle of call
                                  int itnumber,                                             // Iteration number
                                  double m_animal, double c_animal,                         // Gradient/intercept of animal
                                  double disttotal,
                                  std::ofstream &Captures){                                        // Total distance moved
    
    //std::cout<<"SensorCircAndMovement " << "move_angle "<< move_angle<<std::endl;
    
    
    // Initial a variable for the total number of times the animal is captured on the circle part of the detector    
    // TandA is for the Time and Angle of the intercept value
    // XandY are the 2 solutions to where the line passes through the circle: (x1,y1,x2,y2)
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
    // if(Individual_ID== 0 && Sensor_StepOn>17654 && Sensor_StepOn<17656){
        std::cout<<"XandY[0]: "<<XandY[0]<<", XandY[1]: "<<XandY[1]<<std::endl;
        std::cout<<"XandY[2]: "<<XandY[2]<<", XandY[3]: "<<XandY[3]<<std::endl;
     //};
     //----------------------------------------------------------------------------------------------------------*/
    
    //--------- CHECK IN "CapturesIndividual" FOR CAPTURE ---------------//
    //Looking at both possible solutions for the line and the circle
    //For each sucessful capture adds a 1 to the "captured" value
    for(int v=0; v<2; v++){ 
        //time and angle of the interscept
        TandA = TimeAndAngleCal(XandY[(v*2)+1], XandY[v*2], previous_y_animal, previous_x_animal, disttotal);
        
        double time = TandA[0]; double angle = TandA[1];
        // The time has to be less than the total distance away form starting location
        // the angle of movement from the start location to the capture location has to be the same as the movement angle.
        // It the start location is captured then the time and angle are both zero
        if(((time<=1 || approximatelyequal(time,1)) && approximatelyequal(angle,move_angle))
           ||(time==0 && angle==0)){
            //std::cout<<"V: "<< v <<std::endl;
            CapturesIndividual(XandY[v*2], XandY[(v*2)+1],
                                           Individual_ID,
                                           move_angle,
                                           itnumber,
                                           time,
                                           0,
                                           Captures);
        }; // END OF IF LOOP
    }; // END OF FOR v loop
    //--------- END OF CHECK FOR CAPTURE ---------------//
    
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
void Sensor::SensorAndMovement(double location_x_animal, double location_y_animal,
                                  double previous_x_animal, double previous_y_animal,
                                  int Individual_ID,
                                   double move_angle,
                                  int itnumber,
                                  double m_animal,double c_animal,
                                  double m_detector, double c_detector,double g_detector, int det,
                                  double disttotal,std::ofstream &Captures){
    
    //if(Sensor_identifier==209){std::cout<<"SensorAndMovement, m_detector: " << m_detector<<" m_animal: " << m_animal <<std::endl;}
    double currentlocx =location_x_animal;
    double currentlocy =location_y_animal;
    double previouslocx =previous_x_animal;
    double previouslocy =previous_y_animal;
    
    // Initates a count for the number of captures
    // TandA is Time and Angle
    // XandY is an initialised vector for the possible coordinates of capture
    
    std::vector<double> renameXandY(4);
    double X1, X2, Y1, Y2;
    
    std::vector<double> renameTandA(4);

    // Assuming that there is one possible interaction for 2 lines
    // If the lines are exactly on top of each other then the "capture" occurs
    // at the earliest opportunity, for this we need to initialise values for
    // the min and maximum values of the detector lines
    double mindect;
    double maxdect;
    
    //if(Sensor_identifier==209){std::cout<<"Hello"<<std::endl;}

    
    //Animal has vertical movement
    if(currentlocx == previouslocx){
        //if(Sensor_identifier==209){std::cout<<"currentlocx == previouslocx "<<std::endl;}
        //Horzontal detector - then the solution is simply the y coord from horzontal line and the x coord from the vertical line
        if(det==2){ X1 = previous_x_animal; X2 = location_y; } // END IF DET==2
        // Vertical detector it HAS to have the same x-position as the movement
        // Then the x location is simply the value from both vertical lines
        // The y location is earliest location that the two lines overlap.
        else if(det==1 && location_x==previouslocx){
            renameXandY[0] = previous_x_animal;
            // If the detector is facing up then the min-value = Sensor_loc and the max-value = Sensor_loc+radius
            // otherwise the the min-value = Sensor_loc-radius and the max-value = Sensor_loc
            if(approximatelyequal(g_detector,M_PI)){mindect = location_x-radius; maxdect = location_x;}
            else{mindect=location_x; maxdect=location_x+radius;};
            // If the animal is moving up then I want the minumum value which both detector and movement has:
            // There for I want the max. value of the min. movement and min. detector
            // If the animal is moving down then I want the maxumum value which both detector and movement has:
            // There for I want the min. value of the max. movement and min. detector
            // => Animal moving down
            if(approximatelyequal(move_angle,M_PI)){renameXandY[1] = std::min(previouslocy,maxdect);}
            // => Animal moving up
            else{renameXandY[1] = std::max(previouslocy,mindect);};
        } // END IF DET==1
        // Detector at angle
        // The solution is the vertical line, and the y-coord can be calulated using *VertAndAngleInteraction*
        else{renameXandY[0] = currentlocx; renameXandY[1] = VertAndAngleInteraction(currentlocx, m_detector, c_detector);};// END ELSE
    }//--------END OF VERTICAL ANIMAL MOVEMENT ---------------//
    
    //Animal has horizontal movement
    else if(currentlocy == previouslocy){
        //if(Sensor_identifier==209){std::cout<<"currentlocy == previouslocy "<<std::endl;}
        //Horzontal detector
        // Therefore they must both share the same y-coord
        if(det==2 && location_y== previouslocy){
            // The y-coord is simly the value of the y-coord of the animal movement
            renameXandY[1] = previouslocy;
            // I need to idenfy the max. and min. x-value of the detector 
            if(approximatelyequal(g_detector,M_PI/2)){mindect=location_y; maxdect=location_y+radius;}
            else{mindect = location_y-radius; maxdect = location_y;};
            // if the movement is to the left then I want the max. of the min. animal location and the min. detector location
            // if the movement is the right then I need the min. of the max. animal location and the max. detector location
            if(approximatelyequal(move_angle,M_PI/2)){renameXandY[0] = std::max(previouslocx,mindect);}
            else{renameXandY[0] = std::min(previouslocx,maxdect);};
        } // END OF IF DET ==2
        // Vertical detector
        // The x-coord of the intercept is the x-coord of the detector, the y-coord of the movement
        else if(det==1){renameXandY[0] = location_x; renameXandY[1] = previouslocy;} // END OF IF DET ==1
        // Detector at angle
        // The y-coord of the movement, and the x-coord is calculated by HorzAndAngleInteraction
        else { renameXandY[0] = HorzAndAngleInteraction(currentlocy, m_detector, c_detector); renameXandY[1] = currentlocy;};// END ELSE
    }//--------END OF HORZONTAL ANIMAL MOVEMENT ---------------//
    
    
    // If the angle of the movement and the detector is the same, then they have to have the same intercept
    else if(approximatelyequal(m_detector,m_animal) && approximatelyequal(c_detector,c_animal)){
        //if(Sensor_identifier==209){std::cout<<"same line"<<std::endl;}
        // Calculation of x location
        // The min./max. value of the detector is calculated
        if(g_detector<M_PI){mindect=location_x; maxdect=location_x+radius*sin(g_detector);}
        else{mindect = location_x+radius*sin(g_detector); maxdect = location_x;};
        // if the movement is to the left then I want the max. of the min. animal location and the min. detector location
        // if the movement is the right then I need the min. of the max. animal location and the max. detector location
        if(move_angle<M_PI){renameXandY[0] = std::max(previouslocx,mindect);}
        else{renameXandY[0] = std::min(previouslocx,maxdect);};
        
        // Calculation of y location
        // The min./max. value of the detector is calculated
        if(g_detector<M_PI/2 ||g_detector>3*M_PI/2 ){mindect=location_y; maxdect=location_y+radius*cos(g_detector);}
        else{mindect = location_y+radius*cos(g_detector); maxdect = location_y;};
        // if the movement is up then I want the max. of the min. animal location and the min. detector location
        // if the movement is down then I need the min. of the max. animal location and the max. detector location
        if(move_angle<M_PI/2 ||move_angle>3*M_PI/2 ){ renameXandY[1] = std::max(previouslocy,mindect);}
        else{renameXandY[1] = std::min(previouslocy,maxdect);};
    }//--------END OF SAME ANGLE ANIMAL MOVEMENT ---------------//
    
    else if(approximatelyequal(m_detector,m_animal) && c_detector!=c_animal){
        // Calculation of x location
        // The min./max. value of the detector is calculated
        renameXandY[0] = NAN;
        renameXandY[1] = NAN;
    }
    
    else if(m_detector != m_animal){
        //Horzontal detector
        if(det==2){ renameXandY[0] = HorzAndAngleInteraction(location_y, m_animal, c_animal); renameXandY[1] = location_y;} //END OF IF DET==2
        // Vertical detector
        else if(det==1){renameXandY[0] = location_x; renameXandY[1] = VertAndAngleInteraction(location_x, m_animal, c_animal);} //END OF IF DET==1
        // Angle detctor
        else{
            renameXandY =AngleAndAngleInteraction(m_detector, c_detector, m_animal, c_animal);
        };
        
    }//--------END OF DIFFERENT ANGLE ANIMAL MOVEMENT ---------------//
    else{
        std::cout<<"Something is very wrong with Sensor::SensorAndMovement " << "gradient of detector: " <<m_detector <<"gradient of animal: "<< m_animal << std::endl;
        exit (EXIT_FAILURE);
    };
    
    //std::cout<<"x&y cal"<<std::endl;
    //--------- CHECK IN "CapturesIndividual" FOR CAPTURE ---------------//
    // Inputting into the capture inidvidual function
    //Calucaltes time and angle of the interscept
    renameTandA = TimeAndAngleCal(renameXandY[1], renameXandY[0], previouslocy, previouslocx, disttotal);
    double time = renameTandA[0]; double angle = renameTandA[1];

    
    // The time has to be less than the total distance away form starting location
    // the angle of movement from the start location to the capture location has to be the same as the movement angle.
    if((time<=1 && approximatelyequal(angle,move_angle))||(time==0 && angle==0)){
       CapturesIndividual(time,  angle, Individual_ID, move_angle, itnumber,time, 0, Captures);
    }; //--------- END OF CAPTURE CHECK ---------------//

}; // END OF FUNCTION




//-----------------------------------------------------------
// Captures individual
// Are the possible locations of with the Sensor radius?
// Are the possible locations within the width of the Sensor?
// Is the Sensor within the width of the Animal call?
//-----------------------------------------------------------
void Sensor::CapturesIndividual(double location_x_animal,
                                   double location_y_animal,
                                   int Individual_ID,
                                   double move_angle,
                                   int itnumber,
                                   double time,
                                   int call,
                                    std::ofstream &Captures
                                   ){
    //if(Individual_ID== 471 && Sensor_StepOn>212 && Sensor_StepOn<214){std::cout<<"InCap, radius" << radius <<std::endl;//};
    double &currentlocx =location_x_animal;
    double &currentlocy =location_y_animal;

    int captured=0;
    
    // If on the exact same spot as the Sensor assume it will be captured
    if(approximatelyequal(currentlocx,location_x) && approximatelyequal(currentlocy,location_y)){
        double zero =0;
        UpdateCaptures(Individual_ID,itnumber,currentlocx,currentlocy,time,call,zero,zero,zero,Captures);
        captured = 1;
    }
    // If not at the exact same location then check whether it is
    else{
        // Calculates the distance between the Sensor and the animal
        double diff_animal_Sensor = DistTwoPoints(currentlocx,location_x, currentlocy, location_y);
        
        /*---------------------------------------------------------------------------------------------------------
         // Check for specific case of interest
         //if(Individual_ID== 0 && Sensor_StepOn>17654 && Sensor_StepOn<17656){
            std::cout<<"Distance between animal & Sensor: "<<diff_animal_Sensor <<", Radius: "<<radius <<std::endl;
         //};
        // ----------------------------------------------------------------------------------------------------------*/
        
        // If the animal is less than a radius distance away from the detector then it is enters the if statement
        if(approximatelyequal(diff_animal_Sensor,radius)|| diff_animal_Sensor<=radius){
               
            // If in range, is it in the angle?
            // Can use the AngleTwoPoints to calculates the angle 
            // FROM Sensor TO the animal
            double AngleFromSensor = AngleTwoPoints(location_x, currentlocx, location_y, currentlocy);
            
            double AngleFromSensorCentre = AngleFromSensor - angle; // the minus angle bit is to work out the angle from the centre
            AngleFromSensorCentre = RangeAngle(AngleFromSensorCentre);
            // If the Sensor is a circle
            if(angle_HalfWidth == M_PI){
                //if(Sensor_identifier==0){std::cout<<"ello"<<std::endl;}
                double movementcorrection;
                if(move_angle>M_PI){movementcorrection=move_angle-2*M_PI;}else{movementcorrection=move_angle;};
                double AngleFromAnimalCentre = RangeAngle(AngleFromSensor+M_PI - movementcorrection);
                AngleFromAnimalCentre = RangeAngleMinus(AngleFromAnimalCentre);
                //AngleFromAnimalCentre = RangeAngle(AngleFromAnimalCentre);
                UpdateCaptures(Individual_ID,itnumber,currentlocx,currentlocy,time,call,AngleFromSensorCentre,AngleFromAnimalCentre,diff_animal_Sensor,Captures);
            };
        
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
                double AngleFromAnimalCentre = AngleFromAnimal - move_angle; // to calculate from animal centre
                AngleFromAnimalCentre = RangeAngleMinus(AngleFromAnimalCentre);
                // initates values for min and max call angles

                
                // If the animal is identifiable regardless of its direction (we say if has a 360 call)
                // in this case we can say it's captures as it is in the sensor range

                // Call the update captures function and let captured ==1.
                UpdateCaptures(Individual_ID,itnumber,currentlocx,currentlocy,time,call,AngleFromSensorCentre, AngleFromAnimalCentre,diff_animal_Sensor, Captures);
            }; //End of IF - "in width of detector"
        }; //End of "radius" IF
    }; //END of else - not directly on same spot as Sensor
    
}; // End of function