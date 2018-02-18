#include "../include/graph.h"
#include "../include/utils.h"

template<typename N, typename E>
bool graph<N,E>::insertNode(Node* x)
{
    //nodes.insert(x);
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
    /*typename unordered_set <Edge*>::iterator i;
    for( i = x->m_edges.begin(); i != x->m_edges.end(); i++)
    {
        removeEdge(*i, x == (*i)->m_nodes[0]);
    }
    nodes.erase(nodes.find(x));
    delete x;*/
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
    glm::vec3 origin, start, norm, vectorStart, projectionStart, end, vectorEnd, projectionEnd;
    float dot, det, angle, arg;
    float sigma1 = 5, sigma2 = 15, sigma3 = 5, wp = 0.05, wn = -5;

    for( auto& tLine : mod.lines )
    {
        temp = new Node( tLine->a, tLine->b, tLine->vertices[0]->pos, tLine->vertices[1]->pos );
        nodes.insert( make_pair( make_pair(tLine->a, tLine->b), temp ) );
    }

    for( auto& tLine : mod.lines )
    {
        st = nodes[make_pair(tLine->a, tLine->b)];

        origin = tLine->vertices[0]->pos;
        start = tLine->vertices[1]->pos;
        norm = tLine->vertices[0]->normal;
        vectorStart = start - origin;
        projectionStart = vectorStart - glm::dot( vectorStart, norm ) * norm / glm::normalize(norm);
        for( auto& neighbor : tLine->vertices[0]->lines )
        {
            if( neighbor == tLine )
                continue;

            en = nodes[make_pair(neighbor->a, neighbor->b)];

            end = neighbor->vertices[ neighbor->vertices[0]->pos == origin ]->pos;
            vectorEnd = end - origin;
            projectionEnd = vectorEnd - glm::dot( vectorEnd, norm ) * norm / glm::normalize(norm);

            dot = glm::dot(projectionStart, projectionEnd);
            det = glm::dot( norm, glm::cross( projectionStart, projectionEnd ) );
            angle = abs( atan2( det, dot ) );
            angle = ( angle <= 90 ) ? 180 - angle : angle;

            arg = ( angle <= 45 ) ? ( angle / sigma1 ) : ( ( 90 - angle ) / sigma2 );
            arg = exp( -arg * arg);
            arg = ( angle <= 45 ) ? arg : wn * arg;

            insertEdge(st, en, arg);
        }

        origin = tLine->vertices[1]->pos;
        start = tLine->vertices[0]->pos;
        norm = tLine->vertices[1]->normal;
        vectorStart = start - origin;
        projectionStart = vectorStart - glm::dot( vectorStart, norm ) * norm / glm::normalize(norm);

        for( auto& neighbor : tLine->vertices[1]->lines )
        {
            if( neighbor == tLine )
                continue;

            en = nodes[ make_pair( neighbor->a, neighbor->b ) ];

            end = neighbor->vertices[ neighbor->vertices[0]->pos == origin ]->pos;
            vectorEnd = end - origin;
            projectionEnd = vectorEnd - glm::dot( vectorEnd, norm ) * norm / glm::normalize(norm);

            dot = glm::dot( projectionStart, projectionEnd );
            det = glm::dot( norm, glm::cross( projectionStart, projectionEnd ) );
            angle = abs( atan2( det, dot ) );
            angle = ( angle <= 90 ) ? 180 - angle : angle;

            arg = ( angle <= 45 ) ? ( angle / sigma1 ) : ( ( 90 - angle ) / sigma2 );
            arg = exp( -arg * arg);
            arg = ( angle <= 45 ) ? arg : wn * arg;

            insertEdge ( st, en, arg );
        }

    }

    for( Quad tQuad : mod.quads )
    {
        st = nodes[ make_pair( min( tQuad.i1, tQuad.i2 ), max( tQuad.i1, tQuad.i2 ) ) ];
        start = mod.vertices[ tQuad.i1 ]->pos;
        end = mod.vertices[ tQuad.i2 ]->pos;
        norm = ( mod.vertices[ tQuad.i1 ]->pos +
                 mod.vertices[ tQuad.i2 ]->pos +
                 mod.vertices[ tQuad.i3 ]->pos +
                 mod.vertices[ tQuad.i4 ]->pos ) * 0.25f;
        vectorStart = start - end;
        projectionStart = vectorStart - glm::dot( vectorStart, norm ) * norm / glm::normalize(norm);

        en = nodes[ make_pair( min( tQuad.i3, tQuad.i4 ), max( tQuad.i3, tQuad.i4 ) ) ];
        start = mod.vertices[ tQuad.i4 ]->pos;
        end = mod.vertices[ tQuad.i3 ]->pos;
        vectorEnd = start - end;
        projectionStart = vectorStart - glm::dot( vectorStart, norm ) * norm / glm::normalize(norm);

        dot = glm::dot( projectionStart, projectionEnd );
        det = glm::dot( norm, glm::cross( projectionStart, projectionEnd ) );
        angle = abs( atan2( det, dot ) );
        angle = ( angle <= 90 ) ? 180 - angle : angle;

        arg = angle / sigma3;
        arg = exp( -arg * arg);
        arg = wp * arg;

        insertEdge ( st, en, arg );
    }
}


template class graph<float, float>;
