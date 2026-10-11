
#pragma once

#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <random>

#include "PhysicalEntity.hpp"
#include "Maze.hpp"

class FrogEntity : public PhysicalEntity
{
public:
  FrogEntity(glm::vec3 position, Maze& maze);

  void update(float deltaTime) override;
  void move(float deltaTime) override;

  enum FrogState
  {
    Idle,
    Leap,
    Splatted
  };

private:
  FrogState state_ = Idle;
  float timer_ = 0.0f;
  float nextJumpTime_ = 5.0f;
  std::mt19937 rng_;

  void moveLeaping(float deltaTime);
  void moveSplat(float deltaTime);

  static constexpr float minJumpDelay = 1.0f;
  static constexpr float maxJumpDelay = 10.0f;
  std::uniform_real_distribution<float> jumpDelayDistribution{minJumpDelay, maxJumpDelay};

  static constexpr float jumpRange_ = 1.0f;
  static constexpr float jumpVertVelocity_ = 3.0f;
  static constexpr float airTime_ = 2 * jumpVertVelocity_ / GRAVITY;
  static constexpr float jumpHorizVelocity_ = jumpRange_ / airTime_;
  std::uniform_real_distribution<float> yawDistribution{0.0f, 2 * glm::pi<float>()};

  float angularVelocity_ = 0.0f;
  static constexpr float jumpAngularVelocity_ = 2 * glm::pi<float>() / airTime_;

  void doJump();
  void doLand();
  void doSplat();

  static constexpr float splatGravityMult_ = 0.2f;
};
