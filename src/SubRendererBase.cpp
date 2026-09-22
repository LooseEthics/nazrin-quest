
#include "SubRendererBase.hpp"

SubRendererBase::SubRendererBase(uint32_t width, uint32_t height)
  : windowWidth_(width),
    windowHeight_(height)
{}

void SubRendererBase::setWindowDimensions(uint32_t width, uint32_t height)
{
  windowWidth_ = width;
  windowHeight_ = height;
  calculateProjection();
}
