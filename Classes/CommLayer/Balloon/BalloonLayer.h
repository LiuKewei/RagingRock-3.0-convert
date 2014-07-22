#ifndef __BALLOON_LAYER_H__
#define __BALLOON_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Balloon.h"
#include "MsgTypeForObserver.h"
#else
#include "Entity/Balloon.h"
#include "Tools/MsgTypeForObserver.h"
#endif

const float c_balloonTiming = 30.0f;


class BalloonLayer : public cocos2d::Layer
{
	enum
	{
		TAG_BALLOON = 38250,
		TAG_BALLOON_STAT,
		TAG_BALLOON_SUCC,
	};

public:
	BalloonLayer();
	virtual ~BalloonLayer();
	virtual bool init();
	CREATE_FUNC(BalloonLayer);

	// Touches Callback function
	virtual bool TouchBegan(Touch* touch, Event* event);
	virtual void TouchMoved(Touch* touch, Event* event);
	virtual void TouchEnded(Touch* touch, Event* event);

	void bombedreset(float dt);
	void unbombedreset(float dt);
	void playSucc(float dt);
	void timing(float dt);
private:
	void initBalloon();
	void initLabels();

	void balloonGameStart(Ref* pData);

private:
	EventListenerTouchOneByOne* m_listener;
	Size  m_winSize;

	Label* m_bomb;
	Label* m_balloonLabel;

	Label* m_succCntLabel;
	Label* m_timeLabel;
	float m_timing;

	Widget* m_balloonLayout;
};

#endif // __BALLOON_LAYER_H__