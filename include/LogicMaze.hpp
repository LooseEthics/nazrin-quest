
#pragma once

#include <cstdint>
#include <vector>

class LogicMaze
{
public:
  LogicMaze(size_t width, size_t height);
  LogicMaze();

  size_t width;
  size_t height;
  // 0x01 east passage
  // 0x02 south passage
  // 0x04 west passage
  // 0x08 north passage
  // 0x10 is built
  // 0x20 is start
  // 0x40 is goal
  std::vector<uint8_t> data;

  [[nodiscard]]
  size_t size() const noexcept;
};
