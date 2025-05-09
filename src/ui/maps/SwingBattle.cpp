#include <GL/glut.h>
#include <ui/SwingBattle.h>
#include <physics/Platform.h>
#include <vector>

SwingBattle::SwingBattle() {
    Platform horizontal = Platform({-0.67, 0.07}, true, 1.34f, 0.14f, {0.478f, 0.741f, 0.608f});
    platforms.push_back(horizontal);
}

vector<Platform>& SwingBattle::getPlatforms() {
    return platforms;
}
vector<Bouncy> &SwingBattle::getBouncies(){
    return bouncies;
}
void SwingBattle::draw() {
    for (auto& p : platforms) p.draw();
}
