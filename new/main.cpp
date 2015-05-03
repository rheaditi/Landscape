/*
    Terrain Generator Using Diamond Square Algorithm

    by:
    Aditi Anomita Mohanty
    Abinav Nithya Seelan

*/


#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <sys/time.h>
#include <math.h>

//SIZE defines the mesh value and the iterator value
#define SIZE 16

using namespace std;

//Index is used to hold the i-th row and j-th column values
struct index
{
    int i, j;
};

//Structure to hold point position values
struct point{
    float x, y, z;
};

//Flag value to check if the terrain has to be regenerated or not
int flagRegenerate=1;

point mesh[SIZE+1][SIZE+1];

//Angle value to rotate the terrain
double angle=0;

//Initialization Function
void initialize();
//Display Function
void display();
//Reshape Function
void myReshape(int w,int h);
//Function to handle keyboard events
void keyboard(unsigned char key, int mousex, int mousey);
//Function to Draw the mesh
void drawMesh();
//Function to set x and z values to points of the mesh
void generatePoints(float);
//Function that performs the Diamond-Square Algorithm
void diamondSquare(index,index,index,index,int);
//Function to regenerate the mesh
void regenMesh(float);

int main(int argc, char ** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(10,10);
    glutInitWindowSize(600,600);
    glutCreateWindow("Landscape Generator");
    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(keyboard);
    initialize();
    glutMainLoop();
    return 0;
}

void display()
{
    int width=400;

    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //Setting the camera location.
    gluLookAt(100,100,100,0.0,0.0,0.0,0.0,1.0,0.0);

    glColor4f(253/255.0, 227/255.0, 167/255.0,0.5);

    glRotated(angle,0.0,1.0,0.0); //To rotate the terrain based on angle value.

    glBegin(GL_LINE_LOOP);
        glVertex3f(-width,0.0,-width);
        glVertex3f(-width,0.0,width);
        glVertex3f(width,0.0,width);
        glVertex3f(width,0.0,-width);
    glEnd();

    if(flagRegenerate)
    {
        regenMesh(width);   //If flag==1, then regenerate the terrain. It is set to 0 or 1 inside keyboard()
    }

    drawMesh();
    glutSwapBuffers();

}

void regenMesh(float width)
{

    index a, b, c, d; //To hold the four corner points
    generatePoints(width);

    a.i=0, a.j=0;
    b.i=0, b.j=SIZE;
    c.i=SIZE, c.j=SIZE;
    d.i=SIZE, d.j=0;

    //Setting the initial heights for the corner points
    mesh[a.i][a.j].y=60;
    mesh[b.i][b.j].y=40;
    mesh[c.i][c.j].y=55;
    mesh[d.i][d.j].y=40;

    diamondSquare(a,b,c,d,SIZE);

    //Resetting the flag value to 0
    flagRegenerate=0;
}

void initialize()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
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
    if(key=='D' || key=='d')    //On pressing 'D', the rotation is anti-clockwise
        angle+=5;
    else if(key=='A' || key =='a')  //On pressing 'A', the rotation is clock-wise
        angle-=5;
    else if(key=='q')   //On pressing 'Q', the program will quit.
        exit(0);
    else if(key=='R' || key=='r')   //On pressing 'R', the rotation flag is set to 1.
        flagRegenerate=1;
    if(angle>360)   //Reset angle to 0 if it exceeds 360
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
            //Draw the polygons for each individual cell of the mesh
            glColor3f(239/255.0,72.0/255.0,54.0/255.0);
            glBegin(GL_QUADS);
                glVertex3f(mesh[i][j+1].x,mesh[i][j+1].y,mesh[i][j+1].z);
                glVertex3f(mesh[i+1][j+1].x,mesh[i+1][j+1].y,mesh[i+1][j+1].z);
                glVertex3f(mesh[i+1][j].x,mesh[i+1][j].y,mesh[i+1][j].z);
                glVertex3f(mesh[i][j].x,mesh[i][j].y,mesh[i][j].z);
                 glFlush();
            glEnd();

            //Draw the border for the cells. Otherwise you wont be able to distinguish between cells.
            //Using mesh[i][j].y+1 to prominently show the lines above the mesh.
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

    for(i=0; i<(SIZE+1); i++)
    {
        xVal=-width;    //Resetting xVal to start from left edge every time.
        for(j=0; j<(SIZE+1); j++)
        {
            mesh[i][j].x=xVal;
            mesh[i][j].y=0.0;
            mesh[i][j].z=zVal;
            xVal+=incrementVal; //Incrementing xVal to move across the columns of the mesh.

        }
        zVal+=incrementVal; //Incrementing zVal to move down the rows of the mesh.
    }
}

void diamondSquare(index a, index b, index c, index d, int iteration)
{
    if(iteration!=1) //If Iteration==1, The Algorithm must quit.
    {
        //Objects to hold the midpoint values.
        index abMid, bcMid, cdMid, daMid , diamondCenter;

        //Midpoints to find the diamond.
        abMid.i = a.i;
        abMid.j = (a.j+b.j)/2;

        bcMid.i = (b.i+c.i)/2;
        bcMid.j = b.j;

        cdMid.i = c.i;
        cdMid.j = (c.j+d.j)/2;

        daMid.i = (a.i+d.i)/2;
        daMid.j = d.j;

        //Seed Randomization function
        //Using time(NULL) to get the current system time.
        //This is to prevent same random value from appearing
        srand(time(NULL));

        //Setting a random height for the midpoints based on avg value of end vertex heights and a random noise.
        mesh[abMid.i][abMid.j].y = (mesh[a.i][a.j].y+mesh[b.i][b.j].y)/2.0 + rand()%30;
        mesh[bcMid.i][bcMid.j].y = (mesh[b.i][b.j].y+mesh[c.i][c.j].y)/2.0 + rand()%30;
        mesh[cdMid.i][cdMid.j].y = (mesh[c.i][c.j].y+mesh[d.i][d.j].y)/2.0 + rand()%30;
        mesh[daMid.i][daMid.j].y = (mesh[d.i][d.j].y+mesh[a.i][a.j].y)/2.0 + rand()%30;

        //Find The Center Of The Diamond
        diamondCenter.i = daMid.i;
        diamondCenter.j = abMid.j;

        srand(time(NULL));
        //Setting a random height for the diamond center
        mesh[diamondCenter.i][diamondCenter.j].y = (mesh[abMid.i][abMid.j].y + mesh[bcMid.i][bcMid.j].y + mesh[cdMid.i][cdMid.j].y + mesh[daMid.i][daMid.j].y)/4.0 - rand()%10;

        //Recursively calling the Diamond-Square Algorithm for the smaller squares.
        diamondSquare(a,abMid,diamondCenter,daMid,iteration/2);
        diamondSquare(abMid,b,bcMid,diamondCenter,iteration/2);
        diamondSquare(diamondCenter,bcMid,c,cdMid,iteration/2);
        diamondSquare(daMid,diamondCenter,cdMid,d,iteration/2);

    }
}


