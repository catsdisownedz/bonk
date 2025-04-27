#pragma once
#include "../../include/physics/GameObject.h"
#include <vector>
#include <utility>

using namespace std;

struct PlatformBounds {
    double left, right, top, bottom;
};

class Platform : public GameObject {
private:
    bool horizontal;
    double length;
    double width;
    vector<double> colors;

public:
    Platform(pair<double, double> position, bool horizontal, double length, double width, vector<double> colors);

    void draw();
    double getWidth();
    double getLength();
    bool isHorizontal() const;
    PlatformBounds getBounds() const; // 🔥 New function
};
