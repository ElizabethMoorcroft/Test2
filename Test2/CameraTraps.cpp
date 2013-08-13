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
        std::cout<<temp<<std::endl;
        angle = 0;
        if(angle<0){angle = 2*M_PI + angle;} else if(angle>=2*M_PI){angle = angle - 2*M_PI;};
        CT_StepOn = 0 + NoRunIn; // The cameras are on at all steps!!
    };

    //Assigning varaibles
    CT_identifier = a;
    radius =d;
    angle_HalfWidth = CameraWidth;
    Captures.resize(round(DensityAnimals*((Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY))));
    capturecount=0;
    myvector.resize(7);
    //std::cout<<"END create"<< std::endl;
};


void CameraTrap::Add1StepOn(){CT_StepOn=CT_StepOn+1;};
void CameraTrap::ResetStepOn(){CT_StepOn=0;};

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
    double Coordinate2 = sqrt(pow(radius,2) - pow((Vert - x_centre),2))+ y_centre;
    Coord[0] = Coordinate1;
    Coord[1] = Coordinate2;
    return(Coord);
}

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
     if((CT_StepOn >= 195 && CT_StepOn <=196)
     && Individual_ID == 3){std::cout<<"Temp "<<temp<<std::endl;}*/
    if(temp>0){
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
}


std::vector <double> TimeAndAngleCal(double Y, double X, double previous_y_animal, double previous_x_animal, double disttotal){
    
    std::vector <double> returnvalues(4);
    double diffy =(Y-previous_y_animal);
    double diffx =(X-previous_x_animal);
    double distedge = sqrt(pow(diffx,2)+ pow(diffy,2));
    double time = distedge/disttotal;
    double AngleBatCap = atan((diffx)/(diffy));
    if(diffy<=0){AngleBatCap+=M_PI;}//End if
    if(AngleBatCap <0){AngleBatCap += 2*M_PI;} else if(AngleBatCap >=2*M_PI){AngleBatCap -= 2*M_PI;};
    
    returnvalues[0] = time;
    returnvalues[1] = AngleBatCap;
    return(returnvalues);
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
    /*
    // If it's in the possible angle then record in vector
    myvector[0] = Individual_ID;
    myvector[1] = CT_StepOn;
    myvector[2] = CT_identifier;
    myvector[3] = itnumber;
    myvector[4] = location_x_animal;
    myvector[5] = location_y_animal;
    myvector[6] = 100;
    
    Captures[capturecount]=myvector;
    */
    // finds the total distance travelled between it's new location and it's old location
    double disttotal = sqrt(pow(previous_x_animal-location_x_animal,2)+
                            pow(previous_y_animal-location_y_animal,2));
    
    // Variables used later:
    double m_animal;
    double c_animal;
    double m_detector1;
    double m_detector2;
    double c_detector1;
    double c_detector2;
    
    //If the animal movement was a line on a graph with a gradient and a intercept, Y=mX+c, then:
    //  - gadient would be, m=(change x/change y)
    //  - intercpet would be: y-mx=c (where y and x are known)
    m_animal  = 1/(tan(move_angle));
    c_animal  = location_y_animal-location_x_animal*m_animal;
    
    // Detector 1 is line represneting the left hand slide of the detector range
    // Detector 2 is line represnting the RHS of the detector range
    // If the lines of the detcetor were lines of graphs with garident and intercepts, Y=mX+c, then:
    //  - gradient if theta is the ange of the line then, m=1/tan(theta)
    //  - intercpet would be: y-mx=c (where y and x are known)
    double detector1 = angle-angle_HalfWidth;
    double detector2 = angle-angle_HalfWidth;
    
    m_detector1 = 1/(tan(angle-angle_HalfWidth));
    m_detector2 = 1/(tan(angle+angle_HalfWidth));
    c_detector1  = location_y - location_x*m_detector1;
    c_detector2  = location_y - location_x*m_detector2;
    
    int det1 =0;
    int det2 =0;
    
    if(detector1== 0 || detector1== M_PI|| detector1== 2*M_PI){det1 = 1;} //Line is vertical
    else if(detector1 == M_PI/2 || detector1== 3*M_PI/2){det1 = 2;} //Line is Horizontal

    if(detector2== 0 || detector2== M_PI|| detector2== 2*M_PI){det2 = 1;} //Line is vertical
    else if(detector2 == M_PI/2 || detector2== 3*M_PI/2){det2 = 2;} //Line is Horizontal
    
    // Checks for crossing the boundaries for detector 1
    if(Individual_ID== 77 && CT_StepOn>769 && CT_StepOn<771){std::cout<<"Dect1"<<std::endl;}
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
                                  det1,
                                  disttotal);
    
    // Checks for crossing the boundaries for detector 2
    if(Individual_ID== 77 && CT_StepOn>769 && CT_StepOn<771){
        std::cout<<"Dect2 "<< CT_StepOn <<std::endl;
        std::cout<<"Dect grad = "<<m_detector2<<std::endl;
        std::cout<<"Dect int = "<<c_detector2<<std::endl;
        std::cout<<"Animal grad = "<<m_animal<<std::endl;
        std::cout<<"Animal int = "<<c_animal<<std::endl;}
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
                                  det2,
                                  disttotal);
    
    if(Individual_ID== 77 && CT_StepOn>769 && CT_StepOn<771){std::cout<<"Circ"<<std::endl;}
    // Checks for crossing the boundaries for circular edge of detector
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
    
    if(Individual_ID== 77 && CT_StepOn>769 && CT_StepOn<771){std::cout<<"End"<<std::endl;}
    // Checks for at the end of the step if in/out of detection area
    captured += CapturesIndividual(location_x_animal,
                                   location_y_animal,
                                   Individual_ID,
                                   call_halfwidth,
                                   move_angle,
                                   itnumber,
                                   1); // Assumes that it captures right at the end of the movement therefore time=1
    
    // If there are any captures then return 1 else return 0
    if(captured>=1){return(1);} else {return(0);};
};
////////////////////
////////////////////


