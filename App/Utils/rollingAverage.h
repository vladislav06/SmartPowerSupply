//
// Created by vm on 24.17.1.
//
#pragma once

#include <vector>


class RollingAverage {
public:
    explicit RollingAverage(int numberOfPoints) {
        points.reserve(numberOfPoints);
        for (int i = 0; i < numberOfPoints; i++) {
            points.push_back(0);
        }
    }


    /// Get calculated rolling average
    float getAverage();

    void addPoint(float point);

private:
    /// Fixed length list that contains points
    std::vector<float> points{};

    /// Calculated average
    float average = 0;

};

