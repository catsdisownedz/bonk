//hena opengl b2a we hnst5dm gameobkect and ui element for rendering 

#include <GL/glut.h>
#include <core/Renderer.h>
#include <ui/OneVsOne.h>
#include <ui/GangGrounds.h>
#include <physics/PhysicsEngine.h>
#include <core/InputManager.h>
#include <physics/Swing.h>
#include <iostream>

using namespace std;

extern InputManager inputManager;
PhysicsEngine physicsEngine;
// Swing testSwing({0.0, 0.8}, {0.0, 0.1}, 0.1, 0.2); 


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
    if(map->getBouncies().size() > 0){
        for (auto& bouncy : map->getBouncies()) {
            addBouncies(bouncy);
        }
    }
}


void Renderer::display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Draw map
    if (map) {
    map->draw();
    //testSwing.draw();

    }

    // Draw players
    for (auto& player : players) {
        if(player.isSpaceBarPressed){
            player.drawHalo();
            player.isSpaceBarPressed=false;
        }
        player.display();
        player.drawName(); 
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
    double deltaTime = 1.0 / 60.0;
    int isGameOver=-1;
        if (map) {
        for (auto& platform : map->getPlatforms()) {
            platform.update(deltaTime);  // move any moving platforms
        }
      }
    for (auto& player : players) {
        player.handleInput(inputManager);
        player.tick();
        // to check if a player has fallen to end the round
        //isGameOver= physicsEngine.checkGameOver(player);

        if(isGameOver!=-1){
            cout<<"lemme hear you say YEAHAHHA"<< isGameOver<<endl;
            //displayGameOverScreen(isGameOver);
        }

        Platform* collidedPlatform = nullptr;
        Bouncy* collidedBouncy = nullptr;

        if (map) {
            for (auto& platform : map->getPlatforms()) {
                if (physicsEngine.checkCollision(player, platform)) {
                    //player.changeSurface();
                    //player.setJumping(false);
                    collidedPlatform = &platform;
                    physicsEngine.resolveCollision(player, *collidedPlatform);
                }
            }
            if(map->getBouncies().size() > 0){
                for(auto& bouncy : map->getBouncies()){
                    if(physicsEngine.checkCollision(player,bouncy)){
                        collidedBouncy=&bouncy;
                        physicsEngine.resolveCollision(player, *collidedBouncy);
                    }
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
        //  if(physicsEngine.checkSwingCollision(player, testSwing)){
        //          cout<<"collided with swing!\n";
        //     physicsEngine.resolveSwingCollision(player, testSwing);
        //     testSwing.updateSwingPhysics(0.016);
         //}
    } 
    if(isGameOver==-1)
        glutPostRedisplay(); 
} 

// void Renderer::displayGameOverScreen(int loserPlayer) {
//     // Display the Game Over screen here
//     glClear(GL_COLOR_BUFFER_BIT);
//     glColor3f(1.0f, 0.0f, 0.0f);  // Red color for game over text
//     glRasterPos2f(-0.6f, 0.0f);   // Position the text

//     string msg = "GAME OVER! Player " + to_string(loserPlayer ) + " lost ya lozar ya fashel ";
//     for (char c : msg) {
//         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);  // Render each character
//     }

//     glutSwapBuffers();  // Swap buffers to display the game over screen
// }



