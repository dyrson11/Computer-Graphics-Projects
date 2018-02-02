template <class G>
class edge
{
public:
    typedef typename G::Node Node;
    float weight;
    Node* m_nodes[2];
};
