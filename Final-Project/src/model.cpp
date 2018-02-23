#include "../include/model.h"
#include "../include/utils.h"

template<typename V, typename L>
void model<V,L>::clear_model()
{
    vertices.clear();
    lines.clear();
	indices.clear();

    quads.clear();
    tris.clear();
}

template<typename V, typename L>
void model<V,L>::load_model(const char * dir)
{
    clear_model();
    if(freopen(dir, "r", stdin))
	{
		string ln;
		vector <string> args;
		float x, y, z;
		int a, b, c, d;
		Line *line1, *line2, *line3, *line4;
		while(getline(cin, ln))
		{
			args = explode(ln, ' ');
			if(args[0] == "v")
			{
				sscanf(ln.c_str(), "%*s %f %f %f", &x, &y, &z);
				Vertex *vert = new Vertex(x, y, z);
				insertVertex(vert);
			}
			else if(args.size() == 4 && args[0] == "f")
			{
				sscanf(ln.c_str(), "%*s %d %d %d", &a, &b, &c);
                a--;
                b--;
                c--;
				insertLine(a, b, line1);
				insertLine(b, c, line1);
				insertLine(c, a, line1);
				glm::vec3 normal = glm::normalize(glm::cross(
		        glm::vec3(vertices[b]->pos) - glm::vec3(vertices[a]->pos),
		        glm::vec3(vertices[c]->pos) - glm::vec3(vertices[a]->pos)));

				vertices[a]->normal += normal; vertices[a]->num_faces++;
				vertices[b]->normal += normal; vertices[b]->num_faces++;
				vertices[c]->normal += normal; vertices[c]->num_faces++;


                tris.push_back( Tri( a, b, c ) );
			}
			else if(args.size() == 5 && args[0] == "f")
			{
				sscanf(ln.c_str(), "%*s %d %d %d %d", &a, &b, &c, &d);
                a--;
                b--;
                c--;
                d--;
				insertLine(a, b, line1);
				insertLine(b, c, line2);
				insertLine(c, d, line3);
				insertLine(d, a, line4);
				glm::vec3 normal = glm::normalize(glm::cross(
		        glm::vec3(vertices[b]->pos) - glm::vec3(vertices[a]->pos),
		        glm::vec3(vertices[d]->pos) - glm::vec3(vertices[a]->pos)));
				vertices[a]->normal += normal; vertices[a]->num_faces++;

                normal = glm::normalize(glm::cross(
		        glm::vec3(vertices[c]->pos) - glm::vec3(vertices[b]->pos),
		        glm::vec3(vertices[a]->pos) - glm::vec3(vertices[b]->pos)));
				vertices[b]->normal += normal; vertices[b]->num_faces++;

                normal = glm::normalize(glm::cross(
		        glm::vec3(vertices[d]->pos) - glm::vec3(vertices[c]->pos),
		        glm::vec3(vertices[b]->pos) - glm::vec3(vertices[c]->pos)));
				vertices[c]->normal += normal; vertices[c]->num_faces++;

                normal = glm::normalize(glm::cross(
		        glm::vec3(vertices[a]->pos) - glm::vec3(vertices[d]->pos),
		        glm::vec3(vertices[c]->pos) - glm::vec3(vertices[d]->pos)));
				vertices[d]->normal += normal; vertices[d]->num_faces++;

				line1->connections.push_back(line3);
				line2->connections.push_back(line4);

                quads.push_back( Quad( a, b, c, d ) );
			}
		}
	}
	computeNormals();
}

template<typename V, typename L>
void model<V,L>::computeNormals()
{
    for(int i = 0; i < vertices.size(); i++)
	{
		vertices[i]->normal.x = vertices[i]->normal.x / vertices[i]->num_faces;
		vertices[i]->normal.y = vertices[i]->normal.y / vertices[i]->num_faces;
		vertices[i]->normal.z = vertices[i]->normal.z / vertices[i]->num_faces;
        //cout<<vertices[i]->normal.x<<" "<<vertices[i]->normal.y<<" "<<vertices[i]->normal.z<<"\n";
	}
    for(int i = 0; i < lines.size(); i++)
	{
		lines[i]->index = i;
	}
}

template<typename V, typename L>
bool model<V,L>::insertVertex(Vertex* x)
{
    x->index = vertices.size();
    vertices.push_back(x);
	positions.push_back(x->pos);
}

template<typename V, typename L>
void model<V,L>::insertLine(int i, int j, Line *&line1)
{
    //itLine = insertedLines.find( make_pair( i, j ) );
    if(i > j)
        swap(i, j);

    if( insertedLines.find( make_pair( i, j ) ) == insertedLines.end() )
    {
        line1 = new Line;
        Vertex *a = vertices[i];
        Vertex *b = vertices[j];
        line1->a = i;
        line1->b = j;
        line1->vertices[0] = a;
        line1->vertices[1] = b;
        a->lines.push_back(line1);
        b->lines.push_back(line1);
        lines.push_back(line1);
        insertedLines.insert( make_pair( make_pair( i, j ), line1 ) );
        indices.push_back(i);
        indices.push_back(j);
    }
    else
    {
        line1 = insertedLines.find( make_pair( i, j ) )->second;
    }
}

template<typename V, typename L>
void model<V,L>::updateModel()
{
    indices.clear();
    ids.clear();
    positions.clear();
    for( Line* tLine : lines )
    {
        indices.push_back(tLine->a);
        indices.push_back(tLine->b);
        ids.push_back( tLine->clusterID );
        ids.push_back( tLine->clusterID );
        positions.push_back( tLine->vertices[0]->pos );
        positions.push_back( tLine->vertices[1]->pos );
    }
}
template<typename V, typename L>
void model<V,L>::updateModel( int cluster )
{
    indices.clear();
    ids.clear();
    positions.clear();
    for( Line* tLine : lines )
    {
        if( tLine->clusterID != cluster )
            continue;
        indices.push_back(tLine->a);
        indices.push_back(tLine->b);
        ids.push_back( tLine->clusterID );
        ids.push_back( tLine->clusterID );
        positions.push_back( tLine->vertices[0]->pos );
        positions.push_back( tLine->vertices[1]->pos );
    }
}

template<typename V, typename L>
void model<V,L>::updateModelFlowlines()
{
    indices.clear();
    ids.clear();
    positions.clear();
    for( Line* tLine : lines )
    {
        ids.push_back( tLine->flowLineID );
        ids.push_back( tLine->flowLineID );
        indices.push_back(tLine->a);
        indices.push_back(tLine->b);
        positions.push_back( tLine->vertices[0]->pos );
        positions.push_back( tLine->vertices[1]->pos );
    }
}

template<typename V, typename L>
void model<V,L>::updateModelFlowlines( int FlowLine )
{
    indices.clear();
    ids.clear();
    positions.clear();
    for( Line* tLine : lines )
    {
        if( tLine->flowLineID != FlowLine )
            continue;
        ids.push_back( FlowLine );
        ids.push_back( FlowLine );
        indices.push_back( tLine->a );
        indices.push_back( tLine->b );
        positions.push_back( tLine->vertices[0]->pos );
        positions.push_back( tLine->vertices[1]->pos );
    }
}

template class model<float, float>;
