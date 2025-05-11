#pragma once
#include <utility>

using namespace std;

class GameObject; 
class Player;
class Platform;
class Bouncy;
class Swing;

class PhysicsEngine {
    public:
        PhysicsEngine();
        void updatePhysics(GameObject& object, double deltaTime);
        pair<double, double> applyForce(GameObject& object, double forceX, double forceY);
        //void applyGravity(GameObject& object, double gravity);
        bool checkCollision(GameObject& object1, GameObject& object2);
        void resolveCollision(GameObject& object1, GameObject& object2);
        void applyFriction(GameObject& object, double friction);
        void resolvePlayerCollision(Player& player1, Player& player2);
        void resolveWallCollision(Player& player, Platform& platform);
        //bool checkPlayerCollision(Player& player1, Player& player2);
        bool checkWallCollision(Player& player, Platform& platform);
        bool checkPlayerCollision(Player& p1, Player& p2);
        bool checkBouncyCollision(Player &player , Bouncy &bouncy);
        void resolveBouncyCollision(Player &player , Bouncy &bouncy);
        double applyGravity(Player& player);
        int checkGameOver(Player &player);
        bool checkSwingCollision(Player& player, Swing& swing);
        void resolveSwingCollision(Player& player, Swing& swing);

    private:
        const double gravity = 1;
        const double friction = 0.1;
        const double airResistance = 0.05;
        const double timeStep = 0.016; // 60 FPS
};