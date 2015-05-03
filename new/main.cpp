#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <sys/time.h>
#include <math.h>

#define SIZE 16

using namespace std;

struct index{
    int i, j; };


typedef struct {
    float x, y, z;
} point;

int flagRegenerate=1;
point mesh[SIZE+1][SIZE+1];
double angle=0;

void initialize();
void display();
void myReshape(int w,int h);
void keyboard(unsigned char key, int mousex, int mousey);
void drawMesh();
void generatePoints(float);
void setMaterial();
void diamondSquare(index,index,index,index,int);
void regenMesh(float);

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
    int width=400;

    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(100,100,100,0.0,0.0,0.0,0.0,1.0,0.0);

    glColor4f(253/255.0, 227/255.0, 167/255.0,0.5);
    //setMaterial();
    glRotated(angle,0.0,1.0,0.0);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-width,0.0,-width);
        glVertex3f(-width,0.0,width);
        glVertex3f(width,0.0,width);
        glVertex3f(width,0.0,-width);
    glEnd();

    if(flagRegenerate)
        regenMesh(width);
    drawMesh();

    //glutWireCube(100.0);

    glutSwapBuffers();

}

void regenMesh(float width)
{

    index a, b, c, d;
    generatePoints(width);

    a.i=0, a.j=0;
    b.i=0, b.j=SIZE;
    c.i=SIZE, c.j=SIZE;
    d.i=SIZE, d.j=0;

    mesh[a.i][a.j].y=60;
    mesh[b.i][b.j].y=40;
    mesh[c.i][c.j].y=55;
    mesh[d.i][d.j].y=40;

    diamondSquare(a,b,c,d,SIZE);
    flagRegenerate=0;
}

void initialize()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_NORMALIZE);
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
    if(key=='D' || key=='d')
        angle+=5;
    else if(key=='A' || key =='a')
        angle-=5;
    else if(key=='q')
        exit(0);
    else if(key=='R' || key=='r')
        flagRegenerate=1;
    if(angle>360)
        angle-=360;
   glutPostRedisplay();
}

void drawMesh()
{
    int i=0, j=0;
    for(i=0; i<SIZE; i++)
    {
        for(j=0; j<SIZE; j++)
        {


            glColor3f(239/255.0,72.0/255.0,54.0/255.0);
            glBegin(GL_QUADS);
                glVertex3f(mesh[i][j+1].x,mesh[i][j+1].y,mesh[i][j+1].z);
                glVertex3f(mesh[i+1][j+1].x,mesh[i+1][j+1].y,mesh[i+1][j+1].z);
                glVertex3f(mesh[i+1][j].x,mesh[i+1][j].y,mesh[i+1][j].z);
                glVertex3f(mesh[i][j].x,mesh[i][j].y,mesh[i][j].z);
                 glFlush();
            glEnd();

            glColor3f(0.0/255.0,0.0/255.0,0.0/255.0);
            glBegin(GL_LINE_LOOP);
                glVertex3f(mesh[i][j+1].x,mesh[i][j+1].y+1,mesh[i][j+1].z);
                glVertex3f(mesh[i+1][j+1].x,mesh[i+1][j+1].y+1,mesh[i+1][j+1].z);
                glVertex3f(mesh[i+1][j].x,mesh[i+1][j].y+1,mesh[i+1][j].z);
                glVertex3f(mesh[i][j].x,mesh[i][j].y+1,mesh[i][j].z);
                 glFlush();
            glEnd();


        }
    }
}

void generatePoints(float width)
{
    int i=0, j=0;
    float zVal=-width, xVal=-width, incrementVal=(2.0*width)/SIZE;
    time_t t;
    int blah;
    for(i=0; i<(SIZE+1); i++)
    {
        xVal=-width;
        for(j=0; j<(SIZE+1); j++)
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

        mesh[abMid.i][abMid.j].y = (mesh[a.i][a.j].y+mesh[b.i][b.j].y)/2.0 + rand()%30;
        mesh[bcMid.i][bcMid.j].y = (mesh[b.i][b.j].y+mesh[c.i][c.j].y)/2.0 + rand()%30;
        mesh[cdMid.i][cdMid.j].y = (mesh[c.i][c.j].y+mesh[d.i][d.j].y)/2.0 + rand()%30;
        mesh[daMid.i][daMid.j].y = (mesh[d.i][d.j].y+mesh[a.i][a.j].y)/2.0 + rand()%30;

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

void setMaterial()
{
    float mat_ambient[] = {0.7,0.7,0.7,1.0};
    float mat_diffuse[] = {0.5,0.0,0.0,1.0};
    float mat_specular[] = {1.0,1.0,1.0};
    float mat_shininess[] = {50.0};

    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);

    float lightintensity[] = {1.0,1.0,1.0,1.0};
    float light_position[] = {0.0,0,600.0,0.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightintensity);
}
