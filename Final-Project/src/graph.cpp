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
    edges.insert(edge1);
    edge_list.push_back(edge1);
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

    sampleGraph.insertVertices(mod.lines.size());
    int idx = 0;
    for( auto& tLine : mod.lines )
    {
        temp = new Node( tLine->a, tLine->b, tLine->vertices[0]->pos, tLine->vertices[1]->pos, idx++ );
        nodes.insert( make_pair( make_pair(tLine->a, tLine->b), temp ) );
    }

    //std::vector<double> weights( nodes.size() );


    for( auto& tLine : mod.lines )
    {
        st = nodes[make_pair(tLine->a, tLine->b)];

        origin = tLine->vertices[0]->pos;
        start = tLine->vertices[1]->pos;
        norm = tLine->vertices[0]->normal;
        vectorStart = start - origin;
        projectionStart = vectorStart - glm::dot( vectorStart, norm ) / glm::length2( norm ) * norm;
        for( auto& neighbor : tLine->vertices[0]->lines )
        {
            if( neighbor->index == tLine->index )
                continue;

            en = nodes[make_pair(neighbor->a, neighbor->b)];

            end = neighbor->vertices[ neighbor->vertices[0]->pos == origin ]->pos;
            vectorEnd = end - origin;
            projectionEnd = vectorEnd - glm::dot( vectorEnd, norm ) / glm::length2( norm ) * norm;

            dot = glm::dot(projectionStart, projectionEnd);
            det = glm::dot( norm, glm::cross( projectionStart, projectionEnd ) );
            angle = abs( atan2( det, dot ) ) * (180.0/3.141592653589793238463);

            angle = ( angle <= 90 ) ? angle : 180 - angle;
            arg = ( angle <= 45 ) ? ( angle / sigma1 ) : ( ( 90 - angle ) / sigma2 );
            arg = exp( -arg * arg);
            arg = ( angle <= 45 ) ? arg : wn * arg;

            if( inserted_edges.find( make_pair( min(st->index, en->index), max( st->index, en->index ) ) ) == inserted_edges.end() )
            {
                insertEdge(st, en, arg);
                sampleGraph.insertEdge( min(st->index, en->index), max( st->index, en->index ) );
                weights.push_back(arg);
                inserted_edges.insert( make_pair( make_pair( min(st->index, en->index), max( st->index, en->index ) ), arg ) );
            }

            //cout<<angle<<" ";
            /*if(arg < 0)
            {
                cout<<"neg ";
            }
            else
            {
                cout<<"pos ";
            }*/
        }

        origin = tLine->vertices[1]->pos;
        start = tLine->vertices[0]->pos;
        norm = tLine->vertices[1]->normal;
        vectorStart = start - origin;
        projectionStart = vectorStart - glm::dot( vectorStart, norm ) * norm / glm::dot( norm, norm );

        for( auto& neighbor : tLine->vertices[1]->lines )
        {
            if( neighbor->index == tLine->index )
                continue;

            en = nodes[ make_pair( neighbor->a, neighbor->b ) ];

            end = neighbor->vertices[ neighbor->vertices[0]->pos == origin ]->pos;
            vectorEnd = end - origin;
            projectionEnd = vectorEnd - glm::dot( vectorEnd, norm ) * norm / glm::dot( norm, norm );

            dot = glm::dot( projectionStart, projectionEnd );
            det = glm::dot( norm, glm::cross( projectionStart, projectionEnd ) );
            angle = abs( atan2( det, dot ) ) * (180.0/3.141592653589793238463);
            angle = ( angle <= 90 ) ? angle : 180 - angle;

            arg = ( angle <= 45 ) ? ( angle / sigma1 ) : ( ( 90 - angle ) / sigma2 );
            arg = exp( -arg * arg);
            arg = ( angle <= 45 ) ? arg : wn * arg;

            if( inserted_edges.find( make_pair( min(st->index, en->index), max( st->index, en->index ) ) ) == inserted_edges.end() )
            {
                insertEdge ( st, en, arg );
                sampleGraph.insertEdge( min(st->index, en->index), max( st->index, en->index ) );
                weights.push_back(arg);
                inserted_edges.insert( make_pair( make_pair( min(st->index, en->index), max( st->index, en->index ) ), arg ) );
            }
        }

    }

    for( Quad tQuad : mod.quads )
    {
        st = nodes[ make_pair( min( tQuad.i1, tQuad.i2 ), max( tQuad.i1, tQuad.i2 ) ) ];
        start = mod.vertices[ tQuad.i1 ]->pos;
        end = mod.vertices[ tQuad.i2 ]->pos;
        vectorStart = start - end;

        en = nodes[ make_pair( min( tQuad.i3, tQuad.i4 ), max( tQuad.i3, tQuad.i4 ) ) ];
        start = mod.vertices[ tQuad.i4 ]->pos;
        end = mod.vertices[ tQuad.i3 ]->pos;
        vectorEnd = start - end;

        dot = glm::dot( glm::normalize( vectorStart ), glm::normalize( vectorEnd ) );
        angle = abs( acos( dot ) ) * (180.0/3.141592653589793238463);
        angle = ( angle <= 90 ) ? angle : 180 - angle;

        arg = angle / sigma3;
        arg = exp( -arg * arg);
        arg = wp * arg;

        if( inserted_edges.find( make_pair( min(st->index, en->index), max( st->index, en->index ) ) ) == inserted_edges.end() )
        {
            insertEdge ( st, en, arg );
            sampleGraph.insertEdge( min(st->index, en->index), max( st->index, en->index ) );
            weights.push_back(arg);
            inserted_edges.insert( make_pair( make_pair( min(st->index, en->index), max( st->index, en->index ) ), arg ) );
        }



        st = nodes[ make_pair( min( tQuad.i2, tQuad.i3 ), max( tQuad.i2, tQuad.i3 ) ) ];
        start = mod.vertices[ tQuad.i2 ]->pos;
        end = mod.vertices[ tQuad.i3 ]->pos;
        vectorStart = start - end;

        en = nodes[ make_pair( min( tQuad.i1, tQuad.i4 ), max( tQuad.i1, tQuad.i4 ) ) ];
        start = mod.vertices[ tQuad.i1 ]->pos;
        end = mod.vertices[ tQuad.i4 ]->pos;
        vectorEnd = start - end;

        dot = glm::dot( glm::normalize( vectorStart ), glm::normalize( vectorEnd ) );
        angle = abs( acos( dot ) ) * (180.0/3.141592653589793238463);
        angle = ( angle <= 90 ) ? angle : 180 - angle;

        arg = angle / sigma3;
        arg = exp( -arg * arg);
        arg = wp * arg;

        if( inserted_edges.find( make_pair( min(st->index, en->index), max( st->index, en->index ) ) ) == inserted_edges.end() )
        {
            insertEdge ( st, en, arg );
            sampleGraph.insertEdge( min(st->index, en->index), max( st->index, en->index ) );
            weights.push_back(arg);
            inserted_edges.insert( make_pair( make_pair( min(st->index, en->index), max( st->index, en->index ) ), arg ) );
        }
    }
}


template class graph<float, float>;
