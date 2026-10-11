
#include <cmath>

#include "PhysicalEntity.hpp"

PhysicalEntity::PhysicalEntity(glm::vec3 position, Maze& maze)
  : Entity(position),
    maze_(maze)
{}

bool PhysicalEntity::resolveVertCollision(glm::vec3& position)
{
  if (position.y <= FLOOR + LANDING_TOLERANCE && velocity_.y <= 0.0f){
    position.y = FLOOR;
    return true;
  }
  return false;
}

bool PhysicalEntity::resolveCollision(glm::vec3& position)
{
  const int minCellX = static_cast<int>(std::floor((position.x - collisionRadius_) / CELL_SIZE));
  const int maxCellX = static_cast<int>(std::floor((position.x + collisionRadius_) / CELL_SIZE));
  const int minCellZ = static_cast<int>(std::floor((position.z - collisionRadius_) / CELL_SIZE));
  const int maxCellZ = static_cast<int>(std::floor((position.z + collisionRadius_) / CELL_SIZE));

  bool collided = false;

  for (int z = minCellZ; z <= maxCellZ; ++z){
    for (int x = minCellX; x <= maxCellX; ++x){

      if (!maze_.isInside(x, z)) continue;

      if (maze_.get(x, z) != Cell::Wall) continue;

      const float minX = x * CELL_SIZE;
      const float maxX = (x + 1) * CELL_SIZE;
      const float minZ = z * CELL_SIZE;
      const float maxZ = (z + 1) * CELL_SIZE;

      collided |= resolveCircleAABB(
        position,
        minX, maxX,
        minZ, maxZ
      );
    }
  }
  return collided;
}

bool PhysicalEntity::resolveCircleAABB(
  glm::vec3& position,
  float minX, float maxX,
  float minZ, float maxZ
) {
  const float closestX = glm::clamp(position.x, minX, maxX);
  const float closestZ = glm::clamp(position.z, minZ, maxZ);

  float dx = position.x - closestX;
  float dz = position.z - closestZ;

  const float distanceSquared = dx * dx + dz * dz;
  const float radiusSquared = collisionRadius_ * collisionRadius_;

  if (distanceSquared >= radiusSquared) return false;

  // if distanceSquared == 0, then the player is in a wall - this shouldn't happen unless going very very fast
  // add handling for this if it becomes relevant

  const float distance = std::sqrt(distanceSquared);

  if (distance > 0.0f){
    glm::vec3 normal{dx / distance, 0.0f, dz/distance};
    float penetration = collisionRadius_ - distance;

    position += normal * penetration;

    const float normalVelocity = glm::dot(velocity_, normal);

    if (normalVelocity < 0.0f){
      velocity_ -= normal * normalVelocity;
    }
  }
  return true;
}
