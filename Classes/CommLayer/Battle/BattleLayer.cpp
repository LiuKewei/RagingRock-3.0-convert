#include "BattleLayer.h"

BattleLayer::BattleLayer()
	: m_targetCard(nullptr)
{
	m_battleCardGroups = new std::list<ReversibleCard*>();
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
			auto targetCard = static_cast<ReversibleCard*>(event->getCurrentTarget());
			Point locationInNode = touch->getLocation();
			Size s = targetCard->getReversibleCardSize();
			Point pos = targetCard->getPosition();
			Rect rect = Rect(pos.x-s.width/2, pos.y - s.height/2, s.width, s.height);


			if (rect.containsPoint(locationInNode) && m_currentCardGroup->size() == 3)
			{
				int idx = 0;
				for (auto& ele : *m_currentCardGroup)
				{
					if (targetCard != ele)
					{
						ele->openCard(m_openCardDuration*idx);
						++idx;
					}
					//++idx;  shouldn't add here
				}
				targetCard->openCard(m_openCardDuration*idx);
				m_targetCard = targetCard;
				this->schedule(schedule_selector(BattleLayer::waitingForOpened));
			}
			//m_listener->setEnabled(false);
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
		while (cards < 3*4)
		{
			sprintf(cardname, "battle%d.png", MsgTypeForObserver::getRand(TYPE_BATTLE_LEAD_NORMAL,TYPE_BATTLE_LEAD_DEVIL));
			m_battleCardGroups->push_back( ReversibleCard::create(cardname, "battle_card_bg.png", m_openCardDuration) );
			++cards;
		}
	}

	pushCards();
	pileUpCards();
}


void BattleLayer::pushCards()
{
	if (m_battleCardGroups->size() >= 3)
	{
		int column = -1;
		while(column < 2)
		{
			m_battleCardGroups->front()->setPosition(Point(m_winSize.width/2 + 200*column,m_winSize.height/2));
			_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(),m_battleCardGroups->front());
			this->addChild(m_battleCardGroups->front());
			m_currentCardGroup->push_back(m_battleCardGroups->front());
			m_battleCardGroups->pop_front();
			++column;
		}
	}
}

void BattleLayer::pileUpCards()
{
	std::list<ReversibleCard*>::iterator iter = m_battleCardGroups->begin();
	int column = -1;
	int row = 1;
	while(column < 2 && iter != m_battleCardGroups->end())
	{
		(*iter)->setPosition(Point(m_winSize.width/2 + 200*column,m_winSize.height/4 + 12*row));
		(*iter)->verticalTilt(55);
		
		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(),*iter);
		this->addChild(*iter);
		++iter;
		++column;
		if ( column == 2 )
		{
			column = -1;
			++row;
		}
	}
}

void BattleLayer::update(float dt)
{
}

void BattleLayer::waitingForOpened(float dt)
{
	if (m_targetCard != nullptr && m_targetCard->isOpened())
	{
		for (auto& ele : *m_currentCardGroup)
		{
			this->removeChild(ele);
		}
		m_currentCardGroup->clear();
		if (m_battleCardGroups->size() >= 3)
		{
			int column = -1;
			while(column < 2)
			{
				m_battleCardGroups->front()->setPosition(Point(m_winSize.width/2 + 200*column,m_winSize.height/2));
				m_currentCardGroup->push_back(m_battleCardGroups->front());
				m_battleCardGroups->pop_front();
				++column;
			}
		}
		this->unschedule(schedule_selector(BattleLayer::waitingForOpened));
	}
}