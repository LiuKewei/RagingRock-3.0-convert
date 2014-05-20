#ifndef __FINGER_H__
#define __FINGER_H__

#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Finger : public Entity
{
public:
	Finger();
	~Finger();
	virtual bool init();
	CREATE_FUNC(Finger);
	

protected:
	Point m_startPoint;
	Point m_endPoint;
};
#endif // __FINGER_H__