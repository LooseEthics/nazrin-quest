
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "Shader.hpp"

std::string readFile(const std::string& path)
{
  std::ifstream file(path);

  if (!file)
    throw std::runtime_error("Failed to open shader: " + path);

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

Shader::Shader(
  const std::string& vertexShaderPath,
  const std::string& fragmentShaderPath
) {
  const std::string vertexShaderSource = readFile(vertexShaderPath);
  const std::string fragmentShaderSource = readFile(fragmentShaderPath);
  const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
  const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());
  program_ = glCreateProgram();

  glAttachShader(program_, vertexShader);
  glAttachShader(program_, fragmentShader);
  glLinkProgram(program_);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLint success = 0;
  glGetProgramiv(program_, GL_LINK_STATUS, &success);

  if (!success){
    GLint logLength = 0;
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);

    std::string log(logLength, '\0');

    glGetProgramInfoLog(
      program_,
      logLength,
      nullptr,
      log.data()
    );

    glDeleteProgram(program_);

    throw std::runtime_error("Shader linking failed:\n" + log);
  }
}

Shader::~Shader()
{
  glDeleteProgram(program_);
}

Shader::Shader(Shader&& other) noexcept
  : program_(other.program_)
{
  other.program_ = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
  if (this != &other) {
    if (program_ != 0)
      glDeleteProgram(program_);

    program_ = other.program_;
    other.program_ = 0;
  }

  return *this;
}

void Shader::use() const {glUseProgram(program_);}

GLint Shader::colorLocation() const {return glGetUniformLocation(program_, "color");}

GLint Shader::projectionLocation() const {return glGetUniformLocation(program_, "projection");}

GLint Shader::textureLocation() const {return glGetUniformLocation(program_, "textureSampler");}

GLint Shader::viewLocation() const {return glGetUniformLocation(program_, "view");}

GLuint Shader::compileShader(
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
