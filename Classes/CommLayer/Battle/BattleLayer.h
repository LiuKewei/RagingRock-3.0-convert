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

#define CARD_TILT_ANGLE (55)

class BattleLayer : public cocos2d::Layer
{
	enum
	{
		TYPE_BATTLE_LEAD_NORMAL,
		TYPE_BATTLE_LEAD_MAGIC,
		TYPE_BATTLE_LEAD_INVINCIBLE,
		TYPE_BATTLE_LEAD_PET,
		TYPE_BATTLE_LEAD_DEVIL,
	};

	enum
	{
		TAG_BATTLE_LEFT = 37250,
		TAG_BATTLE_MID,
		TAG_BATTLE_RIGHT,

		TAG_BATTLE_CURRENT,
	};

public:
	BattleLayer();
	virtual ~BattleLayer();
	virtual bool init();
	CREATE_FUNC(BattleLayer);

	// Touches Callback function
	virtual bool TouchBegan(Touch* touch, Event* event);

	void update(float dt);
	void waitingForOpened(float dt);
private:
	void battleGameStart(Ref* pData);
	void initBattleGame();

	void pushCards();

	void pileUpCards();

private:
	Size m_winSize;
	std::list<ReversibleCard*>* m_battleCardGroups;
	std::vector<ReversibleCard*>* m_currentCardGroup;

	EventListenerTouchOneByOne* m_listener;

	ReversibleCard* m_targetCard;

	bool m_isOpening;
	float m_openCardDuration;


};

#endif // __BATTLE_LAYER_H__