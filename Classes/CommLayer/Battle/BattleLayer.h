#ifndef __BATTLE_LAYER_H__
#define __BATTLE_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "ReversibleCard.h"
#include "MsgTypeForObserver.h"
#else
#include "Tools/ReversibleCard.h"
#include "Tools/MsgTypeForObserver.h"
#endif

class BattleLayer : public cocos2d::Layer
{
	enum
	{
		TAG_BATTLE = 37250,
	};



public:
	BattleLayer();
	virtual ~BattleLayer();
	virtual bool init();
	CREATE_FUNC(BattleLayer);

	// Touches Callback function
	virtual bool TouchBegan(Touch* touch, Event* event);
	virtual void TouchMoved(Touch* touch, Event* event);
	virtual void TouchEnded(Touch* touch, Event* event);

private:
	void battleGameStart(Ref* pData);

	void initBattleGame();
private:
	EventListenerTouchOneByOne* m_listener;
	Size  m_winSize;

};

#endif // __BATTLE_LAYER_H__