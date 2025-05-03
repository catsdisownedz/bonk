// src/ui/GangGroundsScene.h
#pragma once
#include "../../include/ui/GangGrounds.h"
#include "../../include/physics/Player.h"
#include "../../include/core/Renderer.h"
#include "GameScene.h"

class GangGroundsScene : public GameScene {
    Renderer     renderer;
    GangGrounds  map;
    Player       ball1{1}, ball2{2};

public:
    void onEnter() override {
        renderer.setMap(&map);
        // set positions/colors similar to above…
        renderer.addPlayer(ball1);
        renderer.addPlayer(ball2);
    }
    void onExit() override  {}
    void update() override  { renderer.update(); }
    void render() override  { renderer.display(); }
};
