#include "BattleLayer.h"

BattleLayer::BattleLayer()
	: m_targetCard(nullptr)
	, m_emitter(nullptr)
	, m_isOpening(false)
	, m_pileUpHeight(0.0f)
	, m_openingHeight(0.0f)
	, m_hintInBackground(-1)
	, m_comboValue(1)
	, m_comboflag(false)
{
	m_battleCardGroups = new std::list<ReversibleCard*>();
	m_currentCardGroup = new std::vector<ReversibleCard*>();
}
BattleLayer::~BattleLayer()
{
	CC_SAFE_DELETE(m_battleCardGroups);
	CC_SAFE_DELETE(m_currentCardGroup);
	m_listener->release();
	m_emitter->release();
}
bool BattleLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		m_winSize = Director::getInstance()->getWinSize();
		m_pileUpHeight = m_winSize.height / 4 + 45;
		m_openingHeight = m_winSize.height / 3 + 5;
		m_openCardDuration = 0.4f;
		m_battleBase = Layer::create();
		m_battleBase->setVisible(false);
		this->addChild(m_battleBase);

		m_listener = EventListenerTouchOneByOne::create();
		m_listener->onTouchBegan = CC_CALLBACK_2(BattleLayer::TouchBegan, this);
		m_listener->setSwallowTouches(true);
		m_listener->retain();// 必须保留，要不然出了BattleLayer::init()方法，m_listener就会变成不可访问。

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
		targetCard->openCard(m_openCardDuration*idx, true);
		m_targetCard = targetCard;
	}
	return false;
}

void BattleLayer::battleGameStart(Ref* pData)
{
	increaseCards4Groups(4);
	pushCards();
	this->schedule(schedule_selector(BattleLayer::waitingForOpened));
	m_battleBase->setVisible(true);
}

void BattleLayer::battleGameStop()
{
	this->unschedule(schedule_selector(BattleLayer::waitingForOpened));
	NotificationCenter::getInstance()->postNotification(MsgTypeForObserver::c_BattleStop, NULL);
	m_battleBase->setVisible(false);

	m_emitter->setVisible(false);
}


void BattleLayer::initBattleGame()
{
	m_battleLayout = GUIReader::getInstance()->widgetFromJsonFile("BattleUI_1.ExportJson");
	m_battleBase->addChild(m_battleLayout);

	ArmatureDataManager::getInstance()->addArmatureFileInfo("BattleAnimation.ExportJson");
	auto paw = Armature::create("BattleAnimation");
	paw->setPosition(Point(472, 151));
	paw->setTag(TAG_BATTLE_PAW);
	m_battleBase->addChild(paw);

	initBgHint();
	initLabels();
	initCombo();
	initParticleFire();
}

// Particle
void BattleLayer::initParticleFire()
{
	m_emitter = ParticleFire::create();
	m_emitter->retain();
	m_emitter->setPosition(Point(m_winSize.width / 6, m_winSize.height / 6));
	m_battleBase->addChild(m_emitter, Z_ORDER_MAX);
	m_emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("fire.png"));
	m_emitter->setEmissionRate(800);
	m_emitter->setSpeed(120);
	m_emitter->setSpeedVar(0);
	m_emitter->setContentSize(m_winSize);
	m_emitter->setLifeVar(0.3f);
	m_emitter->setLife(0.2f);
	m_emitter->setDuration(-1);
	m_emitter->setStartSize(20.0f);
	m_emitter->setStartSizeVar(50.0f);
	m_emitter->setEndSize(ParticleSystem::START_SIZE_EQUAL_TO_END_SIZE);
	m_emitter->setBlendAdditive(true);
	m_emitter->setVisible(false);
}

void BattleLayer::initCombo()
{
	auto combo = ProgressTimer::create(Sprite::create("combo.png"));
	CC_ASSERT(combo != NULL);
	combo->setTag(TAG_BATTLE_COMBO);
	combo->setType(ProgressTimer::Type::BAR);
	combo->setPosition(Point(m_winSize.width / 4, m_winSize.height / 6));
	combo->setMidpoint(Point(0, 0));
	combo->setBarChangeRate(Point(1, 0));
	combo->setPercentage(m_comboValue/c_comboMax*100);
	m_battleBase->addChild(combo, Z_ORDER_MAX);

	this->schedule(schedule_selector(BattleLayer::updateCombo));
}

void BattleLayer::initLabels()
{
	TTFConfig config2("Marker Felt.ttf", 30, GlyphCollection::DYNAMIC, nullptr, true);

	m_devilHP = 30;
	m_devilHPLabel = Label::createWithTTF(config2, "30", TextHAlignment::LEFT);//创建显示 魔王血量 的label
	m_devilHPLabel->setPosition(Point(m_winSize.width / 2 + 250, m_winSize.height / 2 + 200));
	m_battleBase->addChild(m_devilHPLabel, 1);

	m_leadHP = 10;
	m_leadHPLabel = Label::createWithTTF(config2, "10", TextHAlignment::LEFT);//创建显示 主角血量 的label
	m_leadHPLabel->setPosition(Point(m_winSize.width / 2 + 250, m_winSize.height / 6));
	m_battleBase->addChild(m_leadHPLabel, 1);
}

