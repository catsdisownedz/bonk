#pragma once

#include "Map.h"
#include "include/physics/GameObject.h"
#include "include/physics/Player.h"
#include <vector>


class OneVsOne : public Map {
public:
    void draw() override;
    void display() override;
private:
    GameObject platform;
    vector<Player> players;
};
