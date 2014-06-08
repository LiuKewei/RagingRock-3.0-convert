#include "BattleLayer.h"

BattleLayer::BattleLayer()
{
	m_battleCardGroups = new std::queue<ReversibleCard*>();
	m_currentCardGroup = new std::vector<ReversibleCard*>();
}
BattleLayer::~BattleLayer()
{
	CC_SAFE_DELETE(m_battleCardGroups);
	CC_SAFE_DELETE(m_currentCardGroup);
}
bool BattleLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		m_winSize = Director::getInstance()->getWinSize();

		m_listener = EventListenerTouchOneByOne::create();
		m_listener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<ReversibleCard*>(event->getCurrentTarget());
			Point locationInNode = touch->getLocation();
			Size s = target->getReversibleCardSize();
			Point pos = target->getPosition();
			Rect rect = Rect(pos.x-s.width/2, pos.y - s.height/2, s.width, s.height);


			if (rect.containsPoint(locationInNode) && m_currentCardGroup->size() == 3)
			{
				int idx = 0;
				for (auto& ele : *m_currentCardGroup)
				{
					if (target != ele)
					{
						ele->openCard(m_openCardDuration*idx);
						++idx;
					}
					//++idx;  shouldn't add here
				}
				target->openCard(m_openCardDuration*idx);

			}
			else
			{
				//把倒下的图片拉上来
			}

			return false;
		};
		m_listener->setSwallowTouches(true);

		NotificationCenter::getInstance()->addObserver(
			this,
			callfuncO_selector(BattleLayer::battleGameStart),
			MsgTypeForObserver::c_BattleStart,
			NULL);

		initBattleGame();

		bRet = true;
	} while (0);

	return bRet;
}

void BattleLayer::battleGameStart(Ref* pData)
{

}


void BattleLayer::initBattleGame()
{
	m_openCardDuration = 0.4f;
	char cardname[12];
	if (m_battleCardGroups != nullptr)
	{
		int cards = 0;
		while (cards < 3*6)
		{
			sprintf(cardname, "battle%d.png", MsgTypeForObserver::getRand(TYPE_BATTLE_LEAD_NORMAL,TYPE_BATTLE_LEAD_DEVIL));
			m_battleCardGroups->push( ReversibleCard::create(cardname, "battle_card_bg.png", m_openCardDuration) );
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
			_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(),m_battleCardGroups->front());
			this->addChild(m_battleCardGroups->front());
			m_currentCardGroup->push_back(m_battleCardGroups->front());
			m_battleCardGroups->pop();
			++i;
		}
	}
}


void BattleLayer::update(float dt)
{
}