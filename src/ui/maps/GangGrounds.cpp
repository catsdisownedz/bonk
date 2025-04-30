#include <GL/glut.h>
#include "../../../include/ui/GangGrounds.h"
#include "../../../include/physics/Platform.h"
#include "../../../include/physics/Bouncy.h"
#include <vector>

GangGrounds::GangGrounds()
{   
    double gray=0.5f;

  Platform leftBox  = Platform({-0.9f,  0.05f}, true, 0.47f, 1.0f, {gray, gray, gray});
  Platform middleBox= Platform({-0.15f,  0.3f}, true, 0.3f, 0.7f, {gray, gray, gray});
  Platform rightBox = Platform({ 0.45f,  0.05f}, true, 0.47f, 1.0f, {gray, gray, gray});
  platforms.push_back(leftBox);
  platforms.push_back(middleBox);
  platforms.push_back(rightBox);
}

vector<Platform> &GangGrounds::getPlatforms()
{
    return platforms;
}

void GangGrounds::draw()
{
    platforms[0].draw();
    platforms[1].draw();
    platforms[2].draw();
}
