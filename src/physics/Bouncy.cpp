#include "../../include/physics/Bouncy.h"
#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

Bouncy::Bouncy(pair<double, double> center, double radius, vector<double> colors)
    :GameObject(center.first, center.second,colors[0], colors[1], colors[2]),
    radius(radius),
    colors(colors) 
{}


void Bouncy::draw(){
    glPushMatrix();
    glColor3f(colors[0],colors[1],colors[2]);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float rad = i * 3.14159f / 180;
        glVertex2f(getPosition().first + getRadius() * cos(rad), getPosition().second + getRadius() * sin(rad));
    }
    glEnd();
    glPopMatrix();
}

double Bouncy::getRadius(){
    return radius;
}
