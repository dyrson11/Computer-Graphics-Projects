#pragma once
#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
using namespace std;

#include "../graph/include/andres/graph/graph.hxx"
#include "../graph/include/andres/graph/complete-graph.hxx"
#include "../graph/include/andres/graph/multicut-lifted/greedy-additive.hxx"
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

    map< pair<int, int>, Node* > nodes;
    unordered_set<Edge*> edges;
    map< pair<int, int>, float > inserted_edges;
    vector<Edge*> edge_list;

    andres::graph::Graph<> sampleGraph;
    vector<float> weights;

    //andres::graph::CompleteGraph<> lifted_graph;

    void init_graph(model<N,E> mod);

    bool insertNode(Node *x);
    bool insertEdge(Node *a, Node *b, float weight);
    bool removeEdge(Edge *x, bool dir);
    bool removeNode(Node *x);
};
