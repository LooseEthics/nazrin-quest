
#pragma once

#include "Camera.hpp"
#include "CommonGeometry.hpp"
#include "Input.hpp"
#include "Maze.hpp"
#include "ViewmodelController.hpp"

class Player
{
public:
  Player(Maze &maze);

  Camera& camera();

  void update(const Input& input, float deltaTime);

  void moveNoclip(float dx, float dy, float dz);
  void setPos(float x, float y, float z);

  void move(glm::vec3 displacement);

  void rotate(float dYaw, float dPitch, bool pitchClamp);
  void rotate(float dYaw, float dPitch);

  glm::vec3 pos();
  float yaw();
  float pitch();

  [[nodiscard]]
  bool goalReached() const noexcept;

  const std::vector<ViewmodelRenderCall*> viewModelVector() const;

private:
  Maze& maze_;

  glm::vec3 pos_;
  float yaw_;
  float pitch_;

  Camera camera_;
  glm::vec3 cameraOffset();

  static constexpr float maxPitch = glm::radians(75.0f);
  static constexpr float collisionRadius = CELL_SIZE / 8.0f;
  void resolveVertCollision(glm::vec3& position);
  void resolveCollision(glm::vec3& position);
  void resolveCircleAABB(
    glm::vec3& position,
    float minX, float maxX,
    float minZ, float maxZ
  );

  glm::vec3 velocity_{0.0f, 0.0f, 0.0f};
  glm::vec3 targetLocalVelocity_{0.0f, 0.0f, 0.0f};
  glm::vec3 targetWorldVelocity() const;
  void setTargetLocalVelocity(const Input& input);
  void updateVelocity(float deltaTime);
  static constexpr float maxSpeed = 5.0f;

  const bool isAirborne() const;

  ViewmodelController vmc_;
};
