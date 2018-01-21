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
	texCoord.clear();
	float textureU = float(100)*0.1f;
	float textureV = float(100)*0.1f;
	for(float r = (cameraPos.x)-15; r < (cameraPos.x)+15; r+=0.3)
	{
        for(float c = (cameraPos.z)-15; c < (cameraPos.z)+15; c+=0.3)
		{
			float scaleR = float(r)/float(100-1);
			float scaleC = float(c)/float(100-1);
			terrain.push_back(r);
			terrain.push_back(c);
			texCoord.push_back(vec2(textureU * scaleC, textureV * scaleR));
        }
    }
}

void genTerrain2()
{
	terrain2.clear();
	for(int r = cameraPos.x-20; r < cameraPos.x+20; r++)
	{
        for(int c = cameraPos.z-20; c < cameraPos.z+20; c++)
		{
			if(r < cameraPos.x - 10 || r >= cameraPos.x + 10 || c < cameraPos.z - 10 || c >= cameraPos.z + 10)
			{
				terrain2.push_back((float)r);
				terrain2.push_back((float)c);
			}

        }
    }
}

void genIndices()
{
	indices.clear();

	int rows = sqrt(terrain.size()/2);
	int columns = rows;
    for (int r = 0; r < rows - 1; r++)
	{
        for(int c = 0; c <= columns; c++)
		{
            if(r % 2 == 0)
			{
                if(c == columns){
                    indices.push_back((c-1) + (r+1)*columns);
                    indices.push_back((c-1) + (r+1)*columns);
                }else{
                    indices.push_back(c + r * columns);
                    indices.push_back(c + (r+1) * columns);
                }
            }
			else
			{
                if(c == columns)
				{
                    indices.push_back((columns) - c + (r +1) * columns);
                    indices.push_back((columns) - c + (r +1) * columns);
                }else{
                    indices.push_back((columns - 1) - c + (r + 1) * columns);
                    indices.push_back(r*columns + (columns-1) - c);
                }
            }

        }
    }
}

void genIndices2()
{
	indices2.clear();

	int rows = sqrt(terrain.size()/2);
	int columns = rows;
    for (int r = 0; r < rows - 1; r++)
	{
        for(int c = 0; c <= columns; c++)
		{
            if(r % 2 == 0)
			{
                if(c == columns){
                    indices2.push_back((c-1) + (r+1)*columns);
                    indices2.push_back((c-1) + (r+1)*columns);
                }else{
                    indices2.push_back(c + r * columns);
                    indices2.push_back(c + (r+1) * columns);
                }
            }
			else
			{
                if(c == columns)
				{
                    indices2.push_back((columns) - c + (r +1) * columns);
                    indices2.push_back((columns) - c + (r +1) * columns);
                }else{
                    indices2.push_back((columns - 1) - c + (r + 1) * columns);
                    indices2.push_back(r*columns + (columns-1) - c);
                }
            }

        }
    }
}


void init (void)
{
	glClearColor (0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	genTerrain();
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
/*
    glUniform3f(glGetUniformLocation(program, "objectColor"), 0.5f, 0.3f, 1.0f);
    glUniform3f(glGetUniformLocation(program, "lightColor"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(program, "lightPos"), 4.0f, 4.0f, 6.0f);
    glUniform3f(glGetUniformLocation(program, "viewPos"), -2.0f, -2.0f, -3.0f);
*/
  	//glBindVertexArray(0);
	program = LoadShaders("../Shaders/color_vShader.glsl", "../Shaders/color_fShader.glsl");
	glUseProgram(program);
}

void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	genTerrain();
	genIndices();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * terrain.size() + sizeof(vec2) * texCoord.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * terrain.size(), terrain.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * terrain.size(), sizeof(vec2) * texCoord.size(), texCoord.data());

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float) * terrain.size()));


	//cout<<cameraPos<<endl;
    glDrawElements( GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, indices.data());

	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vTexCoord);
	glutSwapBuffers();
	/*genTerrain2();
	genIndices2();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * terrain2.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * terrain2.size(), terrain2.data());
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	//cout<<cameraPos<<endl;
    glDrawElements( GL_TRIANGLE_STRIP, indices2.size(), GL_UNSIGNED_INT, indices2.data());
	glDisableVertexAttribArray(vPosition);
    */
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
