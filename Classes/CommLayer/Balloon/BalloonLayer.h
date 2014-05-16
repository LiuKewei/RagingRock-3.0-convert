#ifndef __BALLOON_LAYER_H__
#define __BALLOON_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Balloon.h"
#include "MsgTypeForObserver.h"
#else
#include "Entity/Balloon.h"
#include "Tools/MsgTypeForObserver.h"
#endif

class BalloonLayer : public cocos2d::Layer
{
public:
	BalloonLayer();
	virtual ~BalloonLayer();
	virtual bool init();
	CREATE_FUNC(BalloonLayer);

	// Touches Callback function
	virtual bool TouchBegan(Touch* touch, Event* event);
	virtual void TouchMoved(Touch* touch, Event* event);
	virtual void TouchEnded(Touch* touch, Event* event);


private:
	void initBalloon();

private:
	EventListenerTouchOneByOne* m_listener;
	Size  m_winSize;

	Balloon* m_balloon;
};

#endif // __BALLOON_LAYER_H__