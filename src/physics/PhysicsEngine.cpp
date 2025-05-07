#include <physics/PhysicsEngine.h>
#include <physics/GameObject.h>
#include <physics/Player.h>
#include <physics/Platform.h>
#include <physics/Bouncy.h>
#include <utility>
#include <math.h>
#include <iostream>


PhysicsEngine physics;
using namespace std;

PhysicsEngine::PhysicsEngine() {};
void PhysicsEngine::updatePhysics(GameObject &object, double deltaTime)
{
    const double maxStep = 0.01;
    double timeLeft = deltaTime;
    double gravity = 0.01;
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

        currentAcceleration = {0, 0};

        // need to handle X and y acceleration
       Player *player = dynamic_cast<Player *>(&object);
        if (player) {
            currentMomentum.first = player->getCurrentMass() * currentVelocity.first;
            currentMomentum.second = player->getCurrentMass() * currentVelocity.second;

            if (player->isJumping() || !player->getOnSurface()) {
                cout<<player->isJumping()<<" "<<player->getOnSurface()<<"\n";
                //currentAcceleration.second -= gravity * step; // Only apply gravity if still jumping
                pair<double, double> gravitationalAcceleration = applyForce(*player, 0, -player->getCurrentMass() * gravity);
                currentAcceleration.first += gravitationalAcceleration.first;
                currentAcceleration.second += gravitationalAcceleration.second;
                // player->setJumping(false);
            } else {
                cout<<"changed back!\n";
                currentAcceleration.second = 0; // No more vertical acceleration
                //currentVelocity.second/= 2;     // No more vertical velocity
            }
        }
        else{
            currentMomentum.first = object.getMass() * currentVelocity.first;
            currentMomentum.second = object.getMass() * currentVelocity.second;
        }

        object.update(currentPosition, currentVelocity, currentMomentum, currentAcceleration);
        timeLeft -= step;
    }
}

// f = m * a -> a = f / m
pair<double, double> PhysicsEngine:: applyForce(GameObject& object, double forceX, double forceY){
    auto acc = object.getAcceleration();
    acc.first += forceX/object.getMass();
    acc.second += forceY / object.getMass();
    return {acc.first, acc.second};
}
//not used
double PhysicsEngine::applyGravity(Player& player){
    double mass = player.getCurrentMass();
    return -0.98;
}

bool PhysicsEngine::checkCollision(GameObject &object1, GameObject &object2)
{
    Player *player1 = dynamic_cast<Player *>(&object1);
    Player *player2 = dynamic_cast<Player *>(&object2);
    Platform *platform1 = dynamic_cast<Platform *>(&object1);
    Platform *platform2 = dynamic_cast<Platform *>(&object2);
    Bouncy *bouncy1=dynamic_cast<Bouncy *>(&object1);
    Bouncy *bouncy2=dynamic_cast<Bouncy *>(&object2);

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

    else if(player1 && bouncy2){
        cout<<"[CHECK] Checking player-bouncy collision\n";
        return checkBouncyCollision(*player1, *bouncy2);
    }

    else if(player2 && bouncy1){
        cout<<"[CHECK] Checking player-bouncy collision (swapped)\n";
        return checkBouncyCollision(*player2,*bouncy1);
    }

    return false;
}

void PhysicsEngine::resolveCollision(GameObject &object1, GameObject &object2)
{
    Player *player1 = dynamic_cast<Player *>(&object1);
    Player *player2 = dynamic_cast<Player *>(&object2);
    Platform *platform1 = dynamic_cast<Platform *>(&object1);
    Platform *platform2 = dynamic_cast<Platform *>(&object2);
    Bouncy *bouncy1=dynamic_cast<Bouncy *>(&object1);
    Bouncy *bouncy2=dynamic_cast<Bouncy *>(&object2);

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

    else if(player1 && bouncy2 ){
        cout<<"[Resolve] Resolving player-bouncy collision\n";
        resolveBouncyCollision(*player1 , *bouncy2);
    }

    else if(player2 && bouncy1){
        cout<<"[Resolve] Resolving player-bouncy collision (swapped)\n";
        resolveBouncyCollision(*player2, *bouncy1);
    }
}

bool PhysicsEngine::checkBouncyCollision(Player &player , Bouncy &bouncy){ //pall to pall collision
    auto playerPosition= player.getPosition();
    auto bouncyPosition=bouncy.getPosition();

    double dx= playerPosition.first- bouncyPosition.first;
    double dy= playerPosition.second- bouncyPosition.second;

    double distance = sqrt(dx*dx + dy*dy);

    return distance<= 0.08+ bouncy.getRadius();

}

