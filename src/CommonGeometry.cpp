
#include "CommonGeometry.hpp"

glm::vec3 forwardVector(float yaw, float pitch)
{
  return {
    std::cos(yaw) * std::cos(pitch),
    std::sin(pitch),
    std::sin(yaw) * std::cos(pitch)
  };
}

glm::vec3 flatForwardVector(float yaw, float pitch)
{
  return {
    std::cos(yaw),
    0.0f,
    std::sin(yaw)
  };
}

glm::vec3 rotate(glm::vec3 vector, float angle, glm::vec3 axis)
{
  // rotates vector around axis by angle in radians
  // assumes axis is normalized
  return vector * std::cos(angle)
    + glm::cross(axis, vector) * std::sin(angle)
    + axis * glm::dot(axis, vector) * (1.0f - std::cos(angle));
}
