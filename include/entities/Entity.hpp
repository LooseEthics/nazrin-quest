
#pragma once

#include <glm/glm.hpp>

#include "SpriteId.hpp"
#include "SpriteRenderCall.hpp"

class Entity
{
public:
  Entity(glm::vec3 position);
  Entity(glm::vec3 position, SpriteId sprite);
  Entity(glm::vec3 position, SpriteId sprite, float width, float height);
  virtual ~Entity() = default;

  virtual void update(float deltaTime) = 0;

  void setSprite(SpriteId sprite);
  void setSpriteSize(float width, float height);

  glm::vec3 position() const;
  SpriteId sprite() const;
  float width() const;
  float height() const;

  SpriteRenderCall renderCall() const;

protected:
  glm::vec3 position_;
  SpriteId sprite_;
  float spriteWidth_;
  float spriteHeight_;
};
