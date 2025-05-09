// src/ui/MenuManager.cpp
#include <ui/MenuManager.h>
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
    glClearColor(0,0,0,1);std::string username2;
bool        username2Saved;
bool        triedToProceedWithoutUsername2;
bool        showSecondUsername = false;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);

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

    username2.clear();
    username2Saved = false;
    triedToProceedWithoutUsername2 = false;
    showSecondUsername = false; 
    activeUsername     = 1;

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

    bool curSaved = (activeUsername == 1 ? usernameSaved : username2Saved);
    auto now = std::chrono::steady_clock::now();
    if (curSaved && std::chrono::duration_cast<std::chrono::seconds>(now - lastSaveTime).count() < 2) {
        auto &col = (activeUsername == 1 ? playerColors[0] : playerColors[1]);
        drawStrokedText(WINDOW_WIDTH*0.5f - 30, 20, "Saved", GLUT_BITMAP_HELVETICA_18,
                        col.r, col.g, col.b);  // [MODIFIED] pass player color
    }

    glutSwapBuffers();
}

void MenuManager::handleKeyboard(unsigned char key,int x,int y) {
    // choose which field we’re editing:
    std::string &curName       = (activeUsername == 1 ? username  : username2);
    bool        &curSaved      = (activeUsername == 1 ? usernameSaved  : username2Saved);
    bool        &curTriedFlag  = (activeUsername == 1 ? triedToProceedWithoutUsername  : triedToProceedWithoutUsername2);

    if (key == 8) {               // Backspace
        if (!curName.empty()) curName.pop_back();
    }
    else if (isalnum(key) && curName.size() < 12) {
        curName.push_back(key);
    }
    // mark saved / “*required”
    curSaved     = true;
    curTriedFlag = !curSaved;
    lastSaveTime = std::chrono::steady_clock::now();
}



void MenuManager::handleKeyboardUp(unsigned char /*key*/,int/*x*/,int/*y*/) {
    // nothing for now
}

void MenuManager::handleMouse(int button,int state,int x,int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
    y = WINDOW_HEIGHT - y;

    if (currentScene == Scene::MAIN_MENU) {
        // [ADDED] Allow clicking on username fields to focus/re-edit
        float inputOffset = showSecondUsername ? 30.0f : 0.0f;
        // Player One field bounds
        float bot1 = 480.0f + inputOffset, top1 = 505.0f + inputOffset;
        if (x >= 270 && x <= 530 && y >= bot1 && y <= top1) {
            activeUsername = 1;
            showCursor = true;
            soundPlayer.playSound("click");
            return;
        }
        // Player Two field bounds (if shown)
        if (showSecondUsername) {
            float bot2 = 420.0f + inputOffset, top2 = 445.0f + inputOffset;
            if (x >= 270 && x <= 530 && y >= bot2 && y <= top2) {
                activeUsername = 2;
                showCursor = true;
                soundPlayer.playSound("click");
                return;
            }
        }

        int boxes = showSecondUsername ? 2 : 1;
        for (int i = 0; i < boxes; ++i) {
            auto &b = playerColorBoxes[i];
            if (x >= b.x && x <= b.x + b.width && y >= b.y && y <= b.y + b.height) {
                editingColorBox = i;
                showColorPicker = true;
                soundPlayer.playSound("click");
                return;
            }
        }
    }

    // [KEEP] Original color-picker and menu/map button logic:
    if (showColorPicker) {
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
            float r, g, b;
            switch (i % 6) {
            case 0: r=v; g=t; b=p; break;
            case 1: r=q; g=v; b=p; break;
            case 2: r=p; g=v; b=t; break;
            case 3: r=p; g=q; b=v; break;
            case 4: r=t; g=p; b=v; break;
            default: r=v; g=p; b=q; break;
            }
            playerColors[editingColorBox] = {r,g,b};
            showColorPicker = false;
            return;
        }
        return;
    }
    if (currentScene == Scene::MAIN_MENU) {
        // reveal second username or proceed
        for (auto &b : menuButtons) {
            if (x>=b.x&&x<=b.x+b.width&&y>=b.y&&y<=b.y+b.height) {
                if (b.label=="Local Player") {
                    if (!showSecondUsername) {
                        showSecondUsername = true;
                        activeUsername = 2;
                    } else {
                        if (username2Saved && !username2.empty()) {
                            currentScene = Scene::MAP_SELECTION;
                        } else {
                            triedToProceedWithoutUsername2 = true;
                        }
                    }
                    return;
                }
                if (b.label=="Quit") exit(0);
                if (!usernameSaved) { triedToProceedWithoutUsername = true; return; }
            }
        }
    } else {
        for (auto &b: mapButtons) {
            if (x>=b.x&&x<=b.x+b.width&&y>=b.y&&y<=b.y+b.height) {
                launchGame(b.label);
                return;
            }
        }
    }
}

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
    G.setPlayerNames(username,
                     showSecondUsername ? username2 : "");
    
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
    glRasterPos2f(x,y);
    for(char c: txt) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void MenuManager::drawStrokedText(float x, float y,
                                  const std::string &txt,
                                  void *font,
                                  float r, float g, float b) {
    // draw black outline
    glColor3f(0, 0, 0);
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx || dy) {
                glRasterPos2f(x + dx, y + dy);
                for (char c : txt) glutBitmapCharacter(font, c);
            }
        }
    }
    // draw fill in given color
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : txt) glutBitmapCharacter(font, c);
}

