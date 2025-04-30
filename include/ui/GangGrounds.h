#pragma once

#include "Map.h"
#include "../physics/Player.h"
#include "../physics/Platform.h"
#include "../physics/Bouncy.h"
#include <vector>

class GangGrounds : public Map
{
public:
    GangGrounds();
    void draw() override;
    vector<Platform>& getPlatforms() override; 

private:
    vector<Platform> platforms;
    vector<Player> players;
};
