// include/ui/GameScene.h
#pragma once
//#include <ui/ColorOption.h>
#include <vector>
using std::vector;
/// Abstract base for *every* screen (menu or gameplay).
class GameScene {
public:
    virtual ~GameScene() {}

    // life‐cycle
    virtual void onEnter(vector<double>& p1Color, vector<double>& p2Color) =0;            // called when scene becomes active
    virtual void onExit()  = 0;            // called before switching away

    // per‐frame
    virtual void update()  = 0;            // logic / physics / input
    virtual void render()  = 0;            // drawing

    // window events
    virtual void onReshape(int w,int h) {} // window resized
    //virtual void setPlayerColors(const ColorOption &c1, const ColorOption &c2)=0;
    // input events
    virtual void handleKeyboard     (unsigned char k,int x,int y) {}
    virtual void handleKeyboardUp   (unsigned char k,int x,int y) {}
    virtual void handleSpecialDown  (int key,int x,int y)      {}
    virtual void handleSpecialUp    (int key,int x,int y)      {}
    virtual void handleMouse        (int button,int state,int x,int y) {}
    virtual void handlePassiveMotion(int x,int y)              {}
};
