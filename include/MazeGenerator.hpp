
#include <cstdint>
#include <random>

#include "Maze.hpp"

struct LogicCell
{
  // 0x1 east passage
  // 0x2 south passage
  // 0x4 west passage
  // 0x8 north passage
  // 0x10 is built
  // 0x11 is start
  // 0x12 is goal
  uint8_t data;
};

class MazeGenerator
{
  // LERW
public:
  MazeGenerator(std::uint32_t seed);

  [[nodiscard]]
  Maze generate(int width, int height);

private:
  std::mt19937 rng_;
};
