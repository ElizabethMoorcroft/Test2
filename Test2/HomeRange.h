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
        
    double Home_Size;  // Size of HR
    double Home_X;  //Location of Home Range X
    double Home_Y;  //Location of Home Range Y
    
    int sex;
    
    //std::vector<double> HRvalues; // Size of HR & Location of Home Range X&Y

public:
    
    HomeRange();
    HomeRange(int, double); //identifier, seed
    
    double getSex(){return sex;};
    double getHomeSize(){return Home_Size;};
    double getHomeX(){return Home_X;};
    double getHomeY(){return Home_Y;};
    double getHomeID(){return identifier;};
    
    void CheckForOverlap(std::vector<std::vector<double>>, double);
    void GridDetectors(double, double);
    void RecalGridDetectors(double);
    
};

#endif
