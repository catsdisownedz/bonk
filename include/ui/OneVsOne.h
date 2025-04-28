#pragma once

#include "Map.h"
#include "../physics/Player.h"
#include "../physics/Platform.h"
#include <vector>

class OneVsOne : public Map
{
public:
    OneVsOne();
    void draw() override;
    vector<Platform>& getPlatforms() override; 

private:
    vector<Platform> platforms;
    vector<Player> players;
};
