//
//  HomeRange.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 24/02/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#ifndef Test2_HomeRange_h
#define Test2_HomeRange_h
#include "vector"

class HomeRange{
    
    int identifier; //The HR id number
    int type;       //HR exists, 1=Y 0=N
    int Home_Shape; // Shape of HR, 1=Circle
    
    
    double Home_Size;  // Size of HR
    double Home_X;  //Location of Home Range X
    double Home_Y;  //Location of Home Range Y
    

    
    //std::vector<double> HRvalues; // Size of HR & Location of Home Range X&Y

public:
    
    HomeRange();
    HomeRange(int //identifier; //The HR id number
             ,int //type;       //Type of HR, 1=Y 0=N
             ,int //Home_Shape; // Shape of HR, 1=Circle
            );

    void HRSetValues( double, double, double, double,
                     double, double, double, double,
                     double, double);

    double getHomeX(){return Home_X;};
    double getHomeY(){return Home_Y;};
    double getHomeID(){return identifier;};
    
};

#endif
