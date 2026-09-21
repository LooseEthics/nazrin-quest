
#include <fstream>
#include <glad/gl.h>
#include <stdexcept>
#include <string>

#include <iostream>

#include "CommonGeometry.hpp"
#include "Renderer.hpp"

Renderer::Renderer(int width, int height)
  : width_(width),
    height_(height),
    window_(nullptr),
    context_(nullptr),
    mazeVertexArray_(0),
    mazeVertexBuffer_(0),
    mazeIndexBuffer_(0),
    spriteVertexArray_(0),
    spriteVertexBuffer_(0),
    uiVertexArray_(0),
    uiVertexBuffer_(0),
    mapVertexArray_(0),
    mapVertexBuffer_(0),
    mapMarkerVertexArray_(0),
    mapMarkerVertexBuffer_(0)
{
  if (!SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error(SDL_GetError());

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  window_ = SDL_CreateWindow(
    "Nazrin Quest",
    width_,
    height_,
    SDL_WINDOW_OPENGL
  );
  if (!window_) throw std::runtime_error(SDL_GetError());

  if (!SDL_SetWindowRelativeMouseMode(window_, true))
    throw std::runtime_error(SDL_GetError());

  context_ = SDL_GL_CreateContext(window_);
  if (!context_) throw std::runtime_error(SDL_GetError());

  if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress)))
    throw std::runtime_error("Failed to initialize OpenGL");

  glViewport(0, 0, width_, height_);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  initMaze();
  initSprite();
  initMap();
  initText();
}

Renderer::~Renderer()
{
  destroyText();
  destroyMap();
  destroySprite();
  destroyMaze();

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

void Renderer::present() const
{
  SDL_GL_SwapWindow(window_);
}

void Renderer::drawCamera(Camera& camera) const
{
  drawMaze(camera);
  drawSprite(
    goalTexture_.id(),
    goalPosition_,
    camera,
    CELL_SIZE / 2,
    CELL_SIZE / 2
  );
}

SDL_Window* Renderer::window() const
{
  return window_;
}
