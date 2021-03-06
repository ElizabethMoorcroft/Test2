//
//  Iteration.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 21/11/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#ifndef __Test2__Iteration__
#define __Test2__Iteration__
#include <iostream>
#include <cstdio>
#include <math.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "Animal.h"
#include "AnimalMovement.h"
#include "Sensors.h"
#include "CheckSensor.h"

class Iteration {
    
public:
    Iteration();
    Iteration(std::vector<Sensor*>, std::ofstream &Captures, std::ofstream &Movement, int, double, double, double);
};

#endif /* defined(__Test2__Iteration__) */
