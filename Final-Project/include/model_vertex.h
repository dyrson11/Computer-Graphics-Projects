template <class G>
class vertex
{
public:
    typedef typename G::v V;
    typedef typename G::Line line;
    glm::vec3 pos;
    int num_faces;
    bool processed;
    glm::vec3 normal;
    vector <line*> lines;
    vertex(V x, V y, V z)
    {
        this->pos = glm::vec3(x, y, z);
        num_faces = 0;
        normal = glm::vec3(0, 0, 0);
    }
};
