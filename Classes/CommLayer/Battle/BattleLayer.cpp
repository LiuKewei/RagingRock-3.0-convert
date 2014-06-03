#include "BattleLayer.h"

BattleLayer::BattleLayer()
{

}
BattleLayer::~BattleLayer()
{

}
bool BattleLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		m_winSize = Director::getInstance()->getWinSize();

		initBattleGame();

		m_listener = EventListenerTouchOneByOne::create();
		m_listener->setSwallowTouches(true);
		m_listener->onTouchBegan = CC_CALLBACK_2(BattleLayer::TouchBegan, this);
		//m_listener->onTouchMoved = CC_CALLBACK_2(BalloonLayer::TouchMoved, this);
		//m_listener->onTouchEnded = CC_CALLBACK_2(BalloonLayer::TouchEnded, this);
		m_listener->setSwallowTouches(false);
		//m_listener->setEnabled(false);
		_eventDispatcher->addEventListenerWithFixedPriority(m_listener, -1);

		NotificationCenter::getInstance()->addObserver(
			this,
			callfuncO_selector(BattleLayer::battleGameStart),
			MsgTypeForObserver::c_BattleStart,
			NULL);






		bRet = true;
	} while (0);

	return bRet;
}


bool BattleLayer::TouchBegan(Touch* touch, Event* event)
{
	auto card = (ReversibleCard*)this->getChildByTag(TAG_BATTLE);
	card->openCard();
	m_listener->setEnabled(false);
	return true;
}


void BattleLayer::TouchMoved(Touch* touch, Event* event)
{

}

void BattleLayer::TouchEnded(Touch* touch, Event* event)
{

}

void BattleLayer::battleGameStart(Ref* pData)
{

}


void BattleLayer::initBattleGame()
{
	auto card = ReversibleCard::create("battle1.png", "battle2.png", 4);
	card->setPosition(Point(m_winSize.width / 2, m_winSize.height / 2));
	card->setTag(TAG_BATTLE);
	this->addChild(card);
}

