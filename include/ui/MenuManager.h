#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <array>
#include <GL/glut.h>
#include <core/SoundPlayer.h>
#include <core/Renderer.h>
#include <core/InputManager.h>
#include <ui/OneVsOne.h>
#include <ui/GangGrounds.h>
#include <physics/Player.h>
#include <ui/ColorOption.h>

struct Button {
    float x,y,width,height;
    std::string label;
    bool highlighted = false;
};


enum class Scene { MAIN_MENU, MAP_SELECTION };

class MenuManager {
public:
    MenuManager();
    ~MenuManager() = default;

    // menu‐lifecycle
    void init();
    void onEnter();
    void update();
    void render();

    // input in menu
    void handleKeyboard(unsigned char key,int x,int y);
    void handleKeyboardUp(unsigned char key,int x,int y);
    void handleMouse(int button,int state,int x,int y);
    void handlePassiveMotion(int x,int y);

    // “game”‐mode entry points
    void onReshape(int w,int h);
    void gameDisplay();
    void gameKeyDown(unsigned char key,int x,int y);
    void gameKeyUp(unsigned char key,int x,int y);
    void gameSpecialDown(int key,int x,int y);
    void gameSpecialUp(int key,int x,int y);
    void gameTimer();
    void gameReshape(int w, int h);

    // utility
    void toggleCursor();

    // after map‐click
    std::string getSelectedMap() const { return selectedMap; }
    // index 0 = Player One box, 1 = Player Two box
    std::array<Button,2>      playerColorBoxes;
    std::array<ColorOption,2>       playerColors;        // [0] = P1, [1] = P2
    int                       hoveredColorBox = -1; // -1 = none, 0 or 1
    int                       editingColorBox = 0;  // which box to set when picker closes


private:
    // state
    bool recentlySaved = false; 
    string username2;
    bool username2Saved;
    bool triedToProceedWithoutUsername2;
    bool showSecondUsername = false;
    int  activeUsername  = 1; 

    string username;
    bool usernameSaved               = false;
    bool triedToProceedWithoutUsername = false;
    bool showColorPicker             = false;
    bool showCursor                  = true;
    bool highlightColorBox           = false;
    ColorOption playerColor1, playerColor2;
    std::string selectedMap;
    std::chrono::steady_clock::time_point lastSaveTime;

    // animated background
    std::vector<unsigned int> gifFrames;
    int currentFrame = 0;

    // constants
    static constexpr int WINDOW_WIDTH   = 800;
    static constexpr int WINDOW_HEIGHT  = 600;
    static constexpr float PICKER_MARGIN= 10.0f;
    static constexpr int PICKER_CELLS    = 50;
    static constexpr float PICKER_SIZE   = PICKER_CELLS * 5.0f;

    // buttons
    std::vector<Button> menuButtons;
    std::vector<Button> mapButtons;
    Scene currentScene = Scene::MAIN_MENU;

    // in‐game objects
    SoundPlayer  soundPlayer;
    InputManager inputManager;
    Renderer     renderer;
    OneVsOne     oneVsOneMap;
    GangGrounds  gangGroundsMap;
    Player       ball1{1}, ball2{2};

    int hoverCol = -1, hoverRow = -1;

    // drawing & setup helpers
    void drawText(float x,float y,const std::string& s);
    void drawButton(const Button& b);
    void drawStrokedText(float x,float y,const std::string& txt, void* font = GLUT_BITMAP_HELVETICA_18);
    void drawBackground();
    void drawMainMenu();
    void drawMapSelection();
    void drawColorPicker();
    void initButtons();
    void loadGifFrames(const std::string& dir);
    void launchGame(const std::string& mapName);
    void generateRandomPlayerColors();
};
