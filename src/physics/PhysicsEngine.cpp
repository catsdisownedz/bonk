#include "../../include/physics/PhysicsEngine.h"
#include "../../include/physics/GameObject.h"
#include "../../include/physics/Player.h"
#include <utility>

//need to handle collision at some point
//x and y accelerations are not 100% done
using namespace std;

PhysicsEngine:: PhysicsEngine(){};
void PhysicsEngine::updatePhysics(GameObject& object, double deltaTime){
    pair<double, double> currentVelocity = object.getVelocity();
    pair<double, double> currentAcceleration = object.getAcceleration();
    pair<double, double> currentPosition = object.getPosition();
    pair<double, double> currentMomentum = object.getMomentum();

    currentVelocity.first += currentAcceleration.first * deltaTime;
    currentVelocity.second += currentAcceleration.second * deltaTime;

    currentPosition.first += currentVelocity.first * deltaTime;
    currentPosition.second += currentVelocity.second * deltaTime;

    currentMomentum.first = object.getMass() * currentVelocity.first;
    currentMomentum.second = object.getMass() * currentVelocity.second;

    // need to handle X and y acceleration
    if (Player* player = dynamic_cast<Player*> (&object)){
        if(player->isJumping()){
            currentAcceleration.second -= gravity;
        }
    }
    object.update(currentPosition, currentVelocity, currentMomentum, currentAcceleration);
}
// f = m * a -> a = f / m
void PhysicsEngine::applyForce(GameObject& object, double forceX, double forceY){
    pair<double, double> currentAcceleration = object.getAcceleration();
    double xAcceleration = forceX/object.getMass();
    double yAcceleration = forceY/object.getMass();
    object.addAcceleration(xAcceleration, yAcceleration);
}

// needs adjustments based on object type
// if player -> pos + circle radius
//the idea would be calling this function in the game loop, and if it returns true, game loop should call another function on the 2 objects
bool PhysicsEngine::checkCollision(GameObject& object1, GameObject& object2){
    pair<double, double> object1_position = object1.getPosition();
    pair<double, double> object2_position = object2.getPosition();
    if( object1_position.first == object2_position.second && object1_position.second == object2_position.second){
        return true;
    }
    else {
        return false;
    }
}

void PhysicsEngine::resolveCollision(GameObject& object1, GameObject& object2){
    Player* player1 = dynamic_cast<Player*> (&object1);
    Player* player2 = dynamic_cast<Player*> (&object2);
    if(player1 && player2){
        resolvePlayerCollision(*player1, *player2);
    }
    else if(player1){
        resolveWallCollision(*player1, object2);
    }
    else{
        resolveWallCollision(*player2, object1);
    }
}

void PhysicsEngine::applyFriction(GameObject& object, double friction) {
    pair<double, double> velocity = object.getVelocity();

    velocity.first *= (1 - friction);
    object.setVelocity(velocity);
}