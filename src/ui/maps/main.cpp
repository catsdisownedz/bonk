#include <GL/glut.h>
#include "../../../include/ui/OneVsOne.h"
#include "../../../include/physics/Player.h"
#include "../../../include/core/Renderer.h"
#include "../../../include/core/InputManager.h"
#include "../../../include/physics/PhysicsEngine.h"
#include <iostream>
//PhysicsEngine physics;
InputManager inputManager;
OneVsOne map;  
Player ball;
Renderer renderer;
using namespace std;

void displaykoko(){
   
    renderer.display();
    // map.draw();
    // ball.display();
}

void keyDown(unsigned char key, int x, int y) {
    inputManager.keyDown(key);
}

void keyUp(unsigned char key, int x, int y) {
    inputManager.keyUp(key);
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
    glutInitWindowSize(700, 700);
    glutCreateWindow("ponk");

cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
    // Set up OpenGL environment
    glClearColor(0.1, 0.8, 0.6, 0.6);// alwan el background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    // renderer.addPlatform(map);
    renderer.setMap(map);

    ball.setPosition({0.0, 0.15}); //initial position for the ball, and ig we need to make a loop of players and set the position keda if we are playing with two
    //ball.setJumping(true);
    renderer.addPlayer(ball);
    // renderer.display();
    glutDisplayFunc(displaykoko); 
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutTimerFunc(0, update, 0);

 
    glutMainLoop();

    return 0;
}

