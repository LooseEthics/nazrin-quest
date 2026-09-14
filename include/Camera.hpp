
#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
  Camera(
    float x,
    float y,
    float z,
    float yaw,
    float pitch
  );

  Camera();

  void move(float dx, float dy, float dz);
  void setPos(float x, float y, float z);
  void moveForward(float distance);
  void moveRight(float distance);
  void rotate(float dYaw, float dPitch);

  glm::vec3 forwardVector() const;
  glm::vec3 flatForwardVector() const;
  glm::mat4 viewMatrix() const;

private:
  glm::vec3 pos_;
  float yaw_;
  float pitch_;

  static constexpr glm::vec3 upVector_{0.0f, 1.0f, 0.0f};
};
