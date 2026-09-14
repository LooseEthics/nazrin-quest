
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

Camera::Camera(
  float x,
  float y,
  float z,
  float yaw,
  float pitch
) : pos_({x, y, z}),
    yaw_(yaw),
    pitch_(pitch)
{}

Camera::Camera() : Camera(0, 0, 0, 0, 0) {}

void Camera::move(float dx, float dy, float dz)
{
  pos_ += glm::vec3{dx, dy, dz};
}

void Camera::setPos(float x, float y, float z)
{
  pos_ = glm::vec3{x, y, z};
}

void Camera::moveForward(float distance)
{
  const glm::vec3 forward = flatForwardVector();
  pos_ += glm::vec3{forward.x, 0.0f, forward.z} * distance;
}

void Camera::moveRight(float distance)
{
  const glm::vec3 forward = flatForwardVector();
  pos_ += glm::vec3{forward.z, 0.0f, -forward.x} * distance;
}

void Camera::rotate(float dYaw, float dPitch)
{
  yaw_ += dYaw;
  pitch_ += dPitch;
}

glm::vec3 Camera::forwardVector() const
{
  return {
    std::cos(yaw_) * std::cos(pitch_),
    std::sin(pitch_),
    std::sin(yaw_) * std::cos(pitch_)
  };
}

glm::vec3 Camera::flatForwardVector() const
{
  return {
    std::cos(yaw_),
    0.0f,
    std::sin(yaw_)
  };
}

glm::mat4 Camera::viewMatrix() const
{
  return glm::lookAt(
    pos_,
    pos_ + forwardVector(),
    upVector_
  );
}
