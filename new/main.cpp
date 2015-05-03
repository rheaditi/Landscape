#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <sys/time.h>
#include <math.h>

using namespace std;

void initialize();
void display();
void myReshape(int w,int h);
void keyboard(unsigned char key, int mousex, int mousey);
void drawMesh();
void generatePoints(float);

struct index{
    int i, j; };

void diamondSquare(index,index,index,index,int);

typedef struct {
    float x, y, z;
} point;



point mesh[31][31];

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
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glutMainLoop();
    return 0;
}

void display()
{
    int width=300;
    index a, b, c, d;

    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(100,100,100,0.0,0.0,0.0,0.0,1.0,0.0);
    //glRotated(45,0.0,1.0,0.0);
    //glRotated(45,1.0,0.0,0.0);



    glColor4f(253/255.0, 227/255.0, 167/255.0,0.5);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-width,0.0,-width);
        glVertex3f(-width,0.0,width);
        glVertex3f(width,0.0,width);
        glVertex3f(width,0.0,-width);
    glEnd();

    glColor3f(239/255.0,72.0/255.0,54.0/255.0);
    generatePoints(300.0);

    a.i=0, a.j=0;
    b.i=0, b.j=30;
    c.i=30, c.j=30;
    d.i=30, d.j=0;

    mesh[a.i][a.j].y=40;
    mesh[b.i][b.j].y=80;
    mesh[c.i][c.j].y=160;
    mesh[d.i][d.j].y=320;

    diamondSquare(a,b,c,d,32);
    drawMesh();

    //glutWireCube(100.0);

    glutSwapBuffers();

}

void initialize()
{
    glEnable(GL_DEPTH_TEST);
    //glShadeModel(GL_FLAT);
}

void myReshape(int w,int h)
{
    float winsize = w < h ? w : h;
    glViewport(0,0,winsize,winsize);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-600.0,600.0,-600.0,600.0,-600.0,600.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int mousex, int mousey)
{
   glutPostRedisplay();
}

void drawMesh()
{
    int i=0, j=0;
    for(i=0; i<30; i++)
    {
        for(j=0; j<30; j++)
        {
            glBegin(GL_LINE_LOOP);
                glVertex3f(mesh[i][j+1].x,mesh[i][j+1].y,mesh[i][j+1].z);
                glVertex3f(mesh[i+1][j+1].x,mesh[i+1][j+1].y,mesh[i+1][j+1].z);
                glVertex3f(mesh[i+1][j].x,mesh[i+1][j].y,mesh[i+1][j].z);
                glVertex3f(mesh[i][j].x,mesh[i][j].y,mesh[i][j].z);
                 glFlush();
            glEnd();
        }
    }
}

void generatePoints(float width)
{
    int i=0, j=0;
    float zVal=-width, xVal=-width, incrementVal=(2.0*width)/30;
    time_t t;
    int blah;
    for(i=0; i<31; i++)
    {
        xVal=-width;
        for(j=0; j<31; j++)
        {
            srand((unsigned)time(&t));
            mesh[i][j].x=xVal;
            mesh[i][j].y=0.0;
            mesh[i][j].z=zVal;
            xVal+=incrementVal;

        }
        zVal+=incrementVal;
    }
}

void diamondSquare(index a, index b, index c, index d, int iteration)
{
    if(iteration!=1)
    {
        index abMid, bcMid, cdMid, daMid , diamondCenter;

        //midpoints to find diamond
        abMid.i = a.i;
        abMid.j = (a.j+b.j)/2;

        bcMid.i = (b.i+c.i)/2;
        bcMid.j = b.j;

        cdMid.i = c.i;
        cdMid.j = (c.j+d.j)/2;

        daMid.i = (a.i+d.i)/2;
        daMid.j = d.j;

        srand(time(NULL));

        mesh[abMid.i][abMid.j].y = (mesh[a.i][a.j].y+mesh[b.i][b.j].y)/2.0 - rand()%20;
        mesh[bcMid.i][bcMid.j].y = (mesh[b.i][b.j].y+mesh[c.i][c.j].y)/2.0 - rand()%20;
        mesh[cdMid.i][cdMid.j].y = (mesh[c.i][c.j].y+mesh[d.i][d.j].y)/2.0 - rand()%20;
        mesh[daMid.i][daMid.j].y = (mesh[d.i][d.j].y+mesh[a.i][a.j].y)/2.0 - rand()%20;

        //Find The Center Of The Diamond

        diamondCenter.i = daMid.i;
        diamondCenter.j = abMid.j;

        srand(time(NULL));

        mesh[diamondCenter.i][diamondCenter.j].y = (mesh[abMid.i][abMid.j].y + mesh[bcMid.i][bcMid.j].y + mesh[cdMid.i][cdMid.j].y + mesh[daMid.i][daMid.j].y)/4.0 - rand()%10;

        diamondSquare(a,abMid,diamondCenter,daMid,iteration/2);
        diamondSquare(abMid,b,bcMid,diamondCenter,iteration/2);
        diamondSquare(diamondCenter,bcMid,c,cdMid,iteration/2);
        diamondSquare(daMid,diamondCenter,cdMid,d,iteration/2);

    }
}
