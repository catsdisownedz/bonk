#pragma once
#include <physics/GameObject.h>
#include <physics/Player.h>
#include <ui/Map.h>
#include <vector>
#include <memory>

class Renderer {

    public:
        Renderer();
        // void draw();
        void display();
        void addPlayer(const Player& player);
        void addPlatform(GameObject platform);
        void addBouncies(GameObject bouncy);
        void addMovingObject(const GameObject& movingObject);
        void removePlayer(int id);
        void setMap(Map* newMap);
        void update();

    private:
        vector<Player> players;
        vector<GameObject> platforms;
        vector<GameObject> bouncies;
        vector<GameObject> movingObjects;
        Map* map;
};