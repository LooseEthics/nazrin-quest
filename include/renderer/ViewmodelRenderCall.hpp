
#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "FlatTransform.hpp"
#include "SpriteId.hpp"

struct ViewmodelRenderCall
{
  SpriteId sprite;
  glm::vec3 cameraRelativePos;
  glm::vec3 localQuadNormal;
  glm::vec2 size;
  glm::vec2 uvAnchorPoint;
  FlatTransform tf;
};
