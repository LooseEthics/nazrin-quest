
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <iostream>

#include "Player.hpp"

Player::Player(Maze& maze)
  : maze_(maze),
    pos_(maze_.getStartCoords()),
    yaw_(-glm::half_pi<float>()),
    pitch_(0.0f),
    camera_(
      pos_ + cameraOffset(),
      yaw_,
      pitch_)
{}


Camera& Player::camera()
{
  return camera_;
}

void Player::update(const Input& input, float deltaTime)
{
  constexpr float LOOK_SPEED = 0.0025f;

  setTargetLocalVelocity(input);
  updateVelocity(deltaTime);

  move(velocity_ * deltaTime);

  // moveForward(forward * MOVE_SPEED * deltaTime);
  // moveRight(right * MOVE_SPEED * deltaTime);

  if (input.mouseCaptured())
    rotate(
      input.mouseDeltaX() * LOOK_SPEED,
      -input.mouseDeltaY() * LOOK_SPEED
    );

  camera_.setPos(pos_ + cameraOffset());
  camera_.setRot(yaw_, pitch_);

}

void Player::moveNoclip(float dx, float dy, float dz)
{
  pos_ += glm::vec3{dx, dy, dz};
}

void Player::setPos(float x, float y, float z)
{
  pos_ = glm::vec3{x, y, z};
}

void Player::move(glm::vec3 displacement)
{
  // assuming distance to move is less than collision radius
  float dx = displacement.x;
  float dy = displacement.y;
  float dz = displacement.z;
  if (dx == 0.0f && dy == 0.0f && dz == 0.0f) return;

  glm::vec3 candidate = pos_;

  candidate.x += dx;
  resolveCollision(candidate);

  candidate.y += dy;
  resolveVertCollision(candidate);

  candidate.z += dz;
  resolveCollision(candidate);

  pos_ = candidate;
}

void Player::resolveVertCollision(glm::vec3& position)
{
  if (position.y <= FLOOR + LANDING_TOLERANCE){
    position.y = FLOOR;
  }
}

void Player::resolveCollision(glm::vec3& position)
{
  const int minCellX = static_cast<int>(std::floor((position.x - collisionRadius) / CELL_SIZE));
  const int maxCellX = static_cast<int>(std::floor((position.x + collisionRadius) / CELL_SIZE));
  const int minCellZ = static_cast<int>(std::floor((position.z - collisionRadius) / CELL_SIZE));
  const int maxCellZ = static_cast<int>(std::floor((position.z + collisionRadius) / CELL_SIZE));

  for (int z = minCellZ; z <= maxCellZ; ++z){
    for (int x = minCellX; x <= maxCellX; ++x){

      if (!maze_.isInside(x, z)) continue;

      if (maze_.get(x, z) != Cell::Wall) continue;

      const float minX = x * CELL_SIZE;
      const float maxX = (x + 1) * CELL_SIZE;
      const float minZ = z * CELL_SIZE;
      const float maxZ = (z + 1) * CELL_SIZE;

      resolveCircleAABB(
        position,
        minX, maxX,
        minZ, maxZ
      );
    }
  }
}

void Player::resolveCircleAABB(
  glm::vec3& position,
  float minX, float maxX,
  float minZ, float maxZ
) {
  const float closestX = glm::clamp(position.x, minX, maxX);
  const float closestZ = glm::clamp(position.z, minZ, maxZ);

  float dx = position.x - closestX;
  float dz = position.z - closestZ;

  const float distanceSquared = dx * dx + dz * dz;
  const float radiusSquared = collisionRadius * collisionRadius;

  if (distanceSquared >= radiusSquared) return;

  // if distanceSquared == 0, then the player is in a wall - this shouldn't happen unless going very very fast
  // add handling for this if it becomes relevant

  const float distance = std::sqrt(distanceSquared);

  if (distance > 0.0f){
    float penetration = collisionRadius - distance;

    position.x += dx / distance * penetration;
    position.z += dz / distance * penetration;
  }
}

void Player::rotate(float dYaw, float dPitch, bool pitchClamp)
{
  yaw_ += dYaw;
  if (pitchClamp)
    pitch_ = glm::clamp(
      pitch_ + dPitch,
      -maxPitch,
      maxPitch
    );
  else
    pitch_ += dPitch;
}

void Player::rotate(float dYaw, float dPitch)
{
  rotate(dYaw, dPitch, true);
}

glm::vec3 Player::cameraOffset(){
  return {0.0f, 1.0f, 0.0f};
}

glm::vec3 Player::pos() {return pos_;}
float Player::yaw() {return yaw_;}
float Player::pitch() {return pitch_;}

bool Player::goalReached() const noexcept
{
  return maze_.world2xy(pos_) == maze_.getGoal();
}

glm::vec3 Player::targetWorldVelocity() const
{
  const glm::vec3 forward = flatForwardVector(yaw_, 0.0f);
  const glm::vec3 right ={-forward.z, 0.0f, forward.x};

  return forward * targetLocalVelocity_.x + right * targetLocalVelocity_.z;
}

void Player::setTargetLocalVelocity(const Input& input)
{
  targetLocalVelocity_ = {0.0f, 0.0f, 0.0f};

  if (input.forward()) targetLocalVelocity_.x += 1.0;
  if (input.backward()) targetLocalVelocity_.x -= 1.0;
  if (input.right()) targetLocalVelocity_.z += 1.0;
  if (input.left()) targetLocalVelocity_.z -= 1.0;

  targetLocalVelocity_ *= maxSpeed;

  if (targetLocalVelocity_.x != 0 && targetLocalVelocity_.z != 0){
    targetLocalVelocity_ *= glm::one_over_root_two<float>();
  }
}

void Player::updateVelocity(float deltaTime)
{
  constexpr float MIN_ACC = 5.0f;
  constexpr float MAX_ACC = 20.0f;

  const glm::vec3 target = targetWorldVelocity();
  const glm::vec2 error{
    target.x - velocity_.x,
    target.z - velocity_.z
  };

  const float errorMagnitude = glm::length(error);

  float acceleration = MIN_ACC;

  if (errorMagnitude > 0.0f){
    const float coefficient = glm::clamp(errorMagnitude / maxSpeed, 0.0f, 1.0f);
    acceleration += (MAX_ACC - MIN_ACC) * coefficient;
  }

  const float maxDeltaV = acceleration * deltaTime;

  glm::vec2 deltaV{0.0f, 0.0f};

  if (errorMagnitude <= maxDeltaV){
    deltaV = error;
  } else if (errorMagnitude > 0.0f){
    deltaV = error * maxDeltaV / errorMagnitude;
  }

  velocity_.x += deltaV.x;
  velocity_.z += deltaV.y; // not typo

  const float dVy = isAirborne() ? -deltaTime * GRAVITY : -velocity_.y;

  velocity_.y += dVy;
}

const bool Player::isAirborne() const
{
  return (pos_.y > FLOOR + LANDING_TOLERANCE) || (velocity_.y > 0.0f);
}

const std::vector<ViewmodelRenderCall*> Player::viewModelVector() const
{
  return vmc_.callVector();
}
