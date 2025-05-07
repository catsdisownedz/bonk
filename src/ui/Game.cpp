// src/ui/Game.cpp
#include <ui/Game.h>
#include <GL/glut.h>
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

    // start in main menu
    changeScene("Menu");
}

void Game::changeScene(const std::string& name) {
    if (currentScene) currentScene->onExit();
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

void Game::render() {
    if (currentScene) currentScene->render();
}

void Game::onKeyDown(unsigned char k,int x,int y) {
    if (currentScene) currentScene->handleKeyboard(k,x,y);
}
void Game::onKeyUp(unsigned char k,int x,int y) {
    if (currentScene) currentScene->handleKeyboardUp(k,x,y);
}
void Game::onSpecialDown(int k,int x,int y) {
    if (currentScene) currentScene->handleSpecialDown(k,x,y);
}
void Game::onSpecialUp(int k,int x,int y) {
    if (currentScene) currentScene->handleSpecialUp(k,x,y);
}
void Game::onMouse(int b,int s,int x,int y) {
    if (currentScene) currentScene->handleMouse(b,s,x,y);
}
void Game::onPassiveMotion(int x,int y) {
    if (currentScene) currentScene->handlePassiveMotion(x,y);
}
void Game::onReshape(int w,int h) {
    windowW = w;
    windowH = h;
    if (currentScene) currentScene->onReshape(w,h);
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
    glutInitWindowSize(800,600);
    glutCreateWindow("Bonk");

    // hook callbacks *once*
    glutDisplayFunc(displayCB);
    glutTimerFunc(16, timerCB,   0);
    glutKeyboardFunc(keyDownCB);
    glutKeyboardUpFunc(keyUpCB);
    glutSpecialFunc(specialDownCB);
    glutSpecialUpFunc(specialUpCB);
    glutMouseFunc(mouseCB);
    glutPassiveMotionFunc(passiveCB);
    glutReshapeFunc(reshapeCB);

    // now initialize scenes and enter main loop
    Game::instance().init();
    glutMainLoop();
    return 0;
}
