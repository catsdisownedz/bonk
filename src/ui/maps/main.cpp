#include <GL/glut.h>
#include "../../../include/ui/OneVsOne.h"
#include "../../../include/physics/Player.h"
#include "../../../include/core/Renderer.h"
OneVsOne map;  
Player ball;
Renderer renderer;

void displaykoko(){
   
    renderer.display();
    // map.draw();
    // ball.display();
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
    glClearColor(0.1, 0.8, 0.6, 0.6);// alwan el background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    // renderer.addPlatform(map);
    renderer.setMap(map);
    renderer.addPlayer(ball);
    // renderer.display();
    glutDisplayFunc(displaykoko); 
    glutKeyboardFunc(keyboard);

 
    glutMainLoop();

    return 0;
}

