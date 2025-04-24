#include "../../include/physics/Player.h"
#include <GL/glut.h>
#include <cmath>

Player::Player() : GameObject(0,0) {
    score = 0;
    id = 0;
    jumping = false;
    isAlive = true;
    isMoving = false;
}
void Player::update(pair<double, double> position, pair<double, double> velocity, pair<double, double> momentum, pair<double, double> acceleration) {
    GameObject::update(position, velocity, momentum, acceleration);
    
}

bool Player::isJumping(){
    return jumping;
}

bool Player::getIsMoving() {
    return isMoving;
}
double XPos=0.0, YPos=0.0;

void Player::move(unsigned char key,int x, int y) {
    switch (key) {
        case 'a':
            XPos-=0.05;
            isMoving = true;
            break;
        case 'd':
            XPos+=0.05;            
            isMoving = true;
            break;
        // case GLUT_KEY_UP:
        //     jump();
        //     break;
        // default:
        //     isMoving = false;
        //     break;
    }
    glutPostRedisplay();
}

void Player::draw(){
    auto pos = getPosition();
    glColor3f(1.0, 0.2, 0.2);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float rad = i * 3.14f / 180;
        glVertex2f(pos.first + 0.08 * cos(rad), pos.second + 0.08 * sin(rad)+0.15);//we nedd to somehow coordinate it according to the map's dimensions
    }
    glEnd();
}


void Player::jump() {
    if (!jumping) {
        setVelocity({getVelocity().first, 0.015});
        jumping = true;
    }
}

void Player::display(){
    glPushMatrix();
    glTranslatef(XPos, YPos, 0.0);
    draw();
    glPopMatrix();
    //glFlush();
    glutSwapBuffers();
}