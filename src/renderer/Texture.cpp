
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Texture.hpp"

Texture::Texture()
  : id_(0) {}


Texture::Texture(const std::string& path)
  : Texture(SpriteId::Default, path)
{}

Texture::Texture(SpriteId sid, const std::string& path)
  : sid_(sid)
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

  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);

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
}


Texture::Texture(
  const std::vector<unsigned char>& pixels,
  int width,
  int height,
  int channels
) {
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MIN_FILTER,
    GL_NEAREST
  );

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MAG_FILTER,
    GL_NEAREST
  );

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_WRAP_S,
    GL_CLAMP_TO_EDGE
  );

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_WRAP_T,
    GL_CLAMP_TO_EDGE
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
    pixels.data()
  );
}

Texture::~Texture()
{
  if (id_ !=0) glDeleteTextures(1, &id_);
}

Texture::Texture(Texture&& other) noexcept
  : id_(other.id_)
{
  other.id_ = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
  if (this != &other){
    if (id_ != 0) glDeleteTextures(1, &id_);

    id_ = other.id_;
    other.id_ = 0;
  }

  return *this;
}

GLuint Texture::id() const {return id_;}
