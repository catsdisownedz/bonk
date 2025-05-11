#pragma once
#include <physics/GameObject.h>
#include <physics/Player.h>
#include <ui/Map.h>
#include <vector>
#include <memory>
#include <map>
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
        void displayGameOverScreen(int loserPlayer);

    private:
        vector<Player> players;
        vector<GameObject> platforms;
        vector<GameObject> bouncies;
        vector<GameObject> movingObjects;
        Map* _map;
         bool   _gameOver    = false;
        int    _winnerId    = -1;
        std::map<int,int> winCounts;       // player-id → total wins

        // helpers:
        void triggerGameOver(int loserId);
        void drawGameOverOverlay();
        void drawCircle(float cx, float cy, float r);
        void drawText(float x, float y, const std::string &s);

};