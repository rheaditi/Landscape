#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <sys/time.h>
#include <math.h>

#define SIZE 16
#define LOW 75
#define MEDIUM 100
#define WIDTH 400

using namespace std;

struct index{
    int i, j; };


typedef struct {
    float x, y, z;
    float pointColor[3];
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
void diamondSquare(index,index,index,index,int);
void regenMesh(float);
void setColor(index);
float getRandomF(int mod);

int main(int argc, char ** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(10,10);
    glutInitWindowSize(600,600);
    glutCreateWindow("Landscape!");

    //callback registration
    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(keyboard);

    initialize();

    //begin
    glutMainLoop();
    return 0;
}

void display()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(100,100,100,0.0,0.0,0.0,0.0,1.0,0.0);
    glColor4f(253/255.0, 227/255.0, 167/255.0,0.5);

    glRotated(angle,0.0,1.0,0.0);

    //outer boundary

    glBegin(GL_LINE_LOOP);
        glVertex3f(-WIDTH,0.0,-WIDTH);
        glVertex3f(-WIDTH,0.0,WIDTH);
        glVertex3f(WIDTH,0.0,WIDTH);
        glVertex3f(WIDTH,0.0,-WIDTH);
    glEnd();

    if(flagRegenerate)
        regenMesh(WIDTH);
    drawMesh();

    //to flush
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

    mesh[a.i][a.j].y=60;    setColor(a);

    mesh[b.i][b.j].y=40;    setColor(b);

    mesh[c.i][c.j].y=55;    setColor(c);

    mesh[d.i][d.j].y=40;    setColor(d);

    diamondSquare(a,b,c,d,SIZE);
    flagRegenerate=0;
}

void setColor(index p)
{
    if(mesh[p.i][p.j].y<LOW)//green
    {
        mesh[p.i][p.j].pointColor[0] = 99/255.0;
        mesh[p.i][p.j].pointColor[1] = 186/255.0;
        mesh[p.i][p.j].pointColor[2] = 86/255.0;
    }
    else if(mesh[p.i][p.j].y>=LOW && mesh[p.i][p.j].y<MEDIUM)//brown
    {
        mesh[p.i][p.j].pointColor[0] = 185/255.0;
        mesh[p.i][p.j].pointColor[1] = 122/255.0;
        mesh[p.i][p.j].pointColor[2] = 87/255.0;
    }
    else//white
    {
        mesh[p.i][p.j].pointColor[0] = 1.0;
        mesh[p.i][p.j].pointColor[1] = 1.0;
        mesh[p.i][p.j].pointColor[2] = 1.0;
    }
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
    index holder;
    for(i=0; i<SIZE; i++)
    {
        for(j=0; j<SIZE; j++)
        {
            glBegin(GL_POLYGON);

                holder.i=i; holder.j=j+1;
                setColor(holder);
                glColor3fv(mesh[i][j+1].pointColor);
                glVertex3f(mesh[i][j+1].x,mesh[i][j+1].y,mesh[i][j+1].z);

                holder.i=i+1; holder.j=j+1;
                setColor(holder);
                glColor3fv(mesh[i+1][j+1].pointColor);
                glVertex3f(mesh[i+1][j+1].x,mesh[i+1][j+1].y,mesh[i+1][j+1].z);

                holder.i=i+1; holder.j=j;
                setColor(holder);
                glColor3fv(mesh[i+1][j].pointColor);
                glVertex3f(mesh[i+1][j].x,mesh[i+1][j].y,mesh[i+1][j].z);

                holder.i=i; holder.j=j;
                setColor(holder);
                glColor3fv(mesh[i][j].pointColor);
                glVertex3f(mesh[i][j].x,mesh[i][j].y,mesh[i][j].z);
                glFlush();
            glEnd();

            glColor3f(0.0/255.0,0.0/255.0,0.0/255.0);
            glBegin(GL_LINE_LOOP);
                glVertex3f(mesh[i][j+1].x,mesh[i][j+1].y+1,mesh[i][j+1].z);
                glVertex3f(mesh[i+1][j+1].x,mesh[i+1][j+1].y+1,mesh[i+1][j+1].z);
                glVertex3f(mesh[i+1][j].x,mesh[i+1][j].y+1,mesh[i+1][j].z);
                glVertex3f(mesh[i][j].x,mesh[i][j].y+1,mesh[i][j].z);
            glEnd();
        }
    }
}

void generatePoints(float width)
{
    int i=0, j=0;
    float zVal=-width, xVal=-width, incrementVal=(2.0*width)/SIZE;

    for(i=0; i<(SIZE+1); i++)
    {
        xVal=-width;
        for(j=0; j<(SIZE+1); j++)
        {
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

        mesh[abMid.i][abMid.j].y = (mesh[a.i][a.j].y+mesh[b.i][b.j].y)/2.0 + getRandomF(30);
        mesh[bcMid.i][bcMid.j].y = (mesh[b.i][b.j].y+mesh[c.i][c.j].y)/2.0 + getRandomF(30);
        mesh[cdMid.i][cdMid.j].y = (mesh[c.i][c.j].y+mesh[d.i][d.j].y)/2.0 + getRandomF(30);
        mesh[daMid.i][daMid.j].y = (mesh[d.i][d.j].y+mesh[a.i][a.j].y)/2.0 + getRandomF(30);

        //Find The Center Of The Diamond
        diamondCenter.i = daMid.i;
        diamondCenter.j = abMid.j;

        srand(time(NULL));
        cout<<"Random: "<<(getRandomF(100))<<endl;
        mesh[diamondCenter.i][diamondCenter.j].y = (mesh[abMid.i][abMid.j].y + mesh[bcMid.i][bcMid.j].y + mesh[cdMid.i][cdMid.j].y + mesh[daMid.i][daMid.j].y)/4.0 - rand()%30;

        diamondSquare(a,abMid,diamondCenter,daMid,iteration/2);
        diamondSquare(abMid,b,bcMid,diamondCenter,iteration/2);
        diamondSquare(diamondCenter,bcMid,c,cdMid,iteration/2);
        diamondSquare(daMid,diamondCenter,cdMid,d,iteration/2);

    }
}

float getRandomF(int mod)
{
    timeval t;
    gettimeofday(&t,NULL);
    return ( (t.tv_usec) % mod );
}
