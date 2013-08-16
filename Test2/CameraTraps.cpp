//
//  CameraTraps.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 19/04/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include "CameraTraps.h"
#include "Parameters.h"
#include "RandNum.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <algorithm> 

CameraTrap::CameraTrap(){};
CameraTrap::CameraTrap(int a//CT_identifier;
                       ,double d//radius;
                       ,double RadiusCameraCircle
                       ,double AngleBetweenCameras
                       //,std::vector<std::vector<int>> //::Captures()
                       ){
    
    // Renames variables
    time1=0;
    //std::cout<<DetectorLayOut<<std::endl;
    
    //////////////////////////
    /// Location of camera ///
    //////////////////////////
    if(DetectorLayOut ==0){ //Single sationary detector
        location_x = (Sq_MaxX-Sq_MinX)/2;
        location_y = (Sq_MaxX-Sq_MinX)/2;
        angle = 0;
        CT_StepOn = a + NoRunIn;
    } //End OF Single sationary detector
    else if(DetectorLayOut ==2){ //Camera transect
        // Using polar to cartesian co-ordinates
        // This means that the cameras start at 3o'clock and move in a anti-clockwise direction
        location_x = (RadiusCameraCircle  * cos(a*AngleBetweenCameras)) + Cir_CntX ;
        location_y = (RadiusCameraCircle  * sin(a*AngleBetweenCameras)) + Cir_CntY ;
    
        // The angle (with respect to "north") the camera is facing is:
        // 90 degrees minus the number of degrees between the start and the current camera
        // because of the anti-clockwise motion
        // After passes zero - then
        angle = M_PI/2 - a*AngleBetweenCameras;
        if(angle<0){angle = 2*M_PI + angle;} else if(angle>=2*M_PI){angle = angle - 2*M_PI;};
        CT_StepOn = a + NoRunIn;
    }// End OF Camera transect
    //  Cameras in a Grid formation
    else if(DetectorLayOut ==1){
        double temp =floor((a+1)/MaxNoX);
        location_x = (((a+1)/MaxNoX)-temp)*MaxNoX*Xspace + Xgridmin;
        location_y = temp*Yspace + Ygridmin;
        //std::cout<<temp<<std::endl;
        angle = 0;
        if(angle<0){angle = 2*M_PI + angle;} else if(angle>=2*M_PI){angle = angle - 2*M_PI;};
        CT_StepOn = 0 + NoRunIn; // The cameras are on at all steps!!
    };
    
    
    
    // Detector 1 is line represneting the left hand slide of the detector range
    // Detector 2 is line represnting the RHS of the detector range
    // If the lines of the detcetor were lines of graphs with garident and intercepts, Y=mX+c, then:
    //  - gradient if theta is the ange of the line then, m=1/tan(theta)
    //  - intercpet would be: y-mx=c (where y and x are known)
    double detector1 = angle-angle_HalfWidth;
    double detector2 = angle+angle_HalfWidth;
    if(detector1<0){detector1 = 2*M_PI + detector1;} else if(detector1>=2*M_PI){detector1 = detector1 - 2*M_PI;};
    if(detector2<0){detector1 = 2*M_PI + detector2;} else if(detector2>=2*M_PI){detector2 = detector2 - 2*M_PI;};
    
    m_detector1 = GradientFromAngle(detector1);
    m_detector2 = GradientFromAngle(detector2);
    c_detector1  = location_y - location_x*m_detector1;
    c_detector2  = location_y - location_x*m_detector2;
    
    int vh_det1 =0;
    int vh_det2 =0;
    
    if(detector1== 0 || detector1== M_PI|| detector1== 2*M_PI){vh_det1 = 1;} //Line is vertical
    else if(detector1 == M_PI/2 || detector1== 3*M_PI/2){vh_det1 = 2;} //Line is Horizontal
    
    if(detector2== 0 || detector2== M_PI|| detector2== 2*M_PI){vh_det2 = 1;} //Line is vertical
    else if(detector2 == M_PI/2 || detector2== 3*M_PI/2){vh_det2 = 2;} //Line is Horizontal
    
    
    //Assigning varaibles
    CT_identifier = a;
    radius =d;
    angle_HalfWidth = CameraWidth;
    Captures.resize(round(DensityAnimals*((Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY)))*3);
    capturecount=0;
    myvector.resize(7);
    //std::cout<<"END create"<< std::endl;
};








//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------------------------------------
//
//                                                SETS                                                    
//
--------------------------------------------------------------------------------------------------------*/


