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
                       //,std::vector<std::vector<int>> //::Captures()
                       ){
    
    // Renames variables
    time1=0;
    /*
    // Random placement of cameras
    srand(a);
    
    std::vector<double> RandomNumberStreamCP;
    RandomNumberStreamCP.resize(100);
    for(int i=0; i<100; i++){
        RandomNumberStreamCP[i] = double (rand());
    };
    
    std::vector<int> RandomNumberStreamCPpick;
    RandomNumberStreamCPpick.resize(101);
    for(int i=0; i<101; i++){
        RandomNumberStreamCPpick[i] = round(double (rand())/RAND_MAX*99 +1);
    };
    std::cout<< RandomNumberStreamCPpick[0]<< std::endl;
    std::cout<< RandomNumberStreamCPpick[50]<< std::endl;
    std::cout<< RandomNumberStreamCPpick[100]<< std::endl;
    
    
    srand(RandomNumberStreamCP[0]);
    std::vector<double> RandomNumberStreamCPx;
    RandomNumberStreamCPx.resize(100);
    for(int i=0; i<100; i++){
        RandomNumberStreamCPx[i] = double (rand());
    };

    srand(RandomNumberStreamCP[50]);
    std::vector<double> RandomNumberStreamCPy;
    RandomNumberStreamCPy.resize(100);
    for(int i=0; i<100; i++){
        RandomNumberStreamCPy[i] = double (rand());
    };
    
    srand(RandomNumberStreamCP[100]);
    std::vector<double> RandomNumberStreamCPa;
    RandomNumberStreamCPa.resize(100);
    for(int i=0; i<100; i++){
        RandomNumberStreamCPa[i] = double (rand());
    };
    */
    if(RandomCameraPlacement==1){
        std::cout<<"Inside RCP"<<std::endl;
        //List of random number
        
        /*
        RandNum Number1;
        location_x = Number1.AtoBUnif(RandomNumberStreamCPx[RandomNumberStreamCPpick[0]],Sq_MinX+20,Sq_MaxX-20);
        location_y = Number1.AtoBUnif(RandomNumberStreamCPy[RandomNumberStreamCPpick[50]],Sq_MinY+20,Sq_MaxY-20);
        angle = Number1.AtoBUnif(RandomNumberStreamCPa[RandomNumberStreamCPpick[100]],0,2*M_PI);
        */
        location_x = (Sq_MaxX-Sq_MinX)/2;
        location_y = (Sq_MaxX-Sq_MinX)/2;
        angle = 0;
        //if(angle<0){angle = 2*M_PI + angle;};
        std::cout<<"END RCP: "<< a << std::endl;
        
    } else{
    //////////////////////////
    /// Location of camera ///
    //////////////////////////
    // Using polar to cartesian co-ordinates
    // This means that the cameras start at 3o'clock and move in a anti-clockwise direction
    location_x = (RadiusCameraCircle  * cos(a*AngleBetweenCameras)) + Cir_CntX ;
    location_y = (RadiusCameraCircle  * sin(a*AngleBetweenCameras)) + Cir_CntY ;
    
    
    // The angle (with respect to "north") the camera is facing is:
    // 90 degrees minus the number of degrees between the start and the current camera
    // because of the anti-clockwise motion
    // After passes zero - then
    angle = M_PI/2 - a*AngleBetweenCameras;
    if(angle<0){angle = 2*M_PI + angle;};
    };
    

    //Assigning varaibles
    CT_identifier = a;
    CT_StepOn = a + NoRunIn;
    radius =d;
    angle_HalfWidth = CameraWidth;
    Captures.resize((Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY)*DensityAnimals+100);
    capturecount=0;
    myvector.resize(7);
    //std::cout<<"END create"<< std::endl;
};

CameraTrap::CameraTrap(int a//CT_identifier;
                       ,double d//radius;
                        , double tempangle
                       ,double tempCameraWidth
                       //,std::vector<std::vector<int>> //::Captures()
                       ){
    
    // Renames variables
    time1=0;
    
    //////////////////////////
    /// Location of camera ///
    //////////////////////////
    // Using polar to cartesian co-ordinates
    // This means that the cameras start at 3o'clock and move in a anti-clockwise direction
    location_x = 0 ;
    location_y = 0 ;
    
    // The angle (with respect to "north") the camera is facing is:
    // 90 degrees minus the number of degrees between the start and the current camera
    // because of the anti-clockwise motion
    // After passes zero - then
    angle = tempangle;
    if(angle<0){angle = 2*M_PI + tempangle;};
    
    //Assigning varaibles
    CT_identifier = a;
    CT_StepOn = 0;
    radius =d;
    angle_HalfWidth = tempCameraWidth;
    Captures.resize((Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY)*DensityAnimals);
    capturecount=0;
    myvector.resize(6);
};



