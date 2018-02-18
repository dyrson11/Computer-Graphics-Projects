template <class G>
class node
{
public:
    typedef typename G::n N;
    typedef typename G::Edge Edge;
    glm::vec3 points[2];
    int i1, i2;
    unordered_set <Edge*> m_edges;
    node(int a, int b, glm::vec3 va, glm::vec3 vb)
    {
        i1 = a;
        i2 = b;
        points[0] = va;
        points[1] = vb;
    }
};
