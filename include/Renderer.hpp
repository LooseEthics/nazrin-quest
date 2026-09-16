
#pragma once

#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <SDL3/SDL.h>
#include <string>

#include "Camera.hpp"
#include "Mesh.hpp"

class Renderer
{
public:
  Renderer(
    int width,
    int height
  );
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  void clear() const;
  void drawCamera(Camera& camera) const;
  void drawMaze(Camera& camera) const;
  void drawText(
    const std::string& text,
    const glm::vec2& position,
    float scale,
    const glm::vec3& color
  ) const;
  void drawSprite(
    GLuint texture,
    const glm::vec3 position,
    Camera& camera,
    float width,
    float height
  ) const;
  void drawMap(Camera& camera) const;
  void present() const;

  void uploadMesh(const Mesh& mesh);
  void setGoalPosition(const glm::vec3 position);

  [[nodiscard]]
  SDL_Window* window() const;

private:
  uint32_t width_;
  uint32_t height_;

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

  GLuint createShaderProgram(
    const std::string& vertexShaderPath,
    const std::string& fragmentShaderPath
  ) const;

  GLsizei wallIndexCount_;
  GLsizei floorIndexCount_;
  GLintptr wallIndexOffset_;
  GLintptr floorIndexOffset_;

  GLuint loadTexture(const std::string& path) const;

  GLuint wallTexture_;
  GLuint floorTexture_;
  GLuint goalTexture_;
  glm::vec3 goalPosition_;
  GLint textureLocation_;

  GLuint spriteVertexArray_;
  GLuint spriteVertexBuffer_;
  GLuint spriteShaderProgram_;

  GLint spriteProjectionLocation_;
  GLint spriteViewLocation_;
  GLint spriteTextureLocation_;

  GLuint uiVertexArray_;
  GLuint uiVertexBuffer_;
  GLuint uiShaderProgram_;

  GLuint uiProjectionLocation_;
  GLuint uiColorLocation_;

  glm::mat4 uiProjection_;
};
