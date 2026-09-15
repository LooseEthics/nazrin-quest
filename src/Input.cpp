
#include "Input.hpp"

void Input::processEvent(const SDL_Event& event)
{
  switch(event.type){
    case SDL_EVENT_QUIT:
      quitRequested_ = true;
      break;

    case SDL_EVENT_KEY_DOWN:
      if (event.key.scancode == SDL_SCANCODE_ESCAPE)
        quitRequested_ = true;
      if (!event.key.repeat &&
        ( event.key.scancode == SDL_SCANCODE_LALT ||
          event.key.scancode == SDL_SCANCODE_RALT))
        mouseCaptured_ = !mouseCaptured_;
      if (event.key.scancode == SDL_SCANCODE_W)
        forward_ = true;
      if (event.key.scancode == SDL_SCANCODE_S)
        backward_ = true;
      if (event.key.scancode == SDL_SCANCODE_A)
        left_ = true;
      if (event.key.scancode == SDL_SCANCODE_D)
        right_ = true;
      break;

    case SDL_EVENT_KEY_UP:
      if (event.key.scancode == SDL_SCANCODE_W)
        forward_ = false;
      if (event.key.scancode == SDL_SCANCODE_S)
        backward_ = false;
      if (event.key.scancode == SDL_SCANCODE_A)
        left_ = false;
      if (event.key.scancode == SDL_SCANCODE_D)
        right_ = false;
      break;

    case SDL_EVENT_MOUSE_MOTION:
      mouseDeltaX_ += event.motion.xrel;
      mouseDeltaY_ += event.motion.yrel;
      break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      if (event.button.button == SDL_BUTTON_LEFT)
        mouseCaptured_ = true;
        mouseDeltaX_ = 0.0f;
        mouseDeltaY_ = 0.0f;
      break;
  }
}

bool Input::quitRequested() const noexcept {return quitRequested_;}
void Input::requestQuit() {quitRequested_ = true;}

bool Input::forward() const noexcept {return forward_;}
bool Input::backward() const noexcept {return backward_;}
bool Input::right() const noexcept {return right_;}
bool Input::left() const noexcept {return left_;}

bool Input::mouseCaptured() const noexcept {return mouseCaptured_;}

float Input::mouseDeltaX() const noexcept {return mouseDeltaX_;}
float Input::mouseDeltaY() const noexcept {return mouseDeltaY_;}

void Input::endFrame()
{
  mouseDeltaX_ = 0.0f;
  mouseDeltaY_ = 0.0f;
}
