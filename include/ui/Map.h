#pragma once
#include <physics/Platform.h>
#include <physics/Bouncy.h>
#include <vector>

using namespace std;

class Map {
public:
    virtual void draw() = 0; 
    virtual vector<Platform>& getPlatforms() = 0;
    virtual vector<Bouncy>& getBouncies() = 0;
    virtual ~Map() {}
};