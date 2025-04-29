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
        double step = min(maxStep, timeLeft);
        pair<double, double> currentVelocity = object.getVelocity();
        pair<double, double> currentAcceleration = object.getAcceleration();
        pair<double, double> currentPosition = object.getPosition();
        pair<double, double> currentMomentum = object.getMomentum();

        currentVelocity.first += currentAcceleration.first * step;
        currentVelocity.second += currentAcceleration.second * step;

        currentPosition.first += currentVelocity.first * step;
        currentPosition.second += currentVelocity.second * step;

        currentMomentum.first = object.getMass() * currentVelocity.first;
        currentMomentum.second = object.getMass() * currentVelocity.second;

        // need to handle X and y acceleration
       Player *player = dynamic_cast<Player *>(&object);
        if (player) {
            if (player->isJumping()) {
                currentAcceleration.second -= gravity * step; // Only apply gravity if still jumping
            } else {
                currentAcceleration.second = 0; // No more vertical acceleration
                currentVelocity.second = 0;     // No more vertical velocity
            }
        }


        object.update(currentPosition, currentVelocity, currentMomentum, currentAcceleration);
        timeLeft -= step;
    }
}
// f = m * a -> a = f / m
void PhysicsEngine::applyForce(GameObject &object, double forceX, double forceY)
{
    auto acc = object.getAcceleration();
    acc.first += forceX / object.getMass();
    acc.second += forceY / object.getMass();
    object.setAcceleration(acc);
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

    if (player1 && player2) {
        cout << "[CHECK] Checking player-player collision\n";
        return checkPlayerCollision(*player1, *player2);
    }
    else if (player1 && platform2) {
        cout << "[CHECK] Checking player-platform collision\n";
        return checkWallCollision(*player1, *platform2);
    }
    else if (player2 && platform1) {
        cout << "[CHECK] Checking player-platform collision (swapped)\n";
        return checkWallCollision(*player2, *platform1);
    }

    return false;
}

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
// rola and zeina's function
bool PhysicsEngine::checkWallCollision(Player &player, Platform &platform)
{
    const double radius = 0.08;
    auto pos = player.getPosition();
    auto platPos = platform.getPosition();
    PlatformBounds bounds = platform.getBounds();

    // Clamp player's center to platform bounds
    double closestX = max(bounds.left, min(pos.first, bounds.right));
    double closestY = max(bounds.bottom, min(pos.second, bounds.top));

    // Compute distance between player's center and closest point
    double distanceX = pos.first - closestX;
    double distanceY = pos.second - closestY;
    double distanceSquared = distanceX * distanceX + distanceY * distanceY;

    bool collision = distanceSquared <= radius * radius;

    if (collision) {
        cout << "[CHECK] Collision detected at player (" << pos.first << ", " << pos.second << ") and platform bounds ["
             << bounds.left << ", " << bounds.right << "] x [" << bounds.bottom << ", " << bounds.top << "]\n";
    }

    return collision;
}


void PhysicsEngine::resolveCollision(GameObject &object1, GameObject &object2)
{
    Player *player1 = dynamic_cast<Player *>(&object1);
    Player *player2 = dynamic_cast<Player *>(&object2);
    Platform *platform1 = dynamic_cast<Platform *>(&object1);
    Platform *platform2 = dynamic_cast<Platform *>(&object2);

    if (player1 && player2) {
        cout << "[RESOLVE] Resolving player-player collision\n";
        resolvePlayerCollision(*player1, *player2);
    }
    else if (player1 && platform2) {
        cout << "[RESOLVE] Resolving player-platform collision\n";
        resolveWallCollision(*player1, *platform2);
    }
    else if (player2 && platform1) {
        cout << "[RESOLVE] Resolving player-platform collision (swapped)\n";
        resolveWallCollision(*player2, *platform1);
    }
}

void PhysicsEngine::resolvePlayerCollision(Player &p1, Player &p2)
{
    // Elastic collision
//    / cout<<"[RESOLVE] Resolving player-player collision\n";
    double radius = 0.08;
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

    double overlap = (radius + radius) - distance;
    if (overlap > 0) {// omg there is an overlap
        double correctionFactor = 0.5; // move both players equally
        double correctionX = x_Normal * overlap * correctionFactor;
        double correctionY = y_Normal * overlap * correctionFactor;

        p1.setPosition({
            p1.getPosition().first - correctionX,
            p1.getPosition().second - correctionY
        });
        p2.setPosition({
            p2.getPosition().first + correctionX,
            p2.getPosition().second + correctionY
        });
    }
}

void PhysicsEngine::resolveWallCollision(Player& player, Platform& platform) {
    const double radius = 0.08;
    auto pos = player.getPosition();
    auto vel = player.getVelocity();
    auto acc = player.getAcceleration();
    
    PlatformBounds bounds = platform.getBounds();

    if (platform.isHorizontal()) {
        if (vel.second < 0.0) {
            vel.second = -vel.second * 0.8 + player.isFallingBoosted() * player.storedFallBoost;
             if (abs(vel.second) < 0.05) {  //If vertical speed is tiny after bounce
                vel.second = 0.0;          // Stop fully to prevent sinking
            }   
            player.enableBoostJump();
            player.resetFallBoost(); // reset after applying
        }

        if (abs(vel.second) < 0.03) { // 🔵 Slightly more tolerant than 0.02
            vel.second = 0.0;
            acc.second = 0.0;
            player.setAcceleration(acc);

            // 🔥 Snap the player a little upward to sit nicely on platform
            auto pos = player.getPosition();
            player.setPosition({pos.first, bounds.top + radius});

            player.setVelocity(vel);  // Apply the zero velocity
            player.landedRecently = true;
            player.landedTimer = 0.1;  // Allow small time to reboost
            cout << "[RESOLVE] Landed softly and snapped to platform\n";
        }



        player.setVelocity(vel);
        cout << "[RESOLVE] Bounced on horizontal platform\n";

    } else {
        if (vel.first > 0.0) {
            player.setPosition({bounds.left - radius - 0.001, pos.second});
            vel.first = -abs(vel.first) * 0.8;
            cout << "[RESOLVE] Bounced off left wall\n";
        }
        else if (vel.first < 0.0) {
            player.setPosition({bounds.right + radius + 0.001, pos.second});
            vel.first = abs(vel.first) * 0.8;
            cout << "[RESOLVE] Bounced off right wall\n";
        }

        player.setVelocity(vel);
    }
}



void PhysicsEngine::applyFriction(GameObject &object, double friction)
{
    auto vel = object.getVelocity();
    vel.first *= (1 - friction);
    object.setVelocity(vel);
}