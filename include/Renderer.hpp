
#pragma once

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <SDL3/SDL.h>

#include "Camera.hpp"
#include "Mesh.hpp"

class Renderer
{
public:
  Renderer(
    int width,
    int height,
    Camera& camera
  );
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  void clear() const;
  void draw(
    const Mesh& mesh,
    GLenum primitiveType,
    const glm::vec3& color
  ) const;
  void present() const;

  [[nodiscard]]
  SDL_Window* window() const;

private:
  SDL_Window* window_;
  SDL_GLContext context_;

  uint32_t vertexArray_;
  uint32_t vertexBuffer_;
  uint32_t indexBuffer_;
  uint32_t shaderProgram_;

  glm::mat4 projection_;
  glm::mat4 view_;

  uint32_t compileShader(
    uint32_t type,
    const char* source
  ) const;

  uint32_t createShaderProgram() const;

  Camera& camera_;
};
