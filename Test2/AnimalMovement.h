//
//  AnimalMovement.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 21/11/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//

#ifndef __Test2__AnimalMovement__
#define __Test2__AnimalMovement__

#include <iostream>
#include <vector>
#include "Animal.h"
#include "Sensors.h"
#include "Parameters.h"

class AnimalMovement {
    
public:
    AnimalMovement();
    AnimalMovement( std::vector<Animal*> ,  std::vector<Sensor*>, std::ofstream &Captures, std::ofstream &Movement, double, double, int, int);
};

#endif /* defined(__Test2__AnimalMovement__) */