void PhysicsEngine::resolveBouncyCollision(Player &player , Bouncy &bouncy){
    cout<<"alo meen ma3aya"<<endl;
    auto playerPos = player.getPosition();
    auto bouncyPos = bouncy.getPosition();

    double dx = playerPos.first - bouncyPos.first;
    double dy = playerPos.second - bouncyPos.second;
    double distance = sqrt(dx * dx + dy * dy);

    double minDistance = 0.08 + bouncy.getRadius();

    // bnd7k 3la b3d 3ashan mn3mlsh /0
    if (distance == 0.0) {
        dx = 0.0;
        dy = 1.0;
        distance = 1.0;
    }

    double overlap = minDistance - distance;
    if (overlap > 0.0) {
        // get normal
        double normalX = dx / distance;
        double normalY = dy / distance;

        player.setPosition({
            playerPos.first + normalX * overlap,
            playerPos.second + normalY * overlap
        });

        double bounceFactor=1.5; //the normal is 1 so we can play with it 
        auto velocity = player.getVelocity();
        player.setVelocity({
            -velocity.first*bounceFactor,
            -velocity.second*bounceFactor
        });
    }

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
    const double margin = 0.01;
    PlatformBounds bounds = platform.getBounds();
    bounds.left += margin;
    bounds.top -= margin;
    bounds.bottom += margin;
    bounds.right -= margin;

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

    double m1 = p1.getCurrentMass();
    double m2 = p2.getCurrentMass();

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
    const double margin = 0.01;
    auto pos = player.getPosition();
    auto vel = player.getVelocity();

    // Get the platform bounds
    PlatformBounds bounds = platform.getBounds();
    bounds.left += margin;
    bounds.right -= margin;
    bounds.top -= margin;
    bounds.bottom += margin;

    // Find closest point on platform to player
    double closestX = std::max(bounds.left, std::min(pos.first, bounds.right));
    double closestY = std::max(bounds.bottom, std::min(pos.second, bounds.top));

    // Calculate vector from closest point to player
    double dx = pos.first - closestX;
    double dy = pos.second - closestY;
    double distSq = dx * dx + dy * dy;

    // If already outside, skip
    if (distSq == 0.0) {
        // Perfect corner hit or center overlap — assume upward normal
        dx = 0;
        dy = 1;
        distSq = 1.0;
    }

    double distance = std::sqrt(distSq);
    double normalX = dx / distance;
    double normalY = dy / distance;

    // Project velocity onto normal
    double v_dot_n = vel.first * normalX + vel.second * normalY;

    // Only reflect if moving into wall
    if (v_dot_n < 0) {
        // Reflect velocity
        double bounceFactor = 0.3; // make this >1.0 for stronger bounce
        vel.first -= (1.5 + bounceFactor) * v_dot_n * normalX;
        vel.second -= (1 + bounceFactor) * v_dot_n * normalY;
        player.setVelocity(vel);
    }

    // Push player out of wall
    double overlap = radius - distance;
    if (overlap > 0) {
        pos.first += normalX * overlap;
        pos.second += normalY * overlap;
        player.setPosition(pos);
    }

    // vertical vs horizontal bit
    // instead of using vertical and horizontal, we just compare the player's position with the platform's top position
    if (normalY == 1 && vel.second > 0 && player.getPosition().second - radius <= bounds.top) {
        // The player has landed on top of the platform, so stop vertical velocity:
        //vel.second = 0;
        player.setVelocity(vel);
        player.setJumping(false);
        player.changeSurface();
    }
}

