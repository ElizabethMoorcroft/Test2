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
    double angle_temp = M_PI/2 - a*CircleAngle;
    if(angle_temp<0){angle_temp = 2*M_PI + angle_temp;};
    angle = angle_temp;

    
    //Assigning varaibles
    CT_identifier = a;
    CT_StepOn = a + NoRunIn;

};



////////////////////////
////////////////////////

int CameraTrap::CapturesIndividual(double a,double b, int c, int d, int e, double f, double g){
    
    int captured=0;
    
    double AngleFromCamera = 0;
    
    // Renames values
    double location_x_animal = a;
    double location_y_animal = b;
    int Individual_ID=c;
    int CameraID = d;
    int Time_step =e;
    double call_halfwidth =f;
    double move_angle =g;
    
    //distance to the camera
    double diffx = location_x_animal - location_x;
    double diffy = location_y_animal - location_y;
    double diff_animal_camera = sqrt(pow(diffx, 2) + pow(diffy, 2));
    
    // Is the animal in the range of the camera?
    if(diff_animal_camera < radius){
        
        // If in range, is it in the angle?
        // atan calculates radians => need to change using:
        //              Angle Degress = Angle Radians * 180 / pi
        // If the location is "south" of the camera then add pi becuase
        // atan will only give values between -90 and +90
        if(diffy>=0){AngleFromCamera = atan(diffx/diffy)* 180 / M_PI;}
        else        {AngleFromCamera = atan(diffx/diffy)* 180 / M_PI +M_PI;};
        
        double Min_angle = angle-angle_HalfWidth;
        double Max_angle = angle+angle_HalfWidth;
        
        // If the angle is between min and max possible angle
        if(AngleFromCamera > Min_angle && AngleFromCamera < Max_angle){
            
            //Calcaulates the angle from the Bat to the detector as a bearing from north
            double AngleFromBat = AngleFromCamera+180;
            if(AngleFromBat >360){AngleFromBat = AngleFromBat -360;};
            
            double Min_batangle = move_angle-call_halfwidth;
            double Max_batangle = move_angle+call_halfwidth;
            
            //Is the detector in the width of the call
            if(AngleFromBat > Min_batangle && AngleFromBat < Max_batangle){
                // If it's in the possible angle then record in vector
                std::vector<int> myvector;
                myvector.resize(3);
                myvector[1] = Individual_ID;
                myvector[2] = Time_step;
                myvector[3] = CameraID;
                    
                Captures.push_back (myvector);
                
                captured = 1;
            };//End of "detector in the width of the call" IF
            
            
        }; //End of "detector in the width of the call" IF
    
        
    }; //End of "radius" IF
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
    //aninal location realtive to camera
    double animal_xlocation = location_x +a*radius*(sin(a1*angle_HalfWidth+angle)); 
    double animal_ylocation = location_y +b*radius*(cos(b1*angle_HalfWidth+angle));
    
    // Call angle relative to the camera angle
    double call_angle = angle + c*M_PI; 
    if(call_angle>2*M_PI){call_angle=call_angle-2*M_PI;};
    
    int capture = CapturesIndividual(animal_xlocation,
                                     animal_ylocation,
                                     1, // Animal ID  - the number printed in the output
                                     1, // Camera ID  - the number printed in the output
                                     1, // Step ID  - the number printed in the output
                                     angle,
                                     call_angle);
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


