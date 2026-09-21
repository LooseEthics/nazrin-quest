
#pragma once

#include <SDL3/SDL.h>
#include <unordered_set>

class Input
{
public:
  void processEvent(const SDL_Event& event);

  bool quitRequested() const noexcept;
  void requestQuit();

  bool forward() const noexcept;
  bool backward() const noexcept;
  bool right() const noexcept;
  bool left() const noexcept;
  bool mapTrigger() const noexcept;

  bool mouseCaptured() const noexcept;

  float mouseDeltaX() const noexcept;
  float mouseDeltaY() const noexcept;

  void endFrame();

  bool keyPressed(SDL_Scancode key) const;
  bool keyHeld(SDL_Scancode key) const;

private:
  bool quitRequested_ = false;

  bool forward_ = false;
  bool backward_ = false;
  bool right_ = false;
  bool left_ = false;
  bool mapTrigger_ = false;

  bool mouseCaptured_ = true;

  float mouseDeltaX_ = 0.0f;
  float mouseDeltaY_ = 0.0f;

  std::unordered_set<SDL_Scancode> keysPressed_;
  std::unordered_set<SDL_Scancode> keysHeld_;
};
