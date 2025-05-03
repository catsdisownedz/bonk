#include "../../include/core/SoundPlayer.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cmath>
#include <algorithm>
#include "../../external/stb_image.h"

using namespace std;
SoundPlayer soundPlayer;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

struct Button {
    float x, y, width, height;
    string label;
    bool highlighted = false;
};

struct ColorOption {
    float r, g, b;
};

vector<Button> buttons = {
    {0, 0, 260, 50, "Multiplayer"},
    {0, 0, 260, 50, "Local Player"},
    {0, 0, 260, 50, "Options"},
    {0, 0, 260, 50, "Quit"}
};

vector<unsigned int> gifFrames;
int currentFrame = 0;
string username = "";
bool usernameSaved = false;
bool colorSaved = false;
bool showColorPicker = false;

ColorOption selectedColor = {1.0f, 1.0f, 1.0f};

void drawText(float x, float y, const string& text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(font, c);
}

void drawButton(const Button& btn) {
    if (btn.highlighted) {
        glColor3f(1.0f, 0.8f, 0.85f);
        glBegin(GL_QUADS);
        glVertex2f(btn.x - 5, btn.y - 5);
        glVertex2f(btn.x + btn.width + 5, btn.y - 5);
        glVertex2f(btn.x + btn.width + 5, btn.y + btn.height + 5);
        glVertex2f(btn.x - 5, btn.y + btn.height + 5);
        glEnd();
    }

    glColor3f(0.4f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(btn.x, btn.y);
    glVertex2f(btn.x + btn.width, btn.y);
    glVertex2f(btn.x + btn.width, btn.y + btn.height);
    glVertex2f(btn.x, btn.y + btn.height);
    glEnd();

    glColor3f(1, 1, 1);
    drawText(btn.x + 20, btn.y + 18, btn.label);
}

void drawBackground() {
    if (!gifFrames.empty()) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, gifFrames[currentFrame]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0, 0);
        glTexCoord2f(1, 0); glVertex2f(WINDOW_WIDTH, 0);
        glTexCoord2f(1, 1); glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        glTexCoord2f(0, 1); glVertex2f(0, WINDOW_HEIGHT);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    } else {
        glColor3f(0.3f, 0.25f, 0.15f);
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(WINDOW_WIDTH, 0);
        glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        glVertex2f(0, WINDOW_HEIGHT);
        glEnd();
    }
}

void drawColorPicker() {
    if (!showColorPicker) return;
    glPushMatrix();

    float startX = 120, startY = 110;
    float size = 10;

    glDisable(GL_TEXTURE_2D); // Don't let textures interfere
    for (int y = 0; y < 50; ++y) {
        for (int x = 0; x < 50; ++x) {
            float h = x / 50.0f;
            float s = 1.0f;
            float v = 1.0f - y / 50.0f;

            float r, g, b;
            int i = int(h * 6);
            float f = h * 6 - i;
            float p = v * (1 - s);
            float q = v * (1 - f * s);
            float t = v * (1 - (1 - f) * s);
            switch (i % 6) {
                case 0: r = v, g = t, b = p; break;
                case 1: r = q, g = v, b = p; break;
                case 2: r = p, g = v, b = t; break;
                case 3: r = p, g = q, b = v; break;
                case 4: r = t, g = p, b = v; break;
                case 5: r = v, g = p, b = q; break;
            }

            glColor3f(r, g, b);
            glBegin(GL_QUADS);
            glVertex2f(startX + x * size, startY + y * size);
            glVertex2f(startX + x * size + size, startY + y * size);
            glVertex2f(startX + x * size + size, startY + y * size + size);
            glVertex2f(startX + x * size, startY + y * size + size);
            glEnd();
        }
    }

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();

    drawText(310, 530, "Main Menu", GLUT_BITMAP_TIMES_ROMAN_24);
    drawText(120, 470, "Enter Username: " + username);
    drawText(120, 430, "Choose Color:");

    // Color square
    glColor3f(selectedColor.r, selectedColor.g, selectedColor.b);
    glBegin(GL_QUADS);
    glVertex2f(300, 420); glVertex2f(330, 420);
    glVertex2f(330, 450); glVertex2f(300, 450);
    glEnd();

    if (usernameSaved && colorSaved)
        drawText(600, 100, "✅ Saved");

    for (const Button& b : buttons)
        drawButton(b);

    drawColorPicker();
    glutSwapBuffers();
}

