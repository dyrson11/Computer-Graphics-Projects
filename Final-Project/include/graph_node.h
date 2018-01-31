template <class G>
class node
{
public:
    typedef typename G::n N;
    typedef typename G::Edge Edge;
    N x, y;
    int index;
    unordered_set <Edge*> m_edges;
    int state;
    node(N x, N y)
    {
        this->x = x;
        this->y = y;
        state = 0;
    }
};
