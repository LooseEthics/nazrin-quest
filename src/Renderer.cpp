
#include <cstddef>
#include <fstream>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "CommonGeometry.hpp"
#include "Glyphs.hpp"
#include "Renderer.hpp"


namespace
{
  constexpr const char* VERTEX_SHADER_PATH = "shaders/maze.vert";
  constexpr const char* FRAGMENT_SHADER_PATH = "shaders/maze.frag";
  constexpr const char* SPRITE_VERTEX_SHADER_PATH = "shaders/sprite.vert";
  constexpr const char* SPRITE_FRAGMENT_SHADER_PATH = "shaders/sprite.frag";
  constexpr const char* UI_VERTEX_SHADER_PATH = "shaders/ui.vert";
  constexpr const char* UI_FRAGMENT_SHADER_PATH = "shaders/ui.frag";

  std::string readFile(const std::string& path)
  {
    std::ifstream file(path);

    if (!file)
      throw std::runtime_error("Failed to open shader: " + path);

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
  }
}

Renderer::Renderer(int width, int height)
  : width_(width),
    height_(height),
    window_(nullptr),
    context_(nullptr),
    vertexArray_(0),
    vertexBuffer_(0),
    indexBuffer_(0),
    shaderProgram_(0),
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


  // maze rendering
  glGenVertexArrays(1, &vertexArray_);
  glGenBuffers(1, &vertexBuffer_);
  glGenBuffers(1, &indexBuffer_);

  shaderProgram_ = createShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

  projectionLocation_ = glGetUniformLocation(shaderProgram_, "projection");
  viewLocation_ = glGetUniformLocation(shaderProgram_, "view");
  textureLocation_ = glGetUniformLocation(shaderProgram_, "textureSampler");

  projection_ = glm::perspective(
    glm::radians(80.0f),
    static_cast<float>(width_) / static_cast<float>(height_),
    0.01f,
    500.0f
  );

  glBindVertexArray(vertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);

  glVertexAttribPointer(
    0,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    nullptr
  );

  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    reinterpret_cast<void*>(offsetof(Vertex, texCoord))
  );

  glEnableVertexAttribArray(1);


  // sprite rendering
  glGenVertexArrays(1, &spriteVertexArray_);
  glGenBuffers(1, &spriteVertexBuffer_);

  spriteShaderProgram_ = createShaderProgram(SPRITE_VERTEX_SHADER_PATH, SPRITE_FRAGMENT_SHADER_PATH);

  spriteProjectionLocation_ = glGetUniformLocation(spriteShaderProgram_, "projection");
  spriteViewLocation_ = glGetUniformLocation(spriteShaderProgram_, "view");
  spriteTextureLocation_ = glGetUniformLocation(spriteShaderProgram_, "textureSampler");

  glBindVertexArray(spriteVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, spriteVertexBuffer_);

  glVertexAttribPointer(
    0,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    nullptr
  );

  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    reinterpret_cast<void*>(offsetof(Vertex, texCoord))
  );

  glEnableVertexAttribArray(1);


  // textures
  wallTexture_ = loadTexture("assets/tex_wall.png");
  floorTexture_ = loadTexture("assets/tex_floor.png");
  goalTexture_ = loadTexture("assets/cheese.png");


  // text rendering
  glGenVertexArrays(1, &uiVertexArray_);
  glGenBuffers(1, &uiVertexBuffer_);

  uiShaderProgram_ = createShaderProgram(UI_VERTEX_SHADER_PATH, UI_FRAGMENT_SHADER_PATH);

  uiProjectionLocation_ = glGetUniformLocation(uiShaderProgram_, "projection");
  uiColorLocation_ = glGetUniformLocation(uiShaderProgram_, "color");

  uiProjection_ = glm::ortho(
    0.0f,
    static_cast<float>(width_),
    static_cast<float>(height_),
    0.0f
  );

  glBindVertexArray(uiVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, uiVertexBuffer_);

  glVertexAttribPointer(
    0,
    2,
    GL_FLOAT,
    GL_FALSE,
    2 * sizeof(float),
    nullptr
  );

  glEnableVertexAttribArray(0);
}

