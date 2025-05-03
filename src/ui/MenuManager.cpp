// src/ui/MenuManager.cpp
<<<<<<< Updated upstream

#include "../../include/core/SoundPlayer.h"
#include "../../include/core/Renderer.h"
#include "../../include/physics/Player.h"
#include "../../include/core/InputManager.h"
#include "../../include/ui/OneVsOne.h"
#include "../../include/ui/GangGrounds.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cmath>
#include <algorithm>
#include <map>
#include <chrono>
#include <regex>
#include <ctime>
#include "../../external/stb_image.h"

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

// ————————————————————————————————————————————————
// Global menu state
// ————————————————————————————————————————————————
const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 600;
const float PICKER_MARGIN = 10.0f;
const int   PICKER_CELLS  = 50;
const float PICKER_SIZE   = PICKER_CELLS * 5.0f;

enum class Scene { MAIN_MENU, MAP_SELECTION };
static Scene currentScene = Scene::MAIN_MENU;

static SoundPlayer soundPlayer;
static vector<unsigned int> gifFrames;
static int currentFrame = 0;

// Input State
static string username;
static bool   usernameSaved          = false;
static bool   triedToProceedWithoutUsername = false;
static bool   showColorPicker        = false;
static bool   recentlySaved          = false;
static bool   showCursor             = true;
static steady_clock::time_point lastSaveTime;

// Color‐picker
static int hoverCol = -1, hoverRow = -1;
struct ColorOption { float r,g,b; };
static ColorOption selectedColor;

// Menu & Map buttons
struct Button { float x,y,w,h; string label; bool highlighted = false; };
static vector<Button> menuButtons = {
    {0,   350, 260, 50, "Multiplayer"},
    {0,   275, 260, 50, "Local Player"},
    {0,   200, 260, 50, "Options"},
    {0,   125, 260, 50, "Quit"}
};
static vector<Button> mapButtons = {
    {100, 400, 150,150, "OneVsOne"},
    {300, 400, 150,150, "GangGrounds"},
    {500, 400, 150,150, "Swing"},
    {300, 200, 150,150, "Randomized"}
};

// ————————————————————————————————————————————————
// Game objects for map scenes
// ————————————————————————————————————————————————
static InputManager inputManager;
static Renderer     renderer;
static OneVsOne     oneVsOneMap;
static GangGrounds  gangGroundsMap;
static Player       ball1{1}, ball2{2};

// Forward‐declare map callbacks
static void gameDisplay();
static void gameReshape(int w,int h);
static void gameKeyDown(unsigned char k,int x,int y);
static void gameKeyUp(unsigned char k,int x,int y);
static void gameSpecialDown(int key,int x,int y);
static void gameSpecialUp(int key,int x,int y);
static void updateGame(int);

