#include "Brick.h"

Brick::Brick()
: m_color(-1)
, m_shape(-1)
{
}

Brick::~Brick()
{
}

bool Brick::init()
{
	return true;
}



void Brick::setShape(int shape)
{
	this->m_shape = shape;
}
int Brick::getShape()
{
	return this->m_shape;
}
void Brick::setColor(int color)
{
	this->m_color = color;
}
int Brick::getColor()
{
	return this->m_color;
}
