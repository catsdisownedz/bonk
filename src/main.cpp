#include <GL/freeglut.h>
#include <cmath>

#define PI 3.14159265358979323846

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f); // red

    glBegin(GL_TRIANGLE_FAN);

    // Bottom point of the heart
    glVertex2f(0.0f, -0.4f);

    // Left arc
    for (float angle = PI; angle >= 0; angle -= 0.1f) {
        float x = -0.5f * cos(angle) - 0.25f;
        float y = 0.5f * sin(angle);
        glVertex2f(x, y);
    }

    // Right arc
    for (float angle = 0.0f; angle <= PI; angle += 0.1f) {
        float x = 0.5f * cos(angle) + 0.25f;
        float y = 0.5f * sin(angle);
        glVertex2f(x, y);
    }

    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // white background
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("GLUT Heart 💖");

    init();

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
