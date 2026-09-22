
#pragma once

#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <memory>
#include <SDL3/SDL.h>
#include <string>

#include "Camera.hpp"
#include "MapRenderer.hpp"
#include "MazeRenderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

std::string readFile(const std::string& path);

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
  void present() const;

  void setGoalPosition(const glm::vec3 position);

  [[nodiscard]]
  SDL_Window* window() const;
  MapRenderer& mapRenderer() const;
  MazeRenderer& mazeRenderer() const;

private:
  void initMaze();
  void initSprite();
  void initText();

  void destroyMaze();
  void destroySprite();
  void destroyText();

  uint32_t windowWidth_;
  uint32_t windowHeight_;

  SDL_Window* window_;
  SDL_GLContext context_;

  std::unique_ptr<MazeRenderer> mazeRenderer_ = nullptr;

  Texture goalTexture_;
  glm::vec3 goalPosition_;

  GLuint spriteVertexArray_;
  GLuint spriteVertexBuffer_;
  Shader spriteShaderProgram_;

  GLint spriteProjectionLocation_;
  GLint spriteViewLocation_;
  GLint spriteTextureLocation_;

  GLuint uiVertexArray_;
  GLuint uiVertexBuffer_;
  Shader uiShaderProgram_;

  GLint uiProjectionLocation_;
  GLint uiColorLocation_;

  glm::mat4 uiProjection_;

  std::unique_ptr<MapRenderer> mapRenderer_ = nullptr;
};
