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
        ColorOption co1{ float(p1Color[0]), float(p1Color[1]), float(p1Color[2]) };
        ColorOption co2{ float(p2Color[0]), float(p2Color[1]), float(p2Color[2]) };
        p1.setColorOption(co1);
        p2.setColorOption(co2);

        G.addPlayer(p1); 
        G.addPlayer(p2);
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
    void render() override {
        // 1. set the clear color you want (e.g. dark teal)
        glClearColor(0.1f, 0.8f, 0.6f, 1.0f);
        // 2. clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // 3. now draw the scene
        renderer.display();
    }

    void onReshape(int w, int h) override {
        // 1) update viewport
        glViewport(0, 0, w, h);

        // 2) set up a new projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float a = float(w) / float(h);
        if (a >= 1.0f) {
            gluOrtho2D(-a, a, -1.0, 1.0);
        } else {
            gluOrtho2D(-1.0, 1.0, -1.0 / a, 1.0 / a);
        }

        // 3) switch back to model-view for all normal rendering
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

        
    void handleKeyboard(unsigned char k,int x,int y)    override  { inputManager.keyDown(k); }
    void handleKeyboardUp(unsigned char k,int x,int y)  override  { inputManager.keyUp(k); }
    void handleSpecialDown(int k,int x,int y)       override      { inputManager.specialKeyDown(k); }
    void handleSpecialUp(int k,int x,int y)         override      { inputManager.specialKeyUp(k); }
    void handleMouse(int,int,int,int)              override      {}
    void handlePassiveMotion(int,int)            override        {}
};
