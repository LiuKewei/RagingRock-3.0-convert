#ifndef __TREASURE_H__
#define __TREASURE_H__

#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Treasure : public Entity
{
public:
	Treasure();
	~Treasure();
	virtual bool init();
	CREATE_FUNC(Treasure);

private:

	Vector<Point>* m_allPossiblePos;

	Vector<Point>* m_allValidPos;
};
#endif // __TREASURE_H__