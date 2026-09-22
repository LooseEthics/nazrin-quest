
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
#include "SpriteRenderer.hpp"
#include "Texture.hpp"

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
  void present() const;

  [[nodiscard]]
  SDL_Window* window() const;
  MapRenderer& mapRenderer() const;
  MazeRenderer& mazeRenderer() const;
  SpriteRenderer& spriteRenderer() const;

private:
  void initText();

  void destroyText();

  uint32_t windowWidth_;
  uint32_t windowHeight_;

  SDL_Window* window_;
  SDL_GLContext context_;

  std::unique_ptr<MazeRenderer> mazeRenderer_ = nullptr;
  std::unique_ptr<SpriteRenderer> spriteRenderer_ = nullptr;

  GLuint uiVertexArray_;
  GLuint uiVertexBuffer_;
  Shader uiShaderProgram_;

  GLint uiProjectionLocation_;
  GLint uiColorLocation_;

  glm::mat4 uiProjection_;

  std::unique_ptr<MapRenderer> mapRenderer_ = nullptr;
};
