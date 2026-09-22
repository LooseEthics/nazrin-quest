
#include "CommonGeometry.hpp"
#include "Entity.hpp"

Entity::Entity(glm::vec3 position)
  : Entity{position, SpriteId::Default}
{}

Entity::Entity(glm::vec3 position, SpriteId sprite)
  : Entity{position, sprite, CELL_SIZE / 2, CELL_SIZE / 2}
{}

Entity::Entity(glm::vec3 position, SpriteId sprite, float width, float height)
  : position_(position),
    sprite_(sprite),
    spriteWidth_(width),
    spriteHeight_(height)
{}

void Entity::setSprite(SpriteId sprite)
{
  sprite_ = sprite;
}

void Entity::setSpriteSize(float width, float height)
{
  spriteWidth_ = width;
  spriteHeight_ = height;
}

glm::vec3 Entity::position() const {return position_;}
SpriteId Entity::sprite() const {return sprite_;}
float Entity::width() const {return spriteWidth_;}
float Entity::height() const {return spriteHeight_;}
