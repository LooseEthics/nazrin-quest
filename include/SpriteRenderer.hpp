
#pragma once

#include <glad/gl.h>
#include <glm/mat4x4.hpp>

#include "Camera.hpp"
#include "Shader.hpp"
#include "SubRendererBase.hpp"
#include "Texture.hpp"

class SpriteRenderer : public SubRendererBase
{
public:
  SpriteRenderer(uint32_t width, uint32_t height);
  ~SpriteRenderer();

  void drawSprite(
    const Texture& texture,
    const glm::vec3& position,
    const Camera& camera,
    float width,
    float height
  ) const;

  void setGoalPosition(const glm::vec3 position);
  Texture goalTexture_;
  glm::vec3 goalPosition_;

private:
  void calculateProjection() override;
  glm::mat4 spriteProjection_;


  GLuint spriteVertexArray_;
  GLuint spriteVertexBuffer_;
  Shader spriteShaderProgram_;

  GLint spriteProjectionLocation_;
  GLint spriteViewLocation_;
  GLint spriteTextureLocation_;
};
