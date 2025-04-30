#include <GL/glut.h>
#include "../../../include/ui/Map.h"
#include "../../../include/ui/OneVsOne.h"
#include "../../../include/ui/GangGrounds.h"
#include "../../../include/physics/Player.h"
#include "../../../include/core/Renderer.h"
#include "../../../include/core/InputManager.h"
#include "../../../include/physics/PhysicsEngine.h"
#include <iostream>

//PhysicsEngine physics;
InputManager inputManager;
Renderer renderer;
OneVsOne map;  
GangGrounds gangGrounds;
Player ball1(1);
Player ball2(2);

using namespace std;

void displaykoko(){
    renderer.display();
}

void keyDown(unsigned char key, int x, int y) {
    inputManager.keyDown(key);
}

void keyUp(unsigned char key, int x, int y) {
    inputManager.keyUp(key);
}

void specialKeyboard(int key, int x, int y) {
    inputManager.specialKeyDown(key);
}

void specialKeyboardUp(int key, int x, int y) {
    inputManager.specialKeyUp(key);
}

void update(int value) {
    renderer.update();
    glutTimerFunc(16, update, 0);
    // ball.handleInput(inputManager); // Use keyboard input
    // ball.tick();                    // Apply physics
    // physics.updatePhysics(ball, 0.016);
    // glutPostRedisplay();           // Refresh screen
    // glutTimerFunc(16, update, 0);  // 60 FPS
}


int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(850, 800);
    glutCreateWindow("ponk");

    glClearColor(0.1, 0.8, 0.6, 0.6);// alwaan el background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    // renderer.addPlatform(map);
    renderer.setMap(&gangGrounds);

    ball1.setPosition({0.0, 0.15}); //initial position for the ball, and ig we need to make a loop of players and set the position keda if we are playing with two
    ball1.setColor({1.0, 0.0, 0});
    ball2.setPosition({-0.5, 0.15});
    ball2.setColor({1.0, 0.647, 0.0});


    //ball.setJumping(true);
    renderer.addPlayer(ball1);
    renderer.addPlayer(ball2);
    // renderer.display();
    glutDisplayFunc(displaykoko); 
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutTimerFunc(0, update, 0);

 
    glutMainLoop();

    return 0;
}

