#include "BattleLayer.h"

BattleLayer::BattleLayer()
{
	m_battleCardGroups = new std::queue<ReversibleCard*>();
}
BattleLayer::~BattleLayer()
{
	CC_SAFE_DELETE(m_battleCardGroups);
}
bool BattleLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		m_winSize = Director::getInstance()->getWinSize();

		initBattleGame();

		NotificationCenter::getInstance()->addObserver(
			this,
			callfuncO_selector(BattleLayer::battleGameStart),
			MsgTypeForObserver::c_BattleStart,
			NULL);


		bRet = true;
	} while (0);

	return bRet;
}

void BattleLayer::battleGameStart(Ref* pData)
{

}


void BattleLayer::initBattleGame()
{
	char cardname[12];
	if (m_battleCardGroups != nullptr)
	{
		int cards = 0;
		while (cards < 3*6)
		{
			sprintf(cardname, "battle%d.png", MsgTypeForObserver::getRand(TYPE_BATTLE_LEAD_NORMAL,TYPE_BATTLE_LEAD_DEVIL));
			m_battleCardGroups->push( ReversibleCard::create(cardname, "battle_card_bg.png", 3) );
			++cards;
		}
	}

	pushCards();
}


void BattleLayer::pushCards()
{

	if (m_battleCardGroups->size() >= 3)
	{
		int i = -1;
		while(i < 2)
		{
			m_battleCardGroups->front()->setPosition(Point(m_winSize.width/2 + 200*i,m_winSize.height/2));
			this->addChild(m_battleCardGroups->front());
			m_battleCardGroups->pop();
			++i;
		}
	}
}
