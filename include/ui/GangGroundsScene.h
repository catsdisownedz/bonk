// include/ui/scenes/GangGroundsScene.h
#pragma once
#include <ui/GameScene.h>
#include <ui/GangGrounds.h>
#include <core/Renderer.h>
#include <physics/Player.h>
#include <core/InputManager.h> 
#include <ui/Game.h> 
//#include <ui/ColorOption.h>
#include <vector>
using std::vector;

extern InputManager inputManager;

class GangGroundsScene : public GameScene {
    Renderer     renderer;
    GangGrounds  map;
    Player       ball1{1}, ball2{2};

public:
    GangGroundsScene() = default;
    ~GangGroundsScene() override = default;

     void onEnter(vector<double>& p1Color, vector<double>& p2Color) override {
        // 1) fetch names from Game
        auto &G = Game::instance();
        ball1.setName( G.getPlayerName(1) );
        ball2.setName( G.getPlayerName(2) );

        // 2) set up map & colors
        renderer.setMap(&map);
        ball1.setPosition({0.0, 0.40});
        ball1.setColor(p1Color);
        ball2.setPosition({-0.5, 0.45});
        ball2.setColor(p2Color);

        // 3) add them to the renderer
        renderer.addPlayer(ball1);
        renderer.addPlayer(ball2);
    }
    void onExit() override {
        /* nothing special */
    }

    void update() override {
        renderer.update();
    }
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
    void handleSpecialDown(int k,int x,int y)         override    { inputManager.specialKeyDown(k); }
    void handleSpecialUp(int k,int x,int y)          override     { inputManager.specialKeyUp(k); }
    void handleMouse(int,int,int,int)            override       {}
    void handlePassiveMotion(int,int)            override       {}
        
};

