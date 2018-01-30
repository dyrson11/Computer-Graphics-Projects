#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/model.h"
#include "headers/program.h"

int winwidth, winheight;

void reshapefunc(int width,int height)
{
	winwidth=width;
	winheight=height;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
    glViewport(0,0,width,height);
}

void init (void)
{
    model obj;
    obj.read_model("res/models/bathtub/bathtub.obj");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBN);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * g_vertex_buffer_data.size(), g_vertex_buffer_data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBN);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(2);

    cout<<normals.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    program = LoadShaders("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
}

void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(VAO);
    glDrawElements( GL_POLYGON, indices.size(), GL_UNSIGNED_INT, nullptr);
    glutSwapBuffers();
}

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
    glutReshapeFunc(reshapefunc);
    glutIdleFunc(display);
	glutMainLoop();
}
