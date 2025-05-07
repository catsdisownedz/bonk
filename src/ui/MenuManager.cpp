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
    glClearColor(0,0,0,1);
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
    } else {
        // map-selection
        for (auto &b : mapButtons) {
            if (x>=b.x && x<=b.x+b.width
             && y>=b.y && y<=b.y+b.height)
            {
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

void MenuManager::drawMainMenu() {
    // 1) “Enter Username:” label
    drawStrokedText(120, 490, "Enter Username:");

    // 2) Username input box
    glColor3f(1,1,1);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
      glVertex2f(270,480);
      glVertex2f(530,480);
      glVertex2f(530,505);
      glVertex2f(270,505);
    glEnd();

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