
#include <glm/gtc/constants.hpp>

#include "GoalEntity.hpp"

GoalEntity::GoalEntity(glm::vec3 position)
  : Entity(position, SpriteId::Cheese),
    y0_(position.y)
{}

void GoalEntity::update(float deltaTime)
{
  timer += deltaTime;
  if (timer >= period_) timer -= period_;
  position_.y = y0_ + amplitude_ * std::sin(2 * glm::pi<float>() * timer / period_);
}
