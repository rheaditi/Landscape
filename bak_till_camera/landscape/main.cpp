#include <iostream>
#include<GL/glut.h>

using namespace std;

void initialize();
void display();
void myReshape(int w,int h);
void keyboard(unsigned char key, int mousex, int mousey);
void drawMesh();

GLdouble viewerEye[3]={0.0,200.0,200.0}, viewerAt[3]={0.0,20.0,80.0}, viewerUp[3]={0.0,0.0,20.0};

int main(int argc, char ** argv)
{

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(10,10);
    glutInitWindowSize(600,600);
    glutCreateWindow("Landscape!");
    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(keyboard);
    initialize();
    glutMainLoop();
    return 0;
}

void display()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(viewerEye[0],viewerEye[1],viewerEye[2],viewerAt[0],viewerAt[1],viewerAt[2],viewerUp[0],viewerUp[1],viewerUp[2]);

    glTranslated(0.0,-100.0,0.0);
    glColor3f(1.0,0.0,0.0);
    glutWireCube(100.0);

    glutSwapBuffers();

}

void initialize()
{
    glEnable(GL_DEPTH_TEST);

}

void myReshape(int w,int h)
{
    float winsize = w < h ? w : h;
    glViewport(0,0,winsize,winsize);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-700.0,700.0,-700.0,700.0,-700.0,700.0);
    //glFrustum(-20.0,20.0,-20.0,20.0,-70.0,70.0);
    glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int mousex, int mousey)
{
    switch(key)
    {
    case 'w':
    case 'W':
        viewerEye[2]+=2;
        break;
    case 's':
    case 'S':
        viewerEye[2]-=2;
        break;
    case 'a':
    case 'A':
        viewerEye[0]+=2;
        break;
    case 'd':
    case 'D':
        viewerEye[0]-=2;
        break;
    case 'q':
    case 'Q':
        viewerEye[1]+=2;
        break;
    case 'e':
    case 'E':
        viewerEye[1]-=2;
        break;
    }
    glutPostRedisplay();
}

void drawMesh()
{

}
