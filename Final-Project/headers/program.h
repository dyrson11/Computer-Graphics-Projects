#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

class program
{
public:
    GLuint id;

    void loadShaders(const char *, const char *);
};
