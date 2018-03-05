#pragma once
#include <bits/stdc++.h>
#include <glm.hpp>
#include "model.h"

class crestLines
{
public:
    int V, E, N;
    float rigidness, sphericalness, cyclidness;
    vector< glm::vec3 > vertices;
    vector< vector<int> > lines;

    void readCrestLines( const string ridgesPath, const string ravinesPath );
}
void crestLines::readCrestLines( const string ridgesPath, const string ravinesPath )
{
    if( freopen( ridgesPath, "r", stdin ) )
    {
        cin>>V>>E>>N;
        lines.resize(N);
        float x, y, z;
        int crestLineID;
        for( int i = 0; i < V; i++ )
        {
            cin>>x>>y>>z>>crestLineID;
            vertices.push_back( glm::vec3(x, y, z) );
            lines[ crestLineID ].push_back( i );
        }
        for( int i = 0; i < N; i++ )
        {
            cin>>rigidness>>sphericalness>>cyclidness;
            if( cyclidness < threshold )
                continue;

            callFuncionOfSearchFlowlineToSegment();
        }
    }
}
void crestLines::addSeparator( model<N,E> &mod, int crestLine, vector<FlowLine> &FlowLines )
{
    glm::vec3 vertex = vertices[ lines[crestLine] ];
    int tFlowline = -1;
    for( auto& tVertex : mod.vertices )
    {
        if( glm::length( vertex - tVertex ) != 0 &&
            (   tFlowline == -1 ||
                FlowLines[tFlowline].vertices.find( tVertex->index ) == FlowLines[tFlowline].vertices.end() ) )
            continue;
        if( tFlowline == -1 )
        {
            
        }
    }
}
