#include "../../include/physics/Platform.h"
#include <GL/glut.h>
#include <iostream>
using namespace std;

Platform::Platform(pair<double, double> position, bool horizontal, double length, double width, vector<double> colors)
    : GameObject(position.first, position.second),
      horizontal(horizontal),
      length(length),
      width(width),
      colors(colors)
{}

    //BL BR TR TL
    void Platform::draw() {
        glPushMatrix();
        glColor3f(colors[0], colors[1], colors[2]);
        glBegin(GL_QUADS);
            glVertex2f(getPosition().first, getPosition().second - width);
            glVertex2f(getPosition().first + length, getPosition().second - width);
            glVertex2f(getPosition().first + length, getPosition().second);
            glVertex2f(getPosition().first, getPosition().second);
        glEnd();
        glPopMatrix();
    }

double Platform::getWidth() {
    return width;
}

double Platform::getLength() {
    return length;
}

bool Platform::isHorizontal() const {
    return horizontal;
}

// 🔥 NEW FUNCTION
PlatformBounds Platform::getBounds() const {
    double platformLeft, platformRight, platformTop, platformBottom;
    platformLeft = getPosition().first;
    platformRight = getPosition().first + length;
    platformTop = getPosition().second;
    platformBottom = getPosition().second - width;

    return { platformLeft, platformRight, platformTop, platformBottom };
}
