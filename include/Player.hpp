
#pragma once

#include "Camera.hpp"
#include "CommonGeometry.hpp"

class Player
{
public:
  Player(vec3f startPosition);

  Camera& camera();

  
private:
  vec3f pos_;
  Camera camera_;
};
