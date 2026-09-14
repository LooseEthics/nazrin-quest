
#include <glad/gl.h>
#include <stdexcept>

#include "Renderer.hpp"

Renderer::Renderer(int width, int height)
{
  if (!SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error(SDL_GetError());

  window_ = SDL_CreateWindow(
    "Nazrin Quest",
    width,
    height,
    SDL_WINDOW_OPENGL
  );
  if (!window_) throw std::runtime_error(SDL_GetError());

  context_ = SDL_GL_CreateContext(window_);
  if (!context_) throw std::runtime_error(SDL_GetError());

  if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress)))
    throw std::runtime_error("Failed to initialize OpenGL");

  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);
  glGenVertexArrays(1, &vertexArray_);
  glGenBuffers(1, &vertexBuffer_);
  glGenBuffers(1, &indexBuffer_);
}

Renderer::~Renderer()
{
  glDeleteBuffers(1, &indexBuffer_);
  glDeleteBuffers(1, &vertexBuffer_);
  glDeleteVertexArrays(1, &vertexArray_);

  SDL_GL_DestroyContext(context_);
  SDL_DestroyWindow(window_);

  SDL_Quit();
}

void Renderer::clear() const
{
  glClearColor(
    0.1f,
    0.1f,
    0.1f,
    1.0f
  );

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::present() const
{
  SDL_GL_SwapWindow(window_);
}

void Renderer::draw(const Mesh& mesh) const
{
  glBindVertexArray(vertexArray_);

  glBindBuffer(
    GL_ARRAY_BUFFER,
    vertexBuffer_
  );

  glBufferData(
    GL_ARRAY_BUFFER,
    mesh.vertices.size() * sizeof(Vertex),
    mesh.vertices.data(),
    GL_STATIC_DRAW
  );

  glBindBuffer(
    GL_ELEMENT_ARRAY_BUFFER,
    indexBuffer_
  );

  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    mesh.indices.size() * sizeof(uint32_t),
    mesh.indices.data(),
    GL_STATIC_DRAW
  );

  glDrawElements(
    GL_TRIANGLES,
    static_cast<GLsizei>(mesh.indices.size()),
    GL_UNSIGNED_INT,
    nullptr
  );
}
