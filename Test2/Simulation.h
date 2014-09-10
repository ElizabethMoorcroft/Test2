//
//  Simulation.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 21/11/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#ifndef __Test2__Simulation__
#define __Test2__Simulation__

#include <iostream>
#include <cstdio>
#include <math.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

//Header files
#include "Animal.h"
#include "RandNum.h"
#include "Sensors.h"
#include "Parameters.h"
#include "CheckSensor.h"
#include "AnimalMovement.h"


class Simulation {

public:
    Simulation();
    Simulation(double, double, double);
    std::string make_filename( const std::string& directory ,const std::string& basename);
    std::string make_directory( const std::string& directory, double,double, double);

};

#endif /* defined(__Test2__Simulation__) */
