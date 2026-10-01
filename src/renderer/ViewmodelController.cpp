
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "FlatTransform.hpp"
#include "SpriteId.hpp"
#include "ViewmodelController.hpp"

ViewmodelController::ViewmodelController()
{
  flapCall_ = ViewmodelRenderCall{
    SpriteId::POV_Flap,
    {-0.15f, -0.6f, 0.0f},
    {
      std::sin(glm::radians(flapBaseFloatAngle)),
      std::cos(glm::radians(flapBaseFloatAngle)),
      0.0f
    },
    {0.4f, 0.2f},
    {0.5f, 1.0f},
    {0.0f, false, true}
  };
  callVector_.push_back(&flapCall_);

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

const std::vector<ViewmodelRenderCall*> ViewmodelController::callVector() const
{
  return callVector_;
}

void ViewmodelController::updateFlaps(glm::vec3 localVelocity, float maxSpeed)
{
  float flapFloatAngleDiff = glm::clamp(localVelocity.x / maxSpeed, -1.0f, 1.0f) * flapMaxFloatAngleDiff;
  flapCall_.localQuadNormal = {
    std::sin(glm::radians(flapBaseFloatAngle + flapFloatAngleDiff)),
    std::cos(glm::radians(flapBaseFloatAngle + flapFloatAngleDiff)),
    0.0f
  };
  float flapSway = glm::clamp(localVelocity.z / maxSpeed, -flapMaxSway, flapMaxSway);
  flapCall_.tf.rot = glm::pi<float>() + flapSway;
}
