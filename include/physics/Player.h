#pragma once
#include <utility>
#include "GameObject.h"
#include "../core/InputManager.h"
using namespace std;

class Player : public GameObject {
    public:
        Player();
        void updateScore(){score++;};
        int getId(){return id;};
        int returnScore(){return score;};
        void move(unsigned char key,int x, int y);
        void update(pair<double, double> position, pair<double, double> velocity, pair<double, double> momentum, pair<double, double> acceleration);
        bool isJumping();
        void setJumping(bool isJumping);
        //bool Jump();
        void tick();
        bool getIsMoving();
        void draw();
        void display();
        void jump();
        void handleInput(const InputManager& input);
        void enableBoostJump() { canBoostJump = true; }
        void disableBoostJump() { canBoostJump = false; }
        void resetFallBoost() { storedFallBoost = 0.0; isBoostingFall = false; }
        bool isFallingBoosted() const { return isBoostingFall; }
        int score;
        int id;
        bool jumping;
        bool isAlive;
        bool isMoving;
        bool canBoostJump = false;
        bool isBoostingFall = false;
        double storedFallBoost = 0.0; //how much extra force to add into next bounce 
        double landedTimer = 0.0;
        bool landedRecently = false;
};

