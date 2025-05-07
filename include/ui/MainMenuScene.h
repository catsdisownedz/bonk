// include/ui/MainMenuScene.h
#pragma once
#include <ui/GameScene.h>
#include <ui/MenuManager.h>
#include <ui/Game.h>
#include <ui/ColorOption.h>
#include <vector>
using std::vector;

class MainMenuScene : public GameScene {
    MenuManager mgr;

public:
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
    }

    void handleKeyboard(unsigned char k,int x,int y) override {
        mgr.handleKeyboard(k,x,y);
    }
    void handleKeyboardUp(unsigned char k,int x,int y) override {
        mgr.handleKeyboardUp(k,x,y);
    }
    void handleMouse(int b,int s,int x,int y) override {
        mgr.handleMouse(b,s,x,y);
    }
    void handlePassiveMotion(int x,int y) override {
        mgr.handlePassiveMotion(x,y);
    }
};
