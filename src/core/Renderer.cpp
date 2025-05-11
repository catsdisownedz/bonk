// Renderer.cpp
// hena opengl b2a we hnst5dm gameobkect and ui element for rendering

#include <GL/glut.h>
#include <core/Renderer.h>
#include <ui/OneVsOne.h>
#include <ui/GangGrounds.h>
#include <physics/PhysicsEngine.h>
#include <core/InputManager.h>
#include <physics/Swing.h>
#include <ui/Game.h>
#include <ui/GameOver.h> 
#include <iostream>
#include <map>

using namespace std;

extern InputManager inputManager;
PhysicsEngine physicsEngine;
// Swing testSwing({0.0, 0.8}, {0.0, 0.1}, 0.1, 0.2); 



Renderer::Renderer() { }

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

void Renderer::setMap(Map* newMap) {
    _map = newMap;
    for (auto& platform : _map->getPlatforms()) {
        addPlatform(platform);
    }
    if (_map->getBouncies().size() > 0) {
        for (auto& bouncy : _map->getBouncies()) {
            addBouncies(bouncy);
        }
    }
}

void Renderer::display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // 1) draw the _map
    if (_map) {
        _map->draw();
    }

    // 2) draw players
    for (auto& player : players) {
        if (player.isSpaceBarPressed) {
            player.drawHalo();
            player.isSpaceBarPressed = false;
        }
        player.display();
        player.drawName();

        // debug: draw center point
        auto pos = player.getPosition();
        glPointSize(8.0f);
        glColor3f(1,1,0);
        glBegin(GL_POINTS);
          glVertex2f(pos.first, pos.second);
        glEnd();
    }

    // 3) if game-over, overlay the GameOver scene here
    if (_gameOver) {
        GameOver go;
        go.draw(_winnerId);
    }

    glutSwapBuffers();
}

void Renderer::update() {
    if (_gameOver) return;

    for (auto& player : players) {
        player.handleInput(inputManager);
        player.tick();

        // world collisions
        Platform* collidedPlatform = nullptr;
        Bouncy*   collidedBouncy  = nullptr;
        if (_map) {
            for (auto& platform : _map->getPlatforms()) {
                if (physicsEngine.checkCollision(player, platform)) {
                    collidedPlatform = &platform;
                    physicsEngine.resolveCollision(player, *collidedPlatform);
                }
            }
            for (auto& bouncy : _map->getBouncies()) {
                if (physicsEngine.checkCollision(player, bouncy)) {
                    collidedBouncy = &bouncy;
                    physicsEngine.resolveCollision(player, *collidedBouncy);
                }
            }
        }

        // player vs player
        for (auto& other : players) {
            if (other.getId() != player.getId()) {
                if (physicsEngine.checkCollision(player, other)) {
                    physicsEngine.resolveCollision(player, other);
                }
            }
        }

        // did someone fall off?
        int loser = physicsEngine.checkGameOver(player);
        if (loser != -1) {
           int winnerId = (loser == 1) ? 2 : 1;

            triggerGameOver(loser);
            return;
        }
    }

    // keep redrawing until game-over
    glutPostRedisplay();
}


// mark the game as over, record the winner
void Renderer::triggerGameOver(int loserId) {
    auto& G = Game::instance();
    _gameOver = true;
    for (auto& p : players) {
        if (p.getId() != loserId) {
            _winnerId = p.getId();
            string winnerUser= G.getPlayerName(_winnerId);

            G.recordWin(_winnerId);
            break;
        }
    }
    G.changeScene("GameOver");
    //winCounts[_winnerId]++;
}

// UNUSED helpers—commented out so you can delete later if you want
// void Renderer::drawGameOverOverlay() { /*…*/ }
// void Renderer::drawCircle(float cx, float cy, float r) { /*…*/ }
// void Renderer::drawText(float x, float y, const std::string &s) { /*…*/ }
