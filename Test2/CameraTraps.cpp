//
//  CameraTraps.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 19/04/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include "CameraTraps.h"
#include "Parameters.h"
#include <stdio.h>
#include <math.h>

CameraTrap::CameraTrap(){};
CameraTrap::CameraTrap(int a//CT_identifier;
                       ,double b //Radius of circle
                       ,double ca // Angle between each location
                       ,double d//radius;
                       //,std::vector<std::vector<int>> //::Captures()
                       ){
    
    // Renames variables

    
    double CircleR = b;
    double CircleAngle = ca;
    
    
    //////////////////////////
    /// Location of camera ///
    //////////////////////////
    // Using polar to cartesian co-ordinates
    // This means that the cameras start at 3o'clock and move in a anti-clockwise direction
    location_x = (CircleR * cos(a*CircleAngle)) + Cir_CntX ;
    location_y = (CircleR * sin(a*CircleAngle)) + Cir_CntY ;
    
    // The angle (with respect to "north") the camera is facing is:
    // 90 degrees minus the number of degrees between the start and the current camera
    // because of the anti-clockwise motion
    // After passes zero - then
    angle = M_PI/2 - a*CircleAngle;
    if(angle<0){angle = 2*M_PI + angle;};
    
    //Assigning varaibles
    CT_identifier = a;
    CT_StepOn = a + NoRunIn;
    radius =d;
    angle_HalfWidth = CameraWidth;
    Captures.resize(NoSteps*(Sq_MaxX-Sq_MinX)*(Sq_MaxY-Sq_MinY)*DensityAnimals);
    capturecount=0;
};



////////////////////////
////////////////////////

int approximatelyequal(double a, double b){
    int r;
    if(sqrt(pow(a-b,2))<1*pow(10,-10)){r=1;}else{r=0;}
    return (r);
}

int CameraTrap::CapturesIndividual(double location_x_animal,
                                   double location_y_animal,
                                   int Individual_ID,
                                   int CameraID,
                                   int Time_step,
                                   double call_halfwidth,
                                   double move_angle
                                   ){
    
    int captured=0;
    
    double AngleFromCamera = 0;
    
    //distance to the camera
    double diffx = location_x_animal - location_x;
    double diffy = location_y_animal - location_y;
    
    if(diffx==0 && diffy ==0){ // If on the exact same spot as the camera assume it will be captured 
        std::vector<int> myvector;
        myvector.resize(3);
        myvector[1] = Individual_ID;
        myvector[2] = Time_step;
        myvector[3] = CameraID;
        
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
       
        // If in range, is it in the angle?
        // atan calculates radians => need to change using:
        //              Angle Degress = Angle Radians * 180 / pi
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
        std::cout<< "Min angle"<<Min_angle<<std::endl;
        std::cout<< "Max angle"<<Max_angle<<std::endl;
        std::cout<<"AngleFromCamera"<<AngleFromCamera <<std::endl;
        std::cout<<AngleFromCamera-Max_angle <<std::endl;
        */
        
        // If the angle is between min and max possible angle
        if((Min_angle>Max_angle && AngleFromCamera <= Max_angle) || //IF the min value is less than zero
           (Min_angle>Max_angle && AngleFromCamera >= Min_angle) || //IF the max value is greater than 360
           (approximatelyequal(Min_angle, AngleFromCamera)==1)   ||
           (approximatelyequal(Max_angle, AngleFromCamera)==1)   ||
           (AngleFromCamera >= Min_angle && AngleFromCamera <= Max_angle)){
              //std::cout<< "IN CAM ANGEL"<<std::endl;
            

            
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
                std::vector<int> myvector;
                myvector.resize(3);
                myvector[1] = Individual_ID;
                myvector[2] = Time_step;
                myvector[3] = CameraID;
                    
                Captures[capturecount]=myvector;
                capturecount+=1;
                
                captured = 1;
            };//End of "detector in the width of the call" IF
            
            
        }; //End of "detector in the width of the call" IF
    
        
    }; //End of "radius" IF
    }; //END of not directly on same spot as camera
    if(captured==1){return(1);} else {return(0);};
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
                                        ,double d
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
                                     1, // Camera ID  - the number printed in the output
                                     1, // Step ID  - the number printed in the output
                                      Call_halfwidth,
                                     call_angle);
    //std::cout<<capture<<std::endl;
    //
    if(capture!=d){
        if(d==1){
            std::cout<<"Error! Failed camera test -  Not captured when should. Failed input: "<<ID<< std::endl;
            exit (EXIT_FAILURE);
        } else{
            std::cout<<"Error! Failed camera test - Captured when shouldn't. Failed input: "<<ID<<  std::endl;
            exit (EXIT_FAILURE);
        }
    }
    
}; //END OF FUNCTION


