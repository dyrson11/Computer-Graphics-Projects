//#include <bits/stdc++.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class model
{
public:
    typedef cGraph <V, L> self;
    typedef vertex <self> Vertex;
    typedef line <self> Line;
    typedef V v;
    typedef L l;

    vector <Vertex*> vertices;
    vector <Line*> lines;

    void load_model(string const &path);
    void clear_model();

    bool insertVertex(Node *x);
    bool insertLine(Node *a, Node *b);
};