// this function has some eeshoes
/*
void PhysicsEngine::resolveWallCollision(Player& player, Platform& platform) {
    const double radius = 0.08;
    const double margin = 0.01;
    auto pos = player.getPosition();
    auto vel = player.getVelocity();

    // Get the platform bounds
    PlatformBounds bounds = platform.getBounds();
    bounds.left += margin;
    bounds.right -= margin;
    bounds.top -= margin;
    bounds.bottom += margin;

    // Find closest point on platform to player
    double closestX = std::max(bounds.left, std::min(pos.first, bounds.right));
    double closestY = std::max(bounds.bottom, std::min(pos.second, bounds.top));

    // Calculate vector from closest point to player
    double dx = pos.first - closestX;
    double dy = pos.second - closestY;
    double distSq = dx * dx + dy * dy;

    // If already outside, skip
    if (distSq == 0.0) {
        // Perfect corner hit or center overlap — assume upward normal
        dx = 0;
        dy = 1;
        distSq = 1.0;
    }

    double distance = std::sqrt(distSq);
    double normalX = dx / distance;
    double normalY = dy / distance;

    // Project velocity onto normal
    double v_dot_n = vel.first * normalX + vel.second * normalY;

    // Only reflect if moving into wall
    if (v_dot_n < 0) {
        // Reflect velocity
        double bounceFactor = 1.0; // make this >1.0 for stronger bounce
        vel.first -= (1 + bounceFactor) * v_dot_n * normalX;
        vel.second -= (1 + bounceFactor) * v_dot_n * normalY;
        player.setVelocity(vel);
    }

    // Push player out of wall
    double overlap = radius - distance;
    if (overlap > 0) {
        pos.first += normalX * overlap;
        pos.second += normalY * overlap;
        player.setPosition(pos);
    }
}*/

// this function has some other eeeshoes
/*
void PhysicsEngine::resolveWallCollision(Player& player, Platform& platform) {
    const double radius = 0.08;
    auto pos = player.getPosition();
    auto vel = player.getVelocity();
    auto acc = player.getAcceleration();
    const double margin = 0.01;
    
    PlatformBounds bounds = platform.getBounds();
    bounds.left += margin;
    bounds.top -= margin;
    bounds.bottom += margin;
    bounds.right -= margin;


    Player::PlayerBounds playerBounds = player.getBounds();

    if (platform.isHorizontal()) {
        // if (vel.second < 0.0) {
        //     vel.second = -vel.second * 0.8 + player.isFallingBoosted() * player.storedFallBoost;
        //      if (abs(vel.second) < 0.05) {  //If vertical speed is tiny after bounce
        //         vel.second = 0.0;          // Stop fully to prevent sinking
        //     }   
        //     player.enableBoostJump();
        //     player.resetFallBoost(); // reset after applying
        // }

        // if (abs(vel.second) < 0.03) { // 🔵 Slightly more tolerant than 0.02
        //     vel.second = 0.0;
        //     acc.second = 0.0;
        //     player.setAcceleration(acc);

        //     // 🔥 Snap the player a little upward to sit nicely on platform
        //     auto pos = player.getPosition();
        //     player.setPosition({pos.first, bounds.top + radius});

        //     player.setVelocity(vel);  // Apply the zero velocity
        //     player.landedRecently = true;
        //     player.landedTimer = 0.1;  // Allow small time to reboost
        //     cout << "[RESOLVE] Landed softly and snapped to platform\n";
        // }
        // player.setVelocity(vel);
        // cout << "[RESOLVE] Bounced on horizontal platform\n";
        if (playerBounds.bottom < bounds.top &&
            playerBounds.top > bounds.top &&
            playerBounds.right > bounds.left &&
            playerBounds.left < bounds.right &&
            vel.second < 0.0)
        {
            // bounce upward (with optional fall boost)
            vel.second = -vel.second * 0.8 + player.isFallingBoosted() * player.storedFallBoost;
            if (abs(vel.second) < 0.05) {
                vel.second = 0.0;
            }

            acc.second = 0.0;
            player.setAcceleration(acc);
            player.setVelocity(vel);
            player.enableBoostJump();
            player.resetFallBoost();
            player.landedRecently = true;
            player.landedTimer = 0.1;

            cout << "[RESOLVE] Bounced on horizontal platform (cleaned)\n";

    } else {
       // Right wall
        if (playerBounds.left < bounds.right && playerBounds.right > bounds.right &&
            playerBounds.top > bounds.bottom && playerBounds.bottom < bounds.top) {
            player.setPosition({bounds.right + 0.08 + 0.001, player.getPosition().second}); // 0.08 is radius
            vel.first = abs(vel.first) * 0.8;
            cout << "[RESOLVE] Bounced off right wall\n";
        }

        // Left wall
        else if (playerBounds.right > bounds.left && playerBounds.left < bounds.left &&
                playerBounds.top > bounds.bottom && playerBounds.bottom < bounds.top) {
            player.setPosition({bounds.left - 0.08 - 0.001, player.getPosition().second});
            vel.first = -abs(vel.first) * 0.8;
            cout << "[RESOLVE] Bounced off left wall\n";
        }

        player.setVelocity(vel);
        }
    }

}
*/


void PhysicsEngine::applyFriction(GameObject &object, double friction)
{
    auto vel = object.getVelocity();
    vel.first *= (1 - friction);
    object.setVelocity(vel);
}