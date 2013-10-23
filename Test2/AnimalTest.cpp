//
//  AnimalTest.cpp
//  Test2
//
//  Created by Elizabeth Moorcroft on 22/10/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#include "AnimalTest.h"
#include "Animal.h"
#include "Parameters.h"

void AnimalTest::RunAnimalTests(){
    
    test_totaldistance(4);
    //std::cout<<"Passed - Animal total distance" <<std::endl;
    
    test_changeangle(0);
    test_changeangle(M_PI_4);
    //std::cout<<"Passed - Animal change in move angle" <<std::endl;
    
    test_displacement();
    test_maxdisplacement();
    //std::cout<<"Passed - Displacement" <<std::endl;
    
    test_CalNext_X1();
    test_CalNext_X2();
    test_CalNext_X3();
    test_CalNext_X4();
    test_CalNext_X5();
    test_CalNext_X6();
    test_CalNext_Y1();
    test_CalNext_Y2();
    test_CalNext_Y3();
    test_CalNext_Y4();
    test_CalNext_Y5();
    test_CalNext_Y6();
    //std::cout<<"Passed - Calculating next location" <<std::endl;
    
    test_RangeAngle1();
    test_RangeAngle2();
    test_RangeAngle3();
    test_RangeAngle4();
    //std::cout<<"Passed - Angle range cals" <<std::endl;
    
    test_LeaveEnterWorld1();
    test_LeaveEnterWorld2();
    test_LeaveEnterWorld3();
    test_LeaveEnterWorld4();
    //std::cout<<"Passed - Entering and leaving the world" <<std::endl;

    test_NewLocation1();
    test_NewLocation2();
    //std::cout<<"Passed - Calculating a random new location" <<std::endl;
    
    //std::cout<<"Passed all animal tests" <<std::endl;
    
};


void AnimalTest::test_displacement(){
    
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, Sq_MinX+ 1,Sq_MinY+ 1, M_PI_4);
    Animal1.setMove_speed(4);
    Animal1.setMove_maxangle(0);
    Animal1.setlargelocationvector(pow(10,7));
    
    double displacement =0;
    int error =0;
    double CX =Sq_MinX;     double CY =Sq_MinY;
    
    // Simulate 1mill next distances
    for(int i=0; i<1*pow(10,6); i++){
        srand(i);
        std::vector<double> RandomNumberStream(50);
        for(int j=0;j<50;j++){RandomNumberStream[j] = double (rand());};
        
        //std::cout<< i << " "<<RandomNumberStream[49] <<std::endl;

        Animal1.UpdateLocation(RandomNumberStream[49]);
        
        displacement = sqrt(pow(Animal1.getCurrentX() - CX,2)+pow(Animal1.getCurrentY() - CY,2));
        if(displacement==0){error+=1; std::cout<< i <<" CX: " <<CX <<" CY: "<<CY<<std::endl;};
        
        CX =Animal1.getCurrentX(); CY =Animal1.getCurrentY();
    };
    
    //The average speed must be with in 1% of the true speed.
    if(error > 0){
        std::cout<<"Error! Failed Animal test - Individual displacement" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    //return 0;
};

void AnimalTest::test_maxdisplacement(){
    
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, Sq_MinX+ 1,Sq_MinY+ 1, M_PI_4);
    Animal1.setMove_speed(4);
    Animal1.setMove_maxangle(0);
    Animal1.setlargelocationvector(pow(10,7));
    
    //double totalNextDist =0;
    
    // Simulate 1mill next distances
    for(int i=0; i<1*pow(10,6); i++){
        srand(i);
        std::vector<double> RandomNumberStream(50);
        for(int j=0;j<50;j++){RandomNumberStream[j] = double (rand());};
        
        Animal1.UpdateLocation(RandomNumberStream[49]);
        
        //totalNextDist += Animal1.getNextDist();
    };
    
    double displacement = sqrt(pow(Animal1.getCurrentX() - Sq_MinX,2)+pow(Animal1.getCurrentY() - Sq_MinY,2));
    
    
    //std::cout<<averagespeed<<std::endl;
    
    //The average speed must be with in 1% of the true speed.
    if(displacement > sqrt(pow(Sq_MaxX-Sq_MinX,2)+pow(Sq_MaxY-Sq_MinY,2))){
        std::cout<<"Error! Failed Animal test - Test total displacement" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    //return 0;
};



