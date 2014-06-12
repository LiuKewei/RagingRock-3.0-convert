#include "BattleLayer.h"

BattleLayer::BattleLayer()
	: m_targetCard(nullptr)
	, m_isOpening(false)
	, m_pileUpHeight(0.0f)
	, m_openingHeight(0.0f)
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
		m_pileUpHeight = m_winSize.height / 4;
		m_openingHeight = m_winSize.height / 3;
		m_openCardDuration = 0.4f;

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
	Point anchorPoint = targetCard->getAnchorPoint();
	Rect rect = Rect(pos.x - s.width *anchorPoint.x, pos.y - s.height*anchorPoint.y, s.width, s.height);

	//log("locationInNode ..  x = %f, y = %f", locationInNode.x, locationInNode.y);
	//log("pos ..  x = %f, y = %f", pos.x, pos.y);
	//log("anchorPoint ..  x = %f, y = %f", anchorPoint.x, anchorPoint.y);

	if (!m_isOpening && locationInNode.y > (m_openingHeight + 10) && rect.containsPoint(locationInNode) && m_currentCardGroup->size() == 3)
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
	increaseCards4Groups(4);
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
			m_battleCardGroups->front()->setPosition(Point(m_winSize.width / 2 + 200 * column, m_openingHeight));
			_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(), m_battleCardGroups->front());
			this->addChild(m_battleCardGroups->front(), Z_ORDER_MAX);
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
	int zorder = Z_ORDER_MAX - 1;
	while (column < 2 && iter != m_battleCardGroups->end())
	{
		(*iter)->setPosition(Point(m_winSize.width / 2 + 200 * column, m_pileUpHeight - 12 * row));
		(*iter)->verticalTilt(0.001f, -CARD_TILT_ANGLE);
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

void BattleLayer::increaseCards4Groups(int groupCnt)
{
	if (m_battleCardGroups != nullptr)
	{
		char cardname[12];
		ReversibleCard* card = nullptr;
		unsigned int cardType = 0;
		int countOfCards = 0;
		while (countOfCards < 3 * groupCnt)
		{
			cardType = MsgTypeForObserver::getRand(TYPE_BATTLE_LEAD_NORMAL, TYPE_BATTLE_LEAD_DEVIL);
			sprintf(cardname, "battle%d.png", cardType);
			card = ReversibleCard::create(cardname, "battle_card_bg.png", m_openCardDuration);
			card->setCardType(cardType);
			m_battleCardGroups->push_back(card);
			++countOfCards;
		}
	}
}

void BattleLayer::pileUpOneGroupCardsToTail()
{
	std::list<ReversibleCard*>::iterator iter = m_battleCardGroups->end();
	--iter; --iter;--iter;
	int column = -1;
	int zorder = Z_ORDER_MAX - 3;
	while (iter != m_battleCardGroups->end())
	{
		(*iter)->setPosition(Point(m_winSize.width / 2 + 200 * column, m_pileUpHeight - c_intervalCardHeight*3));
		(*iter)->setVisible(false);
		(*iter)->verticalTilt(0.001f, -CARD_TILT_ANGLE, 0.2f, CallFuncN::create(CC_CALLBACK_0(BattleLayer::showCard,this, (*iter))));
		//_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(), *iter);
		//(*iter)->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener->clone(),*iter);
		this->addChild(*iter, zorder);
		++iter;
		++column;
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
				card->verticalTilt(0.2f, CARD_TILT_ANGLE, MoveBy::create(0.2f, Point(0, 50)), MoveBy::create(0.2f, Point(0, m_openingHeight - card->getPositionY() - 50)));
				card->setLocalZOrder(card->getLocalZOrder() + 1);
				m_currentCardGroup->push_back(card);
				m_battleCardGroups->pop_front();
				++column;
			}
			for(auto& ele : *m_battleCardGroups)
			{
				ele->setLocalZOrder(ele->getLocalZOrder() + 1);
				ele->runAction(MoveBy::create(0.2f, Point(0, c_intervalCardHeight)));
			}
		}
		increaseCards4Groups(1);
		pileUpOneGroupCardsToTail();
	}
}



void BattleLayer::showCard(Node* sender)
{
	auto card = (ReversibleCard*)sender;
	if (!card->isVisible())
	{
		card->setVisible(true);
	}
}