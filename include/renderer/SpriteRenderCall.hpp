
#pragma once

#include <glm/vec3.hpp>

#include "SpriteId.hpp"

struct SpriteRenderCall
{
  SpriteId sprite;
  glm::vec3 position;
  float width;
  float height;
};
