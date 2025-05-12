#pragma once

#include <ui/Map.h>
#include <physics/Player.h>
#include <physics/Platform.h>
#include <physics/Bouncy.h>
#include <vector>

using namespace std;

class GravityOff : public Map
{
public:
    GravityOff();
    void draw() override;
    void update(double deltaTime) override;
    vector<Platform>& getPlatforms() override;
    vector<Bouncy>& getBouncies() override;

private:
    vector<Platform> platforms;
    vector<Bouncy>   bouncies;
};
