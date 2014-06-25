#ifndef __PARABOLA_ACTION_H__
#define __PARABOLA_ACTION_H__

#include "cocos2d.h"

USING_NS_CC;


class ParabolaAction : public ActionInterval
{
public:
	ParabolaAction();
	~ParabolaAction();
	static ParabolaAction* create(float duration, float posX, float posY);

private:

};
#endif // __PARABOLA_ACTION_H__