// Helper: switch from Menu → Game
static void launchGame(const string& mapName) {
    // 1) Configure projection & reshape
    glClearColor(0.1f,0.8f,0.6f,1.0f);
=======
#include "ui/MenuManager.h"
#include <stb_image.h>
#include <filesystem>
#include <regex>
#include <iostream>
#include <random>
#include <ui/Game.h>
#include <vector>
namespace fs = std::filesystem;

MenuManager::MenuManager()
  : menuButtons{
      {0,350,260,50,"Multiplayer"},
      {0,275,260,50,"Local Player"},
      {0,200,260,50,"Options"},
      {0,125,260,50,"Quit"}
    },
    mapButtons{
      {100,400,150,150,"OneVsOne"},
      {300,400,150,150,"GangGrounds"},
      {500,400,150,150,"Swing"},
      {300,200,150,150,"Randomized"}
    }
{
    generateRandomPlayerColors();
    float boxW = 30, boxH = 30, startX = 300, startY = 435, gap = 50;
    playerColorBoxes[0] = { startX,        startY, boxW, boxH, "" };
    playerColorBoxes[1] = { startX+boxW+gap, startY, boxW, boxH, "" };
}

void MenuManager::generateRandomPlayerColors() {
    std::random_device rd;
    std::mt19937        gen(rd());
    std::uniform_real_distribution<float> dist(0.0f,1.0f);
    playerColors[0] = { dist(gen), dist(gen), dist(gen) };
    playerColors[1] = { dist(gen), dist(gen), dist(gen) };
}

void MenuManager::init() {
    glClearColor(0,0,0,1);
>>>>>>> Stashed changes
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,1,-1,1);
    glutReshapeFunc(gameReshape);

<<<<<<< Updated upstream
    // 2) Setup renderer + players
    renderer = Renderer();
    if (mapName == "OneVsOne") {
        renderer.setMap(&oneVsOneMap);
=======
    initButtons();
    soundPlayer.loadSound("click","assets/audio/click.wav");
    loadGifFrames("assets/gif");
    currentFrame = 0;
    lastSaveTime = std::chrono::steady_clock::now();
}

void MenuManager::loadGifFrames(const std::string& dir) {
    // delete any old textures
    for (auto t : gifFrames) glDeleteTextures(1,&t);
    gifFrames.clear();

    std::vector<std::pair<int,fs::path>> files;
    std::regex pat(R"(frame_(\d+).+\.png)");
    for (auto &e : fs::directory_iterator(dir)) {
        std::smatch m;
        auto fn = e.path().filename().string();
        if (std::regex_match(fn, m, pat)) {
            files.emplace_back(std::stoi(m[1].str()), e.path());
        }
    }
    std::sort(files.begin(), files.end(),
              [](auto &a, auto &b){ return a.first < b.first; });

    // now actually load them
    stbi_set_flip_vertically_on_load(true);
    for (auto &pr : files) {
        int w,h,ch;
        auto data = stbi_load(pr.second.string().c_str(), &w,&h,&ch, STBI_rgb_alpha);
        if (!data) continue;
        GLuint tex;
        glGenTextures(1,&tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, w,h, 0, GL_RGBA,GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        gifFrames.push_back(tex);
    }
}

void MenuManager::initButtons() {
    // center those 260px-wide buttons horizontally
    float mx = (WINDOW_WIDTH - 260.f) * 0.5f;
    for (int i = 0; i < (int)menuButtons.size(); ++i) {
        menuButtons[i].x = mx;
        menuButtons[i].y = 350.f - i*75.f;
    }
    // (mapButtons are already positioned in your ctor)
}

void MenuManager::onEnter() {
    username.clear();
    usernameSaved = false;
    triedToProceedWithoutUsername = false;
    showColorPicker = false;
    showCursor = true;
    highlightColorBox = false;
    hoverRow = hoverCol = -1;
    generateRandomPlayerColors();
    init();
}

void MenuManager::update() {
    if (!gifFrames.empty())
        currentFrame = (currentFrame + 1) % gifFrames.size();
}

void MenuManager::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();

    if (currentScene == Scene::MAIN_MENU)
        drawMainMenu();
    else
        drawMapSelection();

    drawColorPicker();

    // “Saved” indicator for 1s after typing
    auto now = std::chrono::steady_clock::now();
    if (usernameSaved &&
        std::chrono::duration_cast<std::chrono::seconds>(now - lastSaveTime).count() < 1)
    {
        drawStrokedText(WINDOW_WIDTH*0.5f - 30, 20, "Saved");
    }

    glutSwapBuffers();
}

void MenuManager::handleKeyboard(unsigned char key,int x,int y) {
    if (key == 8 && !username.empty()) {
        username.pop_back();
    } else if (username.size() < 12 && isalnum(key)) {
        username.push_back(key);
    }
    // mark saved and reset timer
    usernameSaved   = true;
    triedToProceedWithoutUsername = !usernameSaved;
    lastSaveTime    = std::chrono::steady_clock::now();
}


void MenuManager::handleKeyboardUp(unsigned char /*key*/,int/*x*/,int/*y*/) {
    // nothing for now
}

void MenuManager::handleMouse(int button,int state,int x,int y) {
    if (button!=GLUT_LEFT_BUTTON || state!=GLUT_DOWN) return;
    y = WINDOW_HEIGHT - y;

    // color‐picker interaction
    // in MenuManager::handleMouse(...)
    if (showColorPicker) {
        // compute which cell was clicked
        float cell = PICKER_SIZE / float(PICKER_CELLS);
        float sx   = WINDOW_WIDTH - PICKER_MARGIN - PICKER_SIZE;
        float sy   = WINDOW_HEIGHT - PICKER_MARGIN - PICKER_SIZE;
        float dx   = x - sx, dy = y - sy;

        if (dx >= 0 && dy >= 0 && dx < PICKER_SIZE && dy < PICKER_SIZE) {
            int col = int(dx / cell), row = int(dy / cell);
            float h = col / float(PICKER_CELLS - 1);
            float v = 1.0f - row / float(PICKER_CELLS - 1);
            int i = int(h * 6);
            float f = (h*6) - i;
            float p = 0.0f, q = v * (1 - f), t = v * (1 - (1 - f));

            // compute RGB
            float r, g, b;
            switch (i % 6) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            default:r = v; g = p; b = q; break;
            }

            // **assign into the correct player slot**
            playerColors[editingColorBox] = { r, g, b };

            // close the picker
            showColorPicker = false;
        }
        return;
    }

    // if we clicked one of the two color boxes, open picker for that box
  if (currentScene == Scene::MAIN_MENU) {
      for (int i = 0; i < 2; ++i) {
        auto &b = playerColorBoxes[i];
        if (x >= b.x && x <= b.x + b.width
         && y >= b.y && y <= b.y + b.height)
        {
          editingColorBox = i;
          soundPlayer.playSound("click");
          showColorPicker = true;
          return;
        }
      }
    }

    // toggle picker
    if (currentScene==Scene::MAIN_MENU
     && x>=300 && x<=330 && y>=435 && y<=465)
    {
        soundPlayer.playSound("click");
        showColorPicker = !showColorPicker;
        return;
    }

    if (currentScene==Scene::MAIN_MENU) {
        // main-menu buttons
        for (auto &b : menuButtons) {
            if (x>=b.x && x<=b.x+b.width
             && y>=b.y && y<=b.y+b.height)
            {
                if (!usernameSaved) {
                    triedToProceedWithoutUsername = true;
                    return;
                }
                if (b.label=="Quit") exit(0);
                if (b.label=="Local Player") {
                    currentScene = Scene::MAP_SELECTION;
                    return;
                }
            }
        }
>>>>>>> Stashed changes
    } else {
        renderer.setMap(&gangGroundsMap);
    }
    ball1.setPosition({0.0,0.40}); ball1.setColor({1,0,0});
    ball2.setPosition({-0.5,0.45}); ball2.setColor({0,0,1});
    renderer.addPlayer(ball1);
    renderer.addPlayer(ball2);

    // 3) Swap GLUT callbacks
    glutDisplayFunc(gameDisplay);
    glutKeyboardFunc(gameKeyDown);
    glutKeyboardUpFunc(gameKeyUp);
    glutSpecialFunc(gameSpecialDown);
    glutSpecialUpFunc(gameSpecialUp);
    glutTimerFunc(16, updateGame, 0);

    // Done
    cout << "Launching " << mapName << " scene\n";
}

