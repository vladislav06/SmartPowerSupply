//
// Created by vm on 24.17.1.
//

#include "rollingAverage.h"


float RollingAverage::getAverage() {
    return average;
}


void RollingAverage::addPoint(int point) {
    points.erase(points.begin());
    points.push_back(point);
    average += (1 / (float) points.size()) * (float) (point - points[0]);
}
