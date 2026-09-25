
#pragma once

#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <unordered_map>

#include "Camera.hpp"
#include "Shader.hpp"
#include "SpriteRenderCall.hpp"
#include "SubRendererBase.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"
#include "ViewmodelRenderCall.hpp"

class SpriteRenderer : public SubRendererBase
{
public:
  SpriteRenderer(uint32_t width, uint32_t height);
  ~SpriteRenderer();

  void drawSprite(
    SpriteRenderCall renderCall,
    const Camera& camera
  ) const;

  void drawViewModel(
    ViewmodelRenderCall renderCall,
    const Camera& camera
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

  std::unordered_map<SpriteId, Texture> sprites_;
  const std::unordered_map<SpriteId, std::string> texturePaths_{
    {SpriteId::Cheese, "assets/cheese.png"},
    {SpriteId::Frog_Idle, "assets/frog_idle.png"},
    {SpriteId::Frog_Leap, "assets/frog_leap.png"},
    {SpriteId::POV_Mantle, "assets/POV_mantle.png"}
  };
};
