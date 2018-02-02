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
				insertLine(a, b, line1);
				insertLine(b, c, line1);
				insertLine(c, a, line1);
				vec3 normal = glm::normalize(glm::cross(
		        glm::vec3(vertices[b - 1]->pos) - glm::vec3(vertices[a - 1]->pos),
		        glm::vec3(vertices[c - 1]->pos) - glm::vec3(vertices[a - 1]->pos)));

				vertices[a - 1]->normal += normal; vertices[a - 1]->num_faces++;
				vertices[b - 1]->normal += normal; vertices[b - 1]->num_faces++;
				vertices[c - 1]->normal += normal; vertices[c - 1]->num_faces++;


                tris.push_back( Tri( a, b, c ) );
			}
			else if(args.size() == 5 && args[0] == "f")
			{
				sscanf(ln.c_str(), "%*s %d %d %d %d", &a, &b, &c, &d);

				insertLine(a, b, line1);
				insertLine(b, c, line2);
				insertLine(c, d, line3);
				insertLine(d, a, line4);
				vec3 normal = glm::normalize(glm::cross(
		        glm::vec3(vertices[b - 1]->pos) - glm::vec3(vertices[a - 1]->pos),
		        glm::vec3(vertices[c - 1]->pos) - glm::vec3(vertices[b - 1]->pos)));

				vertices[a - 1]->normal += normal; vertices[a - 1]->num_faces++;
				vertices[b - 1]->normal += normal; vertices[b - 1]->num_faces++;
				vertices[c - 1]->normal += normal; vertices[c - 1]->num_faces++;
				vertices[d - 1]->normal += normal; vertices[d - 1]->num_faces++;

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
	}
    for(int i = 0; i < lines.size(); i++)
	{
		lines[i]->index = i;
	}
}

template<typename V, typename L>
bool model<V,L>::insertVertex(Vertex* x)
{
    vertices.push_back(x);
	positions.push_back(x->pos);
}

template<typename V, typename L>
void model<V,L>::insertLine(int i, int j, Line *&line1)
{
    line1 = new Line;
	Vertex *a = vertices[i-1];
	Vertex *b = vertices[j-1];
    line1->vertices[0] = a;
    line1->vertices[1] = b;
    a->lines.push_back(line1);
    b->lines.push_back(line1);
    lines.push_back(line1);
	indices.push_back(i-1);
	indices.push_back(j-1);
}

template class model<float, float>;
