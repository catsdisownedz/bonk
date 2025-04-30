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
  Bouncy leftCircle= Bouncy({-0.43f,-0.6}, 0.05, {1.0,0.0,0.0});
  Bouncy rightCircle= Bouncy({0.45f,-0.6}, 0.05, {1.0,0.0,0.0});
  platforms.push_back(leftBox);
  platforms.push_back(middleBox);
  platforms.push_back(rightBox);
  bouncies.push_back(leftCircle);
  bouncies.push_back(rightCircle);
}

vector<Platform> &GangGrounds::getPlatforms()
{
    return platforms;
}

vector<Bouncy> &GangGrounds::getBouncies(){
    return bouncies;
}

void GangGrounds::draw()
{
    for(auto platform :platforms)   
        platform.draw();
    
    for(auto bouncy: bouncies)
        bouncy.draw();


}
