#pragma once

// #include <glad/glad.h>
#include <array>
#include "common.hpp"

#define GL_UNSIGNED_BYTE 0x1401
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_FRAMEBUFFER 0x8D40
#define GL_LINEAR 0x2601

#define MAX_COLOR_ATTACHMENTS 7
#define MAX_RENDERBUFFER_ATTACHMENTS 7

// NOTE: just provide width / height
// Automatically multiplied by 2 because of macos
struct ColorAttachmentCreateInfo {
    int format;
    int type = GL_UNSIGNED_BYTE;
    int min_texture_filter = GL_LINEAR;
    int mag_texture_filter = GL_LINEAR;
};

struct RenderbufferAttachmentCreateInfo {
    int format = GL_DEPTH24_STENCIL8;
};

class Framebuffer {
public:
    Framebuffer(uint width, uint height);
    ~Framebuffer();

    void bind(int target = GL_FRAMEBUFFER);
    void unbind();

    uint width() const;
    uint height() const;
    uint id();

    const std::array<uint, MAX_COLOR_ATTACHMENTS>& color_attachments();
    const std::array<uint, MAX_RENDERBUFFER_ATTACHMENTS>& renderbuffer_attachments();
    uint n_used_color_attachments() const;
    uint n_used_renderbuffer_attachments() const;

    void create_color_attachment(const ColorAttachmentCreateInfo& cinfo);
    void create_render_buffer_attachment(const RenderbufferAttachmentCreateInfo& cinfo);
    bool is_complete() const;

private:
    uint _width;
    uint _height;
    uint _id;
    int _target = GL_FRAMEBUFFER;

    std::array<uint, MAX_COLOR_ATTACHMENTS> _color_attachments;
    uint _n_color_attachments = 0;

    std::array<uint, MAX_RENDERBUFFER_ATTACHMENTS> _render_buffer_attachments;
    uint _n_renderbuffer_attachments= 0;

    void create_framebuffer();
};