// Is the camera lines at a angles and is the movement also at an angle?
// If so then this should

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
    std::vector<double> XandY(4);
    
    //Animal has vertical movement
    if(location_x_animal == previous_x_animal){
        XandY = VertAndCircInteraction(location_x_animal, location_y, location_x, radius);
        // The first coordinates
        TandA = TimeAndAngleCal(XandY[1], XandY[0], previous_y_animal, previous_x_animal, disttotal);
        if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
            captured += CapturesIndividual(XandY[0],
                                           XandY[1],
                                           Individual_ID,
                                           call_halfwidth,
                                           move_angle,
                                           itnumber,
                                           TandA[0]);}
        // The second coordinates
        TandA = TimeAndAngleCal(XandY[3], XandY[2], previous_y_animal, previous_x_animal, disttotal);
        if(TandA[0]<=1 &&approximatelyequal(TandA[1],move_angle)==1){
            captured += CapturesIndividual(XandY[2],
                                           XandY[3],
                                           Individual_ID,
                                           call_halfwidth,
                                           move_angle,
                                           itnumber,
                                           TandA[0]);}

    }//--------END OF VERTICAL ANIMAL MOVEMENT ---------------//
    
    //Animal has horizontal movement
    else if(location_y_animal == previous_y_animal){
        XandY = HorzAndCircInteraction(location_y_animal, location_y, location_x, radius);
        // The first coordinates
        TandA = TimeAndAngleCal(XandY[1], XandY[0], previous_y_animal, previous_x_animal, disttotal);
        if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
            captured += CapturesIndividual(XandY[0],
                                           XandY[1],
                                           Individual_ID,
                                           call_halfwidth,
                                           move_angle,
                                           itnumber,
                                           TandA[0]);}
        // The second coordinates
        TandA = TimeAndAngleCal(XandY[3], XandY[2], previous_y_animal, previous_x_animal, disttotal);
        if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
            captured += CapturesIndividual(XandY[2],
                                           XandY[3],
                                           Individual_ID,
                                           call_halfwidth,
                                           move_angle,
                                           itnumber,
                                           TandA[0]);}
    }//--------END OF HORZONTAL ANIMAL MOVEMENT ---------------//
    
    
    
    else{
        XandY = AngleAndCircInteraction(m_animal, c_animal, location_y, location_x, radius);
        // The first coordinates
        TandA = TimeAndAngleCal(XandY[1], XandY[0], previous_y_animal, previous_x_animal, disttotal);
        if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
            captured += CapturesIndividual(XandY[0],
                                           XandY[1],
                                           Individual_ID,
                                           call_halfwidth,
                                           move_angle,
                                           itnumber,
                                           TandA[0]);}
        // The second coordinates
        TandA = TimeAndAngleCal(XandY[3], XandY[2], previous_y_animal, previous_x_animal, disttotal);
        if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
            captured += CapturesIndividual(XandY[2],
                                           XandY[3],
                                           Individual_ID,
                                           call_halfwidth,
                                           move_angle,
                                           itnumber,
                                           TandA[0]);}
        
    }//--------END OF DIFFERENT ANGLE ANIMAL MOVEMENT ---------------//
    

    
    return(captured);
};




