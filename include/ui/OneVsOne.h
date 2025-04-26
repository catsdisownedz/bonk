#pragma once

#include "Map.h"
#include "../physics/Player.h"
#include "../physics/Platform.h"
#include <vector>

class OneVsOne : public Map
{
public:
    OneVsOne();
    void draw();
    vector<Platform>& getPlatforms(); //NOW WE CAN RETURN IT BY REFERENCE 
    //virtual void display()=0;
    //virtual void addPlatform(Platform platform)=0;
    //void addPlayers(Player player)=0; 

private:
    vector<Platform> platforms;
    vector<Player> players;
};
