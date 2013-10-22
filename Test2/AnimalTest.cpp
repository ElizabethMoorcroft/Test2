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
    
    //std::cout<<"Hello"<<std::endl;
    
    test_totaldistance(4);
    std::cout<<"Passed - Animal total distance" <<std::endl;
    test_changeangle(0);
    std::cout<<"Passed - Animal change in move angle" <<std::endl;
    test_changeangle(M_PI_4);
    std::cout<<"Passed - Animal change in move angle" <<std::endl;
    
    std::cout<<"Animal tests over" <<std::endl;
    
};


void AnimalTest::test_totaldistance(double a){
    
    // Set up new animal
    Animal Animal1;
    Animal1 = Animal(1, 0, 0, M_PI);
    Animal1.setMove_speed(a);
    double totalNextDist =0;
    
    // Simulate 1mill next distances
    for(int i=0; i<1*pow(10,6); i++){
        srand(i);
        std::vector<double> RandomNumberStream(100);
        for(int j=0;j<100;j++){RandomNumberStream[j] = double (rand());};
        
        Animal1.NewLocation(RandomNumberStream[33] , RandomNumberStream[66] );
        
        totalNextDist += Animal1.getNextDist();
    };
    
    // Calculate the average speed
    double averagespeed = totalNextDist/(StepLength*1*pow(10,6));
    
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
    Animal1 = Animal(1, 0, 0, M_PI);
    Animal1.setMove_maxangle(a);
    std::vector<double> listofangles((1*pow(10,6)));
    for(int i=0; i<1*pow(10,6); i++){
        srand(i);
        std::vector<double> RandomNumberStream(100);
        for(int j=0;j<100;j++){RandomNumberStream[j] = double (rand());};
        
        Animal1.NewLocation(RandomNumberStream[33] , RandomNumberStream[66] );
        
        listofangles[i] = Animal1.getNextAngle();
    };
    
    int errorcount = 0;
    int distributionerror1 =0;
    int distributionerror2 =0;
    int distributionerror3 =0;

    if(a ==0){
        for(int i=0; i<1*pow(10,6); i++){if( listofangles[i] != M_PI){errorcount +=1;};};
    }else{
        for(int i=0; i<1*pow(10,6); i++){
            if(listofangles[i] < M_PI-a || listofangles[i] > M_PI+a){errorcount +=1;};
            if(listofangles[i] < M_PI){distributionerror1 +=1;};
            if(listofangles[i] < M_PI-0.5*a){distributionerror2 +=1;};
            if(listofangles[i] < M_PI-0.9*a){distributionerror3 +=1;};
        };
    };

    //for(int i=0; i<1*pow(10,2); i++){std::cout<<listofangles[i]<<std::endl;}
    //std::cout<<errorcount<<std::endl;
    //std::cout<<distributionerror1<<std::endl;
    if(errorcount>1){
        std::cout<<"Error! Failed Animal test - TestChangeInAngle" <<std::endl;
        exit (EXIT_FAILURE);
    };
     if(a !=0){
         if(distributionerror1>1*pow(10,6)*0.51 || distributionerror1<1*pow(10,6)*0.49){
             std::cout<<"Error! Failed Animal test - TestChangeInAngle (Distribution 1)" <<std::endl;
             exit (EXIT_FAILURE);
         }
         if(distributionerror2>1*pow(10,6)*0.26 || distributionerror2<1*pow(10,6)*0.24){
             std::cout<<"Error! Failed Animal test - TestChangeInAngle (Distribution 2)" <<std::endl;
             exit (EXIT_FAILURE);
         }
         if(distributionerror3>1*pow(10,6)*0.06 || distributionerror3<1*pow(10,6)*0.04){
             std::cout<<distributionerror3<<std::endl;
             std::cout<<"Error! Failed Animal test - TestChangeInAngle (Distribution 3)" <<std::endl;
             exit (EXIT_FAILURE);
         }
    }
    
};

