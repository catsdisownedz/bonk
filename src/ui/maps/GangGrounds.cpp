#include <GL/glut.h>
#include <ui/GangGrounds.h>
#include <physics/Platform.h>
#include <physics/Bouncy.h>
#include <vector>

GangGrounds::GangGrounds()
{   
    double gray=0.5f;

  Platform leftBox  = Platform({-0.9f,  0.05f}, true, 0.47f, 1.0f, {gray, gray, gray},false);
  Platform middleBox= Platform({-0.15f,  0.3f}, true, 0.3f, 0.7f, {gray, gray, gray},false);
  Platform rightBox = Platform({ 0.45f,  0.05f}, true, 0.47f, 1.0f, {gray, gray, gray},false);
//   Bouncy leftCircle= Bouncy({-0.8f,0.2}, 0.05, {1.0,0.0,0.0});//for checking purposes
  Bouncy leftCircle= Bouncy({-0.43f,-0.6}, 0.05, {gray,gray,gray}); //original
  Bouncy rightCircle= Bouncy({0.45f,-0.6f}, 0.05, {gray,gray,gray});
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
