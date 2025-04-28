#include "../../include/physics/Player.h"
#include "../../include/core/InputManager.h"
#include "../../include/physics/PhysicsEngine.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

extern PhysicsEngine physics;

Player::Player(int id) : GameObject(0, 0, 0, 0, 0) {
    this->id = id;
    score = 0;
    jumping = false;
    isAlive = true;
    isMoving = false;
}
// int Player::getId() {
//     return id;
// }

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
    const double maxSpeed = 0.4;               // Horizontal max speed
    const double moveAccel = 0.02;              // Horizontal acceleration
    const double baseJumpVelocity = 0.25;       // Normal jump power
    const double jumpBoost = 0.15;              // 🔵 Boost after bounce (smaller now)
    const double fallBoostForce = -0.12;        // 🔵 Force applied when pressing Space falling
    const double controlledJumpFactor = 0.85;   // 🔵 Shrink vertical speed when holding Space going up
    const double maxFallSpeed = -0.8;            // 🔵 Maximum allowed fall speed
    const double fallBoostGrowthRate = 0.005;    // 🔵 Rate at which boost grows while holding Space
    const double maxStoredFallBoost = 0.3;       // 🔵 Maximum boost you can store

    bool moveLeft ;
    bool moveRight;
    bool jumpPressed;
    bool spacePressed;

    bool moveAndJumpLeft;
    bool moveAndJumpRight;
    bool moveJumpSpaceLeft;
    bool moveJumpSpaceRight;

    if(getId() == 1){
        moveLeft = input.isPressed('a');
        moveRight = input.isPressed('d');
        jumpPressed = input.isPressed('w');
        spacePressed = input.isPressed(' ');

        moveAndJumpLeft = input.isCombo('a', 'w');
        moveAndJumpRight = input.isCombo('d', 'w');
        moveJumpSpaceLeft = input.isTripleCombo('a', 'w', ' ');
        moveJumpSpaceRight = input.isTripleCombo('d', 'w', ' ');
    }
    else if(getId() == 2){
        moveLeft = input.isSpecialPressed(GLUT_KEY_LEFT);  // Left Arrow
        moveRight = input.isSpecialPressed(GLUT_KEY_RIGHT); // Right Arrow
        jumpPressed = input.isSpecialPressed(GLUT_KEY_UP);  // Up Arrow (jump)
        spacePressed = input.isSpecialPressed(GLUT_KEY_DOWN);  // Space for jump

        moveAndJumpLeft = input.isSpecialCombo(GLUT_KEY_LEFT, GLUT_KEY_UP); // Left Arrow + Up Arrow
        moveAndJumpRight = input.isSpecialCombo(GLUT_KEY_RIGHT, GLUT_KEY_UP); // Right Arrow + Up Arrow
        moveJumpSpaceLeft = input.isSpecialTripleCombo(GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_DOWN); // Left Arrow + Up Arrow + Space
        moveJumpSpaceRight = input.isSpecialTripleCombo(GLUT_KEY_RIGHT, GLUT_KEY_UP, GLUT_KEY_DOWN); // Right Arrow + Up Arrow + Space
    }

    // --- Movement (X axis) ---
    if (moveLeft) {
        vel.first = max(vel.first - moveAccel, -maxSpeed);
    }
    if (moveRight) {
        vel.first = min(vel.first + moveAccel, maxSpeed);
    }
    if (!moveLeft && !moveRight) {
        if (isJumping()) {
            vel.first *= 0.99;  // Light air resistance
        } else {
            vel.first *= 0.9;   // Strong ground friction
        }
        if (abs(vel.first) < 0.001) vel.first = 0;
    }

    // --- Handle Jump Combos ---

    if (moveJumpSpaceLeft) { // A+W+Space
        if (!jumping) {
            vel.first = max(vel.first - moveAccel, -maxSpeed);
            vel.second = baseJumpVelocity * 0.8; // 🔵 Controlled diagonal jump
            vel.second -= 0.05;                  // 🔵 Heavier downward feeling
            jumping = true;
            resetFallBoost();
            cout << "[Triple Combo] AW+Space - Controlled Top Left Jump\n";
        }
    }
    else if (moveJumpSpaceRight) { // D+W+Space
        if (!jumping) {
            vel.first = min(vel.first + moveAccel, maxSpeed);
            vel.second = baseJumpVelocity * 0.8;
            vel.second -= 0.05;
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
    else if (jumpPressed && spacePressed) { // 🔥 W + Space together
        if (!jumping) {
            vel.second = baseJumpVelocity * 0.8; // 🔵 Controlled small vertical jump
            vel.second -= 0.05;                  // 🔵 Make it heavier
            jumping = true;
            resetFallBoost();
            cout << "[Combo] W+Space - Controlled Upward Jump\n";
        }
    }
    else if (jumpPressed) { // W only
        if (!jumping || landedRecently) {  // Allow boosting if just landed
            vel.second = baseJumpVelocity;
            jumping = true;
            resetFallBoost();
            landedRecently = false;
            cout << "[Input] W Jump\n";
        }
        else if (canBoostJump) { // Bounce boost
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
             if (vel.second > -0.15) { // 🔵 Always have minimum fall speed after pressing space
                vel.second = -0.15;
            }
            
            storedFallBoost += fallBoostGrowthRate; // 🔵 Gradual fall boost storage
            if (storedFallBoost > maxStoredFallBoost) {
                storedFallBoost = maxStoredFallBoost; // 🔵 Limit maximum stored boost
            }
            
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
        setVelocity({getVelocity().first, 0.25}); // Match base jump velocity
        jumping = true;
    }
}

void Player::draw() {
    auto pos = getPosition();
    auto color=getColor();
    glColor3f(color[0], color[1], color[2]);
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
    physics.updatePhysics(*this, 0.016);

    if (landedRecently) {
        landedTimer -= 0.016;
        if (landedTimer <= 0) {
            landedRecently = false;
        }
    }
}
