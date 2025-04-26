#include "../../include/physics/PhysicsEngine.h"
#include "../../include/physics/GameObject.h"
#include "../../include/physics/Player.h"
#include "../../include/physics/platform.h"
#include <utility>
#include <math.h>
#include <iostream>

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
    //psuh back into el game object 
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
// radius = 0.08
bool PhysicsEngine::checkCollision(GameObject& object1, GameObject& object2){
    Player* player1 = dynamic_cast<Player*> (&object1);
    Player* player2 = dynamic_cast<Player*> (&object2);
    cout<<"idk";
    if(player1 && player2){
        //return(checkPlayerCollision(*player1, *player2));
        return false;
    }
    else if(player1){
        cout<<"second if";
        Platform* platform = dynamic_cast<Platform*> (&object2);
        cout<<"casted";
        return(checkWallCollision(*player1, *platform));
    }
    else{
        cout<<"third else";
        Platform* platform = dynamic_cast<Platform*> (&object1);
        return(checkWallCollision(*player2, *platform));

    }
}

bool PhysicsEngine::checkWallCollision(Player& player, Platform& platform){
    cout<<"why ded";
    pair<double, double> player_position = player.getPosition();
    pair<double, double> platform_position = platform.getPosition();
    cout<<"are you ded";
    double closestX = min(abs(player_position.first - platform_position.first), abs(player_position.first - (platform_position.first + platform.getLength())));
    //double closestX = max(platform_position.first, min(player_position.first, platform_position.first + platform.getLength()));
    double closestY= min(abs(player_position.second - platform_position.second) , abs(player_position.second - (platform_position.second - platform.getWidth())));
    return ( closestX <= 0.08 || closestY <= 0.08);     
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


void PhysicsEngine::resolvePlayerCollision(Player& p1, Player& p2) {
    // Step 1: Find the normal vector
    pair<double, double> delta = {
        p2.getPosition().first - p1.getPosition().first,
        p2.getPosition().second - p1.getPosition().second
    };

    double distance = sqrt(delta.first * delta.first + delta.second * delta.second);
    if (distance == 0.0) return; // avoid division by zero

    double x_Normal = delta.first / distance;
    double y_Normal = delta.second / distance;

    // Step 2: Get velocities
    auto v1 = p1.getVelocity();
    auto v2 = p2.getVelocity();

    // Step 3: Project velocities onto the collision normal
    double v1n = v1.first * x_Normal + v1.second * y_Normal;
    double v2n = v2.first * x_Normal + v2.second * y_Normal;

    // Step 4: Compute new normal velocities using elastic collision
    double m1 = p1.getMass();
    double m2 = p2.getMass();

    double v1nAfter = (v1n * (m1 - m2) + 2 * m2 * v2n) / (m1 + m2);
    double v2nAfter = (v2n * (m2 - m1) + 2 * m1 * v1n) / (m1 + m2);

    // Step 5: Compute change in velocity in normal direction
    double dv1n = v1nAfter - v1n;
    double dv2n = v2nAfter - v2n;

    // Step 6: Apply changes to original velocities
    p1.setVelocity({
        v1.first + dv1n * x_Normal,
        v1.second + dv1n * y_Normal
    });

    p2.setVelocity({
        v2.first + dv2n * x_Normal,
        v2.second + dv2n * y_Normal
    });
}

void PhysicsEngine::resolveWallCollision(Player& player, GameObject& wall) {
    //TODO: implement proper collision
    player.setVelocity({0,0});
}

void PhysicsEngine::applyFriction(GameObject& object, double friction) {
    pair<double, double> velocity = object.getVelocity();

    velocity.first *= (1 - friction);
    object.setVelocity(velocity);
}