//
//  SensorTest.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 14/10/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include "SensorTest.h"
#include "Sensors.h"


////// Sensor::Sensor(int Id, double sensorhalfangle, double sensorradius) /////

void SensorTest::RunSensorTests(){
    
    std::cout<<"Hello"<<std::endl;
    
    void SensorTest_construct();
    
    //---- Get / Set -----//
    SensorTest_setgetAngle();
    SensorTest_setgetHalfAngle();
    
    // ---- Functions ----//
    SensorTest_CaptureIndividual1();
    SensorTest_CaptureIndividual2();
    SensorTest_CaptureIndividual3();
    SensorTest_CaptureIndividual4();
    SensorTest_CaptureIndividual5();
    SensorTest_CaptureIndividual6();
    std::cout<<"Passed - CaptureIndividual tests 1-6" <<std::endl;
    
    
    TestVertAndAngleInteraction();
    TestHorzAndAngleInteraction();
    TestAngleAndAngleInteraction();
    TestHorzAndCircInteraction();
    TestVertAndCircInteraction();
    TestAngleAndCircInteraction();
    
    TestGradientFromAngle();
    
    Test_CapturesIntersection1();
    Test_CapturesIntersection2();
    Test_CapturesIntersection3();
    Test_CapturesIntersection4();
    Test_CapturesIntersection5();
    Test_CapturesIntersection6();
    Test_CapturesIntersection7();
    Test_CapturesIntersection8();
    Test_CapturesIntersection9();
    Test_CapturesIntersection10();
    
};

//---------------------- Constructor -------------------//
// Test Sensor constructor
 void SensorTest::SensorTest_construct(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 1);
    double id = Sensor1.getID();
    if(id!=1){
        std::cout<<"Error! Failed Sensor test - Test construct" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };

