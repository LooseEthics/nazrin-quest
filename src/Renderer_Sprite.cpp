
#include <cstddef>
#include <glad/gl.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "CommonGeometry.hpp"
#include "Renderer.hpp"

namespace
{
  constexpr const char* SPRITE_VERTEX_SHADER_PATH = "shaders/sprite.vert";
  constexpr const char* SPRITE_FRAGMENT_SHADER_PATH = "shaders/sprite.frag";

  constexpr const char* GOAL_TEXTURE_PATH = "assets/cheese.png";
}

void Renderer::initSprite()
{
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

  goalTexture_ = loadTexture(GOAL_TEXTURE_PATH);
}

void Renderer::destroySprite()
{
  if (goalTexture_) glDeleteTextures(1, &goalTexture_);
  if (spriteShaderProgram_) glDeleteProgram(spriteShaderProgram_);
  if (spriteVertexBuffer_) glDeleteBuffers(1, &spriteVertexBuffer_);
  if (spriteVertexArray_) glDeleteVertexArrays(1, &spriteVertexArray_);
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
    glm::value_ptr(mazeProjection_)
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
