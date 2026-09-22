
#include <fstream>
#include <glad/gl.h>
#include <stdexcept>
#include <string>

#include <iostream>

#include "CommonGeometry.hpp"
#include "Renderer.hpp"

Renderer::Renderer(int width, int height)
  : windowWidth_(width),
    windowHeight_(height),
    window_(nullptr),
    context_(nullptr)
{
  if (!SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error(SDL_GetError());

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  window_ = SDL_CreateWindow(
    "Nazrin Quest",
    windowWidth_,
    windowHeight_,
    SDL_WINDOW_OPENGL
  );
  if (!window_) throw std::runtime_error(SDL_GetError());

  if (!SDL_SetWindowRelativeMouseMode(window_, true))
    throw std::runtime_error(SDL_GetError());

  context_ = SDL_GL_CreateContext(window_);
  if (!context_) throw std::runtime_error(SDL_GetError());

  if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress)))
    throw std::runtime_error("Failed to initialize OpenGL");

  glViewport(0, 0, windowWidth_, windowHeight_);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  mapRenderer_ = std::make_unique<MapRenderer>(windowWidth_, windowHeight_);
  mazeRenderer_ = std::make_unique<MazeRenderer>(windowWidth_, windowHeight_);
  spriteRenderer_ = std::make_unique<SpriteRenderer>(windowWidth_, windowHeight_);
  textRenderer_ = std::make_unique<TextRenderer>(windowWidth_, windowHeight_);
}

Renderer::~Renderer()
{

  if (context_) SDL_GL_DestroyContext(context_);
  if (window_) SDL_DestroyWindow(window_);

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

void Renderer::drawCamera(Camera& camera) const
{
  mazeRenderer_->drawMaze(camera);
  spriteRenderer_->drawSprite(
    spriteRenderer_->goalTexture_,
    spriteRenderer_->goalPosition_,
    camera,
    CELL_SIZE / 2,
    CELL_SIZE / 2
  );
}

void Renderer::present() const
{
  SDL_GL_SwapWindow(window_);
}

SDL_Window* Renderer::window() const
{
  return window_;
}

MapRenderer& Renderer::mapRenderer() const {return *mapRenderer_;}
MazeRenderer& Renderer::mazeRenderer() const {return *mazeRenderer_;}
SpriteRenderer& Renderer::spriteRenderer() const {return *spriteRenderer_;}
TextRenderer& Renderer::textRenderer() const {return *textRenderer_;}
