template <class G>
class vertex
{
public:
    typedef typename G::v V;
    typedef typename G::Line line;
    V x, y, z;
    int index;
    vector <Line*> lines;
    CNode(V x, V y, V z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};
