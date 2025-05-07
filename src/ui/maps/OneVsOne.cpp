#include <GL/glut.h>
#include <ui/OneVsOne.h>
#include <physics/Platform.h>
#include <vector>
#include <stdexcept>

OneVsOne::OneVsOne() {
    Platform horizontal = Platform({-0.67, 0.07}, true, 1.34f, 0.14f, {0,0,1});
    Platform vertical = Platform({0.5, 0.87}, false, 0.05, 0.8, {0,0,1});
    platforms.push_back(horizontal);
    platforms.push_back(vertical);

    //platforms.push_back(
      //Platform({-0.67,0.07}, true, 1.34, 0.14, {0,0,1}));
    //platforms.push_back(
      //Platform({ 0.5, 0.87}, false,0.05, 0.8,  {0,0,1}));
}

std::vector<Platform>& OneVsOne::getPlatforms() {
    return platforms;
}

std::vector<Bouncy>& OneVsOne::getBouncies() {
    static std::vector<Bouncy> empty;
    return empty;
}

void OneVsOne::draw() {
    for (auto& p : platforms) p.draw();
}