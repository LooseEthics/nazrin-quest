
#pragma once

#include <SDL3/SDL.h>

class Input
{
public:
  void processEvent(const SDL_Event& event);

  bool quitRequested() const noexcept;

  bool forward() const noexcept;
  bool backward() const noexcept;
  bool right() const noexcept;
  bool left() const noexcept;

  bool mouseCaptured() const noexcept;

  float mouseDeltaX() const noexcept;
  float mouseDeltaY() const noexcept;

  void endFrame();

private:
  bool quitRequested_ = false;

  bool forward_ = false;
  bool backward_ = false;
  bool right_ = false;
  bool left_ = false;

  bool mouseCaptured_ = true;

  float mouseDeltaX_ = 0.0f;
  float mouseDeltaY_ = 0.0f;
};
