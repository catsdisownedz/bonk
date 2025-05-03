// include/ui/scenes/MainMenuScene.h
#pragma once
<<<<<<< Updated upstream
#include "MenuManager.h"
#include "GameScene.h"
#include "Game.h"
=======
#include <ui/GameScene.h>
#include <ui/MenuManager.h>
#include <ui/Game.h>
#include <ui/ColorOption.h>
#include <vector>
using std::vector;
>>>>>>> Stashed changes

class MainMenuScene : public GameScene {
    MenuManager menu;

public:
<<<<<<< Updated upstream
    void onEnter()    override { menu.onEnter(); }
    void onExit()     override { /* nothing special */ }
    void update()     override { 
        menu.update();
        auto choice = menu.getSelectedMap();
        if (!choice.empty())
            Game::instance().changeScene(choice);
=======
    void onEnter(vector<double>& p1Color, vector<double>& p2Color) override {
        mgr.init();
    }
    void onExit() override { /* nothing special */ }

    void update() override {
        mgr.update();
        auto map = mgr.getSelectedMap();
        if (!map.empty()) {
            Game::instance().changeScene(map);
        }
    }
    void render() override {
        mgr.render();
    }

    void onReshape(int w,int h) override {
        mgr.onReshape(w,h);
>>>>>>> Stashed changes
    }
    void render()     override { menu.render(); }

    void handleKeyboard(unsigned char k,int x,int y) override {
        menu.handleKeyboard(k,x,y);
    }
    void handleMouse(int b,int s,int x,int y) override {
        menu.handleMouse(b,s,x,y);
    }
    void handlePassiveMotion(int x,int y) override {
        menu.handlePassiveMotion(x,y);
    }
};
