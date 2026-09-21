
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

#include "Glyphs.hpp"
#include "Renderer.hpp"

namespace
{
  constexpr const char* UI_VERTEX_SHADER_PATH = "shaders/ui.vert";
  constexpr const char* UI_FRAGMENT_SHADER_PATH = "shaders/ui.frag";
}

void Renderer::initText()
{
  glGenVertexArrays(1, &uiVertexArray_);
  glGenBuffers(1, &uiVertexBuffer_);

  uiShaderProgram_ = createShaderProgram(UI_VERTEX_SHADER_PATH, UI_FRAGMENT_SHADER_PATH);

  uiProjectionLocation_ = glGetUniformLocation(uiShaderProgram_, "projection");
  uiColorLocation_ = glGetUniformLocation(uiShaderProgram_, "color");

  uiProjection_ = glm::ortho(
    0.0f,
    static_cast<float>(width_),
    static_cast<float>(height_),
    0.0f
  );

  glBindVertexArray(uiVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, uiVertexBuffer_);

  glVertexAttribPointer(
    0,
    2,
    GL_FLOAT,
    GL_FALSE,
    2 * sizeof(float),
    nullptr
  );

  glEnableVertexAttribArray(0);
}

void Renderer::destroyText()
{
  if (uiShaderProgram_) glDeleteProgram(uiShaderProgram_);
  if (uiVertexBuffer_) glDeleteBuffers(1, &uiVertexBuffer_);
  if (uiVertexArray_) glDeleteVertexArrays(1, &uiVertexArray_);
}

void Renderer::drawText(
  const std::string& text,
  const glm::vec2& position,
  float scale,
  const glm::vec3& color
) const
{
  std::vector<glm::vec2> vertices;

  float x = position.x;
  float y = position.y;

  constexpr float spacing = 1.0f;

  for (char c : text){
    const Glyph& glyph = getGlyph(c);

    for (int row = 0; row < GLYPH_HEIGHT; ++row){
      for (int column = 0; column < GLYPH_WIDTH; ++ column){
        if (!(glyph[row] & (0x1 << (4 - column)))) continue;

        const float x0 = x + column * scale;
        const float y0 = y + row * scale;
        const float x1 = x0 + scale;
        const float y1 = y0 + scale;

        vertices.insert(
          vertices.end(),
          {
            {x0, y0},
            {x1, y0},
            {x1, y1},

            {x0, y0},
            {x1, y1},
            {x0, y1}
          }
        );
      }
    }
    x += (GLYPH_WIDTH + spacing) * scale;
  }

  glBindVertexArray(uiVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, uiVertexBuffer_);

  glBufferData(
    GL_ARRAY_BUFFER,
    vertices.size() * 2 *sizeof(float),
    vertices.data(),
    GL_STATIC_DRAW
  );

  glUseProgram(uiShaderProgram_);

  glUniformMatrix4fv(
    uiProjectionLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(uiProjection_)
  );

  glUniform3fv(
    uiColorLocation_,
    1,
    glm::value_ptr(color)
  );

  glDrawArrays(
    GL_TRIANGLES,
    0,
    static_cast<GLsizei>(vertices.size())
  );
}
