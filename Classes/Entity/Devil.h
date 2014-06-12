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

	std::vector<Point>* getDevilPosVec();

	void setDevilMaxIndexInCurrent(unsigned int index);
	unsigned int getDevilMaxIndexInCurrent();

	void setDevilPosCnt(unsigned int devilPosCnt);
	unsigned int getDevilPosCnt();
	
private:
	std::vector<Point>* m_devilPosVec;
	unsigned int m_devilPosCnt;
	unsigned int m_currentMaxIndex;


};
#endif // __DEVIL_H__