void MenuManager::drawButton(const Button& b) {
    if (b.highlighted) {
        glColor3f(1,0.8f,0.85f);
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
          glVertex2f(b.x-2,b.y-2);
          glVertex2f(b.x+b.width+2,b.y-2);
          glVertex2f(b.x+b.width+2,b.y+b.height+2);
          glVertex2f(b.x-2,b.y+b.height+2);
        glEnd();
    }
    glColor3f(0.4f,0.25f,0.15f);
    glBegin(GL_QUADS);
      glVertex2f(b.x, b.y);
      glVertex2f(b.x+b.width, b.y);
      glVertex2f(b.x+b.width, b.y+b.height);
      glVertex2f(b.x, b.y+b.height);
    glEnd();
    glColor3f(1,1,1);
    drawText(b.x+20,b.y+18,b.label);
}

void MenuManager::drawBackground() {
    if (gifFrames.empty()) return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gifFrames[currentFrame]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
      glTexCoord2f(0,0); glVertex2f(0,0);
      glTexCoord2f(1,0); glVertex2f(WINDOW_WIDTH,0);
      glTexCoord2f(1,1); glVertex2f(WINDOW_WIDTH,WINDOW_HEIGHT);
      glTexCoord2f(0,1); glVertex2f(0,WINDOW_HEIGHT);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// -----------------------------
// 2) drawMainMenu()
// -----------------------------
void MenuManager::drawMainMenu() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground(); 
    // shift elements when two username fields
    float inputOffset = showSecondUsername ? 30.0f : 0.0f;

    // Player One label & input
    float yLabel1 = 490.0f + inputOffset;
    drawStrokedText(120,yLabel1,"Enter Player One:");
    float boxBot1 = 480.0f + inputOffset, boxTop1 = 505.0f + inputOffset;
    glColor3f(1,1,1); glLineWidth(1);
    glBegin(GL_LINE_LOOP);
      glVertex2f(270,boxBot1); glVertex2f(530,boxBot1);
      glVertex2f(530,boxTop1); glVertex2f(270,boxTop1);
    glEnd();
    float textY1 = boxBot1 + ((boxTop1-boxBot1)-12.0f)*0.5f;
    drawStrokedText(275,textY1,username);
    if(activeUsername==1&&showCursor&&username.size()<12) {
      float cx = 275+username.size()*10;
      glBegin(GL_LINES);
        glVertex2f(cx,textY1); glVertex2f(cx,textY1+12.0f);
      glEnd();
    }
    if(triedToProceedWithoutUsername&&username.empty()) {
      glColor3f(1,0,0);
      drawStrokedText(540,yLabel1+5,"*required");
    }

    // Player Two if shown
    float yColorLabel, yColorBox;
    if(showSecondUsername) {
      float yLabel2 = 430.0f + inputOffset;
      drawStrokedText(120,yLabel2,"Enter Player Two:");
      float boxBot2 = 420.0f + inputOffset, boxTop2 = 445.0f + inputOffset;
      glBegin(GL_LINE_LOOP);
        glVertex2f(270,boxBot2); glVertex2f(530,boxBot2);
        glVertex2f(530,boxTop2); glVertex2f(270,boxTop2);
      glEnd();
      float textY2 = boxBot2 + ((boxTop2-boxBot2)-12.0f)*0.5f;
      drawStrokedText(275,textY2,username2);
      if(activeUsername==2&&showCursor&&username2.size()<12) {
        float cx2 = 275+username2.size()*10;
        glBegin(GL_LINES);
          glVertex2f(cx2,textY2); glVertex2f(cx2,textY2+12.0f);
        glEnd();
      }
      if(triedToProceedWithoutUsername2&&username2.empty()) {
        glColor3f(1,0,0);
        drawStrokedText(540,yLabel2+5,"*required");
      }
      // move color picker down
      yColorLabel = 445.0f - inputOffset;
      yColorBox   = 435.0f - inputOffset;
    } else {
      yColorLabel = 445.0f;
      yColorBox   = 435.0f;
    }

    // Color picker & boxes
    drawStrokedText(120,yColorLabel,"Choose Color:");
    int boxes = showSecondUsername ? 2 : 1;
    for(int i=0;i<boxes;++i) {
      auto &box = playerColorBoxes[i];
      box.y = yColorBox; // [MODIFIED] shift down if needed
      auto &col = playerColors[i];
      glColor3f(col.r,col.g,col.b);
      glBegin(GL_QUADS);
        glVertex2f(box.x,box.y);
        glVertex2f(box.x+box.width,box.y);
        glVertex2f(box.x+box.width,box.y+box.height);
        glVertex2f(box.x,box.y+box.height);
      glEnd();
      if(hoveredColorBox==i) {
        glColor3f(1,1,1); glLineWidth(2);
        glBegin(GL_LINE_LOOP);
          glVertex2f(box.x,box.y);
          glVertex2f(box.x+box.width,box.y);
          glVertex2f(box.x+box.width,box.y+box.height);
          glVertex2f(box.x,box.y+box.height);
        glEnd();
      }
      drawStrokedText(box.x,box.y-15,(i==0?"Player One":"Player Two"),GLUT_BITMAP_HELVETICA_12);
    }

    // [MODIFIED] shift buttons down when two username fields
    float btnOffset = showSecondUsername ? -30.0f : 0.0f;
    for(auto &b:menuButtons) {
      Button tmp = b;
      tmp.y += btnOffset;
      drawButton(tmp);
    }
}


void MenuManager::drawMapSelection() {
    for (auto &b : mapButtons) drawButton(b);
}

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
}