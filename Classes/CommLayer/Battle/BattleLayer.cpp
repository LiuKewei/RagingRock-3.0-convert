#include "BattleLayer.h"

BattleLayer::BattleLayer()
: m_targetCard(nullptr)
, m_isOpening(false)
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

		this->schedule(schedule_selector(BattleLayer::waitingForOpened));

		m_listener = EventListenerTouchOneByOne::create();
		m_listener->onTouchBegan = CC_CALLBACK_2(BattleLayer::TouchBegan, this);
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

bool BattleLayer::TouchBegan(Touch* touch, Event* event)
{
	auto targetCard = static_cast<ReversibleCard*>(event->getCurrentTarget());
	Point locationInNode = touch->getLocation();
	Size s = targetCard->getReversibleCardSize();
	Point pos = targetCard->getPosition();
	Rect rect = Rect(pos.x - s.width / 2, pos.y - s.height / 2, s.width, s.height);

	//log("locationInNode ..  x = %f, y = %f", locationInNode.x, locationInNode.y);

	if (!m_isOpening && locationInNode.y > (m_winSize.height / 4 /*+ s.height + 36*/) && rect.containsPoint(locationInNode) && m_currentCardGroup->size() == 3)
	{
		int idx = 0;
		for (auto& ele : *m_currentCardGroup)
		{
			if (targetCard != ele)
			{
				ele->openCard(m_openCardDuration*idx);
				m_isOpening = true;
				++idx;
			}
			//++idx;  shouldn't add here
		}
		targetCard->openCard(m_openCardDuration*idx);
		m_targetCard = targetCard;
	}
	return false;
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
		while (cards < 3 * 4)
		{
			sprintf(cardname, "battle%d.png", MsgTypeForObserver::getRand(TYPE_BATTLE_LEAD_NORMAL, TYPE_BATTLE_LEAD_DEVIL));
			m_battleCardGroups->push_back(ReversibleCard::create(cardname, "battle_card_bg.png", m_openCardDuration));
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
		while (column < 2)
		{
			m_battleCardGroups->front()->setPosition(Point(m_winSize.width / 2 + 200 * column, m_winSize.height / 3));
			_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(), m_battleCardGroups->front());
			this->addChild(m_battleCardGroups->front(),Z_ORDER_MAX);
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
	int zorder = Z_ORDER_MAX-1;
	while (column < 2 && iter != m_battleCardGroups->end())
	{
		(*iter)->setPosition(Point(m_winSize.width / 2 + 200 * column, m_winSize.height / 4 - 12 * row));
		(*iter)->verticalTilt(0.001f, CARD_TILT_ANGLE);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(), *iter);
		this->addChild(*iter, zorder);
		++iter;
		++column;
		if (column == 2)
		{
			column = -1;
			++row;
			--zorder;
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
		m_targetCard = nullptr;
		m_isOpening = false;
		for (auto& ele : *m_currentCardGroup)
		{
			this->removeChild(ele);
		}
		m_currentCardGroup->clear();
		if (m_battleCardGroups->size() >= 3)
		{
			int column = -1;
			ReversibleCard* card = nullptr;
			while (column < 2 && m_currentCardGroup->size() < 3)
			{
				card = m_battleCardGroups->front();
				//card->verticalTilt(0.2f, -CARD_TILT_ANGLE, );
				card->verticalTilt(0.2f, -CARD_TILT_ANGLE, MoveBy::create(0.2f, Point(0, 50)), MoveBy::create(0.2f, Point(0, m_winSize.height / 3 - card->getPositionY() - 50)));
				m_currentCardGroup->push_back(card);
				m_battleCardGroups->pop_front();
				++column;
			}
		}
	}
}