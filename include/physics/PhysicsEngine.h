#pragma once

using namespace std;

class GameObject; // Forward declaration of GameObject class
class Player;


class PhysicsEngine {
    public:
        PhysicsEngine();
        void updatePhysics(GameObject& object, double deltaTime);
        void applyForce(GameObject& object, double forceX, double forceY);
        void applyGravity(GameObject& object, double gravity);
        bool checkCollision(GameObject& object1, GameObject& object2);
        void resolveCollision(GameObject& object1, GameObject& object2);
        void applyFriction(GameObject& object, double friction);
        void resolvePlayerCollision(Player& player1, Player& player2);
        void resolveWallCollision(Player& player, GameObject& object);
    private:
        const double gravity = 9.8;
        const double friction = 0.1;
        const double airResistance = 0.05;
        const double timeStep = 0.016; // 60 FPS
};