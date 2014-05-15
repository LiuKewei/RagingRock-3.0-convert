#ifndef __DEVIL_H__
#define __DEVIL_H__

#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Devil: public Entity
{
public:	
	Devil();	
	~Devil();
	virtual bool init();
	CREATE_FUNC(Devil);

	std::vector<Point> getDevilPosVec();

	Point getDevilTmpPos();
	void setDevilTmpPos(Point p);

private:
	std::vector<Point> m_devilPosVec;

	Point m_tmpPos;

};
#endif // __DEVIL_H__