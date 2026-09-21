
#pragma once

#include <glad/gl.h>
#include <string>
#include <vector>

class Texture
{
public:
  Texture();
  explicit Texture(const std::string& path);
  Texture(
    const std::vector<unsigned char>& pixels,
    int width,
    int height,
    int channels
  );
  ~Texture();

  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

  Texture(Texture&& other) noexcept;
  Texture& operator=(Texture&& other) noexcept;

  GLuint id() const;

private:
  GLuint id_ = 0;
};
