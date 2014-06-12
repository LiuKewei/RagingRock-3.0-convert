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

#define CARD_TILT_ANGLE (75)

const int c_intervalCardHeight = 12;

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
		TAG_BATTLE = 37250,
		TAG_BATTLE_LEFT,
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
	void initBattleGame();
	void initLabels();
	void initBgHint();

	void battleGameStart(Ref* pData);

	void pushCards();
	void increaseCards4Groups(int groupCnt);
	void pileUpCards();
	void pileUpOneGroupCardsToTail();
	void showCard(Node* sender);

	void battle();
	void attack(int cnt);
	void injuredOrCure(int cnt);//负数受伤，正数治疗

	void showBgHint();

private:
	Size m_winSize;

	std::list<ReversibleCard*>* m_battleCardGroups;
	std::vector<ReversibleCard*>* m_currentCardGroup;

	EventListenerTouchOneByOne* m_listener;

	ReversibleCard* m_targetCard;

	Label* m_devilHPLabel;
	int m_devilHP;
	Label* m_leadHPLabel;
	int m_leadHP;

	int m_hintInBackground;
	std::vector<int> m_hintVec;

	bool m_isOpening;

	float m_openCardDuration;
	float m_pileUpHeight;
	float m_openingHeight;
};

#endif // __BATTLE_LAYER_H__