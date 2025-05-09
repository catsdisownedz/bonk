#include <physics/Platform.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;


const double MOVE_SPEED = 0.4; 
const double MOVE_RANGE = 0.3; 

Platform::Platform(pair<double, double> position, bool horizontal, double length, double width, vector<double> colors, bool isMoving)
    : GameObject(position.first, position.second, colors[0], colors[1], colors[2]),
      horizontal(horizontal),
      length(length),
      width(width),
      colors(colors),
      isMoving(isMoving),
      direction(1),
      initialY(position.second)
{}

// Draw the platform (BL BR TR TL)
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


void Platform::update(double deltaTime) {
    if (!isMoving) return;

    auto pos = getPosition();
    pos.second += direction * MOVE_SPEED * deltaTime;

    if (pos.second > initialY + MOVE_RANGE || pos.second < initialY - MOVE_RANGE) {
        direction *= -1;
        pos.second = max(min(pos.second, initialY + MOVE_RANGE), initialY - MOVE_RANGE);
    }

    setPosition(pos);
}

double Platform::getWidth() { return width; }
double Platform::getLength() { return length; }
bool Platform::isHorizontal() const { return horizontal; }

PlatformBounds Platform::getBounds() const {
    double left = getPosition().first;
    double right = left + length;
    double top = getPosition().second;
    double bottom = top - width;
    return { left, right, top, bottom };
}

