
#pragma once

#include <glad/gl.h>
#include <glm/mat4x4.hpp>

#include "Shader.hpp"
#include "SubRendererBase.hpp"

class TextRenderer : public SubRendererBase
{
public:
  TextRenderer(uint32_t width, uint32_t height);
  ~TextRenderer();

  void drawText(
    const std::string& text,
    const glm::vec2& position,
    float scale,
    const glm::vec3& color
  ) const;

private:
  void calculateProjection() override;

  GLuint uiVertexArray_;
  GLuint uiVertexBuffer_;
  Shader uiShaderProgram_;

  GLint uiProjectionLocation_;
  GLint uiColorLocation_;

  glm::mat4 uiProjection_;
};
