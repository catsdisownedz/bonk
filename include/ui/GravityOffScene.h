#pragma once

#include <GL/glut.h>
#include <ui/GameScene.h>
#include <core/Renderer.h>
#include <core/InputManager.h>
#include <ui/GravityOff.h>
#include <physics/Player.h>
#include <ui/ColorOption.h>
#include <vector>
#include <ui/Game.h>



using std::vector;

extern InputManager inputManager;


class GravityOffScene : public GameScene {
    Renderer     renderer;
    GravityOff  map;
    Player       ball1{1}, ball2{2};
public:
    GravityOffScene() = default;
    ~GravityOffScene() override = default;

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
    void onExit() override {}
    void update() override {
        renderer.update();
    }
    void render() override {
        glClearColor(0.11f, 0.15f, 0.18f, 1.0f); // Dark teal background
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.display();
    }

    void onReshape(int w,int h) override {
        // keep aspect + viewport
        float a = float(w)/float(h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (a>=1) gluOrtho2D(-a,a,-1,1);
        else      gluOrtho2D(-1,1,-1/a,1/a);
        glViewport(0,0,w,h);
    }
    void handleKeyboard(unsigned char k,int x,int y)     override { inputManager.keyDown(k); }
    void handleKeyboardUp(unsigned char k,int x,int y)  override  { inputManager.keyUp(k); }
    void handleSpecialDown(int k,int x,int y)          override   { inputManager.specialKeyDown(k); }
    void handleSpecialUp(int k,int x,int y)          override     { inputManager.specialKeyUp(k); }
    void handleMouse(int,int,int,int)           override         {}
    void handlePassiveMotion(int,int)           override        {}
};