void AnimalTest::test_totaldistance(double a){
    
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, Sq_MinX+ 1,Sq_MinY+ 1, M_PI_4);
    Animal1.setMove_speed(a);
    Animal1.setMove_maxangle(0);
    Animal1.setlargelocationvector(pow(10,7));
    
    //double totalNextDist =0;
    
    // Simulate 1mill next distances
    for(int i=0; i<1*pow(10,6); i++){
        
        srand(i);
        std::vector<double> RandomNumberStream(50);
        for(int j=0;j<50;j++){RandomNumberStream[j] = double (rand());};
        
        //std::cout<< i  << " "<< RandomNumberStream[49]<<std::endl;
        Animal1.UpdateLocation(RandomNumberStream[49]);
        
        //totalNextDist += Animal1.getNextDist();
    };
    
    // Calculate the average speed
    double averagespeed = Animal1.getTotalDistance()/(StepLength*1*pow(10,6));
    
    //std::cout<<averagespeed<<std::endl;
    
    //The average speed must be with in 1% of the true speed.
    if(averagespeed< (a-0.01*a) || averagespeed>(a+0.01*a)){
        std::cout<<"Error! Failed Animal test - TestTotalDistance" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
    //return 0;
};

void AnimalTest::test_changeangle(double a){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, Sq_MinX+ 1,Sq_MinY+ 1, M_PI_4);
    Animal1.setMove_speed(4);
    Animal1.setMove_maxangle(a);
    Animal1.setlargelocationvector(pow(10,7));
    
    //double cangle = M_PI_4;
    
    std::vector<double> listofangles((1*pow(10,6)));
    std::vector<double> listofanglesdiff((1*pow(10,6)));
    srand(1);
    for(int i=0; i<1*pow(10,6); i++){
        //srand(i);
        std::vector<double> RandomNumberStream(50);
        for(int j=0;j<50;j++){RandomNumberStream[j] = double (rand());};
        
        //if(i<100){std::cout<<RandomNumberStream[49]<<std::endl;}
        Animal1.UpdateLocation(RandomNumberStream[49]);
        
        listofangles[i] = Animal1.getNextAngle();
        if(i>0 && ((listofangles[i]-listofangles[i-1])<a && (listofangles[i]-listofangles[i-1])>-a)){
            listofanglesdiff[i] =(listofangles[i]-listofangles[i-1]);
        } else if(i>0){listofanglesdiff[i]= M_PI*2-listofangles[i] -listofangles[i-1] ;};
        //cangle = Animal1.getNextAngle();
    };
    
    int errorcount = 0;
    int distributionerror1 =0;
    int distributionerror2 =0;
    int distributionerror3 =0;

    if(a ==0){
        for(int i=0; i<1*pow(10,6); i++){if((listofangles[i]- 0)<-0.0001 && (listofangles[i]- 0)>0.0001){errorcount +=1;};};
    }else{
        for(int i=1; i<1*pow(10,6); i++){
            if((listofanglesdiff[i]  < -a || listofanglesdiff[i] > +a)){
                errorcount +=1;
                std::cout<<i<< " "<<listofangles[i-1]<<  " "<<listofangles[i] <<" "<<listofanglesdiff[i]<< std::endl;
            }
            if(listofanglesdiff[i] < 0){distributionerror1 +=1;};
            if(listofanglesdiff[i] < -0.5*a){distributionerror2 +=1; };
            if(listofanglesdiff[i]< -0.9*a){distributionerror3 +=1;};
        };
    };

    //if(a>0){for(int i=0; i<1*pow(10,2); i++){std::cout<<listofangles[i]<<std::endl;}};
    //std::cout<<distributionerror1<<std::endl;
    if(errorcount>0){
        std::cout<<"Error! Failed Animal test - TestChangeInAngle " << a <<std::endl;
        exit (EXIT_FAILURE);
    };
     if(a !=0){
         if(distributionerror1>1*pow(10,6)*0.51 || distributionerror1<1*pow(10,6)*0.49){
             std::cout<<"Error! Failed Animal test - TestChangeInAngle (Distribution 1)" <<std::endl;
             exit (EXIT_FAILURE);
         }
         if(distributionerror2>1*pow(10,6)*0.26 || distributionerror2<1*pow(10,6)*0.22){
             std::cout<<"Error! Failed Animal test - TestChangeInAngle (Distribution 2)" << distributionerror2 <<std::endl;
             exit (EXIT_FAILURE);
         }
         if(distributionerror3>1*pow(10,6)*0.06 || distributionerror3<1*pow(10,6)*0.04){
             std::cout<<distributionerror3<<std::endl;
             std::cout<<"Error! Failed Animal test - TestChangeInAngle (Distribution 3)" <<std::endl;
             exit (EXIT_FAILURE);
         }
    }
    
};

