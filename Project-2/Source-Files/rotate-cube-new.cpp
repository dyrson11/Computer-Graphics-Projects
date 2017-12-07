#include "Angel-yjc.h"

#include <bits/stdc++.h>
using namespace std;
#include "declarations.h"
#include "tools.h"
using namespace Angel;

// OpenGL initialization
void init()
{
	glGenBuffers(1, &sphere_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sphere_pos_vec.size() * 2, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*sphere_pos_vec.size(), &sphere_pos_vec[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*sphere_pos_vec.size(), sizeof(float)*sphereSmoothNorm.size(), &sphereSmoothNorm[0]);

    //SET AXIS VALUES
	lines();
	glGenBuffers(1, &x_axis);
	glBindBuffer(GL_ARRAY_BUFFER, x_axis);
	glBufferData(GL_ARRAY_BUFFER, sizeof(x_points) + sizeof(x_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(x_colors), x_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_points), sizeof(x_colors), x_colors);

	glGenBuffers(1, &y_axis);
	glBindBuffer(GL_ARRAY_BUFFER, y_axis);
	glBufferData(GL_ARRAY_BUFFER, sizeof(y_points) + sizeof(y_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(y_colors), y_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(y_points), sizeof(y_colors), y_colors);

	glGenBuffers(1, &z_axis);
	glBindBuffer(GL_ARRAY_BUFFER, z_axis);
	glBufferData(GL_ARRAY_BUFFER, sizeof(z_points) + sizeof(z_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(z_colors), z_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(z_points), sizeof(z_colors), z_colors);

    //SET FLOOR VALUES
	floor();
	glGenBuffers(1, &floor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_colors),
		floor_colors);

	//LOAD SHADERS
	program = InitShader("../Shaders/vshader42.glsl", "../Shaders/fshader42.glsl");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.529, 0.807, 0.92, 0.0);
	glLineWidth(2.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_CULL_FACE);

    glUseProgram(program);

}

void drawObj(GLuint buffer, int num_vertices)
{
	//--- Activate the vertex buffer object to be drawn ---//
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	/*----- Set up vertex attribute arrays for each vertex attribute -----*/
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(vec3) * num_vertices));

	glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	/*--- Disable each vertex attribute array being enabled ---*/
	glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vNormal);
}
//----------------------------------------------------------------------------
void display(void)
{
	GLuint  model;  // model-view matrix uniform shader variable location
    GLuint  view;
    GLuint  imodel;
	GLuint  projection;  // projection matrix uniform shader variable location

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program); // Use the shader program

	model = glGetUniformLocation(program, "model");
    imodel = glGetUniformLocation(program, "imodel");
    view = glGetUniformLocation(program, "view");
	projection = glGetUniformLocation(program, "projection");

	/*---  Set up and pass on Projection matrix to the shader ---*/
	mat4  p = Perspective(fovy, aspect, zNear, zFar);
	glUniformMatrix4fv(projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major

	vec4 at(-7.0, -3.0, 10.0, 0.0);
	vec4 up(0.0, 1.0, 0.0, 0.0);

	mat4  m = Translate(a, 1, c) * Rotate(angle, angleX, 0, angleZ);
    mat4  im = mat4WithUpperLeftMat3(inverse(upperLeftMat3(m)));
    mat4  v = LookAt(eye, at, up);
    mat4  mv = m * v;

	glUniformMatrix4fv(model, 1, GL_TRUE, m);
    glUniformMatrix4fv(view, 1, GL_TRUE, v);
    glUniformMatrix4fv(imodel, 1, GL_TRUE, im);

	if (cubeFlag == 1) // Filled cube
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else              // Wireframe cube
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUniform3f(glGetUniformLocation(program, "material.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(program, "material.diffuse"), 1.0f, 0.84f, 0.0f);
    glUniform3f(glGetUniformLocation(program, "material.specular"), 1.0f, 0.84f, 0.0f);
    glUniform1f(glGetUniformLocation(program, "shininess"), 125.0f);

    vec3 dir = projection * inverse(upperLeftMat3(v)) * model * vec3(0.1f, 0.0f, -1.0f);
    glUniform3f(glGetUniformLocation(program, "dirLight.direction"), -0.638813, -0.238667, 0.738211);
    glUniform3f(glGetUniformLocation(program, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(program, "dirLight.diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(program, "globalAmbientLight"), 1.0f, 1.0f, 1.0f);
    drawObj(sphere_buffer, sphere_pos_vec.size() / 3);

    glUseProgram(0);
    glUseProgram(program);

	m = 1;
    v = LookAt(eye, at, up);
    im = 1;
	glUniformMatrix4fv(model, 1, GL_TRUE, m); // GL_TRUE: matrix is row-major
    glUniformMatrix4fv(view, 1, GL_TRUE, v); // GL_TRUE: matrix is row-major
    glUniformMatrix4fv(imodel, 1, GL_TRUE, im);

	if (floorFlag == 1) // Filled floor
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else              // Wireframe floor
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUniform3f(glGetUniformLocation(program, "material.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(program, "material.diffuse"), 0.0f, 1.0f, 0.0f);
    glUniform3f(glGetUniformLocation(program, "material.specular"), 0.0f, 0.0f, 0.0f);
	drawObj(floor_buffer, floor_NumVertices);  // draw the floor

    glUniform3f(glGetUniformLocation(program, "material.diffuse"), 1.0f, 0.0f, 0.0f);
    drawObj(x_axis, 3);
    glUniform3f(glGetUniformLocation(program, "material.diffuse"), 1.0f, 0.0f, 1.0f);
	drawObj(y_axis, 3);
    glUniform3f(glGetUniformLocation(program, "material.diffuse"), 0.0f, 0.0f, 1.0f);
	drawObj(z_axis, 3);

    v = LookAt(eye, at, up);

    glUseProgram(0);


	glutSwapBuffers();
}
//---------------------------------------------------------------------------

void idle(void)
{
	angle += 0.05*180 / M_PI;

	switch (flagr)
    {
	case 1:
		a -= 0.02;
        c -= 0.045;
        angleX = v1.z;
        angleZ = -v1.x;
		if (a <= -1 && c <= -4)
        {
            a = -1;
            c = -4;
            flagr = 2;
        }
		break;

	case 2:
        a += 0.225 / 4.74341649;
		c += 0.075 / 4.74341649;
        angleX = c;
        angleZ = -a;
		if (a >= 3.5 && c >= -2.5)
        {
            a = 3.5;
            c = -2.5;
            flagr = 3;
        }
		break;

	case 3:
		a -= 0.025 / 5;
		c += 0.375 / 5;
        angleX = c;
        angleZ = -a;
		if (a <= 3 && c >= 5)
        {
            a = 3;
            c = 5;
            flagr = 1;
        }
		break;
	}
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
    {
    	case 033: // Escape Key
    	case 'q': case 'Q':
    		exit(EXIT_SUCCESS);
    		break;

    	case 'X': eye[0] += 1.0; break;
    	case 'x': eye[0] -= 1.0; break;
    	case 'Y': eye[1] += 1.0; break;
    	case 'y': eye[1] -= 1.0; break;
    	case 'Z': eye[2] += 1.0; break;
    	case 'z': eye[2] -= 1.0; break;

    	case 'a': case 'A': // Toggle between animation and non-animation
    		animationFlag = 1 - animationFlag;
    		if (animationFlag == 1) glutIdleFunc(idle);
    		else                    glutIdleFunc(NULL);
    		break;

    	case 'c': case 'C': // Toggle between filled and wireframe cube
    		cubeFlag = 1 - cubeFlag;
    		break;

    	case 'f': case 'F': // Toggle between filled and wireframe floor
    		floorFlag = 1 - floorFlag;
    		break;

    	case ' ':  // reset to initial viewer/eye position
    		eye = init_eye;
    		break;
	}
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	aspect = (GLfloat)width / (GLfloat)height;
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int err;
	//Cargar datos
	loadSphere(argv[1]);


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	// glutInitContextVersion(3, 2);
	// glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("esfera");

	/* Call glewInit() and error checking */
	err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Error: glewInit failed: %s\n", (char*)glewGetErrorString(err));
		exit(1);
	}
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	init();
	glutMainLoop();
	return 0;
}
