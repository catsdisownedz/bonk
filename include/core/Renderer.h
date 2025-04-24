#pragma once
#include "../physics/GameObject.h"
#include "../physics/Player.h"
#include <vector>

class Renderer {

    public:
        Renderer();
        // void draw();
        void display();
        void addPlayer(const Player& player);
        void addPlatform(const GameObject& platform);
        void addMovingObject(const GameObject& movingObject);
        void removePlayer(int id);
    private:
        vector<Player> players;
        vector<GameObject> platforms;
        vector<GameObject> movingObjects;
};