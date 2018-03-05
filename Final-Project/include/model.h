#pragma once
#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
using namespace std;

#include "model_line.h"
#include "model_vertex.h"

struct Quad
{
    int i1;
    int i2;
    int i3;
    int i4;

    Quad()
    {
        i1 = -1;
        i2 = -1;
        i3 = -1;
        i4 = -1;
    }

    Quad( int a, int b, int c, int d )
    {
        i1 = a;
        i2 = b;
        i3 = c;
        i4 = d;
    }
};

struct Tri
{
    int i1;
    int i2;
    int i3;

    Tri()
    {
        i1 = -1;
        i2 = -1;
        i3 = -1;
    }

    Tri( int a, int b, int c )
    {
        i1 = a;
        i2 = b;
        i3 = c;
    }
};


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
    vector<glm::vec3> positions;
    vector <GLuint> indices;
    vector <GLuint> ids;

    map< pair<int, int>, Line* > insertedLines;
    //typename unordered_map< pair<int, int>, Line* >::iterator itLine;

    vector<Quad> quads;
    vector<Tri> tris;

    void clear_model();
    void load_model(const char *);
    void updateModelInitial();
    void updateModelClusters();
    void updateModelClusters( int );
    void updateModelFlowlines();
    void updateModelFlowlines( int );
    void computeNormals();

    bool insertVertex(Vertex*);
    void insertLine(int, int, Line*&);
};