void timer(int value) {
    currentFrame = (currentFrame + 1) % gifFrames.size();
    glutPostRedisplay();
    glutTimerFunc(100, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 13 && !username.empty() && colorSaved) {
        usernameSaved = true;
    } else if (key == 8 && !username.empty()) {
        username.pop_back();
    } else if (username.length() < 12 && isalnum(key)) {
        username += key;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    y = WINDOW_HEIGHT - y;

    if (x >= 300 && x <= 330 && y >= 420 && y <= 450) {
        showColorPicker = !showColorPicker;
        glutPostRedisplay();
        return;
    }

    if (showColorPicker) {
        int relX = (x - 120) / 10;
        int relY = (y - 110) / 10;
        if (relX >= 0 && relX < 50 && relY >= 0 && relY < 50) {
            float h = relX / 50.0f;
            float s = 1.0f;
            float v = 1.0f - relY / 50.0f;

            int i = int(h * 6);
            float f = h * 6 - i;
            float p = v * (1 - s);
            float q = v * (1 - f * s);
            float t = v * (1 - (1 - f) * s);
            switch (i % 6) {
                case 0: selectedColor = {v, t, p}; break;
                case 1: selectedColor = {q, v, p}; break;
                case 2: selectedColor = {p, v, t}; break;
                case 3: selectedColor = {p, q, v}; break;
                case 4: selectedColor = {t, p, v}; break;
                case 5: selectedColor = {v, p, q}; break;
            }
            colorSaved = true;
            showColorPicker = false;
        } else {
            showColorPicker = false;
        }
        glutPostRedisplay();
        return;
    }

    for (auto& btn : buttons) {
        if (x >= btn.x && x <= btn.x + btn.width &&
            y >= btn.y && y <= btn.y + btn.height) {

            if (!usernameSaved || !colorSaved) {
                cout << "Fill in username and color first!\n";
                return;
            }

            if (btn.label == "Quit") exit(0);
            else if (btn.label == "Local Player") {
                system("start ../../build/output/opengl_glut.exe");
                glutDestroyWindow(glutGetWindow());
                exit(0);
            }
        }
    }
}

void passiveMotion(int x, int y) {
    y = WINDOW_HEIGHT - y;
    for (auto& btn : buttons) {
        bool wasHighlighted = btn.highlighted;
        btn.highlighted = (x >= btn.x && x <= btn.x + btn.width &&
                           y >= btn.y && y <= btn.y + btn.height);
        if (btn.highlighted && !wasHighlighted) {
            soundPlayer.playSound("click");
        }
    }
    glutPostRedisplay();
}

void loadGifFrames(const string& dir) {
    gifFrames.clear();
    for (const auto& entry : filesystem::directory_iterator(dir)) {
        if (entry.path().extension() == ".png") {
            int width, height, channels;
            unsigned char* image = stbi_load(entry.path().string().c_str(), &width, &height, &channels, STBI_rgb_alpha);
            if (!image) {
                cerr << "Failed to load image: " << entry.path() << endl;
                continue;
            }

            GLuint texID;
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_2D, texID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            stbi_image_free(image);
            gifFrames.push_back(texID);
        }
    }
    sort(gifFrames.begin(), gifFrames.end());
}

void initButtons() {
    float centerX = (WINDOW_WIDTH - buttons[0].width) / 2.0f;
    float startY = 350;
    float gap = 25;
    for (int i = 0; i < buttons.size(); ++i) {
        buttons[i].x = centerX;
        buttons[i].y = startY - i * (buttons[i].height + gap);
    }
}

void init() {
    glClearColor(0, 0, 0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    soundPlayer.loadSound("click", "../../assets/audio/click.wav");
    initButtons();
    loadGifFrames("../../assets/gif");
    glutTimerFunc(100, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Bonk Menu");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion);
    glutMainLoop();
    return 0;
}
