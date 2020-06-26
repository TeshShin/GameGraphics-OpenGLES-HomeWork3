#ifndef HW3_PROGRAM_H
#define HW3_PROGRAM_H

#include "global.h"
#include "shader.h"

class Program {
public:
    Program(Shader* vertexShader, Shader* fragmentShader);
    virtual ~Program();
    virtual GLuint get() const { return program; }
    virtual void use() const;

private:
    GLuint program;

    virtual void create();
    virtual void attach(Shader* vertexShader, Shader* fragmentShader);
    virtual void link() const;
    virtual void destroy();
};

#endif //HW3_PROGRAM_H
