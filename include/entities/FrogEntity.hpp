
#pragma once

#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <random>

#include "Entity.hpp"
#include "Maze.hpp"

class FrogEntity : public Entity
{
public:
  FrogEntity(Maze& maze, glm::vec3 position);

  void update(float deltaTime) override;

  enum FrogState
  {
    Idle,
    Leap
  };

private:
  FrogState state_ = Idle;
  float timer_ = 0.0f;
  float nextJumpTime_ = 5.0f;
  glm::vec3 currentVelocity_ = {0.0f, 0.0f, 0.0f};
  std::mt19937 rng_;
  Maze& maze_;

  static constexpr float minJumpDelay = 1.0f;
  static constexpr float maxJumpDelay = 10.0f;
  std::uniform_real_distribution<float> jumpDelayDistribution{minJumpDelay, maxJumpDelay};

  static constexpr float jumpRange_ = 1.0f;
  static constexpr float frogGravity_ = 9.8f;
  static constexpr float jumpVertVelocity_ = 3.0f;
  static constexpr float jumpHorizVelocity_ = jumpRange_ * frogGravity_ / (2 * jumpVertVelocity_);
  std::uniform_real_distribution<float> yawDistribution{0.0f, 2 * glm::pi<float>()};
  static constexpr float landingTolerance_ = 0.05f;

  void doJump();
  void doLand();
};
