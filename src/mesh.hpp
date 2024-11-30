#pragma once

#include <vector>
#include "shader.hpp"
#include "texture2d.hpp"
#include "vertex.hpp"
#include "draw_command.hpp"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    DrawCommand draw_command;

    Mesh() {}
    Mesh(std::vector<Vertex> vertices, std::vector<uint> indices);
    Mesh(const Mesh& mesh);
    ~Mesh();
    
    uint vao() const;
    // NOTE: returns whether the mesh is ready to render
    // if not call Mesh::create_buffers
    bool ready() const;
    bool buffers_created() const;
    bool custom_vao_set() const;

    void create_buffers();
    void delete_buffers();
    // NOTE: Only call this if using a custom VAO
    void set_vao(uint vao);
    // NOTE: unsets the vao set using set_vao
    void reset_vao();

private:
    uint _vao;
    uint _vbo;
    uint _ebo;
    // true if set_vao is called
    bool _vao_ready = false;
    // true if create_buffers is called
    bool _buffers_created = false;
};

