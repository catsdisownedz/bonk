//hena opengl b2a we hnst5dm gameobkect and ui element for rendering 

#include <GL/glut.h>
#include "../../include/core/Renderer.h"
#include "../../include/ui/OneVsOne.h"
#include "../../include/ui/GangGrounds.h"
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
void Renderer::addBouncies(GameObject bouncy) {
    bouncies.push_back(bouncy);
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
void Renderer::setMap(Map* newMap) {
    map = newMap;
    for (auto& platform : map->getPlatforms()) {
        addPlatform(platform);
    }
    for (auto& bouncy : map->getBouncies()) {
        addBouncies(bouncy);
    }
}


void Renderer::display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Draw map
    if (map) {
    map->draw();
    }

    // Draw players
    for (auto& player : players) {
        if(player.isSpaceBarPressed){
            player.drawHalo();
            player.isSpaceBarPressed=false;
        }
        player.display();
        //cout<<"count:"<<players.size()<<"\n";
        
        // player's center
        auto pos = player.getPosition();
        glPointSize(8.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
            glVertex2f(pos.first, pos.second);
        glEnd();
    }
    //glEnd();
    glutSwapBuffers();
}


void Renderer::update() {
    for (auto& player : players) {
        player.handleInput(inputManager);
        player.tick();

        

        Platform* collidedPlatform = nullptr;
        Bouncy* collidedBouncy = nullptr;

        if (map) {
            for (auto& platform : map->getPlatforms()) {
                if (physicsEngine.checkCollision(player, platform)) {
                    collidedPlatform = &platform;
                    physicsEngine.resolveCollision(player, *collidedPlatform);
                }
            }

            for(auto& bouncy : map->getBouncies()){
                if(physicsEngine.checkCollision(player,bouncy)){
                    collidedBouncy=&bouncy;
                    //resolve
                }


            }

        }

        for(auto& otherPlayer : players){
            if(otherPlayer.getId() != player.getId()){
                if(physicsEngine.checkCollision(player, otherPlayer)){
                    physicsEngine.resolveCollision(player, otherPlayer);
                }
            }
        }
    } 

    glutPostRedisplay(); 
} 