////////////////////
////////////////////


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
    double Y;
    double X;
    
    //Animal has vertical movement
    if(location_x_animal == previous_x_animal){
        //Horzontal detector
            if(det==2){
                TandA = TimeAndAngleCal(location_y, previous_x_animal, previous_y_animal, previous_x_animal, disttotal);
                if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
                    captured += CapturesIndividual(previous_x_animal,
                                                   location_y,
                                                   Individual_ID,
                                                   call_halfwidth,
                                                   move_angle,
                                                   itnumber,
                                                   TandA[0]);}
            } // END IF DET==2
        // Vertical detector AND x-position as teh movement
        else if(det==1 && location_x== previous_x_animal){
            TandA = TimeAndAngleCal(previous_y_animal, previous_x_animal, previous_y_animal, previous_x_animal, disttotal);
            if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
                captured += CapturesIndividual(previous_x_animal,
                                               previous_y_animal,
                                               Individual_ID,
                                               call_halfwidth,
                                               move_angle,
                                               itnumber,
                                               TandA[0]);}
            } // END IF DET==1
        // Detcteor at angle
        else{
            Y = HorzAndAngleInteraction(location_x_animal, m_animal, c_animal);
            TandA = TimeAndAngleCal(Y, location_x_animal, previous_y_animal, previous_x_animal, disttotal);
            if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
                captured += CapturesIndividual(location_x_animal,
                                               Y,
                                               Individual_ID,
                                               call_halfwidth,
                                               move_angle,
                                               itnumber,
                                               TandA[0]);}
        }// END ELSE
    }//--------END OF VERTICAL ANIMAL MOVEMENT ---------------//
    
    //Animal has horizontal movement
    else if(location_y_animal == previous_y_animal){
        //Horzontal detector AND y-position as the movement
        if(det==2 && location_y== previous_y_animal){
            TandA = TimeAndAngleCal(location_y, previous_x_animal, previous_y_animal, previous_x_animal, disttotal);
            if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
                captured += CapturesIndividual(previous_x_animal,
                                               location_y,
                                               Individual_ID,
                                               call_halfwidth,
                                               move_angle,
                                               itnumber,
                                               TandA[0]);}
        } // END OF IF DET ==2
        // Vertical detector 
        else if(det==1){
            TandA = TimeAndAngleCal(previous_y_animal, previous_x_animal, previous_y_animal, previous_x_animal, disttotal);
            if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
                captured += CapturesIndividual(previous_x_animal,
                                               previous_y_animal,
                                               Individual_ID,
                                               call_halfwidth,
                                               move_angle,
                                               itnumber,
                                               TandA[0]);}
        } // END OF IF DET ==1
        // Detecteor at angle
        else {
            X = VertAndAngleInteraction(location_y_animal, m_animal, c_animal);
            TandA = TimeAndAngleCal(X, location_x_animal, previous_y_animal, previous_x_animal, disttotal);
            if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
                captured += CapturesIndividual(X,
                                               location_y_animal,
                                               Individual_ID,
                                               call_halfwidth,
                                               move_angle,
                                               itnumber,
                                               TandA[0]);}
        }// END ELSE
    }//--------END OF HORZONTAL ANIMAL MOVEMENT ---------------//
    
    
    // If the angle of the movement and the detector is the same, then they have to have the same intercept
    else if(m_detector == m_animal && c_detector == c_animal){
            
            TandA = TimeAndAngleCal(previous_y_animal, previous_x_animal, previous_y_animal, previous_x_animal, disttotal);
            if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
            captured += CapturesIndividual(previous_x_animal,
                                           previous_y_animal,
                                           Individual_ID,
                                           call_halfwidth,
                                           move_angle,
                                           itnumber,
                                           TandA[0]);}
            
            TandA = TimeAndAngleCal(location_y_animal, location_x_animal, previous_y_animal, previous_x_animal, disttotal);
            if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
            captured += CapturesIndividual(location_x_animal,
                                           location_y_animal,
                                           Individual_ID,
                                           call_halfwidth,
                                           move_angle,
                                           itnumber,
                                           TandA[0]);}
        }//--------END OF SAME ANGLE ANIMAL MOVEMENT ---------------//
    
    else if(m_detector != m_animal){
        
        //Horzontal detector
        if(det==2){
            X = HorzAndAngleInteraction(location_y, m_animal, c_animal);
            TandA = TimeAndAngleCal(location_y, X, previous_y_animal, previous_x_animal, disttotal);
            if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
                captured += CapturesIndividual(X,
                                               location_y,
                                               Individual_ID,
                                               call_halfwidth,
                                               move_angle,
                                               itnumber,
                                               TandA[0]);}
        } // END OF IF DET ==2
        // Vertical detector
        else if(det==1){
            Y = VertAndAngleInteraction(location_x, m_animal, c_animal);
            TandA = TimeAndAngleCal(Y, location_x, previous_y_animal, previous_x_animal, disttotal);
            if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
                captured += CapturesIndividual(location_x,
                                               Y,
                                               Individual_ID,
                                               call_halfwidth,
                                               move_angle,
                                               itnumber,
                                               TandA[0]);}
        } // END OF IF DET ==1

        else{
        XandY =AngleAndAngleInteraction(m_detector, c_detector, m_animal, c_animal);
        TandA = TimeAndAngleCal(XandY[1], XandY[0], previous_y_animal, previous_x_animal, disttotal);
        
        /*
        if(Individual_ID== 77 && CT_StepOn>769 && CT_StepOn<774){
            std::cout<<"X ="<<XandY[0] <<", Y=" << XandY[1]<<std::endl;
            std::cout<<"PrevX ="<<previous_x_animal <<", PrevY=" << previous_y_animal<<std::endl;
            std::cout<<"T ="<<TandA[0] <<", A=" << TandA[1]<<std::endl;
            std::cout<<"Time ="<<1 <<", Angle=" << move_angle<<std::endl;}*/
        
        if(TandA[0]<=1 && approximatelyequal(TandA[1],move_angle)==1){
            //if(Individual_ID== 326 && CT_StepOn>901 && CT_StepOn<903){ std::cout<<"HELLO"<<std::endl;}
            captured += CapturesIndividual(XandY[0],
                                           XandY[1],
                                           Individual_ID,
                                           call_halfwidth,
                                           move_angle,
                                           itnumber,
                                           TandA[0]);
             //if(Individual_ID== 77 && CT_StepOn>769 && CT_StepOn<771){ std::cout<<"END"<<std::endl;}
            }
        }
        
    }//--------END OF DIFFERENT ANGLE ANIMAL MOVEMENT ---------------//
    
    else{
        std::cout<<" Something is very wrong with CameraTrap::CameraAndMovement " <<  std::endl;
        exit (EXIT_FAILURE);
    };
    
