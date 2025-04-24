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

void Player::draw(){
    auto pos = getPosition();
    glColor3f(1.0, 0.2, 0.2);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float rad = i * 3.14f / 180;
        glVertex2f(pos.first + 0.05 * cos(rad), pos.second + 0.05 * sin(rad));
    }
    glEnd();
}

void Player::jump() {
    if (!jumping) {
        setVelocity({getVelocity().first, 0.015});
        jumping = true;
    }
}