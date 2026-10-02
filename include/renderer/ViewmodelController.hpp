
#pragma once

#include <glm/gtc/constants.hpp>
#include <vector>

#include "ViewmodelRenderCall.hpp"

struct Flap
{
  float baseYaw;
  glm::vec3 cameraRelativePos;
};

class ViewmodelController
{
public:
  ViewmodelController();

  const std::vector<ViewmodelRenderCall*>& callVector() const;
  void updateFlaps(glm::vec3 velocity, float maxSpeed);

private:
  std::vector<ViewmodelRenderCall*> callVector_;

  ViewmodelRenderCall mantleCall_;

  void initializeFlaps();

  std::vector<Flap> flaps_;
  std::vector<ViewmodelRenderCall> flapCallVector_;

  const int flapLayers_ = 4;
  const float flapForwardAngle_ = 90.0f;
  const float flapVertOffset_ = -0.6f;
  const float flapCircleRadius_ = 0.6f;
  const glm::vec2 flapCircleCenter_ = {-0.75f, 0.0f};

  const glm::vec2 flapSize_ = {0.3f, 0.2f};
  const glm::vec2 flapAnchor_ = {0.5f, 1.0f};
  const float flapBaseRotation_ = glm::pi<float>();
  const FlatTransform flapTf_ = {flapBaseRotation_, false, true};

  const float flapMaxSway_ = 0.1f;
  const float flapBaseFloatAngle_ = 45.0f;
  const float flapMaxFloatAngleDiff_ = 15.0f;
};
