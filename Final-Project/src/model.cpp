#include "../include/model.h"

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

template<typename V, typename L>
void model<V,L>::clear_model()
{
    vertices.clear();
    lines.clear();
	indices.clear();
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
				insertLine(a, b);
				insertLine(b, c);
				insertLine(c, a);
			}
			else if(args.size() == 5 && args[0] == "f")
			{
				sscanf(ln.c_str(), "%*s %d %d %d %d", &a, &b, &c, &d);
				insertLine(a, b);
				insertLine(b, c);
				insertLine(c, d);
				insertLine(d, a);
			}
		}
	}
}

template<typename V, typename L>
bool model<V,L>::insertVertex(Vertex* x)
{
    vertices.push_back(x);
	positions.push_back(x->pos);
}

template<typename V, typename L>
bool model<V,L>::insertLine(int i, int j)
{
    Line *line1 = new Line;
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
