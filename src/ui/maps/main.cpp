#include <GL/glut.h>
#include "OneVsOne.h"
#include "../../../include/physics/Player.h"

OneVsOne map;  // Declare the map here
Player ball;

void displaykoko(){
    glClear(GL_COLOR_BUFFER_BIT);
    map.draw();
    ball.display();
    // glFlush();
}

void keyboard(unsigned char key, int x, int y){
    ball.move(key,x,y);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutCreateWindow("ponk");

    // Set up OpenGL environment
    glClearColor(0.1, 0.8, 0.6, 0.6);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    glutDisplayFunc(displaykoko); 
     glutKeyboardFunc(keyboard);

    // Start the GLUT main loop
    glutMainLoop();

    return 0;
}

