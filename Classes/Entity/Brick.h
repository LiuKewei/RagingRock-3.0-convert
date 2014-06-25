#ifndef __BRICK_H__
#define __BRICK_H__

#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;


class Brick : public Entity
{
public:
	Brick();
	~Brick();
	virtual bool init();
	CREATE_FUNC(Brick);

private:

	
};
#endif // __BRICK_H__