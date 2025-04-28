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
    
    const double maxSpeed = 0.4;
    const double moveAccel = 0.02;
    const double baseJumpVelocity = 0.25;
    const double jumpBoost = 0.2;
    const double fallBoostForce = -0.25;
    const double controlledJumpFactor = 0.85;

    bool moveLeft = input.isPressed('a');
    bool moveRight = input.isPressed('d');
    bool jumpPressed = input.isPressed('w');
    bool spacePressed = input.isPressed(' ');

    bool moveAndJumpLeft = input.isCombo('a', 'w');
    bool moveAndJumpRight = input.isCombo('d', 'w');
    bool moveJumpSpaceLeft = input.isTripleCombo('a', 'w', ' ');
    bool moveJumpSpaceRight = input.isTripleCombo('d', 'w', ' ');

    // --- Movement ---
    if (moveLeft) {
        vel.first = max(vel.first - moveAccel, -maxSpeed);
    }
    if (moveRight) {
        vel.first = min(vel.first + moveAccel, maxSpeed);
    }
    if (!moveLeft && !moveRight) {
    if (isJumping()) {
        //In air, don't slow down X completely (preserve momentum)
        vel.first *= 0.99;  // VERY light air resistance
    } else {
        // On ground: strong friction
        vel.first *= 0.9;
    }

    if (abs(vel.first) < 0.001) vel.first = 0;
}

    // --- Handle Jump Combos ---

    if (moveJumpSpaceLeft) { // A+W+Space
        if (!jumping) {
            vel.first = max(vel.first - moveAccel, -maxSpeed);
            vel.second = baseJumpVelocity * 0.8; // Controlled diagonal jump
            jumping = true;
            resetFallBoost();
            cout << "[Triple Combo] AW+Space - Controlled Top Left Jump\n";
        }
    }
    else if (moveJumpSpaceRight) { // D+W+Space
        if (!jumping) {
            vel.first = min(vel.first + moveAccel, maxSpeed);
            vel.second = baseJumpVelocity * 0.8;
            jumping = true;
            resetFallBoost();
            cout << "[Triple Combo] WD+Space - Controlled Top Right Jump\n";
        }
    }
    else if (moveAndJumpLeft) { // A+W
        if (!jumping) {
            vel.first = max(vel.first - moveAccel, -maxSpeed);
            vel.second = baseJumpVelocity;
            jumping = true;
            resetFallBoost();
            cout << "[Combo] AW - Top Left Jump\n";
        } 
        else if (canBoostJump) {
            vel.second += jumpBoost + storedFallBoost;
            canBoostJump = false;
            storedFallBoost = 0.0;
            cout << "[Combo Boost] AW + W after bounce\n";
        }
    }
    else if (moveAndJumpRight) { // D+W
        if (!jumping) {
            vel.first = min(vel.first + moveAccel, maxSpeed);
            vel.second = baseJumpVelocity;
            jumping = true;
            resetFallBoost();
            cout << "[Combo] WD - Top Right Jump\n";
        }
        else if (canBoostJump) {
            vel.second += jumpBoost + storedFallBoost;
            canBoostJump = false;
            storedFallBoost = 0.0;
            cout << "[Combo Boost] WD + W after bounce\n";
        }
    }
    else if (jumpPressed) { // W only
        if (!jumping) {
            vel.second = baseJumpVelocity;
            jumping = true;
            resetFallBoost();
            cout << "[Input] W Jump\n";
        }
        else if (canBoostJump) {
            vel.second += jumpBoost + storedFallBoost;
            canBoostJump = false;
            storedFallBoost = 0.0;
            cout << "[Input] W after bounce + stored momentum\n";
        }
    }

    // --- Handle Space separately (fall control) ---
    if (spacePressed) {
        if (vel.second < 0.0 || (vel.second < 0.1 && (vel.first > 0.05 || vel.first < -0.05))) {
            // Falling vertically or diagonally
            vel.second += fallBoostForce;
            storedFallBoost += abs(fallBoostForce * 1.2); // Store boost
            isBoostingFall = true;
            cout << "[Space] Boosted Fall\n";
        } 
        else if (vel.second > 0.0) {
            vel.second *= controlledJumpFactor;
            cout << "[Space] Controlled Upward Jump\n";
        }
    }

    setVelocity(vel);
}


void Player::setJumping(bool isJumping) {
    jumping = isJumping;
}


void Player::jump() {
    if (!jumping) {
        setVelocity({getVelocity().first, 0.25}); // Make this match jumpVelocity
        jumping = true;
    }
}

void Player::draw() {
    auto pos = getPosition();
    glColor3f(1.0, 0.2, 0.2);
    glBegin(GL_POLYGON);
      for (int i = 0; i < 360; i += 10) {
        float rad = i * 3.14159f / 180;
        glVertex2f(pos.first + 0.08 * cos(rad), pos.second + 0.08 * sin(rad));
      }
    glEnd();
}

void Player::display() {
    draw(); 
}

void Player::tick() {
    physics.updatePhysics(*this, 0.016); // Apply gravity, velocity, acceleration, position
}
