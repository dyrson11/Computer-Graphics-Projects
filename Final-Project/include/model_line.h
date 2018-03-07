template <class G>
class line
{
public:
    typedef typename G::Vertex vertex;
    vertex* vertices[2];
    int a, b;
    vector<line*> connections;
    int index, clusterID, flowLineID, strandID;
    bool isInFlowline;
    line()
    {
        clusterID = -1;
        flowLineID = -1;
        isInFlowline = false;
    }
};
