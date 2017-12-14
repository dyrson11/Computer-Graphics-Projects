
GLuint colorProgram;
GLuint lightingProgram;
GLuint x_axis, y_axis, z_axis;
GLuint x_axis_norm, y_axis_norm, z_axis_norm;
GLuint floor_buffer_normal;  /* vertex buffer object id for floor */
GLuint floor_buffer_color;
GLuint sphere_buffer_flat_normal;
GLuint sphere_buffer_smooth_normal;
GLuint sphere_buffer_color;


// Projection transformation parameters
GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 15.0;

GLfloat a = 3, b = 1, c = 5;
GLint flagr = 1;

GLfloat angle = 0.0;
GLfloat angle_X = -9/(9.84886*20);
GLfloat angle_Z = 4/(9.84886*20);

vec4 init_eye(7.0, 3.0, -10.0, 1.0); // initial viewer position
vec4 eye = init_eye;               // current viewer position

int animationFlag = 1; // 1: animation; 0: non-animation. Toggled by key 'a' or 'A'

int floorFlag = 1;  // 1: solid floor; 0: wireframe floor. Toggled by key 'f' or 'F'

const int cube_NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
vec3 cube_points[100];
vec3 cube_colors[100];

const int floor_NumVertices = 6; //(1 face)(2 triangles/face)(3 vertices/triangle)
vec3 floor_points[floor_NumVertices]; // positions for all vertices
vec3 floor_colors[floor_NumVertices]; // colors for all vertices
vec3 fn(0, 1, 0);
vec3 an(0, 1, 0);
vec3 floor_normals[] = {fn, fn, fn, fn, fn, fn};

vec3 x_points[3];
vec3 x_colors[3];
vec3 x_normal[] = {fn, fn, fn};
vec3 y_points[3];
vec3 y_colors[3];
vec3 y_normal[] = {fn, fn, fn};
vec3 z_points[3];
vec3 z_colors[3];
vec3 z_normal[] = {fn, fn, fn};

vector<float> sphere_pos_vec;
vector<float> sphere_col_vec;
vector<float> sphereFlatNorm;
vector<float> sphereSmoothNorm;

static int window;
static int menu_id;
static int submenu_id;
static int submenu0_id;
static int submenu1_id;
static int submenu2_id;
static int submenu3_id;

static int value = 0;

bool lighting = false;
bool wireFrame = false;
bool flatShading = false;
bool pLight = false;


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
