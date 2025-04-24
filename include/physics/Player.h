#pragma once
#include <utility>
#include "GameObject.h"
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
        bool Jump();
        bool getIsMoving();
        void draw();
        void display();
        void jump();
    private:
        int score;
        int id;
        bool jumping;
        bool isAlive;
        bool isMoving;

};

