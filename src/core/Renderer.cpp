//hena opengl b2a we hnst5dm gameobkect and ui element for rendering 

#include <GL/glut.h>
#include "../../include/core/Renderer.h"
#include "../../include/ui/OneVsOne.h"
#include "../../include/physics/PhysicsEngine.h"
#include "../../include/core/InputManager.h"
#include <iostream>

using namespace std;

extern InputManager inputManager;
PhysicsEngine physicsEngine;
Renderer::Renderer(){
    

}

void Renderer::addPlayer(const Player& player) {
    players.push_back(player);
}
void Renderer::addPlatform(GameObject platform) {
    platforms.push_back(platform);
}
void Renderer::addMovingObject(const GameObject& movingObject) {
    movingObjects.push_back(movingObject);
}
void Renderer::removePlayer(int id) {
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (it->getId() == id) {
            players.erase(it);
            break;
        }
    }
}

//need to edit this later
void Renderer::setMap(OneVsOne& newMap) {
    map = newMap;
    addPlatform(map.getPlatforms()[0]);
    addPlatform(map.getPlatforms()[1]);

}

void Renderer::display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    // Draw platforms
    // for ( auto& platform : platforms) {
    //     platform.drawPlatform();
    // }

    // Draw map
    map.draw();

    // Draw players
    for (auto& player : players) {
        player.display();
        //cout<<player.getVelocity().first;
    }
    
    
    // Draw moving objects
    // for (const auto& movingObject : movingObjects) {
    //     movingObject.draw();
    // }
    glutSwapBuffers(); // to be added when zeina eats
}

void Renderer::update() {
    for (auto& player : players) {
        //cout<< player.getAcceleration().second<<" "<<player.getVelocity().second<<"\n";
        player.handleInput(inputManager);         // Read player input
        physicsEngine.updatePhysics(player, 0.016); // Apply physics (gravity, velocity)

        for (auto& platform : map.getPlatforms()) {
            if (physicsEngine.checkCollision(player, platform)) {
                //cout << "Collided!\n";
                physicsEngine.resolveCollision(player, platform); // Only resolve if collision detected
                
                //cout << "[Renderer] Collision detected and resolved\n";
            }
        }

        // (Optional) If you have multiple players and want to check player-player collisions later:
        // for (auto& otherPlayer : players) {
        //     if (&otherPlayer != &player) {
        //         if (physicsEngine.checkCollision(player, otherPlayer)) {
        //             physicsEngine.resolveCollision(player, otherPlayer);
        //         }
        //     }
        // }
    }

    glutPostRedisplay(); // Request to redraw the scene
}


/*void update(int value) {
    ball.handleInput(inputManager); // Use keyboard input
    ball.tick();                    // Apply physics
    physics.updatePhysics(ball, 0.016);
    glutPostRedisplay();           // Refresh screen
    glutTimerFunc(16, update, 0);  // 60 FPS
}
*/



