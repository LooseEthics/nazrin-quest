
#include <glm/gtc/constants.hpp>

#include "Player.hpp"

Player::Player(vec3f startPosition)
  : pos_(startPosition),
    camera_(
      startPosition.x,
      startPosition.y + 1.0f,
      startPosition.z,
      -glm::half_pi<float>(),
      0.0f)
{}


Camera& Player::camera()
{
  return camera_;
}
