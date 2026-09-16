
#pragma once

#include <cstdint>
#include <memory>

#include "Input.hpp"
#include "Maze.hpp"
#include "Mesh.hpp"
#include "Player.hpp"
#include "Renderer.hpp"

enum class GameState
{
  Config,
  Playing,
  Won
};

struct GameConfig
{
  int mazeWidth = 9;
  int mazeHeight = 9;
  uint32_t seed = 69420;
};

class Game
{
public:
  Game();
  void run();

private:
  void update(float deltaTime);
  void render();

  void updateConfig(float deltaTime);
  void updatePlaying(float deltaTime);
  void updateWon(float deltaTime);

  void renderConfig();
  void renderPlaying();
  void renderWon();

  void startGame();

  GameState state_ = GameState::Config;
  GameConfig config_;

  std::unique_ptr<Maze> maze_;
  std::unique_ptr<Mesh> mesh_;

  std::unique_ptr<Player> player_;
  std::unique_ptr<Input> input_;
  std::unique_ptr<Renderer> renderer_;

  bool mapVisible_ = false;
};