<<<<<<< Updated upstream
// ————————————————————————————————————————————————
// Menu drawing / logic
// ————————————————————————————————————————————————
static void drawText(float x,float y,const string& text){
=======
void MenuManager::handlePassiveMotion(int x,int y) {
    y = WINDOW_HEIGHT - y;

    if (showColorPicker) {
        float cell = PICKER_SIZE / float(PICKER_CELLS);
        float sx   = WINDOW_WIDTH - PICKER_MARGIN - PICKER_SIZE;
        float sy   = WINDOW_HEIGHT - PICKER_MARGIN - PICKER_SIZE;
        if (x>=sx && x<=sx+PICKER_SIZE && y>=sy && y<=sy+PICKER_SIZE) {
            hoverCol = int((x - sx)/cell);
            hoverRow = int((y - sy)/cell);
        } else {
            hoverCol = hoverRow = -1;
        }
    }

      hoveredColorBox = -1;
  for (int i = 0; i < 2; ++i) {
    auto &b = playerColorBoxes[i];
    bool over = x >= b.x && x <= b.x + b.width
             && y >= b.y && y <= b.y + b.height;
    if (over && hoveredColorBox != i) {
      soundPlayer.playSound("click");
      hoveredColorBox = i;
    }
  }
    auto &btns = (currentScene==Scene::MAIN_MENU ? menuButtons : mapButtons);
    for (auto &b : btns) {
        bool over = x>=b.x&&x<=b.x+b.width && y>=b.y&&y<=b.y+b.height;
        if (over && !b.highlighted)
            soundPlayer.playSound("click");
        b.highlighted = over;
    }
    highlightColorBox = (currentScene==Scene::MAIN_MENU 
                       && x>=300&&x<=330&&y>=435&&y<=465);
}

void MenuManager::onReshape(int w,int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,w,0,h);
    glMatrixMode(GL_MODELVIEW);
}

