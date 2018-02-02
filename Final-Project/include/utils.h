#define M_E         2.71828182845904523536028747135266250

float angleBetween(glm::vec3 a, glm::vec3 b, glm::vec3 origin)
{
    glm::vec3 da=glm::normalize(a-origin);
    glm::vec3 db=glm::normalize(b-origin);
    return glm::acos(glm::dot(da, db));
}

const vector<string> explode(const string& s, const char& c)
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

float matrix_graph[mod.lines.size()][mod.lines.size()];
float sigma1 = 5, sigma2 = 15, wn = -5;


void _fooInitGraph()
{

    //

    // iterate over quads

    for ( Quad tQuad : obj.quads )
    {

    }

}

void initGraph()
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
}
