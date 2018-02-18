#pragma once
#ifndef UTIL_H_
#define UTIL_H_

inline vector<string> explode(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;

	for(auto n:s)
	{
		if(n != c) buff+=n;
        else if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);

	return v;
}

#endif /* UTIL_H_ */

/*void _fooInitGraph()
{
    float matrix_graph[mod.lines.size()][mod.lines.size()];
    float sigma1 = 5, sigma2 = 15, wn = -5;
    unordered_set< pair<int, int> > added_edges;
    int a, b, c, d;
    bool swapped;
    vec3 origin, tEnd1, tEdn2;
    Node *ab, *bc, *ac;
    // iterate over tris
    for ( Tri tTri : mod.tris)
    {
        a = tTri.i1;
        b = tTri.i2;
        c = tTri.i3;
        if( nodes.find( make_pair( min(a, b), max(a, b) ) ) == added_edges.end() )
        {
            if(a > b)
            {
                swap(a, b);
                swapped = true;
            }
            ab = new Node(a, b, mod.vertices[a]->pos, mod.vertices[b]->pos);
            nodes.insert( make_pair( make_pair(a, b), ab ) );
            if(swapped)
            {
                swap(a, b);
                swapped = false;
            }
        }
        else
        {
            ab = nodes[make_pair( min(a, b), max(a, b) )];
        }

        if( nodes.find( make_pair( min(b, c), max(b, c) ) ) == added_edges.end() )
        {
            if(b > c)
            {
                swap(b, c);
                swapped = true;
            }
            bc = new Node(b, c, mod.vertices[b]->pos, mod.vertices[c]->pos);
            nodes.insert( make_pair( make_pair(b, c) ), bc );
            if(swapped)
            {
                swap(b, c);
                swapped = false;
            }
        }
        else
        {
            bc = nodes[make_pair( min(b, c), max(b, c) )];
        }

        if( nodes.find( make_pair( min(b, c), max(b, c) ) ) == added_edges.end() )
        {
            if(a > c)
            {
                swap(b, c);
                swapped = true;
            }
            ac = new Node(a, c, mod.vertices[a]->pos, mod.vertices[c]->pos);
            nodes.insert( make_pair( make_pair(a, c) ), ac );
            if(swapped)
            {
                swap(a, c);
                swapped = false;
            }
        }
        else
        {
            ac = nodes[make_pair( min(a, c), max(a, c) )];
        }

        //compute ab node and neighbor ac
        origin = mod.vertices[a]->pos;
        tEnd1 = mod.vertices[b]->pos;
        tEnd1 = mod.vertices[c]->pos;
        weight =

    }

    for ( Quad tQuad : obj.quads )
    {

        vec3 origin =

    }

}*/

/*void initGraph()
{
    for(int i = 0; i < obj.lines.size(); i++)
    {
        for(int j = 0; j < obj.lines[i]->vertices[0]->lines.size(); j++)
        {
            if(obj.lines[i]->vertices[0]->lines[j] != obj->lines[i])
            {
                vec3 origin = obj.lines[i]->vertices[0]->pos;
                vec3 a = obj.lines[i]->vertices[0]->lines[j]->vertices[obj.lines[i]->vertices[0]->lines[j]->vertices[0]->pos == origin];
                vec3 b = obj.lines[i]->vertices[0]->normal + origin;

                vec3 angle = abs(angleBetween(a, b, origin)) - 90;

                float arg = ( angle <= 45 ) ? ( angle / sigma1 ) : ( ( 90 - angle ) / sigma2 );

                matrix_graph[i][obj.lines[i]->vertices[1]->lines[j]->index] = exp( - arg * arg );
                matrix_graph[obj.lines[i]->vertices[1]->lines[j]->index][i] = exp( - arg * arg );


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
}*/
