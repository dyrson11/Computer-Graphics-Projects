template <class G>
class edge
{
public:
    typedef typename G::Vertex vertex;
    vertex* vertices[2];
};
