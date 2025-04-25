//hena opengl b2a we hnst5dm gameobkect and ui element for rendering 

#include <GL/glut.h>
#include "../../include/core/Renderer.h"
#include "../../include/ui/OneVsOne.h"


Renderer::Renderer(){
    

}

void Renderer::addPlayer(const Player& player) {
    players.push_back(player);
}
void Renderer::addPlatform(const GameObject& platform) {
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

void Renderer::setMap(OneVsOne& newMap) {
    map = newMap;
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
    }
    
    
    // Draw moving objects
    // for (const auto& movingObject : movingObjects) {
    //     movingObject.draw();
    // }
   
  

    glutSwapBuffers(); // to be added when zeina eats
}