Renderer::~Renderer()
{
  if (uiShaderProgram_) glDeleteProgram(uiShaderProgram_);
  if (uiVertexBuffer_) glDeleteBuffers(1, &uiVertexBuffer_);
  if (uiVertexArray_) glDeleteVertexArrays(1, &uiVertexArray_);

  if (goalTexture_) glDeleteTextures(1, &goalTexture_);
  if (floorTexture_) glDeleteTextures(1, &floorTexture_);
  if (wallTexture_) glDeleteTextures(1, &wallTexture_);

  if (shaderProgram_) glDeleteProgram(shaderProgram_);
  if (indexBuffer_) glDeleteBuffers(1, &indexBuffer_);
  if (vertexBuffer_) glDeleteBuffers(1, &vertexBuffer_);
  if (vertexArray_) glDeleteVertexArrays(1, &vertexArray_);

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

void Renderer::uploadMesh(const Mesh& mesh)
{
  glBindVertexArray(vertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);

  glBufferData(
    GL_ARRAY_BUFFER,
    mesh.vertices.size() * sizeof(Vertex),
    mesh.vertices.data(),
    GL_STATIC_DRAW
  );

  wallIndexCount_ = static_cast<GLsizei>(mesh.wallIndices.size());
  floorIndexCount_ = static_cast<GLsizei>(mesh.floorIndices.size());

  const GLsizeiptr wallSize = mesh.wallIndices.size() * sizeof(uint32_t);
  const GLsizeiptr floorSize = mesh.floorIndices.size() * sizeof(uint32_t);

  floorIndexOffset_ = static_cast<GLintptr>(wallSize);


  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    wallSize + floorSize,
    nullptr,
    GL_STATIC_DRAW
  );

  glBufferSubData(
    GL_ELEMENT_ARRAY_BUFFER,
    0,
    wallSize,
    mesh.wallIndices.data()
  );

  glBufferSubData(
    GL_ELEMENT_ARRAY_BUFFER,
    floorIndexOffset_,
    floorSize,
    mesh.floorIndices.data()
  );
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

void Renderer::drawMaze(Camera& camera) const
{
  glBindVertexArray(vertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);

  glUseProgram(shaderProgram_);

  const glm::mat4 view = camera.viewMatrix();

  glUniformMatrix4fv(
    projectionLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(projection_)
  );

  glUniformMatrix4fv(
    viewLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(view)
  );

  glActiveTexture(GL_TEXTURE0);
  glUniform1i(textureLocation_, 0);

  glBindTexture(GL_TEXTURE_2D, wallTexture_);

  glDrawElements(
    GL_TRIANGLES,
    wallIndexCount_,
    GL_UNSIGNED_INT,
    nullptr
  );

  glBindTexture(GL_TEXTURE_2D, floorTexture_);

  glDrawElements(
    GL_TRIANGLES,
    floorIndexCount_,
    GL_UNSIGNED_INT,
    reinterpret_cast<void*>(floorIndexOffset_)
  );
}

void Renderer::drawText(
  const std::string& text,
  const glm::vec2& position,
  float scale,
  const glm::vec3& color
) const
{
  std::vector<glm::vec2> vertices;

  float x = position.x;
  float y = position.y;

  constexpr float spacing = 1.0f;

  for (char c : text){
    const Glyph& glyph = getGlyph(c);

    for (int row = 0; row < GLYPH_HEIGHT; ++row){
      for (int column = 0; column < GLYPH_WIDTH; ++ column){
        if (!(glyph[row] & (0x1 << (4 - column)))) continue;

        const float x0 = x + column * scale;
        const float y0 = y + row * scale;
        const float x1 = x0 + scale;
        const float y1 = y0 + scale;

        vertices.insert(
          vertices.end(),
          {
            {x0, y0},
            {x1, y0},
            {x1, y1},

            {x0, y0},
            {x1, y1},
            {x0, y1}
          }
        );
      }
    }
    x += (GLYPH_WIDTH + spacing) * scale;
  }

  glBindVertexArray(uiVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, uiVertexBuffer_);

  glBufferData(
    GL_ARRAY_BUFFER,
    vertices.size() * 2 *sizeof(float),
    vertices.data(),
    GL_STATIC_DRAW
  );

  glUseProgram(uiShaderProgram_);

  glUniformMatrix4fv(
    uiProjectionLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(uiProjection_)
  );

  glUniform3fv(
    uiColorLocation_,
    1,
    glm::value_ptr(color)
  );

  glDrawArrays(
    GL_TRIANGLES,
    0,
    static_cast<GLsizei>(vertices.size())
  );
}

void Renderer::drawSprite(
  GLuint texture,
  const glm::vec3 position,
  Camera& camera,
  float width,
  float height
) const
{
  glm::vec3 direction = camera.position() - position;
  direction.y = 0.0f;

  if (glm::length(direction) < 0.001f)
    return;

  direction = glm::normalize(direction);

  const glm::vec3 right{
    direction.z,
    0.0f,
    -direction.x
  };

  const glm::vec3 up{0.0f, 1.0f, 0.0f};

  const glm::vec3 bottomLeft  = position - right * (width * 0.5f);
  const glm::vec3 bottomRight = position + right * (width * 0.5f);
  const glm::vec3 topLeft     = bottomLeft + up * height;
  const glm::vec3 topRight    = bottomRight + up * height;

  std::vector<Vertex> vertices{
    {bottomRight, glm::vec2{1.0f, 1.0f}},
    {bottomLeft,  glm::vec2{0.0f, 1.0f}},
    {topLeft,     glm::vec2{0.0f, 0.0f}},

    {bottomRight, glm::vec2{1.0f, 1.0f}},
    {topLeft,     glm::vec2{0.0f, 0.0f}},
    {topRight,    glm::vec2{1.0f, 0.0f}}
  };

  glBindVertexArray(spriteVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, spriteVertexBuffer_);

  glBufferData(
    GL_ARRAY_BUFFER,
    vertices.size() * sizeof(Vertex),
    vertices.data(),
    GL_STATIC_DRAW
  );

  glUseProgram(spriteShaderProgram_);

  const glm::mat4 view = camera.viewMatrix();

  glUniformMatrix4fv(
    spriteProjectionLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(projection_)
  );

  glUniformMatrix4fv(
    spriteViewLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(view)
  );

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glUniform1i(spriteTextureLocation_, 0);

  glDepthMask(GL_FALSE);

  glDrawArrays(
    GL_TRIANGLES,
    0,
    static_cast<GLsizei>(vertices.size())
  );

  glDepthMask(GL_TRUE);
}

void Renderer::drawMap(Camera& camera) const
{

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

void Renderer::setGoalPosition(const glm::vec3 position)
{
  goalPosition_ = position;
}
