#include <GL/glut.h>
#include "../../../include/ui/OneVsOne.h"
#include "../../../include/physics/Platform.h"
#include <vector>

//(0.5,0.87)
//(0.5,0.07)
//(0.67,0.07)
//(0.57,0.87)

OneVsOne::OneVsOne()
{
    // Any other initialization goes here
    // Platform horizontal = Platform({-0.67, 0.07}, true, 1.34, 0.14, {0.0, 0.0, 1.0}); //original
    Platform horizontal = Platform({-0.67, 0.07}, true, 1.34, 0.14, {0.0, 0.0, 1.0});
    Platform vertical = Platform({0.5, 0.87}, false, 0.05, 0.8, {0.0, 0.0, 1.0}); //original
    platforms.push_back(horizontal);
    platforms.push_back(vertical);
}

vector<Platform> &OneVsOne::getPlatforms()
{
    return platforms;
}

void OneVsOne::draw()
{
    platforms[0].draw();
    platforms[1].draw();
}
