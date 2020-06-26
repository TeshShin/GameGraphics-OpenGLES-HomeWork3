#include "object.h"

Object::Object(Program* program, Material* material,
        const vector<Vertex> &vertices, const vector<Index> &indices,
               GLenum primitive) : primitive(primitive) {

    create(program, material);
    load(vertices, indices);
}

Object::~Object() {
    destroy();
}

void Object::create(Program* program, Material* material) {
    this->program = program;
    this->material = material;

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    if(vbo == 0 || ibo == 0)
        LOG_PRINT_ERROR("Fail to generate buffers");

    if (glGetUniformLocation(program->get(), "worldMat") < 0)
        LOG_PRINT_WARN("Fail to get uniform location: %s", "worldMat");
}

void Object::load(const vector<Vertex> &vertices, const vector<Index> &indices) {
    LOG_PRINT_DEBUG("Load object data");

    this->vertices = vertices;
    this->indices = indices;

    // tangent();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices.size(),
            this->vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Index) * this->indices.size(),
                 this->indices.data(), GL_DYNAMIC_DRAW);
}


void Object::draw() const {
    if (material)
        material->update();

    GLint loc = glGetUniformLocation(program->get(), "worldMat");
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(worldMat));

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (const void *) offsetof(Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (const void *) offsetof(Vertex, nor));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (const void *) offsetof(Vertex, tex));

    // draw elements
    glDrawElements(primitive, (GLsizei) indices.size(), GL_UNSIGNED_SHORT, (const void *) 0);
}

void Object::destroy() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}