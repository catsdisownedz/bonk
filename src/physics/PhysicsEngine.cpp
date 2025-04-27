#include "../../include/physics/PhysicsEngine.h"
#include "../../include/physics/GameObject.h"
#include "../../include/physics/Player.h"
#include "../../include/physics/Platform.h"
#include <utility>
#include <math.h>
#include <iostream>

// Global physics instance used by the Player class
PhysicsEngine physics;

// need to handle collision at some point
// x and y accelerations are not 100% done
using namespace std;

PhysicsEngine::PhysicsEngine() {};
void PhysicsEngine::updatePhysics(GameObject &object, double deltaTime)
{
    const double maxStep = 0.01;
    double timeLeft = deltaTime;
    while (timeLeft > 0)
    {
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
        Player *player = dynamic_cast<Player *>(&object);
        if (player)
        {
            if (player->isJumping())
            {
                currentAcceleration.second -= gravity;
                player->setJumping(false);
            }
        }
        // currentAcceleration.second -= gravity;
        // psuh back into el game object
        object.update(currentPosition, currentVelocity, currentMomentum, currentAcceleration);
        timeLeft -= maxStep;
    }
    /*pair<double, double> currentVelocity = object.getVelocity();
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
    Player* player = dynamic_cast<Player*> (&object) ;
    if(player){
        if (player->isJumping()){
            currentAcceleration.second -= gravity;
            player->setJumping(false);
        }
    }
    //currentAcceleration.second -= gravity;
    //psuh back into el game object
    object.update(currentPosition, currentVelocity, currentMomentum, currentAcceleration);
    */
}
// f = m * a -> a = f / m
void PhysicsEngine::applyForce(GameObject &object, double forceX, double forceY)
{
    pair<double, double> currentAcceleration = object.getAcceleration();
    double xAcceleration = forceX / object.getMass();
    double yAcceleration = forceY / object.getMass();
    object.addAcceleration(xAcceleration, yAcceleration);
}

// needs adjustments based on object type
// if player -> pos + circle radius
// the idea would be calling this function in the game loop, and if it returns true, game loop should call another function on the 2 objects
// radius = 0.08
bool PhysicsEngine::checkCollision(GameObject &object1, GameObject &object2)
{
    Player *player1 = dynamic_cast<Player *>(&object1);
    Player *player2 = dynamic_cast<Player *>(&object2);
    Platform *platform1 = dynamic_cast<Platform *>(&object1);
    Platform *platform2 = dynamic_cast<Platform *>(&object2);

    // cout<<"idk";
    if (player1 && player2)
    {
        // cout << "[Physics] Checking player-player collision\n";
        // return(checkPlayerCollision(*player1, *player2));
        return false;
    }
    else if (player1 && platform2)
    {
        // cout<<"second if";
        // Platform* platform = dynamic_cast<Platform*> (&object2);
        // cout<<"casted";
        // cout << "[Physics] Checking player-platform collision\n";
        return (checkWallCollision(*player1, *platform2));
    }
    else if (player2 && platform1)
    {
        // cout << "[Physics] Checking player-platform collision (swapped)\n";
        return (checkWallCollision(*player2, *platform1));
    }

    return false;
}
// zeina's function
/*bool PhysicsEngine::checkWallCollision(Player& player, Platform& platform) {
    const double radius = 0.08;
    auto pos = player.getPosition();
    auto vel = player.getVelocity();
    auto platPos = platform.getPosition();
    double platWidth = platform.getWidth();
    double platLength = platform.getLength();
    bool isHorizontal = platform.isHorizontal();

    double playerBottom = pos.second - radius;
    double playerTop = pos.second + radius;
    double playerLeft = pos.first - radius;
    double playerRight = pos.first + radius;

    double platformTop = platPos.second;
    double platformBottom = platLength;
    double platformLeft = platPos.first;
    double platformRight = platformLeft + platLength;
    //cout<<platformTop <<" " << platformBottom <<" " <<platformLeft << platformRight <<"\n";

    if (isHorizontal) {
        // Allow a bigger vertical margin to catch falling balls
        bool withinHorizontal = (playerRight > platformLeft && playerLeft < platformRight);
        bool fallingOntoPlatform = (playerBottom >= platformTop - 0.02 && playerBottom <= platformTop + 0.02 && vel.second <= -0.001);

        return withinHorizontal && fallingOntoPlatform;
    } else {
        bool hitLeft = (playerRight >= platformLeft - 0.02 && playerLeft < platformLeft &&
            playerTop > platformBottom && playerBottom < platformTop && vel.first > 0);

        bool hitRight = (playerLeft <= platformRight + 0.02 && playerRight > platformRight &&
            playerTop > platformBottom && playerBottom < platformTop && vel.first < 0);

        return hitLeft || hitRight;
    }

}*/
// // omar and rola's function
// bool PhysicsEngine::checkWallCollision(Player& player, Platform& platform){
//     //cout<<"why ded";
//     pair<double, double> player_position = player.getPosition();
//     pair<double, double> platform_position = platform.getPosition();
//     //cout<<"are you ded";
//     double closestX = min(abs(player_position.first - platform_position.first), abs(player_position.first - (platform_position.first + platform.getLength())));
//     //double closestX = max(platform_position.first, min(player_position.first, platform_position.first + platform.getLength()));
//     double closestY= min(abs(player_position.second - platform_position.second) , abs(player_position.second - (platform_position.second - platform.getWidth())));
//     return ( closestX <= 0.08 || closestY <= 0.08);
// }

