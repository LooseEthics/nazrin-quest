
#include <glm/gtc/matrix_transform.hpp>

#include "CommonGeometry.hpp"
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
  rotateQuadAroundNormal(
    bottomLeft,
    bottomRight,
    topLeft,
    topRight,
    tf.rot
  );

  return getQuadVerticesNoRot(
    bottomLeft,
    bottomRight,
    topLeft,
    topRight,
    tf
  );
}

const std::vector<Vertex> getQuadVerticesNoRot(
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

void rotateQuadAroundNormal(
  glm::vec3& bottomLeft,
  glm::vec3& bottomRight,
  glm::vec3& topLeft,
  glm::vec3& topRight,
  float angle
){
  rotateQuadAroundNormal(
    bottomLeft,
    bottomRight,
    topLeft,
    topRight,
    angle,
    {0.5f, 0.5f}
  );
}

void rotateQuadAroundNormal(
  glm::vec3& bottomLeft,
  glm::vec3& bottomRight,
  glm::vec3& topLeft,
  glm::vec3& topRight,
  float angle,
  const glm::vec2& uvAnchor
){
  glm::vec3 bottom = glm::mix(bottomLeft, bottomRight, uvAnchor.x);
  glm::vec3 top = glm::mix(topLeft, topRight, uvAnchor.x);
  glm::vec3 anchorPoint = glm::mix(bottom, top, uvAnchor.y);

  glm::vec3 normal = glm::normalize(
    glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft)
  );

  rotateQuadAroundAxis(
    bottomLeft,
    bottomRight,
    topLeft,
    topRight,
    angle,
    anchorPoint,
    normal
  );
}

void rotateQuadAroundAxis(
  glm::vec3& bottomLeft,
  glm::vec3& bottomRight,
  glm::vec3& topLeft,
  glm::vec3& topRight,
  float angle,
  const glm::vec3& anchorPoint,
  const glm::vec3& axis
){
  bottomLeft =
    rotate(bottomLeft - anchorPoint, angle, axis) + anchorPoint;
  bottomRight =
    rotate(bottomRight - anchorPoint, angle, axis) + anchorPoint;
  topLeft =
    rotate(topLeft - anchorPoint, angle, axis) + anchorPoint;
  topRight =
    rotate(topRight - anchorPoint, angle, axis) + anchorPoint;
}
