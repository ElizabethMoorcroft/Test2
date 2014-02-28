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
    CheckSensor(std::vector<double>, std::vector<double>, std::vector<Sensor*>, std::ofstream &Movement, std::ofstream &Captures , int, int, int);
};

#endif /* defined(__Test2__CheckSensor__) */
