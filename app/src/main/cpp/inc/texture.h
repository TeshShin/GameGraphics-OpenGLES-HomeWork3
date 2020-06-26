#ifndef HW3_TEXTURE_H
#define HW3_TEXTURE_H

#include "global.h"
#include "program.h"
#include "texel.h"

class Texture {
public:
    Texture(Program* program, const int number, const string name,
        const vector<Texel>& data, const GLsizei size);
    virtual ~Texture(void);

    virtual void update(void) const;

private:
    GLuint id;
    Program* program;
    int number;
    string name;
    virtual void create(Program* program, const int number, const string name);
    virtual void load(const vector<Texel>& data, const GLsizei size);
    virtual void destroy(void);
};

#endif //HW3_TEXTURE_H
