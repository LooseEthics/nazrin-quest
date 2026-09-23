
#pragma once

#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <unordered_map>

#include "Camera.hpp"
#include "Shader.hpp"
#include "SpriteRenderCall.hpp"
#include "SubRendererBase.hpp"
#include "Texture.hpp"

class SpriteRenderer : public SubRendererBase
{
public:
  SpriteRenderer(uint32_t width, uint32_t height);
  ~SpriteRenderer();

  void drawSprite(
    SpriteRenderCall spriteCall,
    const Camera& camera
  ) const;

  void drawSprite(
    SpriteId sprite,
    const glm::vec3& position,
    const Camera& camera,
    float width,
    float height
  ) const;


private:
  void calculateProjection() override;
  glm::mat4 spriteProjection_;

  GLuint spriteVertexArray_;
  GLuint spriteVertexBuffer_;
  Shader spriteShaderProgram_;

  GLint spriteProjectionLocation_;
  GLint spriteViewLocation_;
  GLint spriteTextureLocation_;

  Texture goalTexture_;

  std::unordered_map<SpriteId, Texture*> sprites_;
};
