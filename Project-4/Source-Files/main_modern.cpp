#define GLEW_STATIC
#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "Angel-yjc.h"
using namespace Angel;
using namespace std;
#define STB_IMAGE_IMPLEMENTATION

#include "../headers/variables.h"
#include "../headers/perlin_noise.h"
#include "../headers/shader.h"
#include "../headers/stb_image.h"

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
	gluPerspective(60.0,(float)width/height,1.0,100.0);

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

void genTerrain2(float x0, float y0, float x1, float y1)
{
	terrain2.clear();
	texCoord2.clear();
	float textureV = (x1 - x0)*0.1f;
	float textureU = (y1 - y0)*0.1f;
	for(float r = x0; r < x1; r++)
	{
		for(float c = y0; c < y1; c++)
		{
			float scaleR = r/(x1 - x0 - 1);
			float scaleC = c/(y1 - y0 - 1);
			terrain2.push_back(r);
			terrain2.push_back(c);
			texCoord2.push_back(vec2(textureU * scaleC, textureV * scaleR));
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

void genIndices2(int columns, int rows)
{
	indices2.clear();
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

void genSkyboxData()
{
	vSkyBoxVertices.clear();
	vSkyBoxTexCoords.clear();

	vSkyBoxVertices =
	{
		// Front face
		vec3(cameraPos.x+50.0f, cameraPos.y+50.0f, cameraPos.z+50.0f), vec3(cameraPos.x+50.0f, -10, cameraPos.z+50.0f), vec3(cameraPos.x-50.0f, cameraPos.y+50.0f, cameraPos.z+50.0f), vec3(cameraPos.x-50.0f, -10, cameraPos.z+50.0f),
		// Back face
		vec3(cameraPos.x-50.0f, cameraPos.y+50.0f, cameraPos.z-50.0f), vec3(cameraPos.x-50.0f, -10, cameraPos.z-50.0f), vec3(cameraPos.x+50.0f, cameraPos.y+50.0f, cameraPos.z-50.0f), vec3(cameraPos.x+50.0f, -10, cameraPos.z-50.0f),
		// Left face
		vec3(cameraPos.x-50.0f, cameraPos.y+50.0f, cameraPos.z+50.0f), vec3(cameraPos.x-50.0f, -10, cameraPos.z+50.0f), vec3(cameraPos.x-50.0f, cameraPos.y+50.0f, cameraPos.z-50.0f), vec3(cameraPos.x-50.0f, -10, cameraPos.z-50.0f),
		// Right face
		vec3(cameraPos.x+50.0f, cameraPos.y+50.0f, cameraPos.z-50.0f), vec3(cameraPos.x+50.0f, -10, cameraPos.z-50.0f), vec3(cameraPos.x+50.0f, cameraPos.y+50.0f, cameraPos.z+50.0f), vec3(cameraPos.x+50.0f, -10, cameraPos.z+50.0f),
		// Top face
		vec3(cameraPos.x-50.0f, cameraPos.y+50.0f, cameraPos.z-50.0f), vec3(cameraPos.x+50.0f, cameraPos.y+50.0f, cameraPos.z-50.0f), vec3(cameraPos.x-50.0f, cameraPos.y+50.0f, cameraPos.z+50.0f), vec3(cameraPos.x+50.0f, cameraPos.y+50.0f, cameraPos.z+50.0f),
		// Bottom face
		vec3(cameraPos.x+50.0f, -10, cameraPos.z-50.0f), vec3(cameraPos.x-50.0f, -10, cameraPos.z-50.0f), vec3(cameraPos.x+50.0f, -10, cameraPos.z+50.0f), vec3(cameraPos.x-50.0f, -10, cameraPos.z+50.0f),
	};


	vec2 vSkyBoxTmp[] =
	{
		vec2(0.0f, 1.0f), vec2(0.0f, 0.0f), vec2(1.0f, 1.0f), vec2(1.0f, 0.0f)
	};
	for(int i = 0; i < 24; i++)
		vSkyBoxTexCoords.push_back(vSkyBoxTmp[i%4]);

}

void init (void)
{
	glClearColor (0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	genTerrain();
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("../Textures/grass.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	//cout<<"Af texture image2D\n";
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("../Textures/grass2.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("../Textures/grass&rocks.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("../Textures/rocks.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("../Textures/snow.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &textureFront);
	glBindTexture(GL_TEXTURE_2D, textureFront);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	data = stbi_load("../Textures/env/thunder_ft.tga", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &textureBack);
	glBindTexture(GL_TEXTURE_2D, textureBack);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	data = stbi_load("../Textures/env/thunder_bk.tga", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &textureLeft);
	glBindTexture(GL_TEXTURE_2D, textureLeft);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	data = stbi_load("../Textures/env/thunder_lf.tga", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &textureRight);
	glBindTexture(GL_TEXTURE_2D, textureRight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	data = stbi_load("../Textures/env/thunder_rt.tga", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &textureUp);
	glBindTexture(GL_TEXTURE_2D, textureUp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	data = stbi_load("../Textures/env/thunder_up.tga", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &textureDown);
	glBindTexture(GL_TEXTURE_2D, textureDown);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	data = stbi_load("../Textures/env/thunder_dn.tga", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenBuffers(1, &VBO_Skybox);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Skybox);

	skyProgram = LoadShaders("../Shaders/skybox_vShader.glsl", "../Shaders/skybox_fShader.glsl");
	glUseProgram(skyProgram);
	glUniform1i(glGetUniformLocation(skyProgram, "textureBox"), 8);

	glUseProgram(0);
	program = LoadShaders("../Shaders/color_vShader.glsl", "../Shaders/color_fShader.glsl");
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "texture1"), 0);
	glUniform1i(glGetUniformLocation(program, "texture2"), 1);
	glUniform1i(glGetUniformLocation(program, "texture3"), 2);
	glUniform1i(glGetUniformLocation(program, "texture4"), 3);
	glUniform1i(glGetUniformLocation(program, "texture5"), 4);
}

void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(skyProgram);
	genSkyboxData();

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Skybox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vSkyBoxVertices.size() + sizeof(vec2) * vSkyBoxTexCoords.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * vSkyBoxVertices.size(), vSkyBoxVertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3) * vSkyBoxVertices.size(), sizeof(vec2) * vSkyBoxTexCoords.size(), vSkyBoxTexCoords.data());
	//cout<<"bind buffers\n";


	GLuint vPosition = glGetAttribLocation(skyProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint vTexCoord = glGetAttribLocation(skyProgram, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3) * 24));


	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, textureFront);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, textureBack);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, textureLeft);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, textureRight);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, textureUp);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, textureDown);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

	glUseProgram(program);
	genTerrain();
	genIndices();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * terrain.size() + sizeof(vec2) * texCoord.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * terrain.size(), terrain.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * terrain.size(), sizeof(vec2) * texCoord.size(), texCoord.data());

	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float) * terrain.size()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texture5);

	//cout<<cameraPos<<endl;
    glDrawElements( GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, indices.data());

	genTerrain2(cameraPos.x - 45, cameraPos.z + 14, cameraPos.x + 45, cameraPos.z + 45);
	genIndices2(31, 90);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * terrain2.size() + sizeof(vec2) * texCoord2.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * terrain2.size(), terrain2.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * terrain2.size(), sizeof(vec2) * texCoord2.size(), texCoord2.data());

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float) * terrain2.size()));

	glDrawElements( GL_TRIANGLE_STRIP, indices2.size(), GL_UNSIGNED_INT, indices2.data());

	genTerrain2(cameraPos.x - 45, cameraPos.z - 14, cameraPos.x - 14, cameraPos.z + 15);
	genIndices2(31, 31);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * terrain2.size() + sizeof(vec2) * texCoord2.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * terrain2.size(), terrain2.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * terrain2.size(), sizeof(vec2) * texCoord2.size(), texCoord2.data());

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float) * terrain2.size()));

	glDrawElements( GL_TRIANGLE_STRIP, indices2.size(), GL_UNSIGNED_INT, indices2.data());

	genTerrain2(cameraPos.x - 45, cameraPos.z - 45, cameraPos.x + 45, cameraPos.z - 14);
	genIndices2(90, 31);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * terrain2.size() + sizeof(vec2) * texCoord2.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * terrain2.size(), terrain2.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * terrain2.size(), sizeof(vec2) * texCoord2.size(), texCoord2.data());

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float) * terrain2.size()));

	glDrawElements( GL_TRIANGLE_STRIP, indices2.size(), GL_UNSIGNED_INT, indices2.data());

	genTerrain2(cameraPos.x + 14, cameraPos.z - 15, cameraPos.x + 45, cameraPos.z + 15);
	genIndices2(30, 31);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * terrain2.size() + sizeof(vec2) * texCoord2.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * terrain2.size(), terrain2.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * terrain2.size(), sizeof(vec2) * texCoord2.size(), texCoord2.data());

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float) * terrain2.size()));

	glDrawElements( GL_TRIANGLE_STRIP, indices2.size(), GL_UNSIGNED_INT, indices2.data());

	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vTexCoord);

	glUseProgram(0);

	glutSwapBuffers();
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

    float sensitivity = 0.3f; // change this value to your liking
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
	glutCreateWindow("Project 4");

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
