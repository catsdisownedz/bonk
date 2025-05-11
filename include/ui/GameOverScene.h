#pragma once
#include "GameScene.h"
#include "ui/GameOver.h"
#include "ui/Game.h"
#include <GL/glut.h>
#include <vector>
#include<iostream>



class GameOverScene : public GameScene {
    GameOver gameOver;
    int winningPlayer = 1;
    float timer = 0.0f;
  bool transitioning=false; // Prevent multiple transitions
    
    
public:
    GameOverScene() = default;
    ~GameOverScene() override = default;

    void onEnter(std::vector<double>& p1Color, std::vector<double>& p2Color) override {
        // winningPlayer = Game::instance().getWinner(); // Get winner from Game
        timer = 0.0f;
        transitioning = false; 
        winningPlayer = Game::instance().getLastWinner();
    }
void update() override {

    if (transitioning) {
        return; // Skip updates if already transitioning
    }

    auto &G = Game::instance();
    timer += 0.016f; // ~60fps delta time
    string nextScene = G.getPrevoiusSceneName();
    cout << "Timer: " << timer << ", Next Scene: " << nextScene << endl;

    if (timer >= 3.0f) {
        transitioning = true; // Mark transition as in progress
        G.changeScene(nextScene); // Change to the previous scene
        return;
    }
}

    void render() override {
         if (transitioning) {
        return; // Skip renderi ng if transitioning
    }
        gameOver.draw(winningPlayer); // Draw with winner info
    }

      void onExit() override {
        /* nothing special */
    }

    // void update() override {
    //     return;
        
    // }
    // void render() override {
    //     // 1. set the clear color you want (e.g. dark teal)
    //     glClearColor(0.1f, 0.8f, 0.6f, 1.0f);
    //     // 2. clear the color buffer
    //     glClear(GL_COLOR_BUFFER_BIT);

    //     // 3. now draw the scene
    //     renderer.display();
    // }


    void onReshape(int w, int h) override {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float aspect = float(w)/float(h);
        if (aspect >= 1.0f) {
            gluOrtho2D(-aspect, aspect, -1.0, 1.0);
        } else {
            gluOrtho2D(-1.0, 1.0, -1.0/aspect, 1.0/aspect);
        }
        glMatrixMode(GL_MODELVIEW);
    }

    // Empty input handlers
    void handleKeyboard(unsigned char, int, int) override {}
    void handleMouse(int, int, int, int) override {}
    void handleKeyboardUp(unsigned char, int, int) override {}
    void handleSpecialDown(int, int, int) override {}
    void handleSpecialUp(int, int, int) override {}
    void handlePassiveMotion(int, int) override {}
};