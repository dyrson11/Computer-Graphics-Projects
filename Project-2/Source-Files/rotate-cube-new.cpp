#include "Angel-yjc.h"

#include <bits/stdc++.h>
using namespace std;
#include "declarations.h"
#include "tools.h"
using namespace Angel;


// OpenGL initialization
void init()
{

	glGenBuffers(1, &sphere_buffer_color);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sphere_pos_vec.size() * 2, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*sphere_pos_vec.size(), &sphere_pos_vec[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*sphere_pos_vec.size(), sizeof(float)*sphere_pos_vec.size(), sphere_col_vec.data());

	glGenBuffers(1, &sphere_buffer_flat_normal);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer_flat_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sphere_pos_vec.size() * 2, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*sphere_pos_vec.size(), &sphere_pos_vec[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*sphere_pos_vec.size(), sizeof(float)*sphereSmoothNorm.size(), sphereFlatNorm.data());

	glGenBuffers(1, &sphere_buffer_smooth_normal);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer_smooth_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sphere_pos_vec.size() * 2, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*sphere_pos_vec.size(), &sphere_pos_vec[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*sphere_pos_vec.size(), sizeof(float)*sphereSmoothNorm.size(), sphereSmoothNorm.data());

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

	glGenBuffers(1, &x_axis_norm);
	glBindBuffer(GL_ARRAY_BUFFER, x_axis_norm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(x_points) + sizeof(x_normal),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(x_points), x_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_points), sizeof(x_normal), x_normal);

	glGenBuffers(1, &y_axis_norm);
	glBindBuffer(GL_ARRAY_BUFFER, y_axis_norm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(y_points) + sizeof(y_normal),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(y_points), y_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(y_points), sizeof(y_normal), y_normal);

	glGenBuffers(1, &z_axis_norm);
	glBindBuffer(GL_ARRAY_BUFFER, z_axis_norm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(z_points) + sizeof(z_normal),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(z_points), z_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(z_points), sizeof(z_normal), z_normal);

    //SET FLOOR VALUES
	floor();
	glGenBuffers(1, &floor_buffer_color);
	glBindBuffer(GL_ARRAY_BUFFER, floor_buffer_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_colors),
		floor_colors);

	glGenBuffers(1, &floor_buffer_normal);
	glBindBuffer(GL_ARRAY_BUFFER, floor_buffer_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_normals),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_normals),
		floor_normals);

	//LOAD SHADERS
	lightingProgram = InitShader("../Shaders/lighting_vShader.glsl", "../Shaders/lighting_fShader.glsl");
	colorProgram = InitShader("../Shaders/color_vShader.glsl", "../Shaders/color_fShader.glsl");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.529, 0.807, 0.92, 0.0);
	glLineWidth(2.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_CULL_FACE);


}

