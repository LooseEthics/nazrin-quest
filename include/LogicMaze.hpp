
#pragma once

#include <cstdint>
#include <vector>

constexpr uint8_t D_EAST  = 0x01;
constexpr uint8_t D_SOUTH = 0x02;
constexpr uint8_t D_WEST  = 0x04;
constexpr uint8_t D_NORTH = 0x08;

constexpr uint8_t C_BUILT = 0x10;
constexpr uint8_t C_START = 0x20;
constexpr uint8_t C_GOAL = 0x40;

enum class Direction : uint8_t
{
  East = 0,
  South = 1,
  West = 2,
  North = 3,
  Invalid = 4
};

constexpr uint8_t directionMask(Direction dir)
{
  if (dir != Direction::Invalid) return 1u << static_cast<uint8_t>(dir);
  return 0x0;
}

constexpr Direction opposite(Direction dir)
{
  return static_cast<Direction>((static_cast<uint8_t>(dir) + 2) % 4);
}

class LogicMaze
{
public:
  LogicMaze(size_t width, size_t height);
  LogicMaze();

  size_t width;
  size_t height;
  std::vector<uint8_t> data;
  int unbuiltCells;

  [[nodiscard]]
  size_t size() const noexcept;

  void printHexes() const;

  [[nodiscard]]
  size_t getNextUnbuilt(size_t offset) const noexcept;
};
