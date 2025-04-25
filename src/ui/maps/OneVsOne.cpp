#include <GL/glut.h>
#include "../../../include/ui/OneVsOne.h"
#include "../../../include/physics/Platform.h"


Platform platform = Platform({-0.67, 0.07}, true, 1.34, 0.14, {0.0, 0.0, 1.0});

OneVsOne::OneVsOne(){
    // Any other initialization goes here
}

void OneVsOne::draw()
{
    platform.draw();
    // glColor3f(0.0, 0.0, 1.0);

    // glBegin(GL_QUADS);
    // glVertex2f(-0.67f, 0.07f);  
    // glVertex2f(-0.67f, -0.07f); 
    // glVertex2f(0.67f, -0.07f);  
    // glVertex2f(0.67f, 0.07f);   
    // glEnd();
}

// void OneVsOne::display()
// {
//     glClear(GL_COLOR_BUFFER_BIT);
//     draw();
//     glFlush();
// }
