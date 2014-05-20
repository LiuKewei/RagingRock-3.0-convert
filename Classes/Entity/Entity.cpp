#include "Entity.h"

Entity::Entity()
{
	m_sprite = NULL;
}
Entity::~Entity()
{

}

void Entity::drawFunc(const kmMat4 &transform, bool transformUpdated)
{

}

Sprite* Entity::getSprite()
{
	return this->m_sprite;
}

void Entity::setSpriteAsNULL()
{
	this->m_sprite = NULL;
}

void Entity::bindSprite(Sprite* sprite)
{
	this->m_sprite = sprite;
	this->addChild(m_sprite);
}