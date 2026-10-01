
#pragma once

struct FlatTransform
{
  float rot;
  bool flipX;
  bool flipY;
};

constexpr FlatTransform NO_TRANSFORM{0.0f, false, false};
