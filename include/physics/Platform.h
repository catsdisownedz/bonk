#pragma once
#include <physics/GameObject.h>
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


    bool isMoving = false;
    int direction = 1;
    double initialY = 0.0;

public:
    
    Platform(pair<double, double> position, bool horizontal, double length, double width, vector<double> colors, bool isMoving = false);

    void draw();
    void update(double deltaTime); // 🔥 Add this for movement updates

    double getWidth();
    double getLength();
    bool isHorizontal() const;

    PlatformBounds getBounds() const;
};
