// File: ui/GameOver.cpp

#include <GL/glut.h>
#include <ui/GameOver.h>
#include <ui/Game.h>
#include <cmath>
#include <string>

GameOver::GameOver() { }

void GameOver::draw(int winningPlayer) {
    // 1) Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 2) Draw a full‐screen, semi-transparent black quad
    glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    glBegin(GL_QUADS);
      glVertex2f(-1.0f, -1.0f);
      glVertex2f( 1.0f, -1.0f);
      glVertex2f( 1.0f,  1.0f);
      glVertex2f(-1.0f,  1.0f);
    glEnd();

    // 3) Draw the winner’s color circle
    auto& winner = Game::instance().getPlayerById(winningPlayer);
    float cx =  0.0f;
    float cy =  0.6f;
    float r  =  0.15f;
     auto col = winner.getColorOption();
    glColor3f(col.r, col.g, col.b);

    const int slices = 64;
    glBegin(GL_TRIANGLE_FAN);
      glVertex2f(cx, cy);
      for (int i = 0; i <= slices; ++i) {
        float theta = 2.0f * M_PI * i / slices;
        glVertex2f(cx + cos(theta)*r,
                   cy + sin(theta)*r);
      }
    glEnd();

    // 4) Draw "USERNAME WINS!" in white, just under the circle
    glColor3f(1, 1, 1);
    renderText(cx - 0.4f,
               cy - 0.2f,
               winner.getName() + " WINS!",
               GLUT_BITMAP_HELVETICA_18);

    // 5) Draw the semi-transparent stats box
    float bw = 0.6f, bh = 0.3f, bx = 0.0f, by = -0.1f;
    glColor4f(1, 1, 1, 0.2f);
    glBegin(GL_QUADS);
      glVertex2f(bx - bw/2, by - bh/2);
      glVertex2f(bx + bw/2, by - bh/2);
      glVertex2f(bx + bw/2, by + bh/2);
      glVertex2f(bx - bw/2, by + bh/2);
    glEnd();

    // 6) List each player’s total win count
    float tx = bx - bw/2 + 0.05f;
    float ty = by + bh/2 - 0.05f;
    for (Player p : Game::instance().getPlayers()) {
        std::string line = p.getName()
                         + ": "
                         + std::to_string(Game::instance().getWinCount(p.getId()));
        renderText(tx, ty, line, GLUT_BITMAP_HELVETICA_12);
        ty -= 0.08f;
    }

    glDisable(GL_BLEND);

    // 7) Finally, swap buffers for this scene
    glutSwapBuffers();
}

void GameOver::renderText(float x,
                          float y,
                          const std::string& text,
                          void* font) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}
