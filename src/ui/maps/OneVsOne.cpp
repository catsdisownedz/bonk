#include <GL/glut.h>
#include "../../../include/ui/OneVsOne.h"
#include "../../../include/physics/Platform.h"
#include <vector>

Platform horizontal = Platform({-0.67, 0.07}, true, 1.34, 0.14, {0.0, 0.0, 1.0});
Platform vertical = Platform({0.5, 0.87}, false, 0.05, 0.8, {0.0, 0.0, 1.0});
//(0.5,0.87)
//(0.5,0.07)
//(0.67,0.07)
//(0.57,0.87)

OneVsOne::OneVsOne(){
    // Any other initialization goes here
    platforms.push_back(horizontal);
    platforms.push_back(vertical);
}

vector<Platform>& OneVsOne::getPlatforms(){
    return platforms;
}

void OneVsOne::draw()
{
    for(auto& platform : platforms){
        platform.draw();
    }



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
