//
//  Sensor.h
//  Test2
//
//  Created by Elizabeth Moorcroft on 28/03/2013.
//  Copyright (c) 2013 Elizabeth Moorcroft. All rights reserved.
//
#include <iostream>
#include "Parameters.h"
#include <cstdio>
#include <math.h>
#include <time.h>
#include <vector>
#ifndef Test2_Sensors_h
#define Test2_Sensors_h

class Sensor {
    
private:
    int Sensor_identifier;
    int Sensor_StepOn;
    
    double location_x;
    double location_y;
    
    double radius;
    double angle; // Direction of the Sensor
    double angle_HalfWidth; // Half width of the Sensor
    int capturecount;
    
    double g_detector1; // gradient of dectector1
    double g_detector2; // gradient of dectector1
    double m_detector1;
    double m_detector2;
    double c_detector1;
    double c_detector2;
    int vh_det1; // If the line describing detctor1 is vertical =1; horozonatl =2; else=0
    int vh_det2; // If the line describing detctor1 is vertical =1; horozonatl =2; else=0
        
    std::vector<std::vector<double> > Captures;
    std::vector<double> myvector;
    
public:
    Sensor();
    Sensor(int, double, double);
    std::vector<std::vector<double> > getCaptures(){return Captures;};
    
    void resetCaptures();
    
    // "Set functions"
    void setXLoc(double a);
    void setYLoc(double a);
    void setAngle(double a);
    void setHalfAngle(double a);
    void setRadius(double a){radius = a;};
    void setM_detector1(double a){m_detector1 = a;};
    void setM_detector2(double a){m_detector2 = a;};
    void setC_detector1(double a){c_detector1 = a;};
    void setC_detector2(double a){c_detector2 = a;};
    void setVH_det1(int a){vh_det1 = a;}; // If the line describing detctor1 is vertical =1; horozonatl =2; else=0
    void setVH_det2(int a){vh_det2 = a;};
    void setStepOn(int a){Sensor_StepOn = a;};
    // Functions to alter StepOn
    void Add1StepOn();
    void ResetStepOn();
    
    
    // "Get" functions
    double getID(){return Sensor_identifier;};
    double getStepOn(){return Sensor_StepOn;};
    double getXloc(){return location_x;};
    double getYloc(){return location_y;};
    double getAngle(){return angle;};
    double getHalfAngle(){return angle_HalfWidth;};
    double getRadius(){return radius;};

    
    // Range of angle between 0 and 2pi
    double RangeAngle(double);
    double RangeAngleMinus(double);
    double DistTwoPoints(double, double, double, double);
    double AngleTwoPoints(double, double, double, double);
    
    bool approximatelyequal(double, double);
    
    std::vector <double> AngleAndCircInteraction(double, double);
    std::vector <double> VertAndCircInteraction(double);
    std::vector <double> HorzAndCircInteraction(double, double, double, double);
    std::vector <double> AngleAndAngleInteraction(double, double, double, double);
    double HorzAndAngleInteraction(double, double, double);
    double VertAndAngleInteraction(double, double, double);

    
    
    std::vector <double> TimeAndAngleCal(double, double, double, double, double);
    double GradientFromAngle(double);
    void UpdateCaptures(int,int,double,double,double,double,double, double, std::ofstream &);
    void CapturesIndividual(double, double, int, double, int, double, std::ofstream &);
    
    void CapturesIntersection(double, double, double, double, int, double, int, std::ofstream &);
    
    void SensorAndMovement(double, double, double, double,  int,  double,
                        int, double, double, double, double, double, int, double, std::ofstream &);
    void SensorCircAndMovement(double, double, double, double, int, double,
                            int, double, double, double, std::ofstream & );

    
};

#endif
