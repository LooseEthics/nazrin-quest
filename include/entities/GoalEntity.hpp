
#pragma once

#include "Entity.hpp"

class GoalEntity : public Entity
{
public:
  GoalEntity(glm::vec3 position);

  void update(float deltaTime) override;

private:
  const float y0_;
  const float amplitude_ = 0.1f;
  const float period_ = 1.0f;

  float timer = 0.0f;
};
