#  define M_PI  3.14159265358979323846

model<float, float> obj;
graph<float, float> tGraph;
vector<glm::vec3> colors;


program program1;
GLuint VAO, VBO, IBO;
int winwidth, winheight, nClusters, nFlowLines;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
bool firstMouse = true;
float yaw1   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch1 =  0.0f;
float lastX =  600.0f / 2.0;
float lastY =  600.0 / 2.0;
glm::mat4 view;
