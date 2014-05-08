#include "Ball.h"


Ball::Ball()
	: m_isMoving(false)
	, m_movingDirection(true)
	, m_movingSpeed(1.0)
{
}

Ball::~Ball()
{
}

bool Ball::init()
{
	return true;
}

bool Ball::isMoving()
{
	return this->m_isMoving;
}
void Ball::setMoving(bool isMoving)
{
	this->m_isMoving = isMoving;
}
bool Ball::getMovingDirection()
{
	return this->m_movingDirection;
}
void Ball::setMovingDirection(bool movingDirection)
{
	this->m_movingDirection = movingDirection;
}
float Ball::getMovingSpeed()
{
	return this->m_movingSpeed;
}
void Ball::setMovingSpeed(float movingSpeed)
{
	this->m_movingSpeed = movingSpeed;
}
Size Ball::getBallSize()
{
	return this->m_ballSize;
}
void Ball::setBallSize(Size ballSize)
{
	this->m_ballSize = ballSize;
}

Rect Ball::getBoundingBox()
{
	if(getSprite() == NULL)
	{
		return Rect(0,0,0,0);
	}

	Point entityPos = getPosition();

	return Rect(entityPos.x - m_ballSize.width/2, entityPos.y - m_ballSize.height/2, m_ballSize.width, m_ballSize.height);
}