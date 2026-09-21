
#include <fstream>
#include <glad/gl.h>
#include <sstream>
#include <stdexcept>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "CommonGeometry.hpp"
#include "Renderer.hpp"

std::string readFile(const std::string& path)
{
  std::ifstream file(path);

  if (!file)
    throw std::runtime_error("Failed to open shader: " + path);

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

Renderer::Renderer(int width, int height)
  : width_(width),
    height_(height),
    window_(nullptr),
    context_(nullptr),
    mazeVertexArray_(0),
    mazeVertexBuffer_(0),
    mazeIndexBuffer_(0),
    mazeShaderProgram_(0),
    wallTexture_(0),
    floorTexture_(0),
    goalTexture_(0),
    spriteVertexArray_(0),
    spriteVertexBuffer_(0),
    spriteShaderProgram_(0),
    uiVertexArray_(0),
    uiVertexBuffer_(0),
    uiShaderProgram_(0)
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
    goalTexture_,
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

GLuint Renderer::compileShader(
  GLenum type,
  const char* source
) const
{
  const GLuint shader = glCreateShader(type);

  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success){
    GLint logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    std::string log(logLength, '\0');

    glGetShaderInfoLog(
      shader,
      logLength,
      nullptr,
      log.data()
    );

    glDeleteShader(shader);

    throw std::runtime_error("Shader compilation failed:\n" + log);
  }

  return shader;
}

GLuint Renderer::createShaderProgram(
  const std::string& vertexShaderPath,
  const std::string& fragmentShaderPath
) const
{
  const std::string vertexShaderSource = readFile(vertexShaderPath);
  const std::string fragmentShaderSource = readFile(fragmentShaderPath);
  const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
  const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());
  const GLuint program = glCreateProgram();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLint success = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success){
    GLint logLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

    std::string log(logLength, '\0');

    glGetProgramInfoLog(
      program,
      logLength,
      nullptr,
      log.data()
    );

    glDeleteProgram(program);

    throw std::runtime_error("Shader linking failed:\n" + log);
  }

  return program;
}

GLuint Renderer::loadTexture(const std::string& path) const
{
  int width;
  int height;
  int channels;

  unsigned char* pixels = stbi_load(
    path.c_str(),
    &width,
    &height,
    &channels,
    STBI_rgb_alpha
  );

  if (!pixels)
    throw std::runtime_error("Failed to load texture: " + path);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MIN_FILTER,
    GL_NEAREST_MIPMAP_LINEAR
  );

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MAG_FILTER,
    GL_NEAREST
  );

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_WRAP_S,
    GL_REPEAT
  );

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_WRAP_T,
    GL_REPEAT
  );

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA8,
    width,
    height,
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    pixels
  );

  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(pixels);

  return texture;
}
