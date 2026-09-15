
#pragma once

#include "Camera.hpp"
#include "CommonGeometry.hpp"
#include "Input.hpp"
#include "Maze.hpp"

class Player
{
public:
  Player(Maze &maze);

  Camera& camera();

  void update(const Input& input, float deltaTime);

  void moveNoclip(float dx, float dy, float dz);
  void setPos(float x, float y, float z);

  void move(float dx, float dz);
  void resolveCollision(glm::vec3& position);
  void resolveCircleAABB(
    glm::vec3& position,
    float minX, float maxX,
    float minZ, float maxZ
  );

  void rotate(float dYaw, float dPitch, bool pitchClamp);
  void rotate(float dYaw, float dPitch);

  glm::vec3 pos();
  float yaw();
  float pitch();

private:
  Maze& maze_;

  glm::vec3 pos_;
  float yaw_;
  float pitch_;

  Camera camera_;
  glm::vec3 cameraOffset();

  static constexpr float maxPitch = glm::radians(89.0f);
  static constexpr float collisionRadius = CELL_SIZE / 8.0f;
};
