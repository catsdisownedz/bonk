#pragma once

#include <string>
#include <vector>
#include <chrono>
<<<<<<< Updated upstream
#include <filesystem>
#include <regex>
#include <GL/glut.h>
#include "../core/SoundPlayer.h"
=======
#include <array>
#include <GL/glut.h>
#include <core/SoundPlayer.h>
#include <core/Renderer.h>
#include <core/InputManager.h>
#include <ui/OneVsOne.h>
#include <ui/GangGrounds.h>
#include <physics/Player.h>
#include <ui/ColorOption.h>
>>>>>>> Stashed changes

struct Button {
    float x,y,width,height;
    std::string label;
    bool highlighted = false;
};


class MenuManager {
public:
    MenuManager();
    ~MenuManager() = default;

    // Called from the MainMenuScene
    void onEnter();    // load assets, reset state
    void update();     // advance GIF frame, check timers
    void render();     // draw username field, buttons, color picker, etc.

    void handleKeyboard(unsigned char key,int x,int y);
    void handleMouse(int button,int state,int x,int y);
    void handlePassiveMotion(int x,int y);

    // Once the user clicks “OneVsOne” or “GangGrounds” this returns that string
    std::string getSelectedMap() const { return selectedMap; }
    // index 0 = Player One box, 1 = Player Two box
    std::array<Button,2>      playerColorBoxes;
    std::array<ColorOption,2>       playerColors;        // [0] = P1, [1] = P2
    int                       hoveredColorBox = -1; // -1 = none, 0 or 1
    int                       editingColorBox = 0;  // which box to set when picker closes


private:
    // your previous globals become members:
    std::string username;
<<<<<<< Updated upstream
    
    bool usernameSaved = false,
         triedToProceedWithoutUsername = false,
         colorSaved = false,
         showColorPicker = false,
         recentlySaved = false,
         showCursor = true;

=======
    bool usernameSaved               = false;
    bool triedToProceedWithoutUsername = false;
    bool showColorPicker             = false;
    bool showCursor                  = true;
    bool highlightColorBox           = false;
    ColorOption playerColor1, playerColor2;
>>>>>>> Stashed changes
    std::string selectedMap;
    std::chrono::steady_clock::time_point lastSaveTime;

    int hoverCol = -1, hoverRow = -1;
    ColorOption selectedColor{0,0,0};

    std::vector<unsigned int> gifFrames;
    int currentFrame = 0;

    // constants
    static constexpr int WINDOW_WIDTH  = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr float PICKER_MARGIN = 10.0f;
    static constexpr int PICKER_CELLS  = 50;
    static constexpr float PICKER_SIZE = PICKER_CELLS * 5.0f;

    std::vector<Button> menuButtons, mapButtons;

<<<<<<< Updated upstream
    // helpers — move your free-functions into these:
    void drawText(float x,float y,const std::string& text);
    void drawButton(const Button& btn);
=======
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
>>>>>>> Stashed changes
    void drawBackground();
    void drawMainMenu();
    void drawMapSelection();
    void drawColorPicker();

    void loadGifFrames(const std::string& dir);
<<<<<<< Updated upstream
    void initButtons();
=======
    void launchGame(const std::string& mapName);
    void generateRandomPlayerColors();
>>>>>>> Stashed changes
};