void drawObj(GLuint buffer, int num_vertices)
{
	//--- Activate the vertex buffer object to be drawn ---//
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	/*----- Set up vertex attribute arrays for each vertex attribute -----*/
	if(lighting)
	{
		GLuint vPosition = glGetAttribLocation(lightingProgram, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));
		GLuint vNormal = glGetAttribLocation(lightingProgram, "vNormal");
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(vec3) * num_vertices));
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);
		glDisableVertexAttribArray(vPosition);
	    glDisableVertexAttribArray(vNormal);
	}
	else
	{
		GLuint vPosition = glGetAttribLocation(colorProgram, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

		GLuint vColor = glGetAttribLocation(colorProgram, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(vec3) * num_vertices));
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);
		glDisableVertexAttribArray(vPosition);
	    glDisableVertexAttribArray(vColor);
	}

}
//----------------------------------------------------------------------------
void display(void)
{
	GLuint  model;  // model-view matrix uniform shader variable location
    GLuint  view;
    GLuint  imodel;
	GLuint  projection;  // projection matrix uniform shader variable location
	GLuint  viewPos;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(lighting)
	{
		glUseProgram(lightingProgram); // Use the shader lightingProgram

		model = glGetUniformLocation(lightingProgram, "model");
	    imodel = glGetUniformLocation(lightingProgram, "imodel");
	    view = glGetUniformLocation(lightingProgram, "view");
		projection = glGetUniformLocation(lightingProgram, "projection");

		mat4  p = Perspective(fovy, aspect, zNear, zFar);

		vec4 at(-7.0, -3.0, 10.0, 0.0);
		vec4 up(0.0, 1.0, 0.0, 0.0);
	    mat4  v = LookAt(eye, at, up);

	    mat4 m;
		m = Translate(a, 1, c) * Rotate(angle, angle_X, 0, angle_Z);

	    mat4  im = mat4WithUpperLeftMat3(inverse(upperLeftMat3(m)));

		glUniformMatrix4fv(model, 1, GL_TRUE, m);
	    glUniformMatrix4fv(view, 1, GL_TRUE, v);
	    glUniformMatrix4fv(imodel, 1, GL_TRUE, im);
		glUniformMatrix4fv(projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major

		if (wireFrame == 0) // Filled cube
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else              // Wireframe cube
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	    glUniform3f(glGetUniformLocation(lightingProgram, "material.ambient"), 0.2f, 0.2f, 0.2f);
	    glUniform3f(glGetUniformLocation(lightingProgram, "material.diffuse"), 1.0f, 0.84f, 0.0f);
	    glUniform3f(glGetUniformLocation(lightingProgram, "material.specular"), 1.0f, 0.84f, 0.0f);
	    glUniform1f(glGetUniformLocation(lightingProgram, "material.shininess"), 125.0f);

	    vec3 dir = inverse(upperLeftMat3(v)) * vec3(0.1f, 0.0f, -1.0f);
		//cout<<dir.x<<" "<<dir.y<<" "<<dir.z<<endl;
	    //glUniform3f(glGetUniformLocation(lightingProgram, "dirLight.direction"), -0.638813, -0.238667, 0.738211);
		glUniform3f(glGetUniformLocation(lightingProgram, "dirLight.direction"), dir.x, dir.y, dir.z);
	    glUniform3f(glGetUniformLocation(lightingProgram, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);
	    glUniform3f(glGetUniformLocation(lightingProgram, "dirLight.diffuse"), 0.8f, 0.8f, 0.8f);
	    glUniform3f(glGetUniformLocation(lightingProgram, "dirLight.specular"), 0.2f, 0.2f, 0.2f);

		if(pLight)
		{
			glUniform3f(glGetUniformLocation(lightingProgram, "pointLight.position"), -14.0f, 12.0f, -3.0f);
			glUniform3f(glGetUniformLocation(lightingProgram, "pointLight.ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingProgram, "pointLight.diffuse"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(lightingProgram, "pointLight.specular"), 1.0f, 1.0f, 1.0f);
			glUniform1f(glGetUniformLocation(lightingProgram, "pointLight.constant"), 2.0f);
			glUniform1f(glGetUniformLocation(lightingProgram, "pointLight.linear"), 0.01f);
			glUniform1f(glGetUniformLocation(lightingProgram, "pointLight.quadratic"), 0.001f);
			glUniform1i(glGetUniformLocation(lightingProgram, "pLight"), 1);
		}
		else
		{
			glUniform3f(glGetUniformLocation(lightingProgram, "spotLight.position"), -14.0f, 12.0f, -3.0f);
			glUniform3f(glGetUniformLocation(lightingProgram, "spotLight.direction"), -6.0f, 0.0f, -4.5f);
			glUniform3f(glGetUniformLocation(lightingProgram, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingProgram, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(lightingProgram, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
			glUniform1f(glGetUniformLocation(lightingProgram, "spotLight.constant"), 2.0f);
			glUniform1f(glGetUniformLocation(lightingProgram, "spotLight.linear"), 0.01f);
			glUniform1f(glGetUniformLocation(lightingProgram, "spotLight.quadratic"), 0.001f);
			glUniform1f(glGetUniformLocation(lightingProgram, "spotLight.cutOff"), cos(160.0f * M_PI / 180.0));
			glUniform1f(glGetUniformLocation(lightingProgram, "spotLight.outerCutOff"), cos(0.0f * M_PI / 180.0));
			glUniform1i(glGetUniformLocation(lightingProgram, "pLight"), 0);
		}


	    glUniform3f(glGetUniformLocation(lightingProgram, "globalAmbientLight"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingProgram, "viewPos"), eye[0], eye[1], eye[2]);

		if(flatShading)
			drawObj(sphere_buffer_flat_normal, sphere_pos_vec.size() / 3);
		else
			drawObj(sphere_buffer_smooth_normal, sphere_pos_vec.size() / 3);

		m = 1;
	    im = 1;
		glUniformMatrix4fv(model, 1, GL_TRUE, m); // GL_TRUE: matrix is row-major
	    glUniformMatrix4fv(view, 1, GL_TRUE, v); // GL_TRUE: matrix is row-major
	    glUniformMatrix4fv(imodel, 1, GL_TRUE, im);

		if (wireFrame == 0) // Filled floor
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else              // Wireframe floor
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	    glUniform3f(glGetUniformLocation(lightingProgram, "material.ambient"), 0.2f, 0.2f, 0.2f);
	    glUniform3f(glGetUniformLocation(lightingProgram, "material.diffuse"), 0.0f, 1.0f, 0.0f);
	    glUniform3f(glGetUniformLocation(lightingProgram, "material.specular"), 0.0f, 0.0f, 0.0f);
		drawObj(floor_buffer_normal, floor_NumVertices);  // draw the floor

	    glUniform3f(glGetUniformLocation(lightingProgram, "material.diffuse"), 1.0f, 0.0f, 0.0f);
	    drawObj(x_axis_norm, 3);
	    glUniform3f(glGetUniformLocation(lightingProgram, "material.diffuse"), 1.0f, 0.0f, 1.0f);
		drawObj(y_axis_norm, 3);
	    glUniform3f(glGetUniformLocation(lightingProgram, "material.diffuse"), 0.0f, 0.0f, 1.0f);
		drawObj(z_axis_norm, 3);


	    glUseProgram(0);
	}
	else
	{
		model = glGetUniformLocation(colorProgram, "model");
	    view = glGetUniformLocation(colorProgram, "view");
		projection = glGetUniformLocation(colorProgram, "projection");

		mat4  p = Perspective(fovy, aspect, zNear, zFar);
		vec4 at(-7.0, -3.0, 10.0, 0.0);
		vec4 up(0.0, 1.0, 0.0, 0.0);
	    mat4  v = LookAt(eye, at, up);

	    mat4 m;
		m = Translate(a, 1, c) * Rotate(angle, angle_X, 0, angle_Z);

	    mat4  im = mat4WithUpperLeftMat3(inverse(upperLeftMat3(m)));
	    //mat4  mv = m * v;

		glUniformMatrix4fv(model, 1, GL_TRUE, m);
	    glUniformMatrix4fv(view, 1, GL_TRUE, v);
		glUniformMatrix4fv(projection, 1, GL_TRUE, p);

		if (wireFrame == 0) // Filled cube
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else              // Wireframe cube
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glUseProgram(colorProgram);
		drawObj(sphere_buffer_color, sphere_pos_vec.size() / 3);

		m = 1;
	    im = 1;
		glUniformMatrix4fv(model, 1, GL_TRUE, m); // GL_TRUE: matrix is row-major
	    glUniformMatrix4fv(view, 1, GL_TRUE, v); // GL_TRUE: matrix is row-major
	    glUniformMatrix4fv(projection, 1, GL_TRUE, p);

		if (wireFrame == 0) // Filled floor
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else              // Wireframe floor
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		drawObj(floor_buffer_color, floor_NumVertices);

		glUniform3f(glGetUniformLocation(colorProgram, "vColor"), 1.0f, 0.0f, 0.0f);
	    drawObj(x_axis, 3);
	    glUniform3f(glGetUniformLocation(colorProgram, "vColor"), 1.0f, 0.0f, 1.0f);
		drawObj(y_axis, 3);
	    glUniform3f(glGetUniformLocation(colorProgram, "vColor"), 0.0f, 0.0f, 1.0f);
		drawObj(z_axis, 3);
	}
	glUseProgram(colorProgram);




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
		if (a <= -1 && c <= -4)
        {
            a = -1;
            c = -4;
            flagr = 2;
			angle_X = 1/(3.16228*20); // rotation angle in degrees
        	angle_Z = -3/(3.16228*20); // rotation angle in degrees

        }
		break;

	case 2:
        a += 0.225 / 4.74341649;
		c += 0.075 / 4.74341649;
		if (a >= 3.5 && c >= -2.5)
        {
            a = 3.5;
            c = -2.5;
            flagr = 3;
			angle_X = 15/(15.0333*20); // rotation angle in degrees
        	angle_Z = 1/(15.0333*20); // rotation angle in degrees
        }
		break;

	case 3:
		a -= 0.025 / 5;
		c += 0.375 / 5;
		if (a <= 3 && c >= 5)
        {
            a = 3;
            c = 5;
            flagr = 1;
			angle_Z = 4/(9.84886*20);
        	angle_X = -9/(9.84886*20); // rotation angle in degrees
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
    		wireFrame = 1 - wireFrame;
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

void menu(int num)
{
	switch (num)
	{
		case 0:
			glutDestroyWindow(window);
			exit(0);
			break;
		case 1:
			eye = init_eye;
			break;
		case 3:
			lighting = false;
			break;
		case 4:
			lighting = true;
			break;
		case 5:
			wireFrame = false;
			break;
		case 6:
			wireFrame = true;
			break;
		case 7:
			flatShading = true;
			break;
		case 8:
			flatShading  = false;
			break;
		case 9:
			pLight = false;
			break;
		case 10:
			pLight = true;
			break;
		default:
			value = num;

	}
	glutPostRedisplay();
}

//------------------------------------------------
void createMenu(void) {
	submenu_id = glutCreateMenu(menu);
	glutAddMenuEntry("NO", 3);
	glutAddMenuEntry("YES", 4);

	submenu0_id = glutCreateMenu(menu);
	glutAddMenuEntry("flat shading", 7);
	glutAddMenuEntry("smooth shading", 8);

	submenu1_id = glutCreateMenu(menu);
	glutAddMenuEntry("spot light", 9);
	glutAddMenuEntry("point source", 10);

	submenu2_id = glutCreateMenu(menu);
	glutAddMenuEntry("NO", 5);
	glutAddMenuEntry("YES", 6);

	//submenu3_id = glutCreateMenu(menu);

	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Default View Point", 1);
	glutAddSubMenu("Enable Lighting", submenu_id);
	glutAddSubMenu("Shading", submenu0_id);
	glutAddSubMenu("Ligth Source", submenu1_id);
	glutAddSubMenu("Wire Frame", submenu2_id);
	//glutAddSubMenu("Services", submenu3_id);
	glutAddMenuEntry("Quit", 0);

	glutAttachMenu(GLUT_LEFT_BUTTON);
}

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

    createMenu();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	init();
	glutMainLoop();
	return 0;
}