/*------------------------------------------------------------------------------*/
//          Next location
/*------------------------------------------------------------------------------*/

void AnimalTest::test_CalNext_X1(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI);
    Animal1.setNextAngle( M_PI);
    double nextdistance = Animal1.CalNext_X(1);
    
    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if(abs(nextdistance-0)>0.00001){
        std::cout<<"Error! Failed Animal test - CalNext x1" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_CalNext_X2(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_2);
    Animal1.setNextAngle(M_PI_2);
    double nextdistance = Animal1.CalNext_X(1);
    
    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if(abs(nextdistance-1)>0.00001 || nextdistance<0){
        std::cout<<"Error! Failed Animal test - CalNext x2" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_CalNext_X3(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4);
    Animal1.setNextAngle(M_PI_4);
    double nextdistance = Animal1.CalNext_X(1);
    
    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if((nextdistance-(1/sqrt(2)))>0.0001 ||(nextdistance-(1/sqrt(2)))<-0.0001 || nextdistance<0){
        std::cout<<"Error! Failed Animal test - CalNext x3" <<std::endl;
        exit (EXIT_FAILURE);
    };
};
                     
void AnimalTest::test_CalNext_X4(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4*3);
    Animal1.setNextAngle(M_PI_4*3);
    double nextdistance = Animal1.CalNext_X(1);
    
    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if((nextdistance-(1/sqrt(2)))>0.0001 ||(nextdistance-(1/sqrt(2)))<-0.0001 || nextdistance<0){
        std::cout<<"Error! Failed Animal test - CalNext x4" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_CalNext_X5(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4*5);
    Animal1.setNextAngle(M_PI_4*5);
    double nextdistance = Animal1.CalNext_X(1);

    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if((nextdistance-(-1/sqrt(2)))>0.0001 ||(nextdistance-(-1/sqrt(2)))<-0.0001 || nextdistance>0){
        std::cout<<"Error! Failed Animal test - CalNext x5" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_CalNext_X6(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4*7);
    Animal1.setNextAngle(M_PI_4*7);
    double nextdistance = Animal1.CalNext_X(1);
    
    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if((nextdistance-(-1/sqrt(2)))>0.0001 ||(nextdistance-(-1/sqrt(2)))<-0.0001 || nextdistance>0){
        std::cout<<"Error! Failed Animal test - CalNext x6" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_CalNext_Y1(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI);
    // moving straight down
    Animal1.setNextAngle(M_PI);
    double nextdistance = Animal1.CalNext_Y(1);
    
    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if((nextdistance- -1)>0.0001 ||(nextdistance- -1)<-0.0001 || nextdistance>0){
        std::cout<<"Error! Failed Animal test - CalNext y1" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_CalNext_Y2(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_2);
    // moving straight down
    Animal1.setNextAngle(M_PI_2);
    double nextdistance = Animal1.CalNext_Y(1);
    
    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if((nextdistance-0)>0.0001 ||(nextdistance-0)<-0.0001 ){
        std::cout<<"Error! Failed Animal test - CalNext y2" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_CalNext_Y3(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4);
    // moving straight down
    Animal1.setNextAngle(M_PI_4);
    double nextdistance = Animal1.CalNext_Y(1);
    
    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if((nextdistance-(1/sqrt(2)))>0.0001 ||(nextdistance-(1/sqrt(2)))<-0.0001 || nextdistance < 0){
        std::cout<<"Error! Failed Animal test - CalNext y3" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_CalNext_Y4(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4*3);
    // moving straight down
    Animal1.setNextAngle(M_PI_4*3);
    double nextdistance = Animal1.CalNext_Y(1);
    
    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if((nextdistance-(-1/sqrt(2)))>0.0001 ||(nextdistance-(-1/sqrt(2)))<-0.0001 || nextdistance > 0){
        std::cout<<"Error! Failed Animal test - CalNext y4" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_CalNext_Y5(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4*5);
    // moving straight down
    Animal1.setNextAngle(M_PI_4*5);
    double nextdistance = Animal1.CalNext_Y(1);
    
    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if((nextdistance-(-1/sqrt(2)))>0.0001 ||(nextdistance-(-1/sqrt(2)))<-0.0001 || nextdistance > 0){
        std::cout<<"Error! Failed Animal test - CalNext y5" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_CalNext_Y6(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4*7);
    // moving straight down
    Animal1.setNextAngle(M_PI_4*7);
    double nextdistance = Animal1.CalNext_Y(1);
    
    //std::cout<<nextdistance<<std::endl;
    // The values have to be approximately equal
    if((nextdistance-(1/sqrt(2)))>0.0001 ||(nextdistance-(1/sqrt(2)))<-0.0001 || nextdistance < 0){
        std::cout<<"Error! Failed Animal test - CalNext y6" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

/*------------------------------------------------------------------------------*/
//         Angles +ve && within range
/*------------------------------------------------------------------------------*/

void AnimalTest::test_RangeAngle1(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4);
    double angle = Animal1.RangeAngle(0);
    if((angle-0)>0.0001 ||(angle-0)<-0.0001){
        std::cout<<"Error! Failed Animal test -RangeAngle 1" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_RangeAngle2(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4);
    double angle = Animal1.RangeAngle(2*M_PI);
    if((angle-0)>0.0001 ||(angle-0)<-0.0001){
        std::cout<<"Error! Failed Animal test -RangeAngle 2" <<std::endl;
        exit (EXIT_FAILURE);
    };
};

void AnimalTest::test_RangeAngle3(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4);
    double angle = Animal1.RangeAngle(M_PI);
    if((angle-M_PI)>0.0001 ||(angle-M_PI)<-0.0001){
        std::cout<<"Error! Failed Animal test -RangeAngle 3" <<std::endl;
        exit (EXIT_FAILURE);
    };
};


void AnimalTest::test_RangeAngle4(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4);
    double angle = Animal1.RangeAngle(-M_PI);
    if((angle-M_PI)>0.0001 ||(angle-M_PI)<-0.0001){
        std::cout<<"Error! Failed Animal test -RangeAngle 4" <<std::endl;
        exit (EXIT_FAILURE);
    };
};


/*------------------------------------------------------------------------------*/
//         Entering and leaving world
/*------------------------------------------------------------------------------*/

void AnimalTest::test_LeaveEnterWorld1(){
    
    const double maxx = 1000; const double minx = 0;
    const double maxy = 1000; const double miny = 0;
    
    srand(1);
    std::vector<double> RandomNumberMovement(101);
    for(int i=0; i<101; i++){RandomNumberMovement[i] = double(rand());};
    
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, maxx-1, (maxy-miny)/2, M_PI_4);
    
    // Movement based on 3,4,5 triangle -> 4 right, 3 up
    // as it is 1 short of the right boarder of world will move entre
    // the world after 1 right up and move a further 3 right after re-entry.
    Animal1.setNextAngle(asin(0.8));
    Animal1.setNextDist(5);
    Animal1.LeaveEnterWorld(maxy, maxx, miny, minx);
    
    //std::cout<< " X: "<< Animal1.getNextX() << " Y: "<< Animal1.getNextY() <<std::endl;
    if((Animal1.getNextX()-3)>0.0001    ||(Animal1.getNextX()-3)<-0.0001 ||
       (Animal1.getNextY()-503)>0.0001 ||(Animal1.getNextY()-503)<-0.0001){
        std::cout<<"Error! Failed Animal test -LeaveEnterWorld 1" <<std::endl;
        exit (EXIT_FAILURE);
    };

};

void AnimalTest::test_LeaveEnterWorld2(){
    
    const double maxx = 1000; const double minx = 0;
    const double maxy = 1000; const double miny = 0;
    
    srand(1);
    std::vector<double> RandomNumberMovement(101);
    for(int i=0; i<101; i++){RandomNumberMovement[i] = double(rand());};
    
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, (maxx-minx)/2, maxy-1, M_PI_4);
    
    // Movement based on 3,4,5 triangle -> 4 right, 3 up
    // as it is 1 short of the top boarder of world will move entre
    // the world after 1 up and move a further 2 up after re-entry.
    Animal1.setNextAngle(asin(0.8));
    Animal1.setNextDist(5);
    Animal1.LeaveEnterWorld(maxy, maxx, miny, minx);
    
    //std::cout<< " X: "<< Animal1.getNextX() << " Y: "<< Animal1.getNextY() <<std::endl;
    if((Animal1.getNextX()-504)>0.0001    ||(Animal1.getNextX()-504)<-0.0001 ||
       (Animal1.getNextY()-2)>0.0001 ||(Animal1.getNextY()-2)<-0.0001){
        std::cout<<"Error! Failed Animal test -LeaveEnterWorld 2" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
};


void AnimalTest::test_LeaveEnterWorld3(){
    
    const double maxx = 1000; const double minx = 0;
    const double maxy = 1000; const double miny = 0;
    
    srand(1);
    std::vector<double> RandomNumberMovement(101);
    for(int i=0; i<101; i++){RandomNumberMovement[i] = double(rand());};
    
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, (maxx-minx)/2, miny+1, 3*M_PI_4);
    
    // Movement based on 3,4,5 triangle -> 4 left, 3 down
    // as it is 1 short of the bottom boarder of world will move entre
    // the world after 1 down and move a further 2 down after re-entry.
    Animal1.setNextAngle(asin(0.8)+M_PI);
    Animal1.setNextDist(5);
    Animal1.LeaveEnterWorld( miny, minx, maxy, maxx);
    
    //std::cout<< " X: "<< Animal1.getNextX() << " Y: "<< Animal1.getNextY() <<std::endl;
    if((Animal1.getNextX()-496)>0.0001    ||(Animal1.getNextX()-496)<-0.0001 ||
       (Animal1.getNextY()-998)>0.0001 ||(Animal1.getNextY()-998)<-0.0001){
        std::cout<<"Error! Failed Animal test -LeaveEnterWorld 3" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
};

void AnimalTest::test_LeaveEnterWorld4(){
    
    const double maxx = 1000; const double minx = 0;
    const double maxy = 1000; const double miny = 0;
    
    srand(1);
    std::vector<double> RandomNumberMovement(101);
    for(int i=0; i<101; i++){RandomNumberMovement[i] = double(rand());};
    
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, minx+1, (maxy-miny)/2, M_PI_4*3);
    
    // Movement based on 3,4,5 triangle -> 4 left, 3 down
    // as it is 1 short of the left boarder of world will move entre
    // the world after 1 left and move a further 3 left after re-entry.
    Animal1.setNextAngle(asin(0.8)+M_PI);
    Animal1.setNextDist(5);
    Animal1.LeaveEnterWorld(miny, minx, maxy, maxx);
    
    //std::cout<< " X: "<< Animal1.getNextX() << " Y: "<< Animal1.getNextY() <<std::endl;
    if((Animal1.getNextX()-997)>0.0001    ||(Animal1.getNextX()-997)<-0.0001 ||
       (Animal1.getNextY()-497)>0.0001 ||(Animal1.getNextY()-497)<-0.0001){
        std::cout<<"Error! Failed Animal test -LeaveEnterWorld 4" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
};

/*------------------------------------------------------------------------------*/
//         New locations
/*------------------------------------------------------------------------------*/

void AnimalTest::test_NewLocation1(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI_4*3);
    
    double TotalDist=0;
    Animal1.setMove_maxangle(0);
    Animal1.setMove_speed(1);
    
    int error  = 0;
    double Angles = 0;
    int newlocation = 0;
    
    srand(1);
    std::vector<double> RandomNumberMovement(1*pow(10,6));
    for(int i=0; i<1*pow(10,6); i++){RandomNumberMovement[i] = double(rand());};
    for(int i=0; i<1*pow(10,6); i++){
        srand(RandomNumberMovement[i]);
        std::vector<double> RandomNumbertemp(1*pow(10,3));
        for(int i=0; i<1*pow(10,3); i++){RandomNumbertemp[i] = double(rand());};
        Animal1.NewLocation(RandomNumbertemp[33],RandomNumbertemp[66]);
        TotalDist += sqrt(pow(Animal1.getNextX()-0,2)+pow(Animal1.getNextY()-0,2));
        Angles = Animal1.getNextAngle();
        if((Angles- M_PI_4*3)>0.0001 || (Angles- M_PI_4*3)< -0.0001){error +=1;};
        if(Animal1.getNextX()!=0 && Animal1.getNextY()!=0 ){newlocation +=1;};
    };
    
    // Calculate the average speed
    double averagespeed = TotalDist/(StepLength*1*pow(10,6));
    
    //std::cout<<averagespeed<<std::endl;
    
    //The average speed must be with in 1% of the true speed.
    if(averagespeed< (1-0.01*1) || averagespeed>(1+0.01*1) || error>0 || newlocation!= pow(10,6)){
        std::cout<<"Error! Failed Animal test - Test New Location 1" <<std::endl;
        exit (EXIT_FAILURE);
    };

};

void AnimalTest::test_NewLocation2(){
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI);
    
    double TotalDist=0;
    Animal1.setMove_maxangle(M_PI);
    Animal1.setMove_speed(1);
    std::vector<double> Angles(1*pow(10,6));
    double anglelesspi = 0;
    double anglelesspi_2 = 0;
    double anglelesspi_4 = 0;
    double anglelesspi_8 = 0;
    int newlocation  =0;
    
    srand(100);
    std::vector<double> RandomNumberMovement(1*pow(10,8));
    for(int i=0; i<1*pow(10,8); i++){RandomNumberMovement[i] = double(rand());};
    
    
    for(int i=0; i<1*pow(10,6); i++){
        srand(RandomNumberMovement[i*100]);
        std::vector<double> RandomNumbertemp(1*pow(10,2));
        for(int j=0; j<1*pow(10,2); j++){RandomNumbertemp[j] = double(rand());};
        Animal1.NewLocation(RandomNumbertemp[33],RandomNumbertemp[66]);
        TotalDist += sqrt(pow(Animal1.getNextX()-0,2)+pow(Animal1.getNextY()-0,2));
        Angles[i] += Animal1.getNextAngle();
        if(Angles[i]<M_PI){anglelesspi +=1;};
        if(Angles[i]<M_PI_2){anglelesspi_2 +=1;};
        if(Angles[i]<M_PI_4){anglelesspi_4 +=1;};
        if(Angles[i]<M_PI/8){anglelesspi_8 +=1;};
        if(Animal1.getNextX()!=0 && Animal1.getNextY()!=0 ){newlocation +=1;};

    };
    
    // Calculate the average speed
    double averagespeed = TotalDist/(StepLength*1*pow(10,6));
    
    //std::cout<<averagespeed<<std::endl;
    //std::cout<< "anglelesspi_8: "<< anglelesspi_8 <<" anglelesspi_4: " << anglelesspi_4<<std::endl;
    
    //The average speed must be with in 1% of the true speed.
    if(averagespeed< (1-0.01*1) || averagespeed>(1+0.01*1)
       || anglelesspi < 0.49*pow(10,6) || anglelesspi > 0.51*pow(10,6)
       || anglelesspi_2 < 0.24*pow(10,6) || anglelesspi_2 > 0.26*pow(10,6)
       || anglelesspi_4 < 0.124*pow(10,6) || anglelesspi_4 > 0.126*pow(10,6)
       || anglelesspi_8 < 0.062*pow(10,6) || anglelesspi_8 > 0.063*pow(10,6)
       || newlocation!= pow(10,6) // all locations must differ from the start location
       ){
        std::cout<<"Error! Failed Animal test - Test New Location 2" <<std::endl;
        exit (EXIT_FAILURE);
    };
    
};



