#pragma once
#include "../physics/GameObject.h"
#include "../physics/Player.h"
#include "../ui/OneVsOne.h"
#include <vector>
#include <memory>

class Renderer {

    public:
        Renderer();
        // void draw();
        void display();
        void addPlayer(const Player& player);
        void addPlatform(GameObject platform);
        void addMovingObject(const GameObject& movingObject);
        void removePlayer(int id);
        void setMap(OneVsOne& newMap);
        void update();

    private:
        vector<Player> players;
        vector<GameObject> platforms;
        vector<GameObject> movingObjects;
        OneVsOne map;
};