void BattleLayer::initBgHint()
{
	auto bgHint1 = Sprite::create("Hint1.png");
	auto bgHint2 = Sprite::create("Hint2.png");
	//auto bgHint3 = Sprite::create("Hint3.png");
	bgHint1->setVisible(false);
	bgHint2->setVisible(false);
	//bgHint3->setVisible(false);
	bgHint1->setPosition(Point(m_winSize.width / 2, 8000));
	bgHint2->setPosition(Point(m_winSize.width / 2, 8000));
	//bgHint3->setPosition(Point(m_winSize.width / 2, 8000));
	bgHint1->setTag(TAG_BATTLE - TYPE_BATTLE_LEAD_NORMAL);
	bgHint2->setTag(TAG_BATTLE - TYPE_BATTLE_LEAD_MAGIC);
	//bgHint3->setTag(TAG_BATTLE - TYPE_BATTLE_LEAD_INVINCIBLE);
	m_battleBase->addChild(bgHint1, Z_ORDER_MAX);
	m_battleBase->addChild(bgHint2, Z_ORDER_MAX);
	//m_battleBase->addChild(bgHint3, Z_ORDER_MAX);
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
			m_battleBase->addChild(m_battleCardGroups->front(), Z_ORDER_MAX);
			m_currentCardGroup->push_back(m_battleCardGroups->front());
			m_battleCardGroups->pop_front();
			++column;
		}
	}
	pileUpCards();
}

void BattleLayer::pileUpCards()
{
	std::list<ReversibleCard*>::iterator iter = m_battleCardGroups->begin();
	int column = -1;
	int row = 1;
	int zorder = Z_ORDER_MAX - 1;
	while (column < 2 && iter != m_battleCardGroups->end())
	{
		(*iter)->setPosition(Point(m_winSize.width / 2 + 200 * column, m_pileUpHeight - c_intervalCardHeight * row));
		(*iter)->verticalTilt(0.001f, -CARD_TILT_ANGLE);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(), *iter);
		m_battleBase->addChild(*iter, zorder);
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
		char devilcard[12];
		sprintf(devilcard, "battle%d.png", TYPE_BATTLE_LEAD_DEVIL);
		ReversibleCard* card = nullptr;
		unsigned int cardType = 0;
		int countOfCards = 0;
		while (countOfCards < 3 * groupCnt)
		{
			cardType = MsgTypeForObserver::getRand(TYPE_BATTLE_LEAD_NORMAL, TYPE_BATTLE_LEAD_DEVIL);
			sprintf(cardname, "battle%d.png", cardType);
			card = ReversibleCard::create(cardname, devilcard, m_openCardDuration);
			card->setCardType(cardType);
			m_battleCardGroups->push_back(card);
			++countOfCards;
		}
	}
}

void BattleLayer::pileUpOneGroupCardsToTail()
{
	std::list<ReversibleCard*>::iterator iter = m_battleCardGroups->end();
	--iter; --iter; --iter;
	int column = -1;
	int zorder = Z_ORDER_MAX - 3;
	while (iter != m_battleCardGroups->end())
	{
		(*iter)->setPosition(Point(m_winSize.width / 2 + 200 * column, m_pileUpHeight - c_intervalCardHeight * 3));
		(*iter)->setVisible(false);
		(*iter)->verticalTilt(0.2f, -CARD_TILT_ANGLE, CallFuncN::create(CC_CALLBACK_0(BattleLayer::showCard, this, (*iter))));
		(*iter)->setLocalZOrder(zorder);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(), *iter);
		m_battleBase->addChild(*iter);
		++iter;
		++column;
	}
}

void BattleLayer::updateCombo(float dt)
{
	auto combo = (ProgressTimer*)m_battleBase->getChildByTag(TAG_BATTLE_COMBO);
	float combovalue = combo->getPercentage();
	if (combovalue - ((float)m_comboValue / (float)c_comboMax*100.0f ) < 0)
	{
		combo->setPercentage(combo->getPercentage() + 5.0f);
	}
	else if (combovalue - ((float)m_comboValue / (float)c_comboMax*100.0f) > 0)
	{
		combo->setPercentage(combo->getPercentage() - 5.0f);
	}

	
}

void BattleLayer::updateBgHint()
{
	if (m_currentCardGroup->size() == 3)
	{
		m_hintVec.clear();
		for (auto& ele : *m_currentCardGroup)
		{
			if (ele->getCardType() == TYPE_BATTLE_LEAD_DEVIL)
			{
				m_hintInBackground = -1;
				m_hintVec.clear();
				break;
			}
			m_hintVec.push_back(ele->getCardType());
		}
		m_hintVec.push_back(-1);
		m_hintInBackground = m_hintVec.size() == 4 ? m_hintVec.at(MsgTypeForObserver::getRand(0, m_hintVec.size() - 1)) : -1;
		this->showBgHint();
	}
}

