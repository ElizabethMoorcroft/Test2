//
//  CheckSensor.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 21/11/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#ifndef __Test2__CheckSensor__
#define __Test2__CheckSensor__

#include <iostream>
#include <vector>
#include "Sensors.h"
#include "Parameters.h"

class CheckSensor {

public:
    CheckSensor();
    CheckSensor(double currentx,double currenty, double currentangle, double previousx,double previousy, std::vector<Sensor*>, std::ofstream &Movement, std::ofstream &Captures , int, int, int, double,double);
};

#endif /* defined(__Test2__CheckSensor__) */
