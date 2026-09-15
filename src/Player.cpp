
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
  constexpr float MOVE_SPEED = 5.0f;
  constexpr float LOOK_SPEED = 0.0025f;
  const glm::vec3 forwardVec = flatForwardVector(yaw_, pitch_);

  float forward = 0.0f;
  float right = 0.0f;

  if (input.forward()) forward += 1.0;
  if (input.backward()) forward -= 1.0;
  if (input.right()) right += 1.0;
  if (input.left()) right -= 1.0;

  if (forward != 0 && right != 0){
    forward *= glm::one_over_root_two<float>();
    right *= glm::one_over_root_two<float>();
  }

  float dx = (forwardVec.x * forward - forwardVec.z * right) * MOVE_SPEED * deltaTime;
  float dz = (forwardVec.z * forward + forwardVec.x * right) * MOVE_SPEED * deltaTime;

  move(dx, dz);

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

void Player::move(float dx, float dz)
{
  // assuming distance to move is less than collision radius
  if (dx == 0.0f && dz == 0.0f) return;

  glm::vec3 candidate = pos_;

  candidate.x += dx;
  resolveCollision(candidate);

  candidate.z += dz;
  resolveCollision(candidate);

  pos_ = candidate;
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

Maze::Coord Player::mazeCell() const
{
  return {
    static_cast<int>(std::floor(pos_.x / CELL_SIZE)),
    static_cast<int>(std::floor(pos_.z / CELL_SIZE))
  };
}

bool Player::goalReached() const noexcept
{
  return mazeCell() == maze_.getGoal();
}
