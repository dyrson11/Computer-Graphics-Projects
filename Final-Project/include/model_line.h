template <class G>
class line
{
public:
    typedef typename G::Vertex vertex;
    vertex* vertices[2];
    int a, b;
    vector<line*> connections;
    int index;
};
