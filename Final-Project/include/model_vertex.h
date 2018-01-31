template <class G>
class vertex
{
public:
    typedef typename G::v V;
    typedef typename G::Line line;
    vec3 pos;
    int index;
    vector <line*> lines;
    vertex(V x, V y, V z)
    {
        this->pos = vec3(x, y, z);
    }
};
