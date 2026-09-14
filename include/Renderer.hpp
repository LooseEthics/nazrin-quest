
#pragma once

#include <cstdint>
#include <SDL3/SDL.h>

#include "Mesh.hpp"

class Renderer
{
public:
  Renderer(int width, int height);
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  void clear() const;
  void draw(const Mesh& mesh) const;
  void present() const;

  [[nodiscard]]
  SDL_Window* window();

private:
  SDL_Window* window_;
  SDL_GLContext context_;

  uint32_t vertexArray_;
  uint32_t vertexBuffer_;
  uint32_t indexBuffer_;
  uint32_t shaderProgram_;

  int width_;
  int height_;
};
