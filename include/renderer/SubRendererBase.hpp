
#pragma once

#include <cstdint>

class SubRendererBase
{
public:
  SubRendererBase(uint32_t width, uint32_t height);
  virtual ~SubRendererBase() = default;

  void setWindowDimensions(uint32_t width, uint32_t height);

protected:
  uint32_t windowWidth_;
  uint32_t windowHeight_;

  virtual void calculateProjection() = 0;
};
