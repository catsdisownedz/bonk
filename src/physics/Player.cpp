#include "../../include/physics/Player.h"
#include "../../include/core/InputManager.h"
#include "../../include/physics/PhysicsEngine.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

extern PhysicsEngine physics;

Player::Player() : GameObject(0, 0) {
    score = 0;
    id = 0;
    jumping = false;
    isAlive = true;
    isMoving = false;
}

void Player::update(pair<double, double> position, pair<double, double> velocity, pair<double, double> momentum, pair<double, double> acceleration) {
    GameObject::update(position, velocity, momentum, acceleration);
}

bool Player::isJumping() {
    return jumping;
}

bool Player::getIsMoving() {
    return isMoving;
}

void Player::handleInput(const InputManager& input) {
    auto vel = getVelocity();
    const double maxSpeed = 0.2;
    const double accel = 0.01;
    const double jumpVelocity = 0.25;

    bool moveLeft = input.isPressed('a');
    bool moveRight = input.isPressed('d');
    bool jumpPressed = input.isPressed('w');
    bool moveAndJumpLeft = input.isCombo('a', 'w');
    bool moveAndJumpRight = input.isCombo('d', 'w');

    // Movement handling
    if (moveLeft) {
        vel.first = max(vel.first - accel, -maxSpeed);
    } 
    if (moveRight) {
        vel.first = min(vel.first + accel, maxSpeed);
    }
    if (!moveLeft && !moveRight) {
        // Gradual slow down when no horizontal movement key pressed
        vel.first *= 0.9;
        if (abs(vel.first) < 0.001) vel.first = 0;
    }

    // Jumping handling
    if (jumpPressed && !jumping) {
        vel.second = jumpVelocity;
        jumping = true;
        cout << "[Input] Jump pressed (W)\n";
    }

    // Diagonal jump combos
    if (moveAndJumpLeft && !jumping) {
        vel.first = max(vel.first - accel, -maxSpeed);
        vel.second = jumpVelocity;
        jumping = true;
        cout << "[Combo] aw - Top Left Jump\n";
    } 
    if (moveAndJumpRight && !jumping) {
        vel.first = min(vel.first + accel, maxSpeed);
        vel.second = jumpVelocity;
        jumping = true;
        cout << "[Combo] wd - Top Right Jump\n";
    } 

    setVelocity(vel);
}

void Player::setJumping(bool isJumping) {
    jumping = isJumping;
}

void Player::draw() {
    auto pos = getPosition();
    glColor3f(1.0, 0.2, 0.2);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float rad = i * 3.14f / 180;
        glVertex2f(pos.first + 0.08 * cos(rad), pos.second + 0.08 * sin(rad));
    }
    glEnd();
}

void Player::jump() {
    if (!jumping) {
        setVelocity({getVelocity().first, 0.25}); // Make this match jumpVelocity
        jumping = true;
    }
}

void Player::display() {
    auto pos = getPosition();
    glPushMatrix();
    glTranslatef(pos.first, pos.second, 0.0);
    draw();
    glPopMatrix();
}

void Player::tick() {
    physics.updatePhysics(*this, 0.016); // Apply gravity, velocity, acceleration, position
}
