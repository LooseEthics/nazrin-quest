#include <iostream>
#include <cstddef>
#include <glad/gl.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "CommonGeometry.hpp"
#include "FlatTransform.hpp"
#include "Quad.hpp"
#include "SpriteRenderer.hpp"

namespace
{
  constexpr const char* SPRITE_VERTEX_SHADER_PATH = "shaders/sprite.vert";
  constexpr const char* SPRITE_FRAGMENT_SHADER_PATH = "shaders/sprite.frag";
}

SpriteRenderer::SpriteRenderer(uint32_t width, uint32_t height)
  : SubRendererBase(width, height)
{
  calculateProjection();

  glGenVertexArrays(1, &spriteVertexArray_);
  glGenBuffers(1, &spriteVertexBuffer_);

  spriteShaderProgram_ = Shader{
    SPRITE_VERTEX_SHADER_PATH,
    SPRITE_FRAGMENT_SHADER_PATH
  };

  spriteProjectionLocation_ = spriteShaderProgram_.projectionLocation();
  spriteViewLocation_ = spriteShaderProgram_.viewLocation();
  spriteTextureLocation_ = spriteShaderProgram_.textureLocation();

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

  for (const auto& [id, path] : texturePaths_){
    sprites_.emplace(id, Texture{path});
  }
}

SpriteRenderer::~SpriteRenderer()
{
  if (spriteVertexBuffer_) glDeleteBuffers(1, &spriteVertexBuffer_);
  if (spriteVertexArray_) glDeleteVertexArrays(1, &spriteVertexArray_);
}

void SpriteRenderer::drawSprite(
  SpriteRenderCall renderCall,
  const Camera& camera
) const {
  glm::vec3 direction = camera.position() - renderCall.position;
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

  const glm::vec3 bottomLeft  = renderCall.position - right * (renderCall.width * 0.5f);
  const glm::vec3 bottomRight = renderCall.position + right * (renderCall.width * 0.5f);
  const glm::vec3 topLeft     = bottomLeft + up * renderCall.height;
  const glm::vec3 topRight    = bottomRight + up * renderCall.height;

  std::vector<Vertex> vertices = getQuadVertices(bottomLeft, bottomRight, topLeft, topRight, renderCall.tf);

  glBindVertexArray(spriteVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, spriteVertexBuffer_);

  glBufferData(
    GL_ARRAY_BUFFER,
    vertices.size() * sizeof(Vertex),
    vertices.data(),
    GL_STATIC_DRAW
  );

  spriteShaderProgram_.use();

  const glm::mat4 view = camera.viewMatrix();

  glUniformMatrix4fv(
    spriteProjectionLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(spriteProjection_)
  );

  glUniformMatrix4fv(
    spriteViewLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(view)
  );

  const Texture& texture = getTexture(renderCall.sprite);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.id());

  glUniform1i(spriteTextureLocation_, 0);

  glDepthMask(GL_FALSE);

  glDrawArrays(
    GL_TRIANGLES,
    0,
    static_cast<GLsizei>(vertices.size())
  );

  glDepthMask(GL_TRUE);
}

void printVec3(glm::vec3 v){
  std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

void SpriteRenderer::drawViewModel(
  ViewmodelRenderCall renderCall,
  const Camera& camera
) const {

  const glm::vec3 worldUp = {0.0f, 1.0f, 0.0f};

  const glm::vec3 center = camera.position() + rotate(renderCall.cameraRelativePos, -camera.yaw(), worldUp);

  const glm::vec3 worldQuadNormal = glm::normalize(rotate(renderCall.localQuadNormal, -camera.yaw(), worldUp));
  const glm::vec3 reference = std::abs(glm::dot(worldQuadNormal, worldUp)) < 0.999f ?
    worldUp :
    camera.forward();

  const glm::vec3 right = glm::normalize(glm::cross(worldQuadNormal, reference));
  const glm::vec3 rotatedRight = rotate(right, renderCall.tf.rot, worldQuadNormal);

  const glm::vec3 up = glm::cross(worldQuadNormal, rotatedRight);

  const glm::vec2& uvAnchor = renderCall.uvAnchorPoint;
  const glm::vec3 rightX = rotatedRight * renderCall.size.x;
  const glm::vec3 upY = up * renderCall.size.y;

  const glm::vec3 bottomLeft =  center - rightX * uvAnchor.x       - upY * uvAnchor.y;
  const glm::vec3 bottomRight = center + rightX * (1 - uvAnchor.x) - upY * uvAnchor.y;
  const glm::vec3 topLeft =     center - rightX * uvAnchor.x       + upY * (1 - uvAnchor.y);
  const glm::vec3 topRight =    center + rightX * (1 - uvAnchor.x) + upY * (1 - uvAnchor.y);

  const std::vector<Vertex> vertices =
    getQuadVerticesNoRot(bottomLeft, bottomRight, topLeft, topRight, renderCall.tf);

  glBindVertexArray(spriteVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, spriteVertexBuffer_);

  glBufferData(
    GL_ARRAY_BUFFER,
    vertices.size() * sizeof(Vertex),
    vertices.data(),
    GL_STATIC_DRAW
  );

  spriteShaderProgram_.use();

  glUniformMatrix4fv(
    spriteProjectionLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(spriteProjection_)
  );

  const glm::mat4 view = camera.viewMatrix();

  glUniformMatrix4fv(
    spriteViewLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(view)
  );

  const Texture& texture = getTexture(renderCall.sprite);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.id());

  glUniform1i(spriteTextureLocation_, 0);

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glDrawArrays(
    GL_TRIANGLES,
    0,
    static_cast<GLsizei>(vertices.size())
  );

  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
}

void SpriteRenderer::calculateProjection()
{
  spriteProjection_ = glm::perspective(
    glm::radians(FOV),
    static_cast<float>(windowWidth_) / static_cast<float>(windowHeight_),
    NEAR_CULLING_PLANE,
    FAR_CULLING_PLANE
  );
}

const Texture& SpriteRenderer::getTexture(SpriteId id) const
{
  auto it = sprites_.find(id);

  if (it == sprites_.end())
    return sprites_.at(SpriteId::Default);

  return it->second;
}
