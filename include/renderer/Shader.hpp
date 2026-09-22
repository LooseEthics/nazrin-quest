
#pragma once

#include <glad/gl.h>
#include <string>

class Shader
{
public:
  Shader() = default;
  Shader(
    const std::string& vertexPath,
    const std::string& fragmentPath
  );
  ~Shader();

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  Shader(Shader&& other) noexcept;
  Shader& operator=(Shader&& other) noexcept;

  void use() const;

  GLint colorLocation() const;
  GLint projectionLocation() const;
  GLint textureLocation() const;
  GLint viewLocation() const;

private:
  GLuint program_ = 0;

  GLuint compileShader(
    GLenum type,
    const char* source
  ) const;
};