// chatgpt's func
//  bool PhysicsEngine::checkWallCollision(Player& player, Platform& platform) {
//      const double radius = 0.08;
//      auto pos = player.getPosition();
//      auto platPos = platform.getPosition();
//      double platWidth = platform.getWidth();
//      double platLength = platform.getLength();
//      bool isHorizontal = platform.isHorizontal();

//     double platformLeft = platPos.first;
//     double platformRight = platPos.first + platLength;
//     double platformTop = platPos.second;
//     double platformBottom = platPos.second - platWidth;

//     // Clamp player's center to platform bounds
//     double closestX = std::max(platformLeft, std::min(pos.first, platformRight));
//     double closestY = std::max(platformBottom, std::min(pos.second, platformTop));
//     //double closestX = min(abs(platPos.first - platformLeft), abs(platPos.first - platformRight));
//     //double closestY= min(abs(platPos.second - platformTop) , abs(platPos.second - platformBottom));

//     // Compute distance between player's center and closest point
//     double distanceX = pos.first - closestX;
//     double distanceY = pos.second - closestY;
//     double distanceSquared = distanceX * distanceX + distanceY * distanceY;
//     if(distanceSquared <= radius * radius){
//         cout<<"COLLISION"<<"\n";
//     }
//     return distanceSquared <= radius * radius;
// }

bool PhysicsEngine::checkPlayerCollision(Player &p1, Player &p2)
{
    auto pos1 = p1.getPosition();
    auto pos2 = p2.getPosition();
    double dx = pos1.first - pos2.first;
    double dy = pos1.second - pos2.second;
    double distanceSquared = dx * dx + dy * dy;
    double minDistance = 0.16; // (radius1 + radius2)^2
    return distanceSquared <= minDistance * minDistance;
}

bool PhysicsEngine::checkWallCollision(Player &player, Platform &platform)
{
    const double radius = 0.08;
    auto pos = player.getPosition();
    auto platPos = platform.getPosition();
    double platWidth = platform.getWidth();
    double platLength = platform.getLength();
    bool isHorizontal = platform.isHorizontal();

    double platformLeft = platPos.first;
    double platformRight = platPos.first + platLength;
    double platformTop = platPos.second;
    double platformBottom = platPos.second - platWidth;

    // Clamp player's center to platform bounds
    double closestX = std::max(platformLeft, std::min(pos.first, platformRight));
    double closestY = std::max(platformBottom, std::min(pos.second, platformTop));

    // Compute distance between player's center and closest point
    double distanceX = pos.first - closestX;
    double distanceY = pos.second - closestY;
    double distanceSquared = distanceX * distanceX + distanceY * distanceY;

    return distanceSquared <= radius * radius;
}

void PhysicsEngine::resolveCollision(GameObject &object1, GameObject &object2)
{
    Player *player1 = dynamic_cast<Player *>(&object1);
    Player *player2 = dynamic_cast<Player *>(&object2);
    Platform *platform1 = dynamic_cast<Platform *>(&object1);
    Platform *platform2 = dynamic_cast<Platform *>(&object2);

    if (player1 && player2)
    {
        resolvePlayerCollision(*player1, *player2);
    }
    else if (player1 && platform2)
    {
        resolveWallCollision(*player1, *platform2);
    }
    else if (player2 && platform1)
    {
        resolveWallCollision(*player2, *platform1);
    }
}

