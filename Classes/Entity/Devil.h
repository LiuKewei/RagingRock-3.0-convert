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


//	void setIsTrigger(bool isTrigger);
//	bool getIsTrigger();
//private:
//	bool m_isTrigger;
};
#endif // __DEVIL_H__