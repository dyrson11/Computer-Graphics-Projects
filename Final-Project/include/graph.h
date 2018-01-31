#include <bits/stdc++.h>
//using namespace glm;
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

    unordered_set <Node*> m_nodes_list;
    unordered_set <Edge*> m_edges_list;

    void init_graph(model<N,E> mod);

    bool insertNode(Node *x);
    bool insertEdge(Node *a, Node *b);
    bool removeEdge(Edge *x, bool dir);
    bool removeNode(Node *x);
};
