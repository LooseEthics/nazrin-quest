
#include <glm/gtc/matrix_transform.hpp>

#include "Quad.hpp"

glm::vec2 transformUV(
  glm::vec2 uv,
  const FlatTransform& tf
)
{
  uv -= glm::vec2{0.5f, 0.5f};

  if (tf.flipX)
    uv.x = -uv.x;

  if (tf.flipY)
    uv.y = -uv.y;

  uv += glm::vec2{0.5f, 0.5f};

  return uv;
}

const std::vector<Vertex> getQuadVertices(
  glm::vec3 bottomLeft,
  glm::vec3 bottomRight,
  glm::vec3 topLeft,
  glm::vec3 topRight,
  FlatTransform tf
) {
  // only applies uv flipping part of the transform
  // do vertex rotation externally
  const glm::vec2 bottomRightUV =
    transformUV({1.0f, 1.0f}, tf);

  const glm::vec2 bottomLeftUV =
    transformUV({0.0f, 1.0f}, tf);

  const glm::vec2 topLeftUV =
    transformUV({0.0f, 0.0f}, tf);

  const glm::vec2 topRightUV =
    transformUV({1.0f, 0.0f}, tf);

  return {
    {bottomRight, bottomRightUV},
    {bottomLeft,  bottomLeftUV},
    {topLeft,     topLeftUV},

    {bottomRight, bottomRightUV},
    {topLeft,     topLeftUV},
    {topRight,    topRightUV}
  };
}
