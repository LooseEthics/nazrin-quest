
#pragma once

#include "Camera.hpp"
#include "CommonGeometry.hpp"
#include "Input.hpp"

class Player
{
public:
  Player(glm::vec3 startPosition);

  Camera& camera();

  void update(const Input& input, float deltaTime);

  void move(float dx, float dy, float dz);
  void setPos(float x, float y, float z);
  void moveForward(float distance);
  void moveRight(float distance);
  void rotate(float dYaw, float dPitch, bool pitchClamp);
  void rotate(float dYaw, float dPitch);

private:
  glm::vec3 pos_;
  float yaw_;
  float pitch_;

  Camera camera_;
  glm::vec3 cameraOffset();

  static constexpr float maxPitch = glm::radians(89.0f);
};
