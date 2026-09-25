
#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "FlatTransform.hpp"
#include "SpriteId.hpp"

struct ViewmodelRenderCall
{
  SpriteId sprite;
  float cameraRelativeYaw;
  float worldRelativePitch;
  glm::vec2 size;
  glm::vec2 uvAnchorPoint;
  FlatTransform tf;
};
