
#include <glm/vec3.hpp>

#include "Entity.hpp"
#include "Maze.hpp"

class PhysicalEntity : public Entity
{
public:
  PhysicalEntity(glm::vec3 position, Maze& maze);

protected:
  glm::vec3 velocity_ = {0.0f, 0.0f, 0.0f};

  Maze& maze_;

  static constexpr float collisionRadius_ = CELL_SIZE / 8.0f;

  bool resolveVertCollision(glm::vec3& position);
  bool resolveCollision(glm::vec3& position);
  bool resolveCircleAABB(
    glm::vec3& position,
    float minX, float maxX,
    float minZ, float maxZ
  );
};
