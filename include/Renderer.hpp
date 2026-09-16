
#pragma once

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
  void draw() const;
  void present() const;
  void uploadMesh(const Mesh& mesh);

  [[nodiscard]]
  SDL_Window* window() const;

  void setFaceColor(const glm::vec3& color);
  void setEdgeRendering(bool value);
  void setEdgeColor(const glm::vec3& color);

private:
  SDL_Window* window_;
  SDL_GLContext context_;

  GLuint vertexArray_;
  GLuint vertexBuffer_;
  GLuint indexBuffer_;
  GLuint shaderProgram_;

  GLint projectionLocation_;
  GLint viewLocation_;
  GLint colorLocation_;

  glm::mat4 projection_;

  GLuint compileShader(
    GLenum type,
    const char* source
  ) const;

  GLuint createShaderProgram() const;

  Camera& camera_;

  GLsizei triIndexCount_;
  GLsizei edgeIndexCount_;
  GLintptr edgeIndexOffset_;

  glm::vec3 faceColor_;
  bool renderEdges_;
  glm::vec3 edgeColor_;
};
