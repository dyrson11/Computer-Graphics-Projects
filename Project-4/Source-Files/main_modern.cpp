#define GLEW_STATIC
#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "Angel-yjc.h"
using namespace Angel;
using namespace std;

#include "../headers/variables.h"
#include "../headers/perlin_noise.h"
#include "../headers/shader.h"
/*#include "headers/genCube.h"
#include "headers/genLetter.h"*/

void reshapefunc(int width,int height)
{
	winwidth=width;
	winheight=height;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
    //glEnable(GL_LIGHTING);
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,(float)width/height,1.0,50.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraPos.x + cameraFront.x, cameraPos.y + cameraFront.y, cameraPos.z + cameraFront.z, cameraUp.x, cameraUp.y, cameraUp.z);
}
void genTerrain()
{
	terrain.clear();
    for (int y = cameraPos.y*3-50; y < cameraPos.y*3+49; y++)
	{
		for (int x = cameraPos.x-50; x < cameraPos.x+49; x++)
		{
			terrain.push_back(vec2((float)x/50, (float)y/50));
			terrain.push_back(vec2((float)(x+1)/50, (float)(y+1)/50));
			/*terrain.push_back((float)x);
            terrain.push_back((float)y); //noise((float)x/3, (float)y/3) / 5.0
            terrain.push_back((float)(x+1));
            terrain.push_back((float)(y+1)); //noise((float)(x+1)/3, (float)(y+1)/3) / 5.0*/
		}
	}
}
void init (void)
{
	glClearColor (0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	program = LoadShaders("../Shaders/color_vShader.glsl", "../Shaders/color_fShader.glsl");
	glUseProgram(program);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    //glGenBuffers(1, &VBN);

    glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * terrain.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
                    sizeof(vec2) * terrain.size(), terrain.data());
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glBindVertexArray(0);

    /*glBindBuffer(GL_ARRAY_BUFFER, VBN);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(2);

    cout<<normals.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);


    glUniform3f(glGetUniformLocation(program, "objectColor"), 0.5f, 0.3f, 1.0f);
    glUniform3f(glGetUniformLocation(program, "lightColor"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(program, "lightPos"), 4.0f, 4.0f, 6.0f);
    glUniform3f(glGetUniformLocation(program, "viewPos"), -2.0f, -2.0f, -3.0f);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);*/

}

void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	genTerrain();
	glUseProgram(program);
    glBindVertexArray(VAO);
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
			  BUFFER_OFFSET(0) );

    glDrawArrays( GL_TRIANGLE_STRIP, 0, terrain.size()/2);

	glDisableVertexAttribArray(vPosition);
    glutSwapBuffers();
	cout<<terrain[0]<<endl;
}

void mousefunc(int button,int state,int x,int y)
{
}

void motionfunc(int x,int y)
{
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

/*Main*/
int main (int argc,char** argv)
{
    winwidth=winheight=512;

    glutInit(&argc,argv);
    glutInitWindowSize(winwidth,winheight);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutCreateWindow("Practica 1 - Ejercicio 1.1");

    if ( glewInit() != GLEW_OK )
    {
        return -1;
    }

    init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardfunc);
    glutReshapeFunc(reshapefunc);
    glutMouseFunc(mousefunc);
    glutMotionFunc(motionfunc);
    glutIdleFunc(display);
	glutMainLoop();
}
