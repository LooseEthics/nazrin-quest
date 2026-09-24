
#include "Cell.hpp"
#include "CommonGeometry.hpp"
#include "FrogEntity.hpp"

FrogEntity::FrogEntity(Maze& maze, glm::vec3 position)
  : Entity(position, SpriteId::Frog_Idle),
    rng_(static_cast<int>(position.x * position.z * 123456)),
    maze_(maze)
{}

void FrogEntity::update(float deltaTime)
{
  if (state_ == FrogState::Idle){
    timer_ += deltaTime;
    if (timer_ > nextJumpTime_){
      doJump();
    }
  } else if (state_ == FrogState::Leap){
    position_ += deltaTime * currentVelocity_;
    currentVelocity_.y -= deltaTime * frogGravity_;
    if (position_.y <= FLOOR + landingTolerance_ && currentVelocity_.y < 0.0f){
      doLand();
    }
  }
}

void FrogEntity::doJump()
{
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
  currentVelocity_ =
    jumpDirection * jumpHorizVelocity_ + glm::vec3{0.0f, jumpVertVelocity_, 0.0f};
  sprite_ = SpriteId::Frog_Leap;
}

void FrogEntity::doLand()
{
  state_ = FrogState::Idle;
  currentVelocity_ = {0.0f, 0.0f, 0.0f};
  position_.y = FLOOR;
  sprite_ = SpriteId::Frog_Idle;
}
