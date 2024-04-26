//
// Created by vm on 24.17.1.
//

#include "rollingAverage.h"


float RollingAverage::getAverage() {
    return average;
}


void RollingAverage::addPoint(float point) {
    points.erase(points.begin());
    points.push_back(point);
    average = 0;
    for (float p: points) {
        average += p;
    }
    average /= points.size();
    //average += (1 / (float) points.size()) * (float) (point - points[0]);
}
