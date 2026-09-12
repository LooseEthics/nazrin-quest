
#pragma once

#include <cstdint>
#include <vector>

#include "Direction.hpp"

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
