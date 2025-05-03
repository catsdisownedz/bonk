#include <GL/glut.h>
#include "../../../include/ui/OneVsOne.h"
#include "../../../include/physics/Platform.h"
#include <vector>
#include <stdexcept>

OneVsOne::OneVsOne() {
    platforms.push_back(
      Platform({-0.67,0.07}, true, 1.34, 0.14, {0,0,1}));
    platforms.push_back(
      Platform({ 0.5, 0.87}, false,0.05, 0.8,  {0,0,1}));
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