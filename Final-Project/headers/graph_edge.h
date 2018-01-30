template <class G>
class edge
{
public:
    typedef typename G::Node Node;
    Node* m_nodes[2];
};
