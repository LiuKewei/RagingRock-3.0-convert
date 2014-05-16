#ifndef __BALLOON_H__
#define __BALLOON_H__

#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Balloon : public Entity
{
public:
	Balloon();
	~Balloon();
	virtual bool init();
	CREATE_FUNC(Balloon);

private:
	
};
#endif // __BALLOON_H__