#include "graph_node.h"
#include "graph_edge.h"

template <typename N, typename E>
class graph
{
public:
    typedef cGraph <N, E> self;
    typedef CNode <self> Node;
    typedef CEdge <self> Edge;
    typedef N n;
    typedef E e;

    unordered_set <Node*> m_nodes_list;
    unordered_set <Edge*> m_edges_list;

    bool insertNode(Node *x);
    bool insertEdge(Node *a, Node *b);
    bool removeEdge(Edge *x, bool dir);
    bool removeNode(Node *x);
};
