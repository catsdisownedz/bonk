#pragma once
#include <physics/Platform.h>
#include <physics/Bouncy.h>
#include <vector>

class Map {
public:
    virtual void draw() = 0; 
    virtual std::vector<Platform>& getPlatforms() = 0;
    virtual std::vector<Bouncy>& getBouncies()=0;
    virtual ~Map() {}; 
};
