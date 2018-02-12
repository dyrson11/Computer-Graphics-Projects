#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;
using namespace std;

#include "graph_node.h"
#include "graph_edge.h"
#include "model.h"

template <typename N, typename E>
class graph
{
public:
    typedef graph <N, E> self;
    typedef node <self> Node;
    typedef edge <self> Edge;
    typedef N n;
    typedef E e;

    unordered_map< pair<int, int>, Node* > nodes;
    unordered_set<Edge*> edges;

    void init_graph(model<N,E> mod);

    bool insertNode(Node *x);
    bool insertEdge(Node *a, Node *b, float weight);
    bool removeEdge(Edge *x, bool dir);
    bool removeNode(Node *x);
};
