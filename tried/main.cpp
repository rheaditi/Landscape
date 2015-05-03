
#include <iostream>
#include <GL/glut.h>

using namespace std;

void initialize();
void display();
void drawMesh();
void generatePoints();
void myReshape(int w,int h);

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(600,600);
    glutCreateWindow("Mesh Woah!");
    glEnable(GL_DEPTH_TEST);
    initialize();
    //glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutMainLoop();
    return 0;
}

void myReshape(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,260.0,0.0,260.0,0.0,260.0);
    glMatrixMode(GL_MODELVIEW);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);
        glVertex3f(10,0,10);
        glVertex3f(200,0,10);
        glVertex3f(200,0,200);
        glVertex3f(10,0,200);
    glEnd();
    glutSwapBuffers();

    //generatePoints();
    //drawMesh();

    glFlush();
}

void initialize()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glPointSize(2.0);
}
