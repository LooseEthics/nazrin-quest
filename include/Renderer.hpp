
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
#include "TextRenderer.hpp"
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
  void present() const;

  [[nodiscard]]
  SDL_Window* window() const;
  MapRenderer& mapRenderer() const;
  MazeRenderer& mazeRenderer() const;
  SpriteRenderer& spriteRenderer() const;
  TextRenderer& textRenderer() const;

private:
  void initText();

  void destroyText();

  uint32_t windowWidth_;
  uint32_t windowHeight_;

  SDL_Window* window_;
  SDL_GLContext context_;

  std::unique_ptr<MazeRenderer> mazeRenderer_ = nullptr;
  std::unique_ptr<SpriteRenderer> spriteRenderer_ = nullptr;
  std::unique_ptr<TextRenderer> textRenderer_ = nullptr;
  std::unique_ptr<MapRenderer> mapRenderer_ = nullptr;
};
