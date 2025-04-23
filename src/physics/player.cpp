#include "include/physics/player.h"

Player::Player() : GameObject() {
    score = 0;
    id = 0;
    jumping = false;
    isAlive = true;
    isMoving = false;
}
void Player::update(pair<double, double> position, pair<double, double> velocity, pair<double, double> momentum, pair<double, double> acceleration) {
    GameObject::update(position, velocity, momentum, acceleration);
    
}
int Player::returnScore() {
    return score;
}
void Player::updateScore() {
    score++;
}
bool Player::isJumping(){
    return jumping;
}


