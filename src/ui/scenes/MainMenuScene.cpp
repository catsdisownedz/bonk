// src/ui/MainMenuScene.cpp

#include "MainMenuScene.h"
#include "../../include/ui/Game.h"    // for Game::instance()

MainMenuScene::MainMenuScene() = default;

void MainMenuScene::onEnter() {
    menu.onEnter();    // e.g. reload frames, reset state
}

void MainMenuScene::onExit() {
    // nothing to clean up for now
}

void MainMenuScene::update() {
    menu.update();     // advance GIF, cursor blink, etc.

    // if a map was picked, change scenes
    auto choice = menu.getSelectedMap();
    if (!choice.empty()) {
        Game::instance().changeScene(choice);
    }
}

void MainMenuScene::render() {
    menu.render();
}

void MainMenuScene::handleKeyboard(unsigned char key, int x, int y) {
    menu.handleKeyboard(key, x, y);
}

void MainMenuScene::handleMouse(int button, int state, int x, int y) {
    menu.handleMouse(button, state, x, y);
}

void MainMenuScene::handlePassiveMotion(int x, int y) {
    menu.handlePassiveMotion(x, y);
}
