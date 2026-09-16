
#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
  Camera(
    glm::vec3 pos,
    float yaw,
    float pitch
  );

  Camera();

  void setPos(glm::vec3 pos);
  void setRot(float yaw, float pitch);

  glm::vec3 position() const;

  glm::mat4 viewMatrix() const;

private:
  glm::vec3 pos_;
  float yaw_;
  float pitch_;

  static constexpr glm::vec3 upVector_{0.0f, 1.0f, 0.0f};
};
