#pragma once
#include "../include/graph.h"
//#include "../include/utils.h"

template<typename N, typename E>
bool graph<N,E>::insertNode(Node* x)
{
    nodes.insert(x);
}

template<typename N, typename E>
bool graph<N,E>::insertEdge(Node *a, Node *b, float weight)
{
    Edge *edge1 = new Edge;
    edge1->m_nodes[0] = a;
    edge1->m_nodes[1] = b;
    edge1->weight = weight;
    a->m_edges.insert(edge1);
    b->m_edges.insert(edge1);
}

template<typename N, typename E>
bool graph<N,E>::removeNode(Node *x)
{
    typename unordered_set <Edge*>::iterator i;
    for( i = x->m_edges.begin(); i != x->m_edges.end(); i++)
    {
        removeEdge(*i, x == (*i)->m_nodes[0]);
    }
    nodes.erase(nodes.find(x));
    delete x;
}

template<typename N, typename E>
bool graph<N,E>::removeEdge(Edge *x, bool dir)//can change with a direction bool
{
    Node *a = x->m_nodes[dir];
    if(a->m_edges.find(x) == a->m_edges.end())
    {
        return false;
    }
    a->m_edges.erase(a->m_edges.find(x));
    edges.erase(edges.find(x));
    return true;
}

template<typename N, typename E>
void graph<N,E>::init_graph(model<N,E> mod)
{
    Node* temp, *en, *st;
    vec3 origin, start, norm, vectorStart, projectionStart, end, vectorEnd, projectionEnd;
    float dot, det, angle, arg;
    float sigma1 = 5, sigma2 = 15, sigma3 = 5, wp = 0.05, wn = -5;

    for( mod.Line* tLine : mod.lines.size() )
    {
        temp = new Node( tLine->a, tLine->b, tLine->vertices[0]->pos, tLine->vertices[1]->pos );
        nodes.insert( make_pair( make_pair(tLine->a, tLine->b), temp ) );
    }

    for( mod.Line* tLine : mod.lines.size() )
    {
        st = nodes[make_pair(tLine->a, tLine->b)];


        origin = tLine->vertices[0]->pos;
        start = tLine->vertices[1]->pos;
        norm = tLine->vertices[0]->normal;
        vectorStart = start - origin;
        projectionStart = vectorStart - dot( vectorStart, norm ) * norm / norm(norm);
        for( mod.Line* neighbor : tLine->vertices[0]->lines )
        {
            if( neighbor != tLine )
            {
                en = nodes[make_pair(neighbor->a, neighbor->b)];

                end = neighbor->vertices[neighbor->vertices[0]->pos == origin];
                vectorEnd = end - origin;
                projectionEnd = vectorEnd - dot( vectorEnd, norm ) * norm / norm(norm);

                dot = dot(projectionStart, projectionEnd);
                det = dot( norm, cross( projectionStart, projectionEnd ) );
                angle = 180 - abs( atan2( det, dot ) );

                arg = ( angle <= 45 ) ? ( angle / sigma1 ) : ( ( 90 - angle ) / sigma2 );
                arg = ( angle <= 45 ) ? arg : wn * arg;

                insertEdge(st, en, arg);
            }
        }

        origin = tLine->vertices[1]->pos;
        start = tLine->vertices[0]->pos;
        norm = tLine->vertices[1]->normal;
        vectorStart = start - origin;
        projectionStart = vectorStart - dot( vectorStart, norm ) * norm / norm(norm);

        for( mod.Line* neighbor : tLine->vertices[1]->lines )
        {
            if( neighbor != tLine )
            {
                *en = nodes[ make_pair( neighbor->a, neighbor->b ) ];

                end = neighbor->vertices[ neighbor->vertices[0]->pos == origin ];
                vectorEnd = end - origin;
                projectionEnd = vectorEnd - dot( vectorEnd, norm ) * norm / norm(norm);

                dot = dot( projectionStart, projectionEnd );
                det = dot( norm, cross( projectionStart, projectionEnd ) );
                angle = 180 - abs( atan2( det, dot ) );

                arg = ( angle <= 45 ) ? ( angle / sigma1 ) : ( ( 90 - angle ) / sigma2 );
                arg = ( angle <= 45 ) ? arg : wn * arg;

                insertEdge ( st, en, arg );
            }
        }

    }

    for( Quad tQuad : mod.quads )
    {
        start = mod.vertices[ tQuad[i1] ];
        end = mod.vertices[ tQuad[i2] ];
        norm =
        vectorStart = start - end;
        projectionStart = vectorStart - dot( vectorStart,  )

        start = mod.vertices[ tQuad[i4] ];
        end = mod.vertices[ tQuad[i3] ];
        vectorEnd = start - end;

        frontNeighbor
    }
}


//cGraph<float, float> graph;
