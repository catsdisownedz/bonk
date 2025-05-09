#pragma once
#include <physics/GameObject.h>
#include <vector>
#include <utility>

using namespace std;

class Bouncy : public GameObject {
private:
   double radius;
    vector<double> colors;


public:
    Bouncy(pair<double, double> center, double radius, vector<double> colors);

    void draw();
    double getRadius();

};
