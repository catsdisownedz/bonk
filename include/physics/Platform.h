#pragma once

#include "GameObject.h"
#include <vector>

using namespace std;

class Platform: public GameObject{
    public:
        Platform(pair<double, double> position, bool horizontal,double length,double width, vector<double> colors);
        void draw();
    private:
        bool horizontal;
        double length;
        double width;
        vector<double> colors;
};