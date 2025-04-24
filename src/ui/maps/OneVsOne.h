#pragma once

#include "Map.h"
#include "physics/GameObject.h"
#include "physics/Player.h"
#include <vector>

class OneVsOne : public Map
{
public:
    OneVsOne();
    void draw() override;
    void display() override;

private:
    GameObject platform;
    vector<Player> players;
};
