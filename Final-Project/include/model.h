#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
using namespace glm;
using namespace std;

#include "model_line.h"
#include "model_vertex.h"


template <class V, class L>
class model
{
public:
    typedef model <V, L> self;
    typedef vertex <self> Vertex;
    typedef line <self> Line;
    typedef V v;
    typedef L l;

    vector <Vertex*> vertices;
    vector <Line*> lines;
    vector<vec3> positions;
    vector <GLuint> indices;

    void clear_model();
    void load_model(const char *);
    void computeNormals();

    bool insertVertex(Vertex*);
    void insertLine(int, int, Line*&);
};
