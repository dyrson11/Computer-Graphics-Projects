template <class G>
class node
{
public:
    typedef typename G::n N;
    typedef typename G::Edge Edge;
    vec3 points[2];
    unordered_set <Edge*> m_edges;
    node(vec3 first, vec3 second)
    {
        points[0] = first;
        points[0] = second;
    }
};
