#include "../../include/core/SoundPlayer.h"
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

SoundPlayer soundPlayer;

struct Button {
    float x, y, width, height;
    string label;
    bool highlighted = false;
    bool wasHovered = false;  // 🔄 For hover tracking
};

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

Button buttons[4] = {
    {0, 0, 260, 50, "Multiplayer"},
    {0, 0, 260, 50, "Local Player"},
    {0, 0, 260, 50, "Options"},
    {0, 0, 260, 50, "Quit"}
};

void renderBitmapString(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
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

    glColor3f(1.0f, 1.0f, 1.0f);
    float text_x = btn.x + (btn.width / 2) - (btn.label.length() * 4.5f);
    float text_y = btn.y + (btn.height / 2) - 5;
    renderBitmapString(text_x, text_y, GLUT_BITMAP_HELVETICA_18, btn.label.c_str());
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderBitmapString(WINDOW_WIDTH / 2 - 70, 500, GLUT_BITMAP_TIMES_ROMAN_24, "Main Menu");

    for (const Button& btn : buttons) {
        drawButton(btn);
    }

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = WINDOW_HEIGHT - y;
        for (Button& btn : buttons) {
            if (x >= btn.x && x <= btn.x + btn.width &&
                y >= btn.y && y <= btn.y + btn.height) {

                cout << "Button clicked: " << btn.label << endl;
                if (btn.label == "Quit") {
                    exit(0);
                } else if (btn.label == "Local Player") {
                    system("../../../build/output/opengl_glut.exe");
                    glutDestroyWindow(glutGetWindow());
                    exit(0);
                }
            }
        }
    }
}

void passiveMotion(int x, int y) {
    y = WINDOW_HEIGHT - y;
    for (Button& btn : buttons) {
        bool nowHovering = (x >= btn.x && x <= btn.x + btn.width &&
                            y >= btn.y && y <= btn.y + btn.height);

        if (nowHovering && !btn.wasHovered) {
            soundPlayer.playSound("click");
        }

        btn.highlighted = nowHovering;
        btn.wasHovered = nowHovering;
    }
    glutPostRedisplay();
}

void initButtons() {
    float centerX = (WINDOW_WIDTH - buttons[0].width) / 2.0f;
    float startY = 370;
    float gap = 25;

    for (int i = 0; i < 4; ++i) {
        buttons[i].x = centerX;
        buttons[i].y = startY - i * (buttons[i].height + gap);
    }
}

void init() {
    glClearColor(0.3f, 0.25f, 0.32f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    initButtons();

    soundPlayer.loadSound("click", "../../assets/audio/click.wav");  // Adjust path if needed
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Main Menu");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion);

    glutMainLoop();
    return 0;
}
