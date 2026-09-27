
#include <glm/glm.hpp>

#include "FlatTransform.hpp"
#include "SpriteId.hpp"
#include "ViewmodelController.hpp"

ViewmodelController::ViewmodelController()
{
  mantleCall_ = ViewmodelRenderCall{
    SpriteId::POV_Mantle,
    0.0f,
    glm::radians(-90.0f),
    {2.0f, 2.0f},
    {0.5f, 0.3f},
    FlatTransform::Rot180
  };
  callVector_.push_back(&mantleCall_);
}

const std::vector<ViewmodelRenderCall*> ViewmodelController::callVector() const
{
  return callVector_;
}
