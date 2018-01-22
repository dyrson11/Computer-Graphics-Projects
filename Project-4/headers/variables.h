static int winwidth,winheight;
static int mx,my;
static int flag=0;
static float rotx=0.0f, roty=-70.0f;

vector<float> terrain;
vector<vec2> texCoord;
vector<int> indices;

vector<float> terrain2;
vector<vec2> texCoord2;
vector<int> indices2;

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

GLuint VAO;
GLuint VBO, VBO_Skybox;
GLuint program, skyProgram;
GLuint texture1, texture2, texture3, texture4, texture5;
GLuint textureFront, textureBack, textureLeft, textureRight, textureUp, textureDown;
GLuint samplerFront, samplerBack, samplerLeft, samplerRight, samplerUp, samplerDown;

vector<vec3> vSkyBoxVertices;
vector<vec2> vSkyBoxTexCoords;
