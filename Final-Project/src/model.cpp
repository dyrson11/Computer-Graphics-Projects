#include "../headers/model.h"

const vector<string> explode(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;

	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);

	return v;
}

template<typename N, typename E>
void model<N,E>::clear_model()
{
    vertices.clear();
    lines.clear();
}

template<typename N, typename E>
void model<N,E>::read_model(string dir)
{
    clear_model();
    freopen(dir.c_str(), "r", stdin);
    string line;
    vector <string> args;
    float x, y, z;
    int a, b, c
    while(getline(cin, line))
    {
        args = explode(line, ' ');
        if(args[0] == 'v')
        {
            sscanf(line, "%*s %f %f %f", x, y, z);
            Vertex *vert = new Vertex(x, y, z);

        }
        else if(args.size() == 4)
        {
            sscanf(line, "%*s %a %b %c", a, b, c);
            indices.push_back(index);
        }
    }
}

template<typename N, typename E>
bool graph<N,E>::insertVertex(Node* x)
{
    m_nodes_list.push_back(x);
}

template<typename N, typename E>
bool graph<N,E>::insertLine(Node *a, Node *b)
{
    Edge *edge1 = new Edge;
    edge1->m_nodes[0] = a;
    edge1->m_nodes[1] = b;
    a->m_edges.push_back(edge1);
    b->m_edges.push_back(edge1);
    m_edges_list.push_back(edge1);
}
