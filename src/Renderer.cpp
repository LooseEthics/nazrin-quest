
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <string>

#include "Renderer.hpp"

namespace {

constexpr const char* VERTEX_SHADER = R"(
#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

void main()
{
  gl_Position = projection * view * vec4(position, 1.0);
}
)";

constexpr const char* FRAGMENT_SHADER = R"(
#version 330 core

out vec4 fragmentColor;

uniform vec3 color;

void main(){
  fragmentColor = vec4(color, 1.0);
}
)";

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

void Renderer::draw(
  const Mesh& mesh,
  GLenum primitiveType,
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

  glBindVertexArray(vertexArray_);

  glBindBuffer(
    GL_ARRAY_BUFFER,
    vertexBuffer_
  );

  glBufferData(
    GL_ARRAY_BUFFER,
    mesh.vertices.size() * sizeof(Vertex),
    mesh.vertices.data(),
    GL_STATIC_DRAW
  );

  glBindBuffer(
    GL_ELEMENT_ARRAY_BUFFER,
    indexBuffer_
  );

  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    mesh.indices.size() * sizeof(uint32_t),
    mesh.indices.data(),
    GL_STATIC_DRAW
  );

  glVertexAttribPointer(
    0,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    nullptr
  );

  glEnableVertexAttribArray(0);

  glDrawElements(
    primitiveType,
    static_cast<GLsizei>(mesh.indices.size()),
    GL_UNSIGNED_INT,
    nullptr
  );

  glBindVertexArray(0);
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
  const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, VERTEX_SHADER);
  const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
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
