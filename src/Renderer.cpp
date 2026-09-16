
#include <fstream>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Renderer.hpp"

namespace
{
  const char* VERTEX_SHADER_PATH = "shaders/maze.vert";
  const char* FRAGMENT_SHADER_PATH = "shaders/maze.frag";

  constexpr bool renderEdges = true;
  constexpr glm::vec3 edgeColor{0.9f, 0.0f, 0.0f};

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

Renderer::Renderer(int width, int height, Camera& camera)
  : window_(nullptr),
    context_(nullptr),
    vertexArray_(0),
    vertexBuffer_(0),
    indexBuffer_(0),
    shaderProgram_(0),
    camera_(camera)
{
  if (!SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error(SDL_GetError());

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  window_ = SDL_CreateWindow(
    "Nazrin Quest",
    width,
    height,
    SDL_WINDOW_OPENGL
  );
  if (!window_) throw std::runtime_error(SDL_GetError());

  if (!SDL_SetWindowRelativeMouseMode(window_, true))
    throw std::runtime_error(SDL_GetError());

  context_ = SDL_GL_CreateContext(window_);
  if (!context_) throw std::runtime_error(SDL_GetError());

  if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress)))
    throw std::runtime_error("Failed to initialize OpenGL");

  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);
  glGenVertexArrays(1, &vertexArray_);
  glGenBuffers(1, &vertexBuffer_);
  glGenBuffers(1, &indexBuffer_);

  shaderProgram_ = createShaderProgram();

  projection_ = glm::perspective(
    glm::radians(80.0f),
    static_cast<float>(width) / static_cast<float>(height),
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
}

Renderer::~Renderer()
{
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

  glBufferData(
    GL_ARRAY_BUFFER,
    mesh.vertices.size() * sizeof(Vertex),
    mesh.vertices.data(),
    GL_STATIC_DRAW
  );

  triIndexCount_ = static_cast<GLsizei>(mesh.triIndices.size());
  edgeIndexCount_ = static_cast<GLsizei>(mesh.edgeIndices.size());

  const GLsizeiptr triSize = mesh.triIndices.size() * sizeof(uint32_t);
  const GLsizeiptr edgeSize = mesh.edgeIndices.size() * sizeof(uint32_t);

  edgeIndexOffset_ = static_cast<GLintptr>(triSize);

  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    triSize + edgeSize,
    nullptr,
    GL_STATIC_DRAW
  );

  glBufferSubData(
    GL_ELEMENT_ARRAY_BUFFER,
    0,
    triSize,
    mesh.triIndices.data()
  );

  glBufferSubData(
    GL_ELEMENT_ARRAY_BUFFER,
    edgeIndexOffset_,
    edgeSize,
    mesh.edgeIndices.data()
  );
}

void Renderer::draw(
  const glm::vec3& color
) const
{
  glUseProgram(shaderProgram_);

  const glm::mat4 view = camera_.viewMatrix();

  const GLint projectionLocation = glGetUniformLocation(shaderProgram_, "projection");
  const GLint viewLocation = glGetUniformLocation(shaderProgram_, "view");
  const GLint colorLocation = glGetUniformLocation(shaderProgram_, "color");

  glUniformMatrix4fv(
    projectionLocation,
    1,
    GL_FALSE,
    glm::value_ptr(projection_)
  );

  glUniformMatrix4fv(
    viewLocation,
    1,
    GL_FALSE,
    glm::value_ptr(view)
  );

  glUniform3fv(
    colorLocation,
    1,
    glm::value_ptr(color)
  );

  glDrawElements(
    GL_TRIANGLES,
    triIndexCount_,
    GL_UNSIGNED_INT,
    nullptr
  );

  if (renderEdges){
    glLineWidth(3.0f);

    glUniform3fv(
      colorLocation,
      1,
      glm::value_ptr(edgeColor)
    );

    glDrawElements(
      GL_LINES,
      edgeIndexCount_,
      GL_UNSIGNED_INT,
      reinterpret_cast<void*>(edgeIndexOffset_)
    );
  }
}

SDL_Window* Renderer::window() const
{
  return window_;
}

uint32_t Renderer::compileShader(
  uint32_t type,
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

uint32_t Renderer::createShaderProgram() const
{
  const std::string vertexShaderSource = readFile(VERTEX_SHADER_PATH);
  const std::string fragmentShaderSource = readFile(FRAGMENT_SHADER_PATH);
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