//---------------------- Sets / Gets -------------------//
// Test set and get of the angle
 void SensorTest::SensorTest_setgetAngle(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
    Sensor1.setAngle(1);
    double angle  = Sensor1.getAngle();
    if(angle!=1){
        std::cout<<"Error! Failed Sensor test - Test setgetAngle" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 // Test set and get of the halfangle
 void SensorTest::SensorTest_setgetHalfAngle(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
    Sensor1.setHalfAngle(1);
    double halfangle = Sensor1.getHalfAngle();
    if(halfangle!=1){
        std::cout<<"Error! Failed Sensor test - Test setgetHalfAngle" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 
 
//---------------------- Function: CaptureIndividual -------------------//
void SensorTest::SensorTest_CaptureIndividual1(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
    
    int Estimate;
    //location_x_animal,location_y_animal,Individual_ID,call_halfwidth,move_angle,itnumber,time.
    Sensor1.setXLoc(0); Sensor1.setYLoc(0);
    Estimate = Sensor1.CapturesIndividual(0,0,1,M_PI,0,0,0,0); // Animal directly above Sensor
    if(Estimate!=1){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
};


void SensorTest::SensorTest_CaptureIndividual2(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI_2, 1);
    Sensor1.setXLoc(0); Sensor1.setYLoc(0); Sensor1.setAngle(0); Sensor1.setRadius(1);
    int Estimate;
    //double location_x_animal, double location_y_animal, int Individual_ID, double call_halfwidth, double move_angle, int itnumber, double time, int call
    Sensor1.setXLoc(0); Sensor1.setYLoc(0);
    Estimate = Sensor1.CapturesIndividual(0,1,1,M_PI,M_PI,0,0,0); // Animal is on boundary and facing away
    if(Estimate!=1){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };

 
 void SensorTest::SensorTest_CaptureIndividual3(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/2, 1);
    Sensor1.setXLoc(0); Sensor1.setYLoc(0);
    int Estimate;
    //Sensor1.setAngle(0); Sensor1.setHalfAngle(0);
    Estimate = Sensor1.CapturesIndividual(0.5,0.5,1,M_PI,0,0,0,0); // Animal is on within boundary and facing towards Sensor 
    if(Estimate!=1){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 void SensorTest::SensorTest_CaptureIndividual4(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/4, 1);
    Sensor1.setXLoc(0); Sensor1.setYLoc(0);
    int Estimate;
    Sensor1.setAngle(0); Sensor1.setHalfAngle(M_PI_4);
    Estimate = Sensor1.CapturesIndividual(sin(M_PI_4),cos(M_PI_4),1,M_PI/4,M_PI+M_PI_4,0,0,0);//Animal is on right edge and facing towards sensor
    if(Estimate!=1){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"4" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 void SensorTest::SensorTest_CaptureIndividual5(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/4, 1);
    Sensor1.setXLoc(0); Sensor1.setYLoc(0);
    int Estimate;
    Sensor1.setAngle(0); Sensor1.setHalfAngle(M_PI_4);
    Estimate = Sensor1.CapturesIndividual(-sin(M_PI_4),cos(M_PI_4),1,M_PI/4,M_PI-M_PI_4,0,0,0);// Animal is on left edge and facing towards Sensor
    if(Estimate!=1){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"5" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 void SensorTest::SensorTest_CaptureIndividual6(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/4, 1);
    Sensor1.setXLoc(0); Sensor1.setYLoc(0);
    int Estimate;
    Sensor1.setAngle(0); Sensor1.setHalfAngle(M_PI_4);
    Estimate = Sensor1.CapturesIndividual(0,-0.5,1,M_PI/4,0,0,0,0); // Animal is out of range
    if(Estimate!=0){
        std::cout<<"Error! Failed Sensor test - TestCI: "<<"6" <<std::endl;
        exit (EXIT_FAILURE);
    }
 };
 
 
 
 
 
 //---------------------- Function: Interection of lines -------------------//
 
 // Unit testing for:
 //      - VertAndAngleInteraction
void SensorTest::TestVertAndAngleInteraction(){
 
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
    
    // Vertical line X = 0; Angle line: Gradient  = 1, Intercept = 5;
    // The intercept should be at (X= 0, Y=5)
    double v1 = 0; double v2 = 1; double v3 = 5;
    if(Sensor1.VertAndAngleInteraction(v1, v2, v3)!=5){
        std::cout<<"Error! Failed Sensor test - TestVertAndAngleInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Vertical line X = 1; Angle line: Gradient  = -1, Intercept = 5;
    // The intercept should be at (X= 1, Y=4)
     v1 = 1; v2 = -1;  v3 = 5;
    if(Sensor1.VertAndAngleInteraction(v1, v2, v3)!=4){
        std::cout<<"Error! Failed Sensor test - TestVertAndAngleInteraction: "<<"2"<< std::endl;
        exit (EXIT_FAILURE);
    };
    // Vertical line X = -1; Angle line: Gradient  = 1, Intercept = 5;
    // The intercept should be at (X= -1, Y=4)
    v1 = -1; v2 = 1; v3 = 5;
    if(Sensor1.VertAndAngleInteraction(v1, v2, v3)!=4){
        std::cout<<"Error! Failed Sensor test - TestVertAndAngleInteraction: "<<"3"<< std::endl;
        exit (EXIT_FAILURE);
    };
    //IF passed then print to screen
    std::cout<<"Passed! Sensor test - TestVertAndAngleInteraction"<< std::endl;
    
 };


 // Unit testing for:
 //      - TestHorzAndAngleInteraction
void SensorTest::TestHorzAndAngleInteraction(){
 
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
 
    // Horizontal line Y = 0; Angle line: Gradient  = 1, Intercept = 5;
    // The intercept should be at (X= -5, Y=0)
    double v1 = 0; double v2 = 1; double v3 = 5;
    if(Sensor1.HorzAndAngleInteraction(v1, v2, v3)!=-5){
        std::cout<<"Error! Failed Sensor test - TestHorzAndAngleInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Horizontal line Y = 1; Angle line: Gradient  = -1, Intercept = 5;
    // The intercept should be at (X= -5, Y=1)
    v1=1; v2=-1; v3=5;
    if(Sensor1.HorzAndAngleInteraction(v1, v2, v3)!=4){
        std::cout<<"Error! Failed Sensor test - TestHorzAndAngleInteraction: "<<"2"<< std::endl;
        exit (EXIT_FAILURE);
    };
    // Horizontal line Y = 1; Angle line: Gradient  = 2, Intercept = 5;
    // The intercept should be at (X= 2, Y=1)
    v1=1; v2=2; v3=5;
    if(Sensor1.HorzAndAngleInteraction(v1, v2, v3)!=-2){
        std::cout<<"Error! Failed Sensor test - TestHorzAndAngleInteraction: "<<"3"<< std::endl;
        exit (EXIT_FAILURE);
    };
    //IF passed then print to screen
    std::cout<<"Passed! Sensor test - TestHorzAndAngleInteraction "<< std::endl;
 };// END OF UNIT TEST
 
 
 // Unit testing for:
 //      - TestHorzAndAngleInteraction
void SensorTest::TestAngleAndAngleInteraction(){
 
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
 
    // Initate vector for results of the AngleAndAngleInteraction
    std::vector <double> Estimate(2);
    // Line 1: G1 = 1, I1 = 0 ; Line2: G2 = -1, I2 = 0;
    // The intercept should be at (X= 0, Y=0)
    double v1 = 1; double v2 = 0; double v3 = -1; double v4 = 0;
    Estimate = Sensor1.AngleAndAngleInteraction(v1, v2, v3, v4);
    if(Estimate[0]!=0 && Estimate[1]!=0){
        std::cout<<"Error! Failed Sensor test - TestAngleAndAngleInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Line 1: G1 = 1, I1 = 0 ; Line2: G2 = 2, I2 = 0;
    // The intercept should be at (X= 0, Y=0)
    v1 = 1; v2 = 0; v3 = 2; v4 = 0;
    Estimate = Sensor1.AngleAndAngleInteraction(v1, v2, v3, v4);
    if(Estimate[0]!=0 && Estimate[1]!=0){
        std::cout<<"Error! Failed Sensor test - TestAngleAndAngleInteraction: "<<"2"<< std::endl;
        exit (EXIT_FAILURE);
    };
    // Line 1: G1 = 1, I1 = 10 ; Line2: G2 = -1, I2 = 5;
    // The intercept should be at (X= -2.5, Y=7)
    v1 = 1; v2 = 10; v3 = -1; v4 = 5;
    Estimate = Sensor1.AngleAndAngleInteraction(v1, v2, v3, v4);
    if(Estimate[0]!=-2.5 && Estimate[1]!=7){
        std::cout<<"Error! Failed Sensor test - TestAngleAndAngleInteraction: "<<"3"<< std::endl;
        exit (EXIT_FAILURE);
    };
    //IF passed then print to screen
    std::cout<<"Passed! Sensor test - TestAngleAndAngleInteraction"<< std::endl;
    }; // END OF UNIT TEST
 
 // Unit testing for:
 //      - TestHorzAndCircInteraction
 void SensorTest::TestHorzAndCircInteraction(){
 
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
 
    // Initate vector for results of the HorzAndCircInteraction
    std::vector <double> Estimate(4);
    // Horz: Y = 1; Circle: Xcentre = 0, Ycentre = 0, Radius = 1;
    // should be tangent to the top of the circle
    // The intercept should be at (X=0, Y=1) and (X=0, Y=1)
     double v1 = 1; double v2 = 0; double v3 = 0; double v4 =1;
    Estimate = Sensor1.HorzAndCircInteraction(v1, v2, v3, v4); // Line tangent to the top of the circle, coordinates (0,1)
    if((Estimate[0]!=0 || Estimate[1]!=1) || (Estimate[2]!=0 || Estimate[3]!=1) ){
        std::cout<<"Error! Failed Sensor test - TestHorzAndCircInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Horz: Y = 1; Circle: Xcentre = 0, Ycentre = 0, Radius = 1;
    // should be tangent to the top of the circle
    // The intercept should be at (X=-1, Y=0) and (X=1, Y=0)
    v1 = 0; v2 = 0; v3 = 0; v4 =1;
    Estimate = Sensor1.HorzAndCircInteraction(v1, v2, v3, v4);
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
     v1 = 0.5; v2 = 0; v3 = 0; v4 =1;
    Estimate = Sensor1.HorzAndCircInteraction(v1, v2, v3, v4); // Line crosses top part of the circle, coordinates (+/-sqrt(0.75),0.5)
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
 void SensorTest::TestVertAndCircInteraction(){
     // Initate vector for results of the VertAndCircInteraction
     std::vector <double> Estimate(4);
 
     Sensor Sensor1;
     Sensor1 = Sensor(1, M_PI, 1);
     Sensor1.setXLoc(0); Sensor1.setYLoc(0);
     
     double v1 = 0;
    Estimate = Sensor1.VertAndCircInteraction(v1);
    if((Estimate[0]!=0 && Estimate[1]!=1 && Estimate[2]!=0 && Estimate[1]!=-1) ||
       (Estimate[0]!=0 && Estimate[1]!=-1 && Estimate[2]!=0 && Estimate[1]!=1)){
        std::cout<<"Error! Failed Sensor test - TestVertAndCircInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
     v1 = 1;
    Estimate = Sensor1.VertAndCircInteraction(v1);
     if((Estimate[0]!=1 && Estimate[1]!=0 && Estimate[2]!=1 && Estimate[1]!=0) ){
        std::cout<<"Error! Failed Sensor test - TestVertAndCircInteraction: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
     
     v1 = 0.5;
    Estimate = Sensor1.VertAndCircInteraction(v1);
    double temp = sqrt(1-pow(0.5,2));
    if((Estimate[0]!=-temp && Estimate[1]!=temp && Estimate[2]!=temp && Estimate[1]!=-temp) ||
       (Estimate[0]!=temp && Estimate[1]!=-temp && Estimate[2]!=-temp && Estimate[1]!=temp)){
        std::cout<<"Error! Failed Sensor test - TestVertAndCircInteraction: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    };
    //IF passed then print to screen
    std::cout<<"Passed! Sensor test - TestVertAndCircInteraction"<< std::endl;
 }; // END OF UNIT TEST
 
 
 void SensorTest::TestAngleAndCircInteraction(){
    std::vector <double> Estimate(4);
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 1);
     Sensor1.setXLoc(0); Sensor1.setYLoc(0);
 
     double v1 = 1; double v2 = 0;
    Estimate = Sensor1.AngleAndCircInteraction(v1,v2);
    double temp = cos(M_PI/4);
    if(((Sensor1.approximatelyequal(Estimate[0],-temp) && (Sensor1.approximatelyequal(Estimate[2],-temp)))
    &&(Sensor1.approximatelyequal(Estimate[2],temp) && (Sensor1.approximatelyequal(Estimate[3],temp))))
    ||((Sensor1.approximatelyequal(Estimate[0],temp) && (Sensor1.approximatelyequal(Estimate[2],temp)))
    &&(Sensor1.approximatelyequal(Estimate[2],-temp) && (Sensor1.approximatelyequal(Estimate[3],-temp))))){
        std::cout<< temp<<std::endl;
        std::cout<< Estimate[0]<<std::endl;
        std::cout<<"Error! Failed Sensor test - TestAngleAndCircInteraction: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    }
 
     v1 = -1; v2 = 0;
    Estimate = Sensor1.AngleAndCircInteraction(v1,v2);
    if(((Sensor1.approximatelyequal(Estimate[0],-temp) && (Sensor1.approximatelyequal(Estimate[2],-temp)))
    &&(Sensor1.approximatelyequal(Estimate[2],temp) && (Sensor1.approximatelyequal(Estimate[3],temp))))
    ||((Sensor1.approximatelyequal(Estimate[0],temp) && (Sensor1.approximatelyequal(Estimate[2],temp)))
    &&(Sensor1.approximatelyequal(Estimate[2],-temp) && (Sensor1.approximatelyequal(Estimate[3],-temp))))){
        std::cout<<"Error! Failed Sensor test - TestAngleAndCircInteraction: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    }
     
     v1 = 1; v2 = sqrt(2);
    Estimate = Sensor1.AngleAndCircInteraction(v1, v2);
    temp = 1/sqrt(2);
    if(((Sensor1.approximatelyequal(Estimate[0],-temp) && (Sensor1.approximatelyequal(Estimate[2],temp)))
    &&(Sensor1.approximatelyequal(Estimate[2],temp) && (Sensor1.approximatelyequal(Estimate[3],-temp))))
    ||((Sensor1.approximatelyequal(Estimate[0],temp) && (Sensor1.approximatelyequal(Estimate[2],-temp)))
    &&(Sensor1.approximatelyequal(Estimate[2],-temp) && (Sensor1.approximatelyequal(Estimate[3],temp))))){
        std::cout<< Estimate[0] <<" " << (1/sqrt(2))<<std::endl;
        std::cout<< Estimate[0] <<" " << Estimate[1] <<" "<< Estimate[2] <<" " << Estimate[3] <<" "<<std::endl;
        std::cout<<"Error! Failed Sensor test - TestAngleAndCircInteraction: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    }
    std::cout<<"Passed! Sensor test - TestAngleAndCircInteraction"<< std::endl;
};
 
 
 //--------TIME ANGLE TEST  ---------------//
 void SensorTest::TestTimeAndAngleCal(){
    std::vector <double> Estimate(2);
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
 
    // Vertical movement: intersept is at half way & directly below
     double v1= 0; double v2= 0; double v3= 1; double v4= 0; double v5= 2;
    Estimate = Sensor1.TimeAndAngleCal(v1,v2, v3, v4, v5);
    if(Estimate[0]!=0.5 || Estimate[1]!=M_PI){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Vertical movement: intersept is at half way & directly above
     v1 = 1; v2 =0; v3 =0; v4 = 0 ; v5 = 2;
    Estimate = Sensor1.TimeAndAngleCal(v1,v2, v3, v4, v5);
    if(Estimate[0]!=0.5 || Estimate[1]!=0){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Angle movement: intersept is at half way & at 45 degree
     v1 = 1/sqrt(2); v2 =1/sqrt(2); v3 =0; v4 = 0 ; v5 = 2;
    Estimate = Sensor1.TimeAndAngleCal(v1,v2, v3, v4, v5);
    if(Sensor1.approximatelyequal(Estimate[0],0.5)!=1 || Sensor1.approximatelyequal(Estimate[1],M_PI/4)!=1){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Angle movement: intersept is at half way & at 225 degree
     v1 = -1/sqrt(2); v2 =-1/sqrt(2); v3 =0; v4 = 0 ; v5 = 2;
    Estimate = Sensor1.TimeAndAngleCal(v1,v2, v3, v4, v5);
    if(Sensor1.approximatelyequal(Estimate[0],0.5)!=1 || Sensor1.approximatelyequal(Estimate[1],5*M_PI/4)!=1){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"4" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Angle movement: intersept is at 100% of the distance & at 45 degree
     v1 = sqrt(2); v2 =sqrt(2); v3 =0; v4 = 0 ; v5 = 2;
    Estimate = Sensor1.TimeAndAngleCal(v1,v2, v3, v4, v5);
    if(Sensor1.approximatelyequal(Estimate[0],1)!=1 || Sensor1.approximatelyequal(Estimate[1],M_PI/4)!=1){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"5" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Angle movement: intersept is at 200% of the distance & at 45 degree
     v1 = 2*sqrt(2); v2 =2*sqrt(2); v3 =0; v4 = 0 ; v5 = 2;
    Estimate = Sensor1.TimeAndAngleCal(v1,v2, v3, v4, v5);
    if(Sensor1.approximatelyequal(Estimate[0],2)!=1 || Sensor1.approximatelyequal(Estimate[1],M_PI/4)!=1){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"6" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // Angle movement: intersept is at 0% of the distance & at 45 degree
     v1 =0; v2 =0; v3 =0; v4 = 0 ; v5 = 2;
    Estimate = Sensor1.TimeAndAngleCal(v1,v2, v3, v4, v5);
    if(Sensor1.approximatelyequal(Estimate[0],0)!=1 || Sensor1.approximatelyequal(Estimate[1],0)!=1){
        std::cout<<"Error! Failed Sensor test - TestTimeAndAngleCal: "<<"7" <<std::endl;
        exit (EXIT_FAILURE);
    };
    // IF passed print to screen
    std::cout<<"Passed! Sensor test - TestTimeAndAngleCal"<< std::endl;
 };




//---------- Test Gradient from angle -----------------//
void SensorTest::TestGradientFromAngle(){
    // Initate double for results
    double Estimate;
    Sensor Sensor1;
    Sensor1 = Sensor(1, 0, 0);
    
    double v1=0;
    Estimate = Sensor1.GradientFromAngle(v1); // Vertical "upwards" line
    if(isinf(Estimate)!=1){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"1" <<std::endl;
        exit (EXIT_FAILURE);
    };
    v1=M_PI;
    Estimate = Sensor1.GradientFromAngle(v1); // Vertical "downwards" line
    if(isinf(-1*Estimate)!=1 && Estimate>pow(-10,12)){ // Does not produce neg inf, but produces very low number
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"2" <<std::endl;
        exit (EXIT_FAILURE);
    };
    v1=M_PI_2;
    Estimate = Sensor1.GradientFromAngle(v1); // Horizontal "left" line
    if(Estimate==0){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"3" <<std::endl;
        exit (EXIT_FAILURE);
    };
    v1=3*M_PI_2;
    Estimate = Sensor1.GradientFromAngle(v1); // Horizontal "right" line
    if(Estimate==0){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"4" <<std::endl;
        exit (EXIT_FAILURE);
    };
    v1=M_PI_4;
    Estimate = Sensor1.GradientFromAngle(v1); // Line of the identity (Quadrant1)
    if(Estimate==1){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"5" <<std::endl;
        exit (EXIT_FAILURE);
    };
    v1=M_PI_4*3;
    Estimate = Sensor1.GradientFromAngle(v1); // Line of the negative identity (Quadrant2)
    if(Estimate==-1){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"6" <<std::endl;
        exit (EXIT_FAILURE);
    };
    v1=M_PI_4*5;
    Estimate = Sensor1.GradientFromAngle(v1); // Line of the identity (Quadrant3)
    if(Estimate==1){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"7" <<std::endl;
        exit (EXIT_FAILURE);
    };
    v1=M_PI_4*7;
    Estimate = Sensor1.GradientFromAngle(v1); // Line of the negative identity (Quadrant4)
    if(Estimate==-1){
        std::cout<<"Error! Failed Sensor test - TestGradientFromAngle: "<<"8" <<std::endl;
        exit (EXIT_FAILURE);
    }
    // If passed print to screen
    std::cout<<"Passed! Sensor test - TestGradientFromAngle"<< std::endl;
};
//End of test for Gradient from angle//

 
 //NEEDS completing
 // Is the Sensor lines at a angles and is the movement also at an angle?

 void SensorTest::Test_CapturesIntersection1(){
 
    int loc1; int loc2;
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI, 1);
     Sensor1.setXLoc(0); Sensor1.setYLoc(0);

 
    //-------------------------------------------------//
    // Vertical movement through the centre of sector
    // Vert movement Angle Sensor
    loc1 =0; loc2 =0;
    //Angle Sensor and vertical movement. Should get captured:
    //  - At detector1 line (X=0,Y=0.5,T=0)
    //  - At detector2 line  (X=0,Y=0,T=0)
    //  - At Circle line (X=0 ,Y=1 ,T=1)
    //  - At end of the step (X=0,Y=1,T=1)
     double v1 = 0; double v2 = 0; double v3 = 0; double v4 = 1;
     int v5 =1; int v7 =1;
     double v6 = M_PI;
    Sensor1.CapturesIntersection(v1,v2,v3,v4, // Current x/y, previous x,y
                     v5, // ID
                     M_PI, //Call width - circular call (to make this easier!)
                     v6,  // move_angle,
                     v7); // IT
     std::vector<std::vector<double>> caps = Sensor1.getCaptures();

     
    for(int i=0; i<caps.size(); i++){
        if(caps[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"1a" <<std::endl;exit (EXIT_FAILURE);};
            
            //std::cout<<caps[i][4]<<", " <<caps[i][5]<<", " <<caps[i][6]<<std::endl;
            
            if(caps[i][4] ==0 && caps[i][5] ==0){loc1 += 1;};
            if(caps[i][4] ==0 && caps[i][5] ==1){loc2 += 1;};
            if(caps[i][6]<0 || caps[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"1b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< caps[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc1<< std::endl;
    if(loc1!=0 || loc2!=1){
            std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"1c" <<std::endl;
            exit (EXIT_FAILURE);
    };
 };



 void SensorTest::Test_CapturesIntersection2(){
 
    int loc1 =0;
    int loc2 =0;
    Sensor Sensor1;
     Sensor1 = Sensor(1, M_PI_4, 1);
     Sensor1.setXLoc(0); Sensor1.setYLoc(0);
 
    //-------------------------------------------------//
    // Horizontal movement through the centre of sector
    // Horz movement Angle Sensor
    /// this brings up ever  - because something somewhere needs an approx.equal///
    //std::cout<<"Test CapInter: 2"<< std::endl;
    //Angle Sensor and Horizontal movement. Should get captured:
    //  - At detector1 line (X=-0.5,Y=0.5,T=0)
    //  - At detector2 line  (X=0.5,Y=0.5,T=1)
    //  - Not at Circle line (X=0 ,Y= ,T= )
     double v1 = 0.5; double v2 = 0.5; double v3 = -0.5; double v4 = 0.5;
     int v5 =1;
     double v6 =M_PI/2;
     int v7 =1;
     std::cout<<"Hello 2"<<std::endl;

    Sensor1.CapturesIntersection(v1,v2,v3,v4, // Current x/y, previous x,y
                        v5, // ID
                        M_PI, //Call width - circular call (to make this easier!)
                        v6,  // move_angle,
                        v7); // IT
     std::vector<std::vector<double>> caps = Sensor1.getCaptures();
    loc1 =0; loc2 =0;
    for(int i=0; i<caps.size(); i++){
        if(caps[i].size()>0){
            std::cout<<"Hello "<<std::endl;
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"2a" <<std::endl;exit (EXIT_FAILURE);};
 
            //X,Y,TIME
            std::cout<<caps[i][4]<<", " <<caps[i][5]<<", "  <<caps[i][6]<<std::endl;
 
            if(caps[i][4] ==0.5 && caps[i][5] ==0.5){loc1 += 1;};
            if(caps[i][4] ==-0.5 && caps[i][5] ==0.5){loc2 += 1;};
            if(caps[i][6]<0 || caps[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"2b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< caps[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=1 || loc2!=1){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"2c" <<std::endl;
        exit (EXIT_FAILURE);
    };
 };


 void SensorTest::Test_CapturesIntersection3(){
 
    int loc1 =0;
    int loc2 =0;
    double temp;
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/4, 1);
 
    //-------------------------------------------------//
    // Angle movement through the centre of sector
    // Angle movement Angle Sensor
    // Angle Sensor and angle movement. Should get captured:
    //  - At detector1 line (X=-0.5,Y=0.5,T=0)
    //  - Not at detector2 line  (X= ,Y= ,T= )
    //  - At Circle line (X=0 ,Y=1 ,T=1)
    //  - At end of the step (X=0 ,Y=1 ,T=1)
    //std::cout<<"Test CapInter: 3"<< std::endl;
    temp= 1/sqrt(2);
     double v1 = 0; double v2 = 1; double v3 = -0.5; double v4 = 0.5;
     int v5 =1;
     double v6 =M_PI;
     int v7 =1;
    Sensor1.CapturesIntersection(v1,v2,v3,v4,  // Current x/y, previous x,y
                        v5, // ID
                        M_PI, //Call width - circular call (to make this easier!)
                        v6,  // move_angle,
                        v7); // IT
    loc1 =0; loc2 =0;
     std::vector<std::vector<double>> caps = Sensor1.getCaptures();
    for(int i=0; i<caps.size(); i++){
        if(caps[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"3a" <<std::endl;exit (EXIT_FAILURE);};
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            if(caps[i][4] ==-0.5 && caps[i][5] ==0.5){loc1 += 1;};
            if(caps[i][4] ==0 && caps[i][5] ==1){loc2 += 1;};
            if(caps[i][6]<0 || caps[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"3b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< caps[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=1 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"3c" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

 
void SensorTest::Test_CapturesIntersection4(){
    
    
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/4, 1);
    //-------------------------------------------------//
    // Horizontal movement through top of the sector, through both edges and the circle twice
    //Angle Sensor and horz movement. Should get captured:
    //  - At detector1 line (X=-1/sqrt(2),Y=1/sqrt(2),T=0)
    //  - At detector2 line  (X=1/sqrt(2),Y=1/sqrt(2),T=1)
    //  - At Circle line (X=1/sqrt(2),Y=1/sqrt(2),T=1) && (X=-1/sqrt(2),Y=1/sqrt(2),T=0)
    //  - At end of the step (X=1/sqrt(2),Y=1/sqrt(2), T=1)
    //std::cout<<"Test CapInter: 4"<< std::endl;
    double temp= 1/sqrt(2); double negtemp = -temp;
    int v5 =1;
    double v6 =M_PI_2;
    int v7 =1;
    Sensor1.CapturesIntersection(temp,temp,negtemp,temp, // Current x/y, previous x,y
                        v5, // ID
                        M_PI, //Call width - circular call (to make this easier!)
                        v6,  // move_angle,
                        v7); // IT
    int loc1 =0; int loc2 =0;
    std::vector<std::vector<double>> caps = Sensor1.getCaptures();
    for(int i=0; i<caps.size(); i++){
        if(caps[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"4a" <<std::endl;exit (EXIT_FAILURE);};
            //X,Y,TIME
            //std::cout<<"Captures " <<std::endl;
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            if(Sensor1.approximatelyequal(caps[i][4],-temp)==1 && Sensor1.approximatelyequal(caps[i][5],temp)==1){loc1 += 1;};
            if(Sensor1.approximatelyequal(caps[i][4],temp)==1 && Sensor1.approximatelyequal(caps[i][5],temp)==1){loc2 += 1;};
            if(caps[i][6]<0 || caps[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"4b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< caps[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=3){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"4c" <<std::endl;
        exit (EXIT_FAILURE);
    };
 };

 void SensorTest::Test_CapturesIntersection5(){
 Sensor Sensor1;
 Sensor1 = Sensor(1, M_PI/4, 1);
    //-------------------------------------------------//
    // Angle movement through on edge of sector
    //Angle Sensor and same-angle movement. Should get captured:
    //  - At detector1 line (X=0,Y=0,T=0)
    //  - At detector2 (same angle) line  (X=0,Y=0,T=0)
    //  - At Circle line (X=1/sqrt(2),Y=1/sqrt(2), T=1)
    //  - At end of the step (X=1/sqrt(2),Y=1/sqrt(2), T=1)
    /// this brings up ever  - because something somewhere needs an approx.equal
    //std::cout<<"Test CapInter: Number 5"<< std::endl;
     double temp= 1/sqrt(2); double v3 =0; double v4 =0;
     int v5 =1;
     double v6 =M_PI_4;
     int v7 =1;
     
    Sensor1.CapturesIntersection(temp,temp,v3,v4, // Current x/y, previous x,y
                        v5, // ID
                        M_PI, //Call width - circular call (to make this easier!)
                        v6,  // move_angle,
                        v7); // IT
    int loc1 =0; int loc2 =0;
     std::vector<std::vector<double>> caps = Sensor1.getCaptures();

    for(int i=0; i<caps.size(); i++){
        if(caps[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"5a" <<std::endl;exit (EXIT_FAILURE);};
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            if(Sensor1.approximatelyequal(caps[i][4],0) ==1 && Sensor1.approximatelyequal(caps[i][5],0) ==1 ){loc1 += 1;};
            if(Sensor1.approximatelyequal(caps[i][4],temp) ==1 && Sensor1.approximatelyequal(caps[i][5],temp) ==1 ){loc2 += 1;};
            if(caps[i][6]<0 || caps[i][6]>1){
                    std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"5b" <<std::endl;
                    std::cout<<"Time of capture, less than 0 or greater than 1: "<< caps[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"5c" <<std::endl;
        exit (EXIT_FAILURE);
    };
 }

 void SensorTest::Test_CapturesIntersection6(){
 Sensor Sensor1;
 Sensor1 = Sensor(1, M_PI/4, 1);
    //-------------------------------------------------//
    // Angle movement through on edge of sector
    //Angle Sensor and same-angle movement. Should get captured:
    //  - At detector1 (same angle) line (X=0,Y=0,T=0)
    //  - At detector2 line  (X=0,Y=0,T=0)
    //  - At Circle line (X=-1/sqrt(2),Y=1/sqrt(2), T=1)
    //  - At end of the step (X=-1/sqrt(2),Y=1/sqrt(2), T=1)
    //std::cout<<"Test CapInter: Number 6"<< std::endl;
     double temp= 1/sqrt(2);
     double v1 = -temp; double v2 = temp; double v3 =0; double v4 =0;
     int v5 =1;
     double v6 =7*M_PI_4;
     int v7 =1;
    Sensor1.CapturesIntersection(v1,v2,v3,v4, // Current x/y, previous x,y
                        v5, // ID
                        M_PI, //Call width - circular call (to make this easier!)
                        v6,  // move_angle,
                        v7); // IT
    int loc1 =0; int loc2 =0;
     std::vector<std::vector<double>> caps = Sensor1.getCaptures();
    for(int i=0; i<caps.size(); i++){
        if(caps[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"6a" <<std::endl;exit (EXIT_FAILURE);};
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            if(Sensor1.approximatelyequal(caps[i][4],0) ==1 && Sensor1.approximatelyequal(caps[i][5],0) ==1 ){loc1 += 1;};
            if(Sensor1.approximatelyequal(caps[i][4],-temp) ==1 && Sensor1.approximatelyequal(caps[i][5],temp) ==1 ){loc2 += 1;};
            if(caps[i][6]<0 || caps[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"6b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< caps[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"6c" <<std::endl;
        exit (EXIT_FAILURE);
    };
 };

 void SensorTest::Test_CapturesIntersection7(){
 Sensor Sensor1;
 Sensor1 = Sensor(1, M_PI/4, 1);
    //-------------------------------------------------//
    // Vert movement Vert Sensor
    // Vert movement Horz Sensor
    /// this brings up ever  - because something somewhere needs an approx.equal///
    //std::cout<<"Test CapInter: Number 7"<< std::endl;
    //Vertical/horzontal Sensor and veritcal movement. Should get captured:
    //  - AT horzontal detector2 line (X=0,Y=0,T=0)
    //  - At vertical detcector1 line  (X=0,Y=0,T=0)
    //  - At Circle line (X=0,Y=1, T=1)
    //  - At end of the step (X=0,Y=1, T=1)
     double v1 = 0; double v2 = 1; double v3 =0; double v4 =0;
     int v5 =1;
     double v6 =0;
     int v7 =1;
    Sensor1.CapturesIntersection(v1,v2,v3,v4, // Current x/y, previous x,y
                        v5, // ID
                        M_PI, //Call width - circular call (to make this easier!)
                        v6,  // move_angle,
                        v7); // IT
   int loc1 =0; int loc2 =0;
     std::vector<std::vector<double>> caps = Sensor1.getCaptures();
    for(int i=0; i<caps.size(); i++){
        if(caps[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"7a" <<std::endl;exit (EXIT_FAILURE);};
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            if(Sensor1.approximatelyequal(caps[i][4],0) ==1 && Sensor1.approximatelyequal(caps[i][5],0) ==1 ){loc1 += 1;};
            if(Sensor1.approximatelyequal(caps[i][4],0) ==1 && Sensor1.approximatelyequal(caps[i][5],1) ==1 ){loc2 += 1;};
            if(caps[i][6]<0 || caps[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"7b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< caps[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"7c" <<std::endl;
        exit (EXIT_FAILURE);
    };
}

 void SensorTest::Test_CapturesIntersection8(){
 Sensor Sensor1;
 Sensor1 = Sensor(1, M_PI/4, 1);
    //-------------------------------------------------//
    // Horz movement Vert Sensor
    // Horz movement Horz Sensor
    /// this brings up ever  - because something somewhere needs an approx.equal///

    //std::cout<<"Test CapInter: Number 8"<< std::endl;
    //Vertical/horzontal Sensor and horiz movement. Should get captured:
    //  - AT horzontal detector2 line (X=0,Y=0,T=0)
    //  - At vertical detcector1 line  (X=0,Y=0,T=0)
    //  - At Circle line (X=1,Y=0, T=1)
    //  - At end of the step (X=1,Y=0, T=1)
     double v1 = 1; double v2 = 0; double v3 =0; double v4 =0;
     int v5 =1;
     double v6 =M_PI_2;
     int v7 =1;
    Sensor1.CapturesIntersection(v1,v2,v3,v4, // Current x/y, previous x,y
                        v5, // ID
                        M_PI, //Call width - circular call (to make this easier!)
                        v6,  // move_angle,
                        v7); // IT
    int loc1 =0; int loc2 =0;
     std::vector<std::vector<double>> caps = Sensor1.getCaptures();
    for(int i=0; i<caps.size(); i++){
        if(caps[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"8a" <<std::endl;exit (EXIT_FAILURE);};
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl
            if(Sensor1.approximatelyequal(caps[i][4],0) ==1 && Sensor1.approximatelyequal(caps[i][5],0) ==1 ){loc1 += 1;};
            if(Sensor1.approximatelyequal(caps[i][4],1) ==1 && Sensor1.approximatelyequal(caps[i][5],0) ==1 ){loc2 += 1;};
            if(caps[i][6]<0 || caps[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"8b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< caps[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=2 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"8c" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void SensorTest::Test_CapturesIntersection9(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/4, 1);
    //-------------------------------------------------//
    // Angle movement Vert Sensor
    // Angle movement Horz Sensor
    /// this brings up ever  - because something somewhere needs an approx.equal///
    //std::cout<<"Test CapInter: Number 9"<< std::endl;
    //Vertical/horzontal Sensor and angle movement. Should get captured:
    //  - AT horzontal detector1 line (X=0,Y=0.5,T=0)
    //  - At vertical detcector1 line  (X=0.5,Y=0,T=1)
    //  - NOT at Circle line (X= ,Y=, T=)
    //  - At end of the step (X=0.5,Y=0, T=1)
    double v1 = 0.5; double v2 = 0; double v3 =0; double v4 =0.5;
    int v5 =1;
    double v6 =M_PI_4*3;
    int v7 =1;
    Sensor1.CapturesIntersection(v1,v2,v3,v4, // Current x/y, previous x,y
                        v5, // ID
                        M_PI, //Call width - circular call (to make this easier!)
                        v6,  // move_angle,
                        v7); // IT
    int loc1 =0; int loc2 =0;
     std::vector<std::vector<double>> caps = Sensor1.getCaptures();

    for(int i=0; i<caps.size(); i++){
        if(caps[i].size()>0){
            // The maximum number of captures possible for any 1 movement is 5
            if(i>4){std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"9a" <<std::endl;exit (EXIT_FAILURE);};
            //X,Y,TIME
            //std::cout<<Captures[i][4]<<", " <<Captures[i][5]<<", "  <<Captures[i][6]<<std::endl;
            if(Sensor1.approximatelyequal(caps[i][4],0) ==1 && Sensor1.approximatelyequal(caps[i][5],0.5) ==1 ){loc1 += 1;};
            if(Sensor1.approximatelyequal(caps[i][4],0.5) ==1 && Sensor1.approximatelyequal(caps[i][5],0) ==1 ){loc2 += 1;};
            if(caps[i][6]<0 || caps[i][6]>1){
                std::cout<<"Possible Error! Failed Sensor test - TestCapturesIntersection: "<<"9b" <<std::endl;
                std::cout<<"Time of capture, less than 0 or greater than 1: "<< caps[i][6] <<std::endl;
            };
        };
    };
    //std::cout<<"loc1: "<< loc1<<", loc2: "<< loc2<< std::endl;
    if(loc1!=1 || loc2!=2){
        std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"9c" <<std::endl;
        exit (EXIT_FAILURE);
    };
 };

 void SensorTest::Test_CapturesIntersection10(){
    Sensor Sensor1;
    Sensor1 = Sensor(1, M_PI/4, 1);
    //-------------------------------------------------//
    // Angle movement not near Sensor
    //Angle Sensor and vertical movement. Should get NO captured:
    //  - At detector1 line (X= ,Y= ,T=)
    //  - At detector2 line  (X= ,Y= ,T=)
    //  - At Circle line (X= ,Y= ,T=)
    //  - At end of the step (X= ,Y= ,T=)
    //std::cout<<"Test CapInter"<< std::endl;
     double v1 = 100; double v2 = 100; double v3 =99; double v4 =99;
     int v5 =1;
     double v6 =M_PI_4;
     int v7 =1;
    Sensor1.CapturesIntersection(v1,v2,v3,v4, // Current x/y, previous x,y
                        v5, // ID
                        M_PI, //Call width - circular call (to make this easier!)
                        v6,  // move_angle,
                        v7); // IT
     std::vector<std::vector<double>> caps = Sensor1.getCaptures();

    for(int i=0; i<caps.size(); i++){
        if(caps[i].size()>0){
            // Should have No Captures
            std::cout<<"Error! Failed Sensor test - TestCapturesIntersection: "<<"10a " << caps.size() <<std::endl;
            exit (EXIT_FAILURE);
        };
    };
            
    std::cout<<"Passed! Sensor test - TestSensorAndMovement"<< std::endl;
};


 


 //--------END OF TESTS FOR INTERSECTION  ---------------//

/*
 Sensor1.TestCI();
Sensor1.TestVertAndAngleInteraction();
Sensor1.TestHorzAndAngleInteraction();
Sensor1.TestAngleAndAngleInteraction();
Sensor1.TestHorzAndCircInteraction();
Sensor1.TestVertAndCircInteraction();
Sensor1.TestAngleAndCircInteraction();
Sensor1.TestTimeAndAngleCal();
Sensor1.TestGradientFromAngle();
Sensor1.TestCapturesIntersection();*/