void PhysicsEngine::resolvePlayerCollision(Player &p1, Player &p2)
{
    // Elastic collision
    pair<double, double> delta = {
        p2.getPosition().first - p1.getPosition().first,
        p2.getPosition().second - p1.getPosition().second};

    double distance = sqrt(delta.first * delta.first + delta.second * delta.second);
    if (distance == 0.0)
        return;

    double x_Normal = delta.first / distance;
    double y_Normal = delta.second / distance;

    auto v1 = p1.getVelocity();
    auto v2 = p2.getVelocity();

    double v1n = v1.first * x_Normal + v1.second * y_Normal;
    double v2n = v2.first * x_Normal + v2.second * y_Normal;

    double m1 = p1.getMass();
    double m2 = p2.getMass();

    double v1nAfter = (v1n * (m1 - m2) + 2 * m2 * v2n) / (m1 + m2);
    double v2nAfter = (v2n * (m2 - m1) + 2 * m1 * v1n) / (m1 + m2);

    double dv1n = v1nAfter - v1n;
    double dv2n = v2nAfter - v2n;

    p1.setVelocity({v1.first + dv1n * x_Normal, v1.second + dv1n * y_Normal});
    p2.setVelocity({v2.first + dv2n * x_Normal, v2.second + dv2n * y_Normal});
}

void PhysicsEngine::resolveWallCollision(Player &player, Platform &platform)
{
    // cout <<"collision!";
    const double radius = 0.08;
    auto pos = player.getPosition();
    auto vel = player.getVelocity();
    auto acc = player.getAcceleration();
    auto platPos = platform.getPosition();
    double platWidth = platform.getWidth();
    double platLength = platform.getLength();
    bool isHorizontal = platform.isHorizontal();

    double platformTop = platPos.second;
    double platformBottom = platformTop - platWidth;
    double platformLeft = platPos.first;
    double platformRight = platformLeft + platLength;

    if (isHorizontal)
    {
        // Snap the ball ON TOP of the platform
        player.setPosition({pos.first, platformTop + radius});
        if (abs(vel.second) > 0.05)
        {
            vel.second = -abs(vel.second) * 0.3; // Always bounce upward slightly
        }
        else
        {
            vel.second = 0;
            player.setJumping(false);
            acc.second = 0;
            player.setAcceleration(acc);
        }

        player.setVelocity(vel);
        player.setAcceleration({acc.first, 0});

        // cout << "[Resolve] Landed on horizontal platform\n";
    }
    else
    {
        // (keep wall collision the same)
        double playerBottom = pos.second - radius;
        double playerTop = pos.second + radius;
        double playerLeft = pos.first - radius;
        double playerRight = pos.first + radius;

        //  if (playerRight >= platformLeft && playerLeft < platformLeft) {
        if (abs(player.getPosition().first + radius) <= abs(platPos.first))
        {
            // Ball hits left side of platform
            player.setPosition({platformLeft - radius - 0.001, pos.second}); // 🛠 Move slightly more left
            vel.first = -abs(vel.first) * 0.7;
            player.setVelocity(vel);
            cout << "[Resolve] Bounced off left wall\n";
            // Ball hits left side of platform
            player.setPosition({platformLeft - radius - 0.001, pos.second}); // 🛠 Move slightly more left
            vel.first = -abs(vel.first) * 0.7;
            player.setVelocity(vel);
            cout << "[Resolve] Bounced off left wall\n";
        }
        // else if (playerLeft <= platformRight && playerRight > platformRight) {
        else if (abs(player.getPosition().first + radius) <= abs(platPos.first + platLength))
        {
            // Ball hits right side of platform
            player.setPosition({platformRight + radius + 0.001, pos.second}); // 🛠 Move slightly more right
            vel.first = abs(vel.first) * 0.7;
            player.setVelocity(vel);
            cout << "[Resolve] Bounced off right wall\n";
        }
    }
}

void PhysicsEngine::applyFriction(GameObject &object, double friction)
{
    auto vel = object.getVelocity();
    vel.first *= (1 - friction);
    object.setVelocity(vel);
}