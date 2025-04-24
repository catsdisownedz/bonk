#include <GL/glut.h>
#include "OneVsOne.h"

OneVsOne::OneVsOne()
    : platform(0.0, 0.0)
{
    // Any other initialization goes here
}

void OneVsOne::draw()
{

    glColor3f(0.0, 0.0, 1.0);

    glBegin(GL_QUADS);
    glVertex2f(-0.67f, 0.07f);  // Top-left (x=-0.3, y=0.15)
    glVertex2f(-0.67f, -0.07f); // Bottom-left (x=-0.3, y=-0.15)
    glVertex2f(0.67f, -0.07f);  // Bottom-right (x=0.3, y=-0.15)
    glVertex2f(0.67f, 0.07f);   // Top-right (x=0.3, y=0.15)  // Top-right
    glEnd();
}

void OneVsOne::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    glFlush();
}