////////////////////////
////////////////////////

int approximatelyequal(double a, double b){
    int r;
    if(sqrt(pow(a-b,2))<(1*pow(10,-6))){r=1;}else{r=0;}
    return (r);
}


int CameraTrap::CapturesAlg(double location_x_animal,
                                              double location_y_animal,
                                              double previous_x_animal,
                                              double previous_y_animal,
                                              int Individual_ID,
                                              double call_halfwidth,
                                              double move_angle,
                                              int itnumber){
    int captured = 0;
    
    // If the camera is looking for continuous throughout the time step then:
    // CapturesContin==1 -- else the program jumps to the CapturesIndividual function
    if(CapturesContin==1){
        
        // finds the total distance travelled between it's new location and it's old location
        double disttotal = sqrt(pow(previous_x_animal-location_x_animal,2)+
                                pow(previous_y_animal-location_y_animal,2));
        
        // Variables used later
        double distedge;
        double time;
        double diffy;
        double diffx;
        double AngleBatCap;
        
        double m_animal;
        double c_animal;
        
        double m_detector1;
        double m_detector2;
        double c_detector1;
        double c_detector2;
        
        //If the animal movement was a line on a graph with a gradient and a intercept, Y=mX+c, then:
        //  - gadient would be, m=(change x/change y)
        //  - intercpet would be: y-mx=c (where y and x are known)
        if(location_x_animal!=previous_x_animal && location_y_animal!=previous_y_animal
           && (angle-angle_HalfWidth) != 0 && (angle-angle_HalfWidth) != M_PI ){
            m_animal  = 1/(tan(move_angle));
            c_animal  = location_y_animal-location_x_animal*m_animal;
            
        // If the lines of the detcetor were lines of graphs with garident and intercepts, Y=mX+c, then:
        //  - gradient if theta is the ange of the line then, m=1/tan(theta)
        //  - intercpet would be: y-mx=c (where y and x are known)
        m_detector1 = 1/(tan(angle-angle_HalfWidth));
        m_detector2 = 1/(tan(angle+angle_HalfWidth));
        c_detector1  = location_y - location_x*m_detector1;
        c_detector2  = location_y - location_x*m_detector2;
        
        // The animal movement line will cross the line of the detector edges (as they are infinate straight line
        // and they are not paralle
        // Using Y = mX +c
        // Therefore, when they are intercept:
        // y1 = m1*x1 +c1  = y2 = m2*x2 +c2
        // y = m1*x +c1  = y = m2*x +c2
        //  m1*x +c1 = m2*x +c2
        //  m1*x - m2*x = c2 - c1
        //  (m1 - m2)*x = c2 - c1
        //  x = (c2 - c1)/(m1 - m2)
        //  substitue the x value into the y=mx+c to find the y value
        
        if((CT_StepOn >= 1108 && CT_StepOn <=1108)
               && Individual_ID == 2) {
            std::cout<<"Individual_ID "<<Individual_ID<<std::endl;
            std::cout<<"previous_x_animal "<<previous_x_animal<<std::endl;
            std::cout<<"previous_y_animal "<<previous_y_animal<<std::endl;
            std::cout<<"location_x_animal "<<location_x_animal<<std::endl;
            std::cout<<"location_y_animal "<<location_y_animal<<std::endl;
            std::cout<<"CT_StepOn "<<CT_StepOn<<std::endl;
            std::cout<<"m_detector1 "<<m_detector1<<std::endl;
            std::cout<<"m_animal "<<m_animal<<std::endl;}
            
        if(m_detector1 != m_animal){
            double edge1xcoord = (c_detector1 - c_animal)/(m_animal - m_detector1) ;
            double edge1ycoord = m_detector1*edge1xcoord +c_detector1;
            
           
            // The lines will intercept if they aren't parellel so:
            // then check that:
            //      - the distance to the intercept is less than the total distance travlled
            //      - it's in the correct direction
            // Time = disance to intercept / distance travelled,
            //  if the distance is greater than the amount travelled then hasn't been captured
            // AngleBatCap is the angle between the animal and the sensor
            //  If the angle that the bat is travelling in is not in the correct direction then the
            //  bat will not be captured
            diffy =(edge1ycoord-previous_y_animal);
            diffx =(edge1xcoord-previous_x_animal);
            distedge = sqrt(pow(diffx,2)+ pow(diffy,2));
            time = distedge/disttotal;
            AngleBatCap = atan((diffx)/(diffy));
            if(diffy<=0){AngleBatCap+=M_PI;}//End if
            
            if((CT_StepOn >= 1108 && CT_StepOn <=1108)
               && Individual_ID == 2){
                std::cout<<"edge1xcoord "<<edge1xcoord<<std::endl;
                std::cout<<"edge1ycoord "<<edge1ycoord<<std::endl;
                std::cout<<"distedge "<<distedge<<std::endl;
                std::cout<<"time "<<time<<std::endl;
                std::cout<<"AngleBatCap "<<AngleBatCap<<std::endl;
                 std::cout<<"move_angle "<<move_angle<<std::endl;
            }
            
            if(time<=1 && approximatelyequal(AngleBatCap,move_angle)==1){
                //std::cout<<"Hello "<<std::endl;
                captured += CapturesIndividual(edge1xcoord,
                                               edge1ycoord,
                                               Individual_ID,
                                               call_halfwidth,
                                               move_angle,
                                               itnumber,
                                               time);
            };//End if (time and anglebatcap)
        }; //END IF (grad = grad)
        if(m_detector2 != m_animal){
            double edge2xcoord = (c_detector2 - c_animal)/(m_animal - m_detector2) ;
            double edge2ycoord = m_detector2*edge2xcoord +c_detector2;

            if((CT_StepOn >= 1300 && CT_StepOn <=1300)
               && Individual_ID == 10){
                std::cout<<"edge2xcoord "<<edge2xcoord<<std::endl;
                std::cout<<"edge2ycoord "<<edge2ycoord<<std::endl;}
            // See above for reasons for Time and AngleBatCap calculation
            // Has to be done for both vertices
            diffy =(edge2ycoord-previous_y_animal);
            diffx =(edge2xcoord-previous_x_animal);
            distedge = sqrt(pow(diffx,2)+ pow(diffy,2));
            time = distedge/disttotal;
            AngleBatCap = atan((diffx)/(diffy));
            if(diffy<=0){AngleBatCap+=M_PI;} //End if
            
            if(time<=1 && approximatelyequal(AngleBatCap,move_angle)==1){
                if((CT_StepOn >= 1300 && CT_StepOn <=1300)
                   && Individual_ID == 10){
                    std::cout<<"Captured 2nd coordinates "<<edge2xcoord<<std::endl;}
                captured += CapturesIndividual(edge2xcoord,
                                               edge2ycoord,
                                               Individual_ID,
                                               call_halfwidth,
                                               move_angle,
                                               itnumber,
                                               time);
            }; //End if (time and anglebatcap)
        } // END IF (gard2 == grad.animal)

        
        
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
        double circ_term1 = pow(m_animal,2)+1;
        double temp_A =  location_y - c_animal;
        double circ_term2 = -(m_animal*temp_A +location_x)*2;
        double circ_term3 = pow(temp_A,2)+pow(location_x,2) - pow(radius,2);
         
        // The Quadractic formula can be used to solve: (m^2+1)X^2 - 2(Am+b)X +(b^2 +A^2 - r^2) = 0
        // for X and hence for the coordinate of the intercept between circle and line
        // Quaddractic formula:
        //          (-b+-sqrt(b^2-4ac))/2a
        // if the term  (b^2-4ac) is less than zero then there is no iterception between line and circle
        // and there are no solutions
        // can skip the next parts
        double temp = pow(circ_term2,2)-(4*circ_term1*circ_term3);
            if((CT_StepOn >= 1300 && CT_StepOn <=1300)
               && Individual_ID == 10){std::cout<<"Temp "<<temp<<std::endl;}
        if(temp>0){
            double circ_solsqrt = sqrt(temp);
            double circ_xsol1 = (-circ_term2 - circ_solsqrt)/(2*circ_term1);
            double circ_xsol2 = (-circ_term2 + circ_solsqrt)/(2*circ_term1);
            double circ_ysol1 = m_animal*circ_xsol1 + c_animal ;
            double circ_ysol2 = m_animal*circ_xsol2 + c_animal;
            
            if((CT_StepOn >= 1300 && CT_StepOn <=1300)
               && Individual_ID == 10){
                std::cout<<"radius "<<radius<<std::endl;
                std::cout<<"location_x " <<location_x<<std::endl;
                std::cout<<"location_y " <<location_y<<std::endl;
                std::cout<<"circ_xsol1 "<<circ_xsol1<<std::endl;
                std::cout<<"circ_ysol1 "<<circ_ysol1<<std::endl;
                std::cout<<"circ_xsol2 "<<circ_xsol2<<std::endl;
                std::cout<<"circ_ysol2 "<<circ_ysol2<<std::endl;}
            
            // the distance travelled from the preious location to the "capture" incident
            diffy =(circ_ysol1-previous_y_animal);
            diffx =(circ_xsol1-previous_x_animal);
            distedge = sqrt(pow(diffx,2)+ pow(diffy,2));
            time = distedge/disttotal;
            
            AngleBatCap = atan((diffx)/(diffy));
            if(diffy<=0){AngleBatCap+=M_PI;}
            
            if(time <=1 && approximatelyequal(AngleBatCap,move_angle)==1){
                //std::cout<<"Hello again"<<std::endl;
                captured += CapturesIndividual(circ_xsol1,
                               circ_ysol1,
                               Individual_ID,
                               call_halfwidth,
                               move_angle,
                               itnumber,
                               time);
            }; //END IF (time and anglebatcap)
            
            diffy =(circ_ysol2-previous_y_animal);
            diffx =(circ_xsol2-previous_x_animal);
            distedge = sqrt(pow(diffx,2)+ pow(diffy,2));
            time = distedge/disttotal;
            AngleBatCap = atan((diffx)/(diffy));
            if(diffy<=0){AngleBatCap+=M_PI;}
            if(time <=1 && approximatelyequal(AngleBatCap,move_angle)==1){
                captured += CapturesIndividual(circ_xsol2,
                               circ_ysol2,
                               Individual_ID,
                               call_halfwidth,
                               move_angle,
                               itnumber,
                               time);
            };//END IF (time and anglebatcap)
        
        }; //END IF (temp>0)
        }; // END OF IF animal move horizontal
      // END OF IF STATEMENT (CAPTURECONTIN)
    } //else{
        // If not continuous captures run normal Capture individual
        captured += CapturesIndividual( location_x_animal,
                            location_y_animal,
                            Individual_ID,
                            call_halfwidth,
                            move_angle,
                           itnumber,
                           1); // Assumes that it captures right at the end of the movement therefore time=1
      //      };
    
    if((CT_StepOn >= 1300 && CT_StepOn <=1300)
       && Individual_ID == 10){
        std::cout<<"captured "<<captured<<std::endl;}
// If there are any captures then return 1 else return 0
if(captured>=1){return(1);} else {return(0);};

}

