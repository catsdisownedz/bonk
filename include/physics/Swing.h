#pragma once
#include <utility>
#include <vector>
#include "Platform.h"
#include "GameObject.h"

// struct PlatformBounds {
//     double left, right, top, bottom;
// };

class Swing {
private:
    std::pair<double, double> ropeStart, ropeEnd;
    //Platform board;
    pair<double, double> leftCenter;
    pair<double, double> rightCenter;
    double boardLength;
    double boardWidth;
    double angle = 3.14 * 1.5;
    double ropeLength;
    double leanAmount = 0.1;


public:
    Swing(std::pair<double, double> top, std::pair<double, double> bottom, double boardLength, double boardWidth);
    vector<pair<double,double>> leftPlatBounds;
    vector<pair<double,double>> rightPlatBounds;
    void assignLeftPlatBounds(int position, pair<double, double> vertex);
    void assignRightPlatBounds(int position, pair<double, double> vertex);
    void draw();
    double calculateDiagonals();
    pair<double, double> getPivot();
    double angularVelocity;
    void updateSwingPhysics(double deltaTime);
    void applyImpulse(double hitX, double hitY, double forceX, double forceY);
    pair<PlatformBounds, PlatformBounds> getSwingBounds() const;
};