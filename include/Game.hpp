
#pragma once

#include <cstdint>
#include <memory>

#include "Entity.hpp"
#include "Input.hpp"
#include "Maze.hpp"
#include "Mesh.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "VisibilitySystem.hpp"

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

enum class ConfigField
{
  Width,
  Height,
  Seed,
  Start,
  Count
};

inline ConfigField& operator++(ConfigField& field)
{
  field = static_cast<ConfigField>((static_cast<int>(field) + 1) % static_cast<int>(ConfigField::Count));
  return field;
}

inline ConfigField& operator--(ConfigField& field)
{
  field = static_cast<int>(field) > 0 ?
    static_cast<ConfigField>(static_cast<int>(field) - 1) :
    static_cast<ConfigField>(static_cast<int>(ConfigField::Count) - 1);
  return field;
}

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

  void changeConfigValue(ConfigField field, int increment);

  void startGame();

  GameState state_ = GameState::Config;
  GameConfig config_;
  ConfigField configField_ = ConfigField::Start;

  std::unique_ptr<Maze> maze_;
  std::unique_ptr<Mesh> mesh_;

  std::unique_ptr<Player> player_;
  std::unique_ptr<Input> input_;
  std::unique_ptr<Renderer> renderer_;

  bool mapVisible_ = false;
  float winTime_ = 0.0f;

  std::vector<std::unique_ptr<Entity>> entities_;

  std::unique_ptr<VisibilitySystem> visibility_;
  std::set<MazeCoord> visibleCells_;
  std::set<Entity*> visibleEntities_;
};
