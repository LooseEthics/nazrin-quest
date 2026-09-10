
#include <cstdint>
#include <vector>

const int MAZE_MIN_SIZE = 3;

enum class Cell : std::uint8_t
{
  Wall,
  Empty
};

class Maze
{
public:
  Maze(int width, int height);

  [[nodiscard]]
  bool isInside(int x, int y) const;

  [[nodiscard]]
  Cell get(int x, int y) const;

  void set(int x, int y, Cell cell);

  [[nodiscard]]
  int width() const noexcept;
  [[nodiscard]]
  int height() const noexcept;

  void printMazeToConsole();

private:
  int width_;
  int height_;
  std::vector<Cell> cells_;
};