//-------- Return ---------------//
return(captured);
};




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
    
    /*if((CT_StepOn >= 1300 && CT_StepOn <=1300) && Individual_ID == 10){std::cout<<"In Camera "<<std::endl;}*/
    double AngleFromCamera = 0;
    
    //distance to the camera
    double diffx = location_x_animal - location_x;
    double diffy = location_y_animal - location_y;
    
    if(diffx==0 && diffy ==0){ // If on the exact same spot as the camera assume it will be captured

        myvector[0] = Individual_ID;
        myvector[1] = CT_StepOn;
        myvector[2] = CT_identifier;
        myvector[3] = itnumber;
        myvector[4] = location_x_animal;
        myvector[5] = location_y_animal;
        myvector[6] = time;
        
        Captures[capturecount]=myvector;
        capturecount+=1;
        
        captured = 1;
    } else{
    // If not at the exact same location then check whether it is
    
    double diff_animal_camera = sqrt(pow(diffx, 2) + pow(diffy, 2));
        //std::cout<<diff_animal_camera <<std::endl;
        //std::cout<<radius <<std::endl;
    
    // Is the animal in the range of the camera?
        //if(diff_animal_camera > radius){std::cout<< diff_animal_camera - radius<<std::endl;} else
    if(approximatelyequal(diff_animal_camera,radius)==1||
       diff_animal_camera<=radius){
        //std::cout<< "RADIUS"<<std::endl;
        
        if(Individual_ID== 77 && CT_StepOn>769 && CT_StepOn<771){std::cout<<"in camera Radius"<<std::endl;}
       
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
                myvector[0] = Individual_ID;
                myvector[1] = CT_StepOn;
                myvector[2] = CT_identifier;
                myvector[3] = itnumber;
                myvector[4] = location_x_animal;
                myvector[5] = location_y_animal;
                myvector[6] = time;
                
                Captures[capturecount]=myvector;
                capturecount+=1;
                
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
                //std::cout<< Individual_ID<<std::endl;
                //std::cout<< myvector.size() <<std::endl;
                
                // If it's in the possible angle then record in vector
                myvector[0] = Individual_ID;
                myvector[1] = CT_StepOn;
                myvector[2] = CT_identifier;
                myvector[3] = itnumber;
                myvector[4] = location_x_animal;
                myvector[5] = location_y_animal;
                myvector[6] = time;
                
                Captures[capturecount]=myvector;
                capturecount+=1;
                
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

/*--------------------------------------------------------------------------------------------------------
                                                Tests 
 
 --------------------------------------------------------------------------------------------------------*/

//--------TEST FOR CAPTURING WITHIN RADIUS AND ANGLE  ---------------//
void CameraTrap::TestCapturesIndividual(int ID
                                        ,double a
                                        ,double b
                                        ,double a1
                                        ,double b1
                                        ,double c
                                        ,double tempcallangleHalfWidth
                                        ,double outcome
                            ){
    
    //std::cout<<(a1*angle_HalfWidth+angle)<<std::endl;
    //aninal location realtive to camera
    double animal_xlocation = location_x +a*radius*(sin(a1*angle_HalfWidth+angle));
    double animal_ylocation = location_y +b*radius*(cos(b1*angle_HalfWidth+angle));
    
   /*
    std::cout<<radius<<std::endl;
    std::cout<<"Camera ID:"<<CT_identifier<<std::endl;
    std::cout<<"Camera x location:"<<location_x<<std::endl;
    std::cout<<"Camera y location:"<<location_y<<std::endl;
    std::cout<<"Animal x location:"<<animal_xlocation<<std::endl;
    std::cout<<"Animal y location:"<<animal_ylocation<<std::endl;
    */
    
    // Call angle relative to the camera angle
    double call_angle = angle + c*M_PI;
    if(call_angle>2*M_PI){call_angle=call_angle-2*M_PI;};
    
    //std::cout<<"Animal call angle:"<<call_angle<<std::endl;
    //std::cout<<"Camera angle:"<<angle<<std::endl;
    
    int capture = CapturesIndividual(animal_xlocation,
                                     animal_ylocation,
                                     1, // Animal ID  - the number printed in the output
                                     tempcallangleHalfWidth,
                                     call_angle,
                                     1,
                                     1);
    //std::cout<<capture<<std::endl;
    //
    if(capture!=outcome){
        if(outcome==1){
            std::cout<<"Error! Failed camera test -  Not captured when should. Failed input: "<<ID<< std::endl;
            exit (EXIT_FAILURE);
        } else{
            std::cout<<"Error! Failed camera test - Captured when shouldn't. Failed input: "<<ID<<  std::endl;
            exit (EXIT_FAILURE);
        }
    }
    
}; //END OF FUNCTION

//--------TEST FINDING INTERCEPTS  ---------------//