void MenuManager::launchGame(const std::string& mapName) {
    selectedMap = mapName;
    auto &G = Game::instance();
    G.playerColor1 = { playerColors[0].r,
                       playerColors[0].g,
                       playerColors[0].b };
    G.playerColor2 = { playerColors[1].r,
                       playerColors[1].g,
                       playerColors[1].b };
    G.changeScene(mapName);
   // G.setPlayerColors(playerColors[0], playerColors[1]);
}

// — helpers —

void MenuManager::drawText(float x,float y,const std::string& txt) {
>>>>>>> Stashed changes
    glRasterPos2f(x,y);
    for(char c:text) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

<<<<<<< Updated upstream
static void drawButton(const Button& b){
    if(b.highlighted){
=======
void MenuManager::drawStrokedText(float x,float y,const std::string& txt, void* font) {
    glColor3f(0,0,0);
    for (int dx=-1; dx<=1; ++dx) {
      for (int dy=-1; dy<=1; ++dy) {
        if (dx||dy) {
          glRasterPos2f(x+dx, y+dy);
          for (char c: txt) glutBitmapCharacter(font, c);
        }
      }
    }
    glColor3f(1,1,1);
    glRasterPos2f(x,y);
    for (char c: txt) glutBitmapCharacter(font, c);
}


void MenuManager::drawButton(const Button& b) {
    if (b.highlighted) {
>>>>>>> Stashed changes
        glColor3f(1,0.8f,0.85f);
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
          glVertex2f(b.x-2,b.y-2);
          glVertex2f(b.x+b.w+2,b.y-2);
          glVertex2f(b.x+b.w+2,b.y+b.h+2);
          glVertex2f(b.x-2,b.y+b.h+2);
        glEnd();
    }
    glColor3f(0.4f,0.25f,0.15f);
    glBegin(GL_QUADS);
      glVertex2f(b.x,    b.y);
      glVertex2f(b.x+b.w,b.y);
      glVertex2f(b.x+b.w,b.y+b.h);
      glVertex2f(b.x,    b.y+b.h);
    glEnd();
    glColor3f(1,1,1);
    drawText(b.x+20,b.y+18,b.label);
}

static void drawBackground(){
    if(gifFrames.empty()) return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,gifFrames[currentFrame]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
      glTexCoord2f(0,0); glVertex2f(0,0);
      glTexCoord2f(1,0); glVertex2f(WINDOW_WIDTH,0);
      glTexCoord2f(1,1); glVertex2f(WINDOW_WIDTH,WINDOW_HEIGHT);
      glTexCoord2f(0,1); glVertex2f(0,WINDOW_HEIGHT);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

<<<<<<< Updated upstream
static void drawMainMenu(){
    drawText(120,490,"Enter Username:");
=======
void MenuManager::drawMainMenu() {
    // 1) “Enter Username:” label
    drawStrokedText(120, 490, "Enter Username:");

    // 2) Username input box
>>>>>>> Stashed changes
    glColor3f(1,1,1);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
      glVertex2f(270,480);
      glVertex2f(530,480);
      glVertex2f(530,505);
      glVertex2f(270,505);
    glEnd();
<<<<<<< Updated upstream
    drawText(275,495,username);
    if(showCursor && username.size()<12){
        float cx=275+username.size()*10;
        glBegin(GL_LINES);
          glVertex2f(cx,495);
          glVertex2f(cx,507);
        glEnd();
    }
    if(triedToProceedWithoutUsername && username.empty()){
        glColor3f(1,0,0);
        drawText(540,495,"*required");
    }
    drawText(120,445,"Choose Color:");
    glColor3f(selectedColor.r,selectedColor.g,selectedColor.b);
    glBegin(GL_QUADS);
      glVertex2f(300,435); glVertex2f(330,435);
      glVertex2f(330,465); glVertex2f(300,465);
    glEnd();

    for(auto& b:menuButtons) drawButton(b);
=======

    // 3) Draw the current username text
    const float boxBottom = 480, boxTop = 505, fontH = 12;
    float textY = boxBottom + ((boxTop - boxBottom) - fontH)*0.5f;
    drawStrokedText(275, textY, username);

    // 4) Blinking cursor
    if (showCursor && username.size() < 12) {
      float cx = 275 + username.size()*10;
      glColor3f(1,1,1);
      glLineWidth(1);
      glBegin(GL_LINES);
        glVertex2f(cx,       textY);
        glVertex2f(cx, textY + fontH);
      glEnd();
    }

    // 5) “*required” if username empty
    if (triedToProceedWithoutUsername && username.empty()) {
      glColor3f(1,0,0);
      drawStrokedText(540, 495, "*required");
    }

    // 6) “Choose Color:” label
    drawStrokedText(120, 445, "Choose Color:");

    // 7) Player One color box + label
    const float boxW = 30, boxH = 30;
    const float startX = 300, startY = 435;
    const float gap    = 50;

    for (int i = 0; i < 2; ++i) {
    auto &col = playerColors[i];
    auto &b   = playerColorBoxes[i];
    glColor3f(col.r, col.g, col.b);
    glBegin(GL_QUADS);
        glVertex2f(b.x,      b.y);
        glVertex2f(b.x+b.width, b.y);
        glVertex2f(b.x+b.width, b.y+b.height);
        glVertex2f(b.x,      b.y+b.height);
    glEnd();

    // border on hover
    if (hoveredColorBox == i) {
        glColor3f(1,1,1);
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2f(b.x,      b.y);
        glVertex2f(b.x+b.width, b.y);
        glVertex2f(b.x+b.width, b.y+b.height);
        glVertex2f(b.x,      b.y+b.height);
        glEnd();
    }

    // label underneath
    const char* lbl = (i==0 ? "Player One" : "Player Two");
    drawStrokedText(b.x, b.y - 15, lbl, GLUT_BITMAP_HELVETICA_12);
    }

    // 9) Menu buttons
    for (auto &b : menuButtons) {
      drawButton(b);
    }
>>>>>>> Stashed changes
}

static void drawMapSelection(){
    for(auto& b:mapButtons) drawButton(b);
}

<<<<<<< Updated upstream
static void drawColorPicker(){
    if(!showColorPicker) return;
    float cell=PICKER_SIZE/float(PICKER_CELLS);
    float sx=WINDOW_WIDTH- PICKER_MARGIN - PICKER_SIZE;
    float sy=WINDOW_HEIGHT-PICKER_MARGIN - PICKER_SIZE;
    // white border
    glColor3f(1,1,1);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
      glVertex2f(sx-1,    sy-1);
      glVertex2f(sx+PICKER_SIZE+1,sy-1);
      glVertex2f(sx+PICKER_SIZE+1,sy+PICKER_SIZE+1);
      glVertex2f(sx-1,    sy+PICKER_SIZE+1);
    glEnd();
    // cells
    for(int row=0;row<PICKER_CELLS;row++){
      float v=1.0f-row/float(PICKER_CELLS-1);
      for(int col=0;col<PICKER_CELLS;col++){
        float h=col/float(PICKER_CELLS-1);
        int   i=int(h*6);
        float f=(h*6)-i;
        float p=v*(1-1.0f), q=v*(1-f), t=v*(1-(1-f));
=======
void MenuManager::drawColorPicker() {
    if (!showColorPicker) return;

    float cell = PICKER_SIZE / float(PICKER_CELLS);
    float sx   = WINDOW_WIDTH  - PICKER_MARGIN - PICKER_SIZE;
    float sy   = WINDOW_HEIGHT - PICKER_MARGIN - PICKER_SIZE;

    // 1) Outline the picker grid
    glColor3f(1,1,1);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
      glVertex2f(sx-1,                  sy-1);
      glVertex2f(sx+PICKER_SIZE+1,      sy-1);
      glVertex2f(sx+PICKER_SIZE+1, sy+PICKER_SIZE+1);
      glVertex2f(sx-1,             sy+PICKER_SIZE+1);
    glEnd();

    // 2) Draw each cell
    for (int row = 0; row < PICKER_CELLS; ++row) {
      float v = 1.0f - row / float(PICKER_CELLS - 1);
      for (int col = 0; col < PICKER_CELLS; ++col) {
        float h = col / float(PICKER_CELLS - 1);
        int   i = int(h * 6);
        float f = (h*6) - i;
        float p = 0.0f;
        float q = v * (1 - f);
        float t = v * (1 - (1 - f));

        // HSV→RGB with full saturation
>>>>>>> Stashed changes
        float r,g,b;
        switch (i % 6) {
          case 0: r=v; g=t; b=p; break;
          case 1: r=q; g=v; b=p; break;
          case 2: r=p; g=v; b=t; break;
          case 3: r=p; g=q; b=v; break;
          case 4: r=t; g=p; b=v; break;
          default:r=v; g=p; b=q; break;
        }

        float x0 = sx + col*cell;
        float y0 = sy + row*cell;

        // fill cell
        glColor3f(r,g,b);
        glBegin(GL_QUADS);
          glVertex2f(x0,        y0);
          glVertex2f(x0+cell,   y0);
          glVertex2f(x0+cell,   y0+cell);
          glVertex2f(x0,        y0+cell);
        glEnd();

        // highlight the hovered cell
        if (col == hoverCol && row == hoverRow) {
          glColor3f(1,1,1);
          glLineWidth(2);
          glBegin(GL_LINE_LOOP);
            glVertex2f(x0,        y0);
            glVertex2f(x0+cell,   y0);
            glVertex2f(x0+cell,   y0+cell);
            glVertex2f(x0,        y0+cell);
          glEnd();
        }
      }
    }
<<<<<<< Updated upstream
    // hover
    if(hoverCol>=0 && hoverRow>=0){
      glColor3f(1,1,1);
      glLineWidth(2);
      glBegin(GL_LINE_LOOP);
        glVertex2f(sx+hoverCol*cell,    sy+hoverRow*cell);
        glVertex2f(sx+(hoverCol+1)*cell,sy+hoverRow*cell);
        glVertex2f(sx+(hoverCol+1)*cell,sy+(hoverRow+1)*cell);
        glVertex2f(sx+hoverCol*cell,    sy+(hoverRow+1)*cell);
      glEnd();
    }
}

static void loadGifFrames(const string& dir){
    for(auto t:gifFrames) glDeleteTextures(1,&t);
    gifFrames.clear();
    vector<pair<int,fs::path>> list;
    regex pat("ball-(\\d+)\\.png");
    for(auto& e:fs::directory_iterator(dir)){
      smatch m; string fn=e.path().filename().string();
      if(regex_match(fn,m,pat)){
        list.emplace_back(stoi(m[1].str()), e.path());
      }
    }
    sort(list.begin(),list.end());
    for(auto& [_,p]: list){
      int w,h,ch;
      unsigned char* img = stbi_load(p.string().c_str(),&w,&h,&ch,STBI_rgb_alpha);
      if(!img){ cerr<<"bad: "<<p<<endl; continue; }
      GLuint tex; glGenTextures(1,&tex);
      glBindTexture(GL_TEXTURE_2D,tex);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,img);
      stbi_image_free(img);
      gifFrames.push_back(tex);
    }
}

static void initButtons(){
    float mx = (WINDOW_WIDTH-260)/2.0f;
    for(int i=0;i<menuButtons.size();i++){
      menuButtons[i].x = mx;
      menuButtons[i].y = 350 - i*75;
    }
    // mapButtons already hard-coded positions
}

// ————————————————————————————————————————————————
// GLUT menu callbacks
// ————————————————————————————————————————————————
static void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();
    if(currentScene==Scene::MAIN_MENU)      drawMainMenu();
    else /*MAP_SELECTION*/                 drawMapSelection();
    drawColorPicker();
    glutSwapBuffers();
}

static void timerCB_Menu(int){
    currentFrame = (currentFrame+1)%gifFrames.size();
    glutPostRedisplay();
    glutTimerFunc(100,timerCB_Menu,0);
}

static void keyboard(unsigned char key,int x,int y){
    if(key==8 && !username.empty()) username.pop_back();
    else if(username.size()<12 && isalnum(key)) username.push_back(key);
    usernameSaved = !username.empty();
    recentlySaved = true;
    lastSaveTime  = steady_clock::now();
}

static void mouse(int button,int state,int x,int y){
    if(button!=GLUT_LEFT_BUTTON||state!=GLUT_DOWN) return;
    y = WINDOW_HEIGHT - y;

    // color picker
    if(showColorPicker){
      float cell=PICKER_SIZE/float(PICKER_CELLS);
      float sx=WINDOW_WIDTH- PICKER_MARGIN - PICKER_SIZE;
      float sy=WINDOW_HEIGHT-PICKER_MARGIN - PICKER_SIZE;
      float dx=x-sx, dy=y-sy;
      if(dx>=0&&dy>=0&&dx<PICKER_SIZE&&dy<PICKER_SIZE){
        int col=int(dx/cell), row=int(dy/cell);
        float h=col/float(PICKER_CELLS-1),
              v=1.0f-row/float(PICKER_CELLS-1);
        int   i=int(h*6);
        float f=(h*6)-i, p=v*0, q=v*(1-f), t=v*(1-(1-f));
        float r,g,b;
        switch(i%6){
          case 0: r=v; g=t; b=p; break;
          case 1: r=q; g=v; b=p; break;
          case 2: r=p; g=v; b=t; break;
          case 3: r=p; g=q; b=v; break;
          case 4: r=t; g=p; b=v; break;
          default:r=v; g=p; b=q; break;
        }
        selectedColor={r,g,b};
      }
      showColorPicker=false;
      return;
    }

    // toggle picker
    if(x>=300&&x<=330&&y>=435&&y<=465){
      showColorPicker=!showColorPicker;
      soundPlayer.playSound("click");
      return;
    }

    // main menu buttons
    if(currentScene==Scene::MAIN_MENU){
      for(auto& b:menuButtons){
        if(x>=b.x&&x<=b.x+b.w && y>=b.y&&y<=b.y+b.h){
          if(!usernameSaved){ triedToProceedWithoutUsername=true; return; }
          if(b.label=="Quit") exit(0);
          if(b.label=="Local Player"){
            currentScene=Scene::MAP_SELECTION;
            return;
          }
        }
      }
    }
    // map selection → launch game
    else /*MAP_SELECTION*/{
      for(auto& b:mapButtons){
        if(x>=b.x&&x<=b.x+b.w && y>=b.y&&y<=b.y+b.h){
          launchGame(b.label);
          return;
        }
      }
    }
}

static void passiveMotion(int x,int y){
    y = WINDOW_HEIGHT - y;
    auto& btns = (currentScene==Scene::MAIN_MENU ? menuButtons : mapButtons);
    for(auto& b:btns){
      bool now = (x>=b.x&&x<=b.x+b.w&&y>=b.y&&y<=b.y+b.h);
      if(now && !b.highlighted) soundPlayer.playSound("click");
      b.highlighted = now;
    }
    glutPostRedisplay();
}

// ————————————————————————————————————————————————
// GLUT game callbacks
// ————————————————————————————————————————————————
static void gameDisplay(){
    glClear(GL_COLOR_BUFFER_BIT);
    renderer.display();
    glutSwapBuffers();
}
static void gameReshape(int w,int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float a=float(w)/float(h);
    if(a>=1.0f) gluOrtho2D(-a,a,-1,1);
    else       gluOrtho2D(-1,1,-1/a,1/a);
    glMatrixMode(GL_MODELVIEW);
}
static void gameKeyDown(unsigned char k,int x,int y){ inputManager.keyDown(k); }
static void gameKeyUp  (unsigned char k,int x,int y){ inputManager.keyUp(k); }
static void gameSpecialDown(int k,int x,int y){ inputManager.specialKeyDown(k); }
static void gameSpecialUp  (int k,int x,int y){ inputManager.specialKeyUp(k); }
static void updateGame(int){
    renderer.update();
    glutPostRedisplay();
    glutTimerFunc(16, updateGame, 0);
}

// ————————————————————————————————————————————————
// entrypoint
// ————————————————————————————————————————————————
int main(int argc, char** argv) {
    // GLUT + window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutCreateWindow("Bonk Menu");

    // init menu
    initButtons();
    soundPlayer.loadSound("click","assets/audio/click.wav");
    loadGifFrames("assets/gif");
    currentFrame=0;

    // init GL projection for menu
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    // register GLUT callbacks for menu
    glutDisplayFunc(display);
    glutTimerFunc(100,timerCB_Menu,0);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion);

    glutMainLoop();
    return 0;
}
=======
}
>>>>>>> Stashed changes
