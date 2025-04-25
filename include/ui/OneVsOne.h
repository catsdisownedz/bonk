#pragma once

#include "Map.h"
#include "../physics/Player.h"
#include "../physics/platform.h"
#include <vector>

class OneVsOne : public Map
{
public:
    OneVsOne();
    void draw();
    //virtual void display()=0;
    //virtual void addPlatform(Platform platform)=0;
    //void addPlayers(Player player)=0; 

private:
    vector<GameObject> platforms;
    vector<Player> players;
};
