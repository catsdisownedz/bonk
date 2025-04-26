#include "../../include/physics/Player.h"
#include "../../include/core/InputManager.h"
#include "../../include/physics/PhysicsEngine.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

extern PhysicsEngine physics;

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

//double XPos=0.0, YPos=0.0;

void Player::handleInput(const InputManager& input) {
    auto vel = getVelocity();
    const double maxSpeed = 0.2;
    const double accel = 0.01;

    // Accelerate left or right
    if (input.isPressed('a')) {
        vel.first = max(vel.first - accel, -maxSpeed);
    } else if (input.isPressed('d')) {
        vel.first = min(vel.first + accel, maxSpeed);
    } else {
        //gradual slowing down
        vel.first *= 0.9;
        if (abs(vel.first) < 0.001) vel.first = 0;
    }

    // Jumping
    if (input.isPressed('w') && !jumping) {
        vel.second = 0.25;
        jumping = true;
    }
if (input.isCombo('a', 'w')){
        cout << "[Combo] aw - Top Left\n";
        vel.first = max(vel.first - accel, -maxSpeed);
        vel.second += 0.25;
    } 
    if (input.isCombo('d', 'w')){
        cout << "[Combo] wd - Top Right\n";
        vel.first = min(vel.first + accel, maxSpeed);
        vel.second += 0.25;
    } 

    // Print current velocity
    //cout << "Velocity: (" << vel.first << ", " << vel.second << ")\n";

    setVelocity(vel);
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
    auto pos = getPosition();
    glPushMatrix();
    glTranslatef(pos.first, pos.second, 0.0);
    draw();
    glPopMatrix();
    //glFlush();
    // glutSwapBuffers();
}

void Player::tick() {
    physics.updatePhysics(*this, 0.016); // Apply gravity, velocity, acceleration, position
}