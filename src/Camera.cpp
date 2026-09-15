
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "CommonGeometry.hpp"

Camera::Camera(
  glm::vec3 pos,
  float yaw,
  float pitch
) : pos_(pos),
    yaw_(yaw),
    pitch_(pitch)
{}

Camera::Camera() : Camera(glm::vec3{0, 0, 0}, 0, 0) {}

void Camera::setPos(glm::vec3 pos)
{
  pos_ = pos;
}

void Camera::setRot(float yaw, float pitch)
{
  yaw_ = yaw;
  pitch_ = pitch;
}

glm::mat4 Camera::viewMatrix() const
{
  return glm::lookAt(
    pos_,
    pos_ + forwardVector(yaw_, pitch_),
    upVector_
  );
}
