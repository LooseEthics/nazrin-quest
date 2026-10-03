
#include <glm/glm.hpp>

#include "FlatTransform.hpp"
#include "SpriteId.hpp"
#include "ViewmodelController.hpp"

ViewmodelController::ViewmodelController()
{
  initializeFlaps();
  for (auto& fc : flapCallVector_){
    callVector_.push_back(&fc);
  }

  mantleCall_ = ViewmodelRenderCall{
    SpriteId::POV_Mantle,
    {0.0f, -0.5f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {2.0f, 2.0f},
    {0.5f, 0.3f},
    {0.0f, false, true}
  };
  callVector_.push_back(&mantleCall_);
}

const std::vector<ViewmodelRenderCall*>& ViewmodelController::callVector() const
{
  return callVector_;
}

void ViewmodelController::updateFlaps(glm::vec3 localVelocity, float maxSpeed, float yawSpeed)
{
  const float flapFloatAngleDiff = glm::clamp(localVelocity.x / maxSpeed, -1.0f, 1.0f) * flapMaxFloatAngleDiff_;
  const float flapSway = glm::clamp(yawSpeed * flapSwayYawSpeedMult_ + localVelocity.z / maxSpeed, -flapMaxSway_, flapMaxSway_);
  for (std::size_t i = 0; i < flaps_.size(); ++i){
    auto& fc = flapCallVector_[i];
    auto& flap = flaps_[i];
    fc.localQuadNormal = {
      std::sin(glm::radians(flapBaseFloatAngle_ + flapFloatAngleDiff)) * std::cos(glm::radians(flap.baseYaw)),
      std::cos(glm::radians(flapBaseFloatAngle_ + flapFloatAngleDiff)),
      std::sin(glm::radians(flapBaseFloatAngle_ + flapFloatAngleDiff)) * std::sin(glm::radians(flap.baseYaw))
    };
    fc.tf.rot = flapBaseRotation_ + flapSway;
  }
}

void ViewmodelController::initializeFlaps()
{
  if (flapLayers_ > 1){
    float flapAngleSpacing = flapForwardAngle_ / (2 * flapLayers_ - 2);
    for (int layer = flapLayers_; layer > 1; --layer){
      const float angleOffset = (layer - 1) * flapAngleSpacing;
      const float radOffset = glm::radians(angleOffset);
      const float xOffset = flapCircleCenter_.x + flapCircleRadius_ * std::cos(radOffset);
      const float zOffset = flapCircleCenter_.y + flapCircleRadius_ * std::sin(radOffset);
      flaps_.push_back({
        angleOffset,
        {
          xOffset,
          flapVertOffset_,
          zOffset
        }
      });
      flaps_.push_back({
        -angleOffset,
        {
          xOffset,
          flapVertOffset_,
          -zOffset
        }
      });
    }
  }

  // last flap always forward center
  flaps_.push_back({
    0.0f,
    {
      flapCircleCenter_.x + flapCircleRadius_,
      flapVertOffset_,
      flapCircleCenter_.y
    }
  });

  for (const auto& flap : flaps_){
    flapCallVector_.push_back({
      SpriteId::POV_Flap,
      flap.cameraRelativePos,
      {
        std::sin(glm::radians(flapBaseFloatAngle_)) * std::cos(glm::radians(flap.baseYaw)),
        std::cos(glm::radians(flapBaseFloatAngle_)),
        std::sin(glm::radians(flapBaseFloatAngle_)) * std::sin(glm::radians(flap.baseYaw))
      },
      flapSize_,
      flapAnchor_,
      flapTf_
    });
  }
}
