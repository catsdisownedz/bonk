#pragma once

#include <ui/Map.h>
#include <physics/Player.h>
#include <physics/Platform.h>
#include <physics/Bouncy.h>
#include <vector>

class GangGrounds : public Map
{
public:
    GangGrounds();
    void draw() override;
    vector<Platform>& getPlatforms() override; 
    vector<Bouncy>& getBouncies() override;

private:
    vector<Platform> platforms;
    vector<Bouncy> bouncies;
    vector<Player> players;
};
