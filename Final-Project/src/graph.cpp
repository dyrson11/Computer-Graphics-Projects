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
    Node* temp;
    for( mod.Line* tLine : mod.lines.size() )
    {
        temp = new Node( tLine->a, tLine->b, tLine->vertices[0]->pos, tLine->vertices[1]->pos );
        nodes.insert( make_pair( make_pair(tLine->a, tLine->b), temp ) );
    }

    for( mod.Line* tLine : mod.lines.size() )
    {
        vec3 origin = tLine->vertices[0]->pos;
        vec3 start = tLine->vertices[1]->pos;
        vec3 norm = tLine->vertices[0]->normal;
        vec3 vectorStart = start - origin;
        float sigma1 = 5, sigma2 = 15, wn = -5;
        Node *st = nodes[make_pair(tLine->a, tLine->b)];

        vec3 projectionStart = vectorStart - dot( vectorStart, norm ) * norm / norm(norm);
        for( mod.Line* neighbor : tLine->vertices[0]->lines )
        {
            if( neighbor != tLine )
            {
                Node *en = nodes[make_pair(neighbor->a, neighbor->b)];

                vec3 end = neighbor->vertices[neighbor->vertices[0]->pos == origin];
                vec3 vectorEnd = end - origin;
                vec3 projectionEnd = vectorEnd - dot( vectorEnd, norm ) * norm / norm(norm);

                float dot = dot(projectionStart, projectionEnd);
                float det = dot( norm, cross( projectionStart, projectionEnd ) );
                float angle = 180 - abs( atan2( det, dot ) );

                float arg = ( angle <= 45 ) ? ( angle / sigma1 ) : ( ( 90 - angle ) / sigma2 );
                arg = ( angle <= 45 ) ? arg : wn * arg;

                insertEdge(st, en, arg);
            }
        }
        for(int j = 0; j < obj.lines[i]->vertices[1]->lines.size(); j++)
        {
            if(obj.lines[i]->vertices[1]->lines[j] != obj->lines[i])
            {
                vec3 origin = obj.lines[i]->vertices[1]->pos;
                vec3 a = obj.lines[i]->vertices[1]->lines[j]->vertices[obj.lines[i]->vertices[1]->lines[j]->vertices[0]->pos == origin];
                vec3 b = obj.lines[i]->vertices[1]->normal + origin;

                float arg = ( angle <= 45 ) ? ( angle / sigma1 ) : ( ( 90 - angle ) / sigma2 );

                matrix_graph[i][obj.lines[i]->vertices[1]->lines[j]->index] = exp( -arg * arg);
                if(angleBetween(a, b, origin) <= 45)
                {
                    matrix_graph[i][obj.lines[i]->vertices[1]->lines[j]->index] = exp(0 - pow(angle / sigma2, 2));
                    matrix_graph[obj.lines[i]->vertices[1]->lines[j]->index][i] = exp(0 - pow(angle / sigma2, 2));
                }
                else
                {
                    matrix_graph[i][obj.lines[i]->vertices[1]->lines[j]->index] = wn * exp(0 - pow((90 - angle) / sigma2, 2));
                    matrix_graph[obj.lines[i]->vertices[1]->lines[j]->index][i] = wn * exp(0 - pow((90 - angle) / sigma2, 2));
                }
            }
        }
        for(int j = 0; j < obj.lines[i]->connections.size(); j++)
        {
            matrix_graph[i][obj.lines[i]->vertices[1]->lines[j]->index] =
        }
    }
}


//cGraph<float, float> graph;
