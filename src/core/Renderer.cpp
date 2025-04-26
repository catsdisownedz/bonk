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
void Renderer::update(){
    for (auto& player: players){
        player.handleInput(inputManager);
        //player.tick();
        /*for(auto& platform: platforms){
            bool collided = physicsEngine.checkCollision(player, platform);
            cout<< collided <<'\n';
        }*/
        physicsEngine.updatePhysics(player, 0.016);

        //cout<<player.getVelocity().first;
    }
    glutPostRedisplay();
    
}

/*void update(int value) {
    ball.handleInput(inputManager); // Use keyboard input
    ball.tick();                    // Apply physics
    physics.updatePhysics(ball, 0.016);
    glutPostRedisplay();           // Refresh screen
    glutTimerFunc(16, update, 0);  // 60 FPS
}
*/



