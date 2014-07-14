#ifndef __BRICK_H__
#define __BRICK_H__

#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;


enum brickShapeEnum
{
	SHAPE_TRIANGLE,
	SHAPE_SQUARE,
	SHAPE_CIRCLE,
};

enum brickColorEnum
{
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW,
};

class Brick : public Entity
{
public:
	Brick();
	~Brick();
	virtual bool init();
	CREATE_FUNC(Brick);


	void setShape(int shape);
	int getShape();
	void setColor(int color);
	int getColor();

private:

private:
	int m_shape;
	int m_color;
	
};
#endif // __BRICK_H__