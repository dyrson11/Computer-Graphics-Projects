// Para compilar en MacOSX
// g++ -o practica01 practica01.cpp -framework OpenGL -framework GLUT

#include <bits/stdc++.h>
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "Angel-yjc.h"
#include "GL/glut.h"
#endif
#define PI 3.141592
#define PI 3.141592
using namespace Angel;
using namespace std;

#define NUMVTX (8)

static int winwidth,winheight;
static int mx,my;
static int flag=0;
static float rotx=0.0f, roty=-70.0f;

float terrain[100][100];

vec3 cameraPos = vec3(0.0f, 0.5f, 0.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  600.0f / 2.0;
float lastY =  600.0 / 2.0;



inline double findnoise(int x)
{
	x = (x<<13) ^ x;
	return (double)( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589)) / 1073741824.0);
}

inline double findnoise2(double x,double y)
{
	int n=(int)x+(int)y*57;
	n=(n<<13)^n;
	int nn=(n*(n*n*60493+19990303)+1376312589);
	return 1.0-((double)nn/1073741824.0);
}

inline double interpolate1(double a,double b,double x)
{
	double ft=x * 3.1415927;
	double f=(1.0-cos(ft))* 0.5;
	return a*(1.0-f)+b*f;
}


double noise(double x,double y)
{
	double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
	double floory=(double)((int)y);
	double s,t,u,v;//Integer declaration
	s=findnoise2(floorx,floory);
	t=findnoise2(floorx+1,floory);
	u=findnoise2(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
	v=findnoise2(floorx+1,floory+1);
	double int1=interpolate1(s,t,x-floorx);//Interpolate between the values.
	double int2=interpolate1(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	return interpolate1(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}


void init()
{

	for (int y = 0; y < 50; y++)
	{
		for (int x = 0; x < 50; x++)
		{
			terrain[x][y] = noise((float)x/3, (float)y/3) / 10.0;
		}
		glEnd();
	}
}

void drawgraphix()
{
	int i;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();

	glTranslatef(0.0f,0.0f,-3.0f);

	glRotatef(rotx,0.0f,1.0f,0.0f);
	glRotatef(roty,1.0f,0.0f,0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(0.1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //cout<<cameraPos.y-50<<" "<<cameraPos.x-50<<endl;

	for (int y = cameraPos.y*3-50; y < cameraPos.y*3+49; y++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = cameraPos.x-50; x < cameraPos.x+49; x++)
		{
			glVertex3f((float)x, (float)y, noise((float)x/3, (float)y/3) / 5.0);
			glVertex3f((float)(x+1), (float)(y+1), noise((float)(x+1)/3, (float)(y+1)/3) / 5.0);
		}
		glEnd();
	}

	glPopMatrix();

	glutSwapBuffers();
}

void reshapefunc(int width,int height)
{
	winwidth=width;
	winheight=height;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,(float)width/height,1.0,50.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraPos.x + cameraFront.x, cameraPos.y + cameraFront.y, cameraPos.z + cameraFront.z, cameraUp.x, cameraUp.y, cameraUp.z);
}



void iddle()
{
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glutPostRedisplay();
}

void mousefunc(int button,int state,int x,int y)
{
	if (button==GLUT_LEFT_BUTTON) {
		if (state==GLUT_DOWN) {
			flag=1;
		} else {
			flag=0;
		}
	}
}

void motionfunc(int x,int y)
{
	/*if (flag>0) {
		if (flag>1) {
			rotx+=360.0f*(x-mx)/winwidth;
			roty+=360.0f*(y-my)/winheight;
		}

		mx=x;
		my=y;

		//drawgraphix();

		flag=2;
	}*/
    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xoffset = x - lastX;
    float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top
    lastX = x;
    lastY = y;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;


    vec3 front;
    front.x = cos(DegreesToRadians*yaw) * cos(DegreesToRadians*pitch);
    front.y = sin(DegreesToRadians*pitch);
    front.z = sin(DegreesToRadians*yaw) * cos(DegreesToRadians*pitch);
    cameraFront = normalize(front);
}
float cameraSpeed = 0;
void keyboardfunc(unsigned char key,int x,int y)
{
	if (key=='q' || key==27) exit(0);
    float cameraSpeed = 2.5f * deltaTime;
    switch (key)
    {
        case 'w': case 'W':
            cameraPos += 0.2 * cameraFront;
            break;
        case 'a': case 'A':
            cameraPos -= normalize(cross(cameraFront, cameraUp)) * 0.2;
            break;
        case 's': case 'S':
            cameraPos -= 0.2 * cameraFront;
            break;
	glColor3f(1.0f, 1.0f, 1.0f);
        case 'd': case 'D':
            cameraPos += normalize(cross(cameraFront, cameraUp)) * 0.2;
            break;
    }
    //std::cout<<cameraPos<<std::endl;
    //std::cout<<0.05 * cameraFront<<std::endl;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraPos.x + cameraFront.x, cameraPos.y + cameraFront.y, cameraPos.z + cameraFront.z, cameraUp.x, cameraUp.y, cameraUp.z);
    glutPostRedisplay();
}


void idlefunc()
{
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
	glutPostRedisplay();
}

int main(int argc,char **argv)
{
    winwidth=winheight=512;
	init();
    glutInit(&argc,argv);
    glutInitWindowSize(winwidth,winheight);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutCreateWindow("Practica 1 - Ejercicio 1.1");

    glutDisplayFunc(drawgraphix);
    glutReshapeFunc(reshapefunc);
    glutMouseFunc(mousefunc);
    glutMotionFunc(motionfunc);
    glutKeyboardFunc(keyboardfunc);
    glutIdleFunc(idlefunc);

    glutMainLoop();

    return(0);
}
