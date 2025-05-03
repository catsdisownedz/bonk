// src/ui/OneVsOneScene.h
#pragma once
#include "../../include/ui/OneVsOne.h"
#include "../../include/physics/Player.h"
#include "../../include/core/Renderer.h"
#include "GameScene.h"

class OneVsOneScene : public GameScene {
    Renderer   renderer;
    OneVsOne   map;
    Player     ball1{1}, ball2{2};

public:
    void onEnter() override {
        renderer.setMap(&map);
        ball1.setPosition({0.0,  0.40}); ball1.setColor({1,0,0});
        ball2.setPosition({-0.5, 0.45}); ball2.setColor({0,0,1});
        renderer.addPlayer(ball1);
        renderer.addPlayer(ball2);
    }
    void onExit() override     { /* you could clear players if you want */ }
    void update() override     { renderer.update(); }
    void render() override     { renderer.display(); }

    void handleKeyboard(unsigned char k,int x,int y) override {
        // forward to your InputManager if you have one...
    }
};
