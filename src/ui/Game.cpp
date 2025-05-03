// src/ui/Game.cpp
#include <GL/glut.h>
<<<<<<< Updated upstream
#include "../../../include/ui/Game.h"
#include "../../../include/ui/MainMenuScene.h"
#include "../../../include/ui/OneVsOneScene.h"
#include "../../../include/ui/GangGroundsScene.h"

using namespace std;

// GLUT callbacks that forward into Game:
static void displayCB()   { Game::instance().render(); }
static void timerCB(int)  { Game::instance().update(); glutPostRedisplay(); glutTimerFunc(16,timerCB,0); }
static void keyDownCB(unsigned char k,int x,int y) { Game::instance().onKeyDown(k,x,y); }
static void keyUpCB  (unsigned char k,int x,int y) { Game::instance().onKeyUp(k,x,y); }
static void mouseCB  (int b,int s,int x,int y)      { Game::instance().onMouse(b,s,x,y); }
static void passiveCB(int x,int y)                  { Game::instance().onPassive(x,y); }
=======
#include <ui/MainMenuScene.h>
#include <ui/OneVsOneScene.h>
#include <ui/GangGroundsScene.h>
#include <iostream>
#include <ui/ColorOption.h>

// — GLUT callbacks, installed exactly once below —
static void displayCB()            { Game::instance().render(); }

static void timerCB(int) {
    auto& G = Game::instance();
    // advance simulation & queue a redraw
    G.update();
    glutPostRedisplay();
    // choose next delay based on which scene is live
    int delayMs = (G.getCurrentSceneName() == "Menu")
                    ? 50   // run menu at 5 FPS
                    : 16;   // ~60 FPS everywhere else

    glutTimerFunc(delayMs, timerCB, 0);
}

static void keyDownCB       (unsigned char k,int x,int y) { Game::instance().onKeyDown(k,x,y); }
static void keyUpCB         (unsigned char k,int x,int y) { Game::instance().onKeyUp(k,x,y); }
static void specialDownCB   (int k,int x,int y)           { Game::instance().onSpecialDown(k,x,y); }
static void specialUpCB     (int k,int x,int y)           { Game::instance().onSpecialUp(k,x,y); }
static void mouseCB         (int b,int s,int x,int y)     { Game::instance().onMouse(b,s,x,y); }
static void passiveCB       (int x,int y)                 { Game::instance().onPassiveMotion(x,y); }
static void reshapeCB       (int w,int h)                 { Game::instance().onReshape(w,h); }
>>>>>>> Stashed changes

Game& Game::instance() {
    static Game g;
    return g;
}

Game::Game() {}

void Game::init() {
    // register all scenes
    scenes["Menu"]        = new MainMenuScene();
    scenes["OneVsOne"]    = new OneVsOneScene();
    scenes["GangGrounds"] = new GangGroundsScene();

    // start in menu
    changeScene("Menu");
}

void Game::changeScene(const std::string& name) {
    if (currentScene) currentScene->onExit();
<<<<<<< Updated upstream
    currentScene = scenes[name];
    if (currentScene) currentScene->onEnter();
}

void Game::update()  { if (currentScene) currentScene->update(); }
void Game::render()  { if (currentScene) currentScene->render(); }
=======
    currentScene     = scenes[name];
    currentSceneName = name;   
    if (currentScene) {
        // reapply last window size / projection
        currentScene->onReshape(windowW, windowH);
        currentScene->onEnter(playerColor1, playerColor2);
    }
    // if (name != "Menu") {
    //    currentScene->setPlayerColors(playerColor1, playerColor2);
    //   }
}

void Game::update() {
    if (currentScene) {
        // if(currentScene == "Menu"){

        // }
        currentScene->update();
    }
}
>>>>>>> Stashed changes

void Game::onKeyDown(unsigned char k,int x,int y) { 
    if (currentScene) currentScene->handleKeyboard(k,x,y); 
}
void Game::onKeyUp(unsigned char k,int x,int y) { 
    if (currentScene) currentScene->handleKeyboardUp(k,x,y); 
}
void Game::onMouse(int b,int s,int x,int y) { 
    if (currentScene) currentScene->handleMouse(b,s,x,y); 
}
void Game::onPassive(int x,int y) { 
    if (currentScene) currentScene->handlePassiveMotion(x,y); 
}

// void Game::setPlayerColors(const ColorOption &c1,
//                            const ColorOption &c2) {
//      playerColor1 = c1;
//      playerColor2 = c2;
//    // 2) if we’re already in a non-Menu scene, forward immediately
//     if (currentSceneName != "Menu" && currentScene) {
//      currentScene->setPlayerColors(c1, c2);
//         }
//     }

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bonk");

    // Initialize all scenes
    Game::instance().init();

    // Hook up GLUT
    glutDisplayFunc(displayCB);
    glutTimerFunc(16,   timerCB,   0);
    glutKeyboardFunc(keyDownCB);
    glutKeyboardUpFunc(keyUpCB);
    glutMouseFunc(mouseCB);
    glutPassiveMotionFunc(passiveCB);

    glutMainLoop();
    return 0;
}