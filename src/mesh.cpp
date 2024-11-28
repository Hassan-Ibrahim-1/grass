#include <cstddef>
#include <glad/glad.h>
#include "mesh.hpp"
#include "debug.hpp"
#include "texture2d.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> indices)
    : vertices(vertices), indices(indices) {}

Mesh::Mesh(const Mesh& mesh)
    : draw_command(mesh.draw_command),
      _vao(mesh._vao),
      _buffers_created(mesh._buffers_created),
      _vao_ready(mesh._vao_ready),
      vertices(mesh.vertices),
      indices(mesh.indices) {}

Mesh::~Mesh() {
    if (_buffers_created) {
        delete_buffers();
    }
    /*else {*/
    /*    reset_vao();*/
    /*}*/
}

uint Mesh::vao() const {
    ASSERT(_vao_ready || _buffers_created, "Create buffers or set a custom VAO before calling this");
    return _vao;
}

bool Mesh::ready() const {
    return _vao_ready || _buffers_created;
}

bool Mesh::buffers_created() const {
    return _buffers_created;
}
bool Mesh::custom_vao_set() const {
    return _vao_ready;
}

void Mesh::create_buffers() {
    ASSERT(!_vao_ready, "Trying to create custom buffers when a separate VAO is set. Call reset_vao before calling this");
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Tex coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    glBindVertexArray(0);

    draw_command.type = DrawCommandType::DRAW_ELEMENTS;
    draw_command.mode = DrawCommandMode::TRIANGLES;
    draw_command.count = indices.size();

    _buffers_created = true;
}

void Mesh::set_vao(uint vao) {
    ASSERT(!_buffers_created, "Mesh's own buffers already created. Trying to set a custom VAO. ");
    _vao = vao;
    _vao_ready = true;
}

void Mesh::reset_vao() {
    ASSERT(!_buffers_created, "Trying to reset the VAO of mesh's own buffers. Call delete_buffers instead");
    _vao = 0;
    _vao_ready = false;
}

void Mesh::delete_buffers() {
    ASSERT(_buffers_created, "Tried to delete a VAO that is not created by this mesh.");
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_ebo);
    glDeleteBuffers(1, &_vbo);
    _buffers_created = false;
}

