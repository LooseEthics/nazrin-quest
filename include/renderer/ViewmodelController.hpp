
#pragma once

#include <glm/gtc/constants.hpp>
#include <vector>

#include "ViewmodelRenderCall.hpp"

class ViewmodelController
{
public:
  ViewmodelController(float maxSpeed);

  const std::vector<ViewmodelRenderCall*>& callVector() const;
  void update(const glm::vec3& velocity, float yawSpeed, float deltaTime);

private:
  void updateFlaps(const glm::vec3& velocity, float yawSpeed);
  void updateShoes(const glm::vec3& velocity, float deltaTime);

  std::vector<ViewmodelRenderCall*> callVector_;

  const int maxSpeed_;

  ViewmodelRenderCall mantleCall_;

  void initializeFlaps();

  struct Flap
  {
    float baseYaw;
    glm::vec3 cameraRelativePos;
  };

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
  const float flapSwayYawSpeedMult_ = 5.0f;

  struct Shoe
  {
    bool isRight;
    glm::vec3 cameraRelativePos;
  };

  std::vector<Shoe> shoes_;
  std::vector<ViewmodelRenderCall> shoeCallVector_;
  float shoeCycleParam_ = 0.0f;

  const float shoeMaxForward_ = 0.15f;
  const float shoeMaxBackward_ = 0.1f;
  const float shoeSideOffset_ = 0.2f;
  const float shoeVertOffset = -0.8f;
  const float shoeBaseForward_ = -0.2f;
};
