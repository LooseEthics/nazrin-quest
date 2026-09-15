
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
