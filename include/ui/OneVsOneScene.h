// include/ui/scenes/OneVsOneScene.h
#pragma once
<<<<<<< Updated upstream
=======
#include <ui/GameScene.h>
#include <core/Renderer.h>
#include <core/InputManager.h>
#include <ui/OneVsOne.h>
#include <physics/Player.h>
#include <ui/ColorOption.h>
#include <vector>
using std::vector;
>>>>>>> Stashed changes

#include "../core/Renderer.h"
#include "../physics/Player.h"
#include "OneVsOne.h"
#include "GameScene.h"

class OneVsOneScene : public GameScene {
    Renderer renderer;
    OneVsOne map;
    Player ball1{1}, ball2{2};

public:
<<<<<<< Updated upstream
    void onEnter() override {
        renderer.setMap(&map);
        ball1.setPosition({0.0, 0.40});  ball1.setColor({1,0,0});
        ball2.setPosition({-0.5, 0.45}); ball2.setColor({0,0,1});
        renderer.addPlayer(ball1);
        renderer.addPlayer(ball2);
    }
    void onExit() override { /* optional cleanup */ }

    void update() override  { renderer.update(); }
    void render() override  { renderer.display(); }

    // if you need input:
    // void handleKeyboard(...) override { ... }
=======
    void onEnter(vector<double>& p1Color, vector<double>& p2Color) override {
        // projection is already set by Game::changeScene → onReshape
        // configure renderer & players
        renderer.setMap(&mapImpl);
        p1.setPosition({ 0,0.4 }); p1.setColor(p1Color); renderer.addPlayer(p1);
        p2.setPosition({-0.5,0.45}); p2.setColor(p2Color); renderer.addPlayer(p2);
    }
    void onExit() override {}
    void update() override { renderer.update(); }
    void render() override { renderer.display(); }
    void onReshape(int w,int h) override {
        // keep aspect + viewport
        float a = float(w)/float(h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (a>=1) gluOrtho2D(-a,a,-1,1);
        else      gluOrtho2D(-1,1,-1/a,1/a);
        glViewport(0,0,w,h);
    }
        
    void handleKeyboard(unsigned char k,int x,int y)      { inputManager.keyDown(k); }
    void handleKeyboardUp(unsigned char k,int x,int y)    { inputManager.keyUp(k); }
    void handleSpecialDown(int k,int x,int y)             { inputManager.specialKeyDown(k); }
    void handleSpecialUp(int k,int x,int y)               { inputManager.specialKeyUp(k); }
    void handleMouse(int,int,int,int)                    {}
    void handlePassiveMotion(int,int)                    {}
>>>>>>> Stashed changes
};