void CameraTrap::Add1StepOn(){CT_StepOn=CT_StepOn+1;};
void CameraTrap::ResetStepOn(){CT_StepOn=0;};
void CameraTrap::resetCaptures(){
    capturecount =0;
    Captures.clear();
    Captures.resize(round(DensityAnimals*((Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY)))*3);
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------------------------------------
                                        Temporary function things
 
--------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------
 // Some times they values are are the same to 4/5dp
 // But the code doesn't recognise thus
 // This function produces a 1 if the difference is 
 // less than 0.000001 and 0 otherwise.
 // Therefore if two values are approximately equal 
 // then the 1 otherwise 0.
----------------------------------------------*/

int approximatelyequal(double a, double b){
    int r;
    if(sqrt(pow(a-b,2))<(1*pow(10,-6))){r=1;}else{r=0;}
    return (r);
}

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

double VertAndAngleInteraction(double Vert, double m_Angle, double c_Angle){
    //y=mx+c
    double YCoordinate = m_Angle*Vert +c_Angle;
    return(YCoordinate);
}

double HorzAndAngleInteraction(double Horz, double m_Angle, double c_Angle){
    // y=mx+c => x = (y-c) /m
    double XCoordinate = (Horz - c_Angle)/m_Angle;
    return(XCoordinate);
}

std::vector <double> AngleAndAngleInteraction(double m1_Angle, double c1_Angle, double m2_Angle, double c2_Angle){
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
}


std::vector <double> HorzAndCircInteraction(double Horz, double y_centre, double x_centre, double radius){
    // (y-a)^2+ (x-b)^2 = r^2
    // =>
    //  (x-b)^2 = r^2 - (y-a)^2
    // =>
    // x = (+/-)sqrt (r^2 - (y-a)^2) +b
    std::vector <double> Coord(2);
    double Coordinate1 = sqrt(pow(radius,2) - pow((Horz - y_centre),2))+ x_centre;
    double Coordinate2 =  - sqrt(pow(radius,2) - pow((Horz - y_centre),2))+ x_centre;
    Coord[0] = Coordinate1;
    Coord[1] = Coordinate2;
    return(Coord);
}

std::vector <double> VertAndCircInteraction(double Vert, double y_centre, double x_centre, double radius){
    // (y-a)^2+ (x-b)^2 = r^2
    // =>
    //  (y-a)^2 = r^2 - (x-b)^2
    // =>
    // y = (+/-)sqrt (r^2 - (x-b)^2) +a
    std::vector <double> Coord(2);
    double Coordinate1 = sqrt(pow(radius,2) - pow((Vert - x_centre),2))+ y_centre;
    double Coordinate2 = -sqrt(pow(radius,2) - pow((Vert - x_centre),2))+ y_centre;
    Coord[0] = Coordinate1;
    Coord[1] = Coordinate2;
    return(Coord);
};

std::vector <double> AngleAndCircInteraction(double m_Angle, double c_Angle, double location_y, double location_x, double radius){
    
    std::vector <double> Coord(4);

    //If the line of the animal movement cross the circle part of the camera circle
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
    // Second term:  -2* (Grad.of.animal*(intercept.of.animal - y.location.of.camera)+ x.location.of.camera)
    // Third term: x.location.of.camera^2 + (intercept.of.animal - y.location.of.camera)^2 +radius^2
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
    /*
    std::cout<< "circ_term1: "<< circ_term1<<std::endl;
    std::cout<< "circ_term2: "<< circ_term2<<std::endl;
    std::cout<< "circ_term3: "<< circ_term3<<std::endl;
    std::cout<< "InSqRt: "<< temp<<std::endl;
    */
    /*
     if((CT_StepOn >= 195 && CT_StepOn <=196)
     && Individual_ID == 3){std::cout<<"Temp "<<temp<<std::endl;}*/
    if(temp>0 || approximatelyequal(temp,0)==1){
        if(approximatelyequal(temp,0)==1){temp=0;} // This needs to be included for tangents
        
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
std::vector <double> TimeAndAngleCal(double Y, double X, double previous_y_animal, double previous_x_animal, double disttotal){
    
    std::vector <double> returnvalues(4);
    double diffy =(Y-previous_y_animal);
    double diffx =(X-previous_x_animal);
    double distedge = sqrt(pow(diffx,2)+ pow(diffy,2));
    double time = distedge/disttotal;
    double AngleBatCap;
    /*std::cout<<"previous_y_animal: "<<previous_y_animal <<", previous_x_animal: "<< previous_x_animal<< std::endl;
    std::cout<<"Y: "<<Y<<", X: "<< X<< std::endl;
    std::cout<<"diffy: "<<diffy <<", diffx: "<< diffx<< ", distedge: "<< distedge <<std::endl;*/

    AngleBatCap = atan((diffx)/(diffy));
    //std::cout<<"AngleBatCap : "<<AngleBatCap << std::endl;
    if(diffy<0){AngleBatCap+=M_PI;}//End if
    if(AngleBatCap <0){AngleBatCap += 2*M_PI;}
    else if(AngleBatCap>2*M_PI){ AngleBatCap -= 2*M_PI;};
    
    if((approximatelyequal(Y,previous_y_animal)==1 && approximatelyequal(X,previous_x_animal)==1)
       ||approximatelyequal(AngleBatCap,2*M_PI)==1){AngleBatCap=0;};
    if(approximatelyequal(time,0)==1){time=0;};
    
    returnvalues[0] = time;
    returnvalues[1] = AngleBatCap;
    return(returnvalues);
};


double CameraTrap::GradientFromAngle(double angle){
    // The Gradient is calculated as = delta(y)/delta(x) 
    //  tan(angle) = Opposite/Adjacent = delta (x)/delta(y)
    // therefore gradient = 1/tan(angle)
    double Gradient = 1/tan(angle);
    return(Gradient);
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------------------------------------
                                            Capturing of the animal
 
// INPUTS INTO... *CapturesIntersection*
//     - Calculates the total distance moved
//     - Calculates the gradient of the movement
//     - Calculates the gradient of the detector
//     - Enters gradient of movement and detctor edge into * CameraAndMovement * 
//       to calulate intercepts location
//          => which are entered into * CapturesIndividual * to calulate if in camera detection zone
//     - Enters end location into * CapturesIndividual * to calulate if in camera detection zone
//
//
// => Captures individual
//         * Are the possible locations of with the camera radius?
//         * Are the possible locations within the width of the camera?
//         * Is the camera within the width of the bat call?
 
 --------------------------------------------------------------------------------------------------------*/

///////////////////////
///////////////////////
int CameraTrap::CapturesIntersection(double location_x_animal,
                                     double location_y_animal,
                                     double previous_x_animal,
                                     double previous_y_animal,
                                     int Individual_ID,
                                     double call_halfwidth,
                                     double move_angle,
                                     int itnumber){
    int captured = 0;
    
    // finds the total distance travelled between it's new location and it's old location
    double disttotal = sqrt(pow(previous_x_animal-location_x_animal,2)+
                            pow(previous_y_animal-location_y_animal,2));
    
    
    //If the animal movement was a line on a graph with a gradient and a intercept, Y=mX+c, then:
    //  - gadient would be, m=(change x/change y)
    //  - intercpet would be: y-mx=c (where y and x are known)
    double m_animal  = GradientFromAngle(move_angle);
    double c_animal  = location_y_animal-location_x_animal*m_animal;
    //std::cout<<"m_animal: "<< m_animal<<std::endl;
    //std::cout<<"c_animal: "<< c_animal<<std::endl;
    
    // Checks for crossing the boundaries for detector 1
    //std::cout<< "Dect1"<<std::endl;
    //std::cout<< captured<<std::endl;
    captured += CameraAndMovement(location_x_animal,
                                  location_y_animal,
                                  previous_x_animal,
                                  previous_y_animal,
                                  Individual_ID,
                                  call_halfwidth,
                                  move_angle,
                                  itnumber,
                                  m_animal,
                                  c_animal,
                                  m_detector1,
                                  c_detector1,
                                  vh_det1,
                                  disttotal);
    //std::cout<< captured<<std::endl;
    
    // Checks for crossing the boundaries for detector 2
    //std::cout<< "Dect2"<<std::endl;
    captured += CameraAndMovement(location_x_animal,
                                  location_y_animal,
                                  previous_x_animal,
                                  previous_y_animal,
                                  Individual_ID,
                                  call_halfwidth,
                                  move_angle,
                                  itnumber,
                                  m_animal,
                                  c_animal,
                                  m_detector2,
                                  c_detector2,
                                  vh_det2,
                                  disttotal);
    
    // Checks for crossing the boundaries for circular edge of detector
    //std::cout<< "Circ"<<std::endl;
    //std::cout<< captured<<std::endl;
    captured += CameraCircAndMovement(location_x_animal,
                                      location_y_animal,
                                      previous_x_animal,
                                      previous_y_animal,
                                      Individual_ID,
                                      call_halfwidth,
                                      move_angle,
                                      itnumber,
                                      m_animal,
                                      c_animal,
                                      disttotal);
    //std::cout<< captured<<std::endl;
    
    // Checks for at the end of the step if in/out of detection area
    //std::cout<< "End"<<std::endl;
    //std::cout<< captured<<std::endl;
    captured += CapturesIndividual(location_x_animal,
                                   location_y_animal,
                                   Individual_ID,
                                   call_halfwidth,
                                   move_angle,
                                   itnumber,
                                   1); // Assumes that it captures right at the end of the movement therefore time=1
    //std::cout<< captured<<std::endl;
    // If there are any captures then return 1 else return 0
    if(captured>=1){return(1);} else {return(0);};
};
////////////////////
////////////////////


// Is the camera lines at a angles and is the movement also at an angle?
// If so then this should
// NEEDS TEST
int CameraTrap::CameraCircAndMovement(double location_x_animal,
                                  double location_y_animal,
                                  double previous_x_animal,
                                  double previous_y_animal,
                                  int Individual_ID,
                                  double call_halfwidth,
                                  double move_angle,
                                  int itnumber,
                                  double m_animal,
                                  double c_animal,
                                  double disttotal){
    
    int captured =0;
    // TandA is Time and Angle
    std::vector<double> TandA(2);
    std::vector<double> Estimate(2);
    std::vector<double> XandY(4); // There are 2 solutions to where the line passes through the circle: (x1,y1,x2,y2)
    
    //Animal has vertical movement
    if(location_x_animal == previous_x_animal){
        //std::cout<<"Circ, Vert move"<<std::endl;
        Estimate = VertAndCircInteraction(location_x_animal, location_y, location_x, radius);
        XandY[1] = Estimate[0]; XandY[0] = location_x_animal;
        XandY[3] = Estimate[1]; XandY[2] = location_x_animal;

   }//END OF VERTICAL ANIMAL MOVEMENT
    
    //Animal has horizontal movement
    else if(location_y_animal == previous_y_animal){
        //std::cout<<"HorzAndCircInteraction"<<std::endl;
        Estimate = HorzAndCircInteraction(location_y_animal, location_y, location_x, radius);
        XandY[0] = Estimate[0]; XandY[1] = location_y_animal;
        XandY[2] = Estimate[1]; XandY[3] = location_y_animal;
    }//END OF HORZONTAL ANIMAL MOVEMENT
    else{
        //std::cout<<"AngleAndCircInteraction"<<std::endl;
        XandY = AngleAndCircInteraction(m_animal, c_animal, location_y, location_x, radius);

    }//END OF DIFFERENT ANGLE ANIMAL MOVEMENT 
    
    //std::cout<<"XandY[0]: "<<XandY[0]<<", XandY[1]: "<<XandY[1]<<std::endl;
    //std::cout<<"XandY[2]: "<<XandY[2]<<", XandY[3]: "<<XandY[3]<<std::endl;
    //Looking at both possible solutions for the line and the circle
    //For each sucessful capture adds a 1 to the "captured" value
    for(int v=0; v<2; v++){
        //std::cout<<"V"<< v<<std::endl;
        //time and angle of the interscept
        TandA = TimeAndAngleCal(XandY[(v*2)+1], XandY[v*2], previous_y_animal, previous_x_animal, disttotal);
        //std::cout<<"TandA [0]: "<<TandA[0]<<", TandA[1]: "<<TandA[1]<<std::endl;
        // The time has to be less than the total distance away form starting location
        // the angle of movement from the start location to the capture location has to be the same as the movement angle.
        //if(TandA[0]<=1){std::cout<<"Time okay"<<std::endl;};
        //if( approximatelyequal(TandA[1],move_angle)==1){std::cout<<"Angle okay"<<std::endl;};
        if(((TandA[0]<=1 || approximatelyequal(TandA[0],1)==1) && approximatelyequal(TandA[1],move_angle)==1)
            ||(TandA[0]==0 && TandA[1]==0)){
            //std::cout<<"Circ move, CI"<<std::endl;
            //std::cout<<captured<<std::endl;
            captured += CapturesIndividual(XandY[v*2],
                                       XandY[(v*2)+1],
                                       Individual_ID,
                                       call_halfwidth,
                                       move_angle,
                                       itnumber,
                                       TandA[0]);}
            //std::cout<<captured<<std::endl;

    }
    return(captured);
};




////////////////////
////////////////////

//NEEDS TEST
// Is the camera lines at a angles and is the movement also at an angle?
int CameraTrap::CameraAndMovement(double location_x_animal,
                                  double location_y_animal,
                                  double previous_x_animal,
                                  double previous_y_animal,
                                  int Individual_ID,
                                  double call_halfwidth,
                                  double move_angle,
                                  int itnumber,
                                  double m_animal,
                                  double c_animal,
                                  double m_detector,
                                  double c_detector,
                                  int det,
                                  double disttotal){
    
    
    int captured =0;
    // TandA is Time and Angle 
    std::vector<double> TandA(2);
    std::vector<double> XandY(2);
    // For the min and maximum values of the detector lines when the line is vertical or horizonatl
    double mindect;
    double maxdect;
    
    //Animal has vertical movement
    if(location_x_animal == previous_x_animal){
        //std::cout<<"Line, Vert move"<<std::endl;
        //Horzontal detector
            if(det==2){
                XandY[0] = previous_x_animal;
                XandY[1] = location_y;
            } // END IF DET==2
        // Vertical detector AND x-position as the movement
        else if(det==1 && location_x== previous_x_animal){
            XandY[0] = previous_x_animal;
            if(approximatelyequal(m_detector,0)||approximatelyequal(m_detector,2*M_PI)){mindect=location_x; maxdect=location_x+radius;}
            else{mindect = location_x-radius; maxdect = location_x;};
            if(approximatelyequal(move_angle,0)||approximatelyequal(move_angle,2*M_PI)){
                XandY[1] = std::min(std::max(location_y_animal,previous_y_animal),maxdect);
            } else{XandY[1] = std::max(std::min(location_y_animal,previous_y_animal),mindect);}
            } // END IF DET==1
        // Detcteor at angle
        else{
            //std::cout<<"Line, Vert move, angle"<<std::endl;
            XandY[0] = location_x_animal;
            XandY[1] = VertAndAngleInteraction(location_x_animal, m_detector, c_detector);
            //std::cout<<"XandY[0]: "<<XandY[0]<<", XandY[1]: "<<XandY[1]<<std::endl;
        }// END ELSE
    }//--------END OF VERTICAL ANIMAL MOVEMENT ---------------//
    
    //Animal has horizontal movement
    else if(location_y_animal == previous_y_animal){
        //Horzontal detector AND y-position as the movement
        if(det==2 && location_y== previous_y_animal){
            //XandY[0] = location_x; // THIS IS WRONG
            XandY[1] = previous_y_animal; 
            if(approximatelyequal(move_angle,M_PI/2)){mindect=location_y; maxdect=location_y+radius;}
            else{mindect = location_y-radius; maxdect = location_y;};
            if(approximatelyequal(move_angle,0)||approximatelyequal(move_angle,2*M_PI)){
                XandY[0] = std::min(std::max(location_x_animal,previous_x_animal),maxdect);
            }else{XandY[0] = std::max(std::min(location_x_animal,previous_x_animal),mindect);};
        } // END OF IF DET ==2
        // Vertical detector 
        else if(det==1){
            XandY[0] = location_x;
            XandY[1] = previous_y_animal;
        } // END OF IF DET ==1
        // Detecteor at angle
        else {
            //std::cout<<"Line, Horz move, angle"<<std::endl;
            XandY[0] = HorzAndAngleInteraction(location_y_animal, m_detector, c_detector);
            XandY[1] = location_y_animal;
            //std::cout<<"XandY[0]: "<< XandY[0]<<", XandY[1]: "<< XandY[1] <<std::endl;
        }// END ELSE
    }//--------END OF HORZONTAL ANIMAL MOVEMENT ---------------//
    
    
    // If the angle of the movement and the detector is the same, then they have to have the same intercept
    else if(approximatelyequal(m_detector,m_animal)==1 && approximatelyequal(c_detector,c_animal)==1 ){
        std::cout<<"Line, Same angle"<<std::endl;

        if(m_detector<M_PI){mindect=location_x; maxdect=location_x+radius*sin(atan(1/m_detector));}
        else{mindect = location_x-radius*sin(atan(1/m_detector)); maxdect = location_x;};
        //std::cout<< m_detector<<", "<<sin(atan(1/m_detector))<<std::endl;
        //std::cout<<"mindect "<< mindect <<", maxdect "<< maxdect<<std::endl;
        
        if(move_angle<M_PI){
            XandY[0] = std::min(std::max(location_x_animal,previous_x_animal),mindect);
        }else{XandY[0] = std::max(std::min(location_x_animal,previous_x_animal),maxdect);};
        
        if(m_detector<M_PI/2 ||m_detector>3*M_PI/2 ){mindect=location_y; maxdect=location_y+radius*cos(atan(1/m_detector));}
        else{mindect = location_y-radius*cos(atan(1/m_detector)); maxdect = location_y;};
        //std::cout<<"mindect "<< mindect <<", maxdect "<< maxdect<<std::endl;
        if(move_angle<M_PI){
            XandY[1] = std::min(std::max(location_y_animal,previous_y_animal),mindect);
        }else{XandY[1] = std::max(std::min(location_y_animal,previous_y_animal),maxdect);};
        
        XandY[1] = location_y_animal;

        }//--------END OF SAME ANGLE ANIMAL MOVEMENT ---------------//
    
    else if(m_detector != m_animal){
        //std::cout<<"Line, Diff angle"<<std::endl;
        //Horzontal detector
        if(det==2){
            XandY[0] = HorzAndAngleInteraction(location_y, m_animal, c_animal);
            XandY[1] = location_y;
        } // END OF IF DET ==2
        // Vertical detector
        else if(det==1){
            XandY[0] = location_x;
            XandY[1] = VertAndAngleInteraction(location_x, m_animal, c_animal);
        } // END OF IF DET ==1
        else{
            std::cout<<"Line, Diff angle, dect angle"<<std::endl;
            XandY =AngleAndAngleInteraction(m_detector, c_detector, m_animal, c_animal);
            std::cout<<"XandY[0]: "<<XandY[0]<<", XandY[1]: "<< XandY[1]<<std::endl;
        }
        
    }//--------END OF DIFFERENT ANGLE ANIMAL MOVEMENT ---------------//
    else{
        std::cout<<" Something is very wrong with CameraTrap::CameraAndMovement " <<  std::endl;
        exit (EXIT_FAILURE);
    };
    
    //TandA = TimeAndAngleCal(XandY[1], XandY[0], previous_y_animal, previous_x_animal, disttotal);
    //time and angle of the interscept
    TandA = TimeAndAngleCal(XandY[1], XandY[0], previous_y_animal, previous_x_animal, disttotal);
    //std::cout<<"TandA[0]: "<< TandA[0] <<", TandA[1]: "<< TandA[1]<<", move_angle: " <<move_angle<< std::endl;
    // The time has to be less than the total distance away form starting location
    // the angle of movement from the start location to the capture location has to be the same as the movement angle.
    if((TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1)||(TandA[0]==0 && TandA[1]==0)){
        //std::cout<<"line"<<std::endl;
        captured += CapturesIndividual(XandY[0],
                                       XandY[1],
                                       Individual_ID,
                                       call_halfwidth,
                                       move_angle,
                                       itnumber,
                                       TandA[0]);}

    
//-------- Return ---------------//
return(captured);
};


// NEEDS TEST
void CameraTrap::UpdateCaptures(double Individual_ID,double itnumber,double location_x_animal,double location_y_animal,double time){
    myvector[0] = Individual_ID;
    myvector[1] = CT_StepOn;
    myvector[2] = CT_identifier;
    myvector[3] = itnumber;
    myvector[4] = location_x_animal;
    myvector[5] = location_y_animal;
    myvector[6] = time;
    //std::cout<<"HERE"<<std::endl;
    //std::cout<<"capturecount: "<<capturecount<<std::endl;
    Captures[capturecount]=myvector;
    capturecount+=1;
}

//-----------------------------------------------------------
// Captures individual
// Are the possible locations of with the camera radius?
// Are the possible locations within the width of the camera?
// Is the camera within the width of the bat call?
int CameraTrap::CapturesIndividual(double location_x_animal,
                                   double location_y_animal,
                                   int Individual_ID,
                                   double call_halfwidth,
                                   double move_angle,
                                   int itnumber,
                                   double time
                                   ){
    //std::cout<<"InCap"<<std::endl;
    clock_t Time1=clock();
    int captured=0;    
    double AngleFromCamera = 0;
    
    //distance to the camera
    double diffx = location_x_animal - location_x;
    double diffy = location_y_animal - location_y;
    
    if(approximatelyequal(diffx,0)==1 && approximatelyequal(diffy,0)==1){ // If on the exact same spot as the camera assume it will be captured
        UpdateCaptures(Individual_ID,itnumber,location_x_animal,location_y_animal,time);
        captured = 1;
    } else{
    // If not at the exact same location then check whether it is
    
    double diff_animal_camera = sqrt(pow(diffx, 2) + pow(diffy, 2));
        //std::cout<<diff_animal_camera <<std::endl;
        //std::cout<<radius <<std::endl;
    
    // Is the animal in the range of the camera?
        //if(diff_animal_camera > radius){std::cout<< diff_animal_camera - radius<<std::endl;} else
    if(approximatelyequal(diff_animal_camera,radius)==1|| diff_animal_camera<=radius){
        //std::cout<< "RADIUS"<<std::endl;
               
        // If in range, is it in the angle?
        // (atan calculates radians)
        // If the location is "south" of the camera then add pi becuase
        // atan will only give values between -90 and +90
        if(diffx==0 && diffy==0){AngleFromCamera =0;} else
        if(diffy>=0){AngleFromCamera = atan(diffx/diffy);}
        else        {AngleFromCamera = atan(diffx/diffy)+M_PI;};
        
        double Min_angle = angle-angle_HalfWidth;
        double Max_angle = angle+angle_HalfWidth;
        
        if(Max_angle >=2*M_PI){Max_angle = Max_angle -2*M_PI;};
        if(Min_angle <0){Min_angle = Min_angle +2*M_PI;};
        if(AngleFromCamera >=2*M_PI){AngleFromCamera = AngleFromCamera -2*M_PI;};
        if(AngleFromCamera <0){AngleFromCamera = AngleFromCamera +2*M_PI;};
        
        /*
        if(Individual_ID== 77 && CT_StepOn>769 && CT_StepOn<771){
        std::cout<< "location_x_animal: "<<location_x_animal<<std::endl;
        std::cout<< "location_y_animal: "<<location_y_animal<<std::endl;
        std::cout<< "Angle of camera: "<<angle<<std::endl;
        std::cout<< "Movement angle: "<<move_angle<<std::endl;
        std::cout<< "Min. camera angle: "<<Min_angle<<std::endl;
        std::cout<< "Max. camera angle: "<<Max_angle<<std::endl;
        std::cout<<"Angle from camera to bat: "<<AngleFromCamera <<std::endl;
        //std::cout<<AngleFromCamera-Max_angle <<std::endl;
         */
        
        
        // If the angle is between min and max possible angle
        if((Min_angle>Max_angle && AngleFromCamera <= Max_angle) || //IF the min value is less than zero
           (Min_angle>Max_angle && AngleFromCamera >= Min_angle) || //IF the max value is greater than 360
           (approximatelyequal(Min_angle, AngleFromCamera)==1)   ||
           (approximatelyequal(Max_angle, AngleFromCamera)==1)   ||
           (AngleFromCamera >= Min_angle && AngleFromCamera <= Max_angle)){
            //std::cout<< "IN CAM ANGEL"<<std::endl;
            
            //if(Individual_ID== 77 && CT_StepOn>769 && CT_StepOn<771){std::cout<<"In Camera angle"<<std::endl;}
            
            if(call_halfwidth==M_PI){
                //std::cout<< "360 call"<<std::endl;
                //std::cout<<diff_animal_camera<<std::endl;
                //std::cout<<radius<<std::endl;
                UpdateCaptures(Individual_ID,itnumber,location_x_animal,location_y_animal,time);
                captured = 1;
            }else{

            
            //Calcaulates the angle from the Bat to the detector as a bearing from north
            double AngleFromBat = AngleFromCamera+M_PI;
            if(AngleFromBat >=2*M_PI){AngleFromBat = AngleFromBat -2*M_PI;};
                        
            double Min_batangle = move_angle-call_halfwidth;
            double Max_batangle = move_angle+call_halfwidth;
            
            if(Max_batangle >=2*M_PI){Max_batangle = Max_batangle -2*M_PI;};
            if(Min_batangle <0){Min_batangle = Min_batangle +2*M_PI;};
            if(AngleFromBat >=2*M_PI){AngleFromBat = AngleFromBat -2*M_PI;};
            if(AngleFromBat <0){AngleFromBat = AngleFromBat +2*M_PI;};
            
            /*
            std::cout<< "Min batangle"<<Min_batangle<<std::endl;
            std::cout<< "Max batangle"<<Max_batangle<<std::endl;
            std::cout<<"AngleFromBat"<<AngleFromBat <<std::endl;
            */
            
            //Is the detector in the width of the call
            if((Min_batangle>Max_batangle && AngleFromBat <= Max_batangle) || //IF the min < zero
               (Min_batangle>Max_batangle && AngleFromBat >= Min_batangle) || //IF the max > 360
               (approximatelyequal(Min_batangle, AngleFromBat)==1)   ||
               (approximatelyequal(Max_batangle, AngleFromBat)==1)   ||
               (AngleFromBat >= Min_batangle && AngleFromBat <= Max_batangle)){
                //std::cout<< "IN BAT ANGEL"<<std::endl;
                // If it's in the possible angle then record in vector
                UpdateCaptures(Individual_ID,itnumber,location_x_animal,location_y_animal,time);
                captured = 1;           
            };//End of "detector in the width of the call" IF
            }
            
        }; //End of "detector in the width of the call" IF
    
        
    }; //End of "radius" IF
    }; //END of not directly on same spot as camera
    
    if(captured==1){return(1);} else {return(0);};
    
    Time1=clock()-Time1;
    time1+=Time1;
    
}; // End of function


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------------------------------
 //
 //                                           Tests
 //
 // - TEST FOR INTERSECTION
 // - TEST FOR TIME ANGLE calulations 
 // - 
 --------------------------------------------------------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//--------TEST FOR INTERSECTION  ---------------//
void CameraTrap::TestVertAndAngleInteraction(){
    if(VertAndAngleInteraction(0, 1, 5)!=5){ 
        std::cout<<"Error! Failed camera test - TestVertAndAngleInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
    if(VertAndAngleInteraction(1, 1, 5)!=6){
        std::cout<<"Error! Failed camera test - TestVertAndAngleInteraction: "<<"2"<< std::endl;
        exit (EXIT_FAILURE);
    }
    if(VertAndAngleInteraction(1, 2, 5)!=7){
        std::cout<<"Error! Failed camera test - TestVertAndAngleInteraction: "<<"3"<< std::endl;
        exit (EXIT_FAILURE);
    }
    std::cout<<"Passed! Camera test - TestVertAndAngleInteraction"<< std::endl;
}

void CameraTrap::TestHorzAndAngleInteraction(){
    if(HorzAndAngleInteraction(0, 1, 5)!=-5){
        std::cout<<"Error! Failed camera test - TestHorzAndAngleInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
    if(HorzAndAngleInteraction(1, 1, 5)!=-4){
        std::cout<<"Error! Failed camera test - TestHorzAndAngleInteraction: "<<"2"<< std::endl;
        exit (EXIT_FAILURE);
    }
    if(HorzAndAngleInteraction(1, 2, 5)!=-2){
        std::cout<<"Error! Failed camera test - TestHorzAndAngleInteraction: "<<"3"<< std::endl;
        exit (EXIT_FAILURE);
    }
    std::cout<<"Passed! Camera test - TestHorzAndAngleInteraction "<< std::endl;
}


void CameraTrap::TestAngleAndAngleInteraction(){
    
    std::vector <double> Estimate(2);
    
    Estimate = AngleAndAngleInteraction(1, 0, -1, 0);
    if(Estimate[0]!=0 && Estimate[1]!=0){
        std::cout<<"Error! Failed camera test - TestAngleAndAngleInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
    Estimate = AngleAndAngleInteraction(1,0, 2, 0);
    if(Estimate[0]!=0 && Estimate[1]!=0){
        std::cout<<"Error! Failed camera test - TestAngleAndAngleInteraction: "<<"2"<< std::endl;
        exit (EXIT_FAILURE);
    }
    Estimate = AngleAndAngleInteraction(1,10, -1, 5);
    if(Estimate[0]!=-2.5 && Estimate[1]!=7){
        std::cout<<"Error! Failed camera test - TestAngleAndAngleInteraction: "<<"3"<< std::endl;
        exit (EXIT_FAILURE);
    }
    std::cout<<"Passed! Camera test - TestAngleAndAngleInteraction"<< std::endl;
}


void CameraTrap::TestHorzAndCircInteraction(){
    std::vector <double> Estimate(2);
    
    Estimate = HorzAndCircInteraction(1, 0, 0, 1); // Line tangent to the top of the circle, coordinates (0,1)
    if(Estimate[0]!=0 && Estimate[1]!=0){
        std::cout<<"Error! Failed camera test - TestHorzAndCircInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
    Estimate = HorzAndCircInteraction(0, 0, 0, 1);// Line crosses centre of the circle, coordinates (-1,0) or (1,0)
    if((Estimate[0]!=-1 && Estimate[1]!=-1) && (Estimate[0]!=1 && Estimate[1]!=-1)){
        std::cout<< Estimate[0] <<" " <<Estimate[1] << " "<<std::endl;
        std::cout<<"Error! Failed camera test - TestHorzAndCircInteraction: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
    Estimate = HorzAndCircInteraction(0.5, 0, 0, 1); // Line crosses top part of the circle, coordinates (+/-sqrt(0.75),0.5)
    double temp = sqrt(1-pow(0.5,2));
    if((Estimate[0]!= temp && Estimate[1]!=-temp) && (Estimate[0]!=-temp && Estimate[1]!=temp)){
        //std::cout<< temp << " "<<std::endl;
        //std::cout<< Estimate[0] <<" " <<Estimate[1] << " "<<std::endl;
        std::cout<<"Error! Failed camera test - TestHorzAndCircInteraction: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    }
    std::cout<<"Passed! Camera test - TestHorzAndCircInteraction"<< std::endl;
    
};

void CameraTrap::TestVertAndCircInteraction(){
    std::vector <double> Estimate(2);
    
    Estimate = VertAndCircInteraction(1, 0, 0, 1);
    if(Estimate[0]!=0 && Estimate[1]!=0){
        std::cout<<"Error! Failed camera test - TestVertAndCircInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
    Estimate = VertAndCircInteraction(0, 0, 0, 1);
    if((Estimate[0]!=-1 && Estimate[1]!=1)&&(Estimate[0]!=1 && Estimate[1]!=-1)){
        std::cout<<"Error! Failed camera test - TestVertAndCircInteraction: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
    Estimate = VertAndCircInteraction(0.5, 0, 0, 1);
    double temp = sqrt(1-pow(0.5,2));
    if((Estimate[0]!=-temp && Estimate[1]!=temp)&&(Estimate[0]!=temp && Estimate[1]!=-temp)){
        std::cout<<"Error! Failed camera test - TestVertAndCircInteraction: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    }
    std::cout<<"Passed! Camera test - TestVertAndCircInteraction"<< std::endl;
    
};


void CameraTrap::TestAngleAndCircInteraction(){
    std::vector <double> Estimate(4);
    
    Estimate = AngleAndCircInteraction(1,0, 0, 0, 1);
    double temp = cos(M_PI/4);
    if(((approximatelyequal(Estimate[0],-temp)==1 && (approximatelyequal(Estimate[2],-temp)==1))
            &&
         (approximatelyequal(Estimate[2],temp)==1 && (approximatelyequal(Estimate[3],temp)==1)))
        &&
       ((approximatelyequal(Estimate[0],temp)==1 && (approximatelyequal(Estimate[2],temp)==1))
        &&
        (approximatelyequal(Estimate[2],-temp)==1 && (approximatelyequal(Estimate[3],-temp)==1)))){
        std::cout<< temp<<std::endl;
        std::cout<< Estimate[0]<<std::endl;
        std::cout<<"Error! Failed camera test - TestAngleAndCircInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    Estimate = AngleAndCircInteraction(-1,0 , 0, 0, 1);
    if(((approximatelyequal(Estimate[0],-temp)==1 && (approximatelyequal(Estimate[2],temp)==1))
        &&
        (approximatelyequal(Estimate[2],temp)==1 && (approximatelyequal(Estimate[3],-temp)==1)))
       &&
       ((approximatelyequal(Estimate[0],temp)==1 && (approximatelyequal(Estimate[2],-temp)==1))
        &&
        (approximatelyequal(Estimate[2],-temp)==1 && (approximatelyequal(Estimate[3],temp)==1)))){
        std::cout<<"Error! Failed camera test - TestAngleAndCircInteraction: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    Estimate = AngleAndCircInteraction(1, sqrt(2), 0, 0, 1);
    temp = 1/sqrt(2);
    if(((approximatelyequal(Estimate[0],-temp)==1 && (approximatelyequal(Estimate[2],temp)==1))
        &&
        (approximatelyequal(Estimate[2],temp)==1 && (approximatelyequal(Estimate[3],-temp)==1)))
       &&
       ((approximatelyequal(Estimate[0],temp)==1 && (approximatelyequal(Estimate[2],-temp)==1))
        &&
        (approximatelyequal(Estimate[2],-temp)==1 && (approximatelyequal(Estimate[3],temp)==1)))){
        std::cout<< Estimate[0] <<" " << (1/sqrt(2))<<std::endl;
        std::cout<< Estimate[0] <<" " << Estimate[1] <<" "<< Estimate[2] <<" " << Estimate[3] <<" "<<std::endl;
        std::cout<<"Error! Failed camera test - TestAngleAndCircInteraction: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    }
    std::cout<<"Passed! Camera test - TestAngleAndCircInteraction"<< std::endl;
};
/*
 END OF INTERSECPT TESTS
 */

/* TIME ANGLE TEST*/
void CameraTrap::TestTimeAndAngleCal(){
    std::vector <double> Estimate(2);
    
    // Vertical movement: intersept is at half way & directly below
    Estimate = TimeAndAngleCal(0,0,1,0,2);
    if(Estimate[0]!=0.5 || Estimate[1]!=M_PI){
        std::cout<<Estimate[0]<< " "<<Estimate[1]<<std::endl;
        std::cout<<"Error! Failed camera test - TestTimeAndAngleCal: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
    // Vertical movement: intersept is at half way & directly above
    Estimate = TimeAndAngleCal(1,0,0,0,2);
    if(Estimate[0]!=0.5 || Estimate[1]!=0){
        std::cout<<Estimate[0]<< " "<<Estimate[1]<<std::endl;
        std::cout<<"Error! Failed camera test - TestTimeAndAngleCal: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    // Angle movement: intersept is at half way & at 45 degree
    Estimate = TimeAndAngleCal(1/sqrt(2),1/sqrt(2),0,0,2);
    if(approximatelyequal(Estimate[0],0.5)!=1 || approximatelyequal(Estimate[1],M_PI/4)!=1){
        std::cout<< M_PI/4<<std::endl;
        std::cout<< approximatelyequal(Estimate[1],M_PI/4)<<std::endl;
        std::cout<<Estimate[0]<< " "<<Estimate[1]<<std::endl;
        std::cout<<"Error! Failed camera test - TestTimeAndAngleCal: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    // Angle movement: intersept is at half way & at 225 degree
    Estimate = TimeAndAngleCal(-1/sqrt(2),-1/sqrt(2),0,0,2);
    if(approximatelyequal(Estimate[0],0.5)!=1 || approximatelyequal(Estimate[1],5*M_PI/4)!=1){
        std::cout<<Estimate[0]<< " "<<Estimate[1]<<std::endl;
        std::cout<<"Error! Failed camera test - TestTimeAndAngleCal: "<<"4" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    // Angle movement: intersept is at 100% of the distance & at 45 degree
    Estimate = TimeAndAngleCal(sqrt(2),sqrt(2),0,0,2);
    if(approximatelyequal(Estimate[0],1)!=1 || approximatelyequal(Estimate[1],M_PI/4)!=1){
        std::cout<<Estimate[0]<< " "<<Estimate[1]<<std::endl;
        std::cout<<"Error! Failed camera test - TestTimeAndAngleCal: "<<"5" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    // Angle movement: intersept is at 200% of the distance & at 45 degree
    Estimate = TimeAndAngleCal(2*sqrt(2),2*sqrt(2),0,0,2);
    if(approximatelyequal(Estimate[0],2)!=1 || approximatelyequal(Estimate[1],M_PI/4)!=1){
        std::cout<<Estimate[0]<< " "<<Estimate[1]<<std::endl;
        std::cout<<"Error! Failed camera test - TestTimeAndAngleCal: "<<"6" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    // Angle movement: intersept is at 0% of the distance & at 45 degree
    Estimate = TimeAndAngleCal(0,0,0,0,2);
    if(approximatelyequal(Estimate[0],0)!=1 || approximatelyequal(Estimate[1],0)!=1){
        std::cout<<Estimate[0]<< " "<<Estimate[1]<<std::endl;
        std::cout<<"Error! Failed camera test - TestTimeAndAngleCal: "<<"7" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    std::cout<<"Passed! Camera test - TestTimeAndAngleCal"<< std::endl;
    
};
/*END OF TIME ANGLE TEST*/


/*Test Gradient from angle*/
void CameraTrap::TestGradientFromAngle(){
    
    double Estimate;
    Estimate = GradientFromAngle(0); // Vertical "upwards" line
    if(isinf(Estimate)!=1){
        std::cout<<"Error! Failed camera test - TestGradientFromAngle: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    Estimate = GradientFromAngle(M_PI); // Vertical "downwards" line
    if(isinf(-1*Estimate)!=1 && Estimate>pow(-10,12)){ // Does not produce neg inf, but produces very low number
        std::cout<<"Error! Failed camera test - TestGradientFromAngle: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    Estimate = GradientFromAngle(M_PI/2); // Horizontal "left" line
    if(Estimate==0){ 
        std::cout<<"Error! Failed camera test - TestGradientFromAngle: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    Estimate = GradientFromAngle(3*M_PI/2); // Horizontal "right" line
    if(Estimate==0){ 
        std::cout<<"Error! Failed camera test - TestGradientFromAngle: "<<"4" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    Estimate = GradientFromAngle(M_PI/4); // Line of the identity (Quadrant1)
    if(Estimate==1){
        std::cout<<"Error! Failed camera test - TestGradientFromAngle: "<<"5" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    Estimate = GradientFromAngle(3*M_PI/4); // Line of the negative identity (Quadrant2)
    if(Estimate==-1){
        std::cout<<"Error! Failed camera test - TestGradientFromAngle: "<<"6" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    Estimate = GradientFromAngle(5*M_PI/4); // Line of the identity (Quadrant3)
    if(Estimate==1){
        std::cout<<"Error! Failed camera test - TestGradientFromAngle: "<<"7" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    Estimate = GradientFromAngle(7*M_PI/4); // Line of the negative identity (Quadrant4)
    if(Estimate==-1){
        std::cout<<"Error! Failed camera test - TestGradientFromAngle: "<<"8" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    std::cout<<"Passed! Camera test - TestGradientFromAngle"<< std::endl;
};
/*End of test for Gradient from angle*/




//--------TEST FOR CAPTURING WITHIN RADIUS AND ANGLE  ---------------//

void CameraTrap::TestCI(){
    
    int Estimate;
    //location_x_animal,location_y_animal,Individual_ID,call_halfwidth,move_angle,itnumber,time.
    angle = 0; angle_HalfWidth = 0;
    Estimate = CapturesIndividual(location_x,location_y,1,M_PI,0,0,0); // Animal directly above camera
    if(Estimate!=1){
        std::cout<<"Error! Failed camera test - TestCI: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    angle = 0; angle_HalfWidth = 0;
    Estimate = CapturesIndividual(location_x+1,location_y+1,1,M_PI,0,0,0); // Animal is on boundary and facing away
    if(Estimate!=0){
        std::cout<<"Error! Failed camera test - TestCI: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    angle = 0; angle_HalfWidth = M_PI/4; // Animal is on within boundary and facing towards, camera is facing towards
    Estimate = CapturesIndividual(location_x+0.5,location_y+0.5,1,M_PI,M_PI+angle_HalfWidth,0,0);
    if(Estimate!=1){
        std::cout<<"Error! Failed camera test - TestCI: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    //location_x_animal,location_y_animal,Individual_ID,call_halfwidth,move_angle,itnumber,time.
    angle = 0; angle_HalfWidth = M_PI/4; // Animal is on right edge and facing towards camera
    Estimate = CapturesIndividual(location_x+sin(angle_HalfWidth),location_y+cos(angle_HalfWidth),1,M_PI/4,M_PI+angle_HalfWidth,0,0);
    if(Estimate!=1){
        std::cout<<"Error! Failed camera test - TestCI: "<<"4" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    //location_x_animal,location_y_animal,Individual_ID,call_halfwidth,move_angle,itnumber,time.
    angle = 0; angle_HalfWidth = M_PI/4; // Animal is on left edge and facing towards camera
    Estimate = CapturesIndividual(location_x-sin(angle_HalfWidth),location_y+cos(angle_HalfWidth),1,M_PI/4,M_PI-angle_HalfWidth,0,0);
    if(Estimate!=1){
        std::cout<<"Error! Failed camera test - TestCI: "<<"5" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    //location_x_animal,location_y_animal,Individual_ID,call_halfwidth,move_angle,itnumber,time.
    angle = 0; angle_HalfWidth = M_PI/4; // Animal in range and facing towards camera, but camera facing away
    Estimate = CapturesIndividual(location_x,location_y-0.5,1,M_PI/4,angle,0,0);
    if(Estimate!=0){
        std::cout<<"Error! Failed camera test - TestCI: "<<"6" <<std::endl;
        exit (EXIT_FAILURE);
    }
    
    std::cout<<"Passed! Camera test - TestCI"<< std::endl;

};

/*-----------Test Movement Capture thing--------------*/


//NEEDS completeling
// Vert movement Vert camera
// Vert movement Horz camera
// Vert movement Angle camera
// Horz movement Vert camera
// Horz movement Horz camera
// Horz movement Angle camera
// Angle movement Vert camera
// Angle movement Horz camera
// Angle movement Angle camera
// Is the camera lines at a angles and is the movement also at an angle?

void CameraTrap::TestCapturesIntersection(){
    
    int loc1;
    int loc2;
    double temp;
    
    //-------------------------------------------------//
    // Vertical movement through the centre of sector
    loc1 =0; loc2 =0;
    resetCaptures();
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0;
    //std::cout<<"Test CapInter"<< std::endl;
    CapturesIntersection(0,1,0,0, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         0,  // move_angle,
                         1); // IT
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"1a" <<std::endl;exit (EXIT_FAILURE);};
            
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", " <<Captures[i][6]<<std::endl;
             
            if(Captures[i][4] ==0 && Captures[i][5] ==0){loc1 += 1;};
            if(Captures[i][4] ==0 && Captures[i][5] ==1){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"1b" <<std::endl;
                exit (EXIT_FAILURE);};
        };
    };
     //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc1<< std::endl;
     if(loc1!=2 || loc2!=2){
         std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"1c" <<std::endl;
         exit (EXIT_FAILURE);
     };
    
    //-------------------------------------------------//
    // Horizontal movement through the centre of sector
    resetCaptures();
    /// this brings up ever  - because something somewhere needs an approx.equal///
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0;
    //std::cout<<"Test CapInter: 2"<< std::endl;
    CapturesIntersection(0.5,0.5,-0.5,0.5, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         M_PI/2,  // move_angle,
                         1); // IT
    loc1 =0; loc2 =0;
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"2a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(Captures[i][4] ==0.5 && Captures[i][5] ==0.5){loc1 += 1;};
            if(Captures[i][4] ==-0.5 && Captures[i][5] ==0.5){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"2b" <<std::endl;
                exit (EXIT_FAILURE);};
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=1){
        std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"2c" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    //-------------------------------------------------//
    // Angle movement through the centre of sector
    resetCaptures();
    /// this brings up ever  - because something somewhere needs an approx.equal///
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
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
            if(i>4){std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"3a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(Captures[i][4] ==-0.5 && Captures[i][5] ==0.5){loc1 += 1;};
            if(Captures[i][4] ==0 && Captures[i][5] ==1){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"3b" <<std::endl;
                exit (EXIT_FAILURE);};
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=1 || loc2!=2){
        std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"3c" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    
    //-------------------------------------------------//
    // Horizontal movement through top of the sector, through both edges and the circle twice
    resetCaptures();
    /// this brings up ever  - because something somewhere needs an approx.equal///
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0;
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
            if(i>4){std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"4a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(approximatelyequal(Captures[i][4],-temp)==1 && approximatelyequal(Captures[i][5],temp)==1){loc1 += 1;};
            if(approximatelyequal(Captures[i][4],temp)==1 && approximatelyequal(Captures[i][5],temp)==1){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"4b" <<std::endl;
                exit (EXIT_FAILURE);};
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=3){
        std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"4c" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    
    //-------------------------------------------------//
    // Angle movement through on edge of sector
    resetCaptures();
    /// this brings up ever  - because something somewhere needs an approx.equal///
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0;
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
            if(i>4){std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"5a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(approximatelyequal(Captures[i][4],0) ==1 && approximatelyequal(Captures[i][5],0) ==1 ){loc1 += 1;};
            if(approximatelyequal(Captures[i][4],temp) ==1 && approximatelyequal(Captures[i][5],temp) ==1 ){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"5b" <<std::endl;
                //exit (EXIT_FAILURE);
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=2){
        std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"5c" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    
    
    //-------------------------------------------------//
    // Angle movement through on edge of sector
    resetCaptures();
    /// this brings up ever  - because something somewhere needs an approx.equal///
    angle = 0; angle_HalfWidth = M_PI/4; location_x =0; location_y =0; radius = 1;
    m_detector1 = -1; m_detector2 = 1; c_detector1 =0;c_detector2 =0;
    std::cout<<"Test CapInter: Number 6"<< std::endl;
    CapturesIntersection(-temp,temp,0,0, // Current x/y, previous x,y
                         1, // ID
                         M_PI, //Call width - circular call (to make this easier!)
                         M_PI/4,  // move_angle,
                         1); // IT
    loc1 =0; loc2 =0;
    for(int i=0; i<Captures.size(); i++){
        if(Captures[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"6a" <<std::endl;exit (EXIT_FAILURE);};
            
            //X,Y,TIME
            std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            
            if(approximatelyequal(Captures[i][4],0) ==1 && approximatelyequal(Captures[i][5],0) ==1 ){loc1 += 1;};
            if(approximatelyequal(Captures[i][4],-temp) ==1 && approximatelyequal(Captures[i][5],temp) ==1 ){loc2 += 1;};
            if(Captures[i][6]<0 || Captures[i][6]>1){
                std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"6b" <<std::endl;
                //exit (EXIT_FAILURE);
            };
        };
    };
    std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=1 || loc2!=2){
        std::cout<<"Error! Failed camera test - TestCapturesIntersection: "<<"6c" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    
    
    std::cout<<"Passed! Camera test - TestCameraAndMovement"<< std::endl;
};

