
GLuint program;       /* shader program object id */
GLuint x_axis, y_axis, z_axis;
GLuint cube_buffer;   /* vertex buffer object id for cube */
GLuint floor_buffer;  /* vertex buffer object id for floor */
GLuint sphere_buffer;

// Projection transformation parameters
GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 15.0;

GLfloat a = 3, b = 1, c = 5;
GLint flagr = 1;

GLfloat angle = 0.0;
GLfloat angleX = 0;
GLfloat angleZ = 0;

vec4 init_eye(7.0, 3.0, -10.0, 1.0); // initial viewer position
vec4 eye = init_eye;               // current viewer position

int animationFlag = 1; // 1: animation; 0: non-animation. Toggled by key 'a' or 'A'

int cubeFlag = 1;   // 1: solid cube; 0: wireframe cube. Toggled by key 'c' or 'C'
int floorFlag = 1;  // 1: solid floor; 0: wireframe floor. Toggled by key 'f' or 'F'

const int cube_NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
vec3 cube_points[100];
vec3 cube_colors[100];

const int floor_NumVertices = 6; //(1 face)(2 triangles/face)(3 vertices/triangle)
vec3 floor_points[floor_NumVertices]; // positions for all vertices
vec3 floor_colors[floor_NumVertices]; // colors for all vertices

vec3 x_points[3];
vec3 x_colors[3];
vec3 y_points[3];
vec3 y_colors[3];
vec3 z_points[3];
vec3 z_colors[3];

vector<float> sphere_pos_vec;
vector<float> sphere_col_vec;
vector<float> sphereFlatNorm;
vector<float> sphereSmoothNorm;


vec3 vertex_colors[8] =
{
	vec3(0.0, 0.0, 0.0),  // black
	vec3(1.0, 0.0, 0.0),  // red
	vec3(1.0, 1.0, 0.0),  // yellow
	vec3(0.0, 1.0, 0.0),  // green
	vec3(0.0, 0.0, 1.0),  // blue
	vec3(1.0, 0.0, 1.0),  // magenta
	vec3(1.0, 1.0, 1.0),  // white
	vec3(0.0, 1.0, 1.0)   // cyan
};
