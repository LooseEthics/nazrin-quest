
#pragma once

#include <vector>

#include "ViewmodelRenderCall.hpp"

class ViewmodelController
{
public:
  ViewmodelController();

  const std::vector<ViewmodelRenderCall*> callVector() const;
  void updateFlaps(glm::vec3 velocity, float maxSpeed);

private:
  std::vector<ViewmodelRenderCall*> callVector_;

  ViewmodelRenderCall mantleCall_;
  ViewmodelRenderCall flapCall_;

  float flapBaseYsize = 0.2f;
  float flapMaxSway = 0.1f;
  float flapBaseFloatAngle = 30.0f;
  float flapMaxFloatAngleDiff = 15.0f;
};