// Captures individual 
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
    if((CT_StepOn >= 1300 && CT_StepOn <=1300)
       && Individual_ID == 10){
        std::cout<<"In Camera "<<std::endl;}
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
        std::cout<< "RADIUS"<<std::endl;
        
        if((CT_StepOn >= 1300 && CT_StepOn <=1300)
           && Individual_ID == 10){
            std::cout<<"in camera Radius"<<std::endl;}
       
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
        
        
        std::cout<< "location_x_animal: "<<location_x_animal<<std::endl;
        std::cout<< "location_y_animal: "<<location_y_animal<<std::endl;
        std::cout<< "Angle of camera: "<<angle<<std::endl;
        std::cout<< "Movement angle: "<<move_angle<<std::endl;
        std::cout<< "Min. camera angle: "<<Min_angle<<std::endl;
        std::cout<< "Max. camera angle: "<<Max_angle<<std::endl;
        std::cout<<"Angle from camera to bat: "<<AngleFromCamera <<std::endl;
        //std::cout<<AngleFromCamera-Max_angle <<std::endl;
        
        
        // If the angle is between min and max possible angle
        if((Min_angle>Max_angle && AngleFromCamera <= Max_angle) || //IF the min value is less than zero
           (Min_angle>Max_angle && AngleFromCamera >= Min_angle) || //IF the max value is greater than 360
           (approximatelyequal(Min_angle, AngleFromCamera)==1)   ||
           (approximatelyequal(Max_angle, AngleFromCamera)==1)   ||
           (AngleFromCamera >= Min_angle && AngleFromCamera <= Max_angle)){
              std::cout<< "IN CAM ANGEL"<<std::endl;
            
            if((CT_StepOn >= 1300 && CT_StepOn <=1300)
               && Individual_ID == 10){
                std::cout<<"In Camera angle"<<std::endl;}
            
            if(call_halfwidth==M_PI){
                std::cout<< "360 call"<<std::endl;
                
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
                std::cout<< "IN BAT ANGEL"<<std::endl;
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




//////////////////////////////////
// !!!!!!!!!! TEST !!!!!!!!!!!! //
//////////////////////////////////

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


