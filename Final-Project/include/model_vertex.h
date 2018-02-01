template <class G>
class vertex
{
public:
    typedef typename G::v V;
    typedef typename G::Line line;
    vec3 pos;
    int num_faces;
    vec3 normal;
    vector <line*> lines;
    vertex(V x, V y, V z)
    {
        this->pos = vec3(x, y, z);
        num_faces = 0;
        normal = vec3(0, 0, 0);
    }
};
