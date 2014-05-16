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
	
	void setCounter(unsigned int counter);
	unsigned int getCounter();
	
	void setMaxCnt(unsigned int maxCnt);
	unsigned int getMaxCnt();

	void setSuccessCnt(unsigned int successCnt);
	unsigned int getSuccessCnt();

	bool isbombed();
	void setbombed(bool isbombed);

private:
	unsigned int m_counter;
	unsigned int m_maxCnt;
	unsigned int m_successCnt;

	bool m_isbombed;
};
#endif // __BALLOON_H__