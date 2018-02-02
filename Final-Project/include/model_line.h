template <class G>
class line
{
public:
    typedef typename G::Vertex vertex;
    vertex* vertices[2];
    vector<line*> connections;
    int index;
};
