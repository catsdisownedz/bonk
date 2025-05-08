// include/ui/OneVsOneScene.h
#pragma once
#include <ui/GameScene.h>
#include <core/Renderer.h>
#include <core/InputManager.h>
#include <ui/OneVsOne.h>
#include <physics/Player.h>
#include <ui/ColorOption.h>
#include <vector>
#include <ui/Game.h>
using std::vector;

extern InputManager inputManager;

class OneVsOneScene : public GameScene {
    Renderer  renderer;
    OneVsOne  mapImpl;
    Player    p1{1}, p2{2};

public:
    void onEnter(vector<double>& p1Color, vector<double>& p2Color) override {
    auto &G = Game::instance();

        // 1) pull the names out of Game
        p1.setName( G.getPlayerName(1) );
        p2.setName( G.getPlayerName(2) );

        // 2) the rest of your setup
        renderer.setMap(&mapImpl);

        p1.setPosition({ 0,0.4 });
        p1.setColor(p1Color);
        renderer.addPlayer(p1);

        p2.setPosition({-0.5,0.45});
        p2.setColor(p2Color);
        renderer.addPlayer(p2);

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
};