void BattleLayer::waitingForOpened(float dt)
{
	if (m_targetCard != nullptr && m_targetCard->isOpened())
	{
		battle();
		for (auto& ele : *m_currentCardGroup)
		{
			m_battleBase->removeChild(ele);
		}
		m_currentCardGroup->clear();
		if (m_battleCardGroups->size() >= 3)
		{
			int column = -1;
			ReversibleCard* card = nullptr;
			while (column < 2 && m_currentCardGroup->size() < 3) 
			{
				//当前牌翻完以后，把扑在底下的第一排翻转回来之后拿上来
				card = m_battleCardGroups->front();
				card->verticalTilt(0.2f, CARD_TILT_ANGLE, MoveBy::create(0.2f, Point(0, 40)), MoveBy::create(0.2f, Point(0, m_openingHeight - card->getPositionY() - 40)));
				card->setLocalZOrder(card->getLocalZOrder() + 1);
				m_currentCardGroup->push_back(card);
				m_battleCardGroups->pop_front();
				++column;
			}
			for (auto& ele : *m_battleCardGroups)
			{
				//扑在底下的第二排第三排依次上移
				ele->setLocalZOrder(ele->getLocalZOrder() + 1);
				ele->runAction(MoveBy::create(0.2f, Point(0, c_intervalCardHeight)));
			}
		}
		//增加一排，放置牌堆最底下
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
		m_isOpening = false;
	}
}

void BattleLayer::removeTopGroup(Node* sender)
{
	auto card = (ReversibleCard*)sender;
	m_battleBase->removeChild(card);
}

void BattleLayer::showBgHint()
{
	switch (m_hintInBackground)
	{
	case TYPE_BATTLE_LEAD_NORMAL:
		m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_NORMAL)->setVisible(true);
		m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_MAGIC)->setVisible(false);
		//m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_INVINCIBLE)->setVisible(false);
		break;
	case TYPE_BATTLE_LEAD_MAGIC:
		m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_NORMAL)->setVisible(false);
		m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_MAGIC)->setVisible(true);
		//m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_INVINCIBLE)->setVisible(false);
		break;
		//case TYPE_BATTLE_LEAD_INVINCIBLE:
		//	m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_NORMAL)->setVisible(false);
		//	m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_MAGIC)->setVisible(false);
		//	m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_INVINCIBLE)->setVisible(true);
		//	break;
	default:
		//所有背景提示全部关闭
		m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_NORMAL)->setVisible(false);
		m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_MAGIC)->setVisible(false);
		//m_battleBase->getChildByTag(TAG_BATTLE - TYPE_BATTLE_LEAD_INVINCIBLE)->setVisible(false);
		break;
	}
}

void BattleLayer::battle()
{
	if (m_targetCard == nullptr) return;
	switch (m_targetCard->getCardType())
	{
	case TYPE_BATTLE_LEAD_NORMAL:
		if (m_comboflag && m_comboValue == 0)
		{
			this->attack(1);
			m_comboValue = c_comboMax;
			//燃烧效果消失
			m_emitter->setVisible(false);
		}
		else
		{
			this->attack(m_comboValue);
			m_comboValue = m_comboValue == c_comboMax ? c_comboMax : m_comboValue + 1;
		}
		m_comboflag = false;
		break;
	case TYPE_BATTLE_LEAD_MAGIC:
		this->burnCombo();
		this->attack(2);
		break;
		//case TYPE_BATTLE_LEAD_INVINCIBLE:
		//	this->attack(5);
		//	break;
		//case TYPE_BATTLE_LEAD_PET:
		//	this->injuredOrCure(1);
		//	break;
	case TYPE_BATTLE_LEAD_DEVIL:
		this->burnCombo();
		this->injuredOrCure(-1);
		break;
	default:
		break;
	}

	//auto combo = (ProgressTimer*)m_battleBase->getChildByTag(TAG_BATTLE_COMBO);
	//combo->setPercentage( (float)m_comboValue / (float)c_comboMax*100.0f );


	m_targetCard = nullptr;
	if (m_leadHP <= 0 || m_devilHP <= 0)
	{
		battleGameStop();

		this->unschedule(schedule_selector(BattleLayer::updateCombo));
		return;
	}
	updateBgHint();
}
void BattleLayer::attack(int cnt)
{
	m_devilHP = m_devilHP - cnt;
	char tmp[5];
	sprintf(tmp, "%d", m_devilHP);
	m_devilHPLabel->setString(tmp);
}

void BattleLayer::injuredOrCure(int cnt)
{
	if (cnt < 0)
	{
		auto paw = (Armature*)m_battleBase->getChildByTag(TAG_BATTLE_PAW);
		paw->getAnimation()->play("Animation1");
	}
	m_leadHP = m_leadHP + cnt;
	char tmp[5];
	sprintf(tmp, "%d", m_leadHP);
	m_leadHPLabel->setString(tmp);
}

void BattleLayer::burnCombo()
{
	if (m_comboflag)
	{
		//burn
		m_emitter->setVisible(true);
		m_comboValue = 0;
	}
	else
	{
		m_comboflag = true;
		m_comboValue = 1;
	}
}