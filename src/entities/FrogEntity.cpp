
#include "Cell.hpp"
#include "CommonGeometry.hpp"
#include "FrogEntity.hpp"

FrogEntity::FrogEntity(glm::vec3 position, Maze& maze)
  : PhysicalEntity(position, maze),
    rng_(static_cast<int>(position.x * position.z * 123456))
{
  sprite_ = SpriteId::Frog_Idle;
}

void FrogEntity::update(float deltaTime)
{
  if (state_ == FrogState::Idle){
    timer_ += deltaTime;
    if (timer_ > nextJumpTime_){
      doJump();
    }
  } else {
    move(deltaTime);
  }
}

void FrogEntity::move(float deltaTime)
{
  if (state_ == FrogState::Leap){
    moveLeaping(deltaTime);
  } else if (state_ == FrogState::Splatted){
    moveSplat(deltaTime);
  }
}

void FrogEntity::moveLeaping(float deltaTime)
{
  glm::vec3 displacement = velocity_ * deltaTime;
  float dx = displacement.x;
  float dy = displacement.y;
  float dz = displacement.z;
  velocity_.y -= deltaTime * GRAVITY;
  if (dx == 0.0f && dy == 0.0f && dz == 0.0f) return;

  glm::vec3 candidate = position_;

  candidate.x += dx;
  bool shouldSplat = resolveCollision(candidate);

  candidate.y += dy;
  bool shouldLand = resolveVertCollision(candidate);

  candidate.z += dz;
  shouldSplat |= resolveCollision(candidate);

  position_ = candidate;

  tf_.rot += deltaTime * angularVelocity_;

  if (shouldLand){
    doLand();
  }
  else if (shouldSplat){
    doSplat();
  }
}

void FrogEntity::moveSplat(float deltaTime)
{
  float dy = deltaTime * velocity_.y;
  velocity_.y -= deltaTime * GRAVITY * splatGravityMult_;
  if (dy == 0.0f) return;

  glm::vec3 candidate = position_;

  candidate.y += dy;
  bool shouldLand = resolveVertCollision(candidate);

  position_ = candidate;

  if (shouldLand){
    doLand();
  }
}

void FrogEntity::doJump()
{
  // TODO FIX: this can loop infinitely if the frog is ever put in a tiny box
  state_ = FrogState::Leap;
  timer_ = 0.0f;
  nextJumpTime_ = jumpDelayDistribution(rng_);

  glm::vec3 jumpDirection = flatForwardVector(yawDistribution(rng_), 0.0f);
  glm::vec3 jumpTarget = jumpDirection * jumpRange_ + position_;
  while (
    !maze_.isInside(maze_.world2xy(jumpTarget)) ||
    maze_.get(maze_.world2xy(jumpTarget)) == Cell::Wall
  ) {
    jumpDirection = flatForwardVector(yawDistribution(rng_), 0.0f);
    jumpTarget = jumpDirection * jumpRange_ + position_;
  }

  velocity_ =
    jumpDirection * jumpHorizVelocity_ + glm::vec3{0.0f, jumpVertVelocity_, 0.0f};

  sprite_ = SpriteId::Frog_Leap;

  const int njtInt = static_cast<int>(nextJumpTime_ * 10);
  if (njtInt % 2 == 0)
    angularVelocity_ = njtInt % 4 == 0 ? jumpAngularVelocity_ : -jumpAngularVelocity_;
}

void FrogEntity::doLand()
{
  state_ = FrogState::Idle;
  velocity_ = {0.0f, 0.0f, 0.0f};
  position_.y = FLOOR;
  sprite_ = SpriteId::Frog_Idle;
  angularVelocity_ = 0.0f;
  tf_.rot = 0.0f;
}

void FrogEntity::doSplat()
{
  state_ = FrogState::Splatted;
  velocity_ = {0.0f, 0.0f, 0.0f};
  angularVelocity_ = 0.0f;
}
