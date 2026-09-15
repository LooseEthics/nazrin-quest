
#include <glm/gtc/constants.hpp>

#include "Player.hpp"

Player::Player(glm::vec3 startPosition)
  : pos_(startPosition),
    yaw_(-glm::half_pi<float>()),
    pitch_(0.0f),
    camera_(
      startPosition + cameraOffset(),
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

  float forward = 0.0f;
  float right = 0.0f;

  if (input.forward()) forward += 1.0;
  if (input.backward()) forward -= 1.0;
  if (input.right()) right += 1.0;
  if (input.left()) right -= 1.0;

  moveForward(forward * MOVE_SPEED * deltaTime);
  moveRight(right * MOVE_SPEED * deltaTime);

  if (input.mouseCaptured())
    rotate(
      input.mouseDeltaX() * LOOK_SPEED,
      -input.mouseDeltaY() * LOOK_SPEED
    );

  camera_.setPos(pos_ + cameraOffset());
  camera_.setRot(yaw_, pitch_);

}

void Player::move(float dx, float dy, float dz)
{
  pos_ += glm::vec3{dx, dy, dz};
}

void Player::setPos(float x, float y, float z)
{
  pos_ = glm::vec3{x, y, z};
}

void Player::moveForward(float distance)
{
  const glm::vec3 forward = flatForwardVector(yaw_, pitch_);
  pos_ += glm::vec3{forward.x, 0.0f, forward.z} * distance;
}

void Player::moveRight(float distance)
{
  const glm::vec3 forward = flatForwardVector(yaw_, pitch_);
  pos_ += glm::vec3{-forward.z, 0.0f, forward.x} * distance;
}

void Player::rotate(float dYaw, float dPitch, bool pitchClamp)
{
  yaw_ += dYaw;
  if (pitchClamp)
    pitch_ += dPitch;
  else
    pitch_ = glm::clamp(
      pitch_ + dPitch,
      -maxPitch,
      maxPitch
    );
}

void Player::rotate(float dYaw, float dPitch)
{
  rotate(dYaw, dPitch, true);
}

glm::vec3 Player::cameraOffset(){
  return {0.0f, 1.0f, 0.0f};
}
