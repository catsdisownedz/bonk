#include <GL/glut.h>
#include <ui/GravityOff.h>
#include <physics/Platform.h>
#include <vector>

GravityOff::GravityOff() {
    Platform horizontal({-0.9, -0.3}, true, 1.8f, 1.0f, {0.478f, 0.741f, 0.608f},true);
    platforms.push_back(horizontal);
}

vector<Platform>& GravityOff::getPlatforms() {
    return platforms;
}
vector<Bouncy> &GravityOff::getBouncies(){
    return bouncies;
}
void GravityOff::draw() {
    for (auto& p : platforms) p.draw();
}

void GravityOff::update(double deltaTime) {
    for (auto& p : platforms) p.update(deltaTime);
}


