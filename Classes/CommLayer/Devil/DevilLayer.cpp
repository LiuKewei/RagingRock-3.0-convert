#include "DevilLayer.h"


DevilLayer::DevilLayer()
: m_devil(NULL)
, m_fightingVal(0.0f)
, m_fightingMoved(0.0f)
, m_winSize(0, 0)
, m_progress(NULL)
, m_isdevilpos(false)
, m_fingerSparkle(NULL)
{
}

DevilLayer::~DevilLayer()
{
	m_fingers->clear();
	delete m_fingers;
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool DevilLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		m_winSize = Director::getInstance()->getWinSize();
		initDevil();
		initFingers();

		m_listener = EventListenerTouchOneByOne::create();
		m_listener->setSwallowTouches(true);
		m_listener->onTouchBegan = CC_CALLBACK_2(DevilLayer::TouchBegan, this);
		m_listener->onTouchMoved = CC_CALLBACK_2(DevilLayer::TouchMoved, this);
		m_listener->onTouchEnded = CC_CALLBACK_2(DevilLayer::TouchEnded, this);
		m_listener->setEnabled(true);
		_eventDispatcher->addEventListenerWithFixedPriority(m_listener, -1);

		this->scheduleUpdate();

		NotificationCenter::getInstance()->addObserver(
			this,
			callfuncO_selector(DevilLayer::devilFighting),
			MsgTypeForObserver::c_DevilFightingStart,
			NULL);

		NotificationCenter::getInstance()->addObserver(
			this,
			callfuncO_selector(DevilLayer::devilPosHandle),
			MsgTypeForObserver::c_DevilPosPush,
			NULL);



		bRet = true;
	} while (0);

	return bRet;
}

void DevilLayer::update(float dt)
{
	if (m_finger && m_finger->getRowedPath()->size() > 3){
		m_finger->setDeltaRemainder(dt * 60 * 1.2f);
		int pop = (int)(m_finger->getDeltaRemainder() + 0.5f);
		m_finger->setDeltaRemainder(m_finger->getDeltaRemainder() - pop);
		m_finger->pop(pop);
	}
}

bool DevilLayer::TouchBegan(Touch* touch, Event* event)
{
	if (m_progress == NULL)
	{
		return false;
	}
	Point location = touch->getLocation();
	m_fingerSparkle->setPosition(location);
	m_fingerSparkle->resetSystem();

	for (auto finger : *m_fingers)
	{
		if (finger->getRowedPath()->size() == 0)
		{
			m_finger = finger;
			m_finger->push(location);
			break;
		}
	}

	return true;
}

void DevilLayer::TouchMoved(Touch* touch, Event* event)
{
	Point start = touch->getStartLocation();

	Point afterStart = touch->getLocation();
	if (abs(afterStart.x - m_fightingMoved) > 7.0f)
	{
		m_fightingVal = 0.1f;
	}
	else
	{
		m_fightingVal = (m_progress != NULL && (m_progress->getPercentage() <= 80.0f)) ? -0.08f : -0.18f;
	}
	m_fightingMoved = afterStart.x;

	m_fingerSparkle->setPosition(afterStart);
	m_finger->push(afterStart);

}

void DevilLayer::TouchEnded(Touch* touch, Event* event)
{
	m_fightingVal = -0.1f;

	m_fingerSparkle->stopSystem();
	m_finger->setReset(true);
}

/*private function*/
void DevilLayer::initDevil()
{
	m_devil = Devil::create();
	m_devil->bindSpriteWithoutSize(Sprite::create("devil.png"));
	m_devil->setDevilMaxIndexInCurrent(0);
	m_devil->setDevilPosCnt(0);
	m_devil->setPosition(Point::ZERO);
	m_devil->setVisible(false);
	this->addChild(m_devil, Z_ORDER_ZERO);
	NotificationCenter::getInstance()->postNotification(MsgTypeForObserver::c_DevilPosUpdate, m_devil);
}

void DevilLayer::initFingers()
{
	m_finger = NULL;
	m_fingers = new Vector<Finger*>(c_fingers_capability);
	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("streak.png");
	for (int i = 0; i < c_fingers_capability; i++){
		Finger* finger = Finger::createWithMaximumPoint(c_finger_point_limit);
		finger->setAutoDim(false);
		finger->setTexture(texture);
		this->addChild(finger, Z_ORDER_MAX);
		m_fingers->pushBack(finger);
	}


	m_fingerSparkle = ParticleSystemQuad::create("fingerSparkle.plist");
	m_fingerSparkle->stopSystem();
	this->addChild(m_fingerSparkle, Z_ORDER_TWO);
}


void DevilLayer::devilFighting(Ref* pData)
{
	m_devil->setVisible(false);
	auto fightingProgressBg = Sprite::create("slider_bar.png");
	fightingProgressBg->setPosition(Point(150, 300));
	this->addChild(fightingProgressBg, Z_ORDER_ONE);

	auto* fightingProgress = Sprite::create("silder_progressBar.png");
	m_progress = ProgressTimer::create(fightingProgress);
	m_progress->setType(ProgressTimer::Type::BAR);
	m_progress->setPosition(Point(150, 300));
	m_progress->setMidpoint(Point(0, 0));
	m_progress->setBarChangeRate(Point(1, 0));
	m_progress->setPercentage(50);
	this->addChild(m_progress, Z_ORDER_ONE);
	this->schedule(schedule_selector(DevilLayer::updateFightingBar));
}


void DevilLayer::devilPosHandle(Ref* pData)
{
	unsigned int maxIdx = m_devil->getDevilMaxIndexInCurrent();
	//CCLOG("maxIdx %d", maxIdx);
	//CCLOG("m_devil->getDevilPosVec()->size() %d", m_devil->getDevilPosVec()->size());
	CC_ASSERT(maxIdx <= m_devil->getDevilPosVec()->size());
	if (m_devil->getDevilPosVec()->size() != 0)m_devil->setPosition(m_devil->getDevilPosVec()->at(MsgTypeForObserver::getRand(0, maxIdx - 1)));
	m_devil->setVisible(true);
}

void DevilLayer::destoryDevilLayer(Ref* pData)
{
	this->removeAllChildren();
}

void DevilLayer::updateFightingBar(float dt)
{
	float proPercet = m_progress->getPercentage();
	if (proPercet >= 100.0f || proPercet <= 0.0f)
	{
		this->removeAllChildren();
		m_progress = NULL;
		m_devil = NULL;
		m_listener->setEnabled(false);
		this->unschedule(schedule_selector(DevilLayer::updateFightingBar));
		this->unscheduleUpdate();
		NotificationCenter::getInstance()->postNotification(MsgTypeForObserver::c_DevilFightingStop, NULL);
	}
	else
	{
		m_progress->setPercentage(proPercet + m_fightingVal);
